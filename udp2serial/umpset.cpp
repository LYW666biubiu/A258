#include "umpset.h"

umpSet::umpSet()
{

}

umpSet::~umpSet()
{
    FD_CLR(socketUmp,&readFd);
    close(socketUmp);
    socketUmp = -1;
}


bool umpSet::umpIni()
{
    socketUmp = createUdp(UMPPORT);

    AddMemberShip(socketUmp, "224.168.1.1", ETH0);
    FD_SET(socketUmp,&readFd);
    if(socketUmp >= maxFd) {
        maxFd = socketUmp + 1;
    }
    return true;
}

int umpSet::rcvData()
{
    char tmpData[512] = {0};
    int rcvLen = 0;
    char getIp[20];
    socklen_t addrLen = sizeof(struct sockaddr_in);

    memcpy(&umpData,&global_data,sizeof(CONFIGDATA));
    rcvLen = recvfrom(socketUmp,tmpData,512,NULL,(struct sockaddr*)(&umpAddr),&addrLen);
    if(rcvLen > 0) {
        printf("rcv data:%s\n", tmpData);
        if(!strncmp(tmpData,"reply",strlen("reply"))) {
            if(rcvLen > strlen("reply:")) {
                memset(getIp, 0, sizeof(getIp));
                sscanf(tmpData,"reply:%s",getIp);
                if(!strcmp(getIp, local_ip.c_str())) {
                    mlt_echo = true;
                    return REPLY;
                }
            }
            mlt_echo = false;
            return REPLY;
        }
        else if(!strncmp(tmpData,"del",strlen("del"))) {
            return DEL;
        }
        else {
#ifdef __DEBUG__
            printf("rcv ump set data:\n%s\n",tmpData);
#endif
            if(getUmpData(&umpData,tmpData))
                return PARAM;
            else
                return SAME;
        }
    }
    else {
        COMMON_PRT("rcv ump data error\n");
        return true;
    }
}

bool getP(char **start,char **end,char *data,const char *key)
{
    *start = strstr(data,key);
    if(NULL == *start)
        return false;
    else {
        *end = strchr(*start,0);

        if(NULL == *end)
            return false;
        else {
            return true;
        }
    }
}

