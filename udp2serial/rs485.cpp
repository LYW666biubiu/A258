#include "rs485.h"

rs485::rs485()
{

}

rs485::~rs485()
{
    FD_CLR(dev485Fd,&readFd);
//    FD_CLR(socket485,&readFd);
    close(dev485Fd);
//    close(socket485);
    dev485Fd = -1;
//    socket485 = -1;
}

bool rs485::iniRs485()
{
    uint value;

    if(hd_ver == GZ_700S) {
        set_reg(GPIO7_3REG, 0x0);

        get_reg(GPIO7_3DIR);
        value |= GPIO7_3SET;
        set_reg(GPIO7_3DIR,value);

        get_reg(GPIO7_3DATA);
        value &= (~GPIO7_3SET);
        set_reg(GPIO7_3DATA,value);
    }
    else if(hd_ver == NJ_31DV200) {
        get_reg(V200_GPIO3_0REG);
        value &= (~0x1);
        set_reg(V200_GPIO3_0REG, value);

        get_reg(V200_GPIO3_0DIR);
        value |= V200_GPIO3_0SET;
        set_reg(V200_GPIO3_0DIR, value);

        get_reg(V200_GPIO3_0DATA);
        value &= (~V200_GPIO3_0SET);
        set_reg(V200_GPIO3_0DATA, value);
    }
    else {
        set_reg(GPIO4_5REG,0x1);

        get_reg(GPIO4_5DIR);
        value |= GPIO4_5SET;
        set_reg(GPIO4_5DIR,value);

        get_reg(GPIO4_5DATA);
        value &= (~GPIO4_5SET);
        set_reg(GPIO4_5DATA,value);
    }


#ifdef __SEND_485__
    get_reg(GPIO4_5DATA);
    value |= GPIO4_5SET;
    set_reg(GPIO4_5DATA,value);

#endif

    open_port(global_data.port485,&dev485Fd);
    set_optBak(global_data.uartspeed485,global_data.nbit485,global_data.nstop485,   \
               global_data.event485[0],global_data.nVtime485,global_data.nVmin485,&dev485Fd);

    FD_SET(dev485Fd,&readFd);
    if(dev485Fd >= maxFd)
        maxFd = dev485Fd + 1;
    socket485 = createUdp(global_data.sv485port);
    FD_SET(socket485,&readFd);
    if(socket485 >= maxFd)
        maxFd = socket485 + 1;

    return true;
}

bool rs485::rcvData2uart()
{
    char tmpData[128] = {0};
    int rcvLen = 0;
    socklen_t addrLen = 0;
    uint value;
    memset(tmpData,0,sizeof(tmpData));

    addrLen = sizeof(struct sockaddr_in);

    rcvLen = recvfrom(socket485,tmpData,MAXDATA,0,(struct sockaddr*)(&ctrlAddr),&addrLen);
    if(rcvLen > 0) {
#ifdef __DEBUG__
    print0x(tmpData,rcvLen);
#endif

#ifndef __SEND_485__
        if(hd_ver == GZ_700S) {
            get_reg(GPIO7_3DATA);
            value |= GPIO7_3SET;
            set_reg(GPIO7_3DATA, value);
            usleep(500000);
        }
        else if(hd_ver == NJ_31DV200) {
            get_reg(V200_GPIO3_0DATA);
            value |= V200_GPIO3_0SET;
            set_reg(V200_GPIO3_0DATA, value);
            usleep(500000);
        }
        else {
            get_reg(GPIO4_5DATA);
            value |= GPIO4_5SET;
            set_reg(GPIO4_5DATA,value);
            usleep(500000);
        }

        write(dev485Fd,tmpData,rcvLen);
        usleep(500000);

        if(hd_ver == GZ_700S) {
            get_reg(GPIO7_3DATA);
            value &= (~GPIO7_3SET);
            set_reg(GPIO7_3DATA, value);
        }
        else if(hd_ver == NJ_31DV200) {
            get_reg(V200_GPIO3_0DATA);
            value &= (~V200_GPIO3_0SET);
            set_reg(V200_GPIO3_0DATA, value);
        }
        else {
            get_reg(GPIO4_5DATA);
            value &= (~GPIO4_5SET);
            set_reg(GPIO4_5DATA,value);
        }

#else
        write(dev485Fd,tmpData,rcvLen);
#endif
    }
    else {
        COMMON_PRT("net not rcv 485data\n");
        return false;
    }
    return true;
}


