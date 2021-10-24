#include "software_config.h"

char filepath_[36];

SoftwareConfig::SoftwareConfig()
{
    if( !MakeDir(DATA_FILE) ){
        sprintf(filepath_, "./%s", SERIAL_CONFIG_FILE);
        sprintf(netcfg_path_, "./%s", NET_CONFIG_FILE);
        }else{
        sprintf(filepath_, "%s/%s", DATA_FILE, SERIAL_CONFIG_FILE);
        sprintf(netcfg_path_, "%s/%s", DATA_FILE, NET_CONFIG_FILE);
        }

    // 初始化 向量类型softwareConfig,定义其长度为kSoftWareConfigIDMax，内容都先置为空
    configvalue_.assign(kSoftWareConfigIDMax, "");
#ifdef __test__
    //network
    configvalue_[kctlip]            =   "192.168.100.17";
    //configvalue_[kctlip_tc]         =   "0.0.0.0";
//    configvalue_[kctl232port]       =   "0";
//    configvalue_[kctl485port]       =   "0";
    configvalue_[kctl2320port]       =   "11233";
    configvalue_[kctl2321port]       =   "11233";
    configvalue_[kctl2322port]       =   "11233";
    configvalue_[kctl2323port]       =   "11233";
    configvalue_[kctl4850port]       =   "11233";
    configvalue_[kctl4851port]       =   "11233";
    configvalue_[kctl4852port]       =   "11233";
    configvalue_[kctl4853port]       =   "11233";
    configvalue_[kctlrelayport]     =   "11233";
    configvalue_[kctlrelay0port]     =   "11233";
    configvalue_[kctlrelay1port]     =   "11233";
    configvalue_[kctlinfraredport]  =   "0";
    configvalue_[kctlinfraredport_tc] =   "0";
    configvalue_[kctlIO0port]       =   "11233";
    configvalue_[kctlIO1port]       =   "11233";
    configvalue_[kctlIO2port]       =   "11233";
    configvalue_[kctlIO3port]       =   "11233";
    configvalue_[kctlIO4port]       =   "11233";
    configvalue_[kctlIO5port]       =   "11233";
    configvalue_[kctlIO6port]       =   "11233";
    configvalue_[kctlIO7port]       =   "11233";
    //板子UDP接收端口，原则上不变
    configvalue_[ksv232port]        =   "16232";
    configvalue_[ksv485port]        =   "16485";
    configvalue_[ksvrelayport]      =   "16456";
    configvalue_[kinfraredport]     =   "16457";
    configvalue_[ksvIOport]         =   "16458";
    configvalue_[ksvIOport_A258]    =   "16459";
    //RS232--串口使能和属性
    configvalue_[ken232]            =   "1";
    configvalue_[k232port]          =   "com1";
    configvalue_[k232uartspeed]     =   "115200";
    configvalue_[k232nbit]          =   "8";
    configvalue_[k232nstop]         =   "1";
    configvalue_[k232event]         =   "N";
    configvalue_[k232vtime]         =   "3";
    configvalue_[k232vmin]          =   "10";
        //2320~3
    configvalue_[ken2320]            =   "1";
    configvalue_[k2320port]          =   "2320";
    configvalue_[k2320uartspeed]     =   "115200";
    configvalue_[k2320nbit]          =   "8";
    configvalue_[k2320nstop]         =   "1";
    configvalue_[k2320event]         =   "N";
    configvalue_[k2320vtime]         =   "3";
    configvalue_[k2320vmin]          =   "10";

    configvalue_[ken2321]            =   "1";
    configvalue_[k2321port]          =   "2321";
    configvalue_[k2321uartspeed]     =   "115200";
    configvalue_[k2321nbit]          =   "8";
    configvalue_[k2321nstop]         =   "1";
    configvalue_[k2321event]         =   "N";
    configvalue_[k2321vtime]         =   "3";
    configvalue_[k2321vmin]          =   "10";

    configvalue_[ken2322]            =   "1";
    configvalue_[k2322port]          =   "2322";
    configvalue_[k2322uartspeed]     =   "115200";
    configvalue_[k2322nbit]          =   "8";
    configvalue_[k2322nstop]         =   "1";
    configvalue_[k2322event]         =   "N";
    configvalue_[k2322vtime]         =   "3";
    configvalue_[k2322vmin]          =   "10";

    configvalue_[ken2323]            =   "1";
    configvalue_[k2323port]          =   "2323";
    configvalue_[k2323uartspeed]     =   "115200";
    configvalue_[k2323nbit]          =   "8";
    configvalue_[k2323nstop]         =   "1";
    configvalue_[k2323event]         =   "N";
    configvalue_[k2323vtime]         =   "3";
    configvalue_[k2323vmin]          =   "10";
    //RS485--串口使能和属性
    configvalue_[ken485]            =   "1";
    configvalue_[k485port]          =   "com2";
    configvalue_[k485uartspeed]     =   "9600";
    configvalue_[k485nbit]          =   "8";
    configvalue_[k485nstop]         =   "1";
    configvalue_[k485event]         =   "N";
    configvalue_[k485vtime]         =   "3";
    configvalue_[k485vmin]          =   "10";
        //4850~3
    configvalue_[ken4850]            =   "1";
    configvalue_[k4850port]          =   "4850";
    configvalue_[k4850uartspeed]     =   "9600";
    configvalue_[k4850nbit]          =   "8";
    configvalue_[k4850nstop]         =   "1";
    configvalue_[k4850event]         =   "N";
    configvalue_[k4850vtime]         =   "3";
    configvalue_[k4850vmin]          =   "10";

    configvalue_[ken4851]            =   "1";
    configvalue_[k4851port]          =   "4851";
    configvalue_[k4851uartspeed]     =   "9600";
    configvalue_[k4851nbit]          =   "8";
    configvalue_[k4851nstop]         =   "1";
    configvalue_[k4851event]         =   "N";
    configvalue_[k4851vtime]         =   "3";
    configvalue_[k4851vmin]          =   "10";

    configvalue_[ken4852]            =   "1";
    configvalue_[k4852port]          =   "4852";
    configvalue_[k4852uartspeed]     =   "9600";
    configvalue_[k4852nbit]          =   "8";
    configvalue_[k4852nstop]         =   "1";
    configvalue_[k4852event]         =   "N";
    configvalue_[k4852vtime]         =   "3";
    configvalue_[k4852vmin]          =   "10";

    configvalue_[ken4853]            =   "1";
    configvalue_[k4853port]          =   "4853";
    configvalue_[k4853uartspeed]     =   "9600";
    configvalue_[k4853nbit]          =   "8";
    configvalue_[k4853nstop]         =   "1";
    configvalue_[k4853event]         =   "N";
    configvalue_[k4853vtime]         =   "3";
    configvalue_[k4853vmin]          =   "10";
    //继电器
    configvalue_[kenrelay]          =   "0";
    configvalue_[krelayst]          =   "0";
    configvalue_[kenrelay0]          =   "0";
    configvalue_[krelay0st]          =   "0";
    configvalue_[kenrelay1]          =   "0";
    configvalue_[krelay1st]          =   "0";
    //红外---使能和属性（GPIO模拟和串口两种形式，通过硬件号区分）
    configvalue_[kenInfrared]       =   "0";
    configvalue_[kenInfrared_tc]    =   "0";
    configvalue_[kInfraredport]     =   "com2";
    configvalue_[kInfrareduartspeed]=   "9600";
    configvalue_[kInfrarednbit]     =   "8";
    configvalue_[kInfrarednstop]    =   "1";
    configvalue_[kInfraredevent]    =   "N";
    configvalue_[kInfraredvtime]    =   "3";
    configvalue_[kInfraredvmin]     =   "10";
    //IO--0(未使能)，1（接收），2（发送）
    configvalue_[kSet0IO]            =   "2";
    configvalue_[kIO0st]             =   "0";
    configvalue_[kSet1IO]            =   "1";
    configvalue_[kIO1st]             =   "0";
    configvalue_[kSet2IO]            =   "2";
    configvalue_[kIO2st]             =   "0";
    configvalue_[kSet3IO]            =   "1";
    configvalue_[kIO3st]             =   "0";
    configvalue_[kSet4IO]            =   "2";
    configvalue_[kIO4st]             =   "0";
    configvalue_[kSet5IO]            =   "1";
    configvalue_[kIO5st]             =   "0";
    configvalue_[kSet6IO]            =   "2";
    configvalue_[kIO6st]             =   "1";
    configvalue_[kSet7IO]            =   "0";
    configvalue_[kIO7st]             =   "1";
#else
    //network
    configvalue_[kctlip]            =   "0.0.0.0";
    //configvalue_[kctlip_tc]         =   "0.0.0.0";
//    configvalue_[kctl232port]       =   "0";
//    configvalue_[kctl485port]       =   "0";
    configvalue_[kctl2320port]       =   "0";
    configvalue_[kctl2321port]       =   "0";
    configvalue_[kctl2322port]       =   "0";
    configvalue_[kctl2323port]       =   "0";
    configvalue_[kctl4850port]       =   "0";
    configvalue_[kctl4851port]       =   "0";
    configvalue_[kctl4852port]       =   "0";
    configvalue_[kctl4853port]       =   "0";
    configvalue_[kctlrelayport]     =   "0";
    configvalue_[kctlrelay0port]     =   "0";
    configvalue_[kctlrelay1port]     =   "0";
    configvalue_[kctlinfraredport]  =   "0";
    configvalue_[kctlinfraredport_tc] =   "0";
    configvalue_[kctlIO0port]       =   "0";
    configvalue_[kctlIO1port]       =   "0";
    configvalue_[kctlIO2port]       =   "0";
    configvalue_[kctlIO3port]       =   "0";
    configvalue_[kctlIO4port]       =   "0";
    configvalue_[kctlIO5port]       =   "0";
    configvalue_[kctlIO6port]       =   "0";
    configvalue_[kctlIO7port]       =   "0";
    //板子UDP接收端口，原则上不变
    configvalue_[ksv232port]        =   "16232";
    configvalue_[ksv485port]        =   "16485";
    configvalue_[ksvrelayport]      =   "16456";
    configvalue_[kinfraredport]     =   "16457";
    configvalue_[ksvIOport]         =   "16458";
    configvalue_[ksvIOport_A258]    =   "16459";
    //RS232--串口使能和属性
    configvalue_[ken232]            =   "1";
    configvalue_[k232port]          =   "com1";
    configvalue_[k232uartspeed]     =   "115200";
    configvalue_[k232nbit]          =   "8";
    configvalue_[k232nstop]         =   "1";
    configvalue_[k232event]         =   "N";
    configvalue_[k232vtime]         =   "3";
    configvalue_[k232vmin]          =   "10";
        //2320~3
    configvalue_[ken2320]            =   "1";
    configvalue_[k2320port]          =   "2320";
    configvalue_[k2320uartspeed]     =   "115200";
    configvalue_[k2320nbit]          =   "8";
    configvalue_[k2320nstop]         =   "1";
    configvalue_[k2320event]         =   "N";
    configvalue_[k2320vtime]         =   "3";
    configvalue_[k2320vmin]          =   "10";

    configvalue_[ken2321]            =   "1";
    configvalue_[k2321port]          =   "2321";
    configvalue_[k2321uartspeed]     =   "115200";
    configvalue_[k2321nbit]          =   "8";
    configvalue_[k2321nstop]         =   "1";
    configvalue_[k2321event]         =   "N";
    configvalue_[k2321vtime]         =   "3";
    configvalue_[k2321vmin]          =   "10";

    configvalue_[ken2322]            =   "1";
    configvalue_[k2322port]          =   "2322";
    configvalue_[k2322uartspeed]     =   "115200";
    configvalue_[k2322nbit]          =   "8";
    configvalue_[k2322nstop]         =   "1";
    configvalue_[k2322event]         =   "N";
    configvalue_[k2322vtime]         =   "3";
    configvalue_[k2322vmin]          =   "10";

    configvalue_[ken2323]            =   "1";
    configvalue_[k2323port]          =   "2323";
    configvalue_[k2323uartspeed]     =   "115200";
    configvalue_[k2323nbit]          =   "8";
    configvalue_[k2323nstop]         =   "1";
    configvalue_[k2323event]         =   "N";
    configvalue_[k2323vtime]         =   "3";
    configvalue_[k2323vmin]          =   "10";
    //RS485--串口使能和属性
    configvalue_[ken485]            =   "0";
    configvalue_[k485port]          =   "com2";
    configvalue_[k485uartspeed]     =   "9600";
    configvalue_[k485nbit]          =   "8";
    configvalue_[k485nstop]         =   "1";
    configvalue_[k485event]         =   "N";
    configvalue_[k485vtime]         =   "3";
    configvalue_[k485vmin]          =   "10";
        //4850~3
    configvalue_[ken4850]            =   "1";
    configvalue_[k4850port]          =   "4850";
    configvalue_[k4850uartspeed]     =   "9600";
    configvalue_[k4850nbit]          =   "8";
    configvalue_[k4850nstop]         =   "1";
    configvalue_[k4850event]         =   "N";
    configvalue_[k4850vtime]         =   "3";
    configvalue_[k4850vmin]          =   "10";

    configvalue_[ken4851]            =   "1";
    configvalue_[k4851port]          =   "4851";
    configvalue_[k4851uartspeed]     =   "9600";
    configvalue_[k4851nbit]          =   "8";
    configvalue_[k4851nstop]         =   "1";
    configvalue_[k4851event]         =   "N";
    configvalue_[k4851vtime]         =   "3";
    configvalue_[k4851vmin]          =   "10";

    configvalue_[ken4852]            =   "1";
    configvalue_[k4852port]          =   "4852";
    configvalue_[k4852uartspeed]     =   "9600";
    configvalue_[k4852nbit]          =   "8";
    configvalue_[k4852nstop]         =   "1";
    configvalue_[k4852event]         =   "N";
    configvalue_[k4852vtime]         =   "3";
    configvalue_[k4852vmin]          =   "10";

    configvalue_[ken4853]            =   "1";
    configvalue_[k4853port]          =   "4853";
    configvalue_[k4853uartspeed]     =   "9600";
    configvalue_[k4853nbit]          =   "8";
    configvalue_[k4853nstop]         =   "1";
    configvalue_[k4853event]         =   "N";
    configvalue_[k4853vtime]         =   "3";
    configvalue_[k4853vmin]          =   "10";
    //继电器
    configvalue_[kenrelay]          =   "0";
    configvalue_[krelayst]          =   "0";
    configvalue_[kenrelay0]          =   "0";
    configvalue_[krelay0st]          =   "0";
    configvalue_[kenrelay1]          =   "0";
    configvalue_[krelay1st]          =   "0";
    //红外---使能和属性（GPIO模拟和串口两种形式，通过硬件号区分）
    configvalue_[kenInfrared]       =   "0";
    configvalue_[kenInfrared_tc]    =   "0";
    configvalue_[kInfraredport]     =   "com2";
    configvalue_[kInfrareduartspeed]=   "9600";
    configvalue_[kInfrarednbit]     =   "8";
    configvalue_[kInfrarednstop]    =   "1";
    configvalue_[kInfraredevent]    =   "N";
    configvalue_[kInfraredvtime]    =   "3";
    configvalue_[kInfraredvmin]     =   "10";
    //IO--0(未使能)，1（接收），2（发送）
    configvalue_[kSet0IO]            =   "0";
    configvalue_[kIO0st]             =   "0";
    configvalue_[kSet1IO]            =   "0";
    configvalue_[kIO1st]             =   "0";
    configvalue_[kSet2IO]            =   "0";
    configvalue_[kIO2st]             =   "0";
    configvalue_[kSet3IO]            =   "0";
    configvalue_[kIO3st]             =   "0";
    configvalue_[kSet4IO]            =   "0";
    configvalue_[kIO4st]             =   "0";
    configvalue_[kSet5IO]            =   "0";
    configvalue_[kIO5st]             =   "0";
    configvalue_[kSet6IO]            =   "2";
    configvalue_[kIO6st]             =   "1";
    configvalue_[kSet7IO]            =   "0";
    configvalue_[kIO7st]             =   "0";
#endif
    //TCP
    configvalue_[kTcpEnable]         =   "1";
    configvalue_[kTcpServerPort]     =   "8899";
    configvalue_[kTcpLinkMax]        =   "32";
    configvalue_[knetworkstartflag]   =   "0";
    //network
    configvalue_[kEth0Ip]        = "0.0.0.0";
    configvalue_[kEth0Mask]      = "0.0.0.0";
    configvalue_[kEth0Gateway]   = "0.0.0.0";
    configvalue_[kEth0Dns]       = "0.0.0.0";
    configvalue_[kEth0Mac]       = "00:00:00:00:00:00";
    configvalue_[kEth1Ip]        = "0.0.0.0";
    configvalue_[kEth1Mask]      = "0.0.0.0";
    configvalue_[kEth1Gateway]   = "0.0.0.0";
    configvalue_[kEth1Dns]       = "0.0.0.0";
    configvalue_[kEth1Mac]       = "00:00:00:00:00:00";
    configvalue_[kNodeName]      = "null";

    //[system]
//    configvalue_[kSysSyncCompensation]= "3";
//    configvalue_[kSysRotate]          = "0";    // "0" / "90" / "180" / "270"
//    configvalue_[kSysEthUser]         = "eth0";
    configvalue_[kSysEthNum]          = "1";
    configvalue_[kSysFixMac]          = "0";

    snprintf(paramsnamelist[kEth0Ip],               PARA_NAME_LEN, "%s=", EnumtoStr(kEth0Ip));
    snprintf(paramsnamelist[kEth0Mask],             PARA_NAME_LEN, "%s=", EnumtoStr(kEth0Mask));
    snprintf(paramsnamelist[kEth0Gateway],          PARA_NAME_LEN, "%s=", EnumtoStr(kEth0Gateway));
    snprintf(paramsnamelist[kEth0Dns],              PARA_NAME_LEN, "%s=", EnumtoStr(kEth0Dns));
    snprintf(paramsnamelist[kEth0Mac],              PARA_NAME_LEN, "%s=", EnumtoStr(kEth0Mac));
    snprintf(paramsnamelist[kEth1Ip],               PARA_NAME_LEN, "%s=", EnumtoStr(kEth1Ip));
    snprintf(paramsnamelist[kEth1Mask],             PARA_NAME_LEN, "%s=", EnumtoStr(kEth1Mask));
    snprintf(paramsnamelist[kEth1Gateway],          PARA_NAME_LEN, "%s=", EnumtoStr(kEth1Gateway));
    snprintf(paramsnamelist[kEth1Dns],              PARA_NAME_LEN, "%s=", EnumtoStr(kEth1Dns));
    snprintf(paramsnamelist[kEth1Mac],              PARA_NAME_LEN, "%s=", EnumtoStr(kEth1Mac));
    snprintf(paramsnamelist[kNodeName],             PARA_NAME_LEN, "%s=", EnumtoStr(kNodeName));

    snprintf(paramsnamelist[kSysFixMac],            PARA_NAME_LEN, "%s=", EnumtoStr(kSysFixMac));

    snprintf(paramsnamelist[kctlip],                   PARA_NAME_LEN, "%s=", EnumtoStr(kctlip));
    //snprintf(paramsnamelist[kctlip_tc],                   PARA_NAME_LEN, "%s=", EnumtoStr(kctlip_tc));
//    snprintf(paramsnamelist[kctl232port],                 PARA_NAME_LEN, "%s=", EnumtoStr(kctl232port));
//    snprintf(paramsnamelist[kctl485port],              PARA_NAME_LEN, "%s=", EnumtoStr(kctl485port));
    snprintf(paramsnamelist[kctl2320port],                 PARA_NAME_LEN, "%s=", EnumtoStr(kctl2320port));
    snprintf(paramsnamelist[kctl2321port],                 PARA_NAME_LEN, "%s=", EnumtoStr(kctl2321port));
    snprintf(paramsnamelist[kctl2322port],                 PARA_NAME_LEN, "%s=", EnumtoStr(kctl2322port));
    snprintf(paramsnamelist[kctl2323port],                 PARA_NAME_LEN, "%s=", EnumtoStr(kctl2323port));
    snprintf(paramsnamelist[kctl4850port],              PARA_NAME_LEN, "%s=", EnumtoStr(kctl4850port));
    snprintf(paramsnamelist[kctl4851port],              PARA_NAME_LEN, "%s=", EnumtoStr(kctl4851port));
    snprintf(paramsnamelist[kctl4852port],              PARA_NAME_LEN, "%s=", EnumtoStr(kctl4852port));
    snprintf(paramsnamelist[kctl4853port],              PARA_NAME_LEN, "%s=", EnumtoStr(kctl4853port));
    snprintf(paramsnamelist[kctlrelayport],                  PARA_NAME_LEN, "%s=", EnumtoStr(kctlrelayport));
    snprintf(paramsnamelist[kctlrelay0port],                  PARA_NAME_LEN, "%s=", EnumtoStr(kctlrelay0port));
    snprintf(paramsnamelist[kctlrelay1port],                  PARA_NAME_LEN, "%s=", EnumtoStr(kctlrelay1port));
    snprintf(paramsnamelist[kctlinfraredport],                  PARA_NAME_LEN, "%s=", EnumtoStr(kctlinfraredport));
     snprintf(paramsnamelist[kctlinfraredport_tc],                  PARA_NAME_LEN, "%s=", EnumtoStr(kctlinfraredport_tc));
    snprintf(paramsnamelist[kctlIO0port],                  PARA_NAME_LEN, "%s=", EnumtoStr(kctlIO0port));
    snprintf(paramsnamelist[kctlIO1port],                  PARA_NAME_LEN, "%s=", EnumtoStr(kctlIO1port));
    snprintf(paramsnamelist[kctlIO2port],                  PARA_NAME_LEN, "%s=", EnumtoStr(kctlIO2port));
    snprintf(paramsnamelist[kctlIO3port],                  PARA_NAME_LEN, "%s=", EnumtoStr(kctlIO3port));
    snprintf(paramsnamelist[kctlIO4port],                  PARA_NAME_LEN, "%s=", EnumtoStr(kctlIO4port));
    snprintf(paramsnamelist[kctlIO5port],                  PARA_NAME_LEN, "%s=", EnumtoStr(kctlIO5port));
    snprintf(paramsnamelist[kctlIO6port],                  PARA_NAME_LEN, "%s=", EnumtoStr(kctlIO6port));
    snprintf(paramsnamelist[kctlIO7port],                  PARA_NAME_LEN, "%s=", EnumtoStr(kctlIO7port));
    snprintf(paramsnamelist[ksv232port],                PARA_NAME_LEN, "%s=", EnumtoStr(ksv232port));
    snprintf(paramsnamelist[ksv485port],            PARA_NAME_LEN, "%s=", EnumtoStr(ksv485port));
    snprintf(paramsnamelist[ksvrelayport],            PARA_NAME_LEN, "%s=", EnumtoStr(ksvrelayport));
    snprintf(paramsnamelist[kinfraredport],            PARA_NAME_LEN, "%s=", EnumtoStr(kinfraredport));
    snprintf(paramsnamelist[ksvIOport],            PARA_NAME_LEN, "%s=", EnumtoStr(ksvIOport));
    snprintf(paramsnamelist[ksvIOport_A258],            PARA_NAME_LEN, "%s=", EnumtoStr(ksvIOport_A258));
    snprintf(paramsnamelist[ken232],             PARA_NAME_LEN, "%s=", EnumtoStr(ken232));
    snprintf(paramsnamelist[k232port],       PARA_NAME_LEN, "%s=", EnumtoStr(k232port));
    snprintf(paramsnamelist[k232uartspeed],       PARA_NAME_LEN, "%s=", EnumtoStr(k232uartspeed));
    snprintf(paramsnamelist[k232nbit],       PARA_NAME_LEN, "%s=", EnumtoStr(k232nbit));
    snprintf(paramsnamelist[k232nstop],       PARA_NAME_LEN, "%s=", EnumtoStr(k232nstop));
    snprintf(paramsnamelist[k232event],                PARA_NAME_LEN, "%s=", EnumtoStr(k232event));
    snprintf(paramsnamelist[k232vtime],               PARA_NAME_LEN, "%s=", EnumtoStr(k232vtime));
    snprintf(paramsnamelist[k232vmin],               PARA_NAME_LEN, "%s=", EnumtoStr(k232vmin));
    snprintf(paramsnamelist[ken2320],             PARA_NAME_LEN, "%s=", EnumtoStr(ken2320));
    snprintf(paramsnamelist[k2320port],       PARA_NAME_LEN, "%s=", EnumtoStr(k2320port));
    snprintf(paramsnamelist[k2320uartspeed],       PARA_NAME_LEN, "%s=", EnumtoStr(k2320uartspeed));
    snprintf(paramsnamelist[k2320nbit],       PARA_NAME_LEN, "%s=", EnumtoStr(k2320nbit));
    snprintf(paramsnamelist[k2320nstop],       PARA_NAME_LEN, "%s=", EnumtoStr(k2320nstop));
    snprintf(paramsnamelist[k2320event],                PARA_NAME_LEN, "%s=", EnumtoStr(k2320event));
    snprintf(paramsnamelist[k2320vtime],               PARA_NAME_LEN, "%s=", EnumtoStr(k2320vtime));
    snprintf(paramsnamelist[k2320vmin],               PARA_NAME_LEN, "%s=", EnumtoStr(k2320vmin));
    snprintf(paramsnamelist[ken2321],             PARA_NAME_LEN, "%s=", EnumtoStr(ken2321));
    snprintf(paramsnamelist[k2321port],       PARA_NAME_LEN, "%s=", EnumtoStr(k2321port));
    snprintf(paramsnamelist[k2321uartspeed],       PARA_NAME_LEN, "%s=", EnumtoStr(k2321uartspeed));
    snprintf(paramsnamelist[k2321nbit],       PARA_NAME_LEN, "%s=", EnumtoStr(k2321nbit));
    snprintf(paramsnamelist[k2321nstop],       PARA_NAME_LEN, "%s=", EnumtoStr(k2321nstop));
    snprintf(paramsnamelist[k2321event],                PARA_NAME_LEN, "%s=", EnumtoStr(k2321event));
    snprintf(paramsnamelist[k2321vtime],               PARA_NAME_LEN, "%s=", EnumtoStr(k2321vtime));
    snprintf(paramsnamelist[k2321vmin],               PARA_NAME_LEN, "%s=", EnumtoStr(k2321vmin));
    snprintf(paramsnamelist[ken2322],             PARA_NAME_LEN, "%s=", EnumtoStr(ken2322));
    snprintf(paramsnamelist[k2322port],       PARA_NAME_LEN, "%s=", EnumtoStr(k2322port));
    snprintf(paramsnamelist[k2322uartspeed],       PARA_NAME_LEN, "%s=", EnumtoStr(k2322uartspeed));
    snprintf(paramsnamelist[k2322nbit],       PARA_NAME_LEN, "%s=", EnumtoStr(k2322nbit));
    snprintf(paramsnamelist[k2322nstop],       PARA_NAME_LEN, "%s=", EnumtoStr(k2322nstop));
    snprintf(paramsnamelist[k2322event],                PARA_NAME_LEN, "%s=", EnumtoStr(k2322event));
    snprintf(paramsnamelist[k2322vtime],               PARA_NAME_LEN, "%s=", EnumtoStr(k2322vtime));
    snprintf(paramsnamelist[k2322vmin],               PARA_NAME_LEN, "%s=", EnumtoStr(k2322vmin));
    snprintf(paramsnamelist[ken2323],             PARA_NAME_LEN, "%s=", EnumtoStr(ken2323));
    snprintf(paramsnamelist[k2323port],       PARA_NAME_LEN, "%s=", EnumtoStr(k2323port));
    snprintf(paramsnamelist[k2323uartspeed],       PARA_NAME_LEN, "%s=", EnumtoStr(k2323uartspeed));
    snprintf(paramsnamelist[k2323nbit],       PARA_NAME_LEN, "%s=", EnumtoStr(k2323nbit));
    snprintf(paramsnamelist[k2323nstop],       PARA_NAME_LEN, "%s=", EnumtoStr(k2323nstop));
    snprintf(paramsnamelist[k2323event],                PARA_NAME_LEN, "%s=", EnumtoStr(k2323event));
    snprintf(paramsnamelist[k2323vtime],               PARA_NAME_LEN, "%s=", EnumtoStr(k2323vtime));
    snprintf(paramsnamelist[k2323vmin],               PARA_NAME_LEN, "%s=", EnumtoStr(k2323vmin));
    snprintf(paramsnamelist[ken485],             PARA_NAME_LEN, "%s=", EnumtoStr(ken485));
    snprintf(paramsnamelist[k485port],       PARA_NAME_LEN, "%s=", EnumtoStr(k485port));
    snprintf(paramsnamelist[k485uartspeed],       PARA_NAME_LEN, "%s=", EnumtoStr(k485uartspeed));
    snprintf(paramsnamelist[k485nbit],       PARA_NAME_LEN, "%s=", EnumtoStr(k485nbit));
    snprintf(paramsnamelist[k485nstop],       PARA_NAME_LEN, "%s=", EnumtoStr(k485nstop));
    snprintf(paramsnamelist[k485event],                PARA_NAME_LEN, "%s=", EnumtoStr(k485event));
    snprintf(paramsnamelist[k485vtime],               PARA_NAME_LEN, "%s=", EnumtoStr(k485vtime));
    snprintf(paramsnamelist[k485vmin],               PARA_NAME_LEN, "%s=", EnumtoStr(k485vmin));
    snprintf(paramsnamelist[ken4850],             PARA_NAME_LEN, "%s=", EnumtoStr(ken4850));
    snprintf(paramsnamelist[k4850port],       PARA_NAME_LEN, "%s=", EnumtoStr(k4850port));
    snprintf(paramsnamelist[k4850uartspeed],       PARA_NAME_LEN, "%s=", EnumtoStr(k4850uartspeed));
    snprintf(paramsnamelist[k4850nbit],       PARA_NAME_LEN, "%s=", EnumtoStr(k4850nbit));
    snprintf(paramsnamelist[k4850nstop],       PARA_NAME_LEN, "%s=", EnumtoStr(k4850nstop));
    snprintf(paramsnamelist[k4850event],                PARA_NAME_LEN, "%s=", EnumtoStr(k4850event));
    snprintf(paramsnamelist[k4850vtime],               PARA_NAME_LEN, "%s=", EnumtoStr(k4850vtime));
    snprintf(paramsnamelist[k4850vmin],               PARA_NAME_LEN, "%s=", EnumtoStr(k4850vmin));
    snprintf(paramsnamelist[ken4851],             PARA_NAME_LEN, "%s=", EnumtoStr(ken4851));
    snprintf(paramsnamelist[k4851port],       PARA_NAME_LEN, "%s=", EnumtoStr(k4851port));
    snprintf(paramsnamelist[k4851uartspeed],       PARA_NAME_LEN, "%s=", EnumtoStr(k4851uartspeed));
    snprintf(paramsnamelist[k4851nbit],       PARA_NAME_LEN, "%s=", EnumtoStr(k4851nbit));
    snprintf(paramsnamelist[k4851nstop],       PARA_NAME_LEN, "%s=", EnumtoStr(k4851nstop));
    snprintf(paramsnamelist[k4851event],                PARA_NAME_LEN, "%s=", EnumtoStr(k4851event));
    snprintf(paramsnamelist[k4851vtime],               PARA_NAME_LEN, "%s=", EnumtoStr(k4851vtime));
    snprintf(paramsnamelist[k4851vmin],               PARA_NAME_LEN, "%s=", EnumtoStr(k4851vmin));
    snprintf(paramsnamelist[ken4852],             PARA_NAME_LEN, "%s=", EnumtoStr(ken4852));
    snprintf(paramsnamelist[k4852port],       PARA_NAME_LEN, "%s=", EnumtoStr(k4852port));
    snprintf(paramsnamelist[k4852uartspeed],       PARA_NAME_LEN, "%s=", EnumtoStr(k4852uartspeed));
    snprintf(paramsnamelist[k4852nbit],       PARA_NAME_LEN, "%s=", EnumtoStr(k4852nbit));
    snprintf(paramsnamelist[k4852nstop],       PARA_NAME_LEN, "%s=", EnumtoStr(k4852nstop));
    snprintf(paramsnamelist[k4852event],                PARA_NAME_LEN, "%s=", EnumtoStr(k4852event));
    snprintf(paramsnamelist[k4852vtime],               PARA_NAME_LEN, "%s=", EnumtoStr(k4852vtime));
    snprintf(paramsnamelist[k4852vmin],               PARA_NAME_LEN, "%s=", EnumtoStr(k4852vmin));
    snprintf(paramsnamelist[ken4853],             PARA_NAME_LEN, "%s=", EnumtoStr(ken4853));
    snprintf(paramsnamelist[k4853port],       PARA_NAME_LEN, "%s=", EnumtoStr(k4853port));
    snprintf(paramsnamelist[k4853uartspeed],       PARA_NAME_LEN, "%s=", EnumtoStr(k4853uartspeed));
    snprintf(paramsnamelist[k4853nbit],       PARA_NAME_LEN, "%s=", EnumtoStr(k4853nbit));
    snprintf(paramsnamelist[k4853nstop],       PARA_NAME_LEN, "%s=", EnumtoStr(k4853nstop));
    snprintf(paramsnamelist[k4853event],                PARA_NAME_LEN, "%s=", EnumtoStr(k4853event));
    snprintf(paramsnamelist[k4853vtime],               PARA_NAME_LEN, "%s=", EnumtoStr(k4853vtime));
    snprintf(paramsnamelist[k4853vmin],               PARA_NAME_LEN, "%s=", EnumtoStr(k4853vmin));
    snprintf(paramsnamelist[kenrelay],               PARA_NAME_LEN, "%s=", EnumtoStr(kenrelay));
    snprintf(paramsnamelist[kenrelay0],               PARA_NAME_LEN, "%s=", EnumtoStr(kenrelay0));
    snprintf(paramsnamelist[kenrelay1],               PARA_NAME_LEN, "%s=", EnumtoStr(kenrelay1));
    snprintf(paramsnamelist[krelayst],               PARA_NAME_LEN, "%s=", EnumtoStr(krelayst));
    snprintf(paramsnamelist[krelay0st],               PARA_NAME_LEN, "%s=", EnumtoStr(krelay0st));
    snprintf(paramsnamelist[krelay1st],               PARA_NAME_LEN, "%s=", EnumtoStr(krelay1st));
    snprintf(paramsnamelist[kenInfrared],             PARA_NAME_LEN, "%s=", EnumtoStr(kenInfrared));
    snprintf(paramsnamelist[kenInfrared_tc],             PARA_NAME_LEN, "%s=", EnumtoStr(kenInfrared_tc));
    snprintf(paramsnamelist[kInfraredport],       PARA_NAME_LEN, "%s=", EnumtoStr(kInfraredport));
    snprintf(paramsnamelist[kInfrareduartspeed],       PARA_NAME_LEN, "%s=", EnumtoStr(kInfrareduartspeed));
    snprintf(paramsnamelist[kInfrarednbit],       PARA_NAME_LEN, "%s=", EnumtoStr(kInfrarednbit));
    snprintf(paramsnamelist[kInfrarednstop],       PARA_NAME_LEN, "%s=", EnumtoStr(kInfrarednstop));
    snprintf(paramsnamelist[kInfraredevent],                PARA_NAME_LEN, "%s=", EnumtoStr(kInfraredevent));
    snprintf(paramsnamelist[kInfraredvtime],               PARA_NAME_LEN, "%s=", EnumtoStr(kInfraredvtime));
    snprintf(paramsnamelist[kInfraredvmin],               PARA_NAME_LEN, "%s=", EnumtoStr(kInfraredvmin));
    snprintf(paramsnamelist[kSet0IO],                PARA_NAME_LEN, "%s=", EnumtoStr(kSet0IO));
    snprintf(paramsnamelist[kIO0st],                PARA_NAME_LEN, "%s=", EnumtoStr(kIO0st));
    snprintf(paramsnamelist[kSet1IO],                PARA_NAME_LEN, "%s=", EnumtoStr(kSet1IO));
    snprintf(paramsnamelist[kIO1st],                PARA_NAME_LEN, "%s=", EnumtoStr(kIO1st));
    snprintf(paramsnamelist[kSet2IO],                PARA_NAME_LEN, "%s=", EnumtoStr(kSet2IO));
    snprintf(paramsnamelist[kIO2st],                PARA_NAME_LEN, "%s=", EnumtoStr(kIO2st));
    snprintf(paramsnamelist[kSet3IO],                PARA_NAME_LEN, "%s=", EnumtoStr(kSet3IO));
    snprintf(paramsnamelist[kIO3st],                PARA_NAME_LEN, "%s=", EnumtoStr(kIO3st));
    snprintf(paramsnamelist[kSet4IO],                PARA_NAME_LEN, "%s=", EnumtoStr(kSet4IO));
    snprintf(paramsnamelist[kIO4st],                PARA_NAME_LEN, "%s=", EnumtoStr(kIO4st));
    snprintf(paramsnamelist[kSet5IO],                PARA_NAME_LEN, "%s=", EnumtoStr(kSet5IO));
    snprintf(paramsnamelist[kIO5st],                PARA_NAME_LEN, "%s=", EnumtoStr(kIO5st));
    snprintf(paramsnamelist[kSet6IO],                PARA_NAME_LEN, "%s=", EnumtoStr(kSet6IO));
    snprintf(paramsnamelist[kIO6st],                PARA_NAME_LEN, "%s=", EnumtoStr(kIO6st));
    snprintf(paramsnamelist[kSet7IO],                PARA_NAME_LEN, "%s=", EnumtoStr(kSet7IO));
    snprintf(paramsnamelist[kIO7st],                PARA_NAME_LEN, "%s=", EnumtoStr(kIO7st));
    snprintf(paramsnamelist[kTcpEnable],                PARA_NAME_LEN, "%s=", EnumtoStr(kTcpEnable));
    snprintf(paramsnamelist[kTcpServerPort],            PARA_NAME_LEN, "%s=", EnumtoStr(kTcpServerPort));
    snprintf(paramsnamelist[kTcpLinkMax],               PARA_NAME_LEN, "%s=", EnumtoStr(kTcpLinkMax));

}

