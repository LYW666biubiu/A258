#include "infrared.h"

infrared::infrared()
{

}

infrared::~infrared()
{
    FD_CLR(socketInf,&readFd);
    close(socketInf);
    close(devInfFd);
    socketInf = -1;
    devInfFd = -1;
    if(hd_ver == NJ_700K) {
        system("rmmod gpio_ir_timer_nj.ko");
    }
    else {
        system("rmmod gpio_ir_timer.ko");
    }

}

bool infrared::infraredIni(int irModel)
{
    printf("\ninf init start\n");
    uint value;
    if(irTime) {
        if(hd_ver == NJ_700K) {
            system("insmod gpio_ir_timer_nj.ko");
        }
        else {
            printf("\r\ninsmod gpio_ir_timer.ko ready\r\n");
            system("insmod gpio_ir_timer.ko");
            printf("\r\ninsmod gpio_ir_timer.ko ok\r\n");
        }
        sleep(1);
#ifdef __7000__
        get_reg(GPIO0_6DIR);
        value |= GPIO0_6SET;
        set_reg(GPIO0_6DIR,value);

        get_reg(GPIO0_6DATA);
        value |= GPIO0_6SET;
        set_reg(GPIO0_6DATA,value);
#endif

#ifdef  __7000_NJ__
        if(hd_ver ==PTN_21A) {
            set_reg(IO_1_5_6_MUX, 0x1);
            set_reg(IO_DIR_11_6_MUX, 0x0);

            get_reg(IO_DIR_11_6_DIR);
            value |= IO_DIR_11_6_SET;
            set_reg(IO_DIR_11_6_DIR, value);
            if(1 == irModel){
                get_reg(IO_DIR_11_6_DATA);
                value |= IO_DIR_11_6_SET;
                set_reg(IO_DIR_11_6_DATA, value);

                get_reg(IO_1_5_6_DIR, &value);
                value |= IO_1_5_6_SET;
                set_reg(IO_1_5_6_DIR, value);

            }
            else if(2 ==  irModel) {
                get_reg(IO_DIR_11_6_DATA);
                value &= (~IO_DIR_11_6_SET);
                set_reg(IO_DIR_11_6_DATA, value);

                get_reg(IO_1_5_6_DIR);
                value &= (~IO_1_5_6_SET);
                set_reg(IO_1_5_6_DIR, value);

                set_reg(IO_0_10_2_MUX, 0x0);
                set_reg(IO_DIR_11_3_MUX, 0x0);

                get_reg(IO_DIR_11_3_DIR);
                value |= IO_DIR_11_3_SET;
                set_reg(IO_DIR_11_3_DIR, value);

                get_reg(IO_DIR_11_3_DATA);
                value |= IO_DIR_11_3_SET;
                set_reg(IO_DIR_11_3_DATA, value);

                get_reg(IO_0_10_2_DIR);
                value |= IO_0_10_2SET;
                set_reg(IO_0_10_2_DIR, value);

                get_reg(IO_0_10_2_DATA);
                value |= IO_0_10_2SET;
                set_reg(IO_0_10_2_DATA, value);
            }
        }
        else {
            if(1 == irModel) {          //红外为发射状态
                get_reg(GPIO0_4DIR);
                value |= GPIO0_4SET;
                set_reg(GPIO0_4DIR,value);

                get_reg(GPIO0_4DATA);
                value |= GPIO0_4SET;
                set_reg(GPIO0_4DATA,value);
            }
            else if(2 == irModel) {     //红外学习状态，需要IO置为输出高电平，供电
                set_reg(GPIO13_5MUX,0x0);

                get_reg(GPIO13_5DIR);
                value |= GPIO13_5SET;
                set_reg(GPIO13_5DIR,value);

                get_reg(GPIO13_5DATA);
                value |= GPIO13_5SET;
                set_reg(GPIO13_5DATA,value);

                get_reg(GPIO0_6DIR);
                value |= GPIO0_6SET;
                set_reg(GPIO0_6DIR,value);

                get_reg(GPIO0_6DATA);
                value |= GPIO0_6SET;
                set_reg(GPIO0_6DATA,value);


                get_reg(GPIO0_4DIR);
                value |= GPIO0_4SET;
                set_reg(GPIO0_4DIR,value);

                get_reg(GPIO0_4DATA);
                value &= (~GPIO0_4SET);
                set_reg(GPIO0_4DATA,value);
            }
        }



#endif


#ifdef __GPIO1_5__
        if((1 == irModel) && (hd_ver != NJ_700K)) {          //红外为发射状态
            if(hd_ver == GZ_700S) {
                set_reg(GPIO11_3REG, 0);

                get_reg(GPIO11_3DIR, &value);
                value |= GPIO11_3SET;
                set_reg(GPIO11_3DIR, value);

                get_reg(GPIO11_3DATA, &value);
                value |= GPIO11_3SET;
                set_reg(GPIO11_3DATA, value);

                get_reg(GPIO14_7DIR, &value);
                value |= GPIO14_7SET;
                set_reg(GPIO14_7DIR, value);
            }
            else {
                set_reg(GPIO1_5MUX,0x1);

                get_reg(GPIO1_5DIR);
                value |= GPIO1_5SET;
                set_reg(GPIO1_5DIR,value);

                get_reg(GPIO1_5DATA);
                value |= GPIO1_5SET;
                set_reg(GPIO1_5DATA,value);
            }

        }
        else if(2 == irModel) {     //红外学习状态，需要IO置为输出高电平，供电
            if(hd_ver == NJ_700K) {
                set_reg(GPIO20_5MUX,0x0);

                get_reg(GPIO20_5DIR);
                value |= GPIO20_5SET;
                set_reg(GPIO20_5DIR,value);

                get_reg(GPIO20_5DATA);
                value |= GPIO20_5SET;
                set_reg(GPIO20_5DATA,value);
            }
            else if(hd_ver == GZ_700S) {
                set_reg(GPIO11_2REG, 0x1);

                get_reg(GPIO11_2DIR, &value);
                value |= GPIO11_2SET;
                set_reg(GPIO11_2DIR, value);

                get_reg(GPIO11_2DATA, &value);
                value |= GPIO11_2SET;
                set_reg(GPIO11_2DATA, value);

                set_reg(GPIO11_1REG, 0x1);

                get_reg(GPIO11_1DIR, &value);
                value |= GPIO11_1SET;
                set_reg(GPIO11_1DIR, value);
                get_reg(GPIO11_1DATA, &value);
                value |= GPIO11_1SET;
                set_reg(GPIO11_1DATA, value);

                set_reg(GPIO11_3REG, 0x0);
                get_reg(GPIO11_3DIR, &value);
                value |= GPIO11_3SET;
                set_reg(GPIO11_3DIR, value);

                get_reg(GPIO11_3DATA, &value);
                value &= (~GPIO11_3SET);
                set_reg(GPIO11_3DATA, value);

            }
            else {
                set_reg(GPIO1_6MUX,0x1);

                get_reg(GPIO1_6DIR);
                value |= GPIO1_6SET;
                set_reg(GPIO1_6DIR,value);

                get_reg(GPIO1_6DATA);
                value |= GPIO1_6SET;
                set_reg(GPIO1_6DATA,value);

                set_reg(GPIO1_7MUX,0x1);

                get_reg(GPIO1_7DIR);
                value |= GPIO1_7SET;
                set_reg(GPIO1_7DIR,value);

                get_reg(GPIO1_7DATA);
                value |= GPIO1_7SET;
                set_reg(GPIO1_7DATA,value);

                set_reg(GPIO1_5MUX,0x1);

                get_reg(GPIO1_5DIR);
                value |= GPIO1_5SET;
                set_reg(GPIO1_5DIR,value);

                get_reg(GPIO1_5DATA);
                value &= (~GPIO1_5SET);
                set_reg(GPIO1_5DATA,value);
            }

        }

#endif

#ifdef __GPIO1_6__
            set_reg(GPIO1_6MUX,0x1);

            get_reg(GPIO1_6DIR,&value);
            value |= GPIO1_6SET;
            set_reg(GPIO1_6DIR,value);

            get_reg(GPIO1_6DATA,&value);
            value |= GPIO1_6SET;
            set_reg(GPIO1_6DATA,value);
#endif
        devInfFd = open(DEVPATH,   O_RDWR | O_TRUNC);
        if(-1 == devInfFd) {
            COMMON_PRT("open %s error\n",DEVPATH);
            return false;
        }
    }
    else {
        open_port(global_data.portInfrared,&devInfFd);
        set_optBak(global_data.uartspeedInf,global_data.nbitInf,global_data.nstopInf,   \
                   global_data.eventInf[0],global_data.nVtimeInf,global_data.nVminInf,&devInfFd);

        FD_SET(devInfFd,&readFd);
        if(devInfFd >= maxFd)
            maxFd = devInfFd + 1;
    }
    printf("ir sv port:%d\n",global_data.svInfraredport);
    socketInf = createUdp(global_data.svInfraredport);
    printf("!create socketinf ok!");
    FD_SET(socketInf,&readFd);
    if(socketInf >= maxFd)
        maxFd = socketInf + 1;
    return true;
}

