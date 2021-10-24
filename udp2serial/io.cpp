#include "io.h"

io::io()
{

}

io::~io()
{
//    FD_CLR(socketIO_A258,&readFd);
//    close(socketIO_A258);
//    socketIO_A258 = -1;
}

bool io::ioIni(int ioNum)
{
    uint value;
    if((hd_ver == NJ_7000) || (hd_ver == PTN_7000)) {
        if(0 == ioNum) {
            set_reg(GPIO13_5MUX,0x0);

            get_reg(GPIO13_5DIR);
            value |= GPIO13_5SET;
            set_reg(GPIO13_5DIR,value);

            if(1 == global_data.enIo_0) {         //IO作为输入
                get_reg(GPIO13_5DATA);
                value &= (~GPIO13_5SET);
                set_reg(GPIO13_5DATA,value);

                get_reg(GPIO0_6DIR);
                value &= (~GPIO0_6SET);
                set_reg(GPIO0_6DIR,value);
            }
            else if(2 == global_data.enIo_0) {    //IO作为输出
                get_reg(GPIO13_5DATA);
                value |= GPIO13_5SET;
                set_reg(GPIO13_5DATA,value);

                get_reg(GPIO0_6DIR);
                value |= GPIO0_6SET;
                set_reg(GPIO0_6DIR,value);

            }
            else {
                COMMON_PRT("io en value error\n");
                return false;
            }
        }

        if((1 == ioNum) && (0 == global_data.enInfrared)) {

            get_reg(GPIO0_4DIR);
            value |= GPIO0_4SET;
            set_reg(GPIO0_4DIR,value);


            if(1 == global_data.enIo_1) {         //IO作为输入
                get_reg(GPIO0_4DATA);
                value &= (~GPIO0_4SET);
                set_reg(GPIO0_4DATA,value);

                get_reg(GPIO0_3DIR);
                value &= (~GPIO0_3SET);
                set_reg(GPIO0_3DIR,value);

            }
            else if(2 == global_data.enIo_1) {    //IO作为输出
                get_reg(GPIO0_4DATA);
                value |= GPIO0_4SET;
                set_reg(GPIO0_4DATA,value);

                get_reg(GPIO0_3DIR);
                value |= GPIO0_3SET;
                set_reg(GPIO0_3DIR,value);

            }
            else {
                COMMON_PRT("io en value error\n");
                return false;
            }
        }
    }
    else if(hd_ver == PTN_21A) {
        set_reg(IO_DIR_11_6_MUX, 0x0);

        get_reg(IO_DIR_11_6_DIR);
        value |= IO_DIR_11_6_SET;
        set_reg(IO_DIR_11_6_DIR, value);

        set_reg(IO_DIR_11_3_MUX, 0x0);

        get_reg(IO_DIR_11_3_DIR);
        value |= IO_DIR_11_3_SET;
        set_reg(IO_DIR_11_3_DIR, value);

        if(0 == ioNum) {
            set_reg(IO_0_10_2_MUX, 0x0);

            if(1 == global_data.enIo_0) {
                get_reg(IO_DIR_11_3_DATA);
                value &= (~IO_DIR_11_3_SET);
                set_reg(IO_DIR_11_3_DATA, value);

                get_reg(IO_0_10_2_DIR);
                value &= (~IO_0_10_2SET);
                set_reg(IO_0_10_2_DIR, value);
            }
            else if(2 == global_data.enIo_0) {
                get_reg(IO_DIR_11_3_DATA);
                value |= IO_DIR_11_3_SET;
                set_reg(IO_DIR_11_3_DATA, value);

                get_reg(IO_0_10_2_DIR);
                value |= IO_0_10_2SET;
                set_reg(IO_0_10_2_DIR, value);
            }
        }
        else if(1 == ioNum && 0 == global_data.enInfrared) {
            set_reg(IO_1_5_6_MUX, 0x1);
            if(1 == global_data.enIo_1) {
                get_reg(IO_DIR_11_6_DATA);
                value &= (~IO_DIR_11_6_SET);
                set_reg(IO_DIR_11_6_DATA, value);

                get_reg(IO_1_5_6_DIR);
                value &= (~IO_1_5_6_SET);
                set_reg(IO_1_5_6_DIR, value);
            }
            else if(2 == global_data.enIo_1) {
                get_reg(IO_DIR_11_6_DATA);
                value |= IO_DIR_11_6_SET;
                set_reg(IO_DIR_11_6_DATA, value);

                get_reg(IO_1_5_6_DIR);
                value |= IO_1_5_6_SET;
                set_reg(IO_1_5_6_DIR, value);
            }
        }

    }
    else if(hd_ver == GZ_700S) {
        if(0 == ioNum) {
            set_reg(GPIO11_2REG,0x1);

            get_reg(GPIO11_2DIR);
            value |= GPIO11_2SET;
            set_reg(GPIO11_2DIR,value);

            if(1 == global_data.enIo_0) {         //IO作为输入

                get_reg(GPIO11_2DATA);
                value &= (~GPIO11_2SET);
                set_reg(GPIO11_2DATA,value);

                get_reg(GPIO11_1DIR);
                value &= (~GPIO11_1SET);
                set_reg(GPIO11_1DIR,value);
            }
            else if(2 == global_data.enIo_0) {    //IO作为输出
                get_reg(GPIO11_2DATA);
                value |= GPIO11_2SET;
                set_reg(GPIO11_2DATA,value);

                get_reg(GPIO11_1DIR);
                value |= GPIO11_1SET;
                set_reg(GPIO11_1DIR,value);

            }
            else {
                COMMON_PRT("io en value error\n");
                return false;
            }
        }

        if((1 == ioNum) && (0 == global_data.enInfrared)) {

            get_reg(GPIO11_3DIR);
            value |= GPIO11_3SET;
            set_reg(GPIO11_3DIR,value);


            if(1 == global_data.enIo_1) {         //IO作为输入
                get_reg(GPIO11_3DATA);
                value &= (~GPIO11_3SET);
                set_reg(GPIO11_3DATA,value);

                get_reg(GPIO14_7DIR);
                value &= (~GPIO14_7SET);
                set_reg(GPIO14_7DIR,value);

            }
            else if(2 == global_data.enIo_1) {    //IO作为输出
                get_reg(GPIO11_3DATA);
                value |= GPIO11_3SET;
                set_reg(GPIO11_3DATA,value);

                get_reg(GPIO14_7DIR);
                value |= GPIO14_7SET;
                set_reg(GPIO14_7DIR,value);

            }
            else {
                COMMON_PRT("io en value error\n");
                return false;
            }
        }
    }
    else if(hd_ver == NJ_21D) {
        if(0 == ioNum) {
            get_reg(GPIO0_3DIR);


            if(1 == global_data.enIo_0) {         //IO作为输入
                value &= (~GPIO0_3SET);
                set_reg(GPIO0_3DIR,value);
            }
            else if(2 == global_data.enIo_0) {    //IO作为输出
                value |= GPIO0_3SET;
                set_reg(GPIO0_3DIR,value);
            }
            else {
                COMMON_PRT("io en value error\n");
                return false;
            }
        }
        else {
            get_reg(GPIO0_5DIR);
            if(1 == global_data.enIo_1) {         //IO作为输入
                value &= (~GPIO0_5SET);
                set_reg(GPIO0_5DIR,value);
                printf("gpio0_5:0x%x\n",value);
            }
            else if(2 == global_data.enIo_1) {    //IO作为输出
                value |= GPIO0_5SET;
                set_reg(GPIO0_5DIR,value);
            }
            else {
                COMMON_PRT("io en value error\n");
                return false;
            }

        }
    }
    else if((hd_ver == NJ_700K) || (hd_ver == GZ_700K) || (hd_ver == GZ_8000)) {
        if(1 == ioNum) {
            if(ir_nj_700k) {
                set_reg(GPIO15_6MUX,0x1);

                if(1 == global_data.enIo_1) {       //IO作为输入
                    get_reg(GPIO15_6DIR);
                    value &= (~GPIO15_6SET);
                    set_reg(GPIO15_6DIR,value);
                }
                else if(2 == global_data.enIo_1) {  //IO作为输出
                    get_reg(GPIO15_6DIR);
                    value |= GPIO15_6SET;
                    set_reg(GPIO15_6DIR,value);
                }

                set_reg(GPIO1_7MUX, 0x1);

                if(1 == global_data.enIo_1) {       //IO作为输入
                    get_reg(GPIO1_7DIR);
                    value &= (~GPIO1_7SET);
                    set_reg(GPIO1_7DIR,value);
                }
                else if(2 == global_data.enIo_1) {  //IO作为输出
                    get_reg(GPIO1_7DIR);
                    value |= GPIO1_7SET;
                    set_reg(GPIO1_7DIR,value);
                }
            }
            else {
                set_reg(GPIO1_5MUX,0x1);

                get_reg(GPIO1_5DIR);
                value |= GPIO1_5SET;
                set_reg(GPIO1_5DIR,value);

                set_reg(GPIO20_5MUX,0x0);

                if(1 == global_data.enIo_1) {         //IO作为输入
                    get_reg(GPIO1_5DATA);
                    value &= (~GPIO1_5SET);
                    set_reg(GPIO1_5DATA,value);

                    get_reg(GPIO20_5DIR);
                    value &= (~GPIO20_5SET);
                    set_reg(GPIO20_5DIR,value);
                }
                else if(2 == global_data.enIo_1) {    //IO作为输出
                    get_reg(GPIO1_5DATA);
                    value |= GPIO1_5SET;
                    set_reg(GPIO1_5DATA,value);

                    get_reg(GPIO20_5DIR);
                    value |= GPIO20_5SET;
                    set_reg(GPIO20_5DIR,value);
                }
                else {
                    COMMON_PRT("io en value error\n");
                    return false;
                }
            }

        }
        if(0 == ioNum) {
            if(ir_nj_700k) {
                set_reg(GPIO20_5MUX,0x0);

                if(1 == global_data.enIo_0) {           //IO作为输入
                    get_reg(GPIO20_5DIR);
                    value &= (~GPIO20_5SET);
                    set_reg(GPIO20_5DIR,value);
                }
                else if(2 == global_data.enIo_0) {      //IO作为输出
                    get_reg(GPIO20_5DIR);
                    value |= GPIO20_5SET;
                    set_reg(GPIO20_5DIR,value);
                }
                else {
                    COMMON_PRT("io en value error\n");
                    return false;
                }
            }
            else {
                set_reg(GPIO1_6MUX,0x1);

                get_reg(GPIO1_6DIR);
                value |= GPIO1_6SET;
                set_reg(GPIO1_6DIR,value);

                set_reg(GPIO1_7MUX,0x1);

                if(1 == global_data.enIo_0) {         //IO作为输入
                    get_reg(GPIO1_6DATA);
                    value &= (~GPIO1_6SET);
                    set_reg(GPIO1_6DATA,value);

                    get_reg(GPIO1_7DIR);
                    value &= (~GPIO1_7SET);
                    set_reg(GPIO1_7DIR,value);
                }
                else if(2 == global_data.enIo_0) {    //IO作为输出
                    get_reg(GPIO1_6DATA);
                    value |= GPIO1_6SET;
                    set_reg(GPIO1_6DATA,value);

                    get_reg(GPIO1_7DIR);
                    value |= GPIO1_7SET;
                    set_reg(GPIO1_7DIR,value);
                }
                else {
                    COMMON_PRT("io en value error\n");
                    return false;
                }
            }

        }
    }
    else if(hd_ver == NJ_D_700K) {

    }
    else if(hd_ver == NJ_31DV200) {
        if(0 == ioNum) {
            get_reg(IO_0_DIR_REG);
            value &= (~0x1);
            set_reg(IO_0_DIR_REG, value);

            get_reg(IO_0_DIR_DIR);
            value |= IO_0_DIR_SET;
            set_reg(IO_0_DIR_DIR, value);

            get_reg(IO_0_DATA_REG);
            value &= (~0x1);
            set_reg(IO_0_DATA_REG, value);

            if(1 == global_data.enIo_0) {
                get_reg(IO_0_DIR_DATA);
                value &= (~IO_0_DIR_SET);
                set_reg(IO_0_DIR_DATA, value);

                get_reg(IO_0_DATA_DIR);
                value &= (~IO_0_DATA_SET);
                set_reg(IO_0_DATA_DIR, value);
            }
            else if(2 ==  global_data.enIo_0) {
                get_reg(IO_0_DIR_DATA);
                value |= IO_0_DIR_SET;
                set_reg(IO_0_DIR_DATA, value);

                get_reg(IO_0_DATA_DIR);
                value |= IO_0_DATA_SET;
                set_reg(IO_0_DATA_DIR, value);
            }
        }
        else if(1 == ioNum) {
            get_reg(IO_1_DIR_REG);
            value &= (~0x1);
            set_reg(IO_1_DIR_REG, value);

            get_reg(IO_1_DIR_DIR);
            value |= IO_1_DIR_SET;
            set_reg(IO_1_DIR_DIR, value);

            get_reg(IO_1_DATA_REG);
            value &= (~0x1);
            set_reg(IO_1_DATA_REG, value);

            if(1 == global_data.enIo_0) {
                get_reg(IO_1_DIR_DATA);
                value &= (~IO_1_DIR_SET);
                set_reg(IO_1_DIR_DATA, value);

                get_reg(IO_1_DATA_DIR);
                value &= (~IO_1_DATA_SET);
                set_reg(IO_1_DATA_DIR, value);
            }
            else if(2 ==  global_data.enIo_0) {
                get_reg(IO_1_DIR_DATA);
                value |= IO_1_DIR_SET;
                set_reg(IO_1_DIR_DATA, value);

                get_reg(IO_1_DATA_DIR);
                value |= IO_1_DATA_SET;
                set_reg(IO_1_DATA_DIR, value);
            }

        }
        else if(2 == ioNum) {
            get_reg(IO_2_DIR_REG);
            value &= (~0x1);
            set_reg(IO_2_DIR_REG, value);

            get_reg(IO_2_DIR_DIR);
            value |= IO_2_DIR_SET;
            set_reg(IO_2_DIR_DIR, value);

            get_reg(IO_2_DATA_REG);
            value &= (~0x1);
            set_reg(IO_2_DATA_REG, value);

            if(1 == global_data.enIo_0) {
                get_reg(IO_2_DIR_DATA);
                value &= (~IO_2_DIR_SET);
                set_reg(IO_2_DIR_DATA, value);

                get_reg(IO_2_DATA_DIR);
                value &= (~IO_2_DATA_SET);
                set_reg(IO_2_DATA_DIR, value);
            }
            else if(2 ==  global_data.enIo_0) {
                get_reg(IO_2_DIR_DATA);
                value |= IO_2_DIR_SET;
                set_reg(IO_2_DIR_DATA, value);

                get_reg(IO_2_DATA_DIR);
                value |= IO_2_DATA_SET;
                set_reg(IO_2_DATA_DIR, value);
            }

        }
        else if(3 ==  ioNum) {
            get_reg(IO_3_DIR_REG);
            value &= (~0x1);
            set_reg(IO_3_DIR_REG, value);

            get_reg(IO_3_DIR_DIR);
            value |= IO_3_DIR_SET;
            set_reg(IO_3_DIR_DIR, value);

            get_reg(IO_3_DATA_REG);
            value |= 0x1;
            set_reg(IO_3_DATA_REG, value);

            if(1 == global_data.enIo_0) {
                get_reg(IO_3_DIR_DATA);
                value &= (~IO_3_DIR_SET);
                set_reg(IO_3_DIR_DATA, value);

                get_reg(IO_3_DATA_DIR);
                value &= (~IO_3_DATA_SET);
                set_reg(IO_3_DATA_DIR, value);
            }
            else if(2 ==  global_data.enIo_0) {
                get_reg(IO_3_DIR_DATA);
                value |= IO_3_DIR_SET;
                set_reg(IO_3_DIR_DATA, value);

                get_reg(IO_3_DATA_DIR);
                value |= IO_3_DATA_SET;
                set_reg(IO_3_DATA_DIR, value);
            }

        }
    }
    else if(hd_ver == NJ_A258) {       //添加A258  IO输入输出设置
        if(0 == ioNum) {
            value=get_reg(A258_IO0_REG);   //复用成gpio
            value &= (~0x1);
            set_reg(A258_IO0_REG, value);

            if(1 == global_data.enIo_0) {          //IO设置成输入
                value=get_reg(A258_IO0_DIR);
                value &= (~A258_IO0_SET);
                set_reg(A258_IO0_DIR, value);

            }
            else if(2 ==  global_data.enIo_0) {     //IO设置成输出
                value=get_reg(A258_IO0_DIR);
                value |= A258_IO0_SET;
                set_reg(A258_IO0_DIR, value);

            }
        }
        else if(1 == ioNum) {
            value=get_reg(A258_IO1_REG);   //复用成gpio
            value &= (~0x1);
            set_reg(A258_IO1_REG, value);

            if(1 == global_data.enIo_1) {          //IO设置成输入
                value=get_reg(A258_IO1_DIR);
                value &= (~A258_IO1_SET);
                set_reg(A258_IO1_DIR, value);

            }
            else if(2 ==  global_data.enIo_1) {     //IO设置成输出
                value=get_reg(A258_IO1_DIR);
                value |= A258_IO1_SET;
                set_reg(A258_IO1_DIR, value);

            }
        }
        else if(2 == ioNum) {
            value=get_reg(A258_IO2_REG);   //复用成gpio
            value &= (~0x1);
            set_reg(A258_IO2_REG, value);

            if(1 == global_data.enIo_2) {          //IO设置成输入
                value=get_reg(A258_IO2_DIR);
                value &= (~A258_IO2_SET);
                set_reg(A258_IO2_DIR, value);

            }
            else if(2 ==  global_data.enIo_2) {     //IO设置成输出
                value=get_reg(A258_IO2_DIR);
                value |= A258_IO2_SET;
                set_reg(A258_IO2_DIR, value);

            }
        }
        else if(3 == ioNum) {
            value=get_reg(A258_IO3_REG);   //复用成gpio
            value &= (~0x1);
            set_reg(A258_IO3_REG, value);

            if(1 == global_data.enIo_3) {          //IO设置成输入
                value=get_reg(A258_IO3_DIR);
                value &= (~A258_IO3_SET);
                set_reg(A258_IO3_DIR, value);

            }
            else if(2 ==  global_data.enIo_3) {     //IO设置成输出
                value=get_reg(A258_IO3_DIR);
                value |= A258_IO3_SET;
                set_reg(A258_IO3_DIR, value);

            }
        }
        else if(4 == ioNum) {
            value=get_reg(A258_IO4_REG);   //复用成gpio
            value &= (~0x1);
            set_reg(A258_IO4_REG, value);

            if(1 == global_data.enIo_4) {          //IO设置成输入
                value=get_reg(A258_IO4_DIR);
                value &= (~A258_IO4_SET);
                set_reg(A258_IO4_DIR, value);

            }
            else if(2 ==  global_data.enIo_4) {     //IO设置成输出
                value=get_reg(A258_IO4_DIR);
                value |= A258_IO4_SET;
                set_reg(A258_IO4_DIR, value);

            }
        }
        else if(5 == ioNum) {
            value=get_reg(A258_IO5_REG);   //复用成gpio
            value &= (~0x1);
            set_reg(A258_IO5_REG, value);

            if(1 == global_data.enIo_5) {          //IO设置成输入
                value=get_reg(A258_IO5_DIR);
                value &= (~A258_IO5_SET);
                set_reg(A258_IO5_DIR, value);

            }
            else if(2 ==  global_data.enIo_5) {     //IO设置成输出
                value=get_reg(A258_IO5_DIR);
                value |= A258_IO5_SET;
                set_reg(A258_IO5_DIR, value);

            }
        }
        else if(6 == ioNum) {
            value=get_reg(A258_IO6_REG);   //复用成gpio
            value &= (~0x1);
            set_reg(A258_IO6_REG, value);

            if(1 == global_data.enIo_6) {          //IO设置成输入
                value=get_reg(A258_IO6_DIR);
                value &= (~A258_IO6_SET);
                set_reg(A258_IO6_DIR, value);

            }
            else if(2 ==  global_data.enIo_6) {     //IO设置成输出
                value=get_reg(A258_IO6_DIR);
                value |= A258_IO6_SET;
                set_reg(A258_IO6_DIR, value);

            }
        }
        else if(7 == ioNum) {
            value=get_reg(A258_IO7_REG);   //复用成gpio
            value &= (~0x1);
            set_reg(A258_IO7_REG, value);

            if(1 == global_data.enIo_7) {          //IO设置成输入
                value=get_reg(A258_IO7_DIR);
                value &= (~A258_IO7_SET);
                set_reg(A258_IO7_DIR, value);

            }
            else if(2 ==  global_data.enIo_7) {     //IO设置成输出
                value=get_reg(A258_IO7_DIR);
                value |= A258_IO7_SET;
                set_reg(A258_IO7_DIR, value);

            }
        }
    }



#ifdef __7000__
    set_reg(GPIO13_5MUX,0x0);

    get_reg(GPIO13_5DIR);
    value |= GPIO13_5SET;
    set_reg(GPIO13_5DIR,value);


    if(1 == global_data.enIo) {         //IO作为输入
        get_reg(GPIO13_5DATA);
        value &= (~GPIO13_5SET);
        set_reg(GPIO13_5DATA,value);

        get_reg(GPIO0_7DIR);
        value &= (~GPIO0_7SET);
        set_reg(GPIO0_7DIR,value);
    }
    else if(2 == global_data.enIo) {    //IO作为输出
        get_reg(GPIO13_5DATA);
        value |= GPIO13_5SET;
        set_reg(GPIO13_5DATA,value);

        get_reg(GPIO0_7DIR);
        value |= GPIO0_7SET;
        set_reg(GPIO0_7DIR,value);
    }
    else {
        COMMON_PRT("io en value error\n");
        return false;
    }

#endif
//zhuidao
//    if(0 == ioNum) {
//        socketIO = createUdp(global_data.svIOport);
//        FD_SET(socketIO,&readFd);
//        if(socketIO >= maxFd)
//            maxFd = socketIO + 1;
//    }

//    if(1 == ioNum) {
//        socketIO = createUdp(global_data.svInfraredport);
//        FD_SET(socketIO,&readFd);
//        if(socketIO >= maxFd)
//            maxFd = socketIO + 1;
//    }
//    if(-1==socketIO_A258)
//    {
//        socketIO_A258 = createUdp(global_data.svIOport_A258);
//    }
//    return true;
}



