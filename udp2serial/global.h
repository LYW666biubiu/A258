#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <syslog.h>
#include <signal.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>      /* inet(3) functions */
#include <net/if.h>         /* ifreq struct */
#include <sys/ioctl.h>      /* ioctl() functions */
#include <net/route.h>      /* RTF_GATEWAY */
#include <netdb.h>
#include <string.h>
#include <termios.h>
#include <linux/netlink.h>
#include <asm-generic/types.h>
#include <stdbool.h>
#include <pthread.h>
#include "singleton.h"
#include "common_A258.h"
#include "common.h"
#include "software_config.h"
//#include "udpsocket/networkinfo.h"

#define MAXDATA     512
#define UMPPORT     16123

#define ROUTEPATH "/proc/net/route"
#define ETH0      "eth0"
#define ETH1      "eth1"
#define ETHLO     "lo"

typedef enum {
    kHWADDR  = SIOCGIFHWADDR,
    kADDR    = SIOCGIFADDR,
    kBRDADDR = SIOCGIFBRDADDR,
    kNETMASK = SIOCGIFNETMASK,
    kALL
}IF_CONFIG;

#define HD_700K_NJ_V1   "SHNJ2301fd_100_V1"     //232 485 relay
#define HD_700K_NJ_V2   "SHNJ2301fd_100_V2"     //232 485 relay
#define HD_700K_NJ_V3   "SHNJ2301fd_100_V3"     //232 485 relay ir io
#define HD_D_HDMI_NJ    "SHNJ2301fd_110_V1"     //双层HDMI 232 485 relay

#define HD_4HDMI_NJ     "SHNJ2301fd_300_V1"     //232
#define HD_4MIX_NJ      "SHNJ2301fd_400_V1"     //232
#define HD_2SDI_NJ      "SHNJ23015d_400_V1"     //232  uart-ir
#define HD_300T_NJ      "SHNJ23015d_100_V2"    //232  uarr_ir


#define HD_700K_GZ      "SHGZ2301fd_300_V1"     //232 485 relay ir-gpio io
#define HD_700K_GZ_A    "SHGZ2301fa_100_V1"     //232 485 relay ir-gpio io
#define HD_700K_GZ_V2   "SHGZ2301fd_300_V2"     //232 485 relay ir-gpio io
#define HD_8000_GZ      "SHGZ2301fd_310_V1"     //232 485 relay ir-gpio io
#define HD_700S_GZ      "SHGZ230240_100_V1"     //232 485 relay ir-gpio io   --广州3536
#define HD_7000_NJ      "SHNJ23014d4_7000_V1"   //232 ir-gpio io
#define HD_7020_NJ      "SHNJ23014d4_7020_V1"   //232 ir-gpio io
#define HD_7000_NJ_HD   "SHNJ23014d4_7010_V1"   //232 ir-gpio io 南京硬环出7000
#define HD_7000_PTN     "SHDJ23014d4_7000_V1"   //232 ir-gpio io

#define A258_NJ         "SHNJ23014d2_100_V1"    //232 ir-gpio io relay 中控板

#define HD_21A_PTN      "SHDJ23015a_100_V1"     //232 ir-gpio io

#define HD_DEC_GZ       "SHGZ2301fd_400_V1"     //uart1-232,uart2-232  relay
#define HD_300R_GZ      "SHGZ2301fd_100_V1"
#ifdef __GPIO1_5__

#define IRTIMEHD    "SHGZ2301fd_300_V1"


#endif

#define IR_8000     "SHGZ2301fd_310_V1"

#ifdef __7000_NJ__
#define IRTIMEHD    "SHNJ23014d4_7000_V1"
#endif
//extern char tmpData_tc[MAXDATA];
extern int socketIO_A258;
extern int socketRelay;
extern int socket232;
extern int socket485;
extern struct sockaddr_in ctrlAddr_A258;
extern struct sockaddr_in ctrlAddr_A258_relay;
extern struct sockaddr_in ctrlAddr_A258_232;
extern struct sockaddr_in ctrlAddr_A258_485;
extern pthread_t tcp_pid;
// variable
extern char gClientIP[];
extern unsigned int  gClientPort;
extern int      gMCtrllProtocol;
typedef enum MEDIA_CTRL_PROTOCOL_E{
    kJSON    = 0,
    kSCODe,
} MEDIA_CTRL_PROTOCOL_E;

