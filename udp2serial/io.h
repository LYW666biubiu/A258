#ifndef IO_H
#define IO_H
#include "global.h"
#include "uart.h"
#include "hi_mpp.h"
#include "hi_reg.h"

#define GPIO1_5MUX      0x120F0018  //1-GPIO
#define GPIO1_5DIR      0x12160400
#define GPIO1_5DATA     0x12160080
#define GPIO1_5SET      0x20

#define GPIO20_5MUX     0x120F0270  //0-GPIO
#define GPIO20_5DIR     0x12290400
#define GPIO20_5DATA    0x12290080
#define GPIO20_5SET     0x20

#define GPIO1_6MUX      0x120F001C  //1-GPIO
#define GPIO1_6DIR      0x12160400
#define GPIO1_6DATA     0x12160100
#define GPIO1_6SET      0x40

#define GPIO1_7MUX      0x120F0020  //1-GPIO
#define GPIO1_7DIR      0x12160400
#define GPIO1_7DATA     0x12160200
#define GPIO1_7SET      0x80

#define GPIO15_6MUX     0x120F01C8  //1-GPIO
#define GPIO15_6DIR     0x12240400
#define GPIO15_6DATA    0x12240100
#define GPIO15_6SET     0x40

#ifdef __7000__
#define GPIO0_6DIR      0x12150400
#define GPIO0_6DATA     0x12150100
#define GPIO0_6SET      0x40

#define GPIO13_5MUX     0x120F0178
#define GPIO13_5DIR     0x12220400
#define GPIO13_5DATA    0x12220080
#define GPIO13_5SET     0x20

#define GPIO0_7DIR      0x12150400
#define GPIO0_7DATA     0x12150200
#define GPIO0_7SET      0x80
#endif


#define GPIO0_3DIR      0x12150400      //IO_1 or IR
#define GPIO0_3DATA     0x12150020
#define GPIO0_3SET      0x8

#define GPIO0_4DIR      0x12150400      //DIR_1
#define GPIO0_4DATA     0x12150040
#define GPIO0_4SET      0x10

#define GPIO0_6DIR      0x12150400      //IO_2
#define GPIO0_6DATA     0x12150100
#define GPIO0_6SET      0x40

#define GPIO13_5MUX     0x120F0178      //DIR_2
#define GPIO13_5DIR     0x12220400
#define GPIO13_5DATA    0x12220080
#define GPIO13_5SET     0x20


#define GPIO0_5DIR      0x12150400      //io_1 or ir
#define GPIO0_5DATA     0x12150080
#define GPIO0_5SET      0x20

#define GPIO0_3DIR      0x12150400
#define GPIO0_3DATA     0x12150020
#define GPIO0_3SET      0x8


//3536
#define GPIO11_2REG     0x120F0160        //gpio 1
#define GPIO11_2DIR     0x12200400
#define GPIO11_2DATA    0x12200010
#define GPIO11_2SET     0x4

#define GPIO11_1REG     0x120F015C      //gpio 1
#define GPIO11_1DIR     0x12200400
#define GPIO11_1DATA    0x12200008
#define GPIO11_1SET     0x2

#define GPIO11_3REG     0x120F009C      //gpio 0
#define GPIO11_3DIR     0x12200400
#define GPIO11_3DATA    0x12200020
#define GPIO11_3SET     0x8

#define GPIO14_7DIR     0x12230400
#define GPIO14_7DATA    0x12230200
#define GPIO14_7SET     0x80

//南京31DV200
#define IO_0_DIR_REG    0x102F0004      //3:0 0x0 GPIO0_1
#define IO_0_DIR_DIR    0x11090400
#define IO_0_DIR_DATA   0x11090008
#define IO_0_DIR_SET    0x2
#define IO_0_DATA_REG   0x102F0000      //3:0 0x0 GPIO0_0
#define IO_0_DATA_DIR   0x11090400
#define IO_0_DATA_DATA  0x11090004
#define IO_0_DATA_SET   0x1

#define IO_1_DIR_REG    0x102F000C      //3:0 0x0 GPIO0_3
#define IO_1_DIR_DIR    0x11090400
#define IO_1_DIR_DATA   0x11090020
#define IO_1_DIR_SET    0x8
#define IO_1_DATA_REG   0x102F0008      //3:0 0x0 GPIO0_2
#define IO_1_DATA_DIR   0x11090400
#define IO_1_DATA_DATA  0x11090010
#define IO_1_DATA_SET   0x4

#define IO_2_DIR_REG    0x102F0010      //3:0 0x0 GPIO0_4
#define IO_2_DIR_DIR    0x11090400
#define IO_2_DIR_DATA   0x11090040
#define IO_2_DIR_SET    0x10
#define IO_2_DATA_REG   0x102F0090      //3:0 0x0 GPIO5_4
#define IO_2_DATA_DIR   0x11095400
#define IO_2_DATA_DATA  0x11095040
#define IO_2_DATA_SET   0x10

#define IO_3_DIR_REG    0x102F0014      //3:0 0x0 GPIO0_5
#define IO_3_DIR_DIR    0x11090400
#define IO_3_DIR_DATA   0x11090080
#define IO_3_DIR_SET    0x20
#define IO_3_DATA_REG   0x10FF0070      //3:0 0x1 GPIO1_4
#define IO_3_DATA_DIR   0x11091400
#define IO_3_DATA_DATA  0x11091040
#define IO_3_DATA_SET   0x10

