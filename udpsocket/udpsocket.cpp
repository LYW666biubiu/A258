#include <string.h>         /* bzero() functions */
#include <unistd.h>         /* close() functions */
#include <sys/types.h>      /* basic system data types */
#include <sys/socket.h>     /* basic socket definitions */
#include <arpa/inet.h>      /* inet(3) functions */
#include <fcntl.h>          /* nonblocking */
#include <errno.h>
#include <stdio.h>

#include "udpsocket.h"

UDPSocket::UDPSocket()
{
    socklen_ = sizeof(struct sockaddr_in);
    bzero(&serveraddr_, sizeof(serveraddr_));
    bzero(&clientaddr_, sizeof(serveraddr_));
    bconnect_ = false;
    socket_ = -1;
}

UDPSocket::~UDPSocket()
{
    if(socket_ != -1){
        close(socket_);
        socket_ = -1;
    }
}

/*
 * 功能：创建Server端的 udp socket
 * 输入：const uint k_serverport：Server端的端口，即监听端口
 *      bool b_readdr：          是否复用地址
 *      bool b_nonblocking：     是否设为非阻塞
 * 返回：int
 *      失败 -1
 *      成功 创建的socket值
 */
int UDPSocket::CreateUDPServer(const uint k_serverport, bool b_nonblocking, bool b_readdr)
{
    serveraddr_.sin_family = AF_INET;
    serveraddr_.sin_addr.s_addr = htonl (INADDR_ANY);
    serveraddr_.sin_port = htons(k_serverport);

    socket_ = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_ == -1) {
        perror("can't create socket file");
        return -1;
    }

    if(b_readdr){
        int opt = 1;
        if(0 > setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))){
            perror("setsockopt():SO_REUSEADDR");
            return -1;
        }
    }

    if(b_nonblocking){
        if (setnonblocking() < 0){
            perror("setnonblock error");
            return -1;
        }
    }

    if (-1 == bind(socket_, (struct sockaddr *) &serveraddr_, sizeof(struct sockaddr))){
        perror("bind error");
        return -1;
    }

    return socket_;
}

/*
 * 功能：创建Client端的 udp socket
 * 输入：const char *kp_serverip：  Server端的ip，即发送到该ip，当b_broadcast=true，该值为null表示自动获取系统网卡中的广播地址；为非null，则统一置为INADDR_BROADCAST（255.255.255.255）
 *      const uint k_serverport：  Server端的端口，即发送到该端口
 *      bool b_nonblocking：       是否设为非阻塞
 *      bool b_connect：           是否创建有连接的udp，在b_broadcast=true时，失效
 * 返回：int
 *      失败 -1
 *      成功 创建的socket值
 */
int UDPSocket::CreateUDPClient(const char *kp_serverip, const uint k_serverport, bool b_nonblocking, bool b_connect)
{
    if(k_serverport > 0){
        serveraddr_.sin_family = AF_INET;
        serveraddr_.sin_port = htons(k_serverport);
        if(kp_serverip != nullptr)
            serveraddr_.sin_addr.s_addr = inet_addr(kp_serverip); //如果作发送广播使用时，此处不需要赋值
        //inet_pton(AF_INET, kp_serverip, &serveraddr_.sin_addr);
    }
    socket_ = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_ == -1) {
        perror("can't create socket file");
        return -1;
    }

    if(k_serverport > 0 && b_connect){
        if (0 > connect(socket_, (struct sockaddr*)&serveraddr_, sizeof(serveraddr_))) {
            perror("connect error");
            return -1;
        }
        bconnect_ = true;
    }

    if(b_nonblocking){
        if (setnonblocking() < 0){
            perror("setnonblock error");
            return -1;
        }
    }

    return socket_;
}

/*
 * 功能：关闭socket套接字
*/
void UDPSocket::CloseUDPSocket()
{
    if(socket_ != -1){
        close(socket_);
        socket_ = -1;
    }
}

/*
 * 功能：接收来自对方socket的数据
 * 输入：char *p_message：  网络上发送来的数据
 *      long maxdatalen：  指定本次接收数据的最大长度
 * 返回：long 接收到的数据的长度，或者是recvfrom的错误码
 */
