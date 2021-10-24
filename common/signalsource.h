#ifndef SIGNALSOURCE_H
#define SIGNALSOURCE_H

#include <unordered_map>
#include <vector>
#include <mutex>

#include "himpp_master.h"
#include "player_rtsp.h"
//#include "player_pte.h"
#include "player_multicast.h"
#include "player_rtp.h"

#define NEW_CALCULATION
#define FPS_POOL_CNT        60
#define MULTICAST_UDP_LEN   1300
#define RtspTidCnt          32
#define AOADJUST_CNT        (UINT32_MAX >> 10)

using namespace std;

class SignalSource;
typedef map<HI_S32, SignalSource*> SSMap;
typedef map<HI_U32, HI_U32> VO_MAP;  /* 用于存放该信号源对应的vo通道号 <vochn, priority> */
typedef struct AUDIO_INFO_S{
    HI_BOOL bstatus;
    HI_S32 type;
    HI_S32 sample;
    HI_S32 channel;
}AUDIO_INFO_S;

typedef struct THREAD_STATUS_S{
    pthread_t tid;
    bool balive;
}THREAD_STATUS_S;

typedef enum CUT_FORMAT_S{
    kNORMAL     = 0,
    kEXP_LEFT   = 1,
    kEXP_RIGHT  = 2,
    kNEED_EXP   = 3,
}CUT_FORMAT_S;

typedef struct WND_INFO_S{
    CUT_FORMAT_S  format;		//该窗口是否需要format
    HI_U32        layer;
    HI_U32        vdchn;      //该窗口对应的解码通道
    string        signal_id;  //该窗口对应的信号源唯一号
    string        url;
    RECT_S        stCropRect;	//该窗口对应的Crop
    RECT_S        stSizeRect;	//该窗口对应的Size
}WND_INFO_S;
typedef map<uint, WND_INFO_S> WND_INFO_MAP;

class SignalSource : player_media
{
public:
    typedef struct TRANSMIT_INFO_S{
        char protocol[32];
        char ip[SZ_IPADDR];
        unsigned int port;
        long length;
        unsigned int sleeptime; //us
    }TRANSMIT_INFO_S;
    typedef struct VIDEO_CACHE_S{
        unsigned int len;
        char* data;
    }VIDEO_CACHE_S;
    typedef struct AO_ADJUST_S
    {
        HI_U32 counter;
        HI_U32 posover; //正溢出
        HI_U32 negover; //负溢出
        HI_U32 offset;
        struct timeval start;
        struct timeval end;
    }AO_ADJUST_S;

    SignalSource(uint vdchn);
    ~SignalSource();

    virtual void on_video( int video_type, int video_width, int video_heigth, void *data, unsigned int len, unsigned long long pts, unsigned wnd );
    virtual void on_loopvideo( int video_type, int video_width, int video_heigth, void* data, unsigned int len, unsigned int pts, unsigned wnd, bool iframe = false );
    virtual void on_audio( int audio_type, int audio_sample, int audio_channel, void *data, unsigned int len, unsigned long long pts, unsigned wnd );
    virtual void on_loopaudio( int audio_type, int audio_sample, int audio_channel, void *data, unsigned int len, unsigned long long pts, unsigned wnd );

    bool PlayNetStream(const char *url, string protocol = "");
    bool CloseNetStream(string protocol = "");
    bool OpenWindow(HI_U32 u32VoChn, RECT_S stWndRect, HI_U32 u32Priority = 100);
    bool CloseWindow(HI_U32 u32VoChn);
    bool InitTransmit();
    bool DeleteTransmit();

    uint GetVdChn();
    VO_MAP GetVoChnVector();
    AUDIO_INFO_S GetAudioInfo();
#ifdef _VIDEO_CACHE_
    static HI_VOID* VideoCacheDecodeProc(HI_VOID *__this);
#endif

private:
    bool DecodeVideo(void* data, unsigned int len, unsigned int pts, unsigned wnd, bool iframe = false);
    bool DecodeAudio(void* data, unsigned int len, unsigned int pts, int audio_type, int sample, int channel);
    bool TransmitVideo(int video_type, int video_width, int video_heigth, void* data, unsigned int len, unsigned int pts, int frame_rate, int p_sn);
    bool TransmitAudio(int audio_type, int audio_sample, int audio_channel, void* data, unsigned int len, unsigned int pts, unsigned wnd, int p_sn);
    bool AddAdtsHead(char *src, char *dst, unsigned int* len, int sample, int channel);
    bool PushOneFramePCM(unsigned char *data, unsigned int len, unsigned int pts);
#ifdef _VIDEO_CACHE_
    bool VIDEO_CACHE_S(void* data, unsigned int len, unsigned int pts, unsigned wnd);
#endif // _VIDEO_CACHE_
#ifdef _MANUAL_PUSH_VO_
    void AddFrameTab(VIDEO_FRAME_S *pVBuf);
    void SaveYUV2Local(VIDEO_FRAME_S *pVBuf, FILE *pfd);
#endif // _MANUAL_PUSH_VO_

public:
    int  audiomixchn_ = -1;
    bool b_video_;
    bool b_audio_;
    bool b_mute_;
    bool b_transmit_;
    bool b_transloop_;
    string protocol_;
    string url_;
    string transurl_; //用于保存当前转发发送除去的流地址，用于自己接收(loop)时对比新配置使用
    TRANSMIT_INFO_S st_transmit_;

private:
#ifdef _VIDEO_CACHE_
    vector<struct VIDEO_CACHE_S> videocache_;
#endif
    FILE    *fd_ = NULL;
    int     frame_rate_ = 60;
    int     vpackage_sn_ = 0;
    int     apackage_sn_ = 0;
    int     local_tick_ = 0;
    int     video_type_ = -1; /* VIDEO_H264 = 101; VIDEO_H265 = 102 */
    int     i_frame_len_ = 0;
    int     last_gframeID_ = 0;
    int     frame_rate_count_ = 0;
    uint    cnt_[2] = {0, 0};
    uint    vdchn_;
    bool    b_close_; // 是否被关闭
    bool    b_firstfrm_; //第一帧
    bool    is_i_frame_ = false;
    bool    b_fps_pool_[FPS_POOL_CNT];
    char    i_frame_data_[PACK_LEN];
    double  cal_time_ = 0.0;
    unsigned long long last_pts_ = 0;

    mutex       mutex_;
    VO_MAP       tovochn_;
    MyTimer     *mcasttimer_ = NULL;
    UDPSocket   *multicastsocket_ = NULL;
    AO_ADJUST_S    stAoAdjust_ = {0,0,0,0,{0, 0},{0, 0}};
    AUDIO_INFO_S   st_audioinfo_;

    aacdec aac_dec_;
    player_rtsp_sp rtsp_sp_;
//    player_pte_sp  pte_sp_;
    player_multicast_sp multicast_sp_;
    player_rtp_sp rtp_sp_;
};

#endif // SIGNALSOURCE_H