//bool io::rcvData2IO()
//{
//    char tmpData[128] = {0};
//    char ioEcho[8] = {0};
//    int rcvLen = 0, sendLen = 0;
//    socklen_t addrLen = 0;
//    uint value;
//    memset(tmpData,0,sizeof(tmpData));

//    addrLen = sizeof(struct sockaddr_in);


//    rcvLen = recvfrom(socketIO,tmpData,MAXDATA,0,(struct sockaddr*)(&ctrlAddr),&addrLen);
//    if(rcvLen > 0) {
//        printf("rcv io data:%s\n",tmpData);
//        if(!strncmp(tmpData,"IO_0=1",strlen("IO_0=1"))) {
//            io0Str = 1;
//            IOSet(io0Str,0);
//            global_data.io0St = 1;
//        }
//        else if(!strncmp(tmpData,"IO_1=1",strlen("IO_1=1"))) {
//            io1Str = 1;
//            IOSet(io1Str,1);
//            global_data.io1St = 1;
//        }
//        else if(!strncmp(tmpData,"IO_2=1",strlen("IO_2=1"))) {
//           // io2Str = 1;
//           // IOSet(io2Str,2);
//           // global_data.io2St = 1;
//            IOSet(1,2);
//        }
//        else if(!strncmp(tmpData,"IO_3=1",strlen("IO_3=1"))) {
//            IOSet(1,3);
//        }
//        else if(!strncmp(tmpData,"IO_4=1",strlen("IO_4=1"))) {  //增加到8个IO
//            IOSet(1,4);
//        }
//         else if(!strncmp(tmpData,"IO_5=1",strlen("IO_5=1"))) {
//            IOSet(1,5);
//        }
//         else if(!strncmp(tmpData,"IO_6=1",strlen("IO_6=1"))) {
//            IOSet(1,6);
//        }
//         else if(!strncmp(tmpData,"IO_7=1",strlen("IO_7=1"))) {
//            IOSet(1,7);
//        }
//        else if(!strncmp(tmpData,"IO_0=0",strlen("IO_0=0"))) {
//            io0Str = 0;
//            IOSet(io0Str,0);
//            global_data.io0St = 0;
//        }
//        else if(!strncmp(tmpData,"IO_1=0",strlen("IO_1=0"))) {
//            io1Str = 0;
//            IOSet(io1Str,1);
//            global_data.io1St = 0;
//        }
//        else if(!strncmp(tmpData,"IO_2=0",strlen("IO_2=0"))) {
//            io2Str = 0;
//            IOSet(io2Str,2);
//            global_data.io2St = 0;
//        }
//        else if(!strncmp(tmpData,"IO_3=0",strlen("IO_3=0"))) {
//            IOSet(0,3);
//        }
//         else if(!strncmp(tmpData,"IO_4=0",strlen("IO_4=0"))) {
//            IOSet(0,4);
//        }
//         else if(!strncmp(tmpData,"IO_5=0",strlen("IO_5=0"))) {
//            IOSet(0,5);
//        }
//         else if(!strncmp(tmpData,"IO_6=0",strlen("IO_6=0"))) {
//            IOSet(0,6);
//        }
//        else if(!strncmp(tmpData,"IO_7=0",strlen("IO_7=0"))) {
//            IOSet(0,7);
//        }
//        else if(!strncmp(tmpData,"getIO_0",strlen("getIO_0"))) {   //？这里需要添加吗？
//            if(global_data.enIo_0 == 1) {
//#ifdef __7000_NJ__
//                if(hd_ver == NJ_21D) {
//                    get_reg(GPIO0_3DATA,&value);
//                    if(value & GPIO0_3SET) {
//                        io0Str = 1;
//                        global_data.io0St = 1;
//                    }
//                    else {
//                        io0Str = 0;
//                        global_data.io0St = 0;
//                    }
//                }
//                else if(hd_ver == PTN_21A) {
//                    get_reg(IO_0_10_2_DATA, &value);
//                    if(value & IO_0_10_2SET) {
//                        io0Str = 1;
//                        global_data.io0St = 1;
//                    }
//                    else {
//                        io0Str = 0;
//                        global_data.io0St = 0;
//                    }
//                }
//                else {
//                    get_reg(GPIO0_6DATA,&value);
//                    if(value & GPIO0_6SET) {
//                        io0Str = 1;
//                        global_data.io0St = 1;
//                    }
//                    else {
//                        io0Str = 0;
//                        global_data.io0St = 0;
//                    }
//                }


