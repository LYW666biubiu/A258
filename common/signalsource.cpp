#define LOG_TAG    "SignalSource"

#include "signalsource.h"

SignalSource::SignalSource(uint vdchn) :
    vdchn_(vdchn)
{
    rtsp_sp_        = make_shared<player_rtsp>();
//    pte_sp_         = make_shared<player_pte>();
    multicast_sp_   = make_shared<player_multicast>();
    rtp_sp_         = make_shared<player_rtp>();

    b_video_     = false;
    b_audio_     = false;
    b_mute_      = true;
    b_transmit_  = false;
    b_transloop_ = false;
    b_close_     = true;
    cal_time_    = 0.0;
    last_pts_    = -1;
    vpackage_sn_ = 0;
    apackage_sn_ = 0;
    local_tick_  = 0;
    i_frame_len_ = 0;
    frame_rate_count_   = 0;

    for(int i = 0; i < FPS_POOL_CNT; i++)
        b_fps_pool_[i] = false;
    memset(&st_audioinfo_, 0, sizeof(st_audioinfo_));
    memset(&st_transmit_, 0, sizeof(st_transmit_));
    st_transmit_.length = MULTICAST_UDP_LEN;
    st_transmit_.sleeptime = 0;//(us)

    aac_dec_.get_version();
    aac_dec_.open();
}

SignalSource::~SignalSource()
{
    b_video_ = false;
    b_audio_ = false;

    auto lastvochns = tovochn_;
    for(auto iter = lastvochns.begin(); iter != lastvochns.end(); iter++){
        this->CloseWindow(iter->first);
    }

    if(this->b_transloop_){
        this->CloseNetStream("multicast");
        this->b_transloop_ = false;
    }

    if(!b_close_){
        this->CloseNetStream();
    }

    if(this->b_transmit_){
        this->DeleteTransmit();
    }

    aac_dec_.close();

    log_d("~SignalSource: VdChn[%d], Url[%s]", vdchn_, url_.c_str());
}

/** NOTE: H264 Head
 * sps=0x07 pps=0x08 (自定义信息)sei=0x06 I=0x05 P/B=0x01
 *
 * 其中0x67的二进制码为：
 * 0110 0111
 * 4-8为00111，转为十进制7，参考第二幅图：7 对应序列参数集SPS
 *
 * 其中0x68的二进制码为：
 * 0110 1000
 * 4-8为01000，转为十进制8，参考第二幅图：8 对应图像参数集PPS
 *
 * 其中0x65的二进制码为：
 * 0110 0101
 * 4-8为00101，转为十进制5，参考第二幅图：5对应IDR图像中的片(I帧)
 * 所以判断是否为I帧的算法为： （NALU类型  & 0001  1111） = 5   即   NALU类型  & 31 = 5
 * 比如0x65 & 31 = 5
 **/
void SignalSource::on_video(int video_type, int video_width, int video_heigth, void* data, unsigned int len, unsigned long long pts, unsigned wnd )
{
    if(elog_get_filter_lvl() == ELOG_LVL_VERBOSE &&
       !strncmp(elog_get_filter_kw(), "rtsp", 4)){
        printf("\r<rtsp> [wnd:%d] video_type: %d, video_width: %d, video_heigth: %d, len: %d, pts: %lld [%#x %#x %#x %#x %#x]", wnd, video_type, video_width, video_heigth, len, pts, ((char*)data)[0], ((char*)data)[1], ((char*)data)[2], ((char*)data)[3], ((char*)data)[4]);
        fflush(stdout);
    }
    if(!b_video_){
        return ;
    }
#if 0
    if(fd_ == NULL)
        fd_ = fopen("1080P.h264", "w+");
    if(fd_ != NULL)
        fwrite(data, 1, len, fd_);
#endif
    if(wnd == UINT_MAX) {
        return (void)1;
    }

    //NOTE: 切换解码协议
    if(video_type != video_type_){
        video_type_ = video_type;
        if(wnd < (HI_U32)gVdecChnMax){
            if(video_type_ == C_VIDEO_H264){
                log_d("[%d]on_video 264 = video_type: %d", wnd, video_type_);
                Singleton<HimppMaster>::getInstance()->VdecProtocolSwitch(wnd, PT_H264);
            }
            else if(video_type_ == C_VIDEO_H265){
                log_d("[%d]on_video 265 = video_type: %d", wnd, video_type_);
                Singleton<HimppMaster>::getInstance()->VdecProtocolSwitch(wnd, PT_H265);
            }
        }
    }

    int vd = wnd;
    while(vd >= VD_CHN_MAX)
        vd -= VD_CHN_MAX;

    //统计帧率
    if(last_pts_ != pts /*&& last_pts != 0*/){
        if((pts > last_pts_)){
            frame_rate_count_ ++;
            cal_time_ += (pts - last_pts_);

            if( (b_firstfrm_ && frame_rate_count_ == 3) ||
                frame_rate_count_ == 60){

                if(cal_time_ != 0)
                    frame_rate_ = (int)((double)60 * 16.67 * (double)frame_rate_count_ / cal_time_ + 0.5);
                else
                    frame_rate_ = 60;

                if(frame_rate_ < 10)
                    frame_rate_ = 10;
                else if(frame_rate_ > 60)
                    frame_rate_ = 60;

                b_firstfrm_ = false;
                frame_rate_count_ = 0;
                cal_time_ = 0.0;

#ifdef NEW_CALCULATION
                /** NOTE:
                 * 做一个关于帧率的buf池子,池子深度为60,
                 * 根据当前帧率计算出在0~59之中哪些数(即在池子的对应的位置上)应该需要打入帧的pts号中.
                 * 比如25fps时,需要在0,2,5,7,9,11.....,
                 * 那么当全局的glFrameID(0,1,2,3,4,5,6,7.....)来时,
                 * 会将其均匀到计算出来的0,2,5,7,9,11....中.控制发包均匀
                 **/
                for(int i = 0; i < FPS_POOL_CNT; i++)
                    b_fps_pool_[i] = false;

                double frame_step = (FPS_POOL_CNT * 1.0) / frame_rate_;
                double drop_tick = 0.0;
                int fps_id = 0;
                for(int i = 0; i < frame_rate_; i++){
                    b_fps_pool_[fps_id] = true;
                    drop_tick += frame_step;
                    fps_id = min(59, (int)(drop_tick + 0.5));
                }
#endif // NEW_CALCULATION
            }
        }
    }

    //检测 h264 的关键帧
    if(video_type_ == C_VIDEO_H264){
        if((((char*)data)[4] & 0x1F) == 0x05 ||
           (((char*)data)[4] & 0x1F) == 0x06 ||
           (((char*)data)[4] & 0x1F) == 0x07 ||
           (((char*)data)[4] & 0x1F) == 0x08 ){
            is_i_frame_ = true;
        }
        else{
            is_i_frame_ = false;
        }
    }

    if(len < 100 && (i_frame_len_ + len) < PACK_LEN &&
       (((char*)data)[4] != 0x61 && ((char*)data)[4] != 0x2)){
        memcpy(&i_frame_data_[i_frame_len_], data, len);
        i_frame_len_ += len;
    }
    else{
        //根据glFrameID计算lock_tick的值
#ifndef NEW_CALCULATION
        if(local_tick != (gFrameID + 59) % 60){
            local_tick = gFrameID;//(gFrameID + 59) % 60;
        }
        else{
            local_tick =  (local_tick + 60 / frame_rate_) % 60;
        }
#else
        // NOTE: 在池子中查找距离 当前的glFrameID 最近的一个可用id
        int closest = 0;
        for(int i = 0; i < FPS_POOL_CNT / 2; i++){
            //NOTE: 往后查找 i 位
            if(b_fps_pool_[(gFrameID + i) % FPS_POOL_CNT]){

                closest = (gFrameID + i) % FPS_POOL_CNT;
                break;
            }

            //NOTE: 往前查找 i 位
            if(b_fps_pool_[(gFrameID + FPS_POOL_CNT - i) % FPS_POOL_CNT]){

                closest = (gFrameID + FPS_POOL_CNT - i) % FPS_POOL_CNT;
                break;
            }
        }

        // NOTE: 在池子中查找 上次使用的id 的下一个可用id
        int lastnext = 0;
        for(int i = 1; i < FPS_POOL_CNT; i++){
            if(b_fps_pool_[(i + local_tick_) % FPS_POOL_CNT]){
                lastnext = (i + local_tick_) % FPS_POOL_CNT;
                break;
            }
        }

        if(last_gframeID_!= gFrameID){
            int date1 = abs(lastnext - closest);
            int date2 = 60 - date1;
            if(date1 > (2 * 60 / frame_rate_) &&
               date2 > (2 * 60 / frame_rate_)){
                //log_d("---------------------------c: %d, l: %d", closest, lastnext);
                lastnext = closest;
            }
        }

        local_tick_ = lastnext;

        last_gframeID_ = gFrameID;
#endif // NEW_CALCULATION

        if(gbEnKvmMode || (!b_transmit_)){
            gStreamFrameRate[vd] = frame_rate_;

            //NOTE: RTSP连接下的优先级判断
            double cur_pic_num = 0;
            for(int i = 0; i < min(gVdecChnMax, VD_CHN_MAX); i ++){
                cur_pic_num += gdVdecPicNum[gSignalPri[i]] * gStreamFrameRate[gSignalPri[i]]; //计算所有的需要Get的Pic
                if(gSignalPri[i] == vd){
                    break;
                }
            }
            if((cur_pic_num < VDEC_TOTAL_FPS || (vdec_win_num[vd] > 0 && is_i_frame_))){
                if(i_frame_len_ > 0){
#ifdef  __VDEC_SEPARATE__
                    DecodeVideo(i_frame_data_, i_frame_len_, local_tick_ | (gStreamFrameRate[vd] << 16), vd, is_i_frame_);
#else
                    DecodeVideo(i_frame_data_, i_frame_len_, local_tick_ | (gStreamFrameRate[vd] << 16), vd);
#endif
                    DecodeVideo(data, len, local_tick_ | (gStreamFrameRate[vd] << 16), vd);
                }
                else
#ifdef  __VDEC_SEPARATE__
                    DecodeVideo(data, len, local_tick_ | (gStreamFrameRate[vd] << 16), vd, is_i_frame_);
#else
                    DecodeVideo(data, len, local_tick_ | (gStreamFrameRate[vd] << 16), vd);
#endif
            }
        }
        else{
            mutex_.lock();
            TransmitVideo(video_type, video_width, video_heigth, data, len, local_tick_, frame_rate_, vpackage_sn_++);
            mutex_.unlock();
        }
        i_frame_len_ = 0;
    }

    last_pts_ = pts;
}

