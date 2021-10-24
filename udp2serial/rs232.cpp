#include "rs232.h"

rs232::rs232()
{

}

rs232::~rs232()
{
    delete tcp_sv;
    FD_CLR(dev232Fd,&readFd);
//    FD_CLR(socket232,&readFd);
    close(dev232Fd);
//    close(socket232);
    dev232Fd = -1;
//    socket232 = -1;
}

void* rs232::pth_tcp_server(void *arg)
{
    rs232 * p_this = (rs232*)arg;
    p_this->tcp_sv->tcp_running(p_this->dev232Fd);

    return (void*)0;
}

bool rs232::iniRs232()
{
    printf("-----ini 232-----\n");
    open_port(global_data.port232,&dev232Fd);
    set_optBak(global_data.uartspeed232,global_data.nbit232,global_data.nstop232,   \
               global_data.event232[0],global_data.nVtime232,global_data.nVmin232,&dev232Fd);   //配置串口参数

    FD_SET(dev232Fd,&readFd);
    if(dev232Fd >= maxFd)
        maxFd = dev232Fd + 1;
    if(-1==socket232){
        socket232 = createUdp(global_data.sv232port);
        FD_SET(socket232,&readFd);
        if(socket232 >= maxFd)
        maxFd = socket232 + 1;
    }
    if(hd_232_edc) {
        open_port("com2",&dev232Fd_edc);
        set_optBak(global_data.uartspeed232,global_data.nbit232,global_data.nstop232,   \
                   global_data.event232[0],global_data.nVtime232,global_data.nVmin232,&dev232Fd_edc);
        FD_SET(dev232Fd_edc,&readFd);
        if(dev232Fd_edc >= maxFd)
            maxFd = dev232Fd_edc + 1;
        socket232_edc = createUdp(16234);
        FD_SET(socket232_edc,&readFd);
        if(socket232_edc >= maxFd)
            maxFd = socket232_edc + 1;
    }
//    if(tcp_pid == 0)
    tcp_sv = new tcp_server();

//    pthread_t tcp_pid = 0;
    if(global_data.tcp_en) {
        if(tcp_pid == 0){
        tcp_pid = CreateThread(pth_tcp_server, 99, SCHED_FIFO, true, this);
        }
        if(tcp_pid == 0 ) {
            perror("create thread pth_tcp_server error");
        }
    }

    return true;
}

bool rs232::rcvData2uart()
{

    char tmpData[128] = {0};
    int rcvLen = 0;
    socklen_t addrLen = 0;
    memset(tmpData,0,sizeof(tmpData));

    addrLen = sizeof(struct sockaddr_in);

    rcvLen = recvfrom(socket232,tmpData,MAXDATA,0,(struct sockaddr*)(&ctrlAddr),&addrLen);
    if(rcvLen > 0) {
#ifdef __DEBUG__
    print0x(tmpData,rcvLen);
#endif
        write(dev232Fd,tmpData,rcvLen);
    }
    else {
        COMMON_PRT("net not rcv 232data\n");
        return false;
    }
    return true;
}

bool rs232::rcvData2uart_edc()
{
    char tmpData[128] = {0};
    int rcvLen = 0;
    socklen_t addrLen = 0;
    memset(tmpData,0,sizeof(tmpData));

    addrLen = sizeof(struct sockaddr_in);

    rcvLen = recvfrom(socket232_edc,tmpData,MAXDATA,0,(struct sockaddr*)(&ctrlAddr),&addrLen);
    if(rcvLen > 0) {
#ifdef __DEBUG__
    print0x(tmpData,rcvLen);
#endif
        write(dev232Fd_edc,tmpData,rcvLen);
    }
    else {
        COMMON_PRT("net not rcv 232data\n");
        return false;
    }
    return true;
}


