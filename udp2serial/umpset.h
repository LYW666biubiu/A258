#ifndef UMPSET_H
#define UMPSET_H

#include "global.h"
#include "uart.h"


#define REPLY   0
#define DEL     1
#define PARAM   2
#define SAME    -1

class umpSet
{
public:
    umpSet();
    ~umpSet();
    bool umpIni();
    int rcvData();
    bool getUmpData(CONFIGDATA *tmp,char *data);
    bool relyUMP();

public:
    int socketUmp = -1;
    CONFIGDATA umpData;
private:
    struct sockaddr_in umpAddr;
    bool mlt_echo = false;
};

#endif // UMPSET_H
