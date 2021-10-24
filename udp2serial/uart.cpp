#include "uart.h"




int open_port(char *comport,int *pdevfd) //通过参数，打开相应的串口
{
    if (!strncmp(comport,"com0",4))
    {	*pdevfd = open( "/dev/ttyAMA0", O_RDWR|O_NOCTTY|O_NDELAY);
        if (-1 == *pdevfd){
            perror("Can't Open Serial UART0 Port");
            return 0;
        }
        else
            printf("open port 0\n");
    }
    else if(!strncmp(comport,"com1",4))
    {	*pdevfd = open( "/dev/ttyAMA1", O_RDWR|O_NOCTTY|O_NDELAY);
        if (-1 == *pdevfd){
            perror("Can't Open Serial UART1 Port");
            return 0;
        }
        else
            printf("open port 1\n");
    }
    else if (!strncmp(comport,"com2",4))
    {
        *pdevfd = open( "/dev/ttyAMA2", O_RDWR|O_NOCTTY|O_NDELAY);
        if (-1 == *pdevfd){
            perror("Can't Open Serial UART2 Port");
            return 0;
        }
        else
            printf("open port 2\n");
    }
    else if (!strncmp(comport,"com3",4))
    {
        system("himm 0x120F0218     0x1");
        system("himm 0x120F021C     0x1");
        *pdevfd = open( "/dev/ttyAMA3", O_RDWR|O_NOCTTY|O_NDELAY);
        if (-1 == *pdevfd){
            perror("Can't Open Serial UART3 Port");
            return 0;
        }
        else
            printf("open port 3\n");
    }
    else if (!strncmp(comport,"com4",4))
    {
        *pdevfd = open( "/dev/ttyAMA4", O_RDWR|O_NOCTTY|O_NDELAY);
        if (-1 == *pdevfd){
            perror("Can't Open Serial UART4 Port");
            return 0;
        }
        else
            printf("open port 4\n");
    }
    else if(!strncmp(comport,"usb0",4)) {
        *pdevfd = open( "/dev/ttyUSB0", O_RDWR|O_NOCTTY|O_NDELAY);
        if (-1 == *pdevfd){
            perror("Can't Open Serial USB0 Port");
            return 0;
        }
        else
            printf("open USB 0\n");
    }
    else if(!strncmp(comport,"usb1",4)) {
        *pdevfd = open( "/dev/ttyUSB1", O_RDWR|O_NOCTTY|O_NDELAY);
        if (-1 == *pdevfd){
            perror("Can't Open Serial USB1 Port");
            return 0;
        }
        else
            printf("open USB 1\n");
    }
    else if(!strncmp(comport,"usb2",4)) {
        *pdevfd = open( "/dev/ttyUSB2", O_RDWR|O_NOCTTY|O_NDELAY);
        if (-1 == *pdevfd){
            perror("Can't Open Serial USB2 Port");
            return 0;
        }
        else
            printf("open USB 2\n");
    }
    else if(!strncmp(comport,"usb3",4)) {
        *pdevfd = open( "/dev/ttyUSB3", O_RDWR|O_NOCTTY|O_NDELAY);
        if (-1 == *pdevfd){
            perror("Can't Open Serial USB3 Port");
            return 0;
        }
        else
            printf("open USB 3\n");
    }
    else if(!strncmp(comport,"usb4",4)) {
        *pdevfd = open( "/dev/ttyUSB4", O_RDWR|O_NOCTTY|O_NDELAY);
        if (-1 == *pdevfd){
            perror("Can't Open Serial USB4 Port");
            return 0;
        }
        else
            printf("open USB 4\n");
    }
//补充A258中控板232,485设备

    else if(!strncmp(comport,"2320",4)) {
        *pdevfd = open( "/dev/ttyWCHUSB0", O_RDWR|O_NOCTTY|O_NDELAY);
        if (-1 == *pdevfd){
            perror("Can't Open RS232 0 Port");
            return 0;
        }
        else
            printf("open RS232 0\n");
    }
    else if(!strncmp(comport,"2321",4)) {
        *pdevfd = open( "/dev/ttyWCHUSB1", O_RDWR|O_NOCTTY|O_NDELAY);
        if (-1 == *pdevfd){
            perror("Can't Open RS232 1 Port");
            return 0;
        }
        else
            printf("open RS232 1\n");
    }
    else if(!strncmp(comport,"2322",4)) {
        *pdevfd = open( "/dev/ttyWCHUSB2", O_RDWR|O_NOCTTY|O_NDELAY);
        if (-1 == *pdevfd){
            perror("Can't Open RS232 2 Port");
            return 0;
        }
        else
            printf("open RS232 2\n");
    }
        else if(!strncmp(comport,"2323",4)) {
        *pdevfd = open( "/dev/ttyWCHUSB3", O_RDWR|O_NOCTTY|O_NDELAY);
        if (-1 == *pdevfd){
            perror("Can't Open RS232 3 Port");
            return 0;
        }
        else
            printf("open RS232 3\n");
    }
    else if(!strncmp(comport,"4850",4)) {
        *pdevfd = open( "/dev/ttyWCHUSB7", O_RDWR|O_NOCTTY|O_NDELAY);
        if (-1 == *pdevfd){
            perror("Can't Open RS485 0 Port");
            return 0;
        }
        else
            printf("open RS485 0\n");
    }
        else if(!strncmp(comport,"4851",4)) {
        *pdevfd = open( "/dev/ttyWCHUSB6", O_RDWR|O_NOCTTY|O_NDELAY);
        if (-1 == *pdevfd){
            perror("Can't Open RS485 1 Port");
            return 0;
        }
        else
            printf("open RS485 1\n");
    }
    else if(!strncmp(comport,"4852",4)) {
        *pdevfd = open( "/dev/ttyWCHUSB5", O_RDWR|O_NOCTTY|O_NDELAY);
        if (-1 == *pdevfd){
            perror("Can't Open RS485 2 Port");
            return 0;
        }
        else
            printf("open RS485 2\n");
    }
    else if(!strncmp(comport,"4853",4)) {
        *pdevfd = open( "/dev/ttyWCHUSB4", O_RDWR|O_NOCTTY|O_NDELAY);
        if (-1 == *pdevfd){
            perror("Can't Open RS485 3 Port");
            return 0;
        }
        else
            printf("open RS485 3\n");
    }

    if(fcntl(*pdevfd, F_SETFL, 0)<0){
        perror("fcntl failed!");
        return 0;
    }
    if(isatty(STDIN_FILENO)==0){
        perror("standard input is not a terminal device");
//        return 0;
    }

    return 1;
}