bool rs232::readUart2ctrl()
{
    char tmpData[128] = {0};
    int rdLen = 0, sendLen = 0;
    memset(tmpData,0,sizeof(tmpData));

    rdLen = read(dev232Fd,tmpData,MAXDATA);
#ifdef __DEBUG__
    print0x(tmpData,rdLen);
#endif
    if(rdLen > 0) {
        if(global_data.tcp_en) {
            tcp_sv->tcp_send(tmpData, rdLen);
        }
        if((global_data.ctl232port != 0) && (strlen(global_data.ctlip) >= 7)) {
            ctrlAddr.sin_family = AF_INET;
            ctrlAddr.sin_port = htons(global_data.ctl232port);
            ctrlAddr.sin_addr.s_addr = inet_addr(global_data.ctlip);
            sendLen = sendto(socket232,tmpData,rdLen,0,(struct sockaddr*)&ctrlAddr,sizeof(struct sockaddr_in));
            if(sendLen > 0)
                return true;
            else {
                COMMON_PRT("send dev 232 data to ctrl error\n");
                return false;
            }
        }
        else {
            COMMON_PRT("UMP not set ctrl IP PORT\n");
            return true;
        }
    }
    else {
        COMMON_PRT("RS232 dev not read data\n");
        return false;
    }
    return true;
}

bool rs232::readUart2ctrl_edc()
{
    char tmpData[128] = {0};
    int rdLen = 0, sendLen = 0;
    memset(tmpData,0,sizeof(tmpData));

    rdLen = read(dev232Fd_edc,tmpData,MAXDATA);
#ifdef __DEBUG__
    print0x(tmpData,rdLen);
#endif
    if(rdLen > 0) {
        if((global_data.ctl232port != 0) && (strlen(global_data.ctlip) >= 7)) {
            ctrlAddr.sin_family = AF_INET;
            ctrlAddr.sin_port = htons(global_data.ctl232port);
            ctrlAddr.sin_addr.s_addr = inet_addr(global_data.ctlip);
            sendLen = sendto(socket232_edc,tmpData,rdLen,0,(struct sockaddr*)&ctrlAddr,sizeof(struct sockaddr_in));
            if(sendLen > 0)
                return true;
            else {
                COMMON_PRT("send dev 232 data to ctrl error\n");
                return false;
            }
        }
        else {
            COMMON_PRT("UMP not set ctrl IP PORT\n");
            return true;
        }
    }
    else {
        COMMON_PRT("RS232 dev not read data\n");
        return false;
    }
    return true;
}

/*
 * 配合UMP设置，时时生效
 * 全局参数在外部修改，保存
 * */

bool rs232::UMPsetParam(bool uartFlag,bool ctrlPortFlag,bool svPortFlag)
{
    if(uartFlag) {
        COMMON_PRT("UMP set RS232 UART Param\n");
        FD_CLR(dev232Fd,&readFd);
        close(dev232Fd);
        sleep(1);

        open_port(global_data.port232,&dev232Fd);
        set_optBak(global_data.uartspeed232,global_data.nbit232,global_data.nstop232,   \
                   global_data.event232[0],global_data.nVtime232,global_data.nVmin232,&dev232Fd);
        FD_SET(dev232Fd,&readFd);
        if(dev232Fd >= maxFd)
            maxFd = dev232Fd + 1;
    }

    if(ctrlPortFlag) {
        COMMON_PRT("UMP set RS232 ctrl port Param\n");
    }

    if(svPortFlag) {
        FD_CLR(socket232,&readFd);
        close(socket232);
        sleep(1);

        socket232 = createUdp(global_data.sv232port);
        FD_SET(socket232,&readFd);
        if(socket232 >= maxFd)
            maxFd = socket232 + 1;
    }
    return true;
}


//2320


rs2320::rs2320()
{

}

rs2320::~rs2320()
{
    delete tcp_sv;
    FD_CLR(dev232Fd,&readFd);
//    FD_CLR(socket232,&readFd);
    close(dev232Fd);
//    close(socket232);
    dev232Fd = -1;
//    socket232 = -1;
}

void* rs2320::pth_tcp_server(void *arg)
{
    rs2320 * p_this = (rs2320*)arg;
    p_this->tcp_sv->tcp_running(p_this->dev232Fd);

    return (void*)0;
}

