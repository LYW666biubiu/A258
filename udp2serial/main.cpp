#include "global.h"

#include "rs232.h"
#include "rs485.h"
#include "relay.h"
#include "infrared.h"
#include "io.h"
#include "umpset.h"
#include "io_edc.h"
#include "tcp_server.h"

#include <stdlib.h>
#include <string.h>
#include <cstring>
#include <sys/ipc.h>        /* ftok() etc. */
#include <sys/epoll.h>      /* epoll function */
#include <sys/msg.h>        /* msgget() etc. */
#include <errno.h>

#include <udpsocket.h>
#include <cJSON.h>  //cJSON库，标准C编写，函数多以“cJSON_”开头
#include "version.h"
#include "common_A258.h"
#include <common.h>
#include "commandmap_scode.h"
#include <singleton.h>
//#include "tinyinstruction.h"
//#include "http_handler.h"

#define MULTICAST_CMD_IP "224.168.1.1"  //netserver hong
#define CMD_PORT        (41234)
#define BROAD_SCAN_PORT (10)
#define MULTICAST_SCAN_PORT (3700)
#define MAXEPOLLSIZE    (128)
#define SZ_1K           (1024)
#define MAXDATA_net         SZ_1K
#define NODE_MODEL      "output"
#define CMD_KILL   "[KILL]"           //tiny头文件内容
#define CMD_DELETE "[DELETE]"
#define CMD_RESET  "[RESET]"
#define CMD_HELP   "[HELP]"
#define CMD_REMSGQUEUE  "[REMSGQUEUE]"
#define CMD_SWITCH      "[SWITCH]"
#define CMD_CUSTOMERID  "[CUSTOMERID]"
#define MESSAGE_BUF_MAX (5*1024*1024)
char ticksync[24] = "false";

//char tmpData_tc[MAXDATA] = {0};
int socket232=-1;
int socket485=-1;
int socketIO_A258=-1;
int socketRelay=-1;
struct sockaddr_in ctrlAddr_A258;
struct sockaddr_in ctrlAddr_A258_relay;
struct sockaddr_in ctrlAddr_A258_232;
struct sockaddr_in ctrlAddr_A258_485;

pthread_t tcp_pid = 0;

rs232 *rs232P;
rs232 *rs232bakP;       //教育一体板 双232口
rs2320 *rs2320P;
rs2321 *rs2321P;
rs2322 *rs2322P;
rs2323 *rs2323P;
//rs2321 *rs2321P;
rs485 *rs485P;
rs4850 *rs4850P;
rs4851 *rs4851P;
rs4852 *rs4852P;
rs4853 *rs4853P;
relay *relayP;
relay *relayP0;
relay *relayP1;
infrared *irP;
infrared *irP_tc;
io *ioP0;
io *ioP1;
io *ioP2;
io *ioP3;
io *ioP4;
io *ioP5;
io *ioP6;
io *ioP7;
io_edc *ioP_edc;
umpSet *umpP;

struct SW_VERSION sw_version =
{
    "2.0.1",     //<主版本号>.<次版本号>.<修订版本号>
    //<阶段版本>
#ifndef _TRIAL_
#ifdef __DEBUG__
//    STAGE_BASE
    STAGE_ALPHA
//    STAGE_BETA
//    STAGE_RC
#else
    STAGE_RELEASE
#endif
#else
    STAGE_Trial
#endif // _TRIAL_
};


bool MessageTransmit(const char * const kp_msg, uint ulen, int type, struct sockaddr_in clnt_adr)
{
    msg_st msgdata;

    strncpy(msgdata.mtext.msg, kp_msg, ulen);
    strcpy(msgdata.mtext.ip, inet_ntoa(((struct sockaddr_in*)&clnt_adr)->sin_addr));
    msgdata.mtext.port = ntohs(((struct sockaddr_in*)&clnt_adr)->sin_port);

    struct msqid_ds msg_info;
    if(-1 == msgctl(c_mainmsgid, IPC_STAT, &msg_info)){
        printf("msg get info error: %s", strerror(errno));
        c_mainmsgid = CreateMessageQueue(c_mainmsgpath);
        //SetMessageQueue(c_mainmsgid);
        //return false;
    }
    if(MESSAGE_BUF_MAX - msg_info.msg_cbytes > 3000){
        if(type == c_mainmsgtype){
            msgdata.mtype = c_mainmsgtype; // 给主程序发送消息
            if(msgsnd(c_mainmsgid, &msgdata,
                      ulen + sizeof(uint) + (sizeof(char) * SZ_IPADDR),
                      04000) == -1){
                printf("netsever msgsnd to main error: %s", strerror(errno));
                return false;
            }
        }
    }
    else{
        printf("current number of bytes on queue is %ld", msg_info.msg_cbytes);
        printf("number of messages in queue is %ld", msg_info.msg_qnum);
       printf("max number of bytes on queue is %ld", msg_info.msg_qbytes);
    }

    return true;
}

bool GetJsonValue(const char *content, const char *key, char *value)
{
    cJSON *js_root = cJSON_Parse(content);
    if (js_root == NULL){
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
            printf("Error before: %s", error_ptr);

        return false;
    }

    cJSON *element = cJSON_GetObjectItemCaseSensitive(js_root, key);
    if(element ){
        if(cJSON_IsString(element)){
            //printf("%s: %s", key, element->valuestring);
            sprintf(value, "%s", element->valuestring);
        }
    }
    else{
        cJSON_Delete(js_root);
        return false;
    }

    cJSON_Delete(js_root);
    return true;
}
uint GetSystemInfo(char *buf_send, bool isOldFormat = true)
{
    char localip[SZ_IPADDR] = "";
    char macaddr[SZ_IPADDR] = "";
    uint delay = 0;
    // ¶ÁÈ¡°æ±¾ºÅ
    string ver = ReadSthFile("/version/version");

    // ¶ÁÈ¡uuid
    string uuid = ReadSthFile("/uuid/uuid");

    // ¶ÁÈ¡node name
    string name = ReadSthFile("/data/node_name");

    // »ñÈ¡±¾µØipµØÖ·ºÍmacµØÖ·
    NetworkInfo *netinfo = new NetworkInfo();
    if( !netinfo->GetNetworkInfo(ETH0, NetworkInfo::kADDR, localip) ){
        printf("Get ip failed!");
        sprintf(localip, "%s", "get_ip_error");
    }
    if( !netinfo->GetNetworkInfo(ETH0, NetworkInfo::kHWADDR, macaddr) ){
        printf("Get mac failed!");
        sprintf(macaddr, "%s", "get_mac_error");
    }
    delete netinfo;
    string timer( localip );
    // È¡µÃip¶ÎµÄ×îºóÒ»Î»
    timer = timer.c_str() + timer.find(".", timer.find(".", timer.find(".") + 1) + 1) + strlen(".");
    // ¸ù¾ÝipµØÖ·¼ÆËã»Ø¸´ÑÓ³Ù
    delay = atoi(timer.c_str()) * 1000; //printf("delay = %d us\n", delay);

    if(isOldFormat)
        snprintf(buf_send, MAXDATA_net - 1, "BKIP:%s,%s", localip, ver.c_str());
    else{
        // ¶ÁÈ¡hardware_info
        string hw_info = ReadSthFile("/version/hardware_info");
        string node_board = "Illegal_Board";
        char value[128] = "";
        if(!GetJsonValue(hw_info.c_str(), "board", value)){
            printf("GetJsonValue() from file-hardware_info failed!");
        }
        else{
            node_board = value;
        }

        // ¶ÁÈ¡hardware_name
        string hw_name = ReadSthFile("/version/hardware_name");
        if(hw_name == "null"){
            hw_name = node_board;
        }
        //printf("hw_name: %s", hw_name.c_str());

        // ¶ÁÈ¡Í¬²½µÄÖ÷´Ó×´Ì¬
        FILE *fshell = NULL;
        char tickmodule1[24] = {0};
        char tickmodule[24] = {0};
        fshell = popen("cat \"/data/software.ini\" | grep \"kTickModule=\"", "r");
        if(fshell){
            fread(tickmodule1, 1, sizeof(tickmodule1), fshell);
            sprintf(tickmodule1, "%s", tickmodule1 + strlen("kTickModule="));
            snprintf(tickmodule, strlen(tickmodule1), "%s", tickmodule1); // NOTE: È¥µô×îºóµÄ \n
            pclose(fshell);
        }

        // ¶ÁÈ¡Í¬²½³É¹¦×´Ì¬
        if(!strcmp(tickmodule, "slave")){
            string ticklock_status = ReadSthFile("/tmp/sync");
            if(ticklock_status == "1"){
                sprintf(ticksync, "%s", "true");
            }
            else{
                sprintf(ticksync, "%s", "false");
            }
        }
        else if(!strcmp(tickmodule, "master")){
            sprintf(ticksync, "%s", "true");
        }

        // ¶ÁÈ¡ÌõÐÎÂë
        string barcode = ReadSthFile("/barcode/barcode");

        // ¼ì²â /home/public ÎÄ¼þ¼ÐµÄ´æÔÚÐÔ
        string isExistPublic = "true";
        if(access("/home/public", F_OK) == -1){
            isExistPublic = "false";
        }

        // ×é³É»Ø¸´ÄÚÈÝ×Ö·û´®
#if 0
        cJSON *obj = cJSON_CreateObject();
        char v[64] = "";
        if(!GetJsonValue(ver.c_str(), "software_version", v)){
            printf("GetJsonValue() from file-version failed!");
        }
        else{
            cJSON_AddStringToObject(obj, "software_version", v);
        }
        memset(v, '\0', sizeof(v));
        if(!GetJsonValue(ver.c_str(), "hardware_version", v)){
            printf("GetJsonValue() from file-version failed!");
        }
        else{
            cJSON_AddStringToObject(obj, "hardware_version", v);
        }

        cJSON *root = cJSON_CreateObject();
        cJSON_AddStringToObject(root, "node_model", NODE_MODEL);
        cJSON_AddStringToObject(root, "node_board", node_board.c_str());
        cJSON_AddStringToObject(root, "hw_name",    hw_name.c_str());
        cJSON_AddStringToObject(root, "tick_module", tickmodule);
        cJSON_AddStringToObject(root, "node_ip",    localip);
        cJSON_AddStringToObject(root, "node_mac",   macaddr);
        cJSON_AddStringToObject(root, "node_name",  name.c_str());
        cJSON_AddStringToObject(root, "uuid",       uuid.c_str());
        cJSON_AddStringToObject(root, "barcode",    barcode.c_str());
        cJSON_AddItemToObject(  root, "version",    obj);

        char *s = cJSON_PrintUnformatted(root);
        if(s){
            memcpy(buf_send, s, MAXDATA);
            cJSON_free(s);
        }
        else{
            sprintf(buf_send, "%s", "[ERROR_CODE_01]");
        }
        cJSON_Delete(root);
#else
        snprintf(buf_send, MAXDATA_net - 1, "{\n\"node_model\":\"%s\",\n\"node_board\":\"%s\",\n\"hw_name\":\"%s\",\n\"tick_module\":\"%s\",\n\"sync\":%s,\n\"node_ip\":\"%s\",\n\"node_mac\":\"%s\",\n\"node_name\":\"%s\",\n\"version\":%s,\n\"uuid\":\"%s\",\n\"barcode\":\"%s\",\n\"public\":%s\n}", NODE_MODEL, node_board.c_str(), hw_name.c_str(), tickmodule, ticksync, localip, macaddr, name.c_str(), ver.c_str(), uuid.c_str(), barcode.c_str(), isExistPublic.c_str());
#endif
    }

    return delay;
}
void *udpBroadCastProc(void *arg)
{
    int  len = 0;
    uint port = *(uint*)arg;
    char buf[MAXDATA_net] = "";
    printf("starting broadcast listened at port: %d", port);

    UDPSocket *broadsocket = new UDPSocket();
    broadsocket->CreateUDPServer(port, false); //设为阻塞
#if 0
    broadsocket->AddBroadCast(ETH0); //加入eth0网卡的广播地址
#else
    broadsocket->AddBroadCast(); //使用255.255.255.255广播地址
    //broadsocket->BindLocalAddr(ETH0);
#endif

    while(1){
        memset(buf, '\0', sizeof(buf));
        if( (len = broadsocket->RecvFrom(buf, MAXDATA_net)) > 0){
            buf[len] = '\0'; printf("recv data: %s", buf);

            char buf_send[MAXDATA_net] = "";
            uint delay = 0;
            vector<string> cmdlist;

            SplitString(buf, cmdlist, ":");
            if(cmdlist[0] == "GTIP"){
                delay = GetSystemInfo(buf_send);
            }
            else if(cmdlist[0] == "FIND_NODE_UNI" ||
                    cmdlist[0] == "FIND_NODE_BOARD"){ // JSON格式回复
                delay = GetSystemInfo(buf_send, false);
            }
            else{
                //printf("Scan instruction [%s] error.", cmdlist[0].c_str());
                continue;
            }

            if(delay == 0){
                sprintf(buf_send, "%s", "[ERROR_CODE_00]");
                printf("get system info failed!");
            }
            else{
                usleep(delay);
            }
            printf("send data: %s", buf_send);

            struct sockaddr_in clientaddr = broadsocket->GetClientSockaddr();
            uint port = 0;
            if(cmdlist.size() == 1){ // NOTE: UDP回声回复
                port = ntohs(((struct sockaddr_in*)&clientaddr)->sin_port);
            }
            else if(cmdlist.size() == 2){ //NOTE: 回复给指定端口
                port = atoi(cmdlist[1].c_str());
            }
            else{
                printf("The number of scan instruction [%d] error.", cmdlist.size());
                continue;
            }

            for (uint i = 0; i < 2; i++){
                if(cmdlist[0] == "GTIP" ||
                   cmdlist[0] == "FIND_NODE_UNI"){
                    broadsocket->SendTo(inet_ntoa(((struct sockaddr_in*)&clientaddr)->sin_addr),
                                        port,
                                        buf_send,
                                        strlen(buf_send));
                }
                else if(cmdlist[0] == "FIND_NODE_BOARD"){
                    broadsocket->SendTo("255.255.255.255",
                                        port,
                                        buf_send,
                                        strlen(buf_send));
                }

                if(i == 0){
                    usleep(255000 + delay * 3);
                }
            }
        }
    }

    delete broadsocket;

    return (void*)1;
}
void *udpMultiCastProc(void *arg)
{
    uint port = *(uint*)arg;
    char mucip[32] = "239.255.255.250";
    int  len = 0;
    char buf[128];
    printf("starting multicast listened at port: %d", port);

    UDPSocket *mcsocket = new UDPSocket();
    mcsocket->CreateUDPServer(port, false); //设为阻塞
    mcsocket->AddMemberShip(mucip, ETH0);
    mcsocket->SetMulticastTTL(64);

    while(1)  {
        memset(buf, '\0', sizeof(buf));
        if( (len = mcsocket->RecvFrom(buf, MAXDATA_net)) > 0){
            buf[len] = '\0'; //printf("recv data: %s", buf);

            char buf_send[MAXDATA_net] = "";
            uint delay = 0;
            vector<string> cmdlist;

            SplitString(buf, cmdlist, ":");
            if(cmdlist[0] == "GTIP"){
                delay = GetSystemInfo(buf_send);
            }
            else if(cmdlist[0] == "FIND_NODE_UNI" ||
                    cmdlist[0] == "FIND_NODE_MULTICAST"){ // JSON格式回复
                delay = GetSystemInfo(buf_send, false);
            }
            else{
                //printf("Scan instruction [%s] error.", cmdlist[0].c_str());
                continue;
            }

            if(delay == 0){
                sprintf(buf_send, "%s", "[ERROR_CODE_00]");
                printf("get system info failed!");
            }
            else{
                usleep(delay);
            }
            printf("send data: %s", buf_send);

            struct sockaddr_in clientaddr = mcsocket->GetClientSockaddr();
            uint port = 0;
            if(cmdlist.size() == 1){ // NOTE: UDP回声回复
                port = ntohs(((struct sockaddr_in*)&clientaddr)->sin_port);
            }
            else if(cmdlist.size() == 2){ //NOTE: 回复给指定端口
                port = atoi(cmdlist[1].c_str());
            }
            else{
                printf("The number of scan instruction [%d] error.", cmdlist.size());
                continue;
            }

            for (uint i = 0; i < 2; i++){
                if(cmdlist[0] == "GTIP" ||
                   cmdlist[0] == "FIND_NODE_UNI"){
                    mcsocket->SendTo(inet_ntoa(((struct sockaddr_in*)&clientaddr)->sin_addr),
                                     port,
                                     buf_send,
                                     strlen(buf_send));
                }
                else if(cmdlist[0] == "FIND_NODE_MULTICAST"){
                    mcsocket->SendTo("239.255.255.250",
                                     port,
                                     buf_send,
                                     strlen(buf_send));
                }

                if(i == 0){
                    usleep(255000 + delay * 3);
                }
            }
        }
    }

    delete mcsocket;

    return (void*)1;
}
void *UdpEpollServer(void *arg)
{
   printf("Hello Server(netserver)!");

    uint port = *(uint*)arg;
    int  recvlen = 0;
    char recvdata[BUFF_SIZE_A258] = "";

    UDPSocket *udpsocket = new UDPSocket();
    int serversock = udpsocket->CreateUDPServer(port); // 默认非阻塞
    udpsocket->AddMemberShip(MULTICAST_CMD_IP, ETH0);
    printf("recv buf max: %d", udpsocket->GetRevBuf());

   struct msg_st msgdata;
  // printf("-----debug15-------\n");
   //auto cmdmap = Singleton<CommandMap>::getInstance();
   auto cmdmap_scode = Singleton<CommandMap_SCODe>::getInstance();
//   CommandMap_SCODe* cmdmap_scode=new CommandMap_SCODe();
     // printf("-----22222222-------\n");
   SCODe* scode = new SCODe();
//      printf("-----33333333-------\n");

    struct epoll_event ev;
    struct epoll_event events[MAXEPOLLSIZE];
    int kdpfd, nfds;
    int curfds = 1;
    int timeout = -1;
    kdpfd = epoll_create(MAXEPOLLSIZE);
    ev.events = EPOLLIN; //监听读事件
/*
 * EPOLLIN ：表示对应的文件描述符可以读（包括对端SOCKET正常关闭）；
 * EPOLLOUT：表示对应的文件描述符可以写；
 * EPOLLPRI：表示对应的文件描述符有紧急的数据可读（这里应该表示有带外数据到来）；
 * EPOLLERR：表示对应的文件描述符发生错误；
 * EPOLLHUP：表示对应的文件描述符被挂断；
 * EPOLLET： 将EPOLL设为边缘触发(Edge Triggered)模式，这是相对于水平触发(Level Triggered)来说的。
 * EPOLLONESHOT：只监听一次事件，当监听完这次事件之后，如果还需要继续监听这个socket的话，需要再次把这个socket加入到EPOLL队列里
*/
    // To fix valgrind error: Syscall param epoll_ctl(event) points to uninitialised byte(s)
    memset(&ev.data, 0, sizeof(ev.data));
    ev.data.fd = serversock;
    if (epoll_ctl(kdpfd, EPOLL_CTL_ADD, serversock, &ev) < 0)
    {
       printf("epoll set insertion error: %s", strerror(errno));
        return (void*)1;
    }
    printf("epollserver startup,socket %d, port %d, max connection is %d", serversock, port, MAXEPOLLSIZE);

    while(curfds > 0){
        nfds = epoll_wait(kdpfd, events, curfds, timeout); //阻塞的等待
        if (nfds == -1)
        {
            printf("epoll_wait error: %s", strerror(errno));
            continue;
        }
//        memset(&msgdata, 0, sizeof(struct msg_st));
//        if(msgrcv(c_mainmsgid, (void*)&msgdata,
//                  BUFF_SIZE, c_mainmsgtype, 0) == -1){ // 阻塞读取。若设非阻塞，最后一个参数设为IPC_NOWAIT
//            printf("msgrcv failed with errno: %s", strerror(errno));
//            sleep(1);
//            continue;
//        }
//        struct sockaddr_in clnt_adr = udpsocket->GetClientSockaddr();
//        strcpy(msgdata.mtext.ip, inet_ntoa(((struct sockaddr_in*)&clnt_adr)->sin_addr));
//        msgdata.mtext.port = ntohs(((struct sockaddr_in*)&clnt_adr)->sin_port);

        for (int n = 0; n < nfds; ++n)
        {
            if(events[n].data.fd == serversock){
                memset(recvdata, '\0', sizeof(recvdata));
                if( (recvlen = udpsocket->RecvFrom(recvdata, BUFF_SIZE_A258)) > 0){
                    if ( !strcmp(recvdata, "HEART") ){ //过滤心跳包
                        break;
                    }

                    if( !strcmp(recvdata, CMD_RESET) ){ //重启系统
                        printf("This system will be restart -f!");
                        system ( "sync" ); //NOTE: sync命令用于数据同步
                        system ( "sync" ); //NOTE: 多执行一次，以确保
                        system ( "reboot -f" );
                        curfds--;
                        break;
                    }
                    else if( !strncmp(recvdata, CMD_CUSTOMERID, strlen(CMD_CUSTOMERID)) ){
                        log_i("Set CUSTOMERID");
                        if(recvlen > (int)strlen(CMD_CUSTOMERID)){
                            char md5[512] = "\0";
                            string base64 = recvdata + strlen(CMD_CUSTOMERID);
                            our_MD5Data((const unsigned char *)base64.c_str(), base64.size(), md5);
                            WriteSthFile("/version/customerID", md5);
                        }
                        break;
                    }
                    else if(scode->CheckSCODeFormat(recvdata)){
                        //sprintf(gClientIP, msgdata.mtext.ip, SZ_IPADDR);
                        //gClientPort = msgdata.mtext.port;
                        struct sockaddr_in clnt_adr = udpsocket->GetClientSockaddr();
                        sprintf(gClientIP, inet_ntoa(((struct sockaddr_in*)&clnt_adr)->sin_addr), SZ_IPADDR);
                        gClientPort = ntohs(((struct sockaddr_in*)&clnt_adr)->sin_port);
                        printf("client ip: %s, port: %d. You recv message:", gClientIP, gClientPort);
                        //NOTE: 指令的判断与执行入口
                        printf("\n%s\n",recvdata);

                        vstring itemlist = scode->GetSCODeItems(recvdata);
                        if(itemlist.size() > 0){
                            cmdmap_scode->ProcessCmd(itemlist[0], itemlist);

                        }
                        break;
                    }
//                    else if( !strncmp(recvdata, CMD_SWITCH, strlen(CMD_SWITCH)) ){
//                        log_i("Set Software Input/Output!");
//                        WriteSthFile("/home/mode", recvdata + strlen(CMD_SWITCH));
//                        break;
//                    }
//                    else if( !strncmp(recvdata, CMD_CUSTOMERID, strlen(CMD_CUSTOMERID)) ){
//                        log_i("Set CUSTOMERID");
//                        if(recvlen > (int)strlen(CMD_CUSTOMERID)){
//                            char md5[512] = "\0";
//                            string base64 = recvdata + strlen(CMD_CUSTOMERID);
//                            our_MD5Data((const unsigned char *)base64.c_str(), base64.size(), md5);
//                            WriteSthFile("/version/customerID", md5);
//                        }
//                        break;
//                    }
//                    else if( !strcmp(recvdata, CMD_HELP) ){ //返回发送指令帮助文档
//                        ifstream in("example.json");
//                        ostringstream tmp;
//                        tmp << in.rdbuf();
//                        string data = tmp.str();
//                        if(data.length() == 0)
//                            data = "Please call the relevant research staff";

//                        udpsocket->EchoTo(data.c_str(), data.length());

//                        break;
//                    }

                    //NOTE:服务器接收打印log
//                    struct sockaddr_in clnt_adr = udpsocket->GetClientSockaddr();
//                    printf("\033[36;22m[netserver] contrl host:%s, port: %d\033[0m\n",
//                           inet_ntoa(((struct sockaddr_in*)&clnt_adr)->sin_addr),
//                           ntohs(((struct sockaddr_in*)&clnt_adr)->sin_port));
//                    printf("\033[36;22m[netserver] recv data [len=%d]\033[0m\n", recvlen);
//                    printf("%s", recvdata);

//                    MessageTransmit(recvdata, recvlen, c_mainmsgtype, clnt_adr);
                    }

                else if (recvlen == 0) {
                    printf("client close the connection");
                    //epoll_ctl(kdpfd, EPOLL_CTL_DEL, events[n].data.fd,&ev);
                    //curfds--;
                    break;
                }
                else  if (recvlen < 0) {
                   printf("recvfrom error");
                    //epoll_ctl(kdpfd, EPOLL_CTL_DEL, events[n].data.fd,&ev);
                    //curfds--;
                    break;
                }
             }
        }
    }

    delete udpsocket;

}