void SignalSource::on_loopvideo(int video_type, int video_width, int video_heigth, void* data, unsigned int len, unsigned int pts, unsigned wnd, bool iframe)
{
    //log_d("<multicast> [wnd:%d] video_type: %d, video_width: %d, video_heigth: %d, len: %d, pts: %d [%#x %#x %#x %#x %#x]", wnd, video_type, video_width, video_heigth, len, pts, ((char*)data)[0], ((char*)data)[1], ((char*)data)[2], ((char*)data)[3], ((char*)data)[4]);

    if(!b_video_){
        return ;
    }
    if(wnd == UINT_MAX) {
        return (void)1;
    }

    //NOTE: 切换解码协议
    if(video_type != video_type_){
        video_type_ = video_type;
        if(wnd < (HI_U32)gVdecChnMax){
            if(video_type_ == C_VIDEO_H264){
                log_d("[%d]loopvideo 264 = video_type: %d",wnd, video_type_);
                Singleton<HimppMaster>::getInstance()->VdecProtocolSwitch(wnd, PT_H264);
            }
            else if(video_type_ == C_VIDEO_H265){
                log_d("[%d]loopvideo 265 = video_type: %d",wnd, video_type_);
                Singleton<HimppMaster>::getInstance()->VdecProtocolSwitch(wnd, PT_H265);
            }
        }
    }

    if(b_transmit_ && 0) {
        mutex_.lock();
        TransmitVideo(video_type, video_width, video_heigth, data, len, local_tick_, frame_rate_, vpackage_sn_++);
        mutex_.unlock();
    }
    else {
#ifdef _VIDEO_CACHE_
        VIDEO_CACHE_S(data, len, pts, wnd);
#else
#ifdef  __VDEC_SEPARATE__
        DecodeVideo(data, len, pts, wnd, iframe);
#else
        DecodeVideo(data, len, pts, wnd);
#endif
#endif // _VIDEO_CACHE_
    }


}

void SignalSource::on_audio( int audio_type, int audio_sample, int audio_channel, void *data, unsigned int len, unsigned long long pts, unsigned wnd )
{
    //log_d("<on_audio> [wnd:%d] audio_type: %d, audio_sample: %d, audio_channel: %d, len: %d, pts:%lld", wnd, audio_type, audio_sample, audio_channel, len, pts);

    st_audioinfo_.bstatus   = HI_TRUE;
    st_audioinfo_.type      = audio_type;
    st_audioinfo_.sample    = audio_sample;
    st_audioinfo_.channel   = audio_channel;

    if(b_audio_){
        DecodeAudio(data, len, pts, audio_type, audio_sample, audio_channel);

        // 取消静音
        if(b_mute_){
            Singleton<HimppMaster>::getInstance()->SetAudioMute(HI_FALSE, HI_TRUE);
            b_mute_ = false;
        }
    }
    else if(b_transmit_){
        TransmitAudio(audio_type, audio_sample, audio_channel, data, len, pts, wnd, apackage_sn_++);
    }
}

void SignalSource::on_loopaudio(int audio_type, int audio_sample, int audio_channel, void *data, unsigned int len, unsigned long long pts, unsigned wnd)
{
    //log_d("<loop audio> [wnd:%d] audio_type: %d, audio_sample: %d, audio_channel: %d, len: %d, pts:%lld", wnd, audio_type, audio_sample, audio_channel, len, pts);

    if(b_audio_){
        DecodeAudio(data, len, pts, audio_type, audio_sample, audio_channel);

        // 取消静音
        if(b_mute_){
            Singleton<HimppMaster>::getInstance()->SetAudioMute(HI_FALSE, HI_TRUE);
            b_mute_ = false;
        }
    }
}