//#endif

//#ifdef __GPIO1_5__
//                if(ir_nj_700k) {
//                    get_reg(GPIO20_5DATA,&value);
//                    if(value & GPIO20_5SET) {
//                        io0Str = 1;
//                        global_data.io0St = 1;
//                    }
//                    else {
//                        io0Str = 0;
//                        global_data.io0St = 0;
//                    }
//                }
//                else {
//                    if(hd_ver == GZ_700S) {
//                        get_reg(GPIO11_1DATA, &value);
//                        if(value & GPIO11_1SET) {
//                            io0Str = 1;
//                            global_data.io0St = 1;
//                        }
//                        else {
//                            io0Str = 0;
//                            global_data.io0St = 0;
//                        }
//                    }
//                    else {
//                        get_reg(GPIO1_7DATA,&value);
//                        if(value & GPIO1_7SET) {
//                            io0Str = 1;
//                            global_data.io0St = 1;
//                        }
//                        else {
//                            io0Str = 0;
//                            global_data.io0St = 0;
//                        }
//                    }

//                }

//#endif
//                memset(ioEcho,0,sizeof(ioEcho));
//                printf("now io state 0 :%d\n",io0Str);
//                if(io0Str) {
//                    strcpy(ioEcho,"IO_0=ON");
//                }
//                else {
//                    strcpy(ioEcho,"IO_0=OFF");
//                }