SoftwareConfig::~SoftwareConfig()
{
    COMMON_PRT("SoftwareConfig Normal Exit!\n");
}

bool SoftwareConfig::ReadConfig()
{
    COMMON_PRT("read_config:%s\n", filepath_);

    read_ini rnet( netcfg_path_ );
    for(uint i = kEth0Ip; i <= kNodeName; i++){
        if(i != kNodeName){
            rnet.find_value( paramsnamelist[i],   configvalue_[i] );
        }
        else{
            char namefile[64] = "";
            sprintf(namefile, "%s/%s", DATA_FILE, NODE_NAME_FILE);
            configvalue_[i] = ReadSthFile(namefile);
        }
    }


    read_ini ri( filepath_ );
    for(uint i = kctlip; i < knetworkstartflag; i++){
        ri.find_value( paramsnamelist[i],   configvalue_[i] );
    }

    return true;
}

bool SoftwareConfig::SaveConfig_ALL()
{
    FILE *f = NULL;
    f = fopen( filepath_, "wb" );
    if(f)
    {
        for(uint i = kctlip; i < kSoftWareConfigIDMax; i++){
            fprintf(f, "%s%s\n", paramsnamelist[i], configvalue_[i].c_str());
        }
        fflush(f);
        fclose(f);
        f = NULL;
        COMMON_PRT("---save config ALL succeed---");
        return true;

    }

    COMMON_PRT("save config failed");
    return false;
}