bool SignalSource::PlayNetStream(const char *url, string protocol)
{
#ifdef  __HI3531A__
    if(protocol != "audio") {
        HI_S32 s32Ret;
        if( (s32Ret = HI_MPI_VDEC_StartRecvStream(vdchn_)) != HI_SUCCESS ){
            log_e("HI_MPI_VDEC_StartRecvStream[%d] Failed with 0x%x!",vdchn_, s32Ret);
        }
    }
#endif
    //TODO: 最好能加上一个url的合法性检测，防止格式非法导致后续出现问题
    if(protocol == ""){
        protocol = protocol_.c_str();
    }

    if(protocol == "rtsp"){
        rtsp_sp_->open(url, vdchn_, this);
    }
    else if(protocol == "pte"){
//        pte_sp_->open(url, vdchn_, this);
    }
    else if(protocol == "multicast"){
        if(multicast_sp_->open(url, vdchn_, this, MULTICAST_UDP_LEN, player_multicast::UDP_VIDEO) < 0)
            log_e("multicast url Invalid format.");
    }
    else if(protocol == "audio"){
        if(multicast_sp_->open(url, vdchn_, this, MULTICAST_UDP_LEN, player_multicast::UDP_AUDIO) < 0)
            log_e("multicast url Invalid format.");
    }
    else if(protocol == "rtp"){
        if(rtp_sp_->open(url, vdchn_, this) < 0)
            log_e("rtp url Invalid format.");
    }
    else{
        log_e("protocol is Invalid format.");
        return false;
    }
    log_d("PLAY> protocol: %s, url: %s", protocol.c_str(), url);

#ifdef _VIDEO_CACHE_
    /* 如果开启缓存机制（加强同步）则回启动该线程，目前处于测试版 */
    pthread_t tid;
    pthread_attr_t attr;
    struct sched_param  param;
    pthread_attr_init(&attr);
    param.sched_priority = 80; //优先级：1～99
    //pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);//要使优先级其作用必须要有这句话.表示使用在schedpolicy和schedparam属性中显式设置的调度策略和参数
    pthread_attr_setschedparam(&attr, &param);
    pthread_attr_setschedpolicy(&attr, SCHED_RR);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED); //分离模式，无需的等待（pthread_join）之后才释放占用资源，而是返回后自动释放占用的资源
    pthread_create(&tid, &attr, VideoCacheDecodeProc, (void*)this);
    pthread_attr_destroy(&attr);
#endif // _VIDEO_CACHE_


    b_close_ = false;
    b_firstfrm_ = true;
    stAoAdjust_.start.tv_sec  = 0;
    stAoAdjust_.start.tv_usec = 0;

    return true;
}

bool SignalSource::CloseNetStream(string protocol)
{
    // NOTE: 如果是关闭 [指定协议] 的流，则不会将该信号源的状态置为关闭，需要进一步另一个可能存在的协议流
    if(protocol == ""){
        protocol = protocol_.c_str();

        url_.clear();
        b_video_ = false;
        b_audio_ = false;
        b_close_ = true;
#ifdef _VIDEO_CACHE_
        gbPlay = HI_FALSE;
#endif // _VIDEO_CACHE_

#if 0
        /*
         * 当发完所有码流后，把 bEndOfStream 置为 1，表示码流文件结束，这时解码器会
         * 解完发送下来的所有码流并输出所有图像。如果发完所有码流后把 bEndOfStream
         * 置为 0，解码器内部可能残余大于等于一帧的图像未解码输出，因为解码器必须等
         * 到下一帧码流到来才能知道当前帧已经结束，送入解码。
        */
        VDEC_STREAM_S stStream;
        memset(&stStream, 0, sizeof(VDEC_STREAM_S) );
        stStream.bEndOfStream = HI_TRUE;
        HI_MPI_VDEC_SendStream(vdchn_, &stStream, -1);
        HI_MPI_VDEC_StopRecvStream(vdchn_); //停止接收
        while( 1 ){
            VIDEO_FRAME_INFO_S info;
            if( HI_SUCCESS != HI_MPI_VDEC_GetImage( vdchn_, &info, 5) ) break;
            HI_MPI_VDEC_ReleaseImage(vdchn_, &info);
        }
#endif
    }

    if(protocol == "rtsp"){
        rtsp_sp_->close();
    }
    else if (protocol == "pte"){
//        pte_sp_->close();
    }
    else if (protocol == "rtp"){
        rtp_sp_->close_();
    }
    else if(protocol == "multicast" || protocol == "audio"){
        multicast_sp_->close_();
    }

#ifdef  __HI3531A__
    if(protocol != "audio") {
        HI_S32 s32Ret;
        if( (s32Ret = HI_MPI_VDEC_StopRecvStream(vdchn_)) != HI_SUCCESS ){
            log_e("HI_MPI_VDEC_StopRecvStream[%d] Failed with 0x%x!",vdchn_, s32Ret);
        }
        if( (s32Ret = HI_MPI_VDEC_ResetChn(vdchn_)) != HI_SUCCESS ){
            log_e("HI_MPI_VDEC_ResetChn[%d] Failed with 0x%x!",vdchn_, s32Ret);
        }
    }

#endif

    gAoData = 0;
    if(audiomixchn_ >= 0 && audiomixchn_ < AUDIOMIX_MAXCHN){
        extern AUDIO_MIX_S g_stAudioMix[AUDIOMIX_MAXCHN];
        memset(&g_stAudioMix[audiomixchn_], 0, sizeof(AUDIO_MIX_S));
    }

    return true;
}

/*
 * 功能： 设置vo窗口的size或crop参数，并将其打开
 * 输入： HI_U32 u32VoChn： 通道号
 *       HI_S32 s32X： 左上角的x坐标
 *       HI_S32 s32Y： 左上角的y坐标
 *       HI_U32 u32Width： 宽度
 *       HI_U32 u32Height： 高度
 *       HI_U32 u32Priority： 图层优先级。
 *                           若该参数大于1，则表示对crop进行设置；
 *                           若该参数为0或者1，则表示对size进行设置
 * 返回： bool
 * 日期：2018.1.31
 * 作者： zh.sun
 */