//                if((global_data.ctlIO0port != 0) && (strlen(global_data.ctlip) >= 7)) {
//                    ctrlAddr.sin_family = AF_INET;
//                    ctrlAddr.sin_port = htons(global_data.ctlIO0port);
//                    ctrlAddr.sin_addr.s_addr = inet_addr(global_data.ctlip);
//                    sendLen = sendto(socketIO,ioEcho,strlen(ioEcho),0,(struct sockaddr*)&ctrlAddr,sizeof(struct sockaddr_in));
//                    if(sendLen > 0)
//                        return true;
//                    else {
//                        COMMON_PRT("send dev io str to ctrl error\n");
//                        return false;
//                    }
//                }
//                else {
//                    sendLen = sendto(socketIO,ioEcho,strlen(ioEcho),0,(struct sockaddr*)&ctrlAddr,sizeof(struct sockaddr_in));
//                    if(sendLen > 0 ) {
//                        COMMON_PRT("UMP not set ctrl IP PORT and echo\n");
//                        return true;
//                    }
//                    else {
//                        COMMON_PRT("send dev io str to echo error\n");
//                        return false;
//                    }

//                }
//            }
//        }
//        else if(!strncmp(tmpData,"getIO_1",strlen("getIO_1"))) {
//            if(global_data.enIo_1 == 1) {
//                if((hd_ver == NJ_7000) || (hd_ver == PTN_7000)) {
//                    get_reg(GPIO0_3DATA);
//                    if(value & GPIO0_3SET) {
//                        io1Str = 1;
//                        global_data.io1St = 1;
//                    }
//                    else {
//                        io1Str = 0;
//                        global_data.io1St = 0;
//                    }
//                }
//                else if(hd_ver == PTN_21A) {
//                    get_reg(IO_1_5_6_DATA);
//                    if(value & IO_1_5_6_SET){
//                        io1Str = 1;
//                        global_data.io1St = 1;
//                    }
//                    else {
//                        io1Str = 0;
//                        global_data.io1St = 0;
//                    }
//                }
//                else if(hd_ver ==  NJ_21D) {
//                    get_reg(GPIO0_5DATA);
//                    if(value & GPIO0_5SET) {
//                        io1Str = 1;
//                        global_data.io1St = 1;
//                    }
//                    else {
//                        io1Str = 0;
//                        global_data.io1St = 0;
//                    }
//                }
//                else if((hd_ver == NJ_700K) || (hd_ver ==GZ_700K) || (hd_ver == GZ_8000)) {
//                    if(ir_nj_700k) {
//                        get_reg(GPIO15_6DATA);
//                        if(value & GPIO15_6SET) {
//                            io1Str = 1;
//                            global_data.io1St = 1;
//                        }
//                        else {
//                            io1Str = 0;
//                            global_data.io1St = 0;
//                        }