bool SoftwareConfig::SaveConfig()
{
    FILE *f = NULL;
    f = fopen( filepath_, "wb" );
    if(f)
    {
        for(uint i = kctlip; i < knetworkstartflag; i++){
            fprintf(f, "%s%s\n", paramsnamelist[i], configvalue_[i].c_str());
        }
        fflush(f);
        fclose(f);
        f = NULL;
        COMMON_PRT("save config succeed!\n");
        return true;

    }

    COMMON_PRT("save config failed");
    return false;
}

void SoftwareConfig::PrintConfig()
{
    for(uint i = kctlip; i < kSoftWareConfigIDMax; i++){
        printf("%s%s\n", paramsnamelist[i], configvalue_[i].c_str());
    }
}

bool SoftwareConfig::SetConfig(const SoftWareConfigID kId, string value)
{
    if(kId < kSoftWareConfigIDMax)
        configvalue_[kId] = value;
    else
        return false;

    return true;
}

bool SoftwareConfig::SetConfig(const SoftWareConfigID kId, int value)
{
    if(kId < kSoftWareConfigIDMax)
        configvalue_[kId] = int2str(value);
    else
        return false;

    return true;
}

string SoftwareConfig::GetConfig(const SoftWareConfigID kId)
{
    if(kId < kSoftWareConfigIDMax)
        return configvalue_[kId];
    else
        return "error";
}