bool rcvData2IO_fun()
{
    char tmpData[128] = {0};
    char ioEcho[8] = {0};
    int rcvLen = 0, sendLen = 0;
    socklen_t addrLen = 0;
    uint value;
    memset(tmpData,0,sizeof(tmpData));

    addrLen = sizeof(struct sockaddr_in);


    rcvLen = recvfrom(socketIO_A258,tmpData,MAXDATA_net,0,(struct sockaddr*)(&ctrlAddr_A258),&addrLen);
    if(rcvLen > 0) {
        printf("rcv io data:%s\n",tmpData);
        if(!strncmp(tmpData,"IO_0=1",strlen("IO_0=1"))) {
            ioP0->io0Str = 1;
            ioP0->IOSet(ioP0->io0Str,0);
            global_data.io0St = 1;
        }
        else if(!strncmp(tmpData,"IO_1=1",strlen("IO_1=1"))) {
            ioP1->io1Str = 1;
            ioP1->IOSet(ioP1->io1Str,1);
            global_data.io1St = 1;
        }
        else if(!strncmp(tmpData,"IO_2=1",strlen("IO_2=1"))) {
            ioP2->io2Str = 1;
            ioP2->IOSet(ioP2->io2Str,2);
            global_data.io2St = 1;
            //ioP2->IOSet(1,2);
        }
        else if(!strncmp(tmpData,"IO_3=1",strlen("IO_3=1"))) {
            ioP3->io3Str = 1;
            ioP3->IOSet(ioP3->io3Str,3);
            global_data.io3St = 1;
            //ioP3->IOSet(1,3);
        }
        else if(!strncmp(tmpData,"IO_4=1",strlen("IO_4=1"))) {  //增加到8个IO
            ioP4->io4Str = 1;
            ioP4->IOSet(ioP4->io4Str,4);
            global_data.io4St = 1;
            //ioP4->IOSet(1,4);
        }
         else if(!strncmp(tmpData,"IO_5=1",strlen("IO_5=1"))) {
            ioP5->io5Str = 1;
            ioP5->IOSet(ioP5->io5Str,5);
            global_data.io5St = 1;
            //ioP5->IOSet(1,5);
        }
         else if(!strncmp(tmpData,"IO_6=1",strlen("IO_6=1"))) {
            ioP6->io6Str = 1;
            ioP6->IOSet(ioP6->io6Str,6);
            global_data.io6St = 1;
            //ioP6->IOSet(1,6);
        }
         else if(!strncmp(tmpData,"IO_7=1",strlen("IO_7=1"))) {
            ioP7->io7Str = 1;
            ioP7->IOSet(ioP7->io7Str,7);
            global_data.io7St = 1;
            //ioP7->IOSet(1,7);
        }
        else if(!strncmp(tmpData,"IO_0=0",strlen("IO_0=0"))) {
            ioP0->io0Str = 0;
            ioP0->IOSet(ioP0->io0Str,0);
            global_data.io0St = 0;
        }
        else if(!strncmp(tmpData,"IO_1=0",strlen("IO_1=0"))) {
            ioP1->io1Str = 0;
            ioP1->IOSet(ioP1->io1Str,1);
            global_data.io1St = 0;
        }
        else if(!strncmp(tmpData,"IO_2=0",strlen("IO_2=0"))) {
            ioP2->io2Str = 0;
            ioP2->IOSet(ioP2->io2Str,2);
            global_data.io2St = 0;
        }
        else if(!strncmp(tmpData,"IO_3=0",strlen("IO_3=0"))) {
            ioP3->io3Str = 0;
            ioP3->IOSet(ioP3->io3Str,3);
            global_data.io3St = 0;
            //ioP3->IOSet(0,3);
        }
         else if(!strncmp(tmpData,"IO_4=0",strlen("IO_4=0"))) {
            ioP4->io4Str = 0;
            ioP4->IOSet(ioP4->io4Str,4);
            global_data.io4St = 0;
            //ioP4->IOSet(0,4);
        }
         else if(!strncmp(tmpData,"IO_5=0",strlen("IO_5=0"))) {
            ioP5->io5Str = 0;
            ioP5->IOSet(ioP5->io5Str,5);
            global_data.io5St = 0;
            //ioP5->IOSet(0,5);
        }
         else if(!strncmp(tmpData,"IO_6=0",strlen("IO_6=0"))) {
            ioP6->io6Str = 0;
            ioP6->IOSet(ioP6->io6Str,6);
            global_data.io6St = 0;
            //ioP6->IOSet(0,6);
        }
        else if(!strncmp(tmpData,"IO_7=0",strlen("IO_7=0"))) {
            ioP7->io7Str = 0;
            ioP7->IOSet(ioP7->io7Str,7);
            global_data.io7St = 0;
            //ioP7->IOSet(0,7);
        }
        else if(!strncmp(tmpData,"getIO_0",strlen("getIO_0"))) {
            if(global_data.enIo_0 == 1) {
                if(hd_ver == NJ_A258) {
                    value=get_reg(A258_IO0_DATA);
                    if(value & A258_IO0_SET) {
                        ioP0->io0Str = 1;
                        global_data.io0St = 1;
                    }
                    else {
                        ioP0->io0Str = 0;
                        global_data.io0St = 0;
                    }
                }
                memset(ioEcho,0,sizeof(ioEcho));
                printf("now io state 0 :%d\n",ioP0->io0Str);
                if(ioP0->io0Str) {
                    strcpy(ioEcho,"IO_0=ON");
                }
                else {
                    strcpy(ioEcho,"IO_0=OFF");
                }

                if((global_data.ctlIO0port != 0) && (strlen(global_data.ctlip) >= 7)) {
                    ctrlAddr_A258.sin_family = AF_INET;
                    ctrlAddr_A258.sin_port = htons(global_data.ctlIO0port);
                    ctrlAddr_A258.sin_addr.s_addr = inet_addr(global_data.ctlip);
                    sendLen = sendto(socketIO_A258,ioEcho,strlen(ioEcho),0,(struct sockaddr*)&ctrlAddr_A258,sizeof(struct sockaddr_in));
                    if(sendLen > 0)
                        return true;
                    else {
                        COMMON_PRT("send dev io str to ctrl error\n");
                        return false;
                    }
                }
                else {
                    sendLen = sendto(socketIO_A258,ioEcho,strlen(ioEcho),0,(struct sockaddr*)&ctrlAddr_A258,sizeof(struct sockaddr_in));
                    if(sendLen > 0 ) {
                        COMMON_PRT("UMP not set ctrl IP PORT and echo\n");
                        return true;
                    }
                    else {
                        COMMON_PRT("send dev io str to echo error\n");
                        return false;
                    }

                }
            }
        }
        else if(!strncmp(tmpData,"getIO_1",strlen("getIO_1"))) {
            if(global_data.enIo_1 == 1) {
                if(hd_ver == NJ_A258) {
                    value=get_reg(A258_IO1_DATA);
                    if(value & A258_IO1_SET) {
                        ioP1->io1Str = 1;
                        global_data.io1St = 1;
                    }
                    else {
                        ioP1->io1Str = 0;
                        global_data.io1St = 0;
                    }
                }
                memset(ioEcho,0,sizeof(ioEcho));
                printf("now io state 1 :%d\n",ioP1->io1Str);
                if(ioP1->io1Str) {
                    strcpy(ioEcho,"IO_1=ON");
                }
                else {
                    strcpy(ioEcho,"IO_1=OFF");
                }

                if((global_data.ctlIO1port != 0) && (strlen(global_data.ctlip) >= 7)) {
                    ctrlAddr_A258.sin_family = AF_INET;
                    ctrlAddr_A258.sin_port = htons(global_data.ctlIO1port);
                    ctrlAddr_A258.sin_addr.s_addr = inet_addr(global_data.ctlip);
                    sendLen = sendto(socketIO_A258,ioEcho,strlen(ioEcho),0,(struct sockaddr*)&ctrlAddr_A258,sizeof(struct sockaddr_in));
                    if(sendLen > 0)
                        return true;
                    else {
                        COMMON_PRT("send dev io str to ctrl error\n");
                        return false;
                    }
                }
                else {
                    sendLen = sendto(socketIO_A258,ioEcho,strlen(ioEcho),0,(struct sockaddr*)&ctrlAddr_A258,sizeof(struct sockaddr_in));
                    if(sendLen > 0 ) {
                        COMMON_PRT("UMP not set ctrl IP PORT and echo\n");
                        return true;
                    }
                    else {
                        COMMON_PRT("send dev io str to echo error\n");
                        return false;
                    }

                }
            }
        }


        else if(!strncmp(tmpData,"getIO_2",strlen("getIO_2"))) {
            if(global_data.enIo_2 == 1) {

                if(hd_ver == NJ_A258) {
                    value=get_reg(A258_IO2_DATA);
                    if(value & A258_IO2_SET) {
                        ioP2->io2Str = 1;
                        global_data.io2St = 1;
                    }
                    else {
                        ioP2->io2Str = 0;
                        global_data.io2St = 0;
                    }
                }

                memset(ioEcho,0,sizeof(ioEcho));
                printf("now io state 2 :%d\n",ioP2->io2Str);
                if(ioP2->io2Str) {
                    strcpy(ioEcho,"IO_2=ON");
                }
                else {
                    strcpy(ioEcho,"IO_2=OFF");
                }

                if((global_data.ctlIO2port != 0) && (strlen(global_data.ctlip) >= 7)) {
                    ctrlAddr_A258.sin_family = AF_INET;
                    ctrlAddr_A258.sin_port = htons(global_data.ctlIO2port);
                    ctrlAddr_A258.sin_addr.s_addr = inet_addr(global_data.ctlip);
                    sendLen = sendto(socketIO_A258,ioEcho,strlen(ioEcho),0,(struct sockaddr*)&ctrlAddr_A258,sizeof(struct sockaddr_in));
                    if(sendLen > 0)
                        return true;
                    else {
                        COMMON_PRT("send dev io str to ctrl error\n");
                        return false;
                    }
                }
                else {
                    sendLen = sendto(socketIO_A258,ioEcho,strlen(ioEcho),0,(struct sockaddr*)&ctrlAddr_A258,sizeof(struct sockaddr_in));
                    if(sendLen > 0 ) {
                        COMMON_PRT("UMP not set ctrl IP PORT and echo\n");
                        return true;
                    }
                    else {
                        COMMON_PRT("send dev io str to echo error\n");
                        return false;
                    }

                }
            }
        }


        else if(!strncmp(tmpData,"getIO_3",strlen("getIO_3"))) {
            if(global_data.enIo_3 == 1) {

                if(hd_ver == NJ_A258) {
                    value=get_reg(A258_IO3_DATA);
                    if(value & A258_IO3_SET) {
                        ioP3->io3Str = 1;
                        global_data.io3St = 1;
                    }
                    else {
                        ioP3->io3Str = 0;
                        global_data.io3St = 0;
                    }
                }

                memset(ioEcho,0,sizeof(ioEcho));
                printf("now io state 3 :%d\n",ioP3->io3Str);
                if(ioP3->io3Str) {
                    strcpy(ioEcho,"IO_3=ON");
                }
                else {
                    strcpy(ioEcho,"IO_3=OFF");
                }

                if((global_data.ctlIO3port != 0) && (strlen(global_data.ctlip) >= 7)) {
                    ctrlAddr_A258.sin_family = AF_INET;
                    ctrlAddr_A258.sin_port = htons(global_data.ctlIO3port);
                    ctrlAddr_A258.sin_addr.s_addr = inet_addr(global_data.ctlip);
                    sendLen = sendto(socketIO_A258,ioEcho,strlen(ioEcho),0,(struct sockaddr*)&ctrlAddr_A258,sizeof(struct sockaddr_in));
                    if(sendLen > 0)
                        return true;
                    else {
                        COMMON_PRT("send dev io str to ctrl error\n");
                        return false;
                    }
                }
                else {
                    sendLen = sendto(socketIO_A258,ioEcho,strlen(ioEcho),0,(struct sockaddr*)&ctrlAddr_A258,sizeof(struct sockaddr_in));
                    if(sendLen > 0 ) {
                        COMMON_PRT("UMP not set ctrl IP PORT and echo\n");
                        return true;
                    }
                    else {
                        COMMON_PRT("send dev io str to echo error\n");
                        return false;
                    }

                }
            }
        }


        else if(!strncmp(tmpData,"getIO_4",strlen("getIO_4"))) {
            if(global_data.enIo_4 == 1) {

                if(hd_ver == NJ_A258) {
                    value=get_reg(A258_IO4_DATA);
                    if(value & A258_IO4_SET) {
                        ioP4->io4Str = 1;
                        global_data.io4St = 1;
                    }
                    else {
                        ioP4->io4Str = 0;
                        global_data.io4St = 0;
                    }
                }

                memset(ioEcho,0,sizeof(ioEcho));
                printf("now io state 4 :%d\n", ioP4->io4Str);
                if( ioP4->io4Str) {
                    strcpy(ioEcho,"IO_4=ON");
                }
                else {
                    strcpy(ioEcho,"IO_4=OFF");
                }

                if((global_data.ctlIO4port != 0) && (strlen(global_data.ctlip) >= 7)) {
                    ctrlAddr_A258.sin_family = AF_INET;
                    ctrlAddr_A258.sin_port = htons(global_data.ctlIO4port);
                    ctrlAddr_A258.sin_addr.s_addr = inet_addr(global_data.ctlip);
                    sendLen = sendto(socketIO_A258,ioEcho,strlen(ioEcho),0,(struct sockaddr*)&ctrlAddr_A258,sizeof(struct sockaddr_in));
                    if(sendLen > 0)
                        return true;
                    else {
                        COMMON_PRT("send dev io str to ctrl error\n");
                        return false;
                    }
                }
                else {
                    sendLen = sendto(socketIO_A258,ioEcho,strlen(ioEcho),0,(struct sockaddr*)&ctrlAddr_A258,sizeof(struct sockaddr_in));
                    if(sendLen > 0 ) {
                        COMMON_PRT("UMP not set ctrl IP PORT and echo\n");
                        return true;
                    }
                    else {
                        COMMON_PRT("send dev io str to echo error\n");
                        return false;
                    }

                }
            }
        }



        else if(!strncmp(tmpData,"getIO_5",strlen("getIO_5"))) {
            if(global_data.enIo_5 == 1) {

                if(hd_ver == NJ_A258) {
                    value=get_reg(A258_IO5_DATA);
                    if(value & A258_IO5_SET) {
                        ioP5->io5Str = 1;
                        global_data.io5St = 1;
                    }
                    else {
                        ioP5->io5Str = 0;
                        global_data.io5St = 0;
                    }
                }

                memset(ioEcho,0,sizeof(ioEcho));
                printf("now io state 5 :%d\n",ioP5->io5Str );
                if(ioP5->io5Str ) {
                    strcpy(ioEcho,"IO_5=ON");
                }
                else {
                    strcpy(ioEcho,"IO_5=OFF");
                }

                if((global_data.ctlIO5port != 0) && (strlen(global_data.ctlip) >= 7)) {
                    ctrlAddr_A258.sin_family = AF_INET;
                    ctrlAddr_A258.sin_port = htons(global_data.ctlIO5port);
                    ctrlAddr_A258.sin_addr.s_addr = inet_addr(global_data.ctlip);
                    sendLen = sendto(socketIO_A258,ioEcho,strlen(ioEcho),0,(struct sockaddr*)&ctrlAddr_A258,sizeof(struct sockaddr_in));
                    if(sendLen > 0)
                        return true;
                    else {
                        COMMON_PRT("send dev io str to ctrl error\n");
                        return false;
                    }
                }
                else {
                    sendLen = sendto(socketIO_A258,ioEcho,strlen(ioEcho),0,(struct sockaddr*)&ctrlAddr_A258,sizeof(struct sockaddr_in));
                    if(sendLen > 0 ) {
                        COMMON_PRT("UMP not set ctrl IP PORT and echo\n");
                        return true;
                    }
                    else {
                        COMMON_PRT("send dev io str to echo error\n");
                        return false;
                    }

                }
            }
        }


        else if(!strncmp(tmpData,"getIO_6",strlen("getIO_6"))) {
            if(global_data.enIo_6 == 1) {

                if(hd_ver == NJ_A258) {
                    value=get_reg(A258_IO6_DATA);
                    if(value & A258_IO6_SET) {
                        ioP6->io6Str = 1;
                        global_data.io6St = 1;
                    }
                    else {
                        ioP6->io6Str = 0;
                        global_data.io6St = 0;
                    }
                }

                memset(ioEcho,0,sizeof(ioEcho));
                printf("now io state 6 :%d\n",ioP6->io6Str);
                if(ioP6->io6Str) {
                    strcpy(ioEcho,"IO_6=ON");
                }
                else {
                    strcpy(ioEcho,"IO_6=OFF");
                }

                if((global_data.ctlIO6port != 0) && (strlen(global_data.ctlip) >= 7)) {
                    ctrlAddr_A258.sin_family = AF_INET;
                    ctrlAddr_A258.sin_port = htons(global_data.ctlIO6port);
                    ctrlAddr_A258.sin_addr.s_addr = inet_addr(global_data.ctlip);
                    sendLen = sendto(socketIO_A258,ioEcho,strlen(ioEcho),0,(struct sockaddr*)&ctrlAddr_A258,sizeof(struct sockaddr_in));
                    if(sendLen > 0)
                        return true;
                    else {
                        COMMON_PRT("send dev io str to ctrl error\n");
                        return false;
                    }
                }
                else {
                    sendLen = sendto(socketIO_A258,ioEcho,strlen(ioEcho),0,(struct sockaddr*)&ctrlAddr_A258,sizeof(struct sockaddr_in));
                    if(sendLen > 0 ) {
                        COMMON_PRT("UMP not set ctrl IP PORT and echo\n");
                        return true;
                    }
                    else {
                        COMMON_PRT("send dev io str to echo error\n");
                        return false;
                    }

                }
            }
        }


        else if(!strncmp(tmpData,"getIO_7",strlen("getIO_7"))) {
            if(global_data.enIo_7 == 1) {

                if(hd_ver == NJ_A258) {
                    value=get_reg(A258_IO7_DATA);
                    if(value & A258_IO7_SET) {
                        ioP7->io7Str = 1;
                        global_data.io7St = 1;
                    }
                    else {
                        ioP7->io7Str = 0;
                        global_data.io7St = 0;
                    }
                }

                memset(ioEcho,0,sizeof(ioEcho));
                printf("now io state 7 :%d\n",ioP7->io7Str);
                if(ioP7->io7Str) {
                    strcpy(ioEcho,"IO_7=ON");
                }
                else {
                    strcpy(ioEcho,"IO_7=OFF");
                }

                if((global_data.ctlIO7port != 0) && (strlen(global_data.ctlip) >= 7)) {
                    ctrlAddr_A258.sin_family = AF_INET;
                    ctrlAddr_A258.sin_port = htons(global_data.ctlIO7port);
                    ctrlAddr_A258.sin_addr.s_addr = inet_addr(global_data.ctlip);
                    sendLen = sendto(socketIO_A258,ioEcho,strlen(ioEcho),0,(struct sockaddr*)&ctrlAddr_A258,sizeof(struct sockaddr_in));
                    if(sendLen > 0)
                        return true;
                    else {
                        COMMON_PRT("send dev io str to ctrl error\n");
                        return false;
                    }
                }
                else {
                    sendLen = sendto(socketIO_A258,ioEcho,strlen(ioEcho),0,(struct sockaddr*)&ctrlAddr_A258,sizeof(struct sockaddr_in));
                    if(sendLen > 0 ) {
                        COMMON_PRT("UMP not set ctrl IP PORT and echo\n");
                        return true;
                    }
                    else {
                        COMMON_PRT("send dev io str to echo error\n");
                        return false;
                    }

                }
            }
        }


        else {
            COMMON_PRT("ctrl send data error:%s\n",tmpData);
        }

    }
    else {
        COMMON_PRT("not get rcv io data error\n");
        return false;
    }

    auto thisini = Singleton<SoftwareConfig>::getInstance();
    thisini->SetConfig(SoftwareConfig::kIO0st,global_data.io0St);
    thisini->SetConfig(SoftwareConfig::kIO1st,global_data.io1St);
    thisini->SetConfig(SoftwareConfig::kIO2st,global_data.io2St);
    thisini->SetConfig(SoftwareConfig::kIO3st,global_data.io3St);
    thisini->SetConfig(SoftwareConfig::kIO4st,global_data.io4St);
    thisini->SetConfig(SoftwareConfig::kIO5st,global_data.io5St);
    thisini->SetConfig(SoftwareConfig::kIO6st,global_data.io6St);
    thisini->SetConfig(SoftwareConfig::kIO7st,global_data.io7St);

    thisini->SaveConfig();
}
bool rcvData2Relay_fun()
{
    char tmpData[128] = {0};
    char relayEcho[16] = {0};
    int rcvLen = 0;
    socklen_t addrLen = 0;
    int  sendLen;
    memset(tmpData,0,sizeof(tmpData));
    auto thisini = Singleton<SoftwareConfig>::getInstance();

    addrLen = sizeof(struct sockaddr_in);

    rcvLen = recvfrom(socketRelay,tmpData,MAXDATA,0,(struct sockaddr*)(&ctrlAddr_A258_relay),&addrLen);
    if(rcvLen > 0 ) {
#ifdef __DEBUG__
        print0x(tmpData,rcvLen);
#endif
//        if(!strncmp(tmpData,"ON",2)) {
//            relayStr = 1;
//            relaySet(relayStr);
//            global_data.relaySt = 1;
//            thisini->SetConfig(SoftwareConfig::krelayst,global_data.relaySt);
//        }
//        else if(!strncmp(tmpData,"OFF",3)) {
//            relayStr = 0;
//            relaySet(relayStr);
//            global_data.relaySt = 0;
//            thisini->SetConfig(SoftwareConfig::krelayst,global_data.relaySt);
//        }
         if(!strncmp(tmpData, "relay_0=OFF", strlen("relay_0=OFF"))) {
            //printf("set relay 0  on\n");
            relayP0->relay0Str= 1;
            relayP0->relaySet(relayP0->relay0Str,0);
            global_data.relay0St = 1;
            thisini->SetConfig(SoftwareConfig::krelayst,global_data.relay0St);
            //relaySet(1, 0);
        }
        else if(!strncmp(tmpData, "relay_1=OFF", strlen("relay_1=OFF"))) {
            relayP1->relay1Str= 1;
            relayP1->relaySet(relayP1->relay1Str,1);
            global_data.relay1St = 1;
            thisini->SetConfig(SoftwareConfig::krelayst,global_data.relay1St);
            //relaySet(1, 1);
            //printf("set relay 1  on\n");
        }
        else if(!strncmp(tmpData, "relay_0=ON", strlen("relay_0=ON"))) {
            relayP0->relay0Str= 0;
            relayP0->relaySet(relayP0->relay0Str,0);
            global_data.relay0St = 0;
            thisini->SetConfig(SoftwareConfig::krelayst,global_data.relay0St);
            //relaySet(0, 0);
            //printf("set relay 0  off\n");
        }
        else if(!strncmp(tmpData, "relay_1=ON", strlen("relay_1=ON"))) {
            relayP1->relay1Str= 0;
            relayP1->relaySet(relayP1->relay1Str,1);
            global_data.relay1St = 0;
            thisini->SetConfig(SoftwareConfig::krelayst,global_data.relay1St);
            //relaySet(0, 1);
            //printf("set relay 1  off\n");
        }

        else if(!strncmp(tmpData,"getrelay_0",strlen("getrelay_0"))) {
            memset(relayEcho,0,sizeof(relayEcho));
            if(relayP0->relay0Str == 1) {
                strcpy(relayEcho,"relay_0=OFF");
            }
            else if(relayP0->relay0Str == 0) {
                strcpy(relayEcho,"relay_0=ON");
            }
            if((global_data.ctlrelay0port != 0) && (strlen(global_data.ctlip) >= 7)) {
                ctrlAddr_A258_relay.sin_family = AF_INET;
                ctrlAddr_A258_relay.sin_port = htons(global_data.ctlrelay0port);
                ctrlAddr_A258_relay.sin_addr.s_addr = inet_addr(global_data.ctlip);
                sendLen = sendto(socketRelay,relayEcho,strlen(relayEcho),0,(struct sockaddr*)&ctrlAddr_A258_relay,sizeof(struct sockaddr_in));
                if(sendLen > 0)
                    return true;
                else {
                    COMMON_PRT("send dev relay0 str to ctrl error\n");
                    return false;
                }
            }
            else {
                sendLen = sendto(socketRelay,relayEcho,strlen(relayEcho),0,(struct sockaddr*)&ctrlAddr_A258_relay,sizeof(struct sockaddr_in));
                if(sendLen > 0) {
                    COMMON_PRT("UMP not set ctrl IP PORT and echo\n");
                    return true;
                }
                else {
                    COMMON_PRT("send dev relay0 str to ctrl error\n");
                    return false;
                }
            }

        }
        else if(!strncmp(tmpData,"getrelay_1",strlen("getrelay_1"))) {
            memset(relayEcho,0,sizeof(relayEcho));
            if(relayP1->relay1Str == 1) {
                strcpy(relayEcho,"relay_1=OFF");
            }
            else if(relayP1->relay1Str == 0) {
                strcpy(relayEcho,"relay_1=ON");
            }
            if((global_data.ctlrelay1port != 0) && (strlen(global_data.ctlip) >= 7)) {
                ctrlAddr_A258_relay.sin_family = AF_INET;
                ctrlAddr_A258_relay.sin_port = htons(global_data.ctlrelay1port);
                ctrlAddr_A258_relay.sin_addr.s_addr = inet_addr(global_data.ctlip);
                sendLen = sendto(socketRelay,relayEcho,strlen(relayEcho),0,(struct sockaddr*)&ctrlAddr_A258_relay,sizeof(struct sockaddr_in));
                if(sendLen > 0)
                    return true;
                else {
                    COMMON_PRT("send dev relay1 str to ctrl error\n");
                    return false;
                }
            }
            else {
                sendLen = sendto(socketRelay,relayEcho,strlen(relayEcho),0,(struct sockaddr*)&ctrlAddr_A258_relay,sizeof(struct sockaddr_in));
                if(sendLen > 0) {
                    COMMON_PRT("UMP not set ctrl IP PORT and echo\n");
                    return true;
                }
                else {
                    COMMON_PRT("send dev relay1 str to ctrl error\n");
                    return false;
                }
            }

        }

        else {
            COMMON_PRT("ctrl relay send error data:%s\n",tmpData);
            return false;
        }
        thisini->SaveConfig();

    }
    else {
        COMMON_PRT("recv ctrl relay data error\n");
        return false;
    }
    return true;
}
bool rcvData2uart_fun()
{
    char tmpData[128] = {0};
    int rcvLen = 0;
    socklen_t addrLen = 0;
    memset(tmpData,0,sizeof(tmpData));

    addrLen = sizeof(struct sockaddr_in);

    rcvLen = recvfrom(socket232,tmpData,MAXDATA,0,(struct sockaddr*)(&ctrlAddr_A258_232),&addrLen);
    if(rcvLen > 0) {
#ifdef __DEBUG__
    print0x(tmpData,rcvLen);

#endif
        if(tmpData[0]==0x23&&tmpData[1]==0x20)
        {
            printf("write 2320 ready\n");
            write(rs2320P->dev232Fd,&tmpData[2],rcvLen-2);
            printf("write 2320 ok\n");
        }

        if(tmpData[0]==0x23&&tmpData[1]==0x21)
        {
            printf("write 2321 ready\n");
            write(rs2321P->dev232Fd,&tmpData[2],rcvLen-2);
            printf("write 2321 ok\n");
        }

        if(tmpData[0]==0x23&&tmpData[1]==0x22)
        {
            printf("write 2322 ready\n");
            write(rs2322P->dev232Fd,&tmpData[2],rcvLen-2);
            printf("write 2322 ok\n");
        }

        if(tmpData[0]==0x23&&tmpData[1]==0x23)
        {
            printf("write 2323 ready\n");
            write(rs2323P->dev232Fd,&tmpData[2],rcvLen-2);
            printf("write 2323 ok\n");
        }

    }
    else {
        COMMON_PRT("net not rcv 232data\n");
        return false;
    }
    return true;
}
bool rcvData2uart485_fun()
{
    char tmpData[128] = {0};
    int rcvLen = 0;
    socklen_t addrLen = 0;
    uint value;
    memset(tmpData,0,sizeof(tmpData));

    addrLen = sizeof(struct sockaddr_in);

    rcvLen = recvfrom(socket485,tmpData,MAXDATA,0,(struct sockaddr*)(&ctrlAddr_A258_485),&addrLen);
    if(rcvLen > 0)
    {
#ifdef __DEBUG__
        print0x(tmpData,rcvLen);
#endif

        if(tmpData[0]==0x48&&tmpData[1]==0x50)
        {
            write(rs4850P->dev485Fd,&tmpData[2],rcvLen-2);
            printf("write 4850 ok\n");
        }

        if(tmpData[0]==0x48&&tmpData[1]==0x51)
        {
            write(rs4851P->dev485Fd,&tmpData[2],rcvLen-2);
            printf("write 4851 ok\n");
        }

        if(tmpData[0]==0x48&&tmpData[1]==0x52)
        {
            write(rs4852P->dev485Fd,&tmpData[2],rcvLen-2);
            printf("write 4852 ok\n");
        }

        if(tmpData[0]==0x48&&tmpData[1]==0x53)
        {
            write(rs4853P->dev485Fd,&tmpData[2],rcvLen-2);
            printf("write 4853 ok\n");
        }

    }


    else {
        COMMON_PRT("net not rcv 485data\n");
        return false;
    }
    return true;
}


