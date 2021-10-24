#define LOG_TAG    "COMMAND_SCODE"

#include "commandmap_scode.h"

#define SCODeHEAD  0
#define SCODePANEL 1

CommandMap_SCODe::CommandMap_SCODe()
{
    b_newcmd_ = true;

    //创建回复socket
    echoudp_ = new UDPSocket();
    echoudp_->CreateUDPClient(nullptr, 0); //设为非阻塞
    echoudp_->SetRevBuf(32);
    scode_ = new SCODe();
    printf("SCODe Version: %s", scode_->GetVersion());
#if 0
    for(int i = 0; i < gVdecChnMax; ++i){
        VDCHN_STATUS_S vdlist = { .is_empty=true, .vdchn=i, .url="" };
        vVdChnList_.push_back(vdlist);
    }

    //初始化rtsp close线程id
    for(int i = 0; i < RtspTidCnt; i++){
        rtspcloseTid_[i].tid = 0;
        rtspcloseTid_[i].balive = false;
    }
#endif
    //创建数据存储文件夹
    if( !MakeDir(DATA_FILE) )
        sprintf(mediafile_, "./%s", MEDIA_SCODE);
    else
        sprintf(mediafile_, "%s/%s", DATA_FILE, MEDIA_SCODE);

    //创建 Server数据存储文件夹
    sprintf(serverdatafold_, "%s/%s", DATA_FILE, SERVER_DATA);
    if( !MakeDir(serverdatafold_) ){
        sprintf(serverdatafold_, "./%s",  SERVER_DATA);
    }
    sprintf(vwallcfgfile_,      "%s/%s", serverdatafold_, VWALL_CFG);
    sprintf(clientinfofile_,    "%s/%s", serverdatafold_, CLIENT_INFO);
    sprintf(scenesmapfile_,     "%s/%s", serverdatafold_, SCENES_MAP);
    sprintf(scenespollingfile_, "%s/%s", serverdatafold_, SCENES_POLLING);
    sprintf(spolpausefile_,     "%s/%s", serverdatafold_, SPOL_PAUSE);
    sprintf(aomixweightfile_,   "%s/%s", serverdatafold_, AUDIOMIX_WEIGHT);
    sprintf(storagedatafold_,   "%s/%s", serverdatafold_, STORAGE_DATA);//创建 存储上传收到的文件的文件夹
    if( !MakeDir(storagedatafold_) ){
        sprintf(storagedatafold_, "./%s",  STORAGE_DATA);
    }
    sprintf(hostcfgfile_,    "%s/%s", storagedatafold_, HOST_CFG);
    sprintf(picdatfile_,     "%s/%s", storagedatafold_, PIC_DATA);

    //创建用于保存的数据结构
//    mediasaver_ = new MediaSCODeSave(mediafile_);

    handlermap_.insert(make_pair(&EnumtoStr(kGetNodes)[1],
                                ScodeHandler{kGetNodes,      &CommandMap_SCODe::GetNodesHandler}));
    handlermap_.insert(make_pair(&EnumtoStr(kGetNodes2)[1],
                                ScodeHandler{kGetNodes2,     &CommandMap_SCODe::GetNodes2Handler}));

//    handlermap_.insert(make_pair(&EnumtoStr(kSetLockFrame)[1],
//                                ScodeHandler{kSetLockFrame,  &CommandMap_SCODe::SetLockFrameHandler}));
//    handlermap_.insert(make_pair(&EnumtoStr(kGetTransmit)[1],
//                                ScodeHandler{kGetTransmit,   &CommandMap_SCODe::GetTransmitHandler}));
//    handlermap_.insert(make_pair(&EnumtoStr(kSetTransmit)[1],
//                                ScodeHandler{kSetTransmit,   &CommandMap_SCODe::SetTransmitHandler}));
//    handlermap_.insert(make_pair(&EnumtoStr(kGetWindows)[1],
//                                ScodeHandler{kGetWindows,    &CommandMap_SCODe::GetWindowsHandler}));
//    handlermap_.insert(make_pair(&EnumtoStr(kSetWindows)[1],
//                                ScodeHandler{kSetWindows,    &CommandMap_SCODe::SetWindowsHandler}));

//    handlermap_.insert(make_pair(&EnumtoStr(kGetAudios)[1],
//                                ScodeHandler{kGetAudios,     &CommandMap_SCODe::GetAudiosHandler}));
//    handlermap_.insert(make_pair(&EnumtoStr(kSetAudios)[1],
//                                ScodeHandler{kSetAudios,     &CommandMap_SCODe::SetAudiosHandler}));
//    handlermap_.insert(make_pair(&EnumtoStr(kGetAoData)[1],
//                                ScodeHandler{kGetAoData,     &CommandMap_SCODe::GetAoDataHandler}));
//    handlermap_.insert(make_pair(&EnumtoStr(kGetAoMixWeight)[1],
//                                ScodeHandler{kGetAoMixWeight,&CommandMap_SCODe::GetAoMixWeightHandler}));
//    handlermap_.insert(make_pair(&EnumtoStr(kSetAoMixWeight)[1],
//                                ScodeHandler{kSetAoMixWeight,&CommandMap_SCODe::SetAoMixWeightHandler}));
//    handlermap_.insert(make_pair(&EnumtoStr(kSaveAoMixWeight)[1],
//                                ScodeHandler{kSaveAoMixWeight,&CommandMap_SCODe::SaveAoMixWeightHandler}));

//    handlermap_.insert(make_pair(&EnumtoStr(kGetSceneState)[1],
//                                ScodeHandler{kGetSceneState, &CommandMap_SCODe::GetSceneStateHandler}));
//    handlermap_.insert(make_pair(&EnumtoStr(kSetSceneState)[1],
//                                ScodeHandler{kSetSceneState, &CommandMap_SCODe::SetSceneStateHandler}));

//    handlermap_.insert(make_pair(&EnumtoStr(kGetScenes)[1],
//                                ScodeHandler{kGetScenes, &CommandMap_SCODe::GetScenesHandler}));
//    handlermap_.insert(make_pair(&EnumtoStr(kGetScene)[1],
//                                ScodeHandler{kGetScene, &CommandMap_SCODe::GetSceneHandler}));
//    handlermap_.insert(make_pair(&EnumtoStr(kGetSceneNames)[1],
//                                ScodeHandler{kGetSceneNames, &CommandMap_SCODe::GetSceneNamesHandler}));
//    handlermap_.insert(make_pair(&EnumtoStr(kSaveScene)[1],
//                                ScodeHandler{kSaveScene,     &CommandMap_SCODe::SaveSceneHandler}));
//    handlermap_.insert(make_pair(&EnumtoStr(kDeleteScene)[1],
//                                ScodeHandler{kDeleteScene,   &CommandMap_SCODe::DeleteSceneHandler}));
//    handlermap_.insert(make_pair(&EnumtoStr(kClearScenes)[1],
//                                ScodeHandler{kClearScenes,   &CommandMap_SCODe::ClearScenesHandler}));
//    handlermap_.insert(make_pair(&EnumtoStr(kCallScene)[1],
//                                ScodeHandler{kCallScene,     &CommandMap_SCODe::CallSceneHandler}));
//    handlermap_.insert(make_pair(&EnumtoStr(kSetScenesPolling)[1],
//                                ScodeHandler{kSetScenesPolling,&CommandMap_SCODe::SetScenesPollingHandler}));
//    handlermap_.insert(make_pair(&EnumtoStr(kPauseScenesPolling)[1],
//                                ScodeHandler{kPauseScenesPolling,&CommandMap_SCODe::PauseScenesPollingHandler}));

//    handlermap_.insert(make_pair(&EnumtoStr(kSetVideoWall)[1],
//                                ScodeHandler{kSetVideoWall,  &CommandMap_SCODe::SetVideoWallHandler}));
    handlermap_.insert(make_pair(&EnumtoStr(kUploadFile)[1],
                                ScodeHandler{kUploadFile,    &CommandMap_SCODe::UploadFileHandler}));
    handlermap_.insert(make_pair(&EnumtoStr(kDownloadFile)[1],
                                ScodeHandler{kDownloadFile,  &CommandMap_SCODe::DownloadFileHandler}));
    handlermap_.insert(make_pair(&EnumtoStr(kDeleteFiles)[1],
                                ScodeHandler{kDeleteFiles,   &CommandMap_SCODe::DeleteFilesHandler}));

//    handlermap_.insert(make_pair(&EnumtoStr(kSaveClientInfo)[1],
//                                ScodeHandler{kSaveClientInfo,&CommandMap_SCODe::SaveClientInfoHandler}));
//    handlermap_.insert(make_pair(&EnumtoStr(kGetClientInfo)[1],
//                                ScodeHandler{kGetClientInfo, &CommandMap_SCODe::GetClientInfoHandler}));
//    handlermap_.insert(make_pair(&EnumtoStr(kSaveMedia)[1],
//                                ScodeHandler{kSaveMedia, &CommandMap_SCODe::SaveMediaHandler}));


//    handlermap_.insert(make_pair(&EnumtoStr(kConnect)[1],
//                                ScodeHandler{kConnect, &CommandMap_SCODe::ConnectHandler}));
//    handlermap_.insert(make_pair(&EnumtoStr(kGetSignalsState)[1],
//                                ScodeHandler{kGetSignalsState, &CommandMap_SCODe::GetSignalsState}));

//    handlermap_.insert(make_pair(&EnumtoStr(kSubtitleOsd)[1],
//                                ScodeHandler{kSubtitleOsd, &CommandMap_SCODe::SubtitleOsdHandler}));

//    this->LoadConfig(vwallcfgfile_);
//    this->LoadConfig(spolpausefile_);
//    this->LoadSceneMap();
    this->LoadClientInfoMap();
}

CommandMap_SCODe::~CommandMap_SCODe()
{
    if(echoudp_ != NULL)
        delete echoudp_;
    if(scode_ != NULL)
        delete scode_;

//    if(gbAudioMixRun || audiomixtid_ != 0){
//        gbAudioMixRun = HI_FALSE;
//        pthread_join(audiomixtid_, 0);
//        audiomixtid_ = 0;
//    }

//    delete mediasaver_;
}

bool CommandMap_SCODe::LoadInitialMediaSCODe()
{
    this->LoadConfig(mediafile_);

    this->LoadConfig(scenespollingfile_);

    this->LoadConfig(aomixweightfile_);

//    if(!gbAudioMixRun && audiomixtid_ == 0){
//        audiomixtid_ = CreateThread(HI35xx_AUDIO_MixProc, 50, SCHED_FIFO, false, NULL);
//    }

    return true;
}
#if 0
void CommandMap_SCODe::SaveCommand(string scode_head, vstring valuelist)
{
    string mbody;
    string filename;
    if(scode_head == (&EnumtoStr(kSetVideoWall)[1])){
        filename = vwallcfgfile_;
    }
    else if(scode_head == (&EnumtoStr(kSetScenesPolling)[1])){
        filename = scenespollingfile_;
    }
    else if(scode_head == (&EnumtoStr(kPauseScenesPolling)[1])){
        filename = spolpausefile_;
    }

    for(uint i = 0; i < valuelist.size(); ++i){
        scode_->AddItem(mbody, valuelist[i].c_str());
    }
    scode_->CreateSCODeStr(mbody);
    if(!WriteSthFile(filename.c_str(), mbody.c_str())){
        printf(SCODE_WRITE_ERR, filename.c_str());
    }
}

void CommandMap_SCODe::DebugPrint()
{
#ifdef __DEBUG__
   printf("------------------__DEBUG__-------------------");
    for(auto ss : signalsrcmap_){
        if(ss.second->b_audio_)
           printf("vs[%d] url: %s (with audio[%d])", ss.second->GetVdChn(), ss.second->url_.c_str(), ss.second->audiomixchn_);
        else
           printf("vs[%d] url: %s", ss.second->GetVdChn(), ss.second->url_.c_str());
    }
    for(auto as : vaudiosrc_){
       printf("as[%d] url: %s", as->audiomixchn_, as->url_.c_str());
    }
    for(auto ts : vtranssrc_){
       printf("ts(%s -> %s)", ts->url_.c_str(), ts->transurl_.c_str());
    }
   printf("------------------__DEBUG__-------------------");
#endif
}
#endif
bool CommandMap_SCODe::ProcessCmd(string key, vstring valuelist)
{
    auto iter = handlermap_.find(key);
    if(iter != handlermap_.end()){
        if(this->isItemNumLegal(valuelist.size(), iter->second.def_num))
            return (this->*(iter->second.pfun))(valuelist);
        else{
            printf(SCODE_FORMAT_ERR);
            return false;
        }
    }
    else{
        printf(SCODE_INVALID_HEAD, key.c_str());
#ifdef _DEBUG_SCODE_
            printf("The CMD Header supported by the SCODe protocol: \n");
            for (auto iter = handlermap_.begin(); iter != handlermap_.end(); iter++){
                printf("%s ", (iter->first).c_str());
            }
            printf("\n");
#endif // _DEBUG_SCODE_
        return false;
    }
}

/** NOTE: {GetNodes;IP,Port} ：用于扫描所有的节点，支持广播/组播/单播
客户端→设备：
{
GetNodes;	//命令，不能省略
IP,Port		//应答发送的目的地址和端口，可以省略，默认为原路返回
}

设备→客户端：
{
GetNodes;	//命令，不能省略
panel_sn;	//视频墙，为-1表示没有配置视频墙位置,不能省略
host_cfgfile_md5,host_picdata_md5; //上位机软件配置文件的校验和,为0表示没有该文件
hardware_info,NodeState,NodeName;//硬件型号,节点状态，节点名称
customerID  //客户ID
}
**/
bool CommandMap_SCODe::GetNodesHandler(vstring valuelist)
{
 //   printf("\n-----1-------\n");
    string mbody;
    scode_->AddItem(mbody, valuelist[SCODeHEAD].c_str());
    scode_->AddItem(mbody, 99);
//    printf("\n-----2-------\n");
    string item;
    char md5[512] = "\0";
    if(!our_MD5File(hostcfgfile_, md5)){
        md5[0] = '0'; md5[1] = '\0';
    }
    scode_->AddValueToItem(item, md5);
    if(!our_MD5File(picdatfile_, md5)){
        md5[0] = '0'; md5[1] = '\0';
    }
    scode_->AddValueToItem(item, md5);
    scode_->AddItem(mbody, item.c_str());
//    printf("\n-----3-------\n");
    item.clear();
    string s;
    ReadHardwareInfo(s);
    scode_->AddValueToItem(item, s.c_str());


    scode_->AddValueToItem(item, "C");

    s = ReadSthFile("/data/node_name");
    scode_->AddValueToItem(item, s.c_str());

    scode_->AddItem(mbody, item.c_str());

    s = ReadSthFile("/version/customerID");
    scode_->AddItem(mbody, s.c_str());

    scode_->CreateSCODeStr(mbody);
    this->ACK(mbody.c_str(), valuelist, kGetNodes);
 //   printf("\"msg:%s",  mbody.c_str());

    return true;
}

