#ifndef RELAY_H
#define RELAY_H
#include "global.h"
#include "uart.h"
#include "hi_mpp.h"
#include "hi_reg.h"

#define RELAY_DIR   0x12190400      //输出0x40   输入0x00
#define RELAY_DATA  0x12190100
#define RELAY_REG   0x120F008C      //GPIO4_6 复用寄存器0 - VI3_DAT6   1 - GPIO4_6
#define RELAY_SET   0x40


#define RELAY_REG_EDC   0x120F02D0  //GPIO23_0  0x0
#define RELAY_DATA_EDC  0x122C0004
#define RELAY_DIR_EDC   0x122C0400
#define RELAY_SET_EDC   0x1

#define RELAY_36_REG    0x120F0128  //0 gpio
#define RELAY_36_DIR    0x121B0400
#define RELAY_36_DATA   0x121B0100
#define RELAY_36_SET    0x40

#define RELAY_0_REG     0x102F0018    //3:0 0x0 GPIO0_6
#define RELAY_0_DIR     0x11090400
#define RELAY_0_DATA    0x11090100
#define RELAY_0_SET     0x40

#define RELAY_1_REG     0x102F001C  //3:0 0x0 GPIO0_7
#define RELAY_1_DIR     0x11090400
#define RELAY_1_DATA    0x11090200
#define RELAY_1_SET     0x80

//添加A258中控继电器寄存器地址
#define RELAY_A258_1_REG     0x200F00AC  //0:0  0 GPIO0_5  1 RMII_RXD0
#define RELAY_A258_1_DIR     0x20150400
#define RELAY_A258_1_DATA    0x20150080
#define RELAY_A258_1_SET     0x20

#define RELAY_A258_0_REG     0x200F00B0  //0:0  0 GPIO0_6   1 RMII_RXD1
#define RELAY_A258_0_DIR     0x20150400
#define RELAY_A258_0_DATA    0x20150100
#define RELAY_A258_0_SET     0x40



#define  ON 1
#define OFF 0


class relay
{
public:
    relay();
    ~relay();
    bool relayIni();
    bool relaySet(int set, int relay_num = 0);
    bool rcvData2Relay();
    bool UMPsetParam(bool uartFlag,bool ctrlPortFlag,bool svPortFlag);
public:
//    int socketRelay = -1;
    int relayStr = -1;
    int relay0Str = -1;
    int relay1Str = -1;

private:
    struct sockaddr_in ctrlAddr;
};

#endif // RELAY_H
