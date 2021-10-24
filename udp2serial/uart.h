#ifndef UART_H
#define UART_H
#include <fcntl.h>
#include "global.h"



int open_port(char *comport,int *pdevfd); //通过参数，打开相应的串口


int set_optBak(int nSpeed, int nBits, int nStop, char nEvent, int nVtime, int nVmin,int *setfd);  //串口设置


string GetLocalIp();
int createUdp(const uint k_serverport);
bool AddMemberShip(int socket_, const char *kp_sendtoip, const char *kp_ethname);
bool print0x(char *data, int len);
bool printdata(char *data, int len);
int timeRead(char *data_,int *fd_,int time_s);


#endif // UART_H