bool SignalSource::OpenWindow(HI_U32 u32VoChn, RECT_S stWndRect, HI_U32 u32Priority)
{
    // 设置Vo的大小
    if( u32Priority <= 1 ){
        if(!gbLowLatency){
            if( HI_SUCCESS != Singleton<HimppMaster>::getInstance()->SetVoSize(u32VoChn, stWndRect, u32Priority))
                return false;
        }

        //在map中查找指定元素
        if(tovochn_.find(u32VoChn) == tovochn_.end()){ //如果没有查找到对应的元素
            //log_d("vdchn: %d, vochn: %d", vdchn_, u32VoChn);
            tovochn_.insert(make_pair(u32VoChn, u32Priority));
            //Singleton<HimppMaster>::getInstance()->SetVpssGrpFrameRate(u32VoChn, 60, 59);
            if(!gbLowLatency){
#ifndef _MANUAL_PUSH_VPSS_
                Singleton<HimppMaster>::getInstance()->VdecBindVpss(vdchn_, u32VoChn);
#endif
#ifndef _MANUAL_PUSH_VO_
                Singleton<HimppMaster>::getInstance()->VoBindVpss(u32VoChn, u32VoChn, u32Priority);
#endif // _MANUAL_PUSH_VO_
            }
        }
    }
    // 设置Crop的大小
    else{
        if(!gbLowLatency){
            if( HI_SUCCESS != Singleton<HimppMaster>::getInstance()->SetCropSize(u32VoChn, stWndRect))
                return false;
        }
    }

    return true;
}

bool SignalSource::CloseWindow(HI_U32 u32VoChn)
{
    //在map中查找指定元素
    auto iter = tovochn_.find(u32VoChn);
    //删除指定元素
    if(iter != tovochn_.end()){
        if(!gbLowLatency){
            Singleton<HimppMaster>::getInstance()->DisableVo(u32VoChn, iter->second);

#ifndef _MANUAL_PUSH_VPSS_
            Singleton<HimppMaster>::getInstance()->VdecUnbindVpss(vdchn_, u32VoChn);
#endif
#ifndef _MANUAL_PUSH_VO_
            Singleton<HimppMaster>::getInstance()->VoUnBindVpss(u32VoChn, u32VoChn, iter->second);
#endif // _MANUAL_PUSH_VO_
        }

        tovochn_.erase(iter);
    }

    return true;
}

bool SignalSource::InitTransmit()
{
    if(this->b_transmit_){
        log_w("[vdchn:%d] InitTransmit() has been run.", vdchn_);
        return true;
    }

    if(multicastsocket_ == NULL){
        multicastsocket_ = new UDPSocket();
        multicastsocket_->CreateUDPClient(st_transmit_.ip, st_transmit_.port, false);
        multicastsocket_->AddMemberShip(st_transmit_.ip, gSysEthUser.c_str());
        multicastsocket_->BindLocalAddr(gSysEthUser.c_str());
        multicastsocket_->SetRevBuf(32);

        int ttl = gMltTTL;
        multicastsocket_->SetMulticastTTL(ttl);
        //multicastsocket_->SetIpTTL(ttl);
        //log_d("ttl: %d | %d", multicastsocket_->GetMulticastTTL(), multicastsocket_->GetIpTTL());
    }

    if(mcasttimer_ == NULL)
        mcasttimer_ = new MyTimer();

    this->b_transmit_ = true;

    /** TODO:
    瞬时的码率峰值可能会超过系统默认的udp的缓冲区大小，导致解码数据不完整，看到花屏的视频。
    解决这个问题的方法，需要增加udp缓冲区的大小，
    设置的方法分为 设置系统的整体缓冲区; 在程序中设置socket的缓冲区.
    1. 设置linux系统的缓冲区的方法，是在命令行下输入：
    sysctl -w net.core.rmem_max=40960000
    sysctl -w net.core.wmem_max=40960000
    sysctl -w net.core.wmem_default=20480000
    sysctl -w net.core.rmem_default=20480000
    sysctl -w net.ipv4.udp_mem="20480000 20480000 20480000"
    sysctl -w net.ipv4.udp_rmem_min=20480000
    sysctl -w net.ipv4.udp_wmem_min=20480000
    或者
    echo 20480000 > /proc/sys/net/core/wmem_default
    echo 40960000 > /proc/sys/net/core/wmem_max
    echo 20480000 > /proc/sys/net/core/rmem_default
    echo 40960000 > /proc/sys/net/core/rmem_max

    2. 在程序中设置，以c/c++为例：
    int sockSrv = socket(AF_INET,SOCK_DGRAM, 0);
    int value = 40960000;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF,(char *)&value, sizeof(value));

    无论上述哪种方法，最终在socket使用时，都需要验证下是否设置成功，验证方法以c/c++为例：
    socklen_t valSize = sizeof(value);
    getsockopt(sockSrv, SOL_SOCKET, SO_RCVBUF,(char *)&value, &valSize);
    printf("socket size: %d\n",value);
    **/

    return true;
}

bool SignalSource::DeleteTransmit()
{
    if(!this->b_transmit_){
        log_w("[vdchn:%d] DeleteTransmit() has been run.", vdchn_);
        return true;
    }

    this->b_transmit_ = false;

    this->transurl_.clear();

    mutex_.lock();
    if(multicastsocket_ != NULL){
        multicastsocket_->DropMemberShip();
        delete multicastsocket_;
        multicastsocket_ = NULL;
    }
    if(mcasttimer_ != NULL){
        delete mcasttimer_;
        mcasttimer_ = NULL;
    }
    mutex_.unlock();

    log_d("[vdchn:%d] DeleteTransmit() SUCCEED", vdchn_);
    return true;
}

VO_MAP SignalSource::GetVoChnVector()
{
    return tovochn_;
}

uint SignalSource::GetVdChn()
{
    return vdchn_;
}

AUDIO_INFO_S SignalSource::GetAudioInfo()
{
    return st_audioinfo_;
}