bool rs2320::iniRs232()
{
    printf("-----ini 2320-----\n");
    open_port(global_data.port2320,&dev232Fd);
    set_optBak(global_data.uartspeed2320,global_data.nbit2320,global_data.nstop2320,   \
               global_data.event2320[0],global_data.nVtime2320,global_data.nVmin2320,&dev232Fd);   //配置串口参数

    FD_SET(dev232Fd,&readFd);
    if(dev232Fd >= maxFd)
        maxFd = dev232Fd + 1;
    if(-1==socket232){
        socket232 = createUdp(global_data.sv232port);
        FD_SET(socket232,&readFd);
        if(socket232 >= maxFd)
            maxFd = socket232 + 1;
    }

//    if(tcp_pid == 0)
    tcp_sv = new tcp_server();

//    pthread_t tcp_pid = 0;
    if(global_data.tcp_en) {
        if(tcp_pid == 0){
        tcp_pid = CreateThread(pth_tcp_server, 99, SCHED_FIFO, true, this);
        }
        if(tcp_pid == 0 ) {
            perror("create thread pth_tcp_server error");
        }
    }

    return true;
}

bool rs2320::rcvData2uart()
{
    printf("rs2320::rcvData2uart()\n");
    char tmpData[128] = {0};
    int rcvLen = 0;
    socklen_t addrLen = 0;
    memset(tmpData,0,sizeof(tmpData));

    addrLen = sizeof(struct sockaddr_in);

    rcvLen = recvfrom(socket232,tmpData,MAXDATA,0,(struct sockaddr*)(&ctrlAddr),&addrLen);
    if(rcvLen > 0) {
#ifdef __DEBUG__
    print0x(tmpData,rcvLen);

#endif
        if(tmpData[0]==0x23&&tmpData[1]==0x20)
        {
            printf("write 2320 ready\n");
            write(dev232Fd,tmpData,rcvLen);
            printf("write 2320 ok\n");
        }
    }
    else {
        COMMON_PRT("net not rcv 2320data\n");
        return false;
    }
    return true;
}

bool rs2320::readUart2ctrl()
{
   printf("rs2320 read ready\n");
    char tmpData[128] = {0};
    int rdLen = 0, sendLen = 0;
    memset(tmpData,0,sizeof(tmpData));

    tmpData[0]=0x23;
    tmpData[1]=0x20;
    rdLen = read(dev232Fd,&tmpData[2],MAXDATA);
#ifdef __DEBUG__
    //print0x(tmpData,rdLen+2);
#endif
    if(rdLen > 0) {
        if(global_data.tcp_en) {
            tcp_sv->tcp_send(tmpData, rdLen);
        }
        if((global_data.ctl2320port != 0) && (strlen(global_data.ctlip) >= 7)) {
            ctrlAddr_A258_232.sin_family = AF_INET;
            ctrlAddr_A258_232.sin_port = htons(global_data.ctl2320port);
            ctrlAddr_A258_232.sin_addr.s_addr = inet_addr(global_data.ctlip);
            sendLen = sendto(socket232,tmpData,rdLen+2,0,(struct sockaddr*)&ctrlAddr_A258_232,sizeof(struct sockaddr_in));
            if(sendLen > 0)
                return true;
            else {
                COMMON_PRT("send dev 2320 data to ctrl error\n");
                return false;
            }
        }
        else {
            COMMON_PRT("UMP not set ctrl IP PORT\n");
            return true;
        }
    }
    else {
        COMMON_PRT("RS2320 dev not read data\n");
        return false;
    }
    return true;
}