/** NOTE: {GetNodes2;IP,Port} ：用于扫描所有的节点，支持广播/组播/单播
客户端→设备：
{
GetNodes2;	//命令，不能省略
IP,Port		//应答发送的目的地址和端口，可以省略，默认为原路返回
}

设备→客户端：
{
GetNodes2;	//命令，不能省略
panel_sn;	//视频墙，为-1表示没有配置视频墙位置,不能省略
hardware_info,NodeState,NodeName;//硬件型号,节点状态，节点名称
customerID;  //客户ID
filesMds
}
**/
bool CommandMap_SCODe::GetNodes2Handler(vstring valuelist)
{
    string mbody;
    scode_->AddItem(mbody, valuelist[SCODeHEAD].c_str());
    //scode_->AddItem(mbody, vwallcfg_.panel_sn_);

    string item;
    string s;
    ReadHardwareInfo(s);
    scode_->AddValueToItem(item, s.c_str());

    string c = "S";
    s = ReadSthFile("/data/software.ini");
    uint pos = s.find("kEnEncode=");
    if(pos != string::npos &&
       s.substr(pos+strlen("kEnEncode="), 1) == "1")
            c = "D";
    pos = s.find("kEnKvmOSD=");
    if(pos != string::npos &&
       s.substr(pos+strlen("kEnKvmOSD="), 1) == "1")
            c += "K";
    scode_->AddValueToItem(item, c.c_str());

    s = ReadSthFile("/data/node_name");
    scode_->AddValueToItem(item, s.c_str());

    scode_->AddItem(mbody, item.c_str());

    s = ReadSthFile("/version/customerID");
    scode_->AddItem(mbody, s.c_str());

    s.clear();
    bool isValid = false;
    auto files = getFiles(storagedatafold_);
    for (size_t i = 0; i < files.size(); i++){
        char md5[512] = "\0";
        char filePath[512] = "\0";
        snprintf(filePath, sizeof(filePath), "%s/%s", storagedatafold_, files[i].c_str());
        if(!our_MD5File(filePath, md5)){
            md5[0] = '0'; md5[1] = '\0';
        }
        s.append(files[i]);
        s.append(" ");
        s.append(md5);
        if(i < files.size() - 1)
            s.append(",");

        auto pos = files[i].find_last_of(".");
        if(pos && files[i].substr(pos) == ".ds"){
            isValid = true;
            scode_->AddItem(mbody, files[i].c_str());
        }
    }
    if(!isValid)
        scode_->AddItem(mbody, "null");
    scode_->AddItem(mbody, s.c_str());

    scode_->CreateSCODeStr(mbody);
    this->ACK(mbody.c_str(), valuelist, kGetNodes);

    return true;
}
#if 0
/** NOTE: {SetLockFrame} ：用于设置锁屏基数，实际锁屏时间为 锁屏帧系数
客户端→设备：
{
SetLockFrame;  //命令，不能省略
panel_sn;      //指定视频墙，不能省略
baselockframe  //锁屏帧系数
}

设备→客户端： 无
**/
bool CommandMap_SCODe::SetLockFrameHandler(vstring valuelist)
{
    if(str2int(valuelist[SCODePANEL]) == vwallcfg_.panel_sn_){
        gCmdLock = str2int(valuelist[2]);
        if(gCmdLock > 120) gCmdLock = 120;
    }

    return true;
}

/** NOTE: {GetTransmit} ：用于获取指定信号源的输出组播转发，支持单播
客户端→设备：
{
GetTransmit;   //命令，不能省略
panel_sn;      //指定视频墙，不能省略
IP,Port        //应答发送的目的地址和端口，可以省略，默认为原路返回
}

设备→客户端：
{
GetTransmit;      //命令，不能省略
panel_sn;        //视频墙，不能省略
src_ip,url,x,y,w,h,group_id;   //窗口的信号源ip，真实的像素位置大小,信号源组号(可省略，默认-1，即不属于任何组)
……
src_ip,url,x,y,w,h,group_id    //窗口的信号源ip，真实的像素位置大小,信号源组号(可省略，默认-1，即不属于任何组)
}
**/
bool CommandMap_SCODe::GetTransmitHandler(vstring valuelist)
{
    if(str2int(valuelist[SCODePANEL]) == vwallcfg_.panel_sn_){
        string mbody = transmitscode_;
        if(!mbody.empty()){
            mbody.replace(mbody.find("SetTransmit"), strlen("SetTransmit"), valuelist[SCODeHEAD]);
        }
        else{
            scode_->AddItem(mbody, valuelist[SCODeHEAD].c_str());
            scode_->AddItem(mbody, vwallcfg_.panel_sn_);
            scode_->CreateSCODeStr(mbody);
        }

        this->ACK(mbody.c_str(), valuelist, kGetWindows);
    }

    return true;
}

/** NOTE: {SetTransmit} ：用于设定指定信号源的输出组播转发，支持单播
客户端→设备：
{
SetTransmit;    //命令，**不能省略**
panel_sn;       //指定视频墙，**不能省略**
src_url,transmit_url;    //源地址url，转发组播url
……
src_url,transmit_url;    //源地址url，转发组播url
}

设备→客户端：无应答
**/
bool CommandMap_SCODe::SetTransmitHandler(vstring valuelist)
{
    if(str2int(valuelist[SCODePANEL]) == vwallcfg_.panel_sn_){
        if(valuelist.size() == 2){ /* NOTE: empty all transmit */
            for(auto iter = vtranssrc_.begin(); iter != vtranssrc_.end(); ) {
                delete (*iter);
                iter = vtranssrc_.erase(iter);
            }
        }
        else{
            /** NOTE: 解析命令中的字段，将需要转发的源地址和目标地址添加入队列 **/
            vector<vstring> vvs;
            for(uint i = 2; i < valuelist.size(); ++i){
                if(scode_->isArray(valuelist[i])){
                    vstring value = scode_->GetSCODeValues(valuelist[i]);
                    if(value.size() != 2){ // src_url,transmit_url
                        printf(SCODE_FORMAT_ERR);
                        continue;
                    }

                    vvs.push_back(value);
                }
            }

            /** NOTE: 在已有的转播队列中查找本次命令中包含的转播源地址和目标地址 **/
            for(auto iter = vtranssrc_.begin(); iter != vtranssrc_.end(); ) {
                uint i = 0;
                for(i = 0; i < vvs.size(); ++i){
                    if((*iter)->url_ == vvs[i][0]){
                        break;
                    }
                }
                /** NOTE: 对未在本次命令中查找到的转播源地址，进行关闭流操作 **/
                if(i == vvs.size()){
                    delete (*iter);
                    iter = vtranssrc_.erase(iter);
                }
                else{
                    ++iter;
                }
            }

            for(uint i = 0; i < vvs.size(); ++i){
                bool isFound = false;
                string src = vvs[i][0];
                string dst = vvs[i][1];
                SignalSource *currentlyts = NULL;
                /** NOTE: 如果在转播队列中查找到该源地址，则关闭组播转发，准备重新配置 **/
                for(auto ts : vtranssrc_){
                    if(ts->url_ == src){
                        isFound = true;
                        currentlyts = ts;
                        if(ts->transurl_ != dst){
                           printf("Close (%s) Transmit (%s)", ts->url_.c_str(), ts->transurl_.c_str());
                            ts->DeleteTransmit();
                        }
                    }
                }

                /** NOTE: 如果未在转播队列中查找到该源地址，则进行创建并Play的操作 **/
                if(!isFound){
                    currentlyts = new SignalSource(-100);
                    vtranssrc_.push_back(currentlyts);

                    string protocol = src.substr(0, src.find(":"));
                    if ( protocol == "rtsp" || protocol == "udp" )
                        currentlyts->protocol_ =  ((protocol == "udp") ? "multicast" : protocol);
                    currentlyts->b_video_ = true;
                    currentlyts->b_audio_ = true;
                    currentlyts->PlayNetStream(src.c_str());

                   printf("create new one in the ts! [%s] -> [%s]", src.c_str(), dst.c_str());
                }

                /** NOTE: 启动组播转发 **/
                if(currentlyts != NULL && currentlyts->transurl_ != dst){
                    sscanf(dst.c_str(), "%*[^://]://%[^:]:%d", currentlyts->st_transmit_.ip, &currentlyts->st_transmit_.port);
                    currentlyts->InitTransmit();
                    currentlyts->url_ = src;
                    currentlyts->transurl_ = dst;
                }
            }
        }

        /** NOTE: 还原并保存命令 **/
        string mbody;
        for(uint i = 0; i < valuelist.size(); ++i){
            scode_->AddItem(mbody, valuelist[i].c_str());
        }

        scode_->CreateSCODeStr(mbody);
        if(valuelist.size() == 2){ /* NOTE: empty all audios */
            transmitscode_.clear();
        }
        else
            transmitscode_ = mbody;
        this->mediasaver_->newcmd_cnt_++;
        this->mediasaver_->setScode(transmitscode_, MediaSCODeSave::TRANS);

        this->DebugPrint();
    }

    return true;
}

/** NOTE: {GetWindows} ：用于从某个输出节点获取当前输出节点属于的视频墙的实时窗口信息，只能单播
客户端→设备：
{
GetWindows;    //命令，不能省略
panel_sn;      //指定视频墙，不能省略
IP,Port        //应答发送的目的地址和端口，可以省略，默认为原路返回
}

设备→客户端：
{
GetWindows;      //命令，不能省略
panel_sn;        //视频墙，不能省略
src_ip,url,x,y,w,h,group_id;   //窗口的信号源ip，真实的像素位置大小,信号源组号(可省略，默认-1，即不属于任何组)
……
src_ip,url,x,y,w,h,group_id    //窗口的信号源ip，真实的像素位置大小,信号源组号(可省略，默认-1，即不属于任何组)
}
**/
bool CommandMap_SCODe::GetWindowsHandler(vstring valuelist)
{
    if(str2int(valuelist[SCODePANEL]) == vwallcfg_.panel_sn_){
        string mbody = windowssocde_;
        if(!mbody.empty()){
            mbody.replace(mbody.find("SetWindows"), strlen("SetWindows"), valuelist[SCODeHEAD]);
        }
        else{
            scode_->AddItem(mbody, valuelist[SCODeHEAD].c_str());
            scode_->AddItem(mbody, vwallcfg_.panel_sn_);
            scode_->CreateSCODeStr(mbody);
        }

        this->ACK(mbody.c_str(), valuelist, kGetWindows);
    }

    return true;
}