#ifdef  __VDEC_SEPARATE__
typedef struct SEND_VDEC_SEPARATE_S{
    bool bneed;
    bool banother;
    int  vdec_base;
    int  vdec_another;
    int  winnum;
    int  vpsschn[WIN_NUM_MAX];      //目前不能与__VPSS_SEPARATE__混用
}SEND_VDEC_SEPARATE_S;
extern SEND_VDEC_SEPARATE_S        stVdecSeparate[VD_SEP_CHN_NUM];
#endif
bool SignalSource::DecodeVideo(void* data, unsigned int len, unsigned int pts, unsigned wnd, bool iframe)
{
    if(wnd >= (HI_U32)min(gVdecChnMax, VD_CHN_MAX)){
        return true;
    }

    VDEC_STREAM_S stStream;
    stStream.pu8Addr = (HI_U8*)(data);
    stStream.u32Len  = (HI_U32)(len );
    stStream.u64PTS  = pts;
    stStream.bEndOfStream = HI_FALSE;
    HI_S32 s32MilliSec = 0;

    if(!gbSendStream) //当ctrl+c之后控制signalsource类中的DecodeVideo函数不再推送
        return true;

#ifdef  __VDEC_SEPARATE__
    int sep_index = -1;
    for( int i = 0; i < VD_SEP_CHN_NUM; i++){
        if(stVdecSeparate[i].vdec_base == (int)wnd && stVdecSeparate[i].bneed == true){
            sep_index = i;
            break;
        }
    }
    if(sep_index!= -1){
        if(iframe)
            stVdecSeparate[sep_index].banother = !stVdecSeparate[sep_index].banother;
        if(stVdecSeparate[sep_index].banother)
            wnd = stVdecSeparate[sep_index].vdec_another;
    }
#endif
    HI_S32 s32Ret = HI_MPI_VDEC_SendStream( wnd, &stStream, s32MilliSec );
    if( HI_SUCCESS != s32Ret ){
        //log_e("HI_MPI_VDEC_SendStream[%d] failed errno 0x%x\n", wnd, s32Ret);
        return HI_FALSE;
    }

#if 0
    /* 打印解码器信息 */
    VDEC_CHN_STAT_S pstStat;
    if( HI_SUCCESS == HI_MPI_VDEC_Query(wnd, &pstStat)){
        printf( "u32LeftStreamBytes:%d u32LeftStreamFrames:%d u32LeftPics:%d\n", pstStat.u32LeftStreamBytes, pstStat.u32LeftStreamFrames, pstStat.u32LeftPics );
    }
#endif

#if defined (_MANUAL_PUSH_VPSS_)
    /* 手动推送frame： vdec->(vpss-vo) */
    while( !gbLowLatency )
    {
        VIDEO_FRAME_INFO_S info;
        s32Ret = HI_MPI_VDEC_GetImage( wnd, &info, s32MilliSec);
        if( s32Ret != HI_SUCCESS ) break;

        auto iter = tovochn_.begin();
        while(iter != tovochn_.end()){
            s32Ret = HI_MPI_VPSS_SendFrame( iter->first, &info, s32MilliSec );
            //if( HI_SUCCESS != s32Ret )
            //    log_e( "HI_MPI_VPSS_UserSendFrame failed with %#x!\n", s32Ret );

            iter++;
        }

        HI_MPI_VDEC_ReleaseImage(wnd, &info);
    }

#elif defined (_MANUAL_PUSH_VO_)

    /* 手动推送frame： (vdec-vpss)->vo */
    VIDEO_FRAME_INFO_S info;
    VPSS_CHN VpssChn = 0;
    VO_LAYER VoLayer = VO_LAYER_VHD0;

    auto iter = tovochn_.begin();
    while(iter != tovochn_.end()){
        s32Ret = HI_MPI_VPSS_GetChnFrame(iter->first, VpssChn, &info, s32MilliSec);
        if( s32Ret != HI_SUCCESS ){
            log_v( "HI_MPI_VPSS_GetChnFrame(Grp:%d) failed with %#x!\n", iter->first, s32Ret );
            iter++;
            continue;
        }

        AddFrameTab(&(info.stVFrame));

        /*log_d("info.stVFrame.u32Width: %d, info.stVFrame.u32Height: %d,\
                  info.stVFrame.u32Field: %x, info.stVFrame.enPixelFormat: %x,\
                  info.stVFrame.enVideoFormat: %x, info.stVFrame.enCompressMode: %x,\
                  info.stVFrame.u32Stride[0]: %d, info.stVFrame.u32Stride[1]: %d, info.stVFrame.u32Stride[2]: %d,\
                  info.stVFrame.s16OffsetTop: %d, info.stVFrame.s16OffsetBottom: %d,\
                  info.stVFrame.s16OffsetLeft: %d, info.stVFrame.s16OffsetRight: %d,\
                  info.stVFrame.u64pts: %d, info.stVFrame.u32TimeRef: %d",
                  info.stVFrame.u32Width, info.stVFrame.u32Height,
                  info.stVFrame.u32Field, info.stVFrame.enPixelFormat,
                  info.stVFrame.enVideoFormat, info.stVFrame.enCompressMode,
                  info.stVFrame.u32Stride[0], info.stVFrame.u32Stride[1], info.stVFrame.u32Stride[2],
                  info.stVFrame.s16OffsetTop, info.stVFrame.s16OffsetBottom,
                  info.stVFrame.s16OffsetLeft, info.stVFrame.s16OffsetRight,
                  info.stVFrame.u64pts, info.stVFrame.u32TimeRef
                  );*/

        if( HI_SUCCESS != HI_MPI_VO_SendFrame( VoLayer, iter->first, &info, s32MilliSec ) )
            log_v( "HI_MPI_VPSS_UserSendFrame failed with %#x!\n", s32Ret );

        HI_MPI_VPSS_ReleaseChnFrame(iter->first, VpssChn, &info);

        iter++;
    }
#endif // _MANUAL_PUSH_XXX_

    return true;
}

bool SignalSource::DecodeAudio(void* data, unsigned int len, unsigned int pts, int audio_type, int sample, int channel)
{
    if(gbAudioLoop || gbAoDelayAdjust || !gbSendStream) return true;

    if( gbAoEnAdjust ){
        //NOTE: 调整音频输出时钟，以求达到加快/减慢音频播放的效果，来矫正与输入流的音频时间差而导致的长时间播放，音频输出出现延迟的问题。
        AO_CHN_STATE_S stStatus;
        if(HI_SUCCESS == HI_MPI_AO_QueryChnStat(gAoDev, 0, &stStatus)){
            /**NOTE:
            HI_U32 pll = 0;
            SAMPLE_SYS_GetReg(0x13140140, &pll);
             * 0x13140140 调整 AoDev=0的 MCLK 的分频时钟配置值 的寄存器, 48K下默认为 0x003254E7;
             * 0x13140150 调整 AoDev=2的 MCLK 的分频时钟配置值 的寄存器, 48K下默认为 0x003254E7;
             * 1 / (500*10^6 / (48000 * 32 * 8) / 2^27) = 3298534.883328 = 0x003254E7
            **/
            if(stStatus.u32ChnBusyNum > 3 ){
                stAoAdjust_.posover += 1000;
            }
            else if(stStatus.u32ChnBusyNum == 0){
                stAoAdjust_.negover += 1000;
            }

            if((gbEnKvmMode && stStatus.u32ChnBusyNum > gAoDelayFrm + 5) ||
               (!gbEnKvmMode && stStatus.u32ChnBusyNum > gAoDelayFrm + 11)){
                log_d("Clear AO Buf: %d", stStatus.u32ChnBusyNum);
                HI_MPI_AO_ClearChnBuf(gAoDev, 0);
                HI_MPI_AO_ClearChnBuf(gAoDev, 1);
            }

            //log_d("Total: %d, Busy: %d, Delay: %d", stStatus.u32ChnTotalNum, stStatus.u32ChnBusyNum, gAoDelayFrm);
        }

        if(stAoAdjust_.start.tv_sec == 0 && stAoAdjust_.start.tv_usec == 0){
            gettimeofday(&stAoAdjust_.start, 0);
            stAoAdjust_.posover = 0;
            stAoAdjust_.negover = 0;
            stAoAdjust_.counter = 0;
        }
        else{ // NOTE: 一个周期大约为89.5秒
            if(stAoAdjust_.counter < (AOADJUST_CNT - 2000)){
                stAoAdjust_.counter += 1000;
            }
            else{
                stAoAdjust_.counter += 1000;
                gettimeofday(&stAoAdjust_.end, 0);

                if( stAoAdjust_.posover > (AOADJUST_CNT>>1) )
                    stAoAdjust_.offset = 1;
                else if( stAoAdjust_.negover > (AOADJUST_CNT>>1) )
                    stAoAdjust_.offset = 0;

                long difftime = ((stAoAdjust_.end.tv_usec/1000 + stAoAdjust_.end.tv_sec*1000) -
                                 (stAoAdjust_.start.tv_usec/1000 + stAoAdjust_.start.tv_sec*1000));
                if(difftime < 91000) {
                    long ret = ( (stAoAdjust_.counter<<10) / difftime ) + stAoAdjust_.offset;
                    long long newpll = 0x003254E7;
                    newpll = (newpll * ret / 48000);
                    SAMPLE_SYS_SetReg(0x13140140, newpll);
#if !defined  __C7000__ && !defined __HI3521D__
                    SAMPLE_SYS_SetReg(0x13140150, newpll);
#else
                    SAMPLE_SYS_SetReg(0x13140148, newpll);
#endif // __C7000__
                    log_d("real audio freq: %ld, set 0x%llx, offset: %d, difftime: %d", ret, newpll, stAoAdjust_.offset, difftime);
                }
                else{
                    SAMPLE_SYS_SetReg(0x13140140, 0x003254E7);
#if !defined  __C7000__ && !defined __HI3521D__
                    SAMPLE_SYS_SetReg(0x13140150, 0x003254E7);
#else
                    SAMPLE_SYS_SetReg(0x13140148, 0x003254E7);
#endif // __C7000__
                    log_e("difftime: %d", difftime);
                }
                log_d("Total: %d, Busy: %d, Delay: %d", stStatus.u32ChnTotalNum, stStatus.u32ChnBusyNum, gAoDelayFrm);

                stAoAdjust_.start.tv_sec  = 0;
                stAoAdjust_.start.tv_usec = 0;
            }
        }
    }

    // 音频输出只能输出单帧为 stAioAttr.u32PtNumPerFrm * stAioAttr.u32ClkChnCnt 长度的 pcm 音频
    if ( b_audio_ ){
        if(audio_type == C_AUDIO_PCMA){
            byte* pcm;
            pcm = new byte[len*2];
            g711_decode((byte*)data, len, pcm);

            PushOneFramePCM(pcm, len*2, pts);

            delete [] pcm;
        }
        else if(audio_type == C_AUDIO_AAC){
            char *adts_data = new char[len+10];
            this->AddAdtsHead((char*)data, adts_data, &len, sample, channel);

            byte* pcm = new byte[8192];
            size_t pcm_len = 0;
            aac_dec_.dec((unsigned char*)adts_data, len, pcm, pcm_len);

            PushOneFramePCM(pcm, pcm_len, pts);

            delete [] pcm;
            pcm = NULL;
            delete [] adts_data;
            adts_data = NULL;
        }
        else if(audio_type == C_AUDIO_LPCM){
            PushOneFramePCM((byte*)data, len, pts);
        }
    }

    return true;
}