bool rs2320::UMPsetParam(bool uartFlag,bool ctrlPortFlag,bool svPortFlag)
{
    if(uartFlag) {
        COMMON_PRT("UMP set RS2320 UART Param\n");
        FD_CLR(dev232Fd,&readFd);
        close(dev232Fd);
        sleep(1);

        open_port(global_data.port2320,&dev232Fd);
        set_optBak(global_data.uartspeed2320,global_data.nbit2320,global_data.nstop2320,   \
                   global_data.event2320[0],global_data.nVtime2320,global_data.nVmin2320,&dev232Fd);
        FD_SET(dev232Fd,&readFd);
        if(dev232Fd >= maxFd)
            maxFd = dev232Fd + 1;
    }

    if(ctrlPortFlag) {
        COMMON_PRT("UMP set RS2320 ctrl port Param\n");
    }

    if(svPortFlag) {//要注掉吗
        FD_CLR(socket232,&readFd);
        close(socket232);
        sleep(1);

        socket232 = createUdp(global_data.sv232port);
        FD_SET(socket232,&readFd);
        if(socket232 >= maxFd)
            maxFd = socket232 + 1;
    }
    return true;
}


//2321

rs2321::rs2321()
{

}

rs2321::~rs2321()
{
    delete tcp_sv;
    FD_CLR(dev232Fd,&readFd);
//    FD_CLR(socket232,&readFd);
    close(dev232Fd);
//    close(socket232);
    dev232Fd = -1;
//    socket232 = -1;
}

void* rs2321::pth_tcp_server(void *arg)
{
    rs2321 * p_this = (rs2321*)arg;
    p_this->tcp_sv->tcp_running(p_this->dev232Fd);

    return (void*)0;
}

bool rs2321::iniRs232()
{
    printf("-----ini 2321-----\n");
    open_port(global_data.port2321,&dev232Fd);
    set_optBak(global_data.uartspeed2321,global_data.nbit2321,global_data.nstop2321,   \
               global_data.event2321[0],global_data.nVtime2321,global_data.nVmin2321,&dev232Fd);   //配置串口参数

    FD_SET(dev232Fd,&readFd);
    if(dev232Fd >= maxFd)
        maxFd = dev232Fd + 1;
    if(-1==socket232){
        socket232 = createUdp(global_data.sv232port);
        FD_SET(socket232,&readFd);
        if(socket232 >= maxFd)
            maxFd = socket232 + 1;
    }

//    if(tcp_pid == 0)
    tcp_sv = new tcp_server();

//    pthread_t tcp_pid = 0;
    if(global_data.tcp_en) {
        if(tcp_pid == 0){
        tcp_pid = CreateThread(pth_tcp_server, 99, SCHED_FIFO, true, this);
        }
        if(tcp_pid == 0 ) {
            perror("create thread pth_tcp_server error");
        }
    }

    return true;
}

bool rs2321::rcvData2uart()
{
    printf("rs2321::rcvData2uart()\n");
    char tmpData[128] = {0};
    int rcvLen = 0;
    socklen_t addrLen = 0;
    memset(tmpData,0,sizeof(tmpData));

    addrLen = sizeof(struct sockaddr_in);

    rcvLen = recvfrom(socket232,tmpData,MAXDATA,0,(struct sockaddr*)(&ctrlAddr),&addrLen);
    if(rcvLen > 0) {
#ifdef __DEBUG__
    print0x(tmpData,rcvLen);
#endif
        if(tmpData[0]==0x23&&tmpData[1]==0x21)
        {
            printf("write 2321 ready\n");
            write(dev232Fd,tmpData,rcvLen);
            printf("write 2321 ok\n");
        }
    }
    else {
        COMMON_PRT("net not rcv 2321data\n");
        return false;
    }
    return true;
}

bool rs2321::readUart2ctrl()
{
    char tmpData[128] = {0};
    int rdLen = 0, sendLen = 0;
    memset(tmpData,0,sizeof(tmpData));

    tmpData[0]=0x23;
    tmpData[1]=0x21;
    rdLen = read(dev232Fd,&tmpData[2],MAXDATA);
#ifdef __DEBUG__
    //print0x(tmpData,rdLen+2);
#endif
    if(rdLen > 0) {
        if(global_data.tcp_en) {
            tcp_sv->tcp_send(tmpData, rdLen);
        }
        if((global_data.ctl2321port != 0) && (strlen(global_data.ctlip) >= 7)) {
            ctrlAddr_A258_232.sin_family = AF_INET;
            ctrlAddr_A258_232.sin_port = htons(global_data.ctl2321port);
            ctrlAddr_A258_232.sin_addr.s_addr = inet_addr(global_data.ctlip);
            sendLen = sendto(socket232,tmpData,rdLen+2,0,(struct sockaddr*)&ctrlAddr_A258_232,sizeof(struct sockaddr_in));
            if(sendLen > 0)
                return true;
            else {
                COMMON_PRT("send dev 2321 data to ctrl error\n");
                return false;
            }
        }
        else {
            COMMON_PRT("UMP not set ctrl IP PORT\n");
            return true;
        }
    }
    else {
        COMMON_PRT("RS2321 dev not read data\n");
        return false;
    }
    return true;
}