long UDPSocket::RecvFrom(char *p_message, long maxdatalen)
{
    return recvfrom(socket_, p_message, maxdatalen, 0, (struct sockaddr*)&clientaddr_, &socklen_);
}

/*
 * 功能：发送数据
 * 输入：const char *kp_message： 需要发送来的数据
 *      long maxdatalen：        指定本次发送数据的最大长度
 * 返回：long 发送完成的数据的长度，或者是sendto的错误码
 */
long UDPSocket::SendTo(const char *kp_message, long maxdatalen)
{
    if(serveraddr_.sin_addr.s_addr == htonl(INADDR_ANY)){
        printf("please intput client_ip and client_port\n");
        return -1;
    }

    if(bconnect_)
        return write(socket_, kp_message, maxdatalen);
    else
        return sendto(socket_, kp_message, maxdatalen, 0, (struct sockaddr*)&serveraddr_, sizeof(serveraddr_));
}

/*
 * 功能：发送数据（需提供对方的ip和port）
 * 输入：const char* kp_clientip：对方的ip
 *      const uint k_clientport：对方的port
 *      const char *kp_message： 需要发送来的数据
 *      long maxdatalen：        指定本次发送数据的最大长度
 * 返回：long 发送完成的数据的长度，或者是sendto的错误码
 */
long UDPSocket::SendTo(const char* kp_clientip, const uint k_clientport, const char *kp_message, long maxdatalen)
{
    if(!kp_message || !kp_clientip)
        return -1;

    struct sockaddr_in serveraddr;
    serveraddr.sin_family 		= AF_INET;
    serveraddr.sin_port   		= htons(k_clientport);
    serveraddr.sin_addr.s_addr 	= inet_addr(kp_clientip);

    return sendto(socket_, kp_message, maxdatalen, 0, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
}

/*
 * 功能：回传数据（用于UDP Server端，recvfrom之后可以调用，回传数据给来访者）
 * 输入：const char *kp_message： 需要发送来的数据
 *      long maxdatalen：        指定本次发送数据的最大长度
 * 返回：long 发送完成的数据的长度，或者是sendto的错误码
 */
long UDPSocket::EchoTo(const char *kp_message, long maxdatalen)
{
    if(clientaddr_.sin_addr.s_addr == htonl(INADDR_ANY)){
        return -1;
    }

    printf("Receive client (%s:%u) on socket: %d  \nmessage:%s\n", \
           inet_ntoa(((struct sockaddr_in*)&clientaddr_)->sin_addr),\
           ntohs(((struct sockaddr_in*)&clientaddr_)->sin_port),\
           socket_,\
           kp_message);

    return sendto(socket_, kp_message, maxdatalen, 0, (struct sockaddr*)&clientaddr_, socklen_);
}

/*
 * 功能：加入广播，使socket支持发送广播消息（发送广播时需要，接收时不需要）
 * 输入：const char *kp_ethname：  指定本地网卡(默认为null，即不质指定网卡，使用255.255.255.255广播地址，此时需要将socket绑定到指定网卡)
 * 返回：bool
 */
bool UDPSocket::AddBroadCast(const char *kp_ethname)
{
    //默认的套接字描述符sock是不支持广播，必须设置套接字描述符以支持广播
    int opt = 1;
    if(0 > setsockopt(socket_, SOL_SOCKET, SO_BROADCAST, &opt, sizeof(opt))){
        perror("setsockopt():SO_BROADCAST");
        return false;
    }

    if(kp_ethname != nullptr){
        //由于不同的网卡接口的广播地址是不一样的，因此指定网卡接口
        char broadaddr[32] = {0};
        if (GetNetworkInfo(kp_ethname, kBRDADDR, broadaddr))
            serveraddr_.sin_addr.s_addr = inet_addr(broadaddr);
        else
            return false;

        //printf("Broadcast IP: %s\n", broadaddr);
    }
    else
        serveraddr_.sin_addr.s_addr = htonl(INADDR_BROADCAST); //使用INADDR_BROADCAST，在多网卡时需要绑定固定南网卡

    return true;
}

/*
 * 功能：加入组播组
 * 输入：const char *kp_sendtoip： 指定组播地址ip
 *      const char *kp_ethname：  指定本地网卡
 * 返回：bool
 */
bool UDPSocket::AddMemberShip(const char *kp_sendtoip, const char *kp_ethname)
{
    int loop = 1;
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

/*
 * 功能：离开组播组
 * 返回：bool
 */
bool UDPSocket::DropMemberShip()
{
    if (0 > setsockopt(socket_, IPPROTO_IP, IP_DROP_MEMBERSHIP, &multicastreq_, sizeof(multicastreq_))){
        perror("setsockopt():IP_DROP_MEMBERSHIP");
        return false;
    }

    return true;
}

/*
 * 功能：绑定本地地址（多网卡发送组播时可能需要调用）
 * 输入：const char *kp_ethname： 指定本地网卡
 *      const uint k_loaclport： 指定本地port，可为0，表示不指定本地port，由系统随即分配
 *      bool b_readdr：          是否复用地址
 * 返回：bool
 */
bool UDPSocket::BindLocalAddr(const char *kp_ethname, const uint k_loaclport, bool b_readdr)
{
    if(b_readdr){
        int opt = 1;
        if(0 > setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))){
            perror("setsockopt():SO_REUSEADDR");
            return false;
        }
    }

    struct sockaddr_in localaddr;
    bzero(&localaddr, sizeof(localaddr));
    localaddr.sin_family = AF_INET;

    char localip[32] = {0};
    if( GetNetworkInfo(kp_ethname, kADDR, localip) )
        localaddr.sin_addr.s_addr = inet_addr(localip);
    else
        return false;

    if(k_loaclport > 0)
        localaddr.sin_port = htons (k_loaclport);

    if (-1 == bind(socket_, (struct sockaddr *)&localaddr, sizeof(struct sockaddr)))
    {
        perror("bind error");
        return false;
    }

    return true;
}