//                        get_reg(GPIO1_7DATA);
//                        if(value & GPIO1_7SET) {
//                            io1Str = 1;
//                            global_data.io1St = 1;
//                        }
//                        else {
//                            io1Str = 0;
//                            global_data.io1St = 0;
//                        }
//                    }
//                    else {
//                        get_reg(GPIO20_5DATA);
//                        if(value & GPIO20_5SET) {
//                            io1Str = 1;
//                            global_data.io1St = 1;
//                        }
//                        else {
//                            io1Str = 0;
//                            global_data.io1St = 0;
//                        }
//                    }
//                }
//                else if(hd_ver == GZ_700S) {
//                    get_reg(GPIO14_7DATA);
//                    if(value & GPIO14_7SET) {
//                        io1Str = 1;
//                        global_data.io1St = 1;
//                    }
//                    else {
//                        io1Str = 0;
//                        global_data.io1St = 0;
//                    }

//                }

//                printf("now io state 1 :%d\n",io1Str);

//                memset(ioEcho,0,sizeof(ioEcho));
//                if(io1Str) {
//                    strcpy(ioEcho,"IO_1=ON");
//                }
//                else {
//                    strcpy(ioEcho,"IO_1=OFF");
//                }

//                if((global_data.ctlIO1port != 0) && (strlen(global_data.ctlip) >= 7)) {
//                    ctrlAddr.sin_family = AF_INET;
//                    ctrlAddr.sin_port = htons(global_data.ctlIO1port);
//                    ctrlAddr.sin_addr.s_addr = inet_addr(global_data.ctlip);
//                    sendLen = sendto(socketIO,ioEcho,strlen(ioEcho),0,(struct sockaddr*)&ctrlAddr,sizeof(struct sockaddr_in));
//                    if(sendLen > 0)
//                        return true;
//                    else {
//                        COMMON_PRT("send dev io str to ctrl error\n");
//                        return false;
//                    }
//                }
//                else {
//                    sendLen = sendto(socketIO,ioEcho,strlen(ioEcho),0,(struct sockaddr*)&ctrlAddr,sizeof(struct sockaddr_in));
//                    if(sendLen > 0 ) {
//                        COMMON_PRT("UMP not set ctrl IP PORT and echo\n");
//                        return true;
//                    }
//                    else {
//                        COMMON_PRT("send dev io str to echo error\n");
//                        return false;
//                    }