bool SignalSource::TransmitVideo(int video_type, int video_width, int video_heigth, void* data, unsigned int len, unsigned int pts, int frame_rate, int p_sn)
{
    char senddata[PACK_LEN + INFO_HEADER];
    memset(senddata,0, (INFO_HEADER + PACK_LEN));
    senddata[INFO_HEADER_H0] = 'S'; //Head
    senddata[INFO_HEADER_H1] = 'L';
    senddata[INFO_HEADER_FG] = 0;
    if(is_i_frame_)
        senddata[INFO_HEADER_FG] |= 1;
    if(video_type == C_VIDEO_H265)
        senddata[INFO_HEADER_FG] |= 2;

    senddata[INFO_HEADER_FQ] = frame_rate;
    senddata[INFO_HEADER_FD] = pts;
    senddata[INFO_HEADER_SN] = p_sn; //包标号，仅仅用来防止丢包
    senddata[INFO_HEADER_VE] = 0;
    senddata[INFO_HEADER_VA] = 'V';

    void *tmp_pointer;
    tmp_pointer = &senddata[INFO_HEADER_TP];
    *(HI_U16*)tmp_pointer = (len + i_frame_len_ + PACK_LEN - 1) / PACK_LEN;  //总包数
    tmp_pointer = &senddata[INFO_HEADER_CP];
    *(HI_U16*)tmp_pointer = 0;                    //当前包号
    tmp_pointer = &senddata[INFO_HEADER_WW];
    *(HI_U16*)tmp_pointer = video_width;          //宽度
    tmp_pointer = &senddata[INFO_HEADER_HH];
    *(HI_U16*)tmp_pointer = video_heigth;         //高度

    int tmplen = len;
    if(i_frame_len_){
        //拷贝I帧的小包
        memcpy(&senddata[INFO_HEADER], i_frame_data_, i_frame_len_);
        //补齐剩下的数据
        memcpy(&senddata[INFO_HEADER + i_frame_len_], data, PACK_LEN - i_frame_len_);

        if(multicastsocket_ == NULL) return false;
        multicastsocket_->SendTo((const char*)&senddata[0], min(PACK_LEN,tmplen + i_frame_len_) + INFO_HEADER);

        data = (HI_U8*)data + PACK_LEN - i_frame_len_;
        tmplen -= PACK_LEN - i_frame_len_;
        tmp_pointer = &senddata[INFO_HEADER_CP];
        (*(HI_U16*)tmp_pointer) ++;          //指向下一包
    }

    int s32Len = 0;
    while (tmplen > 0){
        s32Len = min(PACK_LEN,tmplen);
        memcpy(&senddata[INFO_HEADER], data, s32Len);

        if(multicastsocket_ == NULL) return false;
        multicastsocket_->SendTo((const char*)&senddata[0], s32Len + INFO_HEADER);

        data = (HI_U8*)data + s32Len;
        tmplen -= s32Len;
        tmp_pointer = &senddata[INFO_HEADER_CP];
        (*(HI_U16*)tmp_pointer) ++;          //指向下一包
    }

    return true;
}

bool SignalSource::TransmitAudio(int audio_type, int audio_sample, int audio_channel, void* data, unsigned int len, unsigned int pts, unsigned wnd, int p_sn)
{
    char senddata[PACK_LEN + INFO_HEADER];
    senddata[INFO_HEADER_H0] = 'S'; //Head
    senddata[INFO_HEADER_H1] = 'L';
    switch (audio_type) {
        case C_AUDIO_PCMA:
            senddata[INFO_HEADER_FG] = 2;
            break;
        case C_AUDIO_AAC:
            senddata[INFO_HEADER_FG] = 1;
            break;
        case C_AUDIO_LPCM:
        default:
            senddata[INFO_HEADER_FG] = 0;
            break;
    }

    senddata[INFO_HEADER_FQ] = 0;
    senddata[INFO_HEADER_FD] = 0;
    senddata[INFO_HEADER_SN] = p_sn; //包标号，仅仅用来防止丢包
    senddata[INFO_HEADER_VE] = 0;
    senddata[INFO_HEADER_VA] = 'A';

    void *tmp_pointer;
    tmp_pointer = &senddata[INFO_HEADER_TP];
    *(HI_U16*)tmp_pointer = (len + PACK_LEN - 1) / PACK_LEN;  //总包数
    tmp_pointer = &senddata[INFO_HEADER_CP];
    *(HI_U16*)tmp_pointer = 0;                    //当前包号
    tmp_pointer = &senddata[INFO_HEADER_WW];
    *(HI_U16*)tmp_pointer = 0;          //宽度
    tmp_pointer = &senddata[INFO_HEADER_HH];
    *(HI_U16*)tmp_pointer = 0;         //高度

    int tmplen = len;
    while (tmplen > 0){
        memcpy(&senddata[INFO_HEADER], data, min(PACK_LEN,tmplen));

        if(multicastsocket_ == NULL) return false;
        multicastsocket_->SendTo((const char*)&senddata[0], min(PACK_LEN,tmplen) + INFO_HEADER);

        data = (HI_U8*)data + min(PACK_LEN,tmplen);
        tmplen -= min(PACK_LEN,tmplen);
        tmp_pointer = &senddata[INFO_HEADER_CP];
        (*(HI_U16*)tmp_pointer) ++;          //指向下一包
    }

    return true;
}