bool rs2321::UMPsetParam(bool uartFlag,bool ctrlPortFlag,bool svPortFlag)
{
    if(uartFlag) {
        COMMON_PRT("UMP set RS2321 UART Param\n");
        FD_CLR(dev232Fd,&readFd);
        close(dev232Fd);
        sleep(1);

        open_port(global_data.port2321,&dev232Fd);
        set_optBak(global_data.uartspeed2321,global_data.nbit2321,global_data.nstop2321,   \
                   global_data.event2321[0],global_data.nVtime2321,global_data.nVmin2321,&dev232Fd);
        FD_SET(dev232Fd,&readFd);
        if(dev232Fd >= maxFd)
            maxFd = dev232Fd + 1;
    }

    if(ctrlPortFlag) {
        COMMON_PRT("UMP set RS2321 ctrl port Param\n");
    }

    if(svPortFlag) {//要注掉吗
        FD_CLR(socket232,&readFd);
        close(socket232);
        sleep(1);

        socket232 = createUdp(global_data.sv232port);
        FD_SET(socket232,&readFd);
        if(socket232 >= maxFd)
            maxFd = socket232 + 1;
    }
    return true;
}


//2322

rs2322::rs2322()
{

}

rs2322::~rs2322()
{
    delete tcp_sv;
    FD_CLR(dev232Fd,&readFd);
//    FD_CLR(socket232,&readFd);
    close(dev232Fd);
//    close(socket232);
    dev232Fd = -1;
//    socket232 = -1;
}

void* rs2322::pth_tcp_server(void *arg)
{
    rs2322 * p_this = (rs2322*)arg;
    p_this->tcp_sv->tcp_running(p_this->dev232Fd);

    return (void*)0;
}

bool rs2322::iniRs232()
{
    printf("-----ini 2322-----\n");
    open_port(global_data.port2322,&dev232Fd);
    set_optBak(global_data.uartspeed2322,global_data.nbit2322,global_data.nstop2322,   \
               global_data.event2322[0],global_data.nVtime2322,global_data.nVmin2322,&dev232Fd);   //配置串口参数

    FD_SET(dev232Fd,&readFd);
    if(dev232Fd >= maxFd)
        maxFd = dev232Fd + 1;
    if(-1==socket232){
        socket232 = createUdp(global_data.sv232port);
        FD_SET(socket232,&readFd);
        if(socket232 >= maxFd)
            maxFd = socket232 + 1;
    }

//    if(tcp_pid == 0)
    tcp_sv = new tcp_server();

//    pthread_t tcp_pid = 0;
    if(global_data.tcp_en) {
        if(tcp_pid == 0){
        tcp_pid = CreateThread(pth_tcp_server, 99, SCHED_FIFO, true, this);
        }
        if(tcp_pid == 0 ) {
            perror("create thread pth_tcp_server error");
        }
    }

    return true;
}

bool rs2322::rcvData2uart()
{

    char tmpData[128] = {0};
    int rcvLen = 0;
    socklen_t addrLen = 0;
    memset(tmpData,0,sizeof(tmpData));

    addrLen = sizeof(struct sockaddr_in);

    rcvLen = recvfrom(socket232,tmpData,MAXDATA,0,(struct sockaddr*)(&ctrlAddr),&addrLen);
    if(rcvLen > 0) {
#ifdef __DEBUG__
    print0x(tmpData,rcvLen);
#endif
        if(tmpData[0]==0x23&&tmpData[1]==0x22)
        {
            printf("write 2322 ready\n");
            write(dev232Fd,tmpData,rcvLen);
            printf("write 2322 ok\n");
        }
    }
    else {
        COMMON_PRT("net not rcv 2322data\n");
        return false;
    }
    return true;
}