/** NOTE: {SetWindows} ：用于设定指定视频墙的实时窗口信息，支持广播/组播/单播
客户端→设备：
{
SetWindows;     //命令，**不能省略**
panel_sn;       //指定视频墙，**不能省略**
src_ip,url,x,y,w,h,group_id;    //窗口的信号源ip，流的URL地址，真实的像素位置大小,信号源组号(可省略，默认-1，即不属于任何组)
……
src_ip,url,x,y,w,h,group_id    //窗口的信号源ip，流的URL地址，真实的像素位置大小,信号源组号(可省略，默认-1，即不属于任何组)
}

设备→客户端：无应答
**/
bool CommandMap_SCODe::SetWindowsHandler(vstring valuelist)
{
    if(str2int(valuelist[SCODePANEL]) == vwallcfg_.panel_sn_){
        bool HasIPC = false;
        gCmdLock = BASE_LOCKFRAME + gSyncCompensation; //进行锁屏操作,锁定时间为 N 帧(60fps计),即 N * 16.667ms

        if(valuelist.size() == 2){ /* NOTE: empty all windows */
            string item;
            /** NOTE: 关闭视频信号,并将需要音频输出的信号源在 audiosrc表重新打开 **/
            for(auto iter = signalsrcmap_.begin(); iter != signalsrcmap_.end(); ){
                if(iter->second->b_audio_){
                    string v = num2str(iter->second->audiomixchn_) + "|" + iter->second->url_;
                    scode_->AddValueToItem(item, v.c_str());
                }
                delete (iter->second);
                signalsrcmap_.erase(iter++);
            }
            if(!item.empty()){
                string s;
                scode_->AddItem(s, "SetAudios");
                scode_->AddItem(s, vwallcfg_.panel_sn_);
                scode_->AddItem(s, item.c_str());
                scode_->CreateSCODeStr(s);
                vstring itemlist = scode_->GetSCODeItems(s);
                if(itemlist.size() > 0)
                    this->ProcessCmd(itemlist[0], itemlist);
            }

            for(auto vdlist = vVdChnList_.begin(); vdlist != vVdChnList_.end(); ++vdlist){
                vdlist->is_empty = true;
                vdlist->url.clear();
            }

            /** NOTE: 关闭图层窗口 **/
            if(!wndinfomap_.empty()){
                wndinfomap_.erase(wndinfomap_.begin(), wndinfomap_.end());
            }
        }
        else{
            vector<LAYER_INFO_S> vLayerInfo;
            /** STEP 1: 解析命令中的字段，并将属于该输出范围内的图层存储为向量队列 **/
            for(uint i = 2; i < valuelist.size(); ++i){
                LAYER_INFO_S stLayerInfo;
                if(scode_->isArray(valuelist[i])){
                   vstring value = scode_->GetSCODeValues(valuelist[i]);
                   if(value.size() < kMAX){ // ip,url,x,y,w,h
                       printf("SCODe CMD Format(Quantity) Error!(Usage(\"ip,url,x,y,w,h\"):%s)", valuelist[i].c_str());
                       valuelist.erase(valuelist.begin()+i); --i;
                       continue;
                   }
                   stLayerInfo.signal_id = value[kIP];
                   stLayerInfo.url = value[kURL];
                   stLayerInfo.stRect.s32X = str2int(value[kX]);
                   stLayerInfo.stRect.s32Y = str2int(value[kY]);
                   stLayerInfo.stRect.u32Width  = str2int(value[kW]);
                   stLayerInfo.stRect.u32Height = str2int(value[kH]);

                   if(stLayerInfo.url.empty() || stLayerInfo.signal_id.empty() ||
                      stLayerInfo.stRect.s32X < 0 || stLayerInfo.stRect.s32Y < 0 ||
                      stLayerInfo.stRect.u32Width <= 0 || stLayerInfo.stRect.u32Height <= 0){
                       printf("SCODe CMD Format(Value) Error!(Usage(\"ip,url,x,y,w,h\"):%s)", valuelist[i].c_str());
                       valuelist.erase(valuelist.begin()+i); --i;
                       continue;
                   }
                }
                else{
                    printf("SCODe CMD Format(Item.Type) Error!(Usage(\"ip,url,x,y,w,h\"):%s)", valuelist[i].c_str());
                    valuelist.erase(valuelist.begin()+i); --i;
                    continue;
                }

                //映射出图层有效坐标,过滤掉不属于该输出范围内的图层
                RECT_S stValidRect = RectAndRect(stLayerInfo.stRect, vwallcfg_.stNodeRect);
                if(stValidRect.u32Width != 0 && stValidRect.u32Height != 0)
                    vLayerInfo.push_back(stLayerInfo);
            }

            /** STEP 2: 计算信号窗口参数 **/
            int layer_cnt = 0;
            for(auto layer = vLayerInfo.begin(); layer != vLayerInfo.end(); ++layer) {
                //映射出图层有效坐标
                RECT_S stValidRect = RectAndRect(layer->stRect, vwallcfg_.stNodeRect);
                // 删除被上层图层拼接区域之和完全遮盖的图层
                bool isFirst = true;
                vector<RECT_S> vDiffRect;
                for (auto next = layer+1; next != vLayerInfo.end(); ++next) {
                    RECT_S stNextValidRect = RectAndRect(stValidRect, RectAndRect(next->stRect, vwallcfg_.stNodeRect));
                    if(stNextValidRect != RECT_S{0,0,0,0}){
                        if(isFirst){
                            isFirst = false;
                            vDiffRect = stValidRect - stNextValidRect;
                        }
                        else{
                            vDiffRect = vDiffRect - stNextValidRect;
                            if(vDiffRect.empty()) break;
                        }
                    }
                }
                if(vDiffRect.empty() && !isFirst) //被完全遮盖的图层，做删除处理
                    continue;

                double scale_w = (double)layer->stRect.u32Width / 1920.0;
                double scale_h = (double)layer->stRect.u32Height / 1080.0;

                WND_INFO_S stWndInfo;
                stWndInfo.format = kNORMAL;
                stWndInfo.vdchn = -1;
                stWndInfo.url = layer->url;
                stWndInfo.layer = layer_cnt;
                stWndInfo.signal_id = layer->signal_id;
                //CROP映射
                stWndInfo.stCropRect.s32X = ((double)(stValidRect.s32X - layer->stRect.s32X) / scale_w) + 0.5;
                stWndInfo.stCropRect.s32Y = ((double)(stValidRect.s32Y - layer->stRect.s32Y) / scale_h) + 0.5;
                stWndInfo.stCropRect.u32Width  = ((double)(stValidRect.s32X + stValidRect.u32Width - layer->stRect.s32X)  / scale_w + 0.5) - stWndInfo.stCropRect.s32X;
                stWndInfo.stCropRect.u32Height = ((double)(stValidRect.s32Y + stValidRect.u32Height - layer->stRect.s32Y) / scale_h + 0.5) - stWndInfo.stCropRect.s32Y;

//                stWndInfo.stCropRect.s32X = AlignNear(stWndInfo.stCropRect.s32X, 4);
//                stWndInfo.stCropRect.s32Y = AlignNear(stWndInfo.stCropRect.s32Y, 4);
//                stWndInfo.stCropRect.u32Width  = AlignNear(stWndInfo.stCropRect.u32Width, 4);
//                stWndInfo.stCropRect.u32Height = AlignNear(stWndInfo.stCropRect.u32Height, 4);
                stWndInfo.stCropRect = RectAndRect(stWndInfo.stCropRect, RECT_S{0, 0, 1920, 1080});

                //SIZE映射
                stWndInfo.stSizeRect.s32X = stValidRect.s32X - vwallcfg_.stNodeRect.s32X;
                stWndInfo.stSizeRect.s32Y = stValidRect.s32Y - vwallcfg_.stNodeRect.s32Y;
                stWndInfo.stSizeRect.u32Width  = stValidRect.u32Width;
                stWndInfo.stSizeRect.u32Height = stValidRect.u32Height;

                stWndInfo.stSizeRect.s32X = AlignNear(stWndInfo.stSizeRect.s32X, 2);
                stWndInfo.stSizeRect.s32Y = AlignNear(stWndInfo.stSizeRect.s32Y, 2);
                stWndInfo.stSizeRect.u32Width  = AlignNear(stWndInfo.stSizeRect.u32Width,  2);
                stWndInfo.stSizeRect.u32Height = AlignNear(stWndInfo.stSizeRect.u32Height, 2);

                if(wndinfomap_.size() <= WIN_NUM_MAX){
                    auto iter = wndinfomap_.find(stWndInfo.layer);
                    if(iter == wndinfomap_.end())
                        wndinfomap_.insert(make_pair(stWndInfo.layer, stWndInfo));
                    else{
                        wndinfomap_[stWndInfo.layer] = stWndInfo;
                    }
                }
                ++layer_cnt;
            }

            /** NOTE:
             * 因为SCODe协议命令是只支持全量发送的,
             * 所以 signalsrcmap_ 和 wndinfomap_ 中不应该保留上一次命令的信息,
             * 而且在每次命令处理开始清空 signalsrcmap_和 wndinfomap_ 并不合适(可能存在非法命令还未踢除),
             * 故放于此处.
             **/
            /** STEP 3.1: 删除上次命令中遗留的多余图层 **/
            auto begin = wndinfomap_.begin();
            for(int i = 0; i < layer_cnt; ++i) begin++;
            wndinfomap_.erase(begin, wndinfomap_.end());

            /** STEP 3.2: 关闭并删除上次命令中遗留的多余信号源**/
            for(auto vdlist = vVdChnList_.begin(); vdlist != vVdChnList_.end(); ++vdlist){
                if(!vdlist->is_empty){
                    int cnt = wndinfomap_.size();
                    for(auto wndinfo = wndinfomap_.begin(); wndinfo != wndinfomap_.end(); ++wndinfo){
                        /*NOTE: 该url 需要续期，则进行保留处理 */
                        if(vdlist->url == wndinfo->second.url){
                            break;
                        }
                        --cnt;
                    }
                    /*NOTE: 该url 不需要续期，则进行关闭处理 */
                    if(cnt <= 0){
                        vdlist->is_empty = true;
                        vdlist->url.clear();
                        auto iter = signalsrcmap_.find(vdlist->vdchn);
                        if(iter != signalsrcmap_.end()){
                           printf("DELETE | VdChn[%d]: %s", (iter->second)->GetVdChn(), (iter->second)->url_.c_str());
                            /** NOTE: 关闭视频信号,并将需要音频输出的信号源在 audiosrc表重新打开 **/
                            string item;
                            if(iter->second->b_audio_){
                                string v = num2str(iter->second->audiomixchn_) + "|" + iter->second->url_;
                                scode_->AddValueToItem(item, v.c_str());
                            }

                            delete (iter->second);
                            signalsrcmap_.erase(iter);

                            if(!item.empty()){
                                string s;
                                scode_->AddItem(s, "SetAudios");
                                scode_->AddItem(s, vwallcfg_.panel_sn_);
                                scode_->AddItem(s, item.c_str());
                                scode_->CreateSCODeStr(s);
                                vstring itemlist = scode_->GetSCODeItems(s);
                                if(itemlist.size() > 0)
                                    this->ProcessCmd(itemlist[0], itemlist);
                            }
                        }
                    }
                }
            }

            /** STEP 4: 计算信号解码参数，并将signal_id与对应窗口关联 **/
            for(auto wndinfo = wndinfomap_.begin(); wndinfo != wndinfomap_.end(); wndinfo++){
                int cnt = vVdChnList_.size();
                bool isNewLink = false;
                for(auto vdlist = vVdChnList_.begin(); vdlist != vVdChnList_.end(); ++vdlist){
                    /*NOTE: 找到以存在的解码该url的vdchn，则仍然使用该vdchn解码 */
                    if(vdlist->url == wndinfo->second.url){
                        wndinfo->second.vdchn = vdlist->vdchn;
                       printf("KEEP |  VdChn[%d]: %s", wndinfo->second.vdchn, wndinfo->second.url.c_str());
                        break;
                    }
                    --cnt;
                }
                /*NOTE: 未找到以存在的解码该url的vdchn，则从vdlist中查找空置的vdchn使用 */
                if(cnt <= 0){
                    for(auto vdlist_empty = vVdChnList_.begin(); vdlist_empty != vVdChnList_.end(); vdlist_empty++){
                        if( vdlist_empty->is_empty ){
                            vdlist_empty->is_empty = false;
                            vdlist_empty->url = wndinfo->second.url;

                            wndinfo->second.vdchn = vdlist_empty->vdchn;

                            isNewLink = true;
                           printf("NEW | VdChn[%d]: %s", wndinfo->second.vdchn, wndinfo->second.url.c_str());
                            break;
                        }
                    }
                }

                /*NOTE: 播放流地址,并打开对应的窗口*/
                SignalSource *currentlyss = NULL;
                auto iter = signalsrcmap_.find( wndinfo->second.vdchn );
                if(iter == signalsrcmap_.end()){ // 如果不存在对应的信号源，则创建一个，并存放入map中
                    currentlyss = new SignalSource( wndinfo->second.vdchn );
                    signalsrcmap_.insert(make_pair(wndinfo->second.vdchn, currentlyss));
                }
                else
                    currentlyss = iter->second;

                if( isNewLink ){
                    /* 1. 如果流地址不相同,先关闭当前正打开的串流 */
                    if(!currentlyss->url_.empty() &&
                        wndinfo->second.url != currentlyss->url_){

                        /** NOTE: 关闭视频信号,并将需要音频输出的信号源在 audiosrc表重新打开 **/
                        string item;
                        if(currentlyss->b_audio_){
                            string v = num2str(currentlyss->audiomixchn_) + "|" + currentlyss->url_;
                            scode_->AddValueToItem(item, v.c_str());
                        }

                        currentlyss->CloseNetStream();

                        if(!item.empty()){
                            string s;
                            scode_->AddItem(s, "SetAudios");
                            scode_->AddItem(s, vwallcfg_.panel_sn_);
                            scode_->AddItem(s, item.c_str());
                            scode_->CreateSCODeStr(s);
                            vstring itemlist = scode_->GetSCODeItems(s);
                            if(itemlist.size() > 0)
                                this->ProcessCmd(itemlist[0], itemlist);
                        }
                    }
                    /* 2. 如果音频源库中已存在该流，则先将音频源库中相同源的流关闭 */
                    for(auto iter = vaudiosrc_.begin(); iter != vaudiosrc_.end(); ) {
                        if((*iter)->url_ == wndinfo->second.url){
                            currentlyss->audiomixchn_ = (*iter)->audiomixchn_;
                            currentlyss->b_audio_ = true; // 在视频源库中打开该流的音频
                           printf("Close AS URL: %s\n", (*iter)->url_.c_str());
                            delete (*iter);
                            iter = vaudiosrc_.erase(iter);
                            break;
                        }
                        else
                            ++iter;
                    }

                    /* 3. 解析指令中携带的串流协议,成功后执行打开串流地址的操作 */
                    string protocol = wndinfo->second.url.substr(0, wndinfo->second.url.find(":"));
                    if ( protocol == "rtsp" || protocol == "udp"){
                        currentlyss->protocol_ =  ((protocol == "udp") ? "multicast" : protocol);

                        //如果流地址不相同,包括上一次为空
                        if(wndinfo->second.url != currentlyss->url_){
                           printf("url is diff(\"%s\" -> \"%s\")", currentlyss->url_.c_str(), wndinfo->second.url.c_str());
                            currentlyss->b_video_ = true;
                            currentlyss->PlayNetStream(wndinfo->second.url.c_str());
                            currentlyss->url_ = wndinfo->second.url;
                            if(wndinfo->second.signal_id.find("IPC") != string::npos){
                                HasIPC = true;
                            }
                        }
                    }
                    else{
                        printf("signals->protocol(%s)(url:%s) Invalid format", protocol.c_str(), wndinfo->second.url.c_str());
                        continue;
                    }
                }

                /* 4. 打开对应的窗口 */
                currentlyss->OpenWindow(wndinfo->second.layer, wndinfo->second.stSizeRect, 0);
                currentlyss->OpenWindow(wndinfo->second.layer, wndinfo->second.stCropRect);
#ifdef _DEBUG_SCODE_
               printf("[Layer:%d] %s | %d | (%d, %d, %d, %d) | (%d, %d, %d, %d)",
                      wndinfo->second.layer, currentlyss->url_.c_str(), wndinfo->second.vdchn,
                      wndinfo->second.stSizeRect.s32X,     wndinfo->second.stSizeRect.s32Y,
                      wndinfo->second.stSizeRect.u32Width, wndinfo->second.stSizeRect.u32Height,
                      wndinfo->second.stCropRect.s32X,     wndinfo->second.stCropRect.s32Y,
                      wndinfo->second.stCropRect.u32Width, wndinfo->second.stCropRect.u32Height);
#endif
            }
        }

        /** NOTE: 还原并保存命令 **/
        string mbody;
        for(uint i = 0; i < valuelist.size(); ++i){
            scode_->AddItem(mbody, valuelist[i].c_str());
        }

        scode_->CreateSCODeStr(mbody);
        windowssocde_ = mbody;
        this->mediasaver_->newcmd_cnt_++;
        this->mediasaver_->setScode(windowssocde_, MediaSCODeSave::VIDEO);

        this->b_newcmd_ = true;

        if(HasIPC){ //NOTE: IPC加长锁屏时间
            UDPSocket *udpsocket = new UDPSocket();
            udpsocket->CreateUDPClient("224.168.1.1", 41234, false);
            udpsocket->AddMemberShip("224.168.1.1", gSysEthUser.c_str());
            udpsocket->SetRevBuf(32);
            string cmd = "{SetLockFrame;" + num2str(vwallcfg_.panel_sn_) + ";120}";
            udpsocket->SendTo(cmd.c_str(), cmd.size());
            delete udpsocket; udpsocket = NULL;
        }

        this->DebugPrint();
    }
    else if(vwallcfg_.panel_sn_ == 100){
        if(access("./lib", F_OK) == -1 &&
           access("/home/QT_LIB/lib", F_OK) == -1){
            UDPSocket *udpsocket = new UDPSocket();
            udpsocket->CreateUDPClient("127.0.0.1", 41237, false);
            udpsocket->SetRevBuf(32);
            char cmd[10];
            memset(cmd, 0, sizeof(cmd));
            sprintf(cmd,"Panel:%s", valuelist[SCODePANEL].c_str());
            udpsocket->SendTo(cmd, strlen(cmd));

            delete udpsocket; udpsocket = NULL;
        }
    }

    return true;
}