bool umpSet::getUmpData(CONFIGDATA *tmp,char *data)
{
    //printf("-----------------\n");
    char *start = NULL, *end = NULL;
    char value[8] = {0};

//设置控制端UDP回复地址和对应端口号
    if(getP(&start,&end,data,"kctlip")) {
        memset(tmp->ctlip,0,32);
        start += (strlen("kctlip")+1);
        strncpy(tmp->ctlip,start,end-start);
        if(strncmp(tmp->ctlip,global_data.ctlip,strlen(global_data.ctlip))) {
            return true;
        }
        else {
            return false;
        }
    }


//    if(getP(&start,&end,data,"kctl232port")) {
//        tmp->ctl232port = 0;
//        start += (strlen("kctl232port")+1);
//        memset(value,0,8);
//        strncpy(value,start,end-start);
//        tmp->ctl232port = atoi(value);
//        if(tmp->ctl232port != global_data.ctl232port) {
//            return true;
//        }
//        else
//            return false;
//    }

//    if(getP(&start,&end,data,"kctl485port")) {
//        tmp->ctl485port = 0;
//        start += (strlen("kctl485port")+1);
//        memset(value,0,8);
//        strncpy(value,start,end-start);
//        tmp->ctl485port = atoi(value);
//        if(tmp->ctl485port != global_data.ctl485port) {
//            return true;
//        }
//        else
//            return false;
//    }

    //2320~3


    if(getP(&start,&end,data,"kctl2320port")) {
        tmp->ctl2320port = 0;
        start += (strlen("kctl2320port")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->ctl2320port = atoi(value);
        if(tmp->ctl2320port != global_data.ctl2320port) {
            return true;
        }
        else
            return false;
    }



    if(getP(&start,&end,data,"kctl2321port")) {
        tmp->ctl2321port = 0;
        start += (strlen("kctl2321port")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->ctl2321port = atoi(value);
        if(tmp->ctl2321port != global_data.ctl2321port) {
            return true;
        }
        else
            return false;
    }



    if(getP(&start,&end,data,"kctl2322port")) {
        tmp->ctl2322port = 0;
        start += (strlen("kctl2322port")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->ctl2322port = atoi(value);
        if(tmp->ctl2322port != global_data.ctl2322port) {
            return true;
        }
        else
            return false;
    }


    if(getP(&start,&end,data,"kctl2323port")) {
        tmp->ctl2323port = 0;
        start += (strlen("kctl2323port")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->ctl2323port = atoi(value);
        if(tmp->ctl2323port != global_data.ctl2323port) {
            return true;
        }
        else
            return false;
    }

        //4850~3
    if(getP(&start,&end,data,"kctl4850port")) {
        tmp->ctl4850port = 0;
        start += (strlen("kctl4850port")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->ctl4850port = atoi(value);
        if(tmp->ctl4850port != global_data.ctl4850port) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"kctl4851port")) {
        tmp->ctl4851port = 0;
        start += (strlen("kctl4851port")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->ctl4851port = atoi(value);
        if(tmp->ctl4851port != global_data.ctl4851port) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"kctl4852port")) {
        tmp->ctl4852port = 0;
        start += (strlen("kctl4852port")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->ctl4852port = atoi(value);
        if(tmp->ctl4852port != global_data.ctl4852port) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"kctl4853port")) {
        tmp->ctl4853port = 0;
        start += (strlen("kctl4853port")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->ctl4853port = atoi(value);
        if(tmp->ctl4853port != global_data.ctl4853port) {
            return true;
        }
        else
            return false;
    }


    if(getP(&start,&end,data,"kctlrelayport")) {
        tmp->ctlrelayport = 0;
        start += (strlen("kctlrelayport")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->ctlrelayport = atoi(value);
        if(tmp->ctlrelayport != global_data.ctlrelayport) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"kctlrelay0port")) {
        tmp->ctlrelay0port = 0;
        start += (strlen("kctlrelay0port")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->ctlrelay0port = atoi(value);
        if(tmp->ctlrelay0port != global_data.ctlrelay0port) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"kctlrelay1port")) {
        tmp->ctlrelay1port = 0;
        start += (strlen("kctlrelay1port")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->ctlrelay1port = atoi(value);
        if(tmp->ctlrelay1port != global_data.ctlrelay1port) {
            return true;
        }
        else
            return false;
    }


    if(getP(&start,&end,data,"kctlinfraredport")) {
        tmp->ctlinfraredport = 0;
        start += (strlen("kctlinfraredport")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->ctlinfraredport = atoi(value);
        if(tmp->ctlinfraredport != global_data.ctlinfraredport) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"kctlIO0port")) {
        tmp->ctlIO0port = 0;
        start += (strlen("kctlIO0port")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->ctlIO0port = atoi(value);
        if(tmp->ctlIO0port != global_data.ctlIO0port) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"kctlIO1port")) {
        tmp->ctlIO1port = 0;
        start += (strlen("kctlIO1port")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->ctlIO1port = atoi(value);
        if(tmp->ctlIO1port != global_data.ctlIO1port) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"kctlIO2port")) {
        tmp->ctlIO2port = 0;
        start += (strlen("kctlIO2port")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->ctlIO2port = atoi(value);
        if(tmp->ctlIO2port != global_data.ctlIO2port) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"kctlIO3port")) {
        tmp->ctlIO3port = 0;
        start += (strlen("kctlIO3port")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->ctlIO3port = atoi(value);
        if(tmp->ctlIO3port != global_data.ctlIO3port) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"kctlIO4port")) {
        tmp->ctlIO4port = 0;
        start += (strlen("kctlIO4port")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->ctlIO4port = atoi(value);
        if(tmp->ctlIO4port != global_data.ctlIO4port) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"kctlIO5port")) {
        tmp->ctlIO5port = 0;
        start += (strlen("kctlIO5port")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->ctlIO5port = atoi(value);
        if(tmp->ctlIO5port != global_data.ctlIO5port) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"kctlIO6port")) {
        tmp->ctlIO6port = 0;
        start += (strlen("kctlIO6port")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->ctlIO6port = atoi(value);
        if(tmp->ctlIO6port != global_data.ctlIO6port) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"kctlIO7port")) {
        tmp->ctlIO7port = 0;
        start += (strlen("kctlIO7port")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->ctlIO7port = atoi(value);
        if(tmp->ctlIO7port != global_data.ctlIO7port) {
            return true;
        }
        else
            return false;
    }

//设置本地接收UDP的对应端口号，一般不变
    if(getP(&start,&end,data,"ksv232port")) {
        tmp->sv232port = 0;
        start += (strlen("ksv232port")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->sv232port = atoi(value);
        if(tmp->sv232port != global_data.sv232port) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"ksv485port")) {
        tmp->sv485port = 0;
        start += (strlen("ksv485port")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->sv485port = atoi(value);
        if(tmp->sv485port != global_data.sv485port) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"ksvrelayport")) {
        tmp->svrelayport = 0;
        start += (strlen("ksvrelayport")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->svrelayport = atoi(value);
        if(tmp->svrelayport != global_data.svrelayport) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"ksvinfraredport")) {
        tmp->svInfraredport = 0;
        start += (strlen("ksvinfraredport")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->svInfraredport = atoi(value);
        if(tmp->svInfraredport != global_data.svInfraredport) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"ksvIOport")) {
        tmp->svIOport = 0;
        start += (strlen("ksvIOport")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->svIOport = atoi(value);
        if(tmp->svIOport != global_data.svIOport ) {
            return true;
        }
        else
            return false;
    }

//设置RS232对应的端口和参数

    if(getP(&start,&end,data,"ken2320")) {
        //printf("--------------------data:%s\n",data);
        tmp->en2320 = 0;
        start += (strlen("ken2320")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->en2320 = atoi(value);
        if(tmp->en2320 != global_data.en2320 ) {
            return true;
        }
        else
            return false;
    }
    if(getP(&start,&end,data,"ken2321")) {
        tmp->en2321 = 0;
        start += (strlen("ken2321")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->en2321 = atoi(value);
        if(tmp->en2321 != global_data.en2321 ) {
            return true;
        }
        else
            return false;
    }
    if(getP(&start,&end,data,"ken2322")) {
        tmp->en2322 = 0;
        start += (strlen("ken2322")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->en2322 = atoi(value);
        if(tmp->en2322 != global_data.en2322 ) {
            return true;
        }
        else
            return false;
    }
    if(getP(&start,&end,data,"ken2323")) {
        tmp->en2323 = 0;
        start += (strlen("ken2323")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->en2323 = atoi(value);
        if(tmp->en2323 != global_data.en2323 ) {
            return true;
        }
        else
            return false;
    }


    if(getP(&start,&end,data,"ken232")) {
        tmp->en232 = 0;
        start += (strlen("ken232")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->en232 = atoi(value);
        if(tmp->en232 != global_data.en232 ) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"k232port")) {
        memset(tmp->port232,0,8);
        start += (strlen("k232port")+1);
        strncpy(tmp->port232,start,end-start);
        if(strncmp(tmp->port232,global_data.port232,strlen(global_data.port232))) {
            return true;
        }
        else {
            return false;
        }
    }

    if(getP(&start,&end,data,"k232uartspeed")) {
        tmp->uartspeed232 = 0;
        start += (strlen("k232uartspeed")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->uartspeed232 = atol(value);
        if(tmp->uartspeed232 != global_data.uartspeed232 ) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"k232nbit")) {
        tmp->nbit232 = 0;
        start += (strlen("k232nbit")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->nbit232 = atoi(value);
        if(tmp->nbit232 != global_data.nbit232 ) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"k232nstop")) {
        tmp->nstop232 = 0;
        start += (strlen("k232nstop")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->nstop232 = atoi(value);
        if(tmp->nstop232 != global_data.nstop232 ) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"k232event")) {
        memset(tmp->event232,0,4);
        start += (strlen("k232event")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->event232[0] = value[0];
        if(tmp->event232[0] != global_data.event232[0] ) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"k232vtime")) {
        tmp->nVtime232 = 0;
        start += (strlen("k232vtime")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->nVtime232 = atoi(value);
        if(tmp->nVtime232 != global_data.nVtime232 ) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"k232vmin")) {
        tmp->nVmin232 = 0;
        start += (strlen("k232vmin")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->nVmin232 = atoi(value);
        if(tmp->nVmin232 != global_data.nVmin232 ) {
            return true;
        }
        else
            return false;
    }

//2320

    if(getP(&start,&end,data,"ken2320")) {
        //printf("--------------------data:%s\n",data);
        tmp->en2320 = 0;
        start += (strlen("ken2320")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->en2320 = atoi(value);
        if(tmp->en2320 != global_data.en2320 ) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"k2320port")) {
        memset(tmp->port2320,0,8);
        start += (strlen("k2320port")+1);
        strncpy(tmp->port2320,start,end-start);
        if(strncmp(tmp->port2320,global_data.port2320,strlen(global_data.port2320))) {
            return true;
        }
        else {
            return false;
        }
    }

    if(getP(&start,&end,data,"k2320uartspeed")) {
        tmp->uartspeed2320 = 0;
        start += (strlen("k2320uartspeed")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->uartspeed2320 = atol(value);
        if(tmp->uartspeed2320 != global_data.uartspeed2320 ) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"k2320nbit")) {
        tmp->nbit2320 = 0;
        start += (strlen("k2320nbit")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->nbit2320 = atoi(value);
        if(tmp->nbit2320 != global_data.nbit2320 ) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"k2320nstop")) {
        tmp->nstop2320 = 0;
        start += (strlen("k2320nstop")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->nstop2320 = atoi(value);
        if(tmp->nstop2320 != global_data.nstop2320 ) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"k2320event")) {
        memset(tmp->event2320,0,4);
        start += (strlen("k232event0")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->event2320[0] = value[0];
        if(tmp->event2320[0] != global_data.event2320[0] ) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"k2320vtime")) {
        tmp->nVtime2320 = 0;
        start += (strlen("k2320vtime")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->nVtime2320 = atoi(value);
        if(tmp->nVtime2320 != global_data.nVtime2320 ) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"k2320vmin")) {
        tmp->nVmin2320 = 0;
        start += (strlen("k2320vmin")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->nVmin2320 = atoi(value);
        if(tmp->nVmin2320 != global_data.nVmin2320 ) {
            return true;
        }
        else
            return false;
    }

    //2321

    if(getP(&start,&end,data,"ken2321")) {
        tmp->en2321 = 0;
        start += (strlen("ken2321")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->en2321 = atoi(value);
        if(tmp->en2321 != global_data.en2321 ) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"k2321port")) {
        memset(tmp->port2321,0,8);
        start += (strlen("k232port1")+1);
        strncpy(tmp->port2321,start,end-start);
        if(strncmp(tmp->port2321,global_data.port2321,strlen(global_data.port2321))) {
            return true;
        }
        else {
            return false;
        }
    }

    if(getP(&start,&end,data,"k2321uartspeed")) {
        tmp->uartspeed2321 = 0;
        start += (strlen("k2321uartspeed")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->uartspeed2321 = atol(value);
        if(tmp->uartspeed2321 != global_data.uartspeed2321 ) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"k2321nbit")) {
        tmp->nbit2321 = 0;
        start += (strlen("k2321nbit")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->nbit2321 = atoi(value);
        if(tmp->nbit2321 != global_data.nbit2321 ) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"k2321nstop")) {
        tmp->nstop2321 = 0;
        start += (strlen("k2321nstop")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->nstop2321 = atoi(value);
        if(tmp->nstop2321 != global_data.nstop2321 ) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"k2321event")) {
        memset(tmp->event2321,0,4);
        start += (strlen("k2321event")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->event2321[0] = value[0];
        if(tmp->event2321[0] != global_data.event2321[0] ) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"k2321vtime")) {
        tmp->nVtime2321 = 0;
        start += (strlen("k2321vtime")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->nVtime2321 = atoi(value);
        if(tmp->nVtime2321 != global_data.nVtime2321 ) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"k2321vmin")) {
        tmp->nVmin2321 = 0;
        start += (strlen("k2321vmin")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->nVmin2321 = atoi(value);
        if(tmp->nVmin2321 != global_data.nVmin2321 ) {
            return true;
        }
        else
            return false;
    }

//2322

    if(getP(&start,&end,data,"ken2322")) {
        tmp->en2322 = 0;
        start += (strlen("ken2322")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->en2322 = atoi(value);
        if(tmp->en2322 != global_data.en2322 ) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"k2322port")) {
        memset(tmp->port2322,0,8);
        start += (strlen("k2322port")+1);
        strncpy(tmp->port2322,start,end-start);
        if(strncmp(tmp->port2322,global_data.port2322,strlen(global_data.port2322))) {
            return true;
        }
        else {
            return false;
        }
    }

    if(getP(&start,&end,data,"k2322uartspeed")) {
        tmp->uartspeed2322 = 0;
        start += (strlen("k2322uartspeed")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->uartspeed2322 = atol(value);
        if(tmp->uartspeed2322 != global_data.uartspeed2322 ) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"k2322nbit")) {
        tmp->nbit2322 = 0;
        start += (strlen("k2322nbit")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->nbit2322 = atoi(value);
        if(tmp->nbit2322 != global_data.nbit2322 ) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"k2322nstop")) {
        tmp->nstop2322 = 0;
        start += (strlen("k2322nstop")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->nstop2322 = atoi(value);
        if(tmp->nstop2322 != global_data.nstop2322 ) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"k2322event")) {
        memset(tmp->event2322,0,4);
        start += (strlen("k2322event")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->event2322[0] = value[0];
        if(tmp->event2322[0] != global_data.event2322[0] ) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"k2322vtime")) {
        tmp->nVtime2322 = 0;
        start += (strlen("k2322vtime")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->nVtime2322 = atoi(value);
        if(tmp->nVtime2322 != global_data.nVtime2322 ) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"k2322vmin")) {
        tmp->nVmin2322 = 0;
        start += (strlen("k2322vmin")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->nVmin2322 = atoi(value);
        if(tmp->nVmin2322 != global_data.nVmin2322 ) {
            return true;
        }
        else
            return false;
    }

    //2323

    if(getP(&start,&end,data,"ken2323")) {
        tmp->en2323 = 0;
        start += (strlen("ken2323")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->en2323 = atoi(value);
        if(tmp->en2323 != global_data.en2323 ) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"k2323port")) {
        memset(tmp->port2323,0,8);
        start += (strlen("k2323port")+1);
        strncpy(tmp->port2323,start,end-start);
        if(strncmp(tmp->port2323,global_data.port2323,strlen(global_data.port2323))) {
            return true;
        }
        else {
            return false;
        }
    }

    if(getP(&start,&end,data,"k2323uartspeed")) {
        tmp->uartspeed2323 = 0;
        start += (strlen("k2323uartspeed")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->uartspeed2323 = atol(value);
        if(tmp->uartspeed2323 != global_data.uartspeed2323 ) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"k2323nbit")) {
        tmp->nbit2323 = 0;
        start += (strlen("k2323nbit")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->nbit2323 = atoi(value);
        if(tmp->nbit2323 != global_data.nbit2323 ) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"k2323nstop")) {
        tmp->nstop2323 = 0;
        start += (strlen("k2323nstop")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->nstop2323 = atoi(value);
        if(tmp->nstop2323 != global_data.nstop2323 ) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"k2323event")) {
        memset(tmp->event2323,0,4);
        start += (strlen("k2323event")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->event2323[0] = value[0];
        if(tmp->event2323[0] != global_data.event2323[0] ) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"k2323vtime")) {
        tmp->nVtime2323 = 0;
        start += (strlen("k2323vtime")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->nVtime2323 = atoi(value);
        if(tmp->nVtime2323 != global_data.nVtime2323 ) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"k2323vmin")) {
        tmp->nVmin2323 = 0;
        start += (strlen("k2323vmin")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->nVmin2323 = atoi(value);
        if(tmp->nVmin2323 != global_data.nVmin2323 ) {
            return true;
        }
        else
            return false;
    }

//设置RS485对应端口和参数
    if(getP(&start,&end,data,"ken4850")) {
        tmp->en4850 = 0;
        start += (strlen("ken4850")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->en4850 = atoi(value);
        if(tmp->en4850 != global_data.en4850 ) {
            return true;
        }
        else
            return false;
    }
    if(getP(&start,&end,data,"ken4851")) {
        tmp->en4851 = 0;
        start += (strlen("ken4851")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->en4851 = atoi(value);
        if(tmp->en4851 != global_data.en4851 ) {
            return true;
        }
        else
            return false;
    }
    if(getP(&start,&end,data,"ken4852")) {
        tmp->en4852 = 0;
        start += (strlen("ken4852")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->en4852 = atoi(value);
        if(tmp->en4852 != global_data.en4852 ) {
            return true;
        }
        else
            return false;
    }
    if(getP(&start,&end,data,"ken4853")) {
        tmp->en4853 = 0;
        start += (strlen("ken4853")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->en4853 = atoi(value);
        if(tmp->en4853 != global_data.en4853 ) {
            return true;
        }
        else
            return false;
    }


    if(getP(&start,&end,data,"ken485")) {
        tmp->en485 = 0;
        start += (strlen("ken485")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->en485 = atoi(value);
        if(tmp->en485 != global_data.en485 ) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"k485port")) {
        memset(tmp->port485,0,8);
        start += (strlen("k485port")+1);
        strncpy(tmp->port485,start,end-start);
        if(strncmp(tmp->port485,global_data.port485,strlen(global_data.port485))) {
            return true;
        }
        else {
            return false;
        }
    }

    if(getP(&start,&end,data,"k485uartspeed")) {
        tmp->uartspeed485 = 0;
        start += (strlen("k485uartspeed")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->uartspeed485 = atol(value);
        if(tmp->uartspeed485 != global_data.uartspeed485 ) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"k485nbit")) {
        tmp->nbit485 = 0;
        start += (strlen("k485nbit")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->nbit485 = atoi(value);
        if(tmp->nbit485 != global_data.nbit485 ) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"k485nstop")) {
        tmp->nstop485 = 0;
        start += (strlen("k485nstop")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->nstop485 = atoi(value);
        if(tmp->nstop485 != global_data.nstop485 ) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"k485event")) {
        memset(tmp->event485,0,4);
        start += (strlen("k485event")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->event485[0] = value[0];
        if(tmp->event485[0] != global_data.event485[0] ) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"k485vtime")) {
        tmp->nVtime485 = 0;
        start += (strlen("k485vtime")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->nVtime485 = atoi(value);
        if(tmp->nVtime485 != global_data.nVtime485 ) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"k485vmin")) {
        tmp->nVmin485 = 0;
        start += (strlen("k485vmin")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->nVmin485 = atoi(value);
        if(tmp->nVmin485 != global_data.nVmin485 ) {
            return true;
        }
        else
            return false;
    }

//4850

    if(getP(&start,&end,data,"ken4850")) {
        tmp->en4850 = 0;
        start += (strlen("ken4850")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->en4850 = atoi(value);
        if(tmp->en4850 != global_data.en4850 ) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"k4850port")) {
        memset(tmp->port4850,0,8);
        start += (strlen("k4850port")+1);
        strncpy(tmp->port4850,start,end-start);
        if(strncmp(tmp->port4850,global_data.port4850,strlen(global_data.port4850))) {
            return true;
        }
        else {
            return false;
        }
    }

    if(getP(&start,&end,data,"k4850uartspeed")) {
        tmp->uartspeed4850 = 0;
        start += (strlen("k4850uartspeed")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->uartspeed4850 = atol(value);
        if(tmp->uartspeed4850 != global_data.uartspeed4850 ) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"k4850nbit")) {
        tmp->nbit4850 = 0;
        start += (strlen("k4850nbit")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->nbit4850 = atoi(value);
        if(tmp->nbit4850 != global_data.nbit4850 ) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"k4850nstop")) {
        tmp->nstop4850 = 0;
        start += (strlen("k4850nstop")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->nstop4850 = atoi(value);
        if(tmp->nstop4850 != global_data.nstop4850 ) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"k4850event")) {
        memset(tmp->event4850,0,4);
        start += (strlen("k4850event")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->event4850[0] = value[0];
        if(tmp->event4850[0] != global_data.event4850[0] ) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"k4850vtime")) {
        tmp->nVtime4850 = 0;
        start += (strlen("k4850vtime")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->nVtime4850 = atoi(value);
        if(tmp->nVtime4850 != global_data.nVtime4850 ) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"k4850vmin")) {
        tmp->nVmin4850 = 0;
        start += (strlen("k4850vmin")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->nVmin4850 = atoi(value);
        if(tmp->nVmin4850 != global_data.nVmin4850 ) {
            return true;
        }
        else
            return false;
    }

//4851

    if(getP(&start,&end,data,"ken4851")) {
        tmp->en4851 = 0;
        start += (strlen("ken4851")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->en4851 = atoi(value);
        if(tmp->en4851 != global_data.en4851 ) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"k4851port")) {
        memset(tmp->port4851,0,8);
        start += (strlen("k4851port")+1);
        strncpy(tmp->port4851,start,end-start);
        if(strncmp(tmp->port4851,global_data.port4851,strlen(global_data.port4851))) {
            return true;
        }
        else {
            return false;
        }
    }

    if(getP(&start,&end,data,"k4851uartspeed")) {
        tmp->uartspeed4851 = 0;
        start += (strlen("k4851uartspeed")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->uartspeed4851 = atol(value);
        if(tmp->uartspeed4851 != global_data.uartspeed4851 ) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"k4851nbit")) {
        tmp->nbit4851 = 0;
        start += (strlen("k4851nbit")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->nbit4851 = atoi(value);
        if(tmp->nbit4851 != global_data.nbit4851 ) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"k4851nstop")) {
        tmp->nstop4851 = 0;
        start += (strlen("k4851nstop")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->nstop4851 = atoi(value);
        if(tmp->nstop4851 != global_data.nstop4851 ) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"k4851event")) {
        memset(tmp->event4851,0,4);
        start += (strlen("k4851event")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->event4851[0] = value[0];
        if(tmp->event4851[0] != global_data.event4851[0] ) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"k4851vtime")) {
        tmp->nVtime4851 = 0;
        start += (strlen("k4851vtime")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->nVtime4851 = atoi(value);
        if(tmp->nVtime4851 != global_data.nVtime4851 ) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"k4851vmin")) {
        tmp->nVmin4851 = 0;
        start += (strlen("k4851vmin")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->nVmin4851 = atoi(value);
        if(tmp->nVmin4851 != global_data.nVmin4851 ) {
            return true;
        }
        else
            return false;
    }

    //4852

    if(getP(&start,&end,data,"ken4852")) {
        tmp->en4852 = 0;
        start += (strlen("ken4852")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->en4852 = atoi(value);
        if(tmp->en4852 != global_data.en4852 ) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"k4852port")) {
        memset(tmp->port4852,0,8);
        start += (strlen("k4852port")+1);
        strncpy(tmp->port4852,start,end-start);
        if(strncmp(tmp->port4852,global_data.port4852,strlen(global_data.port4852))) {
            return true;
        }
        else {
            return false;
        }
    }

    if(getP(&start,&end,data,"k4852uartspeed")) {
        tmp->uartspeed4852 = 0;
        start += (strlen("k4852uartspeed")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->uartspeed4852 = atol(value);
        if(tmp->uartspeed4852 != global_data.uartspeed4852 ) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"k4852nbit")) {
        tmp->nbit4852 = 0;
        start += (strlen("k4852nbit")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->nbit4852 = atoi(value);
        if(tmp->nbit4852 != global_data.nbit4852 ) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"k4852nstop")) {
        tmp->nstop4852 = 0;
        start += (strlen("k4852nstop")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->nstop4852 = atoi(value);
        if(tmp->nstop4852 != global_data.nstop4852 ) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"k4852event")) {
        memset(tmp->event4852,0,4);
        start += (strlen("k4852event")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->event4852[0] = value[0];
        if(tmp->event4852[0] != global_data.event4852[0] ) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"k4852vtime")) {
        tmp->nVtime4852 = 0;
        start += (strlen("k4852vtime")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->nVtime4852 = atoi(value);
        if(tmp->nVtime4852 != global_data.nVtime4852 ) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"k4852vmin")) {
        tmp->nVmin4852 = 0;
        start += (strlen("k4852vmin")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->nVmin4852 = atoi(value);
        if(tmp->nVmin4852 != global_data.nVmin4852 ) {
            return true;
        }
        else
            return false;
    }


    //4853

    if(getP(&start,&end,data,"ken4853")) {
        tmp->en4853 = 0;
        start += (strlen("ken4853")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->en4853 = atoi(value);
        if(tmp->en4853 != global_data.en4853 ) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"k4853port")) {
        memset(tmp->port4853,0,8);
        start += (strlen("k4853port")+1);
        strncpy(tmp->port4853,start,end-start);
        if(strncmp(tmp->port4853,global_data.port4853,strlen(global_data.port4853))) {
            return true;
        }
        else {
            return false;
        }
    }

    if(getP(&start,&end,data,"k4853uartspeed")) {
        tmp->uartspeed4853 = 0;
        start += (strlen("k4853uartspeed")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->uartspeed4853 = atol(value);
        if(tmp->uartspeed4853 != global_data.uartspeed4853 ) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"k4853nbit")) {
        tmp->nbit4853 = 0;
        start += (strlen("k4853nbit")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->nbit4853 = atoi(value);
        if(tmp->nbit4853 != global_data.nbit4853 ) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"k4853nstop")) {
        tmp->nstop4853 = 0;
        start += (strlen("k4853nstop")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->nstop4853 = atoi(value);
        if(tmp->nstop4853 != global_data.nstop4853 ) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"k4853event")) {
        memset(tmp->event4853,0,4);
        start += (strlen("k4853event")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->event4853[0] = value[0];
        if(tmp->event4853[0] != global_data.event4853[0] ) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"k4853vtime")) {
        tmp->nVtime4853 = 0;
        start += (strlen("k4853vtime")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->nVtime4853 = atoi(value);
        if(tmp->nVtime4853 != global_data.nVtime4853 ) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"k4853vmin")) {
        tmp->nVmin4853 = 0;
        start += (strlen("k4853vmin")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->nVmin4853 = atoi(value);
        if(tmp->nVmin4853 != global_data.nVmin4853 ) {
            return true;
        }
        else
            return false;
    }

//设置继电器参数
    //relay0

    if(getP(&start,&end,data,"kenrelay0")) {
        tmp->enRelay0 = 0;
        start += (strlen("kenrelay0")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->enRelay0 = atoi(value);
        if(tmp->enRelay0 != global_data.enRelay0 ) {
            return true;
        }
        else
            return false;
    }

    //relay1

    if(getP(&start,&end,data,"kenrelay1")) {
        tmp->enRelay1 = 0;
        start += (strlen("kenrelay1")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->enRelay1 = atoi(value);
        if(tmp->enRelay1 != global_data.enRelay1 ) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"kenrelay")) {
        tmp->enRelay = 0;
        start += (strlen("kenrelay")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->enRelay = atoi(value);
        if(tmp->enRelay != global_data.enRelay ) {
            return true;
        }
        else
            return false;
    }

//设置红外参数
    if(getP(&start,&end,data,"kenInfrared")) {
        tmp->enInfrared = 0;
        start += (strlen("kenInfrared")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->enInfrared = atoi(value);
        if(tmp->enInfrared != global_data.enInfrared ) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"kInfraredport")) {
        memset(tmp->portInfrared,0,8);
        start += (strlen("kInfraredport")+1);
        strncpy(tmp->portInfrared,start,end-start);
        if(strncmp(tmp->portInfrared,global_data.portInfrared,strlen(global_data.portInfrared))) {
            return true;
        }
        else {
            return false;
        }
    }

    if(getP(&start,&end,data,"kInfrareduartspeed")) {
        tmp->uartspeedInf = 0;
        start += (strlen("kInfrareduartspeed")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->uartspeedInf = atol(value);
        if(tmp->uartspeedInf != global_data.uartspeedInf ) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"kInfrarednbit")) {
        tmp->nbitInf = 0;
        start += (strlen("kInfrarednbit")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->nbitInf = atoi(value);
        if(tmp->nbitInf != global_data.nbitInf ) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"kInfrarednstop")) {
        tmp->nstopInf = 0;
        start += (strlen("kInfrarednstop")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->nstopInf = atoi(value);
        if(tmp->nstopInf != global_data.nstopInf ) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"kInfraredevent")) {
        memset(tmp->eventInf,0,4);
        start += (strlen("kInfraredevent")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->eventInf[0] = value[0];
        if(tmp->eventInf[0] != global_data.eventInf[0] ) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"kInfraredvtime")) {
        tmp->nVtimeInf = 0;
        start += (strlen("kInfraredvtime")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->nVtimeInf = atoi(value);
        if(tmp->nVtimeInf != global_data.nVtimeInf ) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"kInfraredvmin")) {
        tmp->nVminInf = 0;
        start += (strlen("kInfraredvmin")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->nVminInf = atoi(value);
        if(tmp->nVminInf != global_data.nVminInf ) {
            return true;
        }
        else
            return false;
    }

//设置IO参数
    if(getP(&start,&end,data,"kSetIO_0")) {
        tmp->enIo_0 = 0;
        start += (strlen("kSetIO_0")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->enIo_0 = atoi(value);
        if(tmp->enIo_0 != global_data.enIo_0 ) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"kSetIO_1")) {
        tmp->enIo_1 = 0;
        start += (strlen("kSetIO_1")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->enIo_1 = atoi(value);
        if(tmp->enIo_1 != global_data.enIo_1 ) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"kSetIO_2")) {
        tmp->enIo_2 = 0;
        start += (strlen("kSetIO_2")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->enIo_2 = atoi(value);
        if(tmp->enIo_2 != global_data.enIo_2 ) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"kSetIO_3")) {
        tmp->enIo_3 = 0;
        start += (strlen("kSetIO_3")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->enIo_3 = atoi(value);
        if(tmp->enIo_3 != global_data.enIo_3 ) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"kSetIO_4")) {
        tmp->enIo_4 = 0;
        start += (strlen("kSetIO_4")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->enIo_4 = atoi(value);
        if(tmp->enIo_4 != global_data.enIo_4 ) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"kSetIO_5")) {
        tmp->enIo_5 = 0;
        start += (strlen("kSetIO_5")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->enIo_5 = atoi(value);
        if(tmp->enIo_5 != global_data.enIo_5 ) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"kSetIO_6")) {
        tmp->enIo_6 = 0;
        start += (strlen("kSetIO_6")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->enIo_6 = atoi(value);
        if(tmp->enIo_6 != global_data.enIo_6 ) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"kSetIO_7")) {
        tmp->enIo_7 = 0;
        start += (strlen("kSetIO_7")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->enIo_7 = atoi(value);
        if(tmp->enIo_7 != global_data.enIo_7 ) {
            return true;
        }
        else
            return false;
    }
//设置TCP参数
    if(getP(&start,&end,data,"kTcpEnable")) {
        tmp->tcp_en = 0;
        start += (strlen("kTcpEnable")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->tcp_en = atoi(value);
        if(tmp->tcp_en != global_data.tcp_en ) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"kTcpServerPort")) {
        tmp->tcp_port = 0;
        start += (strlen("kTcpServerPort")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->tcp_port = atoi(value);
        if(tmp->tcp_port != global_data.tcp_port ) {
            return true;
        }
        else
            return false;
    }

    if(getP(&start,&end,data,"kTcpLinkMax")) {
        tmp->tcp_link_max = 0;
        start += (strlen("kTcpLinkMax")+1);
        memset(value,0,8);
        strncpy(value,start,end-start);
        tmp->tcp_link_max = atoi(value);
        if(tmp->tcp_link_max != global_data.tcp_link_max ) {
            return true;
        }
        else
            return false;
    }
    return true;
}



bool umpSet::relyUMP()
{
    FILE *file;
    char data[2048] = {0};
    memset(data,0,2048);
    int sendNum = 0, readnum = 0;

    if(NULL == (file = fopen("/data/remserial.ini","r"))) {
        printf("can't fopen ini file\n");
        return false;
    }

    for(int i = 0; i < 2048; i++) {
        if(0 == (readnum = fread(data+i,1,1,file)))
            break;
        else
            printf("%c ",data[i]);
    }
    fclose(file);
    if(mlt_echo) {
        umpAddr.sin_family 		= AF_INET;
        umpAddr.sin_port   		= htons(4001);
        umpAddr.sin_addr.s_addr 	= inet_addr("224.168.1.1");
    }

    sendNum = sendto(socketUmp,data,strlen(data),0,(struct sockaddr*)&umpAddr,sizeof(struct sockaddr_in));


    if(sendNum <= 0 ) {
        return false;
    }

    return true;
}