/*
 * 功能：获取系统设置中socket的recv的最大buff大小
 * 返回：int buff大小
 */
int UDPSocket::GetRevBuf()
{
    int revbuffsize = 0;
    socklen_t optlen = sizeof(revbuffsize);
    if(0 != getsockopt(socket_, SOL_SOCKET, SO_RCVBUF, &revbuffsize, &optlen))
    {
        perror("getsockopt():SO_RCVBUF");
        return -1;
    }

    return revbuffsize;
}

/*
 * 功能：获取系统设置中socket的send的最大buff大小
 * 返回：int buff大小
 */
int UDPSocket::GetSndBuf()
{
    int sendbuffsize = 0;
    socklen_t optlen = sizeof(sendbuffsize);
    if(0 != getsockopt(socket_, SOL_SOCKET, SO_SNDBUF, &sendbuffsize, &optlen))
    {
        perror("getsockopt():SO_SNDBUF");
        return -1;
    }

    return sendbuffsize;
}

/*
 * 功能：设置系统设置中socket的recv的最大buff大小
 * 返回：bool
 */
bool UDPSocket::SetRevBuf(const int recvbuffsize)
{
    if(setsockopt(socket_, SOL_SOCKET, SO_RCVBUF, (const char*)&recvbuffsize, sizeof(int)))
    {
        perror("setsockopt():SO_RCVBUF");
        return false;
    }

    return true;
}

/*
 * 功能：设置系统设置中socket的send的最大buff大小
 * 返回：bool
 */
bool UDPSocket::SetSndBuf(const int sendbuffsize)
{
    if(setsockopt(socket_, SOL_SOCKET, SO_SNDBUF, (const char*)&sendbuffsize, sizeof(int)))
    {
        perror("setsockopt():SO_SNDBUF");
        return false;
    }

    return true;
}

/*
 * 功能： 获取client地址结构体
 * 输入： 无
 * 返回： struct sockaddr_in
 * 日期： 2018.08.28
 * 作者： zh.sun
 */
struct sockaddr_in UDPSocket::GetClientSockaddr()
{
    return clientaddr_;
}

/*
 * 功能：设置系socket为非阻塞
 * 返回：bool
 */
bool UDPSocket::setnonblocking()
{
    if (-1 == fcntl(socket_, F_SETFL, fcntl(socket_, F_GETFD, 0)|O_NONBLOCK))
        return false;

    return true;
}