void SoftwareConfig::SetNetwork()
{
    if((HI_BOOL)str2int(configvalue_[kSysFixMac])){ //固定MAC地址
        // 获取本地mac地址
        char macaddr[2][SZ_IPADDR];
        NetworkInfo *netinfo = new NetworkInfo();
        if( !netinfo->GetNetworkInfo(ETH0, NetworkInfo::kHWADDR, macaddr[0]) ){
            printf("Get mac failed!");
            sprintf(macaddr[0], "%s", "00:00:00:00:00:00");
        }
        if( !netinfo->GetNetworkInfo(ETH1, NetworkInfo::kHWADDR, macaddr[1]) ){
           printf("Get mac failed!");
            sprintf(macaddr[1], "%s", "00:00:00:00:00:00");
        }
        delete netinfo;
        printf("mac[0]: %s mac[1]: %s", macaddr[0], macaddr[1]);

        if(configvalue_[kEth0Mac] == "00:00:00:00:00:00"){
            configvalue_[kEth0Mac] = macaddr[0];
            this->SetConfig(SoftwareConfig::kEth0Mac, macaddr[0]);
        }
        if(configvalue_[kEth1Mac] == "00:00:00:00:00:00"){
            configvalue_[kEth1Mac] = macaddr[1];
            this->SetConfig(SoftwareConfig::kEth1Mac, macaddr[1]);
        }
    }

    do_set_mac( configvalue_[kEth0Mac].c_str(), ETH0_NAME );
    do_set_net( configvalue_[kEth0Ip].c_str(), configvalue_[kEth0Mask].c_str(), configvalue_[kEth0Gateway].c_str(), configvalue_[kEth0Dns].c_str(), ETH0_NAME );

    if(configvalue_[kSysEthNum] == "2"){
        do_set_mac( configvalue_[kEth1Mac].c_str(), ETH1_NAME );
        do_set_net( configvalue_[kEth1Ip].c_str(), configvalue_[kEth1Mask].c_str(), configvalue_[kEth1Gateway].c_str(), configvalue_[kEth1Dns].c_str(), ETH1_NAME );
    }
}