bool rs2322::readUart2ctrl()
{
    char tmpData[128] = {0};
    int rdLen = 0, sendLen = 0;
    memset(tmpData,0,sizeof(tmpData));

    tmpData[0]=0x23;
    tmpData[1]=0x22;
    rdLen = read(dev232Fd,&tmpData[2],MAXDATA);
#ifdef __DEBUG__
    //print0x(tmpData,rdLen+2);
#endif
    if(rdLen > 0) {
        if(global_data.tcp_en) {
            tcp_sv->tcp_send(tmpData, rdLen);
        }
        if((global_data.ctl2322port != 0) && (strlen(global_data.ctlip) >= 7)) {
            ctrlAddr_A258_232.sin_family = AF_INET;
            ctrlAddr_A258_232.sin_port = htons(global_data.ctl2322port);
            ctrlAddr_A258_232.sin_addr.s_addr = inet_addr(global_data.ctlip);
            sendLen = sendto(socket232,tmpData,rdLen+2,0,(struct sockaddr*)&ctrlAddr_A258_232,sizeof(struct sockaddr_in));
            if(sendLen > 0)
                return true;
            else {
                COMMON_PRT("send dev 2322 data to ctrl error\n");
                return false;
            }
        }
        else {
            COMMON_PRT("UMP not set ctrl IP PORT\n");
            return true;
        }
    }
    else {
        COMMON_PRT("RS2322 dev not read data\n");
        return false;
    }
    return true;
}

bool rs2322::UMPsetParam(bool uartFlag,bool ctrlPortFlag,bool svPortFlag)
{
    if(uartFlag) {
        COMMON_PRT("UMP set RS2322 UART Param\n");
        FD_CLR(dev232Fd,&readFd);
        close(dev232Fd);
        sleep(1);

        open_port(global_data.port2322,&dev232Fd);
        set_optBak(global_data.uartspeed2322,global_data.nbit2322,global_data.nstop2322,   \
                   global_data.event2322[0],global_data.nVtime2322,global_data.nVmin2322,&dev232Fd);
        FD_SET(dev232Fd,&readFd);
        if(dev232Fd >= maxFd)
            maxFd = dev232Fd + 1;
    }

    if(ctrlPortFlag) {
        COMMON_PRT("UMP set RS2322 ctrl port Param\n");
    }

    if(svPortFlag) {//要注掉吗
        FD_CLR(socket232,&readFd);
        close(socket232);
        sleep(1);

        socket232 = createUdp(global_data.sv232port);
        FD_SET(socket232,&readFd);
        if(socket232 >= maxFd)
            maxFd = socket232 + 1;
    }
    return true;
}

//2323

rs2323::rs2323()
{

}

rs2323::~rs2323()
{
    delete tcp_sv;
    FD_CLR(dev232Fd,&readFd);
    //FD_CLR(socket232,&readFd);
    close(dev232Fd);
    //close(socket232);
    dev232Fd = -1;
    //socket232 = -1;
}

void* rs2323::pth_tcp_server(void *arg)
{
    rs2323 * p_this = (rs2323*)arg;
    p_this->tcp_sv->tcp_running(p_this->dev232Fd);

    return (void*)0;
}

bool rs2323::iniRs232()
{
    printf("-----ini 2323-----\n");
    open_port(global_data.port2323,&dev232Fd);
    set_optBak(global_data.uartspeed2323,global_data.nbit2323,global_data.nstop2323,   \
               global_data.event2323[0],global_data.nVtime2323,global_data.nVmin2323,&dev232Fd);   //配置串口参数

    FD_SET(dev232Fd,&readFd);
    if(dev232Fd >= maxFd)
        maxFd = dev232Fd + 1;
    if(-1==socket232){
        socket232 = createUdp(global_data.sv232port);
        FD_SET(socket232,&readFd);
        if(socket232 >= maxFd)
            maxFd = socket232 + 1;
    }

//    if(tcp_pid == 0)
        tcp_sv = new tcp_server();

//    pthread_t tcp_pid = 0;
    if(global_data.tcp_en) {
        if(tcp_pid == 0){
        tcp_pid = CreateThread(pth_tcp_server, 99, SCHED_FIFO, true, this);
        }
        if(tcp_pid == 0 ) {
            perror("create thread pth_tcp_server error");
        }
    }

    return true;
}