//A258   添加中控IO寄存器
#define A258_GPIO0_BASE 0x20150000
#define A258_GPIO6_BASE 0x201B0000
#define A258_mux_bace   0x200F0000        //管脚复用基地址

#define A258_gpio0_0_mux (A258_mux_bace+0x098)   //1:RMII_CLK      IO4
#define A258_gpio0_1_mux (A258_mux_bace+0x09C)   //1:RMII_TX_EN    IO7
#define A258_gpio0_2_mux (A258_mux_bace+0x0A0)   //1:RMII_TXD0     IO8
#define A258_gpio0_3_mux (A258_mux_bace+0x0A4)   //1:RMII_TXD1     IO3
#define A258_gpio0_4_mux (A258_mux_bace+0x0A8)   //1:RMII_CRS_DV   IO2
//#define A258_gpio0_5_mux (A258_mux_bace+0x0AC)   //1:RMII_RXD0    继电器1
//#define A258_gpio0_6_mux (A258_mux_bace+0x0B0)   //1:RMII_RXD1    继电器2
#define A258_gpio0_7_mux (A258_mux_bace+0x0B4)   //1:RMII_RX_ER    IO6
#define A258_gpio6_2_mux (A258_mux_bace+0x070)   //1:USB1_OVRCUR   IO5
#define A258_gpio6_3_mux (A258_mux_bace+0x074)   //1:USB1_PWREN    IO1


#define A258_IO0_REG  A258_gpio6_3_mux           //0:0   0  GPIO6_3
#define A258_IO0_DATA (A258_GPIO6_BASE+0x20)
#define A258_IO0_DIR  (A258_GPIO6_BASE+0x400)
#define A258_IO0_SET    0x8

#define A258_IO1_REG  A258_gpio0_4_mux
#define A258_IO1_DATA (A258_GPIO0_BASE+0x40)
#define A258_IO1_DIR  (A258_GPIO0_BASE+0x400)
#define A258_IO1_SET    0x10

#define A258_IO2_REG  A258_gpio0_3_mux
#define A258_IO2_DATA (A258_GPIO0_BASE+0x20)
#define A258_IO2_DIR  (A258_GPIO0_BASE+0x400)
#define A258_IO2_SET    0x8

#define A258_IO3_REG  A258_gpio0_0_mux
#define A258_IO3_DATA (A258_GPIO0_BASE+0x4)
#define A258_IO3_DIR  (A258_GPIO0_BASE+0x400)
#define A258_IO3_SET    0x1

#define A258_IO4_REG  A258_gpio6_2_mux
#define A258_IO4_DATA (A258_GPIO6_BASE+0x10)
#define A258_IO4_DIR  (A258_GPIO6_BASE+0x400)
#define A258_IO4_SET    0x4

#define A258_IO5_REG  A258_gpio0_7_mux
#define A258_IO5_DATA (A258_GPIO0_BASE+0x200)
#define A258_IO5_DIR  (A258_GPIO0_BASE+0x400)
#define A258_IO5_SET    0x80

#define A258_IO6_REG  A258_gpio0_1_mux
#define A258_IO6_DATA (A258_GPIO0_BASE+0x8)
#define A258_IO6_DIR  (A258_GPIO0_BASE+0x400)
#define A258_IO6_SET    0x2

#define A258_IO7_REG  A258_gpio0_2_mux
#define A258_IO7_DATA (A258_GPIO0_BASE+0x10)
#define A258_IO7_DIR  (A258_GPIO0_BASE+0x400)
#define A258_IO7_SET    0x4

//21A
#define IO_DIR_11_6_MUX     0x120F0098              //0x0 GPIO11_6
#define IO_DIR_11_6_DIR     0x12200400
#define IO_DIR_11_6_DATA    0x12200100
#define IO_DIR_11_6_SET     0x40

#define IO_DIR_11_3_MUX     0x120F009C              //0x0 GPIO11_3
#define IO_DIR_11_3_DIR     0x12200400
#define IO_DIR_11_3_DATA    0x12200020
#define IO_DIR_11_3_SET     0x8

#define IO_1_5_6_MUX        0x120F00DC             //0x1 GPIO5_6
#define IO_1_5_6_DIR        0x121A0400
#define IO_1_5_6_DATA       0x121A0100
#define IO_1_5_6_SET        0x40

#define IO_0_10_2_MUX       0x120F0154              //0x0 GPIO10_2
#define IO_0_10_2_DIR       0x121F0400
#define IO_0_10_2_DATA      0x121F0010
#define IO_0_10_2SET        0X4
class io
{
public:
    io();
    ~io();
    bool ioIni(int ioNum);
//    bool rcvData2IO();
    bool IOSet(int set,int ioNum);
    bool UMPsetParam(bool uartFlag,bool ctrlPortFlag,int svPortFlag);


public:
//    int socketIO = -1;
    int io0Str = -1;
    int io1Str = -1;
    int io2Str = -1;
    int io3Str = -1;
    int io4Str = -1;
    int io5Str = -1;
    int io6Str = -1;
    int io7Str = -1;

private:
    //struct sockaddr_in ctrlAddr;
};

#endif // IO_H