/** NOTE: {GetAudios} ：用于从某个输出节点获取当前该节点的音频信号配置信息，只能单播
客户端→设备：
{
GetAudios;     //命令，不能省略
panel_sn;      //指定视频墙，不能省略
IP,Port        //应答发送的目的地址和端口，可以省略，默认为原路返回
}

设备→客户端：
{
GetAudios;          //命令，不能省略
panel_sn;           //视频墙，不能省略
url1,url2,url3...   //音频信号源的流URL地址
}
**/
bool CommandMap_SCODe::GetAudiosHandler(vstring valuelist)
{
    if(str2int(valuelist[SCODePANEL]) == vwallcfg_.panel_sn_){
        string mbody = audiossocde_;
        if(!mbody.empty()){
            mbody.replace(mbody.find("SetAudios"), strlen("SetAudios"), valuelist[SCODeHEAD]);
        }
        else{
            scode_->AddItem(mbody, valuelist[SCODeHEAD].c_str());
            scode_->AddItem(mbody, vwallcfg_.panel_sn_);
            scode_->CreateSCODeStr(mbody);
        }

        this->ACK(mbody.c_str(), valuelist, kGetAudios);
    }

    return true;
}

/** NOTE: {SetAudios} ：用于设定指定视频墙的音频输出信息，支持广播/组播/单播
客户端→设备：
{
SetAudios;          //命令，**不能省略**
panel_sn;           //指定视频墙，**不能省略**
id|url1,id|url2,id|url3...   //音频信号源的流URL地址
}

设备→客户端：无应答
**/
bool CommandMap_SCODe::SetAudiosHandler(vstring valuelist)
{
    if(str2int(valuelist[SCODePANEL]) == vwallcfg_.panel_sn_){
        extern HI_S32 g_s32TotalAudioMixChn;
        if(valuelist.size() == 2){ /* NOTE: empty all audios */
            /** NOTE: 关闭音频混音输出 **/
            for(auto ss : signalsrcmap_){
                if(ss.second->b_audio_){
                    ss.second->audiomixchn_ = -1;
                    ss.second->b_audio_ = false;
                    ss.second->b_mute_  = true;
                }
            }
            for(auto iter = vaudiosrc_.begin(); iter != vaudiosrc_.end(); ) {
                delete (*iter);
                iter = vaudiosrc_.erase(iter);
            }

            extern AUDIO_MIX_S g_stAudioMix[AUDIOMIX_MAXCHN];
            g_s32TotalAudioMixChn = -1;
            memset( g_stAudioMix, 0, sizeof(g_stAudioMix));
        }
        else{
            g_s32TotalAudioMixChn = -1;
            int index = 2;
            uint totalchn = 0;

            /** NOTE: 先关闭已有音频信号源 **/
            for(auto ss : signalsrcmap_){
                if(ss.second->b_audio_){
                    ss.second->audiomixchn_ = -1;
                    ss.second->b_audio_ = false;
                    ss.second->b_mute_  = true;
                }
            }
            for(auto as : vaudiosrc_){
                as->audiomixchn_ = -1;
                as->b_audio_ = false;
            }

            vstring value = scode_->GetSCODeValues(valuelist[index]);
            for(auto id_url : value){
                vstring v;
                SplitString(id_url, v, "|");
                if(v.size() != 2 ) {
                    printf(SCODE_FORMAT_ERR);
                    continue;
                }
                uint id = str2int(v[0]);
                string url = v[1];
                /** NOTE: 在视频信号源库中查找是否存在已经连接着的相同 url **/
                bool isFound = false;
                for(auto ss : signalsrcmap_){
                    if(ss.second->url_ == url){
                        isFound = true;
                        ss.second->audiomixchn_ = id;
                        ss.second->b_audio_ = true;
                       printf("found it in the ss! id[%d]: [%s]", id, url.c_str());
                    }
                }

                /** NOTE: 若未在视频信号源库中找到同源信号，则在音频信号源库中处理 **/
                if(!isFound){
                    /** NOTE: 在音频信号源库中查找是否存在已经连接着的相同 url **/
                    bool isExisted = false;
                    for(auto as : vaudiosrc_){
                        if(as->url_ == url){
                            isExisted = true;
                            as->audiomixchn_ = id;
                            as->b_audio_ = true;
                           printf("found it in the as! id[%d]: [%s]", id, url.c_str());
                            break;
                        }
                    }

                    /** NOTE: 未找到已存在的音源，新建连接 **/
                    if(!isExisted){
                        SignalSource *currentlyas = NULL;
                        currentlyas = new SignalSource(id);
                        vaudiosrc_.push_back(currentlyas);

                        currentlyas->url_ = url;
                        currentlyas->audiomixchn_ = id;
                        currentlyas->b_audio_  = true;
                        if(!currentlyas->url_.compare(0, 7, "rtsp://"))
                            currentlyas->protocol_ = "rtsp";
                        else
                            currentlyas->protocol_ = "audio";
                        currentlyas->PlayNetStream(url.c_str());

                       printf("create new one in the as! id[%d]: [%s]", id, url.c_str());
                    }
                }

                /** NOTE: 音频信号数量 +1 **/
                ++totalchn;
            }

            /** NOTE: 删除上次命令中遗留的多余信号源
             * 因为SCODe协议命令是只支持全量发送的,
             * 所以 vaudiosrc_ 中不应该保留上一次命令的信息
             **/
            for(auto iter = vaudiosrc_.begin(); iter != vaudiosrc_.end(); ) {
                if(!(*iter)->b_audio_){
                   printf("Close AS URL: %s\n", (*iter)->url_.c_str());
                    delete (*iter);
                    iter = vaudiosrc_.erase(iter);
                }
                else
                    ++iter;
            }
            g_s32TotalAudioMixChn = totalchn > AUDIOMIX_MAXCHN ? AUDIOMIX_MAXCHN : totalchn;
           printf("Total Chn: %d | %d(as)", g_s32TotalAudioMixChn, vaudiosrc_.size());
        }

        /** NOTE: 还原并保存命令 **/
        string mbody;
        for(uint i = 0; i < valuelist.size(); ++i){
            scode_->AddItem(mbody, valuelist[i].c_str());
        }

        scode_->CreateSCODeStr(mbody);
        if(valuelist.size() == 2){ /* NOTE: empty all audios */
            audiossocde_.clear();
        }
        else
            audiossocde_ = mbody;
        this->mediasaver_->newcmd_cnt_++;
        this->mediasaver_->setScode(audiossocde_, MediaSCODeSave::AUDIO);

        this->DebugPrint();
    }

    return true;
}

/** NOTE: {GetAoData;reply_time;IP,Port} ：用于获取音频音量值，支持广播/组播/单播
客户端→设备：
{
GetAoData;	//命令，不能省略
reply_time; //自动回复间隔，单位ms，值为零，表示关闭自动回复
IP,Port		//应答发送的目的地址和端口，可以省略，默认为原路返回
}

设备→客户端：
{
GetAoData;	//命令，不能省略
aodata      //左右声道音频值和
}
**/
bool CommandMap_SCODe::GetAoDataHandler(vstring valuelist)
{
    int index = 1;
    string reply_time = valuelist[index];
    if(reply_time == "0"){ //NOTE: 关闭自动回复
        replyao_.reply_time = 0;
        is_replyao_ = false;
        if(replyao_tid_ != 0){
            pthread_join(replyao_tid_, NULL);
        }
        replyao_tid_ = 0;
    }
    else{ //NOTE: 打开自动回复
        if(replyao_tid_ == 0) { //非分离模式
            replyao_.reply_time = str2int(reply_time);
            snprintf(replyao_.ip, SZ_IPADDR, "%s", gClientIP);
            replyao_.port = gClientPort;
            if(valuelist.size() == 3){
                if(scode_->isArray(valuelist[2])){
                    vstring value = scode_->GetSCODeValues(valuelist[2]); //NOTE: Format: IP,Port
                    if(value.size() == 2){
                        snprintf(replyao_.ip, SZ_IPADDR, "%s", value[0].c_str());
                        replyao_.port = str2int(value[1]);
                    }
                }
            }
            replyao_tid_ = CreateThread(AutoReplyAoData, 99, SCHED_FIFO, false, this);
        }
    }

    return true;
}

/** NOTE: {GetAoMixWeight;IP,Port} ：用于获取音频混音各个通道的权重值和音量，只能单播
客户端→设备：
{
GetAoMixWeight;//命令，不能省略
IP,Port        //应答发送的目的地址和端口，可以省略，默认为原路返回
}

设备→客户端：
{
GetAoMixWeight;    //命令，不能省略
0:w0,1:w1,...,7:w7 // 各个通道的权重值。可以省略掉不需要设置的通道权重值，但是该数组中至少需要保证有一个值元素。
}
**/
bool CommandMap_SCODe::GetAoMixWeightHandler(vstring valuelist)
{
    extern vector<float> g_vAudioMixWeight;

    string mbody;
    scode_->AddItem(mbody, "SetAoMixWeight");

    string item;
    scode_->AddValueToItem(item, string("-1:" + num2str(volume_)).c_str());

    for(uint i = 0; i < g_vAudioMixWeight.size(); ++i){
        scode_->AddValueToItem(item, string(num2str(i) + ":" + num2str(volume_)).c_str());
    }
    scode_->AddItem(mbody, item.c_str());

    scode_->CreateSCODeStr(mbody);
    this->ACK(mbody.c_str(), valuelist, kGetAoMixWeight);

    return true;
}

/** NOTE: {SetAoMixWeight} ：用于设置音频混音各个通道的权重值和音量，只能单播
客户端→设备：
{
SetAoMixWeight;                 //命令，不能省略
-1:volume,0:w0,1:w1,...,7:w7    // 各个通道的权重值。可以省略掉不需要设置的通道权重值，但是该数组中至少需要保证有一个值元素。-1通道号表示调节输出节点的输出音量值。
}
设备→客户端： 无
**/
bool CommandMap_SCODe::SetAoMixWeightHandler(vstring valuelist)
{
    extern vector<float> g_vAudioMixWeight;

    vstring value = scode_->GetSCODeValues(valuelist[1]);
    for(auto iter : value){
        vstring v;
        SplitString(iter, v, ":");
        if(v.size() == 2){
            int chn = str2int(v[0]);
            if(chn >= 0 && chn < AUDIOMIX_MAXCHN){
                double w = str2num<double>(v[1]);
                if(w < 0)
                    g_vAudioMixWeight[chn] = 0;
                else if(w > 1)
                    g_vAudioMixWeight[chn] = 1;
                else
                    g_vAudioMixWeight[chn] = w;
                elog_raw("[%d] %f | ", chn, g_vAudioMixWeight[chn]);
            }
            else if(chn == -1){ // NOTE: -1通道号表示调节输出节点的输出音量值
                int valume = str2int(v[1]);
                if(valume >= -121 && valume <= 6){
                    volume_ = valume;
                    elog_raw("[%d] %d | ", chn, volume_);
                    Singleton<HimppMaster>::getInstance()->SetAudioVolume(volume_);
                }
                else
                    printf("audio volume Invalid params.Volume:[-121, 6]");
            }
        }
    }
    elog_raw("\n");

    return true;
}

/** NOTE: {SaveAoMixWeight} ：用于保存音频混音各个通道的权重值和音量，支持组播/单播
客户端→设备：
{
SaveAoMixWeight	//命令，不能省略
}
设备→客户端： 无
**/
bool CommandMap_SCODe::SaveAoMixWeightHandler(vstring valuelist)
{
    extern vector<float> g_vAudioMixWeight;

    string mbody;
    scode_->AddItem(mbody, "SetAoMixWeight");

    string item;
    scode_->AddValueToItem(item, string("-1:" + num2str(volume_)).c_str());

    for(uint i = 0; i < g_vAudioMixWeight.size(); ++i){
        scode_->AddValueToItem(item, string(num2str(i) + ":" + num2str(g_vAudioMixWeight[i])).c_str());
    }
    scode_->AddItem(mbody, item.c_str());

    scode_->CreateSCODeStr(mbody);
    if(!WriteSthFile(aomixweightfile_, mbody.c_str())){
        printf(SCODE_WRITE_ERR, aomixweightfile_);
    }

    return true;
}

