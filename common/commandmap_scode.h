#ifndef COMMANDMAP_SCODE_H
#define COMMANDMAP_SCODE_H

#include <cJSON/cJSON.h>  //cJSON库，标准C编写，函数多以“cJSON_”开头
#include <cSCODe/SCODe.h>
#include <common.h>//#include "signalsource.h"
#include <udpsocket.h>
#include "software_config.h"
#include "our_md5.h"
#include <singleton.h>
#include <map>
//#include <unordered_map>
using namespace std;

#define MEDIA_SCODE     "media.scode"
#define SERVER_DATA     "server_data"
#define STORAGE_DATA    "storage"
#define VWALL_CFG       "config_videowall.scode"
#define HOST_CFG        "config_host.cfg"
#define CLIENT_INFO     "client.info"
#define PIC_DATA        "pic.dat"
#define SCENES_MAP      "scenes.map"
#define SCENES_POLLING  "scenes.polling"
#define SPOL_PAUSE      "spol.pause"
#define AUDIOMIX_WEIGHT "audiomix.weight"

#define BASE_LOCKFRAME (11) //锁屏时间基数
#define EDIT_LOCK   (30)    //本地增加的场景编解锁定时长,单位秒

#define _DEBUG_SCODE_
#define SCODE_INVALID_HEAD  "(%s)Invalid SCODe CMD Header!"
#define SCODE_FORMAT_ERR    "SCODe CMD Format Error!"
#define SCODE_CREATE_ERR    "Create SCODe Format Error!"
#define SCODE_OPERATE_ERR   "Operate Error!"
#define SCODE_WRITE_ERR     "Write Error!(%s)"

/** NOTE: 特别说明
 * @brief: 这个枚举变量中的枚举名称将会用于反算出SCODe协议的命令头
 * @example: #define EnumtoStr(val) #val //把枚举变量的名称. 返回类型为const char[]
 * @example: EnumtoStr(kGetNodes)+1 = "GetNodes"
 **/
typedef enum DEFAULT_ITEM_NUM_E{
    kGetNodes       = 1,
    kGetNodes2      = 1,
//    kSetLockFrame   = 2,
//    kGetTransmit    = 2,
//    kSetTransmit    = 2,
//    kGetWindows     = 2,
//    kSetWindows     = 2,
//    kGetAudios      = 2,
//    kSetAudios      = 2,
//    kGetAoData      = 2,
//    kGetAoMixWeight = 1,
//    kSetAoMixWeight = 2,
//    kSaveAoMixWeight= 1,
//    kGetSceneState  = 2,
//    kSetSceneState  = 3,
//    kGetScene       = 3,
//    kGetScenes      = 2,
//    kGetSceneNames  = 2,
//    kSaveScene      = 3,
//    kDeleteScene    = 3,
//    kClearScenes    = 2,
//    kCallScene      = 3,
//    kSetScenesPolling   = 3,
//    kPauseScenesPolling = 3,
//    kSetVideoWall   = 4,
    kUploadFile     = 4,
    kDownloadFile   = 4,
    kDeleteFiles    = 1,
//    kSaveClientInfo = 1,
    kGetClientInfo  = 2,
//    kSaveMedia      = 2,
//    kConnect        = 1,
//    kGetSignalsState= 3,
//    kSubtitleOsd    = 2,
}DEFAULT_ITEM_NUM_E; //NOTE: 每条Get相关的指令的默认Item个数
class CommandMap_SCODe;
//class MediaSCODeSave;
typedef bool (CommandMap_SCODe::*pScodeFun)(vstring valuelist);
typedef struct ScodeHandler {
    DEFAULT_ITEM_NUM_E  def_num;
    pScodeFun           pfun;
}ScodeHandler;
typedef map<string, ScodeHandler> SCODE_HANDLER_MAP;