void HandleSig(int signo)
{
    printf("program exit--->0x%x\n",signo);
    if(en232) {
        delete rs232P;
    }

    if(en2320) {
        delete rs2320P;
    }

    if(en2321) {
        delete rs2321P;
    }

    if(en2322) {
        delete rs2322P;
    }

    if(en2323) {
        delete rs2323P;
    }
    if(en485) {
        delete rs485P;
    }


    if(en485) {
        delete rs485P;
    }

    if(en4850) {
        delete rs4850P;
    }

    if(en4851) {
        delete rs4851P;
    }

    if(en4852) {
        delete rs4852P;
    }

    if(en4853) {
        delete rs4853P;
    }


    if(enRelay) {
        delete relayP;
    }

    if(enRelay0) {
        delete relayP0;
    }

    if(enRelay1) {
        delete relayP1;
    }

    if(enIr) {
        delete irP;
    }

    if(enIo0) {
        delete ioP0;
    }

    if(enIo1) {
        delete ioP1;
    }

    if(enIo2) {
        delete ioP2;
    }
    if(enIo3) {
        delete ioP3;
    }
    if(enIo4) {
        delete ioP4;
    }
    if(enIo5) {
        delete ioP5;
    }
    if(enIo6) {
        delete ioP6;
    }
    if(enIo7) {
        delete ioP7;
    }

    delete umpP;

    exit(-1);
}