/** NOTE: {GetSceneState} ：用于从某个输出节点获取当前输出节点属于的视频墙的场景编辑状态信息，只能单播
客户端→设备：
{
GetSceneState;	//命令，不能省略
panel_sn;		//指定视频墙，不能省略
IP,Port         //应答发送的目的地址和端口，可以省略，默认为原路返回
}
设备→客户端：
{
GetSceneState；	//命令，不能省略
panel_sn;		//视频墙，不能省略
time            //0：允许编辑，> 0：被占用，自动解除锁定的剩余时间（单位秒）
}
**/
bool CommandMap_SCODe::GetSceneStateHandler(vstring valuelist)
{
    if(str2int(valuelist[SCODePANEL]) == vwallcfg_.panel_sn_){
        int remaining_time = 0;
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        remaining_time = (this->scenestate_.ts.tv_sec + this->scenestate_.state) - ts.tv_sec;
        if(remaining_time < 0 ) remaining_time = 0;

        string mbody;
        scode_->AddItem(mbody, valuelist[SCODeHEAD].c_str());
        scode_->AddItem(mbody, vwallcfg_.panel_sn_);
        scode_->AddItem(mbody, remaining_time);

        scode_->CreateSCODeStr(mbody);
        this->ACK(mbody.c_str(), valuelist, kGetSceneState);
    }
    return true;
}

/** NOTE: {SetSceneState} ：用于设定指定视频墙的场景编辑状态信息，支持广播/组播/单播
客户端→设备：
{
SetSceneState;	//命令，不能省略
panel_sn;		//指定视频墙，不能省略
time            //0：允许编辑，> 0：被占用，不允许编辑，设定时间后自动解除锁定（单位秒）
}

设备→客户端：无应答
**/
bool CommandMap_SCODe::SetSceneStateHandler(vstring valuelist)
{
    if(str2int(valuelist[SCODePANEL]) == vwallcfg_.panel_sn_){
        this->scenestate_.state = str2int(valuelist[2]);
        clock_gettime(CLOCK_MONOTONIC, &this->scenestate_.ts);
    }

    return true;
}

/** NOTE: {GetScenes} ：用于从某个输出节点获取当前输出节点属于的视频墙的实时场景信息，只能单播
客户端→设备：
{
GetScenes;	//命令，不能省略
panel_sn;   //指定视频墙，不能省略
IP,Port		//应答发送的目的地址和端口，可以省略，默认为原路返回
}
设备→客户端：
{
GetScenes;      //命令，不能省略
panel_sn;      //指定视频墙，不能省略
scene_id,name,cmd;	//对应的场景序号和名称
scene_id,name,cmd;	//对应的场景序号和名称
...;                //对应的场景序号和名称
}
**/
bool CommandMap_SCODe::GetScenesHandler(vstring valuelist)
{
    if(str2int(valuelist[SCODePANEL]) == vwallcfg_.panel_sn_){
        string mbody;

        scode_->AddItem(mbody, valuelist[SCODeHEAD].c_str());
        scode_->AddItem(mbody, vwallcfg_.panel_sn_);

        for (auto iter = scenesmap_.begin(); iter != scenesmap_.end(); iter++){
            string item;
            scode_->AddValueToItem(item, (iter->first).c_str());
            scode_->AddValueToItem(item, iter->second.name.c_str());
            scode_->AddValueToItem(item, iter->second.cmd.c_str());
            scode_->AddItem(mbody, item.c_str());
        }

        scode_->CreateSCODeStr(mbody);
        this->ACK(mbody.c_str(), valuelist, kGetSceneNames);
    }

    return true;
}

/** NOTE: {GetScene} ：用于从某个输出节点获取当前输出节点属于的视频墙的实时场景信息，只能单播
客户端→设备：
{
GetScene;	//命令，不能省略
panel_sn;   //指定视频墙，不能省略
scene_id;
IP,Port		//应答发送的目的地址和端口，可以省略，默认为原路返回
}
设备→客户端：
{
GetScene;      //命令，不能省略
panel_sn;      //指定视频墙，不能省略
scene_id,name,cmd;	//对应的场景序号和名称
}
**/
bool CommandMap_SCODe::GetSceneHandler(vstring valuelist)
{
    if(str2int(valuelist[SCODePANEL]) == vwallcfg_.panel_sn_){
        string mbody;
        scode_->AddItem(mbody, valuelist[SCODeHEAD].c_str());
        scode_->AddItem(mbody, vwallcfg_.panel_sn_);

        string item;
        scode_->AddValueToItem(item, valuelist[2].c_str());
        auto ret = scenesmap_.find(valuelist[2]);
        if(ret != scenesmap_.end()){
            scode_->AddValueToItem(item, ret->second.name.c_str());
            scode_->AddValueToItem(item, ret->second.cmd.c_str());
        }
        scode_->AddItem(mbody, item.c_str());

        mbody = "{" + mbody + "}";
        this->ACK(mbody.c_str(), valuelist, kGetScene);
    }

    return true;
}

/** NOTE: {GetSceneNames} ：用于从某个输出节点获取当前输出节点属于的视频墙的实时场景信息，只能单播
客户端→设备：
{
GetSceneNames; //命令，不能省略
panel_sn;   //指定视频墙，不能省略
IP,Port		//应答发送的目的地址和端口，可以省略，默认为原路返回
}
设备→客户端：
{
GetSceneNames;  //命令，不能省略
panel_sn;		//视频墙，不能省略
scene_id,name;	//对应的场景序号和名称
……
scene_id,name;	//对应的场景名称
}
**/
bool CommandMap_SCODe::GetSceneNamesHandler(vstring valuelist)
{
    if(str2int(valuelist[SCODePANEL]) == vwallcfg_.panel_sn_){
        string mbody;

        scode_->AddItem(mbody, valuelist[SCODeHEAD].c_str());
        scode_->AddItem(mbody, vwallcfg_.panel_sn_);

        for (auto iter = scenesmap_.begin(); iter != scenesmap_.end(); iter++){
            string item;
            scode_->AddValueToItem(item, (iter->first).c_str());
            scode_->AddValueToItem(item, iter->second.name.c_str());
            scode_->AddItem(mbody, item.c_str());
        }

        scode_->CreateSCODeStr(mbody);
        this->ACK(mbody.c_str(), valuelist, kGetSceneNames);
    }

    return true;
}

/** NOTE: {SaveScene} ：用于保存指定视频墙的场景，支持广播/组播/单播
客户端→设备：
{
SaveScene;      //命令，不能省略
panel_sn;		//指定视频墙，不能省略
scene_id,name	//场景序号和名称，不能省略
}
设备→客户端：无应答
**/
bool CommandMap_SCODe::SaveSceneHandler(vstring valuelist)
{
    int index = 0;
    if(str2int(valuelist[SCODePANEL]) == vwallcfg_.panel_sn_ &&
       this->isSceneEditable()){
        index = 2;
        if(scode_->isArray(valuelist[index])){
            //NOTE: Format: scene_id,name	//场景序号和名称
            vstring value = scode_->GetSCODeValues(valuelist[index]);
            if(value.size() == 2){
                string id = value[0];
                string name = value[1];

                SCENE_INFO_S sceneinfo = {name, windowssocde_};

               printf("%s | %s | %s", id.c_str(), sceneinfo.name.c_str(), sceneinfo.cmd.c_str());
                auto iter = scenesmap_.find(id);
                if(iter == scenesmap_.end()){
                    scenesmap_.insert(make_pair(id, sceneinfo));
                }
                else{
                    scenesmap_[id] = sceneinfo;
                }

                this->SaveSceneMap();
#ifdef _DEBUG_SCODE_
                //this->ViewSceneMap();
#endif // _DEBUG_SCODE_
            }
        }
        else{
            printf(SCODE_FORMAT_ERR);
            return false;
        }
    }
    else if(vwallcfg_.panel_sn_ == 100){
        if(access("./lib", F_OK) == -1 &&
           access("/home/QT_LIB/lib", F_OK) == -1){
            UDPSocket *udpsocket = new UDPSocket();
            udpsocket->CreateUDPClient("127.0.0.1", 41237, false);
            udpsocket->SetRevBuf(32);
            char cmd[10];
            memset(cmd, 0, sizeof(cmd));
            sprintf(cmd,"Scene:%s", valuelist[SCODePANEL].c_str());
            udpsocket->SendTo(cmd, strlen(cmd));

            delete udpsocket; udpsocket = NULL;
        }
    }

    return true;
}

/** NOTE: {DeleteScene} ：用于删除指定视频墙的场景，支持广播/组播/单播
客户端→设备：
{
DeleteScene;	//命令，不能省略
panel_sn;		//指定视频墙，不能省略
scene_id		//场景序号，不能省略
}
设备→客户端：无应答
**/
bool CommandMap_SCODe::DeleteSceneHandler(vstring valuelist)
{
    int index = 0;
    if(str2int(valuelist[SCODePANEL]) == vwallcfg_.panel_sn_ &&
       this->isSceneEditable()){
        index = 2;
        vstring value = scode_->GetSCODeValues(valuelist[index]);
        for(auto id : value){
            auto iter = scenesmap_.find(id); //vochn表示图层优先级
            if(iter != scenesmap_.end()){
                scenesmap_.erase(iter);

                this->SaveSceneMap();
            }
            else{
                log_w("The scene(%s) was not found", id.c_str());
            }
        }
    }
    else if(vwallcfg_.panel_sn_ == 100){
        if(access("./lib", F_OK) == -1 &&
           access("/home/QT_LIB/lib", F_OK) == -1){
            UDPSocket *udpsocket = new UDPSocket();
            udpsocket->CreateUDPClient("127.0.0.1", 41237, false);
            udpsocket->SetRevBuf(32);
            char cmd[10];
            memset(cmd, 0, sizeof(cmd));
            sprintf(cmd,"Scene:%s", valuelist[SCODePANEL].c_str());
            udpsocket->SendTo(cmd, strlen(cmd));

            delete udpsocket; udpsocket = NULL;
        }
    }
    return true;
}

/** NOTE: {ClearScenes} ：用于删除指定视频墙的场景，支持广播/组播/单播
客户端→设备：
{
ClearScenes;	//命令，不能省略
panel_sn		//指定视频墙，不能省略
}
设备→客户端：无应答
**/
bool CommandMap_SCODe::ClearScenesHandler(vstring valuelist)
{
    if(str2int(valuelist[SCODePANEL]) == vwallcfg_.panel_sn_ &&
       this->isSceneEditable()){
        scenesmap_.erase(scenesmap_.begin(), scenesmap_.end());

        this->SaveSceneMap();
    }

    return true;
}

/** NOTE: {CallScene} ：用于调取指定视频墙的场景，支持广播/组播/单播
客户端→设备：
{
CallScene;		//命令，不能省略
panel_sn;		//指定视频墙，不能省略
scene_id		//场景序号，不能省略
}
设备→客户端：无应答
**/
bool CommandMap_SCODe::CallSceneHandler(vstring valuelist)
{
    int index = 0;
    if(str2int(valuelist[SCODePANEL]) == vwallcfg_.panel_sn_){
        index = 2;
        string id = valuelist[index];

        auto iter = scenesmap_.find(id);
        if(iter != scenesmap_.end()){
            vstring itemlist = scode_->GetSCODeItems(iter->second.cmd);
            if(itemlist.size() > 0)
                this->SetWindowsHandler(itemlist);
        }
        else{
            log_w("The scene(%s) was not found", id.c_str());
        }
    }
    else if(vwallcfg_.panel_sn_ == 100){
        if(access("./lib", F_OK) == -1 &&
           access("/home/QT_LIB/lib", F_OK) == -1){
            UDPSocket *udpsocket = new UDPSocket();
            udpsocket->CreateUDPClient("127.0.0.1", 41237, false);
            udpsocket->SetRevBuf(32);
            char cmd[10];
            memset(cmd, 0, sizeof(cmd));
            sprintf(cmd,"Panel:%s", valuelist[SCODePANEL].c_str());
            udpsocket->SendTo(cmd, strlen(cmd));

            delete udpsocket; udpsocket = NULL;
        }
    }

    return true;
}

/** NOTE: {SetScenesPolling} ：用于设置场景轮询,只支持单播,同一组视频墙,要且仅要设定一个节点.
客户端→设备：
{
SetScenesPolling; //命令，**不能省略**
panel_sn;         //视频墙，不能省略
polling_time;	  //轮询间隔,单位秒, 值为零,代表关闭轮询
scene_id1,scene_id2,....,scene_idn      //参与轮询的场景id列表,列表顺序决定了场景轮询顺序
}

设备→客户端：无应答
**/
bool CommandMap_SCODe::SetScenesPollingHandler(vstring valuelist)
{
    int index = 0;
    if(str2int(valuelist[SCODePANEL]) == vwallcfg_.panel_sn_){
        index = 2;
        string polling_time = valuelist[index];
        this->SaveCommand(valuelist[SCODeHEAD], valuelist);

        if(polling_time == "0"){ //TODO: 关闭轮询
            vstring v;
            v.push_back("PauseScenesPolling");
            v.push_back(valuelist[SCODePANEL]);
            v.push_back("0");
            this->PauseScenesPollingHandler(v);
            is_polling_ = false;
            if(polling_tid_ != 0){
                pthread_join(polling_tid_, NULL);
            }
            polling_tid_ = 0;
        }
        else{
            index = 3;
            if(valuelist.size() == 4 && scode_->isArray(valuelist[index])){
                //TODO: 打开轮询
                if(polling_tid_ == 0 && gMCtrllProtocol == kSCODe) //非分离模式
                    polling_tid_ = CreateThread(AutoPollingScenes, 99, SCHED_FIFO, false, this);
                else
                    printf("Scene polling has been started. If it is repeated, close the current polling first");
            }
            else{
                printf(SCODE_FORMAT_ERR);
                return false;
            }
        }
    }

    return true;
}