bool rs485::readUart2ctrl()
{
    char tmpData[128] = {0};
    int rdLen = 0, sendLen = 0;
    memset(tmpData,0,sizeof(tmpData));
    int addrLen = 0;

    rdLen = read(dev485Fd,tmpData,MAXDATA);
#ifdef __DEBUG__
    print0x(tmpData,rdLen);
#endif
    if(rdLen > 0) {
        if((global_data.ctl485port != 0) && (strlen(global_data.ctlip) >= 7)) {
            ctrlAddr.sin_family = AF_INET;
            ctrlAddr.sin_port = htons(global_data.ctl485port);
            ctrlAddr.sin_addr.s_addr = inet_addr(global_data.ctlip);
            addrLen = sizeof(struct sockaddr_in);
            sendLen = sendto(socket485,tmpData,rdLen,0,(struct sockaddr*)&ctrlAddr,addrLen);
            if(sendLen > 0)
                return true;
            else {
                perror("send dev 485 data to ctrl error\n");
                return false;
            }
        }
        else {
            COMMON_PRT("UMP not set ctrl IP PORT\n");
            return true;
        }
    }
    else {
        COMMON_PRT("RS485 dev not read data\n");
        return false;
    }
    return true;
}

/*
 * 配合UMP设置，时时生效
 * 全局参数在外部修改，保存
 * */

bool rs485::UMPsetParam(bool uartFlag,bool ctrlPortFlag,bool svPortFlag)
{
    if(uartFlag) {
        COMMON_PRT("UMP set RS485 UART Param\n");
        FD_CLR(dev485Fd,&readFd);
        close(dev485Fd);
        sleep(1);

        dev485Fd = -1;
        open_port(global_data.port485,&dev485Fd);
        set_optBak(global_data.uartspeed485,global_data.nbit485,global_data.nstop485,   \
                   global_data.event485[0],global_data.nVtime485,global_data.nVmin485,&dev485Fd);
        FD_SET(dev485Fd,&readFd);
        if(dev485Fd >= maxFd)
            maxFd = dev485Fd + 1;

    }

    if(ctrlPortFlag) {
        COMMON_PRT("UMP set RS485 ctrl port Param\n");
    }

    if(svPortFlag) {
        FD_CLR(socket485,&readFd);
        close(socket485);
        sleep(1);

        socket485 = createUdp(global_data.sv485port);
        FD_SET(socket485,&readFd);
        if(socket485 >= maxFd)
            maxFd = socket485 + 1;
    }
    return true;
}


//4850

rs4850::rs4850()
{

}

rs4850::~rs4850()
{
    FD_CLR(dev485Fd,&readFd);
    //FD_CLR(socket485,&readFd);
    close(dev485Fd);
    //close(socket485);
    dev485Fd = -1;
    //socket485 = -1;
}

bool rs4850::iniRs485()
{

    printf("-----ini 4850-----\n");
    open_port(global_data.port4850,&dev485Fd);
    set_optBak(global_data.uartspeed4850,global_data.nbit4850,global_data.nstop4850,   \
               global_data.event4850[0],global_data.nVtime4850,global_data.nVmin4850,&dev485Fd);

    FD_SET(dev485Fd,&readFd);
    if(dev485Fd >= maxFd)
        maxFd = dev485Fd + 1;
    if(-1==socket485){
        socket485 = createUdp(global_data.sv485port);
        FD_SET(socket485,&readFd);
        if(socket485 >= maxFd)
        maxFd = socket485 + 1;
    }
    return true;
}