class CommandMap_SCODe
{
public:
    typedef enum WND_CMD_ARRAY_E{
        kIP = 0,
        kURL,
        kX,
        kY,
        kW,
        kH,
        kMAX,
    }WND_CMD_ARRAY_E;
    typedef struct FILE_HEADER_S{
        uint  total_pack;
        uint  curr_pack;
    }FILE_HEADER_S;
//    typedef struct VIDEOWALL_CFG_S{
//        int  panel_sn_; //视频墙序号
//        SIZE_S stVWallSize;  //视频墙总尺寸
//        RECT_S stNodeRect;   //该节点所处视频墙的区域
//    }VIDEOWALL_CFG_S;
//    typedef struct LAYER_INFO_S {
//        string  signal_id;
//        string  url;
//        RECT_S  stRect;   //该节点所处视频墙的区域
//    }LAYER_INFO_S;
    typedef struct VDCHN_STATUS_S {
        bool is_empty;
        int  vdchn;
        string url;
    }VDCHN_STATUS_S;
    typedef struct SCENE_STATUS_S {
        int    state;
        struct timespec ts;
    }SCENE_STATUS_S;
    typedef struct SCENE_INFO_S {
        string  name;
        string  cmd;
    }SCENE_INFO_S;
    typedef struct REPLY_AODATA_S {
        char  ip[SZ_IPADDR];
        uint  port;
        int   reply_time;
    }REPLY_AODATA_S;
    typedef map<string, SCENE_INFO_S> SCENE_INFO_MAP;
    typedef map<string, string> CLIENT_INFO_MAP;

    CommandMap_SCODe();
    ~CommandMap_SCODe();

    bool LoadInitialMediaSCODe();
//    void SaveCommand(string scode_head, vstring valuelist);
//    void DebugPrint();

    // 主函数
    bool ProcessCmd(string key, vstring valuelist);

    bool GetNodesHandler(vstring valuelist);
    bool GetNodes2Handler(vstring valuelist);

//    bool SetLockFrameHandler(vstring valuelist);
//    bool GetTransmitHandler(vstring valuelist);
//    bool SetTransmitHandler(vstring valuelist);
//    bool GetWindowsHandler(vstring valuelist);
//    bool SetWindowsHandler(vstring valuelist);

//    bool GetAudiosHandler(vstring valuelist);
//    bool SetAudiosHandler(vstring valuelist);
//    bool GetAoDataHandler(vstring valuelist);
//    bool GetAoMixWeightHandler(vstring valuelist);
//    bool SetAoMixWeightHandler(vstring valuelist);
//    bool SaveAoMixWeightHandler(vstring valuelist);

//    bool GetSceneStateHandler(vstring valuelist);
//    bool SetSceneStateHandler(vstring valuelist);

//    bool GetScenesHandler(vstring valuelist);
//    bool GetSceneHandler(vstring valuelist);
//    bool GetSceneNamesHandler(vstring valuelist);
//    bool SaveSceneHandler(vstring valuelist);
//    bool DeleteSceneHandler(vstring valuelist);
//    bool ClearScenesHandler(vstring valuelist);
//    bool CallSceneHandler(vstring valuelist);
//    bool SetScenesPollingHandler(vstring valuelist);
//    bool PauseScenesPollingHandler(vstring valuelist);

//    bool SetVideoWallHandler(vstring valuelist);
    bool UploadFileHandler(vstring valuelist);
    bool DownloadFileHandler(vstring valuelist);
    bool DeleteFilesHandler(vstring valuelist);