int set_optBak(int nSpeed, int nBits, int nStop, char nEvent, int nVtime, int nVmin,int *setfd)  //串口设置
{
    struct termios newtio, oldtio;
    if  ( tcgetattr( *setfd, &oldtio)  !=  0) {
        perror("SetupSerial");
        return 0;
    }
    bzero( &newtio, sizeof( struct termios ) );
    newtio.c_cflag  |=  CLOCAL | CREAD;
    newtio.c_cflag &= ~CSIZE;

    //	newtio.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    //	newtio.c_oflag &= ~OPOST;
    switch( nBits )
    {
        case 7:
            newtio.c_cflag |= CS7;
            break;
        case 8:
            newtio.c_cflag |= CS8;
            break;
    }

    switch( nEvent )
    {
        case 'O':
            newtio.c_cflag |= PARENB;
            newtio.c_cflag |= PARODD;
            newtio.c_iflag |= (INPCK | ISTRIP);
            break;
        case 'E':
            newtio.c_iflag |= (INPCK | ISTRIP);
            newtio.c_cflag |= PARENB;
            newtio.c_cflag &= ~PARODD;
            break;
        case 'N':
            newtio.c_iflag &= ~INPCK ;
            newtio.c_cflag &= ~PARENB;
            break;
    }

    switch( nSpeed )
    {
        case 2400:
            cfsetispeed(&newtio, B2400);
            cfsetospeed(&newtio, B2400);
            break;
        case 4800:
            cfsetispeed(&newtio, B4800);
            cfsetospeed(&newtio, B4800);
            break;
        case 9600:
            cfsetispeed(&newtio, B9600);
            cfsetospeed(&newtio, B9600);
            break;
        case 38400:
            cfsetispeed(&newtio, B38400);
            cfsetospeed(&newtio, B38400);
            break;
        case 115200:
            cfsetispeed(&newtio, B115200);
            cfsetospeed(&newtio, B115200);
            break;
        case 19200:
            cfsetispeed(&newtio, B19200);
            cfsetospeed(&newtio, B19200);
            break;

        default:
            break;
    }

    if( nStop == 1 )
        newtio.c_cflag &=  ~CSTOPB;
    else if ( nStop == 2 )
        newtio.c_cflag |=  CSTOPB;

    /*
    TIME和MIN值
    这两个值只用于非标准模式,两者结合共同控制对输入的读取方式,还能控制在一个程序试图与一个终端关联的文件描述符时将发生的情况
    MIN = 0, TIME = 0时:read立即返回,如果有待处理的字符,它们就会被返回,如果没有,read调用返回0,且不读取任何字符
    MIN = 0, TIME > 0时:有字符处理或经过TIME个0.1秒后返回
    MIN > 0, TIME = 0时:read一直等待,直到有ＭＩＮ个字符可以读取,返回值是字符的数量.到达文件尾时返回0
    MIN > 0, TIME > 0时:read调用时,它会等待接收一个字符.在接收到第一个字符及其后续的每个字符后,启用一个字符间隔定时器.当有ＭＩＮ个字符可读或两字符间的时间间隔超进TIME个0.1秒时,read返回
    通过设置ＭＩＮ和ＴＩＭＥ值,我们可以逐个字符地对输入进行处理
    */
    newtio.c_cc[VTIME]  = nVtime;
    newtio.c_cc[VMIN] = nVmin;
    tcflush(*setfd,TCIFLUSH);
    if((tcsetattr(*setfd, TCSANOW, &newtio))!=0)
    {
        perror("com set tcsetattr error");
        return 0;
    }
    return 1;
}