bool rs4850::rcvData2uart()
{
    char tmpData[128] = {0};
    int rcvLen = 0;
    socklen_t addrLen = 0;
    uint value;
    memset(tmpData,0,sizeof(tmpData));

    addrLen = sizeof(struct sockaddr_in);

    rcvLen = recvfrom(socket485,tmpData,MAXDATA,0,(struct sockaddr*)(&ctrlAddr),&addrLen);
    if(rcvLen > 0)
    {
#ifdef __DEBUG__
        print0x(tmpData,rcvLen);
#endif

        if(tmpData[0]==0x48&&tmpData[1]==0x50)
        {
            write(dev485Fd,tmpData,rcvLen);
            printf("write 4850 ok\n");
        }
    }


    else {
        COMMON_PRT("net not rcv 4850data\n");
        return false;
    }
    return true;
}

bool rs4850::readUart2ctrl()
{
   printf("rs4850 read ready\n");
    char tmpData[128] = {0};
    int rdLen = 0, sendLen = 0;
    memset(tmpData,0,sizeof(tmpData));
    int addrLen = 0;

    tmpData[0]=0x48;
    tmpData[1]=0x50;
    rdLen = read(dev485Fd,&tmpData[2],MAXDATA);
#ifdef __DEBUG__
    //print0x(tmpData,rdLen+2);
#endif
    if(rdLen > 0) {
        if((global_data.ctl4850port != 0) && (strlen(global_data.ctlip) >= 7)) {
            ctrlAddr_A258_485.sin_family = AF_INET;
            ctrlAddr_A258_485.sin_port = htons(global_data.ctl4850port);
            ctrlAddr_A258_485.sin_addr.s_addr = inet_addr(global_data.ctlip);
            addrLen = sizeof(struct sockaddr_in);
            sendLen = sendto(socket485,tmpData,rdLen+2,0,(struct sockaddr*)&ctrlAddr_A258_485,addrLen);
            if(sendLen > 0)
                return true;
            else {
                perror("send dev 4850 data to ctrl error\n");
                return false;
            }
        }
        else {
            COMMON_PRT("UMP not set ctrl IP PORT\n");
            return true;
        }
    }
    else {
        COMMON_PRT("RS4850 dev not read data\n");
        return false;
    }
    return true;
}

bool rs4850::UMPsetParam(bool uartFlag,bool ctrlPortFlag,bool svPortFlag)
{
    if(uartFlag) {
        COMMON_PRT("UMP set RS4850 UART Param\n");
        FD_CLR(dev485Fd,&readFd);
        close(dev485Fd);
        sleep(1);

        dev485Fd = -1;
        open_port(global_data.port4850,&dev485Fd);
        set_optBak(global_data.uartspeed4850,global_data.nbit4850,global_data.nstop4850,   \
                   global_data.event4850[0],global_data.nVtime4850,global_data.nVmin4850,&dev485Fd);
        FD_SET(dev485Fd,&readFd);
        if(dev485Fd >= maxFd)
            maxFd = dev485Fd + 1;

    }

    if(ctrlPortFlag) {
        COMMON_PRT("UMP set RS4850 ctrl port Param\n");
    }

    if(svPortFlag) {
        FD_CLR(socket485,&readFd);
        close(socket485);
        sleep(1);

        socket485 = createUdp(global_data.sv485port);
        FD_SET(socket485,&readFd);
        if(socket485 >= maxFd)
            maxFd = socket485 + 1;
    }
    return true;
}

//4851


rs4851::rs4851()
{

}

rs4851::~rs4851()
{
    FD_CLR(dev485Fd,&readFd);
    //FD_CLR(socket485,&readFd);
    close(dev485Fd);
    //close(socket485);
    dev485Fd = -1;
    //socket485 = -1;
}

bool rs4851::iniRs485()
{

    printf("-----ini 4851-----\n");
    open_port(global_data.port4851,&dev485Fd);
    set_optBak(global_data.uartspeed4851,global_data.nbit4851,global_data.nstop4851,   \
               global_data.event4851[0],global_data.nVtime4851,global_data.nVmin4851,&dev485Fd);

    FD_SET(dev485Fd,&readFd);
    if(dev485Fd >= maxFd)
        maxFd = dev485Fd + 1;
    if(-1==socket485){
        socket485 = createUdp(global_data.sv485port);
        FD_SET(socket485,&readFd);
        if(socket485 >= maxFd)
        maxFd = socket485 + 1;
    }
    return true;
}