bool rs2323::rcvData2uart()
{

    char tmpData[128] = {0};
    int rcvLen = 0;
    socklen_t addrLen = 0;
    memset(tmpData,0,sizeof(tmpData));

    addrLen = sizeof(struct sockaddr_in);

    rcvLen = recvfrom(socket232,tmpData,MAXDATA,0,(struct sockaddr*)(&ctrlAddr),&addrLen);
    if(rcvLen > 0) {
#ifdef __DEBUG__
    print0x(tmpData,rcvLen);
#endif
        if(tmpData[0]==0x23&&tmpData[1]==0x23)
        {
            printf("write 2323 ready\n");
            write(dev232Fd,tmpData,rcvLen);
            printf("write 2323 ok\n");
        }
    }
    else {
        COMMON_PRT("net not rcv 2323data\n");
        return false;
    }
    return true;
}

bool rs2323::readUart2ctrl()
{
    char tmpData[128] = {0};
    int rdLen = 0, sendLen = 0;
    memset(tmpData,0,sizeof(tmpData));

    tmpData[0]=0x23;
    tmpData[1]=0x23;
    rdLen = read(dev232Fd,&tmpData[2],MAXDATA);
#ifdef __DEBUG__
    //print0x(tmpData,rdLen+2);
#endif
    if(rdLen > 0) {
        if(global_data.tcp_en) {
            tcp_sv->tcp_send(tmpData, rdLen);
        }
        if((global_data.ctl2323port != 0) && (strlen(global_data.ctlip) >= 7)) {
            ctrlAddr_A258_232.sin_family = AF_INET;
            ctrlAddr_A258_232.sin_port = htons(global_data.ctl2323port);
            ctrlAddr_A258_232.sin_addr.s_addr = inet_addr(global_data.ctlip);
            sendLen = sendto(socket232,tmpData,rdLen+2,0,(struct sockaddr*)&ctrlAddr_A258_232,sizeof(struct sockaddr_in));
            if(sendLen > 0)
                return true;
            else {
                COMMON_PRT("send dev 2323 data to ctrl error\n");
                return false;
            }
        }
        else {
            COMMON_PRT("UMP not set ctrl IP PORT\n");
            return true;
        }
    }
    else {
        COMMON_PRT("RS2323 dev not read data\n");
        return false;
    }
    return true;
}

bool rs2323::UMPsetParam(bool uartFlag,bool ctrlPortFlag,bool svPortFlag)
{
    if(uartFlag) {
        COMMON_PRT("UMP set RS2323 UART Param\n");
        FD_CLR(dev232Fd,&readFd);
        close(dev232Fd);
        sleep(1);

        open_port(global_data.port2323,&dev232Fd);
        set_optBak(global_data.uartspeed2323,global_data.nbit2323,global_data.nstop2323,   \
                   global_data.event2323[0],global_data.nVtime2323,global_data.nVmin2323,&dev232Fd);
        FD_SET(dev232Fd,&readFd);
        if(dev232Fd >= maxFd)
            maxFd = dev232Fd + 1;
    }

    if(ctrlPortFlag) {
        COMMON_PRT("UMP set RS2323 ctrl port Param\n");
    }

    if(svPortFlag) {//要注掉吗
        FD_CLR(socket232,&readFd);
        close(socket232);
        sleep(1);

        socket232 = createUdp(global_data.sv232port);
        FD_SET(socket232,&readFd);
        if(socket232 >= maxFd)
            maxFd = socket232 + 1;
    }
    return true;
}

