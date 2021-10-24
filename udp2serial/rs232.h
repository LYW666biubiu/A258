//RS232通常启用UART1
#ifndef RS232_H
#define RS232_H
#include "global.h"
#include "uart.h"
#include "tcp_server.h"


class rs232
{
public:
    rs232();
    ~rs232();
    bool iniRs232();
    bool rcvData2uart();
    bool readUart2ctrl();
    bool rcvData2uart_edc();
    bool readUart2ctrl_edc();
    bool UMPsetParam(bool uartFlag,bool ctrlPortFlag,bool svPortFlag);

public:
    int dev232Fd = -1;
    int dev232Fd_edc = -1;
    int socket232 = -1;
    int socket232_edc = -1;


private:
    struct sockaddr_in ctrlAddr;
    tcp_server *tcp_sv;
    static void* pth_tcp_server(void *arg);


};

class rs2320
{
public:
    rs2320();
    ~rs2320();
    bool iniRs232();
    bool rcvData2uart();
    bool readUart2ctrl();
    bool rcvData2uart_edc();
    bool readUart2ctrl_edc();
    bool UMPsetParam(bool uartFlag,bool ctrlPortFlag,bool svPortFlag);

public:
    int dev232Fd = -1;
    int dev232Fd_edc = -1;
//    int socket232 = -1;
    int socket232_edc = -1;


private:
    struct sockaddr_in ctrlAddr;
    tcp_server *tcp_sv;
    static void* pth_tcp_server(void *arg);


};

class rs2321
{
public:
    rs2321();
    ~rs2321();
    bool iniRs232();
    bool rcvData2uart();
    bool readUart2ctrl();
    bool rcvData2uart_edc();
    bool readUart2ctrl_edc();
    bool UMPsetParam(bool uartFlag,bool ctrlPortFlag,bool svPortFlag);

public:
    int dev232Fd = -1;
    int dev232Fd_edc = -1;
//    int socket232 = -1;
    int socket232_edc = -1;


private:
    struct sockaddr_in ctrlAddr;
    tcp_server *tcp_sv;
    static void* pth_tcp_server(void *arg);


};

class rs2322
{
public:
    rs2322();
    ~rs2322();
    bool iniRs232();
    bool rcvData2uart();
    bool readUart2ctrl();
    bool rcvData2uart_edc();
    bool readUart2ctrl_edc();
    bool UMPsetParam(bool uartFlag,bool ctrlPortFlag,bool svPortFlag);

public:
    int dev232Fd = -1;
    int dev232Fd_edc = -1;
//    int socket232 = -1;
    int socket232_edc = -1;


private:
    struct sockaddr_in ctrlAddr;
    tcp_server *tcp_sv;
    static void* pth_tcp_server(void *arg);


};

class rs2323
{
public:
    rs2323();
    ~rs2323();
    bool iniRs232();
    bool rcvData2uart();
    bool readUart2ctrl();
    bool rcvData2uart_edc();
    bool readUart2ctrl_edc();
    bool UMPsetParam(bool uartFlag,bool ctrlPortFlag,bool svPortFlag);

public:
    int dev232Fd = -1;
    int dev232Fd_edc = -1;
//    int socket232 = -1;
    int socket232_edc = -1;


private:
    struct sockaddr_in ctrlAddr;
    tcp_server *tcp_sv;
    static void* pth_tcp_server(void *arg);


};

#endif // RS232_H