bool rs4851::rcvData2uart()
{
    char tmpData[128] = {0};
    int rcvLen = 0;
    socklen_t addrLen = 0;
    uint value;
    memset(tmpData,0,sizeof(tmpData));

    addrLen = sizeof(struct sockaddr_in);

    rcvLen = recvfrom(socket485,tmpData,MAXDATA,0,(struct sockaddr*)(&ctrlAddr),&addrLen);
    if(rcvLen > 0)
    {
#ifdef __DEBUG__
        print0x(tmpData,rcvLen);
#endif

        if(tmpData[0]==0x48&&tmpData[1]==0x51)
        {
            write(dev485Fd,tmpData,rcvLen);
            printf("write 4851 ok\n");
        }
    }


    else {
        COMMON_PRT("net not rcv 4851data\n");
        return false;
    }
    return true;
}

bool rs4851::readUart2ctrl()
{
    printf("rs4850 read ready\n");
    char tmpData[128] = {0};
    int rdLen = 0, sendLen = 0;
    memset(tmpData,0,sizeof(tmpData));
    int addrLen = 0;

    tmpData[0]=0x48;
    tmpData[1]=0x51;
    rdLen = read(dev485Fd,&tmpData[2],MAXDATA);
#ifdef __DEBUG__
    //print0x(tmpData,rdLen+2);
#endif
    if(rdLen > 0) {
        if((global_data.ctl4851port != 0) && (strlen(global_data.ctlip) >= 7)) {
            ctrlAddr_A258_485.sin_family = AF_INET;
            ctrlAddr_A258_485.sin_port = htons(global_data.ctl4851port);
            ctrlAddr_A258_485.sin_addr.s_addr = inet_addr(global_data.ctlip);
            addrLen = sizeof(struct sockaddr_in);
            sendLen = sendto(socket485,tmpData,rdLen+2,0,(struct sockaddr*)&ctrlAddr_A258_485,addrLen);
            if(sendLen > 0)
                return true;
            else {
                perror("send dev 4851 data to ctrl error\n");
                return false;
            }
        }
        else {
            COMMON_PRT("UMP not set ctrl IP PORT\n");
            return true;
        }
    }
    else {
        COMMON_PRT("RS4851 dev not read data\n");
        return false;
    }
    return true;
}

bool rs4851::UMPsetParam(bool uartFlag,bool ctrlPortFlag,bool svPortFlag)
{
    if(uartFlag) {
        COMMON_PRT("UMP set RS4851 UART Param\n");
        FD_CLR(dev485Fd,&readFd);
        close(dev485Fd);
        sleep(1);

        dev485Fd = -1;
        open_port(global_data.port4851,&dev485Fd);
        set_optBak(global_data.uartspeed4851,global_data.nbit4851,global_data.nstop4851,   \
                   global_data.event4851[0],global_data.nVtime4851,global_data.nVmin4851,&dev485Fd);
        FD_SET(dev485Fd,&readFd);
        if(dev485Fd >= maxFd)
            maxFd = dev485Fd + 1;

    }

    if(ctrlPortFlag) {
        COMMON_PRT("UMP set RS4851 ctrl port Param\n");
    }

    if(svPortFlag) {
        FD_CLR(socket485,&readFd);
        close(socket485);
        sleep(1);

        socket485 = createUdp(global_data.sv485port);
        FD_SET(socket485,&readFd);
        if(socket485 >= maxFd)
            maxFd = socket485 + 1;
    }
    return true;
}

//4852

rs4852::rs4852()
{

}

rs4852::~rs4852()
{
    FD_CLR(dev485Fd,&readFd);
    //FD_CLR(socket485,&readFd);
    close(dev485Fd);
    //close(socket485);
    dev485Fd = -1;
    //socket485 = -1;
}