bool SoftwareConfig::reset()
{

#ifdef __test2__
    //network
    configvalue_[kctlip]            =   "192.168.100.17";
    //configvalue_[kctlip_tc]         =   "0.0.0.0";
//    configvalue_[kctl232port]       =   "0";
//    configvalue_[kctl485port]       =   "0";
    configvalue_[kctl2320port]       =   "11233";
    configvalue_[kctl2321port]       =   "11233";
    configvalue_[kctl2322port]       =   "11233";
    configvalue_[kctl2323port]       =   "11233";
    configvalue_[kctl4850port]       =   "11233";
    configvalue_[kctl4851port]       =   "11233";
    configvalue_[kctl4852port]       =   "11233";
    configvalue_[kctl4853port]       =   "11233";
    configvalue_[kctlrelayport]     =   "11233";
    configvalue_[kctlrelay0port]     =   "11233";
    configvalue_[kctlrelay1port]     =   "11233";
    configvalue_[kctlinfraredport]  =   "0";
    configvalue_[kctlinfraredport_tc] =   "0";
    configvalue_[kctlIO0port]       =   "11233";
    configvalue_[kctlIO1port]       =   "11233";
    configvalue_[kctlIO2port]       =   "11233";
    configvalue_[kctlIO3port]       =   "11233";
    configvalue_[kctlIO4port]       =   "11233";
    configvalue_[kctlIO5port]       =   "11233";
    configvalue_[kctlIO6port]       =   "11233";
    configvalue_[kctlIO7port]       =   "11233";
    //板子UDP接收端口，原则上不变
    configvalue_[ksv232port]        =   "16232";
    configvalue_[ksv485port]        =   "16485";
    configvalue_[ksvrelayport]      =   "16456";
    configvalue_[kinfraredport]     =   "16457";
    configvalue_[ksvIOport]         =   "16458";
    configvalue_[ksvIOport_A258]    =   "16459";
    //RS232--串口使能和属性
    configvalue_[ken232]            =   "1";
    configvalue_[k232port]          =   "com1";
    configvalue_[k232uartspeed]     =   "115200";
    configvalue_[k232nbit]          =   "8";
    configvalue_[k232nstop]         =   "1";
    configvalue_[k232event]         =   "N";
    configvalue_[k232vtime]         =   "3";
    configvalue_[k232vmin]          =   "10";
        //2320~3
    configvalue_[ken2320]            =   "1";
    configvalue_[k2320port]          =   "2320";
    configvalue_[k2320uartspeed]     =   "115200";
    configvalue_[k2320nbit]          =   "8";
    configvalue_[k2320nstop]         =   "1";
    configvalue_[k2320event]         =   "N";
    configvalue_[k2320vtime]         =   "3";
    configvalue_[k2320vmin]          =   "10";

    configvalue_[ken2321]            =   "1";
    configvalue_[k2321port]          =   "2321";
    configvalue_[k2321uartspeed]     =   "115200";
    configvalue_[k2321nbit]          =   "8";
    configvalue_[k2321nstop]         =   "1";
    configvalue_[k2321event]         =   "N";
    configvalue_[k2321vtime]         =   "3";
    configvalue_[k2321vmin]          =   "10";

    configvalue_[ken2322]            =   "1";
    configvalue_[k2322port]          =   "2322";
    configvalue_[k2322uartspeed]     =   "115200";
    configvalue_[k2322nbit]          =   "8";
    configvalue_[k2322nstop]         =   "1";
    configvalue_[k2322event]         =   "N";
    configvalue_[k2322vtime]         =   "3";
    configvalue_[k2322vmin]          =   "10";

    configvalue_[ken2323]            =   "1";
    configvalue_[k2323port]          =   "2323";
    configvalue_[k2323uartspeed]     =   "115200";
    configvalue_[k2323nbit]          =   "8";
    configvalue_[k2323nstop]         =   "1";
    configvalue_[k2323event]         =   "N";
    configvalue_[k2323vtime]         =   "3";
    configvalue_[k2323vmin]          =   "10";
    //RS485--串口使能和属性
    configvalue_[ken485]            =   "1";
    configvalue_[k485port]          =   "com2";
    configvalue_[k485uartspeed]     =   "9600";
    configvalue_[k485nbit]          =   "8";
    configvalue_[k485nstop]         =   "1";
    configvalue_[k485event]         =   "N";
    configvalue_[k485vtime]         =   "3";
    configvalue_[k485vmin]          =   "10";
        //4850~3
    configvalue_[ken4850]            =   "1";
    configvalue_[k4850port]          =   "4850";
    configvalue_[k4850uartspeed]     =   "9600";
    configvalue_[k4850nbit]          =   "8";
    configvalue_[k4850nstop]         =   "1";
    configvalue_[k4850event]         =   "N";
    configvalue_[k4850vtime]         =   "3";
    configvalue_[k4850vmin]          =   "10";

    configvalue_[ken4851]            =   "1";
    configvalue_[k4851port]          =   "4851";
    configvalue_[k4851uartspeed]     =   "9600";
    configvalue_[k4851nbit]          =   "8";
    configvalue_[k4851nstop]         =   "1";
    configvalue_[k4851event]         =   "N";
    configvalue_[k4851vtime]         =   "3";
    configvalue_[k4851vmin]          =   "10";

    configvalue_[ken4852]            =   "1";
    configvalue_[k4852port]          =   "4852";
    configvalue_[k4852uartspeed]     =   "9600";
    configvalue_[k4852nbit]          =   "8";
    configvalue_[k4852nstop]         =   "1";
    configvalue_[k4852event]         =   "N";
    configvalue_[k4852vtime]         =   "3";
    configvalue_[k4852vmin]          =   "10";

    configvalue_[ken4853]            =   "1";
    configvalue_[k4853port]          =   "4853";
    configvalue_[k4853uartspeed]     =   "9600";
    configvalue_[k4853nbit]          =   "8";
    configvalue_[k4853nstop]         =   "1";
    configvalue_[k4853event]         =   "N";
    configvalue_[k4853vtime]         =   "3";
    configvalue_[k4853vmin]          =   "10";
    //继电器
    configvalue_[kenrelay]          =   "0";
    configvalue_[krelayst]          =   "0";
    configvalue_[kenrelay0]          =   "0";
    configvalue_[krelay0st]          =   "0";
    configvalue_[kenrelay1]          =   "0";
    configvalue_[krelay1st]          =   "0";
    //红外---使能和属性（GPIO模拟和串口两种形式，通过硬件号区分）
    configvalue_[kenInfrared]       =   "0";
    configvalue_[kenInfrared_tc]    =   "0";
    configvalue_[kInfraredport]     =   "com2";
    configvalue_[kInfrareduartspeed]=   "9600";
    configvalue_[kInfrarednbit]     =   "8";
    configvalue_[kInfrarednstop]    =   "1";
    configvalue_[kInfraredevent]    =   "N";
    configvalue_[kInfraredvtime]    =   "3";
    configvalue_[kInfraredvmin]     =   "10";
    //IO--0(未使能)，1（接收），2（发送）
    configvalue_[kSet0IO]            =   "2";
    configvalue_[kIO0st]             =   "0";
    configvalue_[kSet1IO]            =   "1";
    configvalue_[kIO1st]             =   "0";
    configvalue_[kSet2IO]            =   "2";
    configvalue_[kIO2st]             =   "0";
    configvalue_[kSet3IO]            =   "1";
    configvalue_[kIO3st]             =   "0";
    configvalue_[kSet4IO]            =   "2";
    configvalue_[kIO4st]             =   "0";
    configvalue_[kSet5IO]            =   "1";
    configvalue_[kIO5st]             =   "0";
    configvalue_[kSet6IO]            =   "2";
    configvalue_[kIO6st]             =   "1";
    configvalue_[kSet7IO]            =   "0";
    configvalue_[kIO7st]             =   "1";
#else

    configvalue_[kctlip]            =   "0.0.0.0";
    //configvalue_[kctlip_tc]         =   "0.0.0.0";
//    configvalue_[kctl232port]       =   "0";
//    configvalue_[kctl485port]       =   "0";
    configvalue_[kctl2320port]       =   "0";
    configvalue_[kctl2321port]       =   "0";
    configvalue_[kctl2322port]       =   "0";
    configvalue_[kctl2323port]       =   "0";
    configvalue_[kctl4850port]       =   "0";
    configvalue_[kctl4851port]       =   "0";
    configvalue_[kctl4852port]       =   "0";
    configvalue_[kctl4853port]       =   "0";
    configvalue_[kctlrelayport]     =   "0";
    configvalue_[kctlrelay0port]     =   "0";
    configvalue_[kctlrelay1port]     =   "0";
    configvalue_[kctlinfraredport]  =   "0";
    configvalue_[kctlinfraredport_tc]  =   "0";
    configvalue_[kctlIO0port]       =   "0";
    configvalue_[kctlIO1port]       =   "0";
    configvalue_[kctlIO2port]       =   "0";
    configvalue_[kctlIO3port]       =   "0";
    configvalue_[kctlIO4port]       =   "0";
    configvalue_[kctlIO5port]       =   "0";
    configvalue_[kctlIO6port]       =   "0";
    configvalue_[kctlIO7port]       =   "0";
    //板子UDP接收端口，原则上不变
    configvalue_[ksv232port]        =   "16232";
    configvalue_[ksv485port]        =   "16485";
    configvalue_[ksvrelayport]      =   "16456";
    configvalue_[kinfraredport]     =   "16457";
    configvalue_[ksvIOport]         =   "16458";
    configvalue_[ksvIOport_A258]    =   "16459";
    //RS232--串口使能和属性
    configvalue_[ken232]            =   "1";
    configvalue_[k232port]          =   "com1";
    configvalue_[k232uartspeed]     =   "115200";
    configvalue_[k232nbit]          =   "8";
    configvalue_[k232nstop]         =   "1";
    configvalue_[k232event]         =   "N";
    configvalue_[k232vtime]         =   "3";
    configvalue_[k232vmin]          =   "10";

    configvalue_[ken2320]            =   "1";
    configvalue_[k2320port]          =   "2320";
    configvalue_[k2320uartspeed]     =   "115200";
    configvalue_[k2320nbit]          =   "8";
    configvalue_[k2320nstop]         =   "1";
    configvalue_[k2320event]         =   "N";
    configvalue_[k2320vtime]         =   "3";
    configvalue_[k2320vmin]          =   "10";

    configvalue_[ken2321]            =   "1";
    configvalue_[k2321port]          =   "2321";
    configvalue_[k2321uartspeed]     =   "115200";
    configvalue_[k2321nbit]          =   "8";
    configvalue_[k2321nstop]         =   "1";
    configvalue_[k2321event]         =   "N";
    configvalue_[k2321vtime]         =   "3";
    configvalue_[k2321vmin]          =   "10";

    configvalue_[ken2322]            =   "1";
    configvalue_[k2322port]          =   "2322";
    configvalue_[k2322uartspeed]     =   "115200";
    configvalue_[k2322nbit]          =   "8";
    configvalue_[k2322nstop]         =   "1";
    configvalue_[k2322event]         =   "N";
    configvalue_[k2322vtime]         =   "3";
    configvalue_[k2322vmin]          =   "10";

    configvalue_[ken2323]            =   "1";
    configvalue_[k2323port]          =   "2323";
    configvalue_[k2323uartspeed]     =   "115200";
    configvalue_[k2323nbit]          =   "8";
    configvalue_[k2323nstop]         =   "1";
    configvalue_[k2323event]         =   "N";
    configvalue_[k2323vtime]         =   "3";
    configvalue_[k2323vmin]          =   "10";
    //RS485--串口使能和属性
    configvalue_[ken485]            =   "0";
    configvalue_[k485port]          =   "com2";
    configvalue_[k485uartspeed]     =   "9600";
    configvalue_[k485nbit]          =   "8";
    configvalue_[k485nstop]         =   "1";
    configvalue_[k485event]         =   "N";
    configvalue_[k485vtime]         =   "3";
    configvalue_[k485vmin]          =   "10";

    configvalue_[ken4850]            =   "1";
    configvalue_[k4850port]          =   "4850";
    configvalue_[k4850uartspeed]     =   "9600";
    configvalue_[k4850nbit]          =   "8";
    configvalue_[k4850nstop]         =   "1";
    configvalue_[k4850event]         =   "N";
    configvalue_[k4850vtime]         =   "3";
    configvalue_[k4850vmin]          =   "10";

    configvalue_[ken4851]            =   "1";
    configvalue_[k4851port]          =   "4851";
    configvalue_[k4851uartspeed]     =   "9600";
    configvalue_[k4851nbit]          =   "8";
    configvalue_[k4851nstop]         =   "1";
    configvalue_[k4851event]         =   "N";
    configvalue_[k4851vtime]         =   "3";
    configvalue_[k4851vmin]          =   "10";

    configvalue_[ken4852]            =   "1";
    configvalue_[k4852port]          =   "4852";
    configvalue_[k4852uartspeed]     =   "9600";
    configvalue_[k4852nbit]          =   "8";
    configvalue_[k4852nstop]         =   "1";
    configvalue_[k4852event]         =   "N";
    configvalue_[k4852vtime]         =   "3";
    configvalue_[k4852vmin]          =   "10";

    configvalue_[ken4853]            =   "1";
    configvalue_[k4853port]          =   "4853";
    configvalue_[k4853uartspeed]     =   "9600";
    configvalue_[k4853nbit]          =   "8";
    configvalue_[k4853nstop]         =   "1";
    configvalue_[k4853event]         =   "N";
    configvalue_[k4853vtime]         =   "3";
    configvalue_[k4853vmin]          =   "10";
    //继电器
    configvalue_[kenrelay]          =   "0";
    configvalue_[krelayst]          =   "0";
    configvalue_[kenrelay0]          =   "0";
    configvalue_[krelay0st]          =   "0";
    configvalue_[kenrelay1]          =   "0";
    configvalue_[krelay1st]          =   "0";
    //红外---使能和属性（GPIO模拟和串口两种形式，通过硬件号区分）
    configvalue_[kenInfrared]       =   "0";
    configvalue_[kenInfrared_tc]    =   "0";
    configvalue_[kInfraredport]     =   "com2";
    configvalue_[kInfrareduartspeed]=   "9600";
    configvalue_[kInfrarednbit]     =   "8";
    configvalue_[kInfrarednstop]    =   "1";
    configvalue_[kInfraredevent]    =   "N";
    configvalue_[kInfraredvtime]    =   "3";
    configvalue_[kInfraredvmin]     =   "10";
    //IO--0(未使能)，1（接收），2（发送）
    configvalue_[kSet0IO]           =   "0";
    configvalue_[kIO0st]            =   "0";
    configvalue_[kSet1IO]           =   "0";
    configvalue_[kIO1st]            =   "0";
    configvalue_[kSet2IO]           =   "0";
    configvalue_[kIO2st]            =   "0";
    configvalue_[kSet3IO]           =   "0";
    configvalue_[kIO3st]            =   "0";
    configvalue_[kSet4IO]           =   "0";
    configvalue_[kIO4st]            =   "0";
    configvalue_[kSet5IO]           =   "0";
    configvalue_[kIO5st]            =   "0";
    configvalue_[kSet6IO]           =   "2";
    configvalue_[kIO6st]            =   "1";
    configvalue_[kSet7IO]           =   "0";
    configvalue_[kIO7st]            =   "0";

#endif
    //TCP
    configvalue_[kTcpEnable]        =   "1";
    configvalue_[kTcpServerPort]    =   "8899";
    configvalue_[kTcpLinkMax]       =   "32";

    this->SaveConfig();
    return true;
}



