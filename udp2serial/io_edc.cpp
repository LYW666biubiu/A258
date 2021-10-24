#include "io_edc.h"


io_edc::io_edc()
{

}

io_edc::~io_edc()
{
    close(socketIO);
}


bool io_edc::ioIni_edc()
{
    uint value = 0,i = 0;
    for(i = 0; i < IO_NUM; i++) {
        set_reg(GPIO22_BASE_REG + 0x4 * i, 0x0);
    }
    get_reg(GPIO22_BASE+0x400);
    value |= 0x3F;
    set_reg(GPIO22_BASE+0x400,value);

    socketIO = createUdp(global_data.svIOport);
    FD_SET(socketIO,&readFd);
    if(socketIO >= maxFd)
        maxFd = socketIO + 1;
}

bool io_edc::rcvData2IO_edc()
{
    char tmpData[128] = {0};
    int io_num = 0;
    int io_set = 0;
    int rcvLen = 0;
    socklen_t addrLen = 0;
    memset(tmpData,0,sizeof(tmpData));

    addrLen = sizeof(struct sockaddr_in);
    rcvLen = recvfrom(socketIO,tmpData,MAXDATA,0,(struct sockaddr*)(&ctrlAddr),&addrLen);
    if(rcvLen > 0) {
        printf("rcv io data:%s\n",tmpData);
        if(!strncmp(tmpData,"IO",strlen("IO"))) {
            io_num = tmpData[3] - 48;
            io_set = tmpData[5] - 48;
            IOSet_edc(io_set,io_num);
        }
        else {
            printf("error data:%s\n",tmpData);
        }
    }
    return true;
}


bool io_edc::IOSet_edc(int set,int ioNum)
{
    uint value;
    if(set == 1) {
        get_reg(GPIO22_BASE+(0x4<<ioNum));
        value |= (0x1<<ioNum);
        set_reg(GPIO22_BASE+(0x4<<ioNum),value);
    }
    else if(0 == set){
        get_reg(GPIO22_BASE+(0x4<<ioNum));
        value &= ~(0x1<<ioNum);
        set_reg(GPIO22_BASE+(0x4<<ioNum),value);
    }

    return true;

}