//                }
//            }
//        }


//#ifdef __7000__
//                get_reg(GPIO0_7DATA,&value);
//                if(value & GPIO0_7SET) {
//                    ioStr = 1;
//                    global_data.ioSt = 1;
//                }
//                else {
//                    ioStr = 0;
//                    global_data.ioSt = 0;
//                }
//#endif


//        else {
//            COMMON_PRT("ctrl send data error:%s\n",tmpData);
//        }

//    }
//    else {
//        COMMON_PRT("not get rcv io data error\n");
//        return false;
//    }

//    auto thisini = Singleton<SoftwareConfig>::getInstance();
//    thisini->SetConfig(SoftwareConfig::kIO0st,global_data.io0St);
//    thisini->SetConfig(SoftwareConfig::kIO1st,global_data.io1St);
//    thisini->SaveConfig();
//}


bool io::IOSet(int set,int ioNum)
{
    uint value;
    if(set == 1) {
        if((hd_ver == NJ_7000) || (hd_ver == PTN_7000)) {
            if(0 == ioNum) {
                get_reg(GPIO0_6DATA);
                value |= GPIO0_6SET;
                set_reg(GPIO0_6DATA,value);
            }
            else if(1 == ioNum) {
                get_reg(GPIO0_3DATA);
                value |= GPIO0_3SET;
                set_reg(GPIO0_3DATA,value);
            }
        }
        else if(hd_ver == PTN_21A) {
            if(0 == ioNum) {
                get_reg(IO_0_10_2_DATA);
                value |= IO_0_10_2SET;
                set_reg(IO_0_10_2_DATA, value);
            }
            else if(1 == ioNum) {
                get_reg(IO_1_5_6_DATA);
                value |= IO_1_5_6_SET;
                set_reg(IO_1_5_6_DATA, value);
            }
        }
        else if(NJ_21D == hd_ver) {
            if(0 == ioNum) {
                get_reg(GPIO0_3DATA);
                value |= GPIO0_3SET;
                set_reg(GPIO0_3DATA,value);
            }
            else if(1 == ioNum) {
                get_reg(GPIO0_5DATA);
                value |= GPIO0_5SET;
                set_reg(GPIO0_5DATA,value);

            }
        }
        else if((hd_ver == NJ_700K) || (hd_ver == GZ_700K) || (hd_ver == GZ_8000)) {
            if(0 == ioNum) {
                if(ir_nj_700k) {
                    get_reg(GPIO20_5DATA);
                    value |= GPIO20_5SET;
                    set_reg(GPIO20_5DATA,value);
                }
                else {
                    get_reg(GPIO1_7DATA);
                    value |= GPIO1_7SET;
                    set_reg(GPIO1_7DATA,value);
                }
            }
            else if(1 == ioNum) {
                if(ir_nj_700k) {
                    get_reg(GPIO15_6DATA);
                    value |= GPIO15_6SET;
                    set_reg(GPIO15_6DATA,value);

                    get_reg(GPIO1_7DATA);
                    value |= GPIO1_7SET;
                    set_reg(GPIO1_7DATA,value);
                }
                else {
                    get_reg(GPIO20_5DATA);
                    value |= GPIO20_5SET;
                    set_reg(GPIO20_5DATA,value);
                }
            }
        }
        else if(hd_ver == GZ_700S) {
            if(0 == ioNum) {
                get_reg(GPIO11_1DATA);
                value |= GPIO11_1SET;
                set_reg(GPIO11_1DATA,value);

            }
            else if(1 == ioNum) {
                get_reg(GPIO14_7DATA);
                value |= GPIO14_7SET;
                set_reg(GPIO14_7DATA,value);
            }

        }
        else if(hd_ver == NJ_31DV200) {
            if(0 == ioNum) {
                get_reg(IO_0_DATA_DATA);
                value |= IO_0_DATA_SET;
                set_reg(IO_0_DATA_DATA, value);
                printf("set io0 up\n");
            }
            else if(1 == ioNum) {
                get_reg(IO_1_DATA_DATA);
                value |= IO_1_DATA_SET;
                set_reg(IO_1_DATA_DATA, value);
            }
            else if(2 == ioNum) {
                get_reg(IO_2_DATA_DATA);
                value |= IO_2_DATA_SET;
                set_reg(IO_2_DATA_DATA, value);

            }
            else if(3 == ioNum) {
                get_reg(IO_3_DATA_DATA);
                value |= IO_3_DATA_SET;
                set_reg(IO_3_DATA_DATA, value);
            }
        }
        else if(hd_ver == NJ_A258) {    //添加A258 IO拉高拉低
            if(0 == ioNum) {
                value=get_reg(A258_IO0_DATA);
                value |= A258_IO0_SET;
                set_reg(A258_IO0_DATA, value);
                printf("set io0_A258 up\n");
            }
            else if(1 == ioNum) {
                value=get_reg(A258_IO1_DATA);
                value |= A258_IO1_SET;
                set_reg(A258_IO1_DATA, value);
                printf("set io1_A258 up\n");
            }
            else if(2 == ioNum) {
                value=get_reg(A258_IO2_DATA);
                value |= A258_IO2_SET;
                set_reg(A258_IO2_DATA, value);
                printf("set io2 up\n");

            }
            else if(3 == ioNum) {
                value=get_reg(A258_IO3_DATA);
                value |= A258_IO3_SET;
                set_reg(A258_IO3_DATA, value);
                printf("set io3 up\n");
            }
             else if(4 == ioNum) {
                value=get_reg(A258_IO4_DATA);
                value |= A258_IO4_SET;
                set_reg(A258_IO4_DATA, value);
                printf("set io4 up\n");
            }
            else if(5 == ioNum) {
                value=get_reg(A258_IO5_DATA);
                value |= A258_IO5_SET;
                set_reg(A258_IO5_DATA, value);
                printf("set io5 up\n");
            }
             else if(6 == ioNum) {
                value=get_reg(A258_IO6_DATA);
                value |= A258_IO6_SET;
                set_reg(A258_IO6_DATA, value);
                printf("set io6 up\n");
            }
             else if(7 == ioNum) {
                value=get_reg(A258_IO7_DATA);
                value |= A258_IO7_SET;
                set_reg(A258_IO7_DATA, value);
                printf("set io7 up\n");
            }                                            
        }
#ifdef __7000__
        get_reg(GPIO0_7DATA,&value);
        value |= GPIO0_7SET;
        set_reg(GPIO0_7DATA,value);
#endif

    }
    else if(set == 0) {
        if((hd_ver == NJ_7000) || (hd_ver == PTN_7000)) {
            if(0 == ioNum) {
                get_reg(GPIO0_6DATA);
                value &= (~GPIO0_6SET);
                set_reg(GPIO0_6DATA,value);
            }
            else if(1 == ioNum) {
                get_reg(GPIO0_3DATA);
                value &= (~GPIO0_3SET);
                set_reg(GPIO0_3DATA,value);
            }
        }
        else if(hd_ver == PTN_21A) {
            if(0 == ioNum) {
                get_reg(IO_0_10_2_DATA);
                value &= (~IO_0_10_2SET);
                set_reg(IO_0_10_2_DATA, value);
            }
            else if(1 == ioNum) {
                get_reg(IO_1_5_6_DATA);
                value &= (~IO_1_5_6_SET);
                set_reg(IO_1_5_6_DATA, value);
            }
        }
        else if(hd_ver == NJ_21D) {
            if(0 ==  ioNum) {
                get_reg(GPIO0_3DATA);
                value &= (~GPIO0_3SET);
                set_reg(GPIO0_3DATA,value);
            }
            else {
                get_reg(GPIO0_5DATA);
                value &= (~GPIO0_5SET);
                set_reg(GPIO0_5DATA,value);
            }
        }
        else if((hd_ver == NJ_700K) || (hd_ver == GZ_700K) || (hd_ver == GZ_8000)) {
            if(0 == ioNum) {
                if(ir_nj_700k) {
                    get_reg(GPIO20_5DATA);
                    value &= (~GPIO20_5SET);
                    set_reg(GPIO20_5DATA,value);
                }
                else {
                    get_reg(GPIO1_7DATA);
                    value &= (~GPIO1_7SET);
                    set_reg(GPIO1_7DATA,value);
                }
            }
            else if(1 == ioNum) {
                if(ir_nj_700k) {
                    get_reg(GPIO15_6DATA);
                    value &= (~GPIO15_6SET);
                    set_reg(GPIO15_6DATA,value);

                    get_reg(GPIO1_7DATA);
                    value &= (~GPIO1_7SET);
                    set_reg(GPIO1_7DATA,value);
                }
                else {
                    get_reg(GPIO20_5DATA);
                    value &= (~GPIO20_5SET);
                    set_reg(GPIO20_5DATA,value);
                }
            }
        }
        else if(hd_ver == GZ_700S) {
            if(0 ==  ioNum) {
                get_reg(GPIO11_1DATA);
                value &= (~GPIO11_1SET);
                set_reg(GPIO11_1DATA,value);
            }
            else {
                get_reg(GPIO14_7DATA);
                value &= (~GPIO14_7SET);
                set_reg(GPIO14_7DATA,value);
            }
        }
        else if(hd_ver == NJ_31DV200) {
            if(0 == ioNum) {
                get_reg(IO_0_DATA_DATA);
                value &= (~IO_0_DATA_SET);
                set_reg(IO_0_DATA_DATA, value);
            }
            else if(1 == ioNum) {
                get_reg(IO_1_DATA_DATA);
                value &= (~IO_1_DATA_SET);
                set_reg(IO_1_DATA_DATA, value);
            }
            else if(2 == ioNum) {
                get_reg(IO_2_DATA_DATA);
                value &= (~IO_2_DATA_SET);
                set_reg(IO_2_DATA_DATA, value);

            }
            else if(3 == ioNum) {
                get_reg(IO_3_DATA_DATA);
                value &= (~IO_3_DATA_SET);
                set_reg(IO_3_DATA_DATA, value);
            }
        }
        else if(hd_ver == NJ_A258) {
            if(0 == ioNum) {
                value=get_reg(A258_IO0_DATA);
                value &= (~A258_IO0_SET);
                set_reg(A258_IO0_DATA, value);
                printf("set io0_A258 down\n");
            }
            else if(1 == ioNum) {
                value=get_reg(A258_IO1_DATA);
                value &= (~A258_IO1_SET);
                set_reg(A258_IO1_DATA, value);
                printf("set io1_A258 down\n");
            }
            else if(2 == ioNum) {
                value=get_reg(A258_IO2_DATA);
                value &= (~A258_IO2_SET);
                set_reg(A258_IO2_DATA, value);
                printf("set io2_A258 down\n");
            }
            else if(3 == ioNum) {
                value=get_reg(A258_IO3_DATA);
                value &= (~A258_IO3_SET);
                set_reg(A258_IO3_DATA, value);
                printf("set io3_A258 down\n");
            }
             else if(4 == ioNum) {
                value=get_reg(A258_IO4_DATA);
                value &= (~A258_IO4_SET);
                set_reg(A258_IO4_DATA, value);
                printf("set io4_A258 down\n");
            }
            else if(5 == ioNum) {
                value=get_reg(A258_IO5_DATA);
                value &= (~A258_IO5_SET);
                set_reg(A258_IO5_DATA, value);
                printf("set io5_A258 down\n");
            }
             else if(6 == ioNum) {
                value=get_reg(A258_IO6_DATA);
                value &= (~A258_IO6_SET);
                set_reg(A258_IO6_DATA, value);
                printf("set io6_A258 down\n");
            }
             else if(7 == ioNum) {
                value=get_reg(A258_IO7_DATA);
                value &= (~A258_IO7_SET);
                set_reg(A258_IO7_DATA, value);
                printf("set io7_A258 down\n");
            }                                            
        }        

#ifdef __7000__
        get_reg(GPIO0_7DATA,&value);
        value &= (~GPIO0_7SET);
        set_reg(GPIO0_7DATA,value);
#endif
    }
    else {
        COMMON_PRT("io set value error\n");
        return  false;
    }

    return true;
}


/*
 * 配合UMP设置，时时生效
 * 全局参数在外部修改，保存
 * */

bool io::UMPsetParam(bool uartFlag,bool ctrlPortFlag,int svPortFlag)
{

    if(ctrlPortFlag) {
        COMMON_PRT("UMP set io ctrl port Param\n");
    }

    if(svPortFlag) {
//        FD_CLR(socketIO_A258,&readFd);
//        close(socketIO_A258);
//        sleep(1);
//        if(1 == svPortFlag)
//            socketIO = createUdp(global_data.svIOport);         //IO0
//        else if(2 == svPortFlag)
//            socketIO = createUdp(global_data.svInfraredport);    //IO1
//        FD_SET(socketIO,&readFd);
//        if(socketIO >= maxFd)
//            maxFd = socketIO + 1;
    }

    if(uartFlag) {

    }
    return true;
}
