#ifndef RS485_H
#define RS485_H
#include "global.h"
#include "uart.h"
#include "hi_mpp.h"
#include "hi_reg.h"

//GPIO4_5作为485芯片方向控制GPIO，拉低为接收485数据，拉高发送485数据
#define GPIO4_5SET  0x20
#define GPIO4_5REG  0x120F0088       //GPIO4_5 复用寄存器0 - VI3_DAT5   1 - GPIO4_5
#define GPIO4_5DIR  0x12190400      //输出 0x20   输入0x00
#define GPIO4_5DATA 0x12190080

//GPIO7_3作为3536 485芯片方向控制GPIO，拉低为接收485数据，拉高发送485数据
#define GPIO7_3REG  0x120F013C          //0-GPIO
#define GPIO7_3SET  0x8
#define GPIO7_3DIR  0x121C0400
#define GPIO7_3DATA 0x121C0020


#define V200_GPIO3_0REG     0x102F0040      //3:0  0x0 GPIO3_0
#define V200_GPIO3_0DIR     0x11093400
#define V200_GPIO3_0DATA    0x11093004
#define V200_GPIO3_0SET     0x1

class rs485
{
public:
    rs485();
    ~rs485();
    bool iniRs485();
    bool rcvData2uart();
    bool readUart2ctrl();
    bool UMPsetParam(bool uartFlag,bool ctrlPortFlag,bool svPortFlag);

public:
    int dev485Fd = -1;
    int socket485 = -1;

private:
    struct sockaddr_in ctrlAddr;
};


class rs4850
{
public:
    rs4850();
    ~rs4850();
    bool iniRs485();
    bool rcvData2uart();
    bool readUart2ctrl();
    bool UMPsetParam(bool uartFlag,bool ctrlPortFlag,bool svPortFlag);

public:
    int dev485Fd = -1;
    //int socket485 = -1;

private:
    struct sockaddr_in ctrlAddr;
};


class rs4851
{
public:
    rs4851();
    ~rs4851();
    bool iniRs485();
    bool rcvData2uart();
    bool readUart2ctrl();
    bool UMPsetParam(bool uartFlag,bool ctrlPortFlag,bool svPortFlag);

public:
    int dev485Fd = -1;
    //int socket485 = -1;

private:
    struct sockaddr_in ctrlAddr;
};


class rs4852
{
public:
    rs4852();
    ~rs4852();
    bool iniRs485();
    bool rcvData2uart();
    bool readUart2ctrl();
    bool UMPsetParam(bool uartFlag,bool ctrlPortFlag,bool svPortFlag);

public:
    int dev485Fd = -1;
    //int socket485 = -1;

private:
    struct sockaddr_in ctrlAddr;
};


class rs4853
{
public:
    rs4853();
    ~rs4853();
    bool iniRs485();
    bool rcvData2uart();
    bool readUart2ctrl();
    bool UMPsetParam(bool uartFlag,bool ctrlPortFlag,bool svPortFlag);

public:
    int dev485Fd = -1;
    //int socket485 = -1;

private:
    struct sockaddr_in ctrlAddr;
};

#endif // RS485_H