typedef struct configdata_
{
    char ctlip[32];
    char ctlip_tc[32];
    int ctl232port;
    int ctl485port;
    int ctl2320port;
    int ctl2321port;
    int ctl2322port;
    int ctl2323port;
    int ctl4850port;
    int ctl4851port;
    int ctl4852port;
    int ctl4853port;
    int ctlrelayport;  //需要增加成2个吗
    int ctlrelay0port;
    int ctlrelay1port;
    int ctlinfraredport;
    int ctlinfraredport_tc;
    int ctlIO0port;
    int ctlIO1port;
    int ctlIO2port;
    int ctlIO3port;
    int ctlIO4port;
    int ctlIO5port;
    int ctlIO6port;
    int ctlIO7port;
    int sv232port;
    int sv485port;
    int svrelayport;
    int svInfraredport;
    int svIOport;
    int svIOport_A258;
    int en232;
    char port232[8];
    long uartspeed232;
    int nbit232;
    int nstop232;
    char event232[4];
    int nVtime232;
    int nVmin232;
//A258 232
    int en2320;
    char port2320[8];
    long uartspeed2320;
    int nbit2320;
    int nstop2320;
    char event2320[4];
    int nVtime2320;
    int nVmin2320;
    int en2321;
    char port2321[8];
    long uartspeed2321;
    int nbit2321;
    int nstop2321;
    char event2321[4];
    int nVtime2321;
    int nVmin2321;
    int en2322;
    char port2322[8];
    long uartspeed2322;
    int nbit2322;
    int nstop2322;
    char event2322[4];
    int nVtime2322;
    int nVmin2322;
    int en2323;
    char port2323[8];
    long uartspeed2323;
    int nbit2323;
    int nstop2323;
    char event2323[4];
    int nVtime2323;
    int nVmin2323;
    int en485;
    char port485[8];
    long uartspeed485;
    int nbit485;
    int nstop485;
    char event485[4];
    int nVtime485;
    int nVmin485;
//A258 485
    int en4850;
    char port4850[8];
    long uartspeed4850;
    int nbit4850;
    int nstop4850;
    char event4850[4];
    int nVtime4850;
    int nVmin4850;
    int en4851;
    char port4851[8];
    long uartspeed4851;
    int nbit4851;
    int nstop4851;
    char event4851[4];
    int nVtime4851;
    int nVmin4851;
    int en4852;
    char port4852[8];
    long uartspeed4852;
    int nbit4852;
    int nstop4852;
    char event4852[4];
    int nVtime4852;
    int nVmin4852;
    int en4853;
    char port4853[8];
    long uartspeed4853;
    int nbit4853;
    int nstop4853;
    char event4853[4];
    int nVtime4853;
    int nVmin4853;
    int enRelay;
    int enRelay0;
    int enRelay1;
    int relaySt;    //1-on  0-off
    int relay0St;
    int relay1St;
    int enInfrared;
    int enInfrared_tc;
    char portInfrared[8];
    long uartspeedInf;
    int nbitInf;
    int nstopInf;
    char eventInf[4];
    int nVtimeInf;
    int nVminInf;
    int enIo_0;
    int io0St;
    int enIo_1;
    int io1St;
    int enIo_2;
    int enIo_3;
    int enIo_4;
    int enIo_5;
    int enIo_6;
    int enIo_7;
    int io2St;
    int io3St;
    int io4St;
    int io5St;
    int io6St;
    int io7St;
    int tcp_en;
    int tcp_port;
    int tcp_link_max;
}CONFIGDATA;


typedef struct uartParam
{
    char port[8];
    long speed;
    int nbit;
    int nstop;
    int event;
    int Vtime;
    int Vmin;
}UARTPARAM;

typedef enum {
    NJ_700K = 0,
    NJ_D_700K,      //双层HDMI
    NJ_7000,
    NJ_21D,
    GZ_700K,
    GZ_8000,
    GZ_EDC,
    PTN_7000,
    PTN_21A,
    GZ_700S,
    NJ_31DV200,
    NJ_A258,
    MAX_HD
}VERSION;


extern CONFIGDATA global_data;
extern fd_set readFd;
extern int maxFd;
extern bool en232;
extern bool en485;
extern bool en2320;
extern bool en4850;
extern bool en2321;
extern bool en4851;
extern bool en2322;
extern bool en4852;
extern bool en2323;
extern bool en4853;
extern bool enRelay;
extern bool enRelay0;
extern bool enRelay1;
extern bool enIr;
extern bool enIr_tc;
extern bool enIo0;
extern bool enIo1;
extern bool enIo2;
extern bool enIo3;
extern bool enIo4;
extern bool enIo5;
extern bool enIo6;
extern bool enIo7;
extern bool irTime;

extern bool hd_232_edc;
extern bool ir_nj_700k;
extern bool ir_nj_21d;
extern VERSION hd_ver;

extern string local_ip;
extern bool en_3536;




#endif // GLOBAL_H
