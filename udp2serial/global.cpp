#include "global.h"

CONFIGDATA global_data;

char gClientIP[SZ_IPADDR] = "";
unsigned int  gClientPort = 0;
int     gMCtrllProtocol = kSCODe; //媒体控制协议

fd_set readFd;
int maxFd = 0;
bool en232 = false;
bool en485 = false;
bool en2320 = false;
bool en4850 = false;
bool en2321 = false;
bool en4851 = false;
bool en2322 = false;
bool en4852 = false;
bool en2323 = false;
bool en4853 = false;
bool enRelay = false;
bool enRelay0 = false;
bool enRelay1 = false;
bool enIr = false;
bool enIr_tc = false;
bool enIo0 = false;
bool enIo1 = false;
bool enIo2 = false;
bool enIo3 = false;
bool enIo4 = false;
bool enIo5 = false;
bool enIo6 = false;
bool enIo7 = false;

bool irTime = false;
bool en_3536 = false;
string local_ip;
VERSION hd_ver;