/*
  * NOTE:  ADTS的 Head bit图和代码实现：
         0                             1
         0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 byte=0 |               syncword               |ID|layer|
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
      2 |pa| pro |  sampling |pb|channel |oc|h |cb|aac_..
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
      4         aac_frame_length         |adts_buffer_...
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
      6  adts_buffer_fullness|num r|
        +==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+

   ADTS头分为固定头信息-adts_fixed_header() 和 可变头信息-adts_variable_header()两个部分
Syntax                        No.of bits
adts_fixed_header()
{
syncword;                       12
ID;                             1
layer;                          2
protection_absent;              1
profile;                        2
sampling_frequency_index;       4
private_bit;                    1
channel_configuration;          3
original_copy;                  1
home;                           1
}

syncword：帧同步标识一个帧的开始，固定为0xFFF
ID：MPEG 标示符。0表示MPEG-4，1表示MPEG-2
layer：固定为'00'
protection_absent：标识是否进行误码校验。0表示有CRC校验，1表示没有CRC校验
profile：标识使用哪个级别的AAC。1: AAC Main  2:AAC LC (Low Complexity)  3:AAC SSR (Scalable Sample Rate) 4:AAC LTP (Long Term Prediction)
sampling_frequency_index：标识使用的采样率的下标
private_bit：私有位，编码时设置为0，解码时忽略
channel_configuration：标识声道数
original_copy：编码时设置为0，解码时忽略
home：编码时设置为0，解码时忽略

sampling_frequency_index    Value
        0x0                 96000
        0x1                 88200
        0x2                 64000
        0x3                 48000
        0x4                 44100
        0x5                 32000
        0x6                 24000
        0x7                 22050
        0x8                 16000
        0x9                 12000
        ...
        ...

***注：ADTS Header的长度可能位7字节或者9字节，protection_absent=0时，9字节；protection_absent=1时，7字节

Syntax                        No.of bits
adts_variable_header()
{
copyrighted_id_bit;                     1
copyrighted_id_start;                   1
aac_frame_length;                       13
adts_buffer_fullness;                   11
number_of_raw_data_blocks_in_frame;     2
}

copyrighted_id_bit：编码时设置为0，解码时忽略
copyrighted_id_start：编码时设置为0，解码时忽略
aac_frame_length：ADTS帧长度包括ADTS长度和AAC声音数据长度的和。即 aac_frame_length = (protection_absent == 0 ? 9 : 7) + audio_data_length
adts_buffer_fullness：固定为0x7FF。表示是码率可变的码流
number_of_raw_data_blocks_in_frame：表示当前帧有number_of_raw_data_blocks_in_frame + 1 个原始帧(一个AAC原始帧包含一段时间内1024个采样及相关数据)。
*/
bool SignalSource::AddAdtsHead(char *src, char *dst, unsigned int* len, int sample, int channel)
{
    int profile = 2; // AAC LC
    int freqIdx = 3; // 48KHz
    int chanCfg = channel; // CPE

    switch (sample) {
        case 96000: {freqIdx = 0; break;}
        case 88200: {freqIdx = 1; break;}
        case 64000: {freqIdx = 2; break;}
        case 48000: {freqIdx = 3; break;}
        case 44100: {freqIdx = 4; break;}
        case 32000: {freqIdx = 5; break;}
        case 24000: {freqIdx = 6; break;}
        case 22050: {freqIdx = 7; break;}
        case 16000: {freqIdx = 8; break;}
        case 12000: {freqIdx = 9; break;}
        default: {freqIdx = 3; break;}
    }

    *len += 7;

    dst[0] = 0xFF;
    dst[1] = 0xF1;
    dst[2] = (((profile - 1) << 6) + (freqIdx << 2) + (chanCfg >> 2));
    dst[3] = (((chanCfg & 3) << 6) + (*len >> 11));
    dst[4] = ((*len & 0x7FF) >> 3);
    dst[5] = (((*len & 0x7) << 5) | 0x1F);
    dst[6] = 0xFC;

    //log_d("0x%x, 0x%x, 0x%x|0x%x, 0x%x|0x%x", (((profile - 1) << 6) + (freqIdx << 2) + (chanCfg >> 2)), (((chanCfg & 3) << 6) + (*len >> 11)), ((*len & 0x7FF) >> 3), (*len & 0x1FF8) >> 3, (((*len & 7) << 5) + 0x1F), (((*len & 0x7) << 5) | 0x1F));

    memcpy(dst + 7, src, *len - 7);

    return true;
}

bool SignalSource::PushOneFramePCM(unsigned char *data, unsigned int len, unsigned int pts)
{
    HI_U32  u32NumPerFrm = 1024 * 2; // stAioAttr.u32PtNumPerFrm * stAioAttr.u32ClkChnCnt
    HI_U32  u32FrameLen = u32NumPerFrm;
    HI_BOOL bRealDual = HI_FALSE;
    if( len / u32NumPerFrm > 1 ){
        u32FrameLen = u32NumPerFrm * 2;
        bRealDual = HI_TRUE;
    }

    while(len > 0){
        HI_U32 u32Len = min(u32FrameLen, len);
        //printf("pcm_len: %d, u32FrameLen: %d, bRealDual: %d, u32Len: %d\n", len, u32FrameLen, bRealDual, u32Len);
        HI35xx_AUDIO_SendData( gAoDev, data, u32Len, pts,
                               bRealDual, audiomixchn_ );

        data = (HI_U8*)data + u32Len;
        len -= u32Len;
    }

    return true;
}

#ifdef _VIDEO_CACHE_
HI_VOID* SignalSource::VideoCacheDecodeProc(HI_VOID *__this)
{
    SignalSource *_this = (SignalSource *)__this;

    while(!gbPlay){
        usleep(10);
    }

    while(gbPlay){
        if(_this->videocache_.size() >= 1){
            log_d("videocache_ size: %d", _this->videocache_.size());
            _this->DecodeVideo((void*)_this->videocache_[0].data, _this->videocache_[0].len, 0, 0);
            _this->videocache_.erase(_this->videocache_.begin());
        }
        else
            usleep(1000);
    }

    _this->videocache_.clear();

    return (HI_VOID*)1;
}

