#include "relay.h"

relay::relay()
{

}

relay::~relay()
{
//    close(socketRelay);
//    socketRelay = -1;
    relay0Str = 0;
    relay1Str = 0;
//    relaySet(relayStr);
}

bool relay::relayIni()
{
    uint value;
    if(hd_ver == GZ_700S) {
        set_reg(RELAY_36_REG,0x0);
        relayStr = global_data.relaySt;
        relaySet(relayStr);

        get_reg(RELAY_36_DIR);
        value |= RELAY_36_SET;
        set_reg(RELAY_36_DIR,value);
    }

    else if(hd_ver == GZ_EDC) {
        set_reg(RELAY_REG_EDC,0x0);
        relayStr = global_data.relaySt;
        relaySet(relayStr);

        get_reg(RELAY_DIR_EDC);
        value |= RELAY_SET_EDC;
        set_reg(RELAY_DIR_EDC,value);
    }
    else if(hd_ver == NJ_31DV200) {
        get_reg(RELAY_0_REG);
        value &= (~0x1);
        set_reg(RELAY_0_REG, value);

        get_reg(RELAY_0_DIR);
        value |= RELAY_0_SET;
        set_reg(RELAY_0_DIR, value);

        get_reg(RELAY_1_REG);
        value &= (~0x1);
        set_reg(RELAY_1_REG, value);

        get_reg(RELAY_1_DIR);
        value |= RELAY_1_SET;
        set_reg(RELAY_1_DIR, value);
    }

        else if(hd_ver == NJ_A258) {                   //添加A258继电器初始化
        if(1==enRelay0){
            value=get_reg(RELAY_A258_0_REG);
            value &= (~0x1);
            set_reg(RELAY_A258_0_REG, value);

            value=get_reg(RELAY_A258_0_DIR);
            value |= RELAY_A258_0_SET;
            set_reg(RELAY_A258_0_DIR, value);
        }

        if(1==enRelay1){
            value=get_reg(RELAY_A258_1_REG);
            value &= (~0x1);
            set_reg(RELAY_A258_1_REG, value);

            value=get_reg(RELAY_A258_1_DIR);
            value |= RELAY_A258_1_SET;
            set_reg(RELAY_A258_1_DIR, value);
        }
    }
    else {
        value=set_reg(RELAY_REG,0x1);
        relayStr = global_data.relaySt;
        relaySet(relayStr);

        value=get_reg(RELAY_DIR);
        value |= RELAY_SET;
        set_reg(RELAY_DIR,value);

    }


    if(-1==socketRelay){
        socketRelay = createUdp(global_data.svrelayport);
        FD_SET(socketRelay,&readFd);
        if(socketRelay >= maxFd)
            maxFd = socketRelay + 1;
    }

    return true;
}

//bool relay::rcvData2Relay()
//{
//    char tmpData[128] = {0};
//    char relayEcho[8] = {0};
//    int rcvLen = 0;
//    socklen_t addrLen = 0;
//    int  sendLen;
//    memset(tmpData,0,sizeof(tmpData));
//    auto thisini = Singleton<SoftwareConfig>::getInstance();

//    addrLen = sizeof(struct sockaddr_in);

//    rcvLen = recvfrom(socketRelay,tmpData,MAXDATA,0,(struct sockaddr*)(&ctrlAddr),&addrLen);
//    if(rcvLen > 0 ) {
//#ifdef __DEBUG__
//        print0x(tmpData,rcvLen);
//#endif
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
//        else if(!strncmp(tmpData, "relay_0=ON", strlen("relay_0=ON"))) {
//            //printf("set relay 0  on\n");
//            relaySet(1, 0);
//        }
//        else if(!strncmp(tmpData, "relay_1=ON", strlen("relay_1=ON"))) {
//            relaySet(1, 1);
//            //printf("set relay 1  on\n");
//        }
//        else if(!strncmp(tmpData, "relay_0=OFF", strlen("relay_0=OFF"))) {
//            relaySet(0, 0);
//            //printf("set relay 0  off\n");
//        }
//        else if(!strncmp(tmpData, "relay_1=OFF", strlen("relay_1=OFF"))) {
//            relaySet(0, 1);
//            //printf("set relay 1  off\n");
//        }
//        else if(!strncmp(tmpData,"GET",3)) {
//            memset(relayEcho,0,sizeof(relayEcho));
//            if(relayStr == 1) {
//                strcpy(relayEcho,"ON");
//            }
//            else if(relayStr == 0) {
//                strcpy(relayEcho,"OFF");
//            }
//            if((global_data.ctlrelayport != 0) && (strlen(global_data.ctlip) >= 7)) {
//                ctrlAddr.sin_family = AF_INET;
//                ctrlAddr.sin_port = htons(global_data.ctlrelayport);
//                ctrlAddr.sin_addr.s_addr = inet_addr(global_data.ctlip);
//                sendLen = sendto(socketRelay,relayEcho,strlen(relayEcho),0,(struct sockaddr*)&ctrlAddr,sizeof(struct sockaddr_in));
//                if(sendLen > 0)
//                    return true;
//                else {
//                    COMMON_PRT("send dev relay str to ctrl error\n");
//                    return false;
//                }
//            }
//            else {
//                sendLen = sendto(socketRelay,relayEcho,strlen(relayEcho),0,(struct sockaddr*)&ctrlAddr,sizeof(struct sockaddr_in));
//                if(sendLen > 0) {
//                    COMMON_PRT("UMP not set ctrl IP PORT and echo\n");
//                    return true;
//                }
//                else {
//                    COMMON_PRT("send dev relay str to ctrl error\n");
//                    return false;
//                }
//            }