/** NOTE: {PauseScenesPolling;panel_sn;stat} ：场景轮询暂停与继续
客户端→设备：
{
PauseScenesPolling;	//命令，不能省略
panel_sn;           //指定视频墙，不能省略
stat                //场景轮询暂停/继续的状态，1为暂停，0为继续
}
**/
bool CommandMap_SCODe::PauseScenesPollingHandler(vstring valuelist)
{
    if(str2int(valuelist[SCODePANEL]) == vwallcfg_.panel_sn_){
        int state = str2int(valuelist[2]);
        if(state == 0){
            spolpause_ = 0;
        }
        else if(state == 1){
            spolpause_ = 1;
        }

        this->SaveCommand(valuelist[SCODeHEAD], valuelist);
    }

    return true;
}

/** NOTE: {SetVideoWallCfg} ：用于设定该组视频墙的配置信息，只能单播
客户端→设备：
{
SetVideoWallCfg;    //命令，**不能省略**
panel_sn;           //设定视频墙，**不能省略**
VideoWall_Width,videowall_Height;                       //视频墙总尺寸,**不能省略**
NodeRect_X,NodeRect_Y,NodeRect_Width,NodeRect_Height    //该节点所处视频墙的区域,**不能省略**
}

设备→客户端：无应答
**/
bool CommandMap_SCODe::SetVideoWallHandler(vstring valuelist)
{
    int index = 0;
    if(valuelist.size() == 4){
        index = SCODePANEL;
        vwallcfg_.panel_sn_ = str2int(valuelist[index]);

        index = 2;
        if(scode_->isArray(valuelist[index])){
           vstring value = scode_->GetSCODeValues(valuelist[index]);
           if(value.size() != 2){ // VideoWall_Width,videowall_Height
               printf(SCODE_FORMAT_ERR);
           }
           else{
               vwallcfg_.stVWallSize.u32Width  = str2int(value[0]);
               vwallcfg_.stVWallSize.u32Height = str2int(value[1]);
           }
        }
        else{
            printf(SCODE_FORMAT_ERR);
        }

        index = 3;
        if(scode_->isArray(valuelist[index])){
           vstring value = scode_->GetSCODeValues(valuelist[index]);
           if(value.size() != 4){ // NodeRect_X,NodeRect_Y,NodeRect_Width,NodeRect_Height
               printf(SCODE_FORMAT_ERR);
           }
           else{
               vwallcfg_.stNodeRect.s32X = str2int(value[0]);
               vwallcfg_.stNodeRect.s32Y = str2int(value[1]);
               vwallcfg_.stNodeRect.u32Width  = str2int(value[2]);
               vwallcfg_.stNodeRect.u32Height = str2int(value[3]);
           }
        }
        else{
            printf(SCODE_FORMAT_ERR);
        }

        this->SaveCommand(valuelist[SCODeHEAD], valuelist);
    }
    else{
        printf(SCODE_FORMAT_ERR);
    }

    return true;
}
#endif
/** NOTE: {UploadFile} ：用于上传文件，支持广播/组播/单播
客户端→设备：
{
UploadFile;     //命令，**不能省略**
panel_sn;       //指定视频墙，**不能省略**
filename;       //文件名称
recv_ip,recv_prot,recv_len //指定接收地址和端口和每次接收长度,接收地址为0.0.0.0或0,则表示只监听端口即可
IP,Port		//应答发送的目的地址和端口，可以省略，默认为原路返回
}

设备→客户端：
{
UploadFile;	 //命令，不能省略
panel_sn;	 //视频墙，不能省略
0/非0        //0表示上传成功; 非0表示上传出错
}
**/
bool CommandMap_SCODe::UploadFileHandler(vstring valuelist)
{
     printf("\n-----------upload--------\n");
    int ret = 1;
    int index = 0;

    index = 2;
    string filename = storagedatafold_;
    filename += "/" + valuelist[index];

    index = 3;
    if(scode_->isArray(valuelist[index])){
        vstring value = scode_->GetSCODeValues(valuelist[index]);
        //NOTE: Format: recv_ip,recv_prot,recv_len
        if(value.size() == 3){
            string mucip = value[0];
            int  port = str2int(value[1]);
            int  maxlex = str2int(value[2]);
            char data[maxlex];
   printf("\n-----222-------\n");
            UDPSocket *udpsocket = new UDPSocket();
            int s = udpsocket->CreateUDPServer(port, false); //设为阻塞
            if(mucip != "0" && mucip != "0.0.0.0")
                udpsocket->AddMemberShip(mucip.c_str(), ETH0);

            /* 设置阻塞超时 */
            struct timeval timeout;
            timeout.tv_sec  = 1;
            timeout.tv_usec = 0;
            if (setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0)
                perror("socket time out set failed\n");

            int  recv_len = 0;
            uint total_pack = 0;
            uint curr_pack  = 0;
            map<int, string> content;
            while(1){
                memset(data, '\0', sizeof(data));
                if( (recv_len = udpsocket->RecvFrom(data, maxlex)) > 0){
                    FILE_HEADER_S *file_header = (FILE_HEADER_S*)data;
                    total_pack = file_header->total_pack;
                    curr_pack  = file_header->curr_pack;
                    string s;
                    s.resize(recv_len - sizeof(FILE_HEADER_S));
                    for(int i = sizeof(FILE_HEADER_S), j = 0; i < recv_len; ++i, ++j) {
                        s[j] = data[i];
                    }

                    auto iter = content.find(curr_pack);
                    if(iter == content.end()){
                        //NOTE: map会自动按照key的升序排列
                        content.insert(make_pair(curr_pack, s));
                    }
                    else{
                        content[curr_pack] = s;
                    }

                    if(content.size() >= total_pack){
                        ret = 0;
                        break;
                    }
                }
                else
                    break;
            }
            delete udpsocket; udpsocket = NULL;

#ifdef _DEBUG_SCODE_
                   printf("T:%02d | C:%02d | cnt:%02d", total_pack, curr_pack, content.size());
#endif // _DEBUG_SCODE_

            if(!ret){
                stringstream ss;
                for(auto iter = content.begin(); iter != content.end(); iter++) {
                    ss << iter->second;
                }
               printf("%s", filename.c_str());
                ofstream out_file(filename.c_str());
                if(out_file && out_file.is_open()){
                    out_file << ss.str();

//                    if(vwallcfg_.panel_sn_ == 100){
//                        if(access("./lib", F_OK) == -1 &&
//                            access("/home/QT_LIB/lib", F_OK) == -1){
//                            UDPSocket *udpsocket = new UDPSocket();
//                            udpsocket->CreateUDPClient("127.0.0.1", 41237, false);
//                            udpsocket->SendTo("UploadFile", 10);

//                            delete udpsocket; udpsocket = NULL;
//                        }
//                    }
                }
                else {
                    printf(SCODE_WRITE_ERR, filename.c_str());
                }
            }
        }
        else{
            printf(SCODE_FORMAT_ERR);
        }
    }
    else{
        printf(SCODE_FORMAT_ERR);
    }

    string mbody;
    scode_->AddItem(mbody, valuelist[SCODeHEAD].c_str());
//    scode_->AddItem(mbody, vwallcfg_.panel_sn_);
    scode_->AddItem(mbody, ret);
    scode_->CreateSCODeStr(mbody);
    this->ACK(mbody.c_str(), valuelist, kUploadFile);

    return true;
}

/** NOTE: {DownloadFile} ：用于下载文件，只支持单播
客户端→设备：
{
DownloadFile;   //命令，**不能省略**
panel_sn;       //指定视频墙，**不能省略**
filename;       //文件名称
send_ip,send_prot,send_len,send_interval;   //指定发送地址和端口和每次发送长度和发包间隔(us),发送地址和端口必须合法
package_sn,package_sn,...,package_sn        //指定的发送包
}

设备→客户端：无应答
**/
bool CommandMap_SCODe::DownloadFileHandler(vstring valuelist)
{
    printf("\n-----------download--------\n");
    int index = 0;

    index = 2;
    string filename = storagedatafold_;
    filename += "/" + valuelist[index];
   printf("filename: %s", filename.c_str());
    uint N = 0;
    bool isSpecified = false;
    unsigned short replySN[65536];

    index = 3;
    if(scode_->isArray(valuelist[index])){
        vstring value = scode_->GetSCODeValues(valuelist[index]);
        //NOTE: Format: send_ip,send_prot,send_len
        if(value.size() == 3 || value.size() == 4){
            string ip = value[0];
            int  port = str2int(value[1]);
            int  sendlen = str2int(value[2]) - sizeof(FILE_HEADER_S);
            long long  interval = 10000; // 10000 nsec = 10 us
            if(sendlen <= 0){
                printf("The send_len must not be <= 8");
                return false;
            }
            if(value.size() == 4){
                interval = str2num<long long>(value[3]) * 1000;
            }

            if(valuelist.size() == 5){
                index = 4;
                vstring v = scode_->GetSCODeValues(valuelist[index]);
                N = min((int)v.size(), 65536);
                for(uint i = 0; i < N; ++i){
                    replySN[i] = str2int(v[i]);
                }
                isSpecified = true;
            }
            UDPSocket *udpsocket = new UDPSocket();
            udpsocket->CreateUDPClient(ip.c_str(), port);
            udpsocket->SetRevBuf(32);

            string content = ReadSthFile(filename.c_str(), false);
            int total_len = content.size();
           printf("file len: %d", total_len);

            uint n = 0;
            char *packet = new char[sendlen + sizeof(FILE_HEADER_S) + 1];
            FILE_HEADER_S *header = (FILE_HEADER_S *)packet;
            header->total_pack = (total_len + sendlen - 1) / sendlen;  //总包数
            header->curr_pack  = 0;
            while(total_len > 0){
                if(isSpecified){
                    if(replySN[n] == header->curr_pack){
                        memcpy(packet + sizeof(FILE_HEADER_S),
                               content.c_str() + (header->curr_pack * sendlen),
                               min(sendlen, total_len));
                        if( udpsocket->SendTo(packet, min(sendlen, total_len) + sizeof(FILE_HEADER_S)) > 0){
                            header->curr_pack++;
                            total_len -= sendlen;

                            FreezeInSitu(interval);
                        }

                        ++n;
                        if(n >= N)
                            break;
                    }
                    else{
                        header->curr_pack++;
                        total_len -= sendlen;
                    }
                }
                else{
                    memcpy(packet + sizeof(FILE_HEADER_S),
                           content.c_str() + (header->curr_pack * sendlen), min(sendlen, total_len));
                    if( udpsocket->SendTo(packet, min(sendlen, total_len) + sizeof(FILE_HEADER_S)) > 0){
                        header->curr_pack++;
                        total_len -= sendlen;

                        FreezeInSitu(interval);
                    }
                    else
                        break;
                }
            }
#ifdef _DEBUG_SCODE_
                   printf("T:%02d | C:%02d | N:%02d", header->total_pack, header->curr_pack, N);
#endif // _DEBUG_SCODE_
            delete []packet; packet = NULL;
            delete udpsocket; udpsocket = NULL;
        }
        else{
            printf(SCODE_FORMAT_ERR);
            return false;
        }
    }
    else{
        printf(SCODE_FORMAT_ERR);
        return false;
    }

    return true;
}

/** NOTE: {DeleteFiles} ：用于删除客户端希望存储于节点上面信息,支持广播/组播/单播
客户端→设备：
{
DeleteFiles  //命令，**不能省略**
}

设备→客户端：无应答
**/
bool CommandMap_SCODe::DeleteFilesHandler(vstring valuelist)
{
    char cmd[512];
    sprintf(cmd, "rm %s/* -rf", storagedatafold_);
    string ret;
    LinuxShell(cmd, ret);

    return true;
}

/** NOTE: {GetClientInfo} ：用于获取客户端希望存储于节点上面信息,只支持单播
客户端→设备：
{
GetClientInfo;  //命令，**不能省略**
client_id;      //客户端id
IP,Port         //应答发送的目的地址和端口，可以省略，默认为原路返回
}

设备→客户端：
{
GetClientInfo;  //命令，**不能省略**
dataXXXXX;      //客户端id 对应的客户端希望存储于节点上的内容,内容不限,长度限制为4M,内容不可以包含;分号
**/
bool CommandMap_SCODe::GetClientInfoHandler(vstring valuelist)
{
    string mbody;

    scode_->AddItem(mbody, valuelist[SCODeHEAD].c_str());
    string id = valuelist[1];
    auto iter = clientinfomap_.find(id);
    if(iter != clientinfomap_.end()){
        scode_->AddItem(mbody, (iter->second).c_str());
    }

    scode_->CreateSCODeStr(mbody);
    this->ACK(mbody.c_str(), valuelist, kGetClientInfo);

    return true;
}

/** NOTE: {SaveClientInfo} ：用于存储客户端希望存储于节点上面信息,支持广播/组播/单播
客户端→设备：
{
SaveClientInfo;         //命令，**不能省略**
client_id,dataXXXXX;    //客户端id 和 客户端希望存储于节点上的内容,内容不限,长度限制为4M,内容不可以包含;分号，如果省略，则表示将该配置清空
}

设备→客户端：无应答
**/
bool CommandMap_SCODe::SaveClientInfoHandler(vstring valuelist)
{
    if(valuelist.size() == 1){
        for(auto iter = clientinfomap_.begin(); iter != clientinfomap_.end(); ){
            clientinfomap_.erase(iter++);
        }
        stringstream ss;
        if(!WriteSthFile(clientinfofile_, ss.str().c_str())){
            printf(SCODE_WRITE_ERR, clientinfofile_);
        }
    }
    else{
        uint pos = valuelist[1].find(SCODe_VALUE_BREAK);
        if(pos != string::npos){
            string id = valuelist[1].substr(0, pos);
            string data = valuelist[1].substr(pos+strlen(SCODe_VALUE_BREAK));

            if(valuelist.size() > 2){
                for(uint i = 2; i < valuelist.size(); ++i){
                    data += SCODe_ITEM_BREAK + valuelist[i];
                }
            }
            cout << "ID: " + id + " | DATA: " + data << endl;

           printf("%s | data: %s", id.c_str(), data.c_str());

            auto iter = clientinfomap_.find(id);
            if(iter == clientinfomap_.end()){
                clientinfomap_.insert(make_pair(id, data));
            }
            else{
                clientinfomap_[id] = data;
            }

            stringstream ss;
            for (auto iter = clientinfomap_.begin(); iter != clientinfomap_.end(); iter++){
                ss << iter->first << " | " << iter->second << endl;
            }
            if(!WriteSthFile(clientinfofile_, ss.str().c_str())){
                printf(SCODE_WRITE_ERR, clientinfofile_);
            }
        }
        else{
            printf(SCODE_FORMAT_ERR, clientinfofile_);
        }
    }

    return true;
}
#if 0
/** NOTE: {SaveMedia} ：用于立即存储media.scode,支持广播/组播/单播
客户端→设备：
{
SaveMedia;         //命令，**不能省略**
panel_sn      //指定视频墙，不能省略
}

设备→客户端：无应答
**/
bool CommandMap_SCODe::SaveMediaHandler(vstring valuelist)
{
    if(str2int(valuelist[SCODePANEL]) == vwallcfg_.panel_sn_){
        this->mediasaver_->setScode(windowssocde_, MediaSCODeSave::VIDEO);
        this->mediasaver_->setScode(audiossocde_, MediaSCODeSave::AUDIO);
        this->mediasaver_->SaveSCODe();
    }

    return true;
}