bool GetNetworkInfo(const char *ethname, IF_CONFIG id, char *getch)
{
    int fd;
    int interfaceNum = 0;
    bool b_geterror = false;
    struct ifreq ifbuf[16];
    struct ifconf ifc;
    struct ifreq ifrcopy;
    char mac[32] = {0};
    char ip[32] = {0};
    char broadAddr[32] = {0};
    char subnetMask[32] = {0};

    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket create error");
        close(fd);
        return false;
    }

    ifc.ifc_len = sizeof(ifbuf);
    ifc.ifc_ifcu.ifcu_buf = (caddr_t)ifbuf;
    if (ioctl(fd, SIOCGIFCONF, (char *)&ifc))
    {
        perror("ioctl error: SIOCGIFCONF");
        close(fd);
        return false;
    }

    interfaceNum = ifc.ifc_len / sizeof(struct ifreq);
    //printf("interface num = %d\n", interfaceNum);

    while (interfaceNum-- > 0)
    {
        //printf("ndevice name: %s\n", ifbuf[interfaceNum].ifr_name);
        if(!strcmp(ifbuf[interfaceNum].ifr_name, ethname)){ //只获取指定的网卡参数
            //ignore the interface that not up or not runing
            ifrcopy = ifbuf[interfaceNum];
            if (ioctl(fd, SIOCGIFFLAGS, &ifrcopy))
            {
                perror("ioctl warning: SIOCGIFFLAGS");
                continue;
            }

            //get the mac of this interface
            if (!ioctl(fd, SIOCGIFHWADDR, (char *)(&ifbuf[interfaceNum])))
            {
                memset(mac, 0, sizeof(mac));
                snprintf(mac, sizeof(mac), "%02x:%02x:%02x:%02x:%02x:%02x",
                         (unsigned char)ifbuf[interfaceNum].ifr_hwaddr.sa_data[0],
                        (unsigned char)ifbuf[interfaceNum].ifr_hwaddr.sa_data[1],
                        (unsigned char)ifbuf[interfaceNum].ifr_hwaddr.sa_data[2],

                        (unsigned char)ifbuf[interfaceNum].ifr_hwaddr.sa_data[3],
                        (unsigned char)ifbuf[interfaceNum].ifr_hwaddr.sa_data[4],
                        (unsigned char)ifbuf[interfaceNum].ifr_hwaddr.sa_data[5]);
                //printf("device mac: %s\n", mac);
            }
            else
            {
                perror("ioctl error: SIOCGIFHWADDR");
                close(fd);
                return false;
            }

            //get the IP of this interface
            if (!ioctl(fd, SIOCGIFADDR, (char *)&ifbuf[interfaceNum]))
            {
                snprintf(ip, sizeof(ip), "%s",
                         (char *)inet_ntoa(((struct sockaddr_in *)&(ifbuf[interfaceNum].ifr_addr))->sin_addr));
                //printf("device ip: %s\n", ip);
            }
            else
            {
                perror("ioctl error: SIOCGIFADDR");
                close(fd);
                return false;
            }

            //get the broad address of this interface
            if (!ioctl(fd, SIOCGIFBRDADDR, &ifbuf[interfaceNum]))
            {
                snprintf(broadAddr, sizeof(broadAddr), "%s",
                         (char *)inet_ntoa(((struct sockaddr_in *)&(ifbuf[interfaceNum].ifr_broadaddr))->sin_addr));
                //printf("device broadAddr: %s\n", broadAddr);
            }
            else
            {
                perror("ioctl error: SIOCGIFBRDADDR");
                close(fd);
                return false;
            }

            //get the subnet mask of this interface
            if (!ioctl(fd, SIOCGIFNETMASK, &ifbuf[interfaceNum]))
            {
                snprintf(subnetMask, sizeof(subnetMask), "%s",
                         (char *)inet_ntoa(((struct sockaddr_in *)&(ifbuf[interfaceNum].ifr_netmask))->sin_addr));
                //printf("device subnetMask: %s\n", subnetMask);
            }
            else
            {
                perror("ioctl error: SIOCGIFNETMASK");
                close(fd);
                return false;

            }
        }
    }

    switch (id) {
        case kHWADDR:
            sprintf(getch, "%s", mac);
            break;
        case kADDR:
            sprintf(getch, "%s", ip);
            break;
        case kBRDADDR:
            sprintf(getch, "%s", broadAddr);
            break;
        case kNETMASK:
            sprintf(getch, "%s", subnetMask);
            break;
        case kALL:
            sprintf(getch, "mac:%s@ip:%s@mask:%s@braod:%s", mac, ip, subnetMask, broadAddr);
            break;
        default:
            b_geterror = true;
            break;
    }

    close(fd);

    if(b_geterror)
        return false;

    return true;
}