void SoftwareConfig::do_set_net(const char *kIp, const char *kMask, const char *kGateway,
                                const char *kDns, const char* kEthName )
{
    if( 0 == strcmp( kIp, "0.0.0.0" ) ) {
        printf("[%s] ip not set, use default ip.", kEthName);
        return;
    }

    char cmd[1024]={0};
    // set ip and netmask
    memset( cmd, 0, 1024 );
    sprintf( cmd, "ifconfig %s %s netmask %s", kEthName, kIp, kMask );
    string ret;
    LinuxShell(cmd, ret);
    printf( "%s", cmd );
    // set dns
    memset( cmd, 0, 1024 );
    sprintf( cmd, "nameserver %s", kDns );
    FILE * f = fopen( DNS_NAME, "wb");
    if( f ){
        fwrite( cmd, strlen( (const char*)(cmd) ), 1, f );
        fclose( f );
    }

    if( 0 == strcmp( kGateway, "0.0.0.0" ) ) {
        printf("[%s] gateway not set, use default gateway.", kEthName);
        return;
    }
    // del gateway
    sprintf( cmd, "route del default gw %s dev %s", "0.0.0.0", kEthName);
    LinuxShell(cmd, ret);
   printf( "%s", cmd );
    // set gateway
    memset ( cmd, 0, 1024 );
    sprintf( cmd, "route add default gw %s dev %s", kGateway, kEthName);
    LinuxShell(cmd, ret);
    printf( "%s", cmd );
}

void SoftwareConfig::do_set_mac(const char* kMac, const char* kEthName )
{
    if( 0 == strcmp( kMac, "00:00:00:00:00:00" ) ) {
        printf("[%s] mac not set, use default mac.", kEthName);
        return;
    }

    char buffer[1024];
    sprintf(buffer, "ifconfig %s down", kEthName);
    string ret;
    LinuxShell(buffer, ret);
    printf( "%s", buffer );

    sprintf(buffer, "ifconfig %s hw ether %s", kEthName, kMac );
    printf("%s", buffer);
    LinuxShell(buffer, ret);
   printf( "%s", buffer );

    sprintf(buffer, "ifconfig %s up", kEthName );
    LinuxShell(buffer, ret);
    printf( "%s", buffer );
}