/** NOTE: {Connect} ：用于连接输入（KVM矩阵切换指令），支持广播/组播/单播
客户端→设备：
{
Connect;         //命令，**不能省略**
inputnode_ipaddr //输入节点的ip，可省略，省略表示断开链接
}

设备→客户端：无应答

说明
1. inputnode_ipaddr为思朗264/265系列的输入节点的ip
2. 该命令的功能为自动连接输入信号的音视频，并全屏显示，自动连接KVM
**/
bool CommandMap_SCODe::ConnectHandler(vstring valuelist)
{
    string mbody;
    scode_->AddItem(mbody, "SetWindows");
    scode_->AddItem(mbody, vwallcfg_.panel_sn_);

//    // NOTE: 从开窗配置的socde命令串中获取上一次连接的输入ip地址，根据Connect协议规定，只需要获取第一个输入ip
//    if(!windowssocde_.empty()){
//        vstring itemlist = scode_->GetSCODeItems(windowssocde_);
//        if(itemlist.size() >= 3){
//            int index = 2;
//            vstring value = scode_->GetSCODeValues(itemlist[index]);
//            if(value.size() < kMAX){ // ip,url,x,y,w,h
//                printf("SCODe CMD Format(Quantity) Error!(Usage(\"ip,url,x,y,w,h\"):%s)", valuelist[index].c_str());
//                last_input_.clear();
//            }
//            else
//                last_input_ = value[kIP];
//        }
//        else
//            last_input_.clear();
//    }
//    else
//        last_input_.clear();

    if(valuelist.size() == 1){
        scode_->CreateSCODeStr(mbody);
        vstring itemlist = scode_->GetSCODeItems(mbody);
        if(itemlist.size() > 0)
            this->ProcessCmd(itemlist[0], itemlist);

        UDPSocket *udpsocket = new UDPSocket();
        udpsocket->CreateUDPClient("127.0.0.1", 41236, false);
        udpsocket->SetRevBuf(32);

        string cmd;

        // NOTE: 为了跳开RTSP，将输入组播设置为单播使用
//            if(!last_input_.empty()){
//                for(int i = 0; i < 3; ++i){
//                    cmd = "{\"Type\":\"System\",\"Function\":\"setMulticast\",\"Value\":{\"channel\":\"chn0\",\"name\":\"main\",\"mcast_enable\":0}}";
//                    udpsocket->SendTo(last_input_.c_str(), 12345, cmd.c_str(), cmd.size());

//                    cmd = "{\"Type\":\"System\",\"Function\":\"setAuMulticast\",\"Value\":{\"name\":\"hdmi0\",\"mcast_enable\":0}}";
//                    udpsocket->SendTo(valuelist[1].c_str(), 12345, cmd.c_str(), cmd.size());
//                    cmd = "{\"Type\":\"System\",\"Function\":\"setAuMulticast\",\"Value\":{\"name\":\"3.5mm\",\"mcast_enable\":0}}";
//                    udpsocket->SendTo(valuelist[1].c_str(), 12345, cmd.c_str(), cmd.size());
//                }
//            }
//            last_input_.clear();

        auto sw_config = Singleton<SoftwareConfig>::getInstance();
        string local_ipaddr = sw_config->GetConfig(SoftwareConfig::kEth0Ip);
        cmd = "details&" + local_ipaddr + "#0,0,0.0.0.0";
        udpsocket->SendTo(cmd.c_str(), cmd.size());

        delete udpsocket; udpsocket = NULL;
    }
    else if(valuelist.size() == 2){
        string item;
        scode_->AddValueToItem(item, valuelist[1].c_str());
        scode_->AddValueToItem(item, string("rtsp://" + valuelist[1] + "/0").c_str());
//        scode_->AddValueToItem(item, string("udp://" + valuelist[1] + ":53124").c_str());
        scode_->AddValueToItem(item, vwallcfg_.stNodeRect.s32X);
        scode_->AddValueToItem(item, vwallcfg_.stNodeRect.s32Y);
        scode_->AddValueToItem(item, vwallcfg_.stNodeRect.u32Width);
        scode_->AddValueToItem(item, vwallcfg_.stNodeRect.u32Height);
        scode_->AddItem(mbody, item.c_str());
        scode_->CreateSCODeStr(mbody);

        vstring itemlist = scode_->GetSCODeItems(mbody);
        if(itemlist.size() > 0)
            this->ProcessCmd(itemlist[0], itemlist);

        UDPSocket *udpsocket = new UDPSocket();
        udpsocket->CreateUDPClient("127.0.0.1", 41236, false);
        udpsocket->SetRevBuf(32);

        string cmd;
        auto sw_config = Singleton<SoftwareConfig>::getInstance();
        string local_ipaddr = sw_config->GetConfig(SoftwareConfig::kEth0Ip);

        // NOTE: 为了跳开RTSP，将输入组播设置为单播使用
//            if(!last_input_.empty()){
//                for(int i = 0; i < 3; ++i){
//                    cmd = "{\"Type\":\"System\",\"Function\":\"setMulticast\",\"Value\":{\"channel\":\"chn0\",\"name\":\"main\",\"mcast_enable\":0}}";
//                    udpsocket->SendTo(last_input_.c_str(), 12345, cmd.c_str(), cmd.size());

//                    cmd = "{\"Type\":\"System\",\"Function\":\"setAuMulticast\",\"Value\":{\"name\":\"hdmi0\",\"mcast_enable\":0}}";
//                    udpsocket->SendTo(valuelist[1].c_str(), 12345, cmd.c_str(), cmd.size());
//                    cmd = "{\"Type\":\"System\",\"Function\":\"setAuMulticast\",\"Value\":{\"name\":\"3.5mm\",\"mcast_enable\":0}}";
//                    udpsocket->SendTo(valuelist[1].c_str(), 12345, cmd.c_str(), cmd.size());
//                }
//            }

//            cmd = "{\"Type\":\"System\",\"Function\":\"setMulticast\",\"Value\":{\"channel\":\"chn0\",\"name\":\"main\",\"mcast_enable\":1,\"mcast_ip\":\"" + local_ipaddr + "\",\"mcast_port\":53124}}";
//            udpsocket->SendTo(valuelist[1].c_str(), 12345, cmd.c_str(), cmd.size());

//            cmd = "{\"Type\":\"System\",\"Function\":\"setAuMulticast\",\"Value\":{\"name\":\"hdmi0\",\"mcast_enable\":1,\"mcast_ip\":\"" + local_ipaddr + "\",\"mcast_port\":53124}}";
//            udpsocket->SendTo(valuelist[1].c_str(), 12345, cmd.c_str(), cmd.size());
//            cmd = "{\"Type\":\"System\",\"Function\":\"setAuMulticast\",\"Value\":{\"name\":\"3.5mm\",\"mcast_enable\":1,\"mcast_ip\":\"" + local_ipaddr + "\",\"mcast_port\":53124}}";
//            udpsocket->SendTo(valuelist[1].c_str(), 12345, cmd.c_str(), cmd.size());


        cmd = "relation&0,0," + local_ipaddr;
        udpsocket->SendTo(cmd.c_str(), cmd.size());

        cmd = "details&" + local_ipaddr + "#0,0," + valuelist[1];
        udpsocket->SendTo(cmd.c_str(), cmd.size());

        delete udpsocket; udpsocket = NULL;
    }
    else{
        printf(SCODE_FORMAT_ERR);
    }

    return true;
}

/** NOTE: {GetSignalState} ： 用于获取信号连接状态,支持广播/组播/单播
客户端→设备：
{
GetSignalState; 		  // 命令，**不能省略**
panel_sn;                 // 指定视频墙，不能省略
vdchn1,vdchn2,vdchn3...;  // 解码通道
IP,Port         //应答发送的目的地址和端口，可以省略，默认为原路返回
}

设备→客户端：
{
GetSignalState; 		//命令，**不能省略**
panel_sn;               //指定视频墙，不能省略
vdchn1State,vdchn2State,vdchn3State...; // 解码通道
}
 */
extern int l_vdecFrame[VD_CHN_MAX];
bool CommandMap_SCODe::GetSignalsState(vstring valuelist)
{
    if(str2int(valuelist[SCODePANEL]) == vwallcfg_.panel_sn_){
        string mbody;
        scode_->AddItem(mbody, valuelist[SCODeHEAD].c_str());
        scode_->AddItem(mbody, vwallcfg_.panel_sn_);

        string item;
        vstring value = scode_->GetSCODeValues(valuelist[2]);
        for(const auto &v : value){
            int vdchn = str2int(v);
            if(vdchn < 16)
                scode_->AddValueToItem(item, l_vdecFrame[vdchn]);
            else
                scode_->AddValueToItem(item, "ERR");
        }

        scode_->AddItem(mbody, item.c_str());
        scode_->CreateSCODeStr(mbody);
        this->ACK(mbody.c_str(), valuelist, kGetSignalsState);
    }

    return true;
}

/** NOTE: {SubtitleOsd} ：用于设置输出节点跑马灯的显示，但是跑马灯所需的YUV图片需要由上位机生成并上传，支持广播/组播/单播
客户端→设备：
{
SubtitleOsd;            //命令，不能省略
panel_sn;               //指定视频墙，不能省略
osd_start_x,osd_start_y,osd_high;	//osd显示的起始x坐标，范围为[0,视频墙总宽度]；osd显示的起始y坐标，范围为[0,单个输出节点输出分辨率的总高度-osd_high]；osd的总高度，范围为[0,200]
isTransparent,background_color,cycle,direction	//osd背景是否透明，取值{0,1}；osd背景色，格式为 #FF00AA；osd运动轮询时间，单位秒，其值为0，表示静止跑马灯显示；osd运动方向，取值{"left","right"}
}
设备→客户端： 无应答
**/
bool CommandMap_SCODe::SubtitleOsdHandler(vstring valuelist)
{
    if(str2int(valuelist[SCODePANEL]) == vwallcfg_.panel_sn_){
        int osd_start_x = 0, osd_start_y = 0, osd_high = 0, cycle = 0;
        bool isTransparent = false;
        bool enable = false;
        string background_color = "#000000";
        string direction = "left";

        if(valuelist.size() == 2){
            enable = false;
        }
        else if(valuelist.size() == 4){
            enable = true;

            int index = 2;
            if(scode_->isArray(valuelist[index])){
                vstring value = scode_->GetSCODeValues(valuelist[index]);
                if(value.size() == 3){
                    osd_start_x = str2int(value[0]);
                    osd_start_y = str2int(value[1]);
                    osd_high = str2int(value[2]);

                    if(osd_start_x < 0 || osd_start_x > (int)vwallcfg_.stVWallSize.u32Width){
                        osd_start_x = 0;
                        log_w("subtitle osd \"osd_start_x\" overflow.");
                    }
                    if(osd_high < 0 || osd_high > (HI_S32)gOsdMaxHeight){
                        osd_high = gOsdMaxHeight;
                        log_w("subtitle osd \"osd_high\" overflow.");
                    }
                    if(osd_start_y < 0 || osd_start_y > (int)vwallcfg_.stNodeRect.u32Height - osd_high){
                        osd_start_y = 0;
                        log_w("subtitle osd \"osd_start_y\" overflow.");
                    }
                }
                else{
                    printf(SCODE_FORMAT_ERR);
                    return false;
                }
            }
            else{
                printf(SCODE_FORMAT_ERR);
                return false;
            }

            index = 3;
            if(scode_->isArray(valuelist[index])){
                vstring value = scode_->GetSCODeValues(valuelist[index]);
                if(value.size() == 4){
                    isTransparent = (bool)str2int(value[0]);
                    background_color = value[1];
                    cycle = str2int(value[2]);
                    direction = value[3];

                    if(background_color.size() != strlen("#000000") || background_color.find("#") != 0){
                        background_color = "#000000";
                        log_w("subtitle osd \"background_color\" overflow.");
                    }
                    if(direction != "left" && direction != "right"){
                        direction = "left";
                        log_w("subtitle osd \"direction\" overflow.");
                    }
                }
                else{
                    printf(SCODE_FORMAT_ERR);
                    return false;
                }
            }
            else{
                printf(SCODE_FORMAT_ERR);
                return false;
            }
        }
        else{
            printf(SCODE_FORMAT_ERR);
            return false;
        }

        string ret;
        LinuxShell("ln -s /data/server_data/storage/osd1.yuv ./osd1.yuv -f", ret);
        LinuxShell("ln -s /data/server_data/storage/osd2.yuv ./osd2.yuv -f", ret);

        //创建转发socket
        UDPSocket *udp = new UDPSocket();
        udp->CreateUDPClient("127.0.0.1", 41234); //设为非阻塞
        udp->SetRevBuf(32);

        //{"Type":"System","Function":"setAdvancedConfig","Value":{"en_title_osd":false}}
        cJSON *value1 = cJSON_CreateObject();
        cJSON_AddTrueToObject(value1,   "en_title_osd");
        cJSON *obj1 = cJSON_CreateObject();
        cJSON_AddStringToObject(obj1, "Type",     "System");
        cJSON_AddStringToObject(obj1, "Function", "setAdvancedConfig");
        cJSON_AddItemToObject(  obj1, "Value",    value1);

        //{"Type":"OSD","Function":"setSubtitleOSD","Value":{"transparent":false,"videowall_id":3,"total_length":1920,"pos_x":0,"pos_y":0,"osd_high":100,"background_color":"#000000","cycle":10,"direction":"left"}}
        cJSON *value2 = cJSON_CreateObject();
        cJSON_AddBoolToObject(value2,   "transparent",      isTransparent);
        cJSON_AddNumberToObject(value2, "videowall_id",     vwallcfg_.panel_sn_);
        cJSON_AddNumberToObject(value2, "total_length",     vwallcfg_.stVWallSize.u32Width);
        cJSON_AddNumberToObject(value2, "pos_x",            vwallcfg_.stNodeRect.s32X);
        cJSON_AddNumberToObject(value2, "pos_y",            osd_start_y);
        cJSON_AddNumberToObject(value2, "osd_high",         osd_high);
        cJSON_AddStringToObject(value2, "background_color", background_color.c_str());
        cJSON_AddNumberToObject(value2, "cycle",            cycle);
        cJSON_AddStringToObject(value2, "direction",        direction.c_str());
        cJSON *obj2 = cJSON_CreateObject();
        cJSON_AddStringToObject(obj2, "Type",     "OSD");
        cJSON_AddStringToObject(obj2, "Function", "setSubtitleOSD");
        cJSON_AddItemToObject(  obj2, "Value",    value2);

        //{"Type":"OSD","Function":"ctrlSubtitleOSD","Value":{"enable":true,"text_x_start":500,"move":true}}
        cJSON *value3 = cJSON_CreateObject();
        cJSON_AddBoolToObject(value3,   "enable",       enable);
        cJSON_AddNumberToObject(value3, "text_x_start", osd_start_x);
        if(cycle <= 0)
            cJSON_AddFalseToObject(value3, "move");
        else
            cJSON_AddTrueToObject(value3, "move");
        cJSON *obj3 = cJSON_CreateObject();
        cJSON_AddStringToObject(obj3,   "Type",     "OSD");
        cJSON_AddStringToObject(obj3,   "Function", "ctrlSubtitleOSD");
        cJSON_AddItemToObject(  obj3,   "Value",     value3);

        cJSON *root = cJSON_CreateArray();
        cJSON_AddItemToArray(root,  obj1);
        cJSON_AddItemToArray(root,  obj2);
        cJSON_AddItemToArray(root,  obj3);

        char *s = cJSON_PrintUnformatted(root);
        if(s){
            udp->SendTo(s, strlen(s));
            cJSON_free(s);
        }
        cJSON_Delete(root);

        delete udp; udp = NULL;
    }

    return true;
}
#endif
void CommandMap_SCODe::ACK(const char *msg, vstring valuelist, uint default_item_num)
{
    if(valuelist.size() == default_item_num){
        this->LetMeTellYou(msg);
//         printf("\n-----22----\n");
    }
    else if(valuelist.size() == (default_item_num+1)){
        int hostaddr_pos = default_item_num;
        if(scode_->isArray(valuelist[hostaddr_pos])){
            vstring value = scode_->GetSCODeValues(valuelist[hostaddr_pos]); //NOTE: Format: IP,Port
            if(value.size() == 2){
                this->LetMeTellYou(msg, value[0].c_str(), str2int(value[1]));
            }
            else{
                printf(SCODE_FORMAT_ERR);
            }
        }
        else{
            printf(SCODE_FORMAT_ERR);
        }
    }
    else{
        printf(SCODE_FORMAT_ERR);
    }
}