bool rs4852::iniRs485()
{

    printf("-----ini 4852-----\n");
    open_port(global_data.port4852,&dev485Fd);
    set_optBak(global_data.uartspeed4852,global_data.nbit4852,global_data.nstop4852,   \
               global_data.event4852[0],global_data.nVtime4852,global_data.nVmin4852,&dev485Fd);

    FD_SET(dev485Fd,&readFd);
    if(dev485Fd >= maxFd)
        maxFd = dev485Fd + 1;
    if(-1==socket485){
        socket485 = createUdp(global_data.sv485port);
        FD_SET(socket485,&readFd);
        if(socket485 >= maxFd)
        maxFd = socket485 + 1;
    }
    return true;
}

bool rs4852::rcvData2uart()
{
    char tmpData[128] = {0};
    int rcvLen = 0;
    socklen_t addrLen = 0;
    uint value;
    memset(tmpData,0,sizeof(tmpData));

    addrLen = sizeof(struct sockaddr_in);

    rcvLen = recvfrom(socket485,tmpData,MAXDATA,0,(struct sockaddr*)(&ctrlAddr),&addrLen);
    if(rcvLen > 0)
    {
#ifdef __DEBUG__
        print0x(tmpData,rcvLen);
#endif

        if(tmpData[0]==0x48&&tmpData[1]==0x52)
        {
            write(dev485Fd,tmpData,rcvLen);
            printf("write 4852 ok\n");
        }
    }


    else {
        COMMON_PRT("net not rcv 4852data\n");
        return false;
    }
    return true;
}

bool rs4852::readUart2ctrl()
{
    char tmpData[128] = {0};
    int rdLen = 0, sendLen = 0;
    memset(tmpData,0,sizeof(tmpData));
    int addrLen = 0;

    tmpData[0]=0x48;
    tmpData[1]=0x52;
    rdLen = read(dev485Fd,&tmpData[2],MAXDATA);
#ifdef __DEBUG__
    //print0x(tmpData,rdLen+2);
#endif
    if(rdLen > 0) {
        if((global_data.ctl4852port != 0) && (strlen(global_data.ctlip) >= 7)) {
            ctrlAddr_A258_485.sin_family = AF_INET;
            ctrlAddr_A258_485.sin_port = htons(global_data.ctl4852port);
            ctrlAddr_A258_485.sin_addr.s_addr = inet_addr(global_data.ctlip);
            addrLen = sizeof(struct sockaddr_in);
            sendLen = sendto(socket485,tmpData,rdLen+2,0,(struct sockaddr*)&ctrlAddr_A258_485,addrLen);
            if(sendLen > 0)
                return true;
            else {
                perror("send dev 4852 data to ctrl error\n");
                return false;
            }
        }
        else {
            COMMON_PRT("UMP not set ctrl IP PORT\n");
            return true;
        }
    }
    else {
        COMMON_PRT("RS4852 dev not read data\n");
        return false;
    }
    return true;
}

bool rs4852::UMPsetParam(bool uartFlag,bool ctrlPortFlag,bool svPortFlag)
{
    if(uartFlag) {
        COMMON_PRT("UMP set RS4852 UART Param\n");
        FD_CLR(dev485Fd,&readFd);
        close(dev485Fd);
        sleep(1);

        dev485Fd = -1;
        open_port(global_data.port4850,&dev485Fd);
        set_optBak(global_data.uartspeed4852,global_data.nbit4852,global_data.nstop4852,   \
                   global_data.event4852[0],global_data.nVtime4852,global_data.nVmin4852,&dev485Fd);
        FD_SET(dev485Fd,&readFd);
        if(dev485Fd >= maxFd)
            maxFd = dev485Fd + 1;

    }

    if(ctrlPortFlag) {
        COMMON_PRT("UMP set RS4852 ctrl port Param\n");
    }

    if(svPortFlag) {
        FD_CLR(socket485,&readFd);
        close(socket485);
        sleep(1);

        socket485 = createUdp(global_data.sv485port);
        FD_SET(socket485,&readFd);
        if(socket485 >= maxFd)
            maxFd = socket485 + 1;
    }
    return true;
}

//4853

rs4853::rs4853()
{

}

rs4853::~rs4853()
{
    FD_CLR(dev485Fd,&readFd);
    //FD_CLR(socket485,&readFd);
    close(dev485Fd);
    //close(socket485);
    dev485Fd = -1;
    //socket485 = -1;
}