bool iniParam()
{
    auto thisini = Singleton<SoftwareConfig>::getInstance();
    memset(&global_data,0,sizeof(CONFIGDATA));

//控制软件回复端口
    strcpy(global_data.ctlip,thisini->GetConfig(SoftwareConfig::kctlip).c_str());
    //strcpy(global_data.ctlip_tc,thisini->GetConfig(SoftwareConfig::kctlip_tc).c_str());// 透传ip
//    global_data.ctl232port = str2int(thisini->GetConfig(SoftwareConfig::kctl232port));
//    global_data.ctl485port = str2int(thisini->GetConfig(SoftwareConfig::kctl485port));
    global_data.ctl2320port = str2int(thisini->GetConfig(SoftwareConfig::kctl2320port));
    global_data.ctl2321port = str2int(thisini->GetConfig(SoftwareConfig::kctl2321port));
    global_data.ctl2322port = str2int(thisini->GetConfig(SoftwareConfig::kctl2322port));
    global_data.ctl2323port = str2int(thisini->GetConfig(SoftwareConfig::kctl2323port));
    global_data.ctl4850port = str2int(thisini->GetConfig(SoftwareConfig::kctl4850port));
    global_data.ctl4851port = str2int(thisini->GetConfig(SoftwareConfig::kctl4851port));
    global_data.ctl4852port = str2int(thisini->GetConfig(SoftwareConfig::kctl4852port));
    global_data.ctl4853port = str2int(thisini->GetConfig(SoftwareConfig::kctl4853port));
    global_data.ctlrelayport = str2int(thisini->GetConfig(SoftwareConfig::kctlrelayport));
    global_data.ctlrelay0port = str2int(thisini->GetConfig(SoftwareConfig::kctlrelay0port));
    global_data.ctlrelay1port = str2int(thisini->GetConfig(SoftwareConfig::kctlrelay1port));
    global_data.ctlinfraredport = str2int(thisini->GetConfig(SoftwareConfig::kctlinfraredport));
    global_data.ctlinfraredport_tc = str2int(thisini->GetConfig(SoftwareConfig::kctlinfraredport_tc));
    global_data.ctlIO0port = str2int(thisini->GetConfig(SoftwareConfig::kctlIO0port));
    global_data.ctlIO1port = str2int(thisini->GetConfig(SoftwareConfig::kctlIO1port));
    global_data.ctlIO2port = str2int(thisini->GetConfig(SoftwareConfig::kctlIO2port));
    global_data.ctlIO3port = str2int(thisini->GetConfig(SoftwareConfig::kctlIO3port));
    global_data.ctlIO4port = str2int(thisini->GetConfig(SoftwareConfig::kctlIO4port));
    global_data.ctlIO5port = str2int(thisini->GetConfig(SoftwareConfig::kctlIO5port));
    global_data.ctlIO6port = str2int(thisini->GetConfig(SoftwareConfig::kctlIO6port));
    global_data.ctlIO7port = str2int(thisini->GetConfig(SoftwareConfig::kctlIO7port));

//本地受控端口
    global_data.sv232port = str2int(thisini->GetConfig(SoftwareConfig::ksv232port));
    global_data.sv485port = str2int(thisini->GetConfig(SoftwareConfig::ksv485port));
    global_data.svrelayport = str2int(thisini->GetConfig(SoftwareConfig::ksvrelayport));
    global_data.svInfraredport = str2int(thisini->GetConfig(SoftwareConfig::kinfraredport));
    global_data.svIOport = str2int(thisini->GetConfig(SoftwareConfig::ksvIOport));
    global_data.svIOport_A258 = str2int(thisini->GetConfig(SoftwareConfig::ksvIOport_A258));  //绑定上
//RS232参数
    global_data.en232 = str2int(thisini->GetConfig(SoftwareConfig::ken232));
    strcpy(global_data.port232,thisini->GetConfig(SoftwareConfig::k232port).c_str());
    global_data.uartspeed232 = str2int(thisini->GetConfig(SoftwareConfig::k232uartspeed));
    global_data.nbit232 = str2int(thisini->GetConfig(SoftwareConfig::k232nbit));
    global_data.nstop232 = str2int(thisini->GetConfig(SoftwareConfig::k232nstop));
    strcpy(global_data.event232,thisini->GetConfig(SoftwareConfig::k232event).c_str());
    global_data.nVtime232 = str2int(thisini->GetConfig(SoftwareConfig::k232vtime));
    global_data.nVmin232 = str2int(thisini->GetConfig(SoftwareConfig::k232vmin));

    global_data.en2320 = str2int(thisini->GetConfig(SoftwareConfig::ken2320));
    strcpy(global_data.port2320,thisini->GetConfig(SoftwareConfig::k2320port).c_str());
    global_data.uartspeed2320 = str2int(thisini->GetConfig(SoftwareConfig::k2320uartspeed));
    global_data.nbit2320 = str2int(thisini->GetConfig(SoftwareConfig::k2320nbit));
    global_data.nstop2320 = str2int(thisini->GetConfig(SoftwareConfig::k2320nstop));
    strcpy(global_data.event2320,thisini->GetConfig(SoftwareConfig::k2320event).c_str());
    global_data.nVtime2320 = str2int(thisini->GetConfig(SoftwareConfig::k2320vtime));
    global_data.nVmin2320 = str2int(thisini->GetConfig(SoftwareConfig::k2320vmin));

    global_data.en2321 = str2int(thisini->GetConfig(SoftwareConfig::ken2321));
    strcpy(global_data.port2321,thisini->GetConfig(SoftwareConfig::k2321port).c_str());
    global_data.uartspeed2321 = str2int(thisini->GetConfig(SoftwareConfig::k2321uartspeed));
    global_data.nbit2321 = str2int(thisini->GetConfig(SoftwareConfig::k2321nbit));
    global_data.nstop2321 = str2int(thisini->GetConfig(SoftwareConfig::k2321nstop));
    strcpy(global_data.event2321,thisini->GetConfig(SoftwareConfig::k2321event).c_str());
    global_data.nVtime2321 = str2int(thisini->GetConfig(SoftwareConfig::k2321vtime));
    global_data.nVmin2321 = str2int(thisini->GetConfig(SoftwareConfig::k2321vmin));

    global_data.en2322 = str2int(thisini->GetConfig(SoftwareConfig::ken2322));
    strcpy(global_data.port2322,thisini->GetConfig(SoftwareConfig::k2322port).c_str());
    global_data.uartspeed2322 = str2int(thisini->GetConfig(SoftwareConfig::k2322uartspeed));
    global_data.nbit2322 = str2int(thisini->GetConfig(SoftwareConfig::k2322nbit));
    global_data.nstop2322 = str2int(thisini->GetConfig(SoftwareConfig::k2322nstop));
    strcpy(global_data.event2322,thisini->GetConfig(SoftwareConfig::k2322event).c_str());
    global_data.nVtime2322 = str2int(thisini->GetConfig(SoftwareConfig::k2322vtime));
    global_data.nVmin2322 = str2int(thisini->GetConfig(SoftwareConfig::k2322vmin));

    global_data.en2323 = str2int(thisini->GetConfig(SoftwareConfig::ken2323));
    strcpy(global_data.port2323,thisini->GetConfig(SoftwareConfig::k2323port).c_str());
    global_data.uartspeed2323 = str2int(thisini->GetConfig(SoftwareConfig::k2323uartspeed));
    global_data.nbit2323 = str2int(thisini->GetConfig(SoftwareConfig::k2323nbit));
    global_data.nstop2323 = str2int(thisini->GetConfig(SoftwareConfig::k2323nstop));
    strcpy(global_data.event2323,thisini->GetConfig(SoftwareConfig::k2323event).c_str());
    global_data.nVtime2323 = str2int(thisini->GetConfig(SoftwareConfig::k2323vtime));
    global_data.nVmin2323 = str2int(thisini->GetConfig(SoftwareConfig::k2323vmin));

//RS485参数
    global_data.en485 = str2int(thisini->GetConfig(SoftwareConfig::ken485));
    strcpy(global_data.port485,thisini->GetConfig(SoftwareConfig::k485port).c_str());
    global_data.uartspeed485 = str2int(thisini->GetConfig(SoftwareConfig::k485uartspeed));
    global_data.nbit485 = str2int(thisini->GetConfig(SoftwareConfig::k485nbit));
    global_data.nstop485 = str2int(thisini->GetConfig(SoftwareConfig::k485nstop));
    strcpy(global_data.event485,thisini->GetConfig(SoftwareConfig::k485event).c_str());
    global_data.nVtime485 = str2int(thisini->GetConfig(SoftwareConfig::k485vtime));
    global_data.nVmin485 = str2int(thisini->GetConfig(SoftwareConfig::k485vmin));

    global_data.en4850 = str2int(thisini->GetConfig(SoftwareConfig::ken4850));
    strcpy(global_data.port4850,thisini->GetConfig(SoftwareConfig::k4850port).c_str());
    global_data.uartspeed4850 = str2int(thisini->GetConfig(SoftwareConfig::k4850uartspeed));
    global_data.nbit4850 = str2int(thisini->GetConfig(SoftwareConfig::k4850nbit));
    global_data.nstop4850 = str2int(thisini->GetConfig(SoftwareConfig::k4850nstop));
    strcpy(global_data.event4850,thisini->GetConfig(SoftwareConfig::k4850event).c_str());
    global_data.nVtime4850 = str2int(thisini->GetConfig(SoftwareConfig::k4850vtime));
    global_data.nVmin4850 = str2int(thisini->GetConfig(SoftwareConfig::k4850vmin));

    global_data.en4851 = str2int(thisini->GetConfig(SoftwareConfig::ken4851));
    strcpy(global_data.port4851,thisini->GetConfig(SoftwareConfig::k4851port).c_str());
    global_data.uartspeed4851 = str2int(thisini->GetConfig(SoftwareConfig::k4851uartspeed));
    global_data.nbit4851 = str2int(thisini->GetConfig(SoftwareConfig::k4851nbit));
    global_data.nstop4851 = str2int(thisini->GetConfig(SoftwareConfig::k4851nstop));
    strcpy(global_data.event4851,thisini->GetConfig(SoftwareConfig::k4851event).c_str());
    global_data.nVtime4851 = str2int(thisini->GetConfig(SoftwareConfig::k4851vtime));
    global_data.nVmin4851 = str2int(thisini->GetConfig(SoftwareConfig::k4851vmin));

    global_data.en4852 = str2int(thisini->GetConfig(SoftwareConfig::ken4852));
    strcpy(global_data.port4852,thisini->GetConfig(SoftwareConfig::k4852port).c_str());
    global_data.uartspeed4852 = str2int(thisini->GetConfig(SoftwareConfig::k4852uartspeed));
    global_data.nbit4852 = str2int(thisini->GetConfig(SoftwareConfig::k4852nbit));
    global_data.nstop4852 = str2int(thisini->GetConfig(SoftwareConfig::k4852nstop));
    strcpy(global_data.event4852,thisini->GetConfig(SoftwareConfig::k4852event).c_str());
    global_data.nVtime4852 = str2int(thisini->GetConfig(SoftwareConfig::k4852vtime));
    global_data.nVmin4852 = str2int(thisini->GetConfig(SoftwareConfig::k4852vmin));

    global_data.en4853 = str2int(thisini->GetConfig(SoftwareConfig::ken4853));
    strcpy(global_data.port4853,thisini->GetConfig(SoftwareConfig::k4853port).c_str());
    global_data.uartspeed4853 = str2int(thisini->GetConfig(SoftwareConfig::k4853uartspeed));
    global_data.nbit4853 = str2int(thisini->GetConfig(SoftwareConfig::k4853nbit));
    global_data.nstop4853 = str2int(thisini->GetConfig(SoftwareConfig::k4853nstop));
    strcpy(global_data.event4853,thisini->GetConfig(SoftwareConfig::k4853event).c_str());
    global_data.nVtime4853 = str2int(thisini->GetConfig(SoftwareConfig::k4853vtime));
    global_data.nVmin4853 = str2int(thisini->GetConfig(SoftwareConfig::k4853vmin));

//继电器参数
    global_data.enRelay = str2int(thisini->GetConfig(SoftwareConfig::kenrelay));
    global_data.relaySt = str2int(thisini->GetConfig(SoftwareConfig::krelayst));
    global_data.enRelay0 = str2int(thisini->GetConfig(SoftwareConfig::kenrelay0));
    global_data.relay0St = str2int(thisini->GetConfig(SoftwareConfig::krelay0st));
    global_data.enRelay1 = str2int(thisini->GetConfig(SoftwareConfig::kenrelay1));
    global_data.relay1St = str2int(thisini->GetConfig(SoftwareConfig::krelay1st));

//红外参数  串口红外和GPIO定时器红外
    global_data.enInfrared = str2int(thisini->GetConfig(SoftwareConfig::kenInfrared));
//    global_data.enInfrared_tc = str2int(thisini->GetConfig(SoftwareConfig::kenInfrared_tc));  //增加透传使能
    strcpy(global_data.portInfrared,thisini->GetConfig(SoftwareConfig::kInfraredport).c_str());
    global_data.uartspeedInf = str2int(thisini->GetConfig(SoftwareConfig::kInfrareduartspeed));
    global_data.nbitInf = str2int(thisini->GetConfig(SoftwareConfig::kInfrarednbit));
    global_data.nstopInf = str2int(thisini->GetConfig(SoftwareConfig::kInfrarednstop));
    strcpy(global_data.eventInf,thisini->GetConfig(SoftwareConfig::kInfraredevent).c_str());
    global_data.nVtimeInf = str2int(thisini->GetConfig(SoftwareConfig::kInfraredvtime));
    global_data.nVminInf = str2int(thisini->GetConfig(SoftwareConfig::kInfraredvmin));

//IO参数
    global_data.enIo_0 = str2int(thisini->GetConfig(SoftwareConfig::kSet0IO).c_str());
    global_data.io0St = str2int(thisini->GetConfig(SoftwareConfig::kIO0st).c_str());
    global_data.enIo_1 = str2int(thisini->GetConfig(SoftwareConfig::kSet1IO).c_str());
    global_data.io1St = str2int(thisini->GetConfig(SoftwareConfig::kIO1st).c_str());
    global_data.enIo_2 = str2int(thisini->GetConfig(SoftwareConfig::kSet2IO).c_str());
    global_data.io2St = str2int(thisini->GetConfig(SoftwareConfig::kIO2st).c_str());
    global_data.enIo_3 = str2int(thisini->GetConfig(SoftwareConfig::kSet3IO).c_str());
    global_data.io3St = str2int(thisini->GetConfig(SoftwareConfig::kIO3st).c_str());
    global_data.enIo_4 = str2int(thisini->GetConfig(SoftwareConfig::kSet4IO).c_str());
    global_data.io4St = str2int(thisini->GetConfig(SoftwareConfig::kIO4st).c_str());
    global_data.enIo_5 = str2int(thisini->GetConfig(SoftwareConfig::kSet5IO).c_str());
    global_data.io5St = str2int(thisini->GetConfig(SoftwareConfig::kIO5st).c_str());
    global_data.enIo_6 =str2int(thisini->GetConfig(SoftwareConfig::kSet6IO).c_str());
    global_data.io6St = str2int(thisini->GetConfig(SoftwareConfig::kIO6st).c_str());
    global_data.enIo_7 = str2int(thisini->GetConfig(SoftwareConfig::kSet7IO).c_str()); //暂可复用于红外透传
    global_data.io7St = str2int(thisini->GetConfig(SoftwareConfig::kIO7st).c_str());


//TCP参数
    global_data.tcp_en = str2int(thisini->GetConfig(SoftwareConfig::kTcpEnable).c_str());
    global_data.tcp_port = str2int(thisini->GetConfig(SoftwareConfig::kTcpServerPort).c_str());
    global_data.tcp_link_max = str2int(thisini->GetConfig(SoftwareConfig::kTcpLinkMax).c_str());

    return true;
}


