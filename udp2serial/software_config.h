#ifndef Software_Config_H
#define Software_Config_H

#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include <iostream>

#include <readini.h>
#include "global.h"
#include "networkinfo.h"
#include <common.h>
#include"common_A258.h"
#include"hi_type.h"    //定义HI_BOOL

using namespace std;
#define ETH0_NAME "eth0"
#define ETH1_NAME "eth1"
#define DNS_NAME  "/etc/resolv.conf"
#define DATA_FILE "/data"
#define SERIAL_CONFIG_FILE "remserial.ini"
#define NET_CONFIG_FILE "net.ini"
#define NODE_NAME_FILE  "node_name"

extern char filepath_[36];

#define PARA_NAME_LEN 56
#define EnumtoStr(val) #val //把枚举变量的名称. 返回类型为const char[]


class SoftwareConfig
{
public:
    typedef enum {
        //控制端IP和端口，UMP可设置
        kctlip = 0,
        //kctlip_tc = 0,
//        kctl232port,
//        kctl485port,
        kctl2320port,
        kctl2321port,
        kctl2322port,
        kctl2323port,
        kctl4850port,
        kctl4851port,
        kctl4852port,
        kctl4853port,
        kctlrelayport,
        kctlrelay0port,
        kctlrelay1port,
        kctlinfraredport,
        kctlinfraredport_tc,
        kctlIO0port,
        kctlIO1port,
        kctlIO2port,
        kctlIO3port,
        kctlIO4port,
        kctlIO5port,
        kctlIO6port,
        kctlIO7port,
        //板子UDP接收端口，原则上不变
        ksv232port,
        ksv485port,
        ksvrelayport,
        kinfraredport,
        ksvIOport,
        ksvIOport_A258,
        //RS232--串口使能和属性
        ken232,
        k232port,
        k232uartspeed,
        k232nbit,
        k232nstop,
        k232event,
        k232vtime,
        k232vmin,
            //2320~3
        ken2320,
        k2320port,
        k2320uartspeed,
        k2320nbit,
        k2320nstop,
        k2320event,
        k2320vtime,
        k2320vmin,
        ken2321,
        k2321port,
        k2321uartspeed,
        k2321nbit,
        k2321nstop,
        k2321event,
        k2321vtime,
        k2321vmin,
        ken2322,
        k2322port,
        k2322uartspeed,
        k2322nbit,
        k2322nstop,
        k2322event,
        k2322vtime,
        k2322vmin,
        ken2323,
        k2323port,
        k2323uartspeed,
        k2323nbit,
        k2323nstop,
        k2323event,
        k2323vtime,
        k2323vmin,
        //RS485--串口使能和属性
        ken485,
        k485port,
        k485uartspeed,
        k485nbit,
        k485nstop,
        k485event,
        k485vtime,
        k485vmin,
            //4850~3
        ken4850,
        k4850port,
        k4850uartspeed,
        k4850nbit,
        k4850nstop,
        k4850event,
        k4850vtime,
        k4850vmin,
        ken4851,
        k4851port,
        k4851uartspeed,
        k4851nbit,
        k4851nstop,
        k4851event,
        k4851vtime,
        k4851vmin,
        ken4852,
        k4852port,
        k4852uartspeed,
        k4852nbit,
        k4852nstop,
        k4852event,
        k4852vtime,
        k4852vmin,
        ken4853,
        k4853port,
        k4853uartspeed,
        k4853nbit,
        k4853nstop,
        k4853event,
        k4853vtime,
        k4853vmin,
        //继电器---使能和状态
        kenrelay,
        krelayst,
        kenrelay0,
        krelay0st,
        kenrelay1,
        krelay1st,
        //红外---使能和属性（GPIO模拟和串口两种形式，通过硬件号区分）
        kenInfrared,
        kenInfrared_tc,
        kInfraredport,
        kInfrareduartspeed,
        kInfrarednbit,
        kInfrarednstop,
        kInfraredevent,
        kInfraredvtime,
        kInfraredvmin,
        //IO--0(未使能)，1（接收），2（发送）
        kSet0IO,
        kIO0st,
        kSet1IO,
        kIO1st,
        kSet2IO,
        kIO2st,
        kSet3IO,
        kIO3st,
        kSet4IO,
        kIO4st,
        kSet5IO,
        kIO5st,
        kSet6IO,
        kIO6st,
        kSet7IO,
        kIO7st,
        //TCP
        kTcpEnable,
        kTcpServerPort,
        kTcpLinkMax,

        knetworkstartflag, //
        //[network]
        kEth0Ip       ,
        kEth0Mask     ,
        kEth0Gateway  ,
        kEth0Dns      ,
        kEth0Mac      ,
        kEth1Ip       ,
        kEth1Mask     ,
        kEth1Gateway  ,
        kEth1Dns      ,
        kEth1Mac      ,
        kNodeName     ,
        //[system]
        kSysFixMac    ,//[system]移上来的 使他在readconfig列表里
//        kSysSyncCompensation,
//        kSysRotate          ,
//        kSysEthUser         ,
        kSysEthNum          ,

        kSoftWareConfigIDMax,
    }SoftWareConfigID;

    SoftwareConfig();
    ~SoftwareConfig();

    bool ReadConfig();
    bool SaveConfig();
    bool SaveConfig_ALL();
    void PrintConfig();
    void SetNetwork();
    bool SetConfig(const SoftWareConfigID kId, string value);
    bool SetConfig(const SoftWareConfigID kId, int value);
    string GetConfig(const SoftWareConfigID kId);
    bool reset();

private:
    void do_set_net(const char *kIp, const char *kMask, const char *kGateway,
                    const char *kDns, const char* kEthName = ETH0_NAME);
    void do_set_mac(const char* kMac, const char* kEthName = ETH0_NAME);

private:
    vector<string> configvalue_;
    //const char *const filepath_ = "./hi3531d_venc.ini"; //const data,const pointer
    char netcfg_path_[36]; //存放网络参数配置文件的路径
    char paramsnamelist[kSoftWareConfigIDMax][PARA_NAME_LEN]; //存放配置文件的前缀名称
};

#endif // Software_Config_H