bool SignalSource::VIDEO_CACHE_S(void* data, unsigned int len, unsigned int pts, unsigned wnd)
{
    struct VIDEO_CACHE_S vCache;
    vCache.data = (char*)data;
    vCache.len  = len;
    videocache_.push_back(vCache); //NOTE：这里这种push_back进去的数据时错误的，如果要使用则需要进一步修改
    //log_d("videocache_ size: %d", videocache_.size());

    return HI_TRUE;
}
#endif // _VIDEO_CACHE_

#ifdef _MANUAL_PUSH_VO_
// 给每一帧加一个帧号识别码
void SignalSource::AddFrameTab(VIDEO_FRAME_S *pVBuf)
{
    unsigned int w, h;
    char * pVBufVirt_Y;
    char * pVBufVirt_C;
    char * pMemContent;
    HI_U32 Ysize,Csize;
    PIXEL_FORMAT_E  enPixelFormat = pVBuf->enPixelFormat;
    HI_U32 u32UvHeight;/* 存为planar 格式时的UV分量的高度 */

    Ysize = (pVBuf->u32Stride[0])*(pVBuf->u32Height);
    if (PIXEL_FORMAT_YUV_SEMIPLANAR_420 == enPixelFormat){
        Csize = (pVBuf->u32Stride[1])*(pVBuf->u32Height)/2;
        u32UvHeight = pVBuf->u32Height/2;
    }
    else{
        Csize = (pVBuf->u32Stride[1])*(pVBuf->u32Height);
        u32UvHeight = pVBuf->u32Height;
    }

    pVBufVirt_Y = (HI_CHAR *) HI_MPI_SYS_Mmap(pVBuf->u32PhyAddr[0], Ysize);
    if (NULL == pVBufVirt_Y)
    {
        return;
    }

    pVBufVirt_C = (HI_CHAR *) HI_MPI_SYS_Mmap(pVBuf->u32PhyAddr[1], Csize);
    if (NULL == pVBufVirt_C)
    {
        HI_MPI_SYS_Munmap(pVBufVirt_Y, Ysize);
        return;
    }

    /* modify Y ----------------------------------------------------------------*/
    for(h=0; h<pVBuf->u32Height; h++)
        pMemContent = pVBufVirt_Y + h*pVBuf->u32Stride[0];

    /* modify U ----------------------------------------------------------------*/
    for(h=0; h<u32UvHeight; h++)
    {
        pMemContent = pVBufVirt_C + h*pVBuf->u32Stride[1];
        pMemContent += 1;

        for(w=0; w<pVBuf->u32Width/2; w++)
        {
            if(h < 2 && w < 2)
                *pMemContent = ((pVBuf->u32TimeRef%120)/2); //修改对应位置的 U数值
            pMemContent += 2;
        }
    }

    /* modify V ----------------------------------------------------------------*/
    for(h=0; h<u32UvHeight; h++)
    {
        pMemContent = pVBufVirt_C + h*pVBuf->u32Stride[1];

        for(w=0; w<pVBuf->u32Width/2; w++){
            if(h < 2 && w < 2)
                *pMemContent = ((pVBuf->u32TimeRef%120)/2);//修改对应位置的 V数值
            pMemContent += 2;
        }
    }

    //printf("u32TimeRef: %d\n", (pVBuf->u32TimeRef%120)/2); //取0～59的循环数

    HI_MPI_SYS_Munmap(pVBufVirt_Y, Ysize); //将取出的值反向写回
    HI_MPI_SYS_Munmap(pVBufVirt_C, Csize); //将取出的值反向写回
}

/* sp420 转存为 p420 ; sp422 转存为 p422  */
void SignalSource::SaveYUV2Local(VIDEO_FRAME_S * pVBuf, FILE *pfd)
{
    unsigned int w, h;
    char * pVBufVirt_Y;
    char * pVBufVirt_C;
    char * pMemContent;
    unsigned char TmpBuff[4096]; //如果这个值太小，图像很大的话存不了
    HI_U32 size,Csize;
    PIXEL_FORMAT_E  enPixelFormat = pVBuf->enPixelFormat;
    HI_U32 u32UvHeight;/* 存为planar 格式时的UV分量的高度 */

    Ysize = (pVBuf->u32Stride[0])*(pVBuf->u32Height);
    if (PIXEL_FORMAT_YUV_SEMIPLANAR_420 == enPixelFormat)
    {
        Csize = (pVBuf->u32Stride[1])*(pVBuf->u32Height)/2;
        u32UvHeight = pVBuf->u32Height/2;
    }
    else
    {
        Csize = (pVBuf->u32Stride[1])*(pVBuf->u32Height);
        u32UvHeight = pVBuf->u32Height;
    }

    pVBufVirt_Y = (HI_CHAR *) HI_MPI_SYS_Mmap(pVBuf->u32PhyAddr[0], Ysize);
    if (NULL == pVBufVirt_Y)
    {
        return;
    }

    pVBufVirt_C = (HI_CHAR *) HI_MPI_SYS_Mmap(pVBuf->u32PhyAddr[1], Csize);
    if (NULL == pVBufVirt_C)
    {
        HI_MPI_SYS_Munmap(pVBufVirt_Y, Ysize);
        return;
    }

    /* save Y ----------------------------------------------------------------*/
    fprintf(stderr, "saving......Y......");
    fflush(stderr);
    for(h=0; h<pVBuf->u32Height; h++)
    {
        pMemContent = pVBufVirt_Y + h*pVBuf->u32Stride[0];
        fwrite(pMemContent, pVBuf->u32Width, 1, pfd);
    }
    fflush(pfd);


    /* save U ----------------------------------------------------------------*/
    fprintf(stderr, "U......");
    fflush(stderr);
    for(h=0; h<u32UvHeight; h++)
    {
        pMemContent = pVBufVirt_C + h*pVBuf->u32Stride[1];

        pMemContent += 1;

        for(w=0; w<pVBuf->u32Width/2; w++)
        {
            TmpBuff[w] = *pMemContent;
            pMemContent += 2;
        }
        fwrite(TmpBuff, pVBuf->u32Width/2, 1, pfd);
    }
    fflush(pfd);

    /* save V ----------------------------------------------------------------*/
    fprintf(stderr, "V......");
    fflush(stderr);
    for(h=0; h<u32UvHeight; h++)
    {
        pMemContent = pVBufVirt_C + h*pVBuf->u32Stride[1];

        for(w=0; w<pVBuf->u32Width/2; w++)
        {
            TmpBuff[w] = *pMemContent;
            pMemContent += 2;
        }
        fwrite(TmpBuff, pVBuf->u32Width/2, 1, pfd);
    }
    fflush(pfd);

    fprintf(stderr, "done %d!\n", pVBuf->u32TimeRef);
    fflush(stderr);

    HI_MPI_SYS_Munmap(pVBufVirt_Y, Ysize);
    HI_MPI_SYS_Munmap(pVBufVirt_C, Csize);
}
#endif //_MANUAL_PUSH_VO_