bool rs4853::iniRs485()
{

    printf("-----ini 4853-----\n");
    open_port(global_data.port4853,&dev485Fd);
    set_optBak(global_data.uartspeed4853,global_data.nbit4853,global_data.nstop4853,   \
               global_data.event4853[0],global_data.nVtime4853,global_data.nVmin4853,&dev485Fd);

    FD_SET(dev485Fd,&readFd);
    if(dev485Fd >= maxFd)
        maxFd = dev485Fd + 1;
    if(-1==socket485){
        socket485 = createUdp(global_data.sv485port);
        FD_SET(socket485,&readFd);
        if(socket485 >= maxFd)
        maxFd = socket485 + 1;
    }
    return true;
}

bool rs4853::rcvData2uart()
{
    char tmpData[128] = {0};
    int rcvLen = 0;
    socklen_t addrLen = 0;
    uint value;
    memset(tmpData,0,sizeof(tmpData));

    addrLen = sizeof(struct sockaddr_in);

    rcvLen = recvfrom(socket485,tmpData,MAXDATA,0,(struct sockaddr*)(&ctrlAddr),&addrLen);
    if(rcvLen > 0)
    {
#ifdef __DEBUG__
        print0x(tmpData,rcvLen);
#endif

        if(tmpData[0]==0x48&&tmpData[1]==0x53)
        {
            write(dev485Fd,tmpData,rcvLen);
            printf("write 4853 ok\n");
        }
    }


    else {
        COMMON_PRT("net not rcv 4853data\n");
        return false;
    }
    return true;
}

bool rs4853::readUart2ctrl()
{
    printf("rs4853 read ready\n");
    char tmpData[128] = {0};
    int rdLen = 0, sendLen = 0;
    memset(tmpData,0,sizeof(tmpData));
    int addrLen = 0;

    tmpData[0]=0x48;
    tmpData[1]=0x53;
    rdLen = read(dev485Fd,&tmpData[2],MAXDATA);
#ifdef __DEBUG__
    //print0x(tmpData,rdLen+2);
#endif
    if(rdLen > 0) {
        if((global_data.ctl4853port != 0) && (strlen(global_data.ctlip) >= 7)) {
            ctrlAddr_A258_485.sin_family = AF_INET;
            ctrlAddr_A258_485.sin_port = htons(global_data.ctl4853port);
            ctrlAddr_A258_485.sin_addr.s_addr = inet_addr(global_data.ctlip);
            addrLen = sizeof(struct sockaddr_in);
            sendLen = sendto(socket485,tmpData,rdLen+2,0,(struct sockaddr*)&ctrlAddr_A258_485,addrLen);
            if(sendLen > 0)
                return true;
            else {
                perror("send dev 4853data to ctrl error\n");
                return false;
            }
        }
        else {
            COMMON_PRT("UMP not set ctrl IP PORT\n");
            return true;
        }
    }
    else {
        COMMON_PRT("RS4853 dev not read data\n");
        return false;
    }
    return true;
}

bool rs4853::UMPsetParam(bool uartFlag,bool ctrlPortFlag,bool svPortFlag)
{
    if(uartFlag) {
        COMMON_PRT("UMP set RS4853 UART Param\n");
        FD_CLR(dev485Fd,&readFd);
        close(dev485Fd);
        sleep(1);

        dev485Fd = -1;
        open_port(global_data.port4853,&dev485Fd);
        set_optBak(global_data.uartspeed4853,global_data.nbit4853,global_data.nstop4853,   \
                   global_data.event4853[0],global_data.nVtime4853,global_data.nVmin4853,&dev485Fd);
        FD_SET(dev485Fd,&readFd);
        if(dev485Fd >= maxFd)
            maxFd = dev485Fd + 1;

    }

    if(ctrlPortFlag) {
        COMMON_PRT("UMP set RS4853 ctrl port Param\n");
    }

    if(svPortFlag) {
        FD_CLR(socket485,&readFd);
        close(socket485);
        sleep(1);

        socket485 = createUdp(global_data.sv485port);
        FD_SET(socket485,&readFd);
        if(socket485 >= maxFd)
            maxFd = socket485 + 1;
    }
    return true;
}