bool CommandMap_SCODe::LetMeTellYou(const char *msg, const char *ip, const int port)
{
#ifdef _DEBUG_SCODE_
       printf("\"msg:%s\" Sendto (%s:%d)", msg, ip, port);
#endif // _DEBUG_SCODE_
 //   printf("\nletmetellu\n");
//    printf("\"msg:%s\" Sendto (%s:%d)", msg, ip, port);
    if((port != 0) &&
       (strlen(ip) != 0)){
        if( echoudp_->SendTo(ip, port, msg, strlen(msg)) < 0 ){
            printf("echoudp sendto error: %s", strerror(errno));
            return false;
        }
    }

    return true;
}

bool CommandMap_SCODe::isItemNumLegal(const int size, const int standard_size)
{
    if(size < standard_size){
        printf(SCODE_FORMAT_ERR);
        return false;
    }
    return true;
}
#if 0
bool CommandMap_SCODe::isSceneEditable()
{
    if(scenestate_.state != -1){
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        if((ts.tv_sec - scenestate_.ts.tv_sec) < (scenestate_.state + EDIT_LOCK)){
            printf(SCODE_OPERATE_ERR);
            return false;
        }
        else{
            scenestate_.state = -1;
            scenestate_.ts.tv_sec  = 0;
            scenestate_.ts.tv_nsec = 0;
        }
    }

    return true;
}

void CommandMap_SCODe::ViewSceneMap()
{
    elog_raw("SceneID \tSceneName \tCMD\n");
    for (auto iter = scenesmap_.begin(); iter != scenesmap_.end(); iter++){
        elog_raw("%s \t%s \t%s\n", (iter->first).c_str(), iter->second.name.c_str(), iter->second.cmd.c_str());
    }
}

void CommandMap_SCODe::SaveSceneMap()
{
    stringstream ss;
    for (auto iter = scenesmap_.begin(); iter != scenesmap_.end(); iter++){
        ss << iter->first << " | " << iter->second.name << " | " << iter->second.cmd << endl;
    }
    if(!WriteSthFile(scenesmapfile_, ss.str().c_str())){
        printf(SCODE_WRITE_ERR, scenesmapfile_);
    }
}

void CommandMap_SCODe::LoadSceneMap()
{
    scenesmap_.erase(scenesmap_.begin(), scenesmap_.end());
    string s = ReadSthFile(scenesmapfile_);
    if(s != "null"){
        vstring scenelist;
        SplitString(s, scenelist, "\n");
        for(uint i = 0; i < scenelist.size(); ++i){
            vstring itemlist;
            SplitString(scenelist[i], itemlist, " | ");
            if(itemlist.size() == 3){
                cout << itemlist[0] << itemlist[1] << itemlist[2] << endl;
                vstring list = scode_->GetSCODeItems(itemlist[2]);
                if(list.size() >= kSetWindows &&
                   str2int(list[1].c_str()) == this->vwallcfg_.panel_sn_){
                    SCENE_INFO_S sceneinfo = {itemlist[1], itemlist[2]};
                    scenesmap_.insert(make_pair(itemlist[0], sceneinfo));
                }
            }
            else{
                printf(SCODE_FORMAT_ERR);
            }
        }

        this->SaveSceneMap();
    }
#ifdef _DEBUG_SCODE_
    //this->ViewSceneMap();
#endif // _DEBUG_SCODE_
}
#endif
void CommandMap_SCODe::LoadClientInfoMap()
{
    clientinfomap_.erase(clientinfomap_.begin(), clientinfomap_.end());
    string s = ReadSthFile(clientinfofile_);
    if(s != "null"){
        vstring infolist;
        SplitString(s, infolist, "\n");
        for(uint i = 0; i < infolist.size(); ++i){
            vstring itemlist;
            SplitString(infolist[i], itemlist, " | ");
            if(itemlist.size() == 2){
                clientinfomap_.insert(make_pair(itemlist[0], itemlist[1]));
            }
            else{
                printf(SCODE_FORMAT_ERR);
            }
        }
    }
}

bool CommandMap_SCODe::LoadConfig(const char* file)
{
    string s = ReadSthFile(file);
    if(s != "null"){
        vstring scodelist;
        SplitString(s, scodelist, "\n");
        for(uint i = 0; i < scodelist.size(); ++i){
            if(scode_->CheckSCODeFormat(scodelist[i])){
                vstring itemlist = scode_->GetSCODeItems(scodelist[i]);
                if(itemlist.size() > 0)
                    this->ProcessCmd(itemlist[0], itemlist);
            }

#ifdef _DEBUG_SCODE_
           printf("%s", scodelist[i].c_str());
#endif // _DEBUG_SCODE_
        }
    }

    return true;
}

void CommandMap_SCODe::FreezeInSitu(long long nsec)
{
    struct timespec ts1;
    struct timespec ts2 = {0, 0};
    clock_gettime(CLOCK_MONOTONIC, &ts1);
    do{
        clock_gettime(CLOCK_MONOTONIC, &ts2);
    }while( (ts2.tv_sec*1000000000+ts2.tv_nsec) -
            (ts1.tv_sec*1000000000+ts1.tv_nsec) < nsec);
}
#if 0
void *CommandMap_SCODe::AutoPollingScenes(void *arg)
{
    CommandMap_SCODe *p = (CommandMap_SCODe *)arg;
    UDPSocket *ms = new UDPSocket();
    ms->CreateUDPClient("224.168.1.1", 41234, true);
    ms->SetRevBuf(32);
    ms->AddMemberShip("224.168.1.1", gSysEthUser.c_str());
    ms->BindLocalAddr(gSysEthUser.c_str());

    string mbody = ReadSthFile(p->scenespollingfile_);
    if(!p->scode_->CheckSCODeFormat(mbody)) return (void*)0;
    vstring itemlist = p->scode_->GetSCODeItems(mbody);
    vstring sceneslist;
    int polling_time = 0, time_cnt = 0;
    if(itemlist.size() > kSetScenesPolling && p->scode_->isArray(itemlist[3])){
        polling_time = str2int(itemlist[2]);
        sceneslist = p->scode_->GetSCODeValues(itemlist[3]);
        time_cnt = polling_time;
        if(polling_time <= 0)
            return (void*)0;
    }
    else{
        printf(SCODE_FORMAT_ERR);
        return (void*)0;
    }

    INTERRUPT_DATA_S kodata = {0,0,0};
    int irqfd = open("/dev/interrupt2", 0); //阻塞读取； NOTE: 非阻塞 O_RDONLY|O_NONBLOCK
    if (irqfd < 0){
        printf("open %s error: %s.", "/dev/interrupt2", strerror(errno));
        return (void*)0;
    }

    int scenes_num = 0;
    p->is_polling_ = true;
    while(p->is_polling_){
        scenes_num = 0;
        for(uint i = 0; i < sceneslist.size(); ){
            read(irqfd, &kodata, sizeof(kodata));
            if(!p->is_polling_) {
               printf("Stop AutoPollingScenes!");
                return (void*)0;
            }

            if(p->spolpause_ == 1){
                time_cnt = -1;
            }
            else if(kodata.frame_num == 0){
                ++time_cnt;
            }

            if(time_cnt >= polling_time){
                auto iter = p->scenesmap_.find(sceneslist[i].c_str());
                if(iter != p->scenesmap_.end()){ //NOTE: 查找到该场景存在的情况下...
                    time_cnt = 0;
                    ++scenes_num;
                    mbody.clear();
                    p->scode_->AddItem(mbody, (&EnumtoStr(kCallScene)[1]));
                    p->scode_->AddItem(mbody, p->vwallcfg_.panel_sn_);
                    p->scode_->AddItem(mbody, sceneslist[i].c_str());

                    p->scode_->CreateSCODeStr(mbody);
                    ms->SendTo(mbody.c_str(), mbody.size());
                   printf("%s", mbody.c_str());
                }
                ++i;
            }
        }

        if(scenes_num <= 1) break; //NOTE: 参与场景轮询的场景数量不足以启动轮询时退出
    }

    delete ms;
    ms = NULL;
   printf("Stop AutoPollingScenes!");

    return (void*)0;
}

void *CommandMap_SCODe::DeleteSignalSource(void *arg)
{
    SignalSource *ss = (SignalSource*)arg;
    delete ss;

    return (void*)0;
}

void *CommandMap_SCODe::AutoReplyAoData(void *arg)
{
    CommandMap_SCODe *p = (CommandMap_SCODe*)arg;
    UDPSocket *ms = new UDPSocket();
    ms->CreateUDPClient(p->replyao_.ip, p->replyao_.port, true);
    ms->SetRevBuf(32);
    ms->BindLocalAddr(gSysEthUser.c_str());
    HI_U32 u32VoDevFrameRate;
    HI_U32 reply_step = 1, time_cnt = 0;

    int ttl = gMltTTL;
    ms->SetMulticastTTL(ttl);
    //ms->SetIpTTL(ttl);

    HI_MPI_VO_GetDevFrameRate(VO_DEV_DHD, &u32VoDevFrameRate);
    u32VoDevFrameRate = ((u32VoDevFrameRate < 45) ? 30 : 60);
    reply_step = (p->replyao_.reply_time / (1000.0/u32VoDevFrameRate)) + 1;
   printf("reply_step: %d", reply_step);

    INTERRUPT_DATA_S kodata = {0,0,0};
    int irqfd = open("/dev/interrupt3", 0); //阻塞读取； NOTE: 非阻塞 O_RDONLY|O_NONBLOCK
    if (irqfd < 0){
        printf("open %s error: %s.", "/dev/interrupt3", strerror(errno));
        return (void*)0;
    }

    p->is_replyao_ = true;
    while(p->is_replyao_){
        read(irqfd, &kodata, sizeof(kodata));
        if(!p->is_replyao_) {
           printf("Stop AutoReplyAoData!");
            return (void*)0;
        }

        ++time_cnt;

        if(time_cnt >= reply_step){
            string mbody;
            p->scode_->AddItem(mbody, "GetAoData");
            string item;
            int aodata = (abs(gAoData + gAoData) >> 7) * (p->volume_ + 121); //NOTE: 右移7防止溢出
            p->scode_->AddValueToItem(item, aodata);
            p->scode_->AddItem(mbody, item.c_str());

            p->scode_->CreateSCODeStr(mbody);
            ms->SendTo(mbody.c_str(), mbody.size());

            time_cnt = 0;
        }
    }

    delete ms;
    ms = NULL;
   printf("Stop AutoReplyAoData!");

    return (void*)0;
}
#endif