//        }
//        else {
//            COMMON_PRT("ctrl relay send error data:%s\n",tmpData);
//            return false;
//        }
//        thisini->SaveConfig();

//    }
//    else {
//        COMMON_PRT("recv ctrl relay data error\n");
//        return false;
//    }
//    return true;
//}

bool relay::relaySet(int set, int relay_num)
{
    uint value;
    if(0 == relay_num) {
        if(set == 1) {
            if(hd_ver == GZ_700S) {
                get_reg(RELAY_36_DATA);
                value |= RELAY_36_SET;
                set_reg(RELAY_36_DATA,value);
            }

            else if(hd_ver == GZ_EDC) {
                get_reg(RELAY_DATA_EDC);
                value |= RELAY_SET_EDC;
                set_reg(RELAY_DATA_EDC,value);
            }
            else if(hd_ver == NJ_31DV200) {
                get_reg(RELAY_0_DATA);
                value |= RELAY_0_SET;
                set_reg(RELAY_0_DATA, value);
                printf("set relay 0 on\n");
            }
            else if(hd_ver == NJ_A258) {                            //添加A258继电器0设置 开，关
                value=get_reg(RELAY_A258_0_DATA);
                value |= RELAY_A258_0_SET;
                set_reg(RELAY_A258_0_DATA, value);
                printf("set A258_relay 0 on\n");
            }
            else {
                value=get_reg(RELAY_DATA);
                value |= RELAY_SET;
                set_reg(RELAY_DATA,value);
            }
        }
        else if(set == 0) {
            if(hd_ver == GZ_700S) {
                get_reg(RELAY_36_DATA);
                value &= (~RELAY_36_SET);
                set_reg(RELAY_36_DATA,value);
            }
            else if(hd_ver == GZ_EDC) {
                get_reg(RELAY_DATA_EDC);
                value &= (~RELAY_SET_EDC);
                set_reg(RELAY_DATA_EDC,value);
            }
            else if(hd_ver == NJ_31DV200) {
                get_reg(RELAY_0_DATA);
                value &= (~RELAY_0_SET);
                set_reg(RELAY_0_DATA, value);
            }
            else if(hd_ver == NJ_A258) {
                value=get_reg(RELAY_A258_0_DATA);
                value &= (~RELAY_A258_0_SET);
                set_reg(RELAY_A258_0_DATA, value);
                printf("set A258_relay 0 off\n");
            }
            else {
                value=get_reg(RELAY_DATA);
                value &= (~RELAY_SET);
                set_reg(RELAY_DATA,value);
            }
        }
        else {
            COMMON_PRT("relay set value error\n");
            return  false;
        }
    }
    else if(1 == relay_num){
        if(set == 1) {
            if(hd_ver == NJ_31DV200) {
                get_reg(RELAY_1_DATA);
                value |= RELAY_1_SET;
                set_reg(RELAY_1_DATA, value);
            }
            else if(hd_ver == NJ_A258) {                                    //添加A258继电器1设置 开，关 
                value=get_reg(RELAY_A258_1_DATA);
                value |= RELAY_A258_1_SET;
                set_reg(RELAY_A258_1_DATA, value);
                printf("set A258_relay 1 on\n");
            }

        }
        else if(set == 0) {
            if(hd_ver == NJ_31DV200) {
                get_reg(RELAY_1_DATA);
                value &= (~RELAY_1_SET);
                set_reg(RELAY_1_DATA, value);
            }
            else if(hd_ver == NJ_A258) {
                value=get_reg(RELAY_A258_1_DATA);
                value &= (~RELAY_A258_1_SET);
                set_reg(RELAY_A258_1_DATA, value);
                printf("set A258_relay 1 off\n");
            }
        }
        else {
            COMMON_PRT("relay set value error\n");
            return  false;
        }
    }


    return true;
}

/*
 * 配合UMP设置，时时生效
 * 全局参数在外部修改，保存
 * */

bool relay::UMPsetParam(bool uartFlag,bool ctrlPortFlag,bool svPortFlag)
{

    if(ctrlPortFlag) {
        COMMON_PRT("UMP set relay ctrl port Param\n");
    }

    if(svPortFlag) {
        FD_CLR(socketRelay,&readFd);
        close(socketRelay);
        sleep(1);
        if(-1==socketRelay){
            socketRelay = createUdp(global_data.svrelayport);
            FD_SET(socketRelay,&readFd);
            if(socketRelay >= maxFd)
                maxFd = socketRelay + 1;
        }
    }
    return true;
}
