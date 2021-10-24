//广州8000开始启用定时器GPIO方式发送红外,通过获取版本号来区分
#ifndef INFRARED_H
#define INFRARED_H
#include "global.h"
#include "uart.h"
#include "hi_mpp.h"
#include "io.h"
#include "hi_reg.h"

#define DEVPATH   "/dev/gpio_ir" //"/sys/module/gpio_ir"

class infrared
{
public:
    infrared();
    ~infrared();
    bool infraredIni(int irModel);
    bool rcvData2Ir();
    bool readData2Ir_tc();
    bool UMPsetParam(bool uartFlag,bool ctrlPortFlag,bool svPortFlag);
public:
    int devInfFd = -1;
    int socketInf = -1;
private:
    struct sockaddr_in ctrlAddr;
};

#endif // INFRARED_H
