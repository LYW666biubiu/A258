#ifndef IO_EDC_H
#define IO_EDC_H
#include "global.h"
#include "uart.h"
#include "hi_mpp.h"
#include "hi_reg.h"

#define GPIO22_BASE_REG 0x120F0290
#define GPIO22_BASE     0x122B0000
#define IO_NUM          6
class io_edc
{
public:
    io_edc();
    ~io_edc();
    bool ioIni_edc();
    bool rcvData2IO_edc();
    bool IOSet_edc(int set,int ioNum);
public:
    int socketIO = -1;
private:
    struct sockaddr_in ctrlAddr;
};

#endif // IO_EDC_H