    bool GetClientInfoHandler(vstring valuelist);
    bool SaveClientInfoHandler(vstring valuelist);
//    bool SaveMediaHandler(vstring valuelist);

//    bool ConnectHandler(vstring valuelist);
//    bool GetSignalsState(vstring valuelist);

//    bool SubtitleOsdHandler(vstring valuelist);

private:
    void ACK(const char *msg, vstring valuelist, uint default_item_num);
    bool LetMeTellYou(const char *msg, const char *ip = gClientIP, const int port = gClientPort);
    inline bool isItemNumLegal(const int size, const int standard_size);
//    bool isSceneEditable();
//    inline void ViewSceneMap();
//    inline void SaveSceneMap();
//    void LoadSceneMap();
    void LoadClientInfoMap();
    bool LoadConfig(const char* file);
    inline void FreezeInSitu(long long nsec); /** NOTE: 使用while循环让程序在原地冻结 N纳秒 **/
//    static void *AutoPollingScenes(void *arg);
//    static void *DeleteSignalSource(void *arg);
//    static void *AutoReplyAoData(void *arg);

public:
    bool b_newcmd_;
    //WND_INFO_MAP wndinfomap_;

private:
    char mediafile_[36];        //存放媒体配置文件的路径
    char serverdatafold_[36];   //存放Server配置文件的文件夹的路径
    char storagedatafold_[46];  //存放上传收到的文件的文件夹的路径
    char vwallcfgfile_[46];     //视频墙配置文件的路径
    char hostcfgfile_[46];      //host机配置文件的路径
    char picdatfile_[46];       //host机图片文件的路径
    char clientinfofile_[46];   //客户端相关信息文件的路径
    char scenesmapfile_[46];    //场景列表文件的路径
    char scenespollingfile_[46];//场景轮询列表文件的路径
    char spolpausefile_[46];    //场景轮询暂停状态文件的路径
    char aomixweightfile_[46];  //混音权重配置文件的路径
    int  spolpause_ = 0;
    int  volume_ = 0;
    string filechecksum_;
    string windowssocde_;
    string audiossocde_;
    string transmitscode_;
    //SSMap signalsrcmap_;
    vector<VDCHN_STATUS_S> vVdChnList_;
//    vector<SignalSource*> vaudiosrc_;
//    vector<SignalSource*> vtranssrc_;
    CLIENT_INFO_MAP clientinfomap_;
    SCENE_INFO_MAP scenesmap_;
    SCENE_STATUS_S scenestate_ = {-1, {0,0}};
//    VIDEOWALL_CFG_S vwallcfg_ = {-1, {0,0}, {0,0,1920,1080}};

    bool is_polling_ = false;
    bool is_replyao_ = false;
    pthread_t polling_tid_ = 0;
    pthread_t replyao_tid_ = 0;
    pthread_t audiomixtid_ = 0;
    REPLY_AODATA_S replyao_;
    SCODE_HANDLER_MAP handlermap_;
//    MediaSCODeSave *mediasaver_;
//    THREAD_STATUS_S rtspcloseTid_[RtspTidCnt];
    SCODe     *scode_ = NULL;
    UDPSocket *echoudp_ = NULL;

//    string last_input_;
};
#if 0
class MediaSCODeSave
{
public:
    enum SCODeSavePos
    {
        VIDEO = 0,
        AUDIO,
        TRANS,
        POS_MAX
    };
    MediaSCODeSave(const char* filename) : filename_(filename)
    {
        vscode_.assign(POS_MAX, "");
        newcmd_cnt_ = -1;
        ttid_run_ = true;
        uint priority = 40; //0~99
        bool b_detached = true; //是否设线程为分离模式
        ttid_ = CreateThread(timer_autosave, priority, SCHED_FIFO, b_detached, this);
    }
    ~MediaSCODeSave(){
        ttid_run_ = false;
    }

    bool SaveSCODe(){
        string s;
        for(uint i = 0; i < vscode_.size(); ++i){
            if( !vscode_[i].empty() ){
               s += vscode_[i] + '\n';
            }
        }
        s = s.substr(0, s.size() - 1); //去掉字符串最后的 '/n'
        if(!WriteSthFile(filename_, s.c_str())){
             printf(SCODE_WRITE_ERR, filename_);
             return false;
         }
        log_i("save media.scode config succeed");
        return true;
    }

    void setScode(const string &scode, const int pos){
        vscode_[pos] = scode;
    }

private:
    static void *timer_autosave( void *arg )
    {
        MediaSCODeSave *p = (MediaSCODeSave *)arg;
        int last_cnt = p->newcmd_cnt_;
        bool b_save = false; //是否需要进行保存

        while( p->ttid_run_ ){
            sleep(2); //NOTE: 强制冷静2秒钟，防止一直有新指令来,导致死循环

            if(last_cnt != p->newcmd_cnt_){ //NOTE: 判断是否有新的指令到来
                last_cnt = p->newcmd_cnt_;
                b_save = true;
            }
            else if(b_save){
                p->SaveSCODe();

                p->newcmd_cnt_ = 0;
                last_cnt = 0;
                b_save = false;
            }
        }

        return (void*)1;
    }

public:
    int newcmd_cnt_;

private:
    pthread_t    ttid_;
    bool  ttid_run_;
    vstring vscode_;
    const char* filename_;

};
#endif
#endif // COMMANDMAP_SCODE_H