bool hd_232 = true;
bool hd_485 = false;
bool hd_relay = false;
bool hd_ir = false;
bool hd_io = false;
bool hd_232_edc = false;

bool ir_nj_700k = false;

bool ir_nj_21d = false;



int main()
{


    /* NOTE: 生成版本号: <主版本号>.<次版本号>.<修订版本号>.<编译日期>_<阶段版本> */
    HI_U32 u32ChipId = 0;

    if ( !GenerateVersion(sw_version, u32ChipId) )
        printf("generate version failed!");

        Singleton<CommandMap_SCODe>::getInstance()->LoadInitialMediaSCODe();

    // 3. 启动 广播监听，10端口 的线程
    bool b_detached = true; //设置线程为分离模式
    int broad_port = BROAD_SCAN_PORT;
    pthread_t broadcast_tid = CreateThread(udpBroadCastProc, 0, SCHED_FIFO, b_detached, &broad_port);
    if(broadcast_tid == 0){
        printf("pthread_create() failed: net udpBroadCastProc");
    }
    // 3.1. 启动 组播监听，3700端口 的线程
    int multi_port = MULTICAST_SCAN_PORT;
    pthread_t multicast_tid = CreateThread(udpMultiCastProc, 0, SCHED_FIFO, b_detached, &multi_port);
    if(multicast_tid == 0){
        printf("pthread_create() failed: net udpMultiCastProc");
    }

    printf("ready UdpEpollServer!\n");
    // 5. 启动udp server接收信息（改为了线程）
    int cmd_port = CMD_PORT;
    pthread_t EpollServer_tid = CreateThread(UdpEpollServer, 0, SCHED_FIFO, b_detached, &cmd_port);
    if(EpollServer_tid == 0){
        printf("pthread_create() failed: net UdpEpollServer");
    }



bool hi3521d_7000 = false;



#ifdef  __7000_NJ__
        hi3521d_7000 = true;
#endif
    signal(SIGHUP, HandleSig); //NOTE: 关闭终端
    signal(SIGINT, HandleSig); //NOTE: 检测Ctrl+C按键
    signal(SIGTERM, HandleSig); //NOTE: 检测killall 命令
    signal(SIGSEGV, HandleSig); //NOTE: 检测 Segmentation fault!

    char hdVersion[32] = {0};
    memset(hdVersion,0,32);

#ifdef __AUTO_TEST__

#else

    readHdVersion(hdVersion);
#endif
    local_ip = GetLocalIp();

//根据硬件版本号，使能不同的硬件
    if(!strncmp(hdVersion,HD_2SDI_NJ,strlen(HD_2SDI_NJ))) {
        hd_232 = true;
        hd_ir = true;
    }
    else if(!strncmp(hdVersion,A258_NJ,strlen(A258_NJ))) {    //根据硬件号选择A258
        hd_232 = true;
        hd_relay = true;
        hd_485 = true;
        hd_ir = true;
        hd_io = true;
        hd_ver = NJ_A258;
        irTime = true;

    }
#if 0
    if(!strncmp(hdVersion,IRTIMEHD,strlen(IRTIMEHD))) {
        COMMON_PRT("this hardware version support time ir\n");
        irTime = true;
    }
#endif
    else if(!strncmp(hdVersion,HD_7000_PTN,strlen(HD_7000_PTN))) {
        COMMON_PRT("this hardware version support time ir\n");
        irTime = true;
    }
    else if(!strncmp(hdVersion,HD_21A_PTN,strlen(HD_21A_PTN))) {
        COMMON_PRT("this hardware version support time ir\n");
        irTime = true;
    }
    else if(!strncmp(hdVersion,IR_8000,strlen(IR_8000))) {
        COMMON_PRT("this hardware version support time ir\n");
        irTime = true;
    }
    else if(!strncmp(hdVersion,HD_700K_NJ_V2,strlen(HD_700K_NJ_V2))) {
        COMMON_PRT("this hardware version support time ir\n");
        irTime = true;
    }
    else if(!strncmp(hdVersion,HD_D_HDMI_NJ,strlen(HD_D_HDMI_NJ))) {
        COMMON_PRT("this hardware version support time ir\n");
        irTime = true;
    }

#ifdef __7000__
    irTime = true;
#endif

    //1-读取本地配置文件，初始化全局变量
    auto thisini = Singleton<SoftwareConfig>::getInstance();
    thisini->ReadConfig();
    thisini->PrintConfig();
    thisini->SetNetwork();
    thisini->SaveConfig_ALL();

    if(!strncmp(hdVersion,HD_7020_NJ,strlen(HD_7020_NJ))) {
        thisini->SetConfig(SoftwareConfig::k232port, "com2");
        thisini->SaveConfig();
    }
    if(PTN_21A == hd_ver) {
        thisini->SetConfig(SoftwareConfig::k232port, "com0");
        thisini->SaveConfig();
    }

    if(NJ_A258 == hd_ver) {//初始化IO6为发送状态 红外接收供电用
        thisini->SetConfig(SoftwareConfig::kSet6IO, "2");
        //printf("\n初始化IO6为发送状态 红外接收供电用\n");
        thisini->SaveConfig();
    }

    iniParam();   //初始化

//   printf("-*****--updata ok6666--******--\r\n");
//   printf("-*****--9999--******--\r\n");

    if(-1==socket232)
    {
        socket232 = createUdp(global_data.sv232port);
        printf("socket232_A258 creat ok\r\n");
    }

    if(-1==socket485)
    {
        socket485 = createUdp(global_data.sv485port);
        printf("socket485_A258 creat ok\r\n");
    }

    if(-1==socketRelay)
    {
        socketRelay = createUdp(global_data.svrelayport);
        printf("socketRelay creat ok\r\n");
    }

    if(-1==socketIO_A258)
    {
        socketIO_A258 = createUdp(global_data.svIOport_A258);
        printf("socketIO_A258 creat ok\r\n");
    }

    FD_ZERO(&readFd);

    int umpFlag = -1;


    //2-RS232初始化
//    if(global_data.en232 && hd_232) {
//        en232 = true;
//        COMMON_PRT("rs232 enable\n");
//        rs232P = new rs232();
//        rs232P->iniRs232();
//    }

        //2320
    if(global_data.en2320 && hd_232) {
        en2320 = true;
        COMMON_PRT("rs2320 enable\n");
        rs2320P = new rs2320();
        rs2320P->iniRs232();
    }

        //2321
    if(global_data.en2321 && hd_232) {
        en2321 = true;
        COMMON_PRT("rs2321 enable\n");
        rs2321P = new rs2321();
        rs2321P->iniRs232();
    }

        //2322
    if(global_data.en2322 && hd_232) {
        en2322 = true;
        COMMON_PRT("rs2322 enable\n");
        rs2322P = new rs2322();
        rs2322P->iniRs232();
    }

        //2323

    if(global_data.en2323 && hd_232) {
        en2323 = true;
        COMMON_PRT("rs2323 enable\n");
        rs2323P = new rs2323();
        rs2323P->iniRs232();
    }

    //3-RS485初始化
//    if(global_data.en485 && (false == hi3521d_7000) && hd_485) {
//        en485 = true;
//        COMMON_PRT("rs485 enable\n");
//        rs485P = new rs485();
//        rs485P->iniRs485();
//    }
        //4850
    if(global_data.en4850 && (false == hi3521d_7000) && hd_485) {
        en4850 = true;
        COMMON_PRT("rs4850 enable\n");
        rs4850P = new rs4850();
        rs4850P->iniRs485();
    }
        //4851
    if(global_data.en4851 && (false == hi3521d_7000) && hd_485) {
        en4851 = true;
        COMMON_PRT("rs4851 enable\n");
        rs4851P = new rs4851();
        rs4851P->iniRs485();
    }

        //4852
    if(global_data.en4852 && (false == hi3521d_7000) && hd_485) {
        en4852 = true;
        COMMON_PRT("rs4852 enable\n");
        rs4852P = new rs4852();
        rs4852P->iniRs485();
    }

        //4853
     if(global_data.en4853 && (false == hi3521d_7000) && hd_485) {
        en4853 = true;
        COMMON_PRT("rs4853 enable\n");
        rs4853P = new rs4853();
        rs4853P->iniRs485();
        }

    //4-relay初始化
//    if(global_data.enRelay && (false == hi3521d_7000) && hd_relay) {
//        enRelay = true;
//        COMMON_PRT("relay enable\n");
//        relayP = new relay();
//        relayP->relayIni();
//        relayP->relaySet(global_data.relaySt);
//    }

    if(global_data.enRelay0 && (false == hi3521d_7000) && hd_relay) {
        enRelay0 = true;
        COMMON_PRT("relay0 enable\n");
        relayP0 = new relay();
        relayP0->relayIni();
        relayP0->relaySet(global_data.relay0St,0);
    }

    if(global_data.enRelay1 && (false == hi3521d_7000) && hd_relay) {
        enRelay1 = true;
        COMMON_PRT("relay1 enable\n");
        relayP1 = new relay();
        relayP1->relayIni();
        relayP1->relaySet(global_data.relay1St,1);
    }

    //5-ir初始化
    if(global_data.enInfrared && hd_ir) {
        enIr = true;
        COMMON_PRT("ir enable\n");
        irP = new infrared();
        irP->infraredIni(global_data.enInfrared);
    }
#if 0  //需要这样吗
    if(global_data.enInfrared_tc && hd_ir) {
        enIr_tc = true;
        COMMON_PRT("ir_tc enable\n");
        irP_tc = new infrared();
        irP_tc->infraredIni(global_data.enInfrared_tc);
    }
#endif
    //6-0-io0初始化
    if(global_data.enIo_0 && hd_io) {
//new 8ge
        COMMON_PRT("IO 0 enable\n");
        enIo0 = true;
        ioP0 = new io();
        ioP0->ioIni(0);
        if(global_data.enIo_0 == 2) {
            ioP0->IOSet(global_data.io0St,0);
        }

    }

    if(global_data.enIo_0 && hd_232_edc) {
        COMMON_PRT("IO_edc 0 enable\n");
        sleep(2);
        ioP_edc = new io_edc();
        ioP_edc->ioIni_edc();
    }


    //6-1-io1未初始化为红外，初始化为IO;红外优先级高于IO  //改为7
    if((global_data.enIo_1) && hd_io) {
        enIo1 = true;
        COMMON_PRT("IO 1 enablea\n");
        ioP1 = new io();
        ioP1->ioIni(1);
        if(global_data.enIo_1 == 2) {
            ioP1->IOSet(global_data.io1St,1);
        }
    }
#if 1
    if(global_data.enIo_2 && hd_io) {

        COMMON_PRT("IO 2 enable\n");
        enIo2 = true;
        ioP2 = new io();
        ioP2->ioIni(2);
        if(global_data.enIo_2 == 2) {
            ioP2->IOSet(global_data.io2St,2);  //发送模式初始化为 初始电平ioxst 在配置文件里改
        }

    }
    if(global_data.enIo_3 && hd_io) {

        COMMON_PRT("IO 3 enable\n");
        enIo3 = true;
        ioP3 = new io();
        ioP3->ioIni(3);
        if(global_data.enIo_3 == 2) {
            ioP3->IOSet(global_data.io3St,3);  //发送模式初始化为 初始电平ioxst 在配置文件里改
        }

    }
    if(global_data.enIo_4 && hd_io) {

        COMMON_PRT("IO 4 enable\n");
        enIo4 = true;
        ioP4 = new io();
        ioP4->ioIni(4);
        if(global_data.enIo_4 == 2) {
            ioP4->IOSet(global_data.io4St,4);  //发送模式初始化为 初始电平ioxst 在配置文件里改
        }

    }
    if(global_data.enIo_5 && hd_io) {

        COMMON_PRT("IO 5 enable\n");
        enIo5 = true;
        ioP5 = new io();
        ioP5->ioIni(5);
        if(global_data.enIo_5 == 2) {
            ioP5->IOSet(global_data.io5St,5);  //发送模式初始化为 初始电平ioxst 在配置文件里改
        }

    }
    if(global_data.enIo_6 && hd_io) {

        COMMON_PRT("IO 6 enable\n");
        enIo6 = true;
        ioP6 = new io();
        ioP6->ioIni(6);
        if(global_data.enIo_6 == 2) {
            ioP6->IOSet(1,6);  //发送模式初始化为 1 一直拉高 给红外接收供电
        }

    }
    if(global_data.enIo_7&& (global_data.enInfrared == false) && hd_io) { //io7未初始化为红外，初始化为IO;红外优先级高于IO

        COMMON_PRT("IO 7 enable\n");
        enIo7 = true;
        ioP7 = new io();
        ioP7->ioIni(7);
        if(global_data.enIo_7 == 2) {
            ioP7->IOSet(global_data.io7St,7);  //发送模式初始化为 初始电平ioxst 在配置文件里改
        }

    }
#endif
    //7-UMP初始化
    umpP = new umpSet();
    umpP->umpIni();
printf("ump init ok\n");
    while(1) {
//        printf("whlile erro\n");
        FD_ZERO(&readFd);
        FD_SET(socket232,&readFd);  // 加入全局232socket
        FD_SET(socket485,&readFd);  // 加入全局485socket
        FD_SET(socketRelay,&readFd);// 加入全局socketRelay
        FD_SET(socketIO_A258,&readFd); //加入socketIO_A258
        if(en232 && hd_232) {
            FD_SET(rs232P->dev232Fd,&readFd);
            FD_SET(rs232P->socket232,&readFd);
            if(hd_232_edc) {
                FD_SET(rs232P->dev232Fd_edc,&readFd);
                FD_SET(rs232P->socket232_edc,&readFd);
            }
        }

        if(en2320 && hd_232) {
            FD_SET(rs2320P->dev232Fd,&readFd);

        }

        if(en2321 && hd_232) {
            FD_SET(rs2321P->dev232Fd,&readFd);

        }

        if(en2322 && hd_232) {
            FD_SET(rs2322P->dev232Fd,&readFd);

        }

        if(en2323 && hd_232) {
            FD_SET(rs2323P->dev232Fd,&readFd);

        }

        if(en485 && (false == hi3521d_7000) && hd_485) {
            FD_SET(rs485P->dev485Fd,&readFd);
            FD_SET(rs485P->socket485,&readFd);
        }

        if(en4850 && (false == hi3521d_7000) && hd_485) {
            FD_SET(rs4850P->dev485Fd,&readFd);
        }

        if(en4851 && (false == hi3521d_7000) && hd_485) {
            FD_SET(rs4851P->dev485Fd,&readFd);
        }

        if(en4852 && (false == hi3521d_7000) && hd_485) {
            FD_SET(rs4852P->dev485Fd,&readFd);
        }

        if(en4853 && (false == hi3521d_7000) && hd_485) {
            FD_SET(rs4853P->dev485Fd,&readFd);
        }

//        if(enRelay && (false == hi3521d_7000) && hd_relay) {
//            FD_SET(relayP->socketRelay,&readFd);
//        }

        if(enIr && hd_ir) {
            FD_SET(irP->devInfFd,&readFd);       //加入红外驱动文件描述符到监测集和 需要加上enIr_tc判断条件吗
            FD_SET(irP->socketInf,&readFd);
        }

//        if(enIr && hd_ir) {
//            FD_SET(irP->devInfFd,&readFd);
//        }
//printf("whlile erro2\n");
        //    FD_SET(socketIO_A258,&readFd); //加入socketIO_A258
//        if(enIo0 && hd_io) {
//            FD_SET(ioP0->socketIO,&readFd);
//        }

//        if(enIo1 && hd_io) {
//            FD_SET(ioP1->socketIO,&readFd);
//        }

        if(hd_232_edc && global_data.enIo_0) {
            FD_SET(ioP_edc->socketIO,&readFd);
        }

        FD_SET(umpP->socketUmp,&readFd);



 //       int len = read(irP->devInfFd,&tmpData_tc[2],MAXDATA); //一直读 让其一直被选上
        if ( select(maxFd,&readFd,NULL,NULL,0) <= 0 ){
            printf("select error\n");
            break;
        }

        if(en232 && hd_232) {
            if (FD_ISSET(rs232P->socket232,&readFd)) {
                rs232P->rcvData2uart();
                continue;
            }

            if(FD_ISSET(rs232P->dev232Fd,&readFd)) {
                rs232P->readUart2ctrl();
                continue;
            }


            if(hd_232_edc) {
                if (FD_ISSET(rs232P->socket232_edc,&readFd)) {
                    rs232P->rcvData2uart_edc();
                    continue;
                }

                if(FD_ISSET(rs232P->dev232Fd_edc,&readFd)) {
                    rs232P->readUart2ctrl_edc();
                    continue;
                }
            }
        }


        if(en2320 && hd_232) {
//            if (FD_ISSET(socket232,&readFd)) {
//                rs2320P->rcvData2uart();
//                continue;
//            }

            if(FD_ISSET(rs2320P->dev232Fd,&readFd)) {
                rs2320P->readUart2ctrl();
                continue;
            }
        }

        if(en2321 && hd_232) {
//            if (FD_ISSET(socket232,&readFd)) {
//                rs2321P->rcvData2uart();
//                continue;
//            }

            if(FD_ISSET(rs2321P->dev232Fd,&readFd)) {
                rs2321P->readUart2ctrl();
                continue;
            }
        }

        if(en2322 && hd_232) {
//            if (FD_ISSET(socket232,&readFd)) {
//                rs2322P->rcvData2uart();
//                continue;
//            }

            if(FD_ISSET(rs2322P->dev232Fd,&readFd)) {
                rs2322P->readUart2ctrl();
                continue;
            }
        }

        if(en2323 && hd_232) {
//            if (FD_ISSET(socket232,&readFd)) {
//                rs2323P->rcvData2uart();
//                continue;
//            }

            if(FD_ISSET(rs2323P->dev232Fd,&readFd)) {
                rs2323P->readUart2ctrl();
                continue;
            }
        }


        if(1){

            if (FD_ISSET(socket232,&readFd)) {
                rcvData2uart_fun();
                continue;
            }
        }


        if(en485 && (false == hi3521d_7000) && hd_485) {
            if(FD_ISSET(rs485P->socket485,&readFd)) {
                rs485P->rcvData2uart();
                continue;
            }

            if(FD_ISSET(rs485P->dev485Fd,&readFd)) {
                rs485P->readUart2ctrl();
                continue;
            }
        }

        if(en4850 && (false == hi3521d_7000) && hd_485) {
//            if(FD_ISSET(socket485,&readFd)) {
//                rs4850P->rcvData2uart();
//                continue;
//            }

            if(FD_ISSET(rs4850P->dev485Fd,&readFd)) {
                rs4850P->readUart2ctrl();
                continue;
            }
        }

        if(en4851 && (false == hi3521d_7000) && hd_485) {
//            if(FD_ISSET(socket485,&readFd)) {
//                rs4851P->rcvData2uart();
//                continue;
//            }

            if(FD_ISSET(rs4851P->dev485Fd,&readFd)) {
                rs4851P->readUart2ctrl();
                continue;
            }
        }

        if(en4852 && (false == hi3521d_7000) && hd_485) {
//            if(FD_ISSET(socket485,&readFd)) {
//                rs4852P->rcvData2uart();
//                continue;
//            }

            if(FD_ISSET(rs4852P->dev485Fd,&readFd)) {
                rs4852P->readUart2ctrl();
                continue;
            }
        }

        if(en4853 && (false == hi3521d_7000) && hd_485) {
//            if(FD_ISSET(socket485,&readFd)) {
//                rs4853P->rcvData2uart();
//                continue;
//            }

            if(FD_ISSET(rs4853P->dev485Fd,&readFd)) {
                rs4853P->readUart2ctrl();
                continue;
            }
        }

        if(1){
            if(FD_ISSET(socket485,&readFd)) {
                rcvData2uart485_fun();
                continue;
            }
        }


//        if(enRelay && (false ==  hi3521d_7000) && hd_relay) {
//            if(FD_ISSET(relayP->socketRelay,&readFd)) {
//                relayP->rcvData2Relay();
//                continue;
//            }
//        }

        if(enRelay0||enRelay1) {
            if(FD_ISSET(socketRelay,&readFd)) { //同IO 一直监听
                rcvData2Relay_fun();
                continue;
            }
        }


        if(enIr && hd_ir) {
            if(FD_ISSET(irP->socketInf,&readFd)) {
                irP->rcvData2Ir();
                //irP->rcvData2Ir_tc();
                continue;
            }
            if(FD_ISSET(irP->devInfFd,&readFd)) {
                irP->readData2Ir_tc();
            continue;
            }
        }

//        if(enIr && hd_ir){
//            if(FD_ISSET(irP->devInfFd,&readFd)) {
//                irP->readData2Ir_tc();
//            continue;
//            }

//        }


        if(1) {
            if(FD_ISSET(socketIO_A258,&readFd)) {
                //printf("socketIO_A258 has been set\r\n");
                //ioP0->rcvData2IO();
                rcvData2IO_fun();
//                if("IO_0=1")
//                    ioP0->IOSet(1, 0);
//                else if("IO_1=1")
//                    ioP1->IOSet(1, 1);

                continue;
            }
            else
            {
                //printf("socketIO_A258 has  not been set\r\n");
            }
        }

        if(hd_232_edc && global_data.enIo_0) {
            if(FD_ISSET(ioP_edc->socketIO,&readFd)) {
                ioP_edc->rcvData2IO_edc();
                continue;
            }
        }

//        if(enIo1 && hd_io) {
//            if(FD_ISSET(ioP1->socketIO,&readFd)) {
//                ioP1->rcvData2IO();
//                continue;
//            }
//        }
//enio2?
        if(FD_ISSET(umpP->socketUmp,&readFd)) {
            umpFlag = umpP->rcvData();
            switch (umpFlag) {
                case DEL:
                    thisini->reset();
                    break;
                case REPLY:
                    umpP->relyUMP();
                    break;
                case PARAM:
//UMP设置控制端回复IP和端口号
                    memset(global_data.ctlip,0,32);
                    strcpy(global_data.ctlip,umpP->umpData.ctlip);
                    global_data.ctl232port =  umpP->umpData.ctl232port;
                    global_data.ctl485port = umpP->umpData.ctl485port;
                    global_data.ctl2320port =  umpP->umpData.ctl2320port;
                    global_data.ctl2321port =  umpP->umpData.ctl2321port;
                    global_data.ctl2322port =  umpP->umpData.ctl2322port;
                    global_data.ctl2323port =  umpP->umpData.ctl2323port;
                    global_data.ctl4850port = umpP->umpData.ctl4850port;
                    global_data.ctl4851port = umpP->umpData.ctl4851port;
                    global_data.ctl4852port = umpP->umpData.ctl4852port;
                    global_data.ctl4853port = umpP->umpData.ctl4853port;
                    global_data.ctlrelayport = umpP->umpData.ctlrelayport;
                    global_data.ctlrelay0port = umpP->umpData.ctlrelay0port;
                    global_data.ctlrelay1port = umpP->umpData.ctlrelay1port;
                    global_data.ctlinfraredport = umpP->umpData.ctlinfraredport;
                    global_data.ctlIO0port = umpP->umpData.ctlIO0port;
                    global_data.ctlIO1port = umpP->umpData.ctlIO1port;
                    global_data.ctlIO2port = umpP->umpData.ctlIO2port;
                    global_data.ctlIO3port = umpP->umpData.ctlIO3port;
                    global_data.ctlIO4port = umpP->umpData.ctlIO4port;
                    global_data.ctlIO5port = umpP->umpData.ctlIO5port;
                    global_data.ctlIO6port = umpP->umpData.ctlIO6port;
                    global_data.ctlIO7port = umpP->umpData.ctlIO7port;

//                    printf("**********umpData.ctl4850port: %d\n",umpP->umpData.ctl4850port);
//                    printf("**********umpData.ctlrelay0port: %d\n",umpP->umpData.ctlrelay0port);
//                    printf("**********umpData.ctlrelay1port: %d\n",umpP->umpData.ctlrelay1port);

                    thisini->SetConfig(SoftwareConfig::kctlip,global_data.ctlip);
//                    thisini->SetConfig(SoftwareConfig::kctl232port,global_data.ctl232port);
//                    thisini->SetConfig(SoftwareConfig::kctl485port,global_data.ctl485port);
                    thisini->SetConfig(SoftwareConfig::kctl2320port,global_data.ctl2320port);
                    thisini->SetConfig(SoftwareConfig::kctl2321port,global_data.ctl2321port);
                    thisini->SetConfig(SoftwareConfig::kctl2322port,global_data.ctl2322port);
                    thisini->SetConfig(SoftwareConfig::kctl2323port,global_data.ctl2323port);
                    thisini->SetConfig(SoftwareConfig::kctl4850port,global_data.ctl4850port);
                    thisini->SetConfig(SoftwareConfig::kctl4851port,global_data.ctl4851port);
                    thisini->SetConfig(SoftwareConfig::kctl4852port,global_data.ctl4852port);
                    thisini->SetConfig(SoftwareConfig::kctl4853port,global_data.ctl4853port);
                    thisini->SetConfig(SoftwareConfig::kctlrelayport,global_data.ctlrelayport);
                    thisini->SetConfig(SoftwareConfig::kctlrelay0port,global_data.ctlrelay0port);
                    thisini->SetConfig(SoftwareConfig::kctlrelay1port,global_data.ctlrelay1port);
                    thisini->SetConfig(SoftwareConfig::kctlinfraredport,global_data.ctlinfraredport);
                    thisini->SetConfig(SoftwareConfig::kctlIO0port,global_data.ctlIO0port);
                    thisini->SetConfig(SoftwareConfig::kctlIO1port,global_data.ctlIO1port);
                    thisini->SetConfig(SoftwareConfig::kctlIO2port,global_data.ctlIO2port);
                    thisini->SetConfig(SoftwareConfig::kctlIO3port,global_data.ctlIO3port);
                    thisini->SetConfig(SoftwareConfig::kctlIO4port,global_data.ctlIO4port);
                    thisini->SetConfig(SoftwareConfig::kctlIO5port,global_data.ctlIO5port);
                    thisini->SetConfig(SoftwareConfig::kctlIO6port,global_data.ctlIO6port);
                    thisini->SetConfig(SoftwareConfig::kctlIO7port,global_data.ctlIO7port);

                    global_data.tcp_en = umpP->umpData.tcp_en;
                    global_data.tcp_port = umpP->umpData.tcp_port;
                    global_data.tcp_link_max = umpP->umpData.tcp_link_max;
                    thisini->SetConfig(SoftwareConfig::kTcpEnable, global_data.tcp_en);
                    thisini->SetConfig(SoftwareConfig::kTcpServerPort, global_data.tcp_port);
                    thisini->SetConfig(SoftwareConfig::kTcpLinkMax, global_data.tcp_link_max);

//UMP设置RS232
                    if((global_data.en232 != umpP->umpData.en232) && hd_232) {
                        if(en232) {
                            delete rs232P;
                            en232 = false;
                        }
                        else {
                            rs232P = new rs232();
                            rs232P->iniRs232();
                            en232 = true;
                        }
                        global_data.en232 = umpP->umpData.en232;
                    }

                    memset(global_data.port232,0,8);
                    strcpy(global_data.port232,umpP->umpData.port232);
                    global_data.uartspeed232 = umpP->umpData.uartspeed232;
                    global_data.nbit232 = umpP->umpData.nbit232;
                    global_data.nstop232 = umpP->umpData.nstop232;
                    global_data.event232[0] = umpP->umpData.event232[0];
                    global_data.nVtime232 = umpP->umpData.nVtime232;
                    global_data.nVmin232 = umpP->umpData.nVmin232;

                    thisini->SetConfig(SoftwareConfig::ken232,global_data.en232);
                    thisini->SetConfig(SoftwareConfig::k232port,global_data.port232);
                    thisini->SetConfig(SoftwareConfig::k232uartspeed,global_data.uartspeed232);
                    thisini->SetConfig(SoftwareConfig::k232nbit,global_data.nbit232);
                    thisini->SetConfig(SoftwareConfig::k232nstop,global_data.nstop232);
                    thisini->SetConfig(SoftwareConfig::k232event,global_data.event232);
                    thisini->SetConfig(SoftwareConfig::k232vtime,global_data.nVtime232);
                    thisini->SetConfig(SoftwareConfig::k232vmin,global_data.nVmin232);

                    if(en232 && hd_232) {
                        rs232P->UMPsetParam(false,false,true);
                    }

             //2320
//                    printf("**********global_data.en2320 is: %d\n",global_data.en2320);
//                    printf("**********umpData.en2320 is: %d\n",umpP->umpData.en2320);
//                    printf("**********global_data.nbit2320 is: %d\n",global_data.nbit2320);
//                    printf("**********umpData.nbit2320 is: %d\n",umpP->umpData.nbit2320);
                    if((global_data.en2320 != umpP->umpData.en2320) && hd_232) {
                        if(en2320) {
                            delete rs2320P;
                            en2320 = false;
                        }
                        else {
                            rs2320P = new rs2320();
                            rs2320P->iniRs232();
                            en2320 = true;
                        }
                        global_data.en2320 = umpP->umpData.en2320;

                    }

                    memset(global_data.port2320,0,8);
                    strcpy(global_data.port2320,umpP->umpData.port2320);
                    global_data.uartspeed2320 = umpP->umpData.uartspeed2320;
                    global_data.nbit2320 = umpP->umpData.nbit2320;
                    global_data.nstop2320 = umpP->umpData.nstop2320;
                    global_data.event2320[0] = umpP->umpData.event2320[0];
                    global_data.nVtime2320 = umpP->umpData.nVtime2320;
                    global_data.nVmin2320 = umpP->umpData.nVmin2320;

                    thisini->SetConfig(SoftwareConfig::ken2320,global_data.en2320);
                    //printf("-----global_data.en2320:%d\n",global_data.en2320);
                    thisini->SetConfig(SoftwareConfig::k2320port,global_data.port2320);
                    thisini->SetConfig(SoftwareConfig::k2320uartspeed,global_data.uartspeed2320);
                    thisini->SetConfig(SoftwareConfig::k2320nbit,global_data.nbit2320);
                    thisini->SetConfig(SoftwareConfig::k2320nstop,global_data.nstop2320);
                    thisini->SetConfig(SoftwareConfig::k2320event,global_data.event2320);
                    thisini->SetConfig(SoftwareConfig::k2320vtime,global_data.nVtime2320);
                    thisini->SetConfig(SoftwareConfig::k2320vmin,global_data.nVmin2320);

                    if(en2320 && hd_232) {
                        rs2320P->UMPsetParam(false,false,true);
                    }


                    //2321
                   if((global_data.en2321 != umpP->umpData.en2321) && hd_232) {
                       if(en2321) {
                           delete rs2321P;
                           en2321 = false;
                       }
                       else {
                           rs2321P = new rs2321();
                           rs2321P->iniRs232();
                           en2321 = true;
                       }
                       global_data.en2321 = umpP->umpData.en2321;
                   }

                   memset(global_data.port2321,0,8);
                   strcpy(global_data.port2321,umpP->umpData.port2321);
                   global_data.uartspeed2321 = umpP->umpData.uartspeed2321;
                   global_data.nbit2321 = umpP->umpData.nbit2321;
                   global_data.nstop2321 = umpP->umpData.nstop2321;
                   global_data.event2321[0] = umpP->umpData.event2321[0];
                   global_data.nVtime2321 = umpP->umpData.nVtime2321;
                   global_data.nVmin2321 = umpP->umpData.nVmin2321;

                   thisini->SetConfig(SoftwareConfig::ken2321,global_data.en2321);
                   thisini->SetConfig(SoftwareConfig::k2321port,global_data.port2321);
                   thisini->SetConfig(SoftwareConfig::k2321uartspeed,global_data.uartspeed2321);
                   thisini->SetConfig(SoftwareConfig::k2321nbit,global_data.nbit2321);
                   thisini->SetConfig(SoftwareConfig::k2321nstop,global_data.nstop2321);
                   thisini->SetConfig(SoftwareConfig::k2321event,global_data.event2321);
                   thisini->SetConfig(SoftwareConfig::k2321vtime,global_data.nVtime2321);
                   thisini->SetConfig(SoftwareConfig::k2321vmin,global_data.nVmin2321);

                   if(en2321 && hd_232) {
                       rs2321P->UMPsetParam(false,false,true);
                   }


                       //2322
                  if((global_data.en2322 != umpP->umpData.en2322) && hd_232) {
                      if(en2322) {
                          delete rs2322P;
                          en2322 = false;
                      }
                      else {
                          rs2322P = new rs2322();
                          rs2322P->iniRs232();
                          en2322 = true;
                      }
                      global_data.en2322 = umpP->umpData.en2322;
                  }

                  memset(global_data.port2322,0,8);
                  strcpy(global_data.port2322,umpP->umpData.port2322);
                  global_data.uartspeed2322 = umpP->umpData.uartspeed2322;
                  global_data.nbit2322 = umpP->umpData.nbit2322;
                  global_data.nstop2322 = umpP->umpData.nstop2322;
                  global_data.event2322[0] = umpP->umpData.event2322[0];
                  global_data.nVtime2322 = umpP->umpData.nVtime2322;
                  global_data.nVmin2322 = umpP->umpData.nVmin2322;

                  thisini->SetConfig(SoftwareConfig::ken2322,global_data.en2322);
                  thisini->SetConfig(SoftwareConfig::k2322port,global_data.port2322);
                  thisini->SetConfig(SoftwareConfig::k2322uartspeed,global_data.uartspeed2322);
                  thisini->SetConfig(SoftwareConfig::k2322nbit,global_data.nbit2322);
                  thisini->SetConfig(SoftwareConfig::k2322nstop,global_data.nstop2322);
                  thisini->SetConfig(SoftwareConfig::k2322event,global_data.event2322);
                  thisini->SetConfig(SoftwareConfig::k2322vtime,global_data.nVtime2322);
                  thisini->SetConfig(SoftwareConfig::k2322vmin,global_data.nVmin2322);

                  if(en2322 && hd_232) {
                      rs2322P->UMPsetParam(false,false,true);
                  }


          //2323
                 if((global_data.en2323 != umpP->umpData.en2323) && hd_232) {
                     if(en2323) {
                         delete rs2323P;
                         en2323 = false;
                     }
                     else {
                         rs2323P = new rs2323();
                         rs2323P->iniRs232();
                         en2323 = true;
                     }
                     global_data.en2323 = umpP->umpData.en2323;
                 }

                 memset(global_data.port2323,0,8);
                 strcpy(global_data.port2323,umpP->umpData.port2323);
                 global_data.uartspeed2323 = umpP->umpData.uartspeed2323;
                 global_data.nbit2323 = umpP->umpData.nbit2323;
                 global_data.nstop2323 = umpP->umpData.nstop2323;
                 global_data.event2323[0] = umpP->umpData.event2323[0];
                 global_data.nVtime2323 = umpP->umpData.nVtime2323;
                 global_data.nVmin2323 = umpP->umpData.nVmin2323;

                 thisini->SetConfig(SoftwareConfig::ken2323,global_data.en2323);
                 //printf("-----global_data.en2323:%d\n",global_data.en2323);
                 thisini->SetConfig(SoftwareConfig::k2323port,global_data.port2323);
                 thisini->SetConfig(SoftwareConfig::k2323uartspeed,global_data.uartspeed2323);
                 thisini->SetConfig(SoftwareConfig::k2323nbit,global_data.nbit2323);
                 thisini->SetConfig(SoftwareConfig::k2323nstop,global_data.nstop2323);
                 thisini->SetConfig(SoftwareConfig::k2323event,global_data.event2323);
                 thisini->SetConfig(SoftwareConfig::k2323vtime,global_data.nVtime2323);
                 thisini->SetConfig(SoftwareConfig::k2323vmin,global_data.nVmin2323);

                 if(en2323 && hd_232) {
                     rs2323P->UMPsetParam(false,false,true);
                 }


//UMP设置RS485
                    if(false ==  hi3521d_7000) {
                        if((global_data.en485 != umpP->umpData.en485) && hd_485) {
                            if(en485) {
                                delete rs485P;
                                en485 = false;
                            }
                            else {
                                rs485P = new rs485();
                                rs485P->iniRs485();
                                en485 = true;
                            }
                            global_data.en485 = umpP->umpData.en485;
                        }
                        memset(global_data.port485,0,8);
                        strcpy(global_data.port485,umpP->umpData.port485);
                        global_data.uartspeed485 = umpP->umpData.uartspeed485;
                        global_data.nbit485 = umpP->umpData.nbit485;
                        global_data.nstop485 = umpP->umpData.nstop485;
                        global_data.event485[0] = umpP->umpData.event485[0];
                        global_data.nVtime485 = umpP->umpData.nVtime485;
                        global_data.nVmin485 = umpP->umpData.nVmin485;

                        thisini->SetConfig(SoftwareConfig::ken485,global_data.en485);
                        thisini->SetConfig(SoftwareConfig::k485port,global_data.port485);
                        thisini->SetConfig(SoftwareConfig::k485uartspeed,global_data.uartspeed485);
                        thisini->SetConfig(SoftwareConfig::k485nbit,global_data.nbit485);
                        thisini->SetConfig(SoftwareConfig::k485nstop,global_data.nstop485);
                        thisini->SetConfig(SoftwareConfig::k485event,global_data.event485);
                        thisini->SetConfig(SoftwareConfig::k485vtime,global_data.nVtime485);
                        thisini->SetConfig(SoftwareConfig::k485vmin,global_data.nVmin485);

                        if(en485 && hd_485) {
                            rs485P->UMPsetParam(false,false,true);
                        }
                    }


                    //4850
                    if((global_data.en4850 != umpP->umpData.en4850) && hd_485) {
                        if(en4850) {
                            delete rs4850P;
                            en4850 = false;
                        }
                        else {
                            rs4850P = new rs4850();
                            rs4850P->iniRs485();
                            en4850 = true;
                        }
                        global_data.en4850 = umpP->umpData.en4850;
                    }
                    memset(global_data.port4850,0,8);
                    strcpy(global_data.port4850,umpP->umpData.port4850);
                    global_data.uartspeed4850 = umpP->umpData.uartspeed4850;
                    global_data.nbit4850 = umpP->umpData.nbit4850;
                    global_data.nstop4850 = umpP->umpData.nstop4850;
                    global_data.event4850[0] = umpP->umpData.event4850[0];
                    global_data.nVtime4850 = umpP->umpData.nVtime4850;
                    global_data.nVmin4850 = umpP->umpData.nVmin4850;

                    thisini->SetConfig(SoftwareConfig::ken4850,global_data.en4850);
                    thisini->SetConfig(SoftwareConfig::k4850port,global_data.port4850);
                    thisini->SetConfig(SoftwareConfig::k4850uartspeed,global_data.uartspeed4850);
                    thisini->SetConfig(SoftwareConfig::k4850nbit,global_data.nbit4850);
                    thisini->SetConfig(SoftwareConfig::k4850nstop,global_data.nstop4850);
                    thisini->SetConfig(SoftwareConfig::k4850event,global_data.event4850);
                    thisini->SetConfig(SoftwareConfig::k4850vtime,global_data.nVtime4850);
                    thisini->SetConfig(SoftwareConfig::k4850vmin,global_data.nVmin4850);

                    if(en4850 && hd_485) {
                        rs4850P->UMPsetParam(false,false,true);
                    }

                    //4851
                    if((global_data.en4851 != umpP->umpData.en4851) && hd_485) {
                        if(en4851) {
                            delete rs4851P;
                            en4851 = false;
                        }
                        else {
                            rs4851P = new rs4851();
                            rs4851P->iniRs485();
                            en4851 = true;
                        }
                        global_data.en4851 = umpP->umpData.en4851;
                    }
                    memset(global_data.port4851,0,8);
                    strcpy(global_data.port4851,umpP->umpData.port4851);
                    global_data.uartspeed4851 = umpP->umpData.uartspeed4851;
                    global_data.nbit4851 = umpP->umpData.nbit4851;
                    global_data.nstop4851 = umpP->umpData.nstop4851;
                    global_data.event4851[0] = umpP->umpData.event4851[0];
                    global_data.nVtime4851 = umpP->umpData.nVtime4851;
                    global_data.nVmin4851 = umpP->umpData.nVmin4851;

                    thisini->SetConfig(SoftwareConfig::ken4851,global_data.en4851);
                    thisini->SetConfig(SoftwareConfig::k4851port,global_data.port4851);
                    thisini->SetConfig(SoftwareConfig::k4851uartspeed,global_data.uartspeed4851);
                    thisini->SetConfig(SoftwareConfig::k4851nbit,global_data.nbit4851);
                    thisini->SetConfig(SoftwareConfig::k4851nstop,global_data.nstop4851);
                    thisini->SetConfig(SoftwareConfig::k4851event,global_data.event4851);
                    thisini->SetConfig(SoftwareConfig::k4851vtime,global_data.nVtime4851);
                    thisini->SetConfig(SoftwareConfig::k4851vmin,global_data.nVmin4851);

                    if(en4851 && hd_485) {
                        rs4851P->UMPsetParam(false,false,true);
                    }


                    //4852
                    if((global_data.en4852 != umpP->umpData.en4852) && hd_485) {
                        if(en4852) {
                            delete rs4852P;
                            en4852 = false;
                        }
                        else {
                            rs4852P = new rs4852();
                            rs4852P->iniRs485();
                            en4852 = true;
                        }
                        global_data.en4852 = umpP->umpData.en4852;
                    }
                    memset(global_data.port4852,0,8);
                    strcpy(global_data.port4852,umpP->umpData.port4852);
                    global_data.uartspeed4852 = umpP->umpData.uartspeed4852;
                    global_data.nbit4852 = umpP->umpData.nbit4852;
                    global_data.nstop4852 = umpP->umpData.nstop4852;
                    global_data.event4852[0] = umpP->umpData.event4852[0];
                    global_data.nVtime4852 = umpP->umpData.nVtime4852;
                    global_data.nVmin4852 = umpP->umpData.nVmin4852;

                    thisini->SetConfig(SoftwareConfig::ken4852,global_data.en4852);
                    thisini->SetConfig(SoftwareConfig::k4852port,global_data.port4852);
                    thisini->SetConfig(SoftwareConfig::k4852uartspeed,global_data.uartspeed4852);
                    thisini->SetConfig(SoftwareConfig::k4852nbit,global_data.nbit4852);
                    thisini->SetConfig(SoftwareConfig::k4852nstop,global_data.nstop4852);
                    thisini->SetConfig(SoftwareConfig::k4852event,global_data.event4852);
                    thisini->SetConfig(SoftwareConfig::k4852vtime,global_data.nVtime4852);
                    thisini->SetConfig(SoftwareConfig::k4852vmin,global_data.nVmin4852);

                    if(en4852 && hd_485) {
                        rs4852P->UMPsetParam(false,false,true);
                    }

                    //4853
                    if((global_data.en4853 != umpP->umpData.en4853) && hd_485) {
                        if(en4853) {
                            delete rs4853P;
                            en4853 = false;
                        }
                        else {
                            rs4853P = new rs4853();
                            rs4853P->iniRs485();
                            en4853 = true;
                        }
                        global_data.en4853 = umpP->umpData.en4853;
                    }
                    memset(global_data.port4853,0,8);
                    strcpy(global_data.port4853,umpP->umpData.port4853);
                    global_data.uartspeed4853 = umpP->umpData.uartspeed4853;
                    global_data.nbit4853 = umpP->umpData.nbit4853;
                    global_data.nstop4853 = umpP->umpData.nstop4853;
                    global_data.event4853[0] = umpP->umpData.event4853[0];
                    global_data.nVtime4853 = umpP->umpData.nVtime4853;
                    global_data.nVmin4853 = umpP->umpData.nVmin4853;

                    thisini->SetConfig(SoftwareConfig::ken4853,global_data.en4853);
                    thisini->SetConfig(SoftwareConfig::k4853port,global_data.port4853);
                    thisini->SetConfig(SoftwareConfig::k4853uartspeed,global_data.uartspeed4853);
                    thisini->SetConfig(SoftwareConfig::k4853nbit,global_data.nbit4853);
                    thisini->SetConfig(SoftwareConfig::k4853nstop,global_data.nstop4853);
                    thisini->SetConfig(SoftwareConfig::k4853event,global_data.event4853);
                    thisini->SetConfig(SoftwareConfig::k4853vtime,global_data.nVtime4853);
                    thisini->SetConfig(SoftwareConfig::k4853vmin,global_data.nVmin4853);

                    if(en4853 && hd_485) {
                        rs4853P->UMPsetParam(false,false,true);
                    }

//UMP设置relay
                    if(false == hi3521d_7000) {
                        if((global_data.enRelay != umpP->umpData.enRelay) && hd_relay) {
                            if(enRelay) {
                                delete relayP;
                                enRelay = false;
                            }
                            else {
                                relayP =  new relay();
                                relayP->relayIni();
                                enRelay = true;
                            }
                            global_data.enRelay = umpP->umpData.enRelay;
                        }

                        thisini->SetConfig(SoftwareConfig::kenrelay,global_data.enRelay);
                    }
                //relay0
                    if(false == hi3521d_7000) {
                        if((global_data.enRelay0 != umpP->umpData.enRelay0) && hd_relay) {
                            if(enRelay0) {
                                delete relayP0;
                                enRelay0 = false;
                            }
                            else {
                                relayP0 =  new relay();
                                relayP0->relayIni();
                                enRelay0 = true;
                            }
                            global_data.enRelay0 = umpP->umpData.enRelay0;
                        }

                        thisini->SetConfig(SoftwareConfig::kenrelay0,global_data.enRelay0);
                    }
               //relay1
                    if(false == hi3521d_7000) {
                        if((global_data.enRelay1 != umpP->umpData.enRelay1) && hd_relay) {
                            if(enRelay1) {
                                delete relayP1;
                                enRelay1 = false;
                            }
                            else {
                                relayP1 =  new relay();
                                relayP1->relayIni();
                                enRelay1 = true;
                            }
                            global_data.enRelay1 = umpP->umpData.enRelay1;
                        }

                        thisini->SetConfig(SoftwareConfig::kenrelay1,global_data.enRelay1);
                    }

//UMP设置红外
                    if((global_data.enInfrared != umpP->umpData.enInfrared) && hd_ir) {
                        if(enIr) {
                            if(umpP->umpData.enInfrared) {      //UMP切换模式
                                delete irP;
                                usleep(20000);
                                irP = new infrared();
                                irP->infraredIni(umpP->umpData.enInfrared);
                            }
                            else {
                                delete irP;
                                enIr = false;
                            }
                        }
                        else {
                            irP = new infrared();
                            irP->infraredIni(umpP->umpData.enInfrared);
                            enIr = true;
                        }
                        global_data.enInfrared = umpP->umpData.enInfrared;
                    }
                    if(!irTime) {
                        memset(global_data.portInfrared,0,8);
                        strcpy(global_data.portInfrared,umpP->umpData.portInfrared);
                        global_data.uartspeedInf = umpP->umpData.uartspeedInf;
                        global_data.nbitInf = umpP->umpData.nbitInf;
                        global_data.nstopInf = umpP->umpData.nstopInf;
                        global_data.eventInf[0] = umpP->umpData.eventInf[0];
                        global_data.nVtimeInf = umpP->umpData.nVtimeInf;
                        global_data.nVminInf = umpP->umpData.nVminInf;


                        thisini->SetConfig(SoftwareConfig::kInfraredport,global_data.portInfrared);
                        thisini->SetConfig(SoftwareConfig::kInfrareduartspeed,global_data.uartspeedInf);
                        thisini->SetConfig(SoftwareConfig::kInfrarednbit,global_data.nbitInf);
                        thisini->SetConfig(SoftwareConfig::kInfrarednstop,global_data.nstopInf);
                        thisini->SetConfig(SoftwareConfig::kInfraredevent,global_data.eventInf);
                        thisini->SetConfig(SoftwareConfig::kInfraredvtime,global_data.nVtimeInf);
                        thisini->SetConfig(SoftwareConfig::kInfraredvmin,global_data.nVminInf);

                        if(enIr && hd_ir) {
                            irP->UMPsetParam(true,false,false);
                        }
                    }
                    thisini->SetConfig(SoftwareConfig::kenInfrared,global_data.enInfrared);

//设置IO
                    if((global_data.enIo_0 != umpP->umpData.enIo_0) && hd_io) {
                        if(enIo0) {
                            if(umpP->umpData.enIo_0) {
                                global_data.enIo_0 = umpP->umpData.enIo_0;
                                delete ioP0;
                                ioP0 = new io();
                                ioP0->ioIni(0);
                            }
                            else {
                                global_data.enIo_0 = umpP->umpData.enIo_0;
                                delete ioP0;
                                enIo0 = false;
                            }
                        }
                        else {
                            global_data.enIo_0 = umpP->umpData.enIo_0;
                            ioP0 = new io();
                            ioP0->ioIni(0);
                            enIo0 = true;
                        }
                    }

                    if((global_data.enIo_0 != umpP->umpData.enIo_0) && hd_232_edc) {
                        if(global_data.enIo_0) {
                            global_data.enIo_0 = umpP->umpData.enIo_0;
                            delete ioP_edc;
                        }
                        else {
                            global_data.enIo_0 = umpP->umpData.enIo_0;
                            ioP_edc = new io_edc;
                            ioP_edc->ioIni_edc();
                        }
                    }
                    thisini->SetConfig(SoftwareConfig::kSet0IO,global_data.enIo_0);

                    if((global_data.enIo_1 != umpP->umpData.enIo_1) && hd_io) {
                        if(enIo1) {
                            if(umpP->umpData.enIo_1) {
                                global_data.enIo_1 = umpP->umpData.enIo_1;
                                delete ioP1;
                                ioP1 = new io();
                                ioP1->ioIni(1);
                            }
                            else {
                                global_data.enIo_1 = umpP->umpData.enIo_1;
                                delete ioP1;
                                enIo1 = false;
                            }
                        }
                        else {
                            global_data.enIo_1 = umpP->umpData.enIo_1;
                            ioP1 = new io();
                            ioP1->ioIni(1);
                            enIo1 = true;
                        }
                    }
                    thisini->SetConfig(SoftwareConfig::kSet1IO,global_data.enIo_1);

                    if((global_data.enIo_2 != umpP->umpData.enIo_2) && hd_io) {
                        if(enIo2) {
                            if(umpP->umpData.enIo_2) {
                                global_data.enIo_2 = umpP->umpData.enIo_2;
                                delete ioP2;
                                ioP2 = new io();
                                ioP2->ioIni(2);
                            }
                            else {
                                global_data.enIo_2 = umpP->umpData.enIo_2;
                                delete ioP2;
                                enIo2 = false;
                            }
                        }
                        else {
                            global_data.enIo_2 = umpP->umpData.enIo_2;
                            ioP2 = new io();
                            ioP2->ioIni(2);
                            enIo2 = true;
                        }
                    }
                    thisini->SetConfig(SoftwareConfig::kSet2IO,global_data.enIo_2);

                    if((global_data.enIo_3 != umpP->umpData.enIo_3) && hd_io) {
                        if(enIo3) {
                            if(umpP->umpData.enIo_3) {
                                global_data.enIo_3 = umpP->umpData.enIo_3;
                                delete ioP3;
                                ioP3 = new io();
                                ioP3->ioIni(3);
                            }
                            else {
                                global_data.enIo_3 = umpP->umpData.enIo_3;
                                delete ioP3;
                                enIo3 = false;
                            }
                        }
                        else {
                            global_data.enIo_3 = umpP->umpData.enIo_3;
                            ioP3 = new io();
                            ioP3->ioIni(3);
                            enIo3 = true;
                        }
                    }
                    thisini->SetConfig(SoftwareConfig::kSet3IO,global_data.enIo_3);

                    if((global_data.enIo_4 != umpP->umpData.enIo_4) && hd_io) {
                        if(enIo4) {
                            if(umpP->umpData.enIo_4) {
                                global_data.enIo_4 = umpP->umpData.enIo_4;
                                delete ioP4;
                                ioP4 = new io();
                                ioP4->ioIni(4);
                            }
                            else {
                                global_data.enIo_4 = umpP->umpData.enIo_4;
                                delete ioP4;
                                enIo4 = false;
                            }
                        }
                        else {
                            global_data.enIo_4 = umpP->umpData.enIo_4;
                            ioP4 = new io();
                            ioP4->ioIni(4);
                            enIo4 = true;
                        }
                    }
                    thisini->SetConfig(SoftwareConfig::kSet4IO,global_data.enIo_4);

                    if((global_data.enIo_5 != umpP->umpData.enIo_5) && hd_io) {
                        if(enIo5) {
                            if(umpP->umpData.enIo_5) {
                                global_data.enIo_5 = umpP->umpData.enIo_5;
                                delete ioP5;
                                ioP5 = new io();
                                ioP5->ioIni(5);
                            }
                            else {
                                global_data.enIo_5 = umpP->umpData.enIo_5;
                                delete ioP5;
                                enIo5 = false;
                            }
                        }
                        else {
                            global_data.enIo_5 = umpP->umpData.enIo_5;
                            ioP5 = new io();
                            ioP5->ioIni(5);
                            enIo5 = true;
                        }
                    }
                    thisini->SetConfig(SoftwareConfig::kSet5IO,global_data.enIo_5);

                    if((global_data.enIo_6 != umpP->umpData.enIo_6) && hd_io) {
                        if(enIo6) {
                            if(umpP->umpData.enIo_6) {
                                global_data.enIo_6 = umpP->umpData.enIo_6;
                                delete ioP6;
                                ioP6 = new io();
                                ioP6->ioIni(6);
                            }
                            else {
                                global_data.enIo_6 = umpP->umpData.enIo_6;
                                delete ioP6;
                                enIo6 = false;
                            }
                        }
                        else {
                            global_data.enIo_6 = umpP->umpData.enIo_6;
                            ioP6 = new io();
                            ioP6->ioIni(6);
                            enIo6 = true;
                        }
                    }
                    thisini->SetConfig(SoftwareConfig::kSet6IO,global_data.enIo_6);

                    if((global_data.enIo_7 != umpP->umpData.enIo_7) && hd_io) {
                        if(enIo7) {
                            if(umpP->umpData.enIo_7) {
                                global_data.enIo_7 = umpP->umpData.enIo_7;
                                delete ioP7;
                                ioP7 = new io();
                                ioP7->ioIni(7);
                            }
                            else {
                                global_data.enIo_7 = umpP->umpData.enIo_7;
                                delete ioP7;
                                enIo7 = false;
                            }
                        }
                        else {
                            global_data.enIo_7 = umpP->umpData.enIo_7;
                            ioP7 = new io();
                            ioP7->ioIni(7);
                            enIo7 = true;
                        }
                    }
                    thisini->SetConfig(SoftwareConfig::kSet7IO,global_data.enIo_7);

                    thisini->SaveConfig();
                    break;
                case SAME:
                    break;
                default:
                    break;
            }
            continue;
        }

    }
    return 0;
}