string GetLocalIp()
{
    FILE *fp;
    int pos;
    char buff[2048] = "";
    //string str = "inet addr";
    string str = "kEth0Ip";


    while( 1 ){
        fp = NULL;

        // execute command get all video
        //if(NULL == (fp = popen("ifconfig eth0 | awk '/inet addr:/'","r"))){
        if(NULL == (fp = popen("cat /data/net.ini | awk '/kEth0Ip/'","r"))){
            pclose(fp);
            sleep(2);
            continue;
        }
        while (fgets(buff, sizeof(buff), fp) != NULL) {
            string data(buff);
            pos = 0;
            if((pos = data.find(str)) != string::npos){
                data = data.c_str() + pos + str.length() + 1;
                //str  = data.substr(0, data.find("B")-2);
                str  = data.substr(0, data.find("\n"));
                break;
            }
        }

        pclose(fp);
        return str;
    }
}


int createUdp(const uint k_serverport)
{
    struct sockaddr_in serveraddr_;
    serveraddr_.sin_family = AF_INET;
    serveraddr_.sin_addr.s_addr = htonl (INADDR_ANY);
    serveraddr_.sin_port = htons(k_serverport);
    int socket_;
    socket_ = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_ == -1) {
        perror("can't create socket file");
        return -1;
    }


    if (-1 == bind(socket_, (struct sockaddr *) &serveraddr_, sizeof(struct sockaddr))){
        perror("bind error");
    }

    return socket_;
}

/*
 * 功能：加入组播组
 * 输入：const char *kp_sendtoip： 指定组播地址ip
 *      const char *kp_ethname：  指定本地网卡
 * 返回：bool
 */
bool AddMemberShip(int socket_, const char *kp_sendtoip, const char *kp_ethname)
{
    int loop = 1;
    struct ip_mreq multicastreq_;
    if( (setsockopt(socket_,IPPROTO_IP, IP_MULTICAST_LOOP, &loop, sizeof(loop))) < 0 ){
        perror("setsockopt():IP_MULTICAST_LOOP");
        return false;
    }

    memset(&multicastreq_, 0, sizeof(struct ip_mreq));
    multicastreq_.imr_multiaddr.s_addr = inet_addr(kp_sendtoip); //组播源地址
    if(kp_ethname != nullptr){
        char localip[32] = {0};
        if( GetNetworkInfo(kp_ethname, kADDR, localip) )
            multicastreq_.imr_interface.s_addr = inet_addr(localip);//本地地址
        else
            return false;
    }
    else
        multicastreq_.imr_interface.s_addr = htonl(INADDR_ANY);

    if(0 > setsockopt(socket_, IPPROTO_IP, IP_ADD_MEMBERSHIP, &multicastreq_, sizeof(multicastreq_))){
        perror("setsockopt():IP_ADD_MEMBERSHIP");
        return false;
    }

    return true;
}

bool print0x(char *data, int len)
{
    for(int i =0; i < len; i++)
    {
        printf("0x%x ",data[i]);
    }
    printf("\n");
}

bool printdata(char *data, int len)
{
    for(int i =0; i < len; i++)
    {
        printf("%x ",data[i]);
    }
    printf("\n");
}
int timeRead(char *data_,int *fd_,int time_s)
{
    fd_set read_fd;
    int max_fd = 0, ret = 0, len = 0;
    struct timeval timeout;
    timeout.tv_sec = time_s;
    timeout.tv_usec = 0;
    FD_ZERO(&read_fd);
    FD_SET(*fd_,&read_fd);
    max_fd = max(max_fd,*fd_);
    ret = select(max_fd,&read_fd,NULL,NULL,&timeout);
    if(ret == 0) {
        printf("select read timeout\n");
        return 0;
    }
    else if(ret < 0) {
        printf("select error\n");
        return -1;
    }
    else {
        len = read(*fd_,data_,MAXDATA);
        return len;
    }

}