bool infrared::readData2Ir_tc(){

//        printf("----readData2Ir_tc  start--------\n");
        char tmpData_tc[MAXDATA] = {0};
        int rcvLen = 0,len = 0,errorlen = 0, sendLen = 0 ;
        socklen_t addrLen = 0;
        memset(tmpData_tc,0,sizeof(tmpData_tc));
        char irEcho[8] = {0};
//        addrLen = sizeof(struct sockaddr_in);
//        rcvLen = recvfrom(socketInf,tmpData,MAXDATA,0,(struct sockaddr*)(&ctrlAddr),&addrLen);//不能别人告诉你什么时候透传 是read完就发

        tmpData_tc[0]=0x55;//这应该加发送的协议头
        tmpData_tc[1]=0xA3;
        //printf("8881\n");
        len = read(devInfFd,&tmpData_tc[3],MAXDATA);  //创建devinffd的socket 然后把加进slect （fd_set  fd_isset..）
//在下面加是否满足透传的条件 是否发数据
        //printf("len:%d\n",len);
//        for (int i = len+4; i >= 6; i--) {
//               tmpData_tc[i] = tmpData_tc[i-1];
//            }
//            tmpData_tc[5] =0x28;
        printdata(tmpData_tc,len+3);

        if(len>0)
        {
            for(int i = 0; i < len; i++) {
                if(0 == tmpData_tc[i])
                    errorlen++;

            }
            if((errorlen > len/3) || (len < 30)) {
                memset(irEcho,0,sizeof(irEcho));
                strcpy(irEcho,"error");
                sendLen = sendto(socketInf,irEcho,strlen(irEcho),0,(struct sockaddr*)&ctrlAddr,sizeof(struct sockaddr_in));
                printf("----send irEcho \n");
            }
            else {
                ctrlAddr.sin_family = AF_INET;
                ctrlAddr.sin_port = htons(global_data.ctlinfraredport);
                ctrlAddr.sin_addr.s_addr = inet_addr(global_data.ctlip);
                sendLen = sendto(socketInf,tmpData_tc,len+3,0,(struct sockaddr*)&ctrlAddr,sizeof(struct sockaddr_in));//
//                printf("----sendto  socketInf ok--------\n");
                printf("\nctlip:%s   ctlinfraredport:%d\n",global_data.ctlip,global_data.ctlinfraredport);
            }
        }

//                rcvLen = recvfrom(socketInf,tmpData_tc,MAXDATA,0,(struct sockaddr*)(&ctrlAddr),&addrLen);
//                if(rcvLen > 0)
//                {
//                    write(devInfFd,tmpData_tc,rcvLen);
//                }
}

bool infrared::rcvData2Ir()
{
   //printf("----rcvData2Ir  start--------\n");
    char tmpData[MAXDATA] = {0};
    int rcvLen = 0,len = 0,errorlen = 0, sendLen = 0 ;
    socklen_t addrLen = 0;
    memset(tmpData,0,sizeof(tmpData));
    char irEcho[8] = {0};

    addrLen = sizeof(struct sockaddr_in);

    rcvLen = recvfrom(socketInf,tmpData,MAXDATA,0,(struct sockaddr*)(&ctrlAddr),&addrLen);
    if(rcvLen > 0) {
#ifdef __DEBUG__
    //print0x(tmpData,rcvLen);
#endif
    if(1 == global_data.enInfrared) {
        len = write(devInfFd,tmpData,rcvLen);//把网络数据(协议头+红外数据)根据驱动整合以后写到板子 开启发送
        sleep(1);
    }
    else if(2 == global_data.enInfrared) {
        if((tmpData[0] == 0x55) && (tmpData[1] == 0xA1)) {
            len = write(devInfFd,tmpData,2);//网络数据写到板子 告知板子是学习模式 板子启动相关设置 改透传这里好像不需要了 一直是学习模式
            memset(tmpData,0,MAXDATA);
            len = read(devInfFd,tmpData,MAXDATA);//读取板子学习到的红外数据 给到tmpdata
            for(int i = 0; i < len; i++) {
                if(0 == tmpData[i])
                    errorlen++;
            }
            if((errorlen > len/3) || (len < 30)) {
                memset(irEcho,0,sizeof(irEcho));
                strcpy(irEcho,"error");
                sendLen = sendto(socketInf,irEcho,strlen(irEcho),0,(struct sockaddr*)&ctrlAddr,sizeof(struct sockaddr_in));
            }
            else {
                sendLen = sendto(socketInf,tmpData,len,0,(struct sockaddr*)&ctrlAddr,sizeof(struct sockaddr_in));//把读取的红外数据发给网络
//                printf("----sendto  socketInf ok2--------\n");
            }
        }
    }

    }
    else {
        COMMON_PRT("net not rcv ir data\n");
        return false;
    }
    return true;
}

/*
 * 配合UMP设置，时时生效
 * 全局参数在外部修改，保存
 * */

bool infrared::UMPsetParam(bool uartFlag,bool ctrlPortFlag,bool svPortFlag)
{
    if(uartFlag) {
        COMMON_PRT("UMP set ir UART Param\n");
        FD_CLR(devInfFd,&readFd);
        close(devInfFd);
        sleep(1);

        devInfFd = -1;
        open_port(global_data.portInfrared,&devInfFd);
        set_optBak(global_data.uartspeedInf,global_data.nbitInf,global_data.nstopInf,   \
                   global_data.eventInf[0],global_data.nVtimeInf,global_data.nVminInf,&devInfFd);

        FD_SET(devInfFd,&readFd);
        if(devInfFd >= maxFd)
            maxFd = devInfFd + 1;
    }

    if(ctrlPortFlag) {
        COMMON_PRT("UMP set ir ctrl port Param\n");
    }

    if(svPortFlag) {
        FD_CLR(socketInf,&readFd);
        close(socketInf);
        sleep(1);

        socketInf = createUdp(global_data.svInfraredport);
        FD_SET(socketInf,&readFd);
        if(socketInf >= maxFd)
            maxFd = socketInf + 1;
    }

    return true;
}

