/*
 * 创建于 2018.01.16
 * 创建者：孙志豪
 * version：0.0.1
 * 该类支持：单播，组播，广播
 *
*/

#ifndef UDPSOCKET_H
#define UDPSOCKET_H

#include "networkinfo.h"

class UDPSocket : NetworkInfo
{
public:
    UDPSocket();
    ~UDPSocket();

    // 创建Server端的 udp socket
    int CreateUDPServer(const uint k_serverport, bool b_nonblocking = true, bool b_readdr = true);
    // 创建Client端的 udp socket
    int CreateUDPClient(const char *kp_serverip, const uint k_serverport, bool b_nonblocking = true, bool b_connect = false);
    // 关闭socket
    void CloseUDPSocket();
    // 接收数据
    long RecvFrom(char *p_message, long maxdatalen);
    // 发送数据（对方的ip和port在Create的时候赋值了）- 多为client端时使用
    long SendTo(const char *kp_message, long maxdatalen);
    // 指定对方的ip和port，并对其发送数据 - 多为server端时使用
    long SendTo(const char *kp_clientip, const uint k_clientport, const char *kp_message, long maxdatalen);
    // 返回数据至发送端（和RecvFrom()配合）- 多为server端时使用
    long EchoTo(const char *kp_message, long maxdatalen);
    // 加入广播，使socket支持发送广播消息（发送广播时需要，接收时不需要）
    bool AddBroadCast(const char *kp_ethname = nullptr);
    // 加入组播组，监听组播上的消息（接收消息时需要）
    bool AddMemberShip(const char *kp_sendtoip, const char *kp_ethname = nullptr);
    // 离开组播组
    bool DropMemberShip();
    // 多网卡 (发送) 组播/广播数据前，一般需要般绑定本地IP（发送消息时需要）
    bool BindLocalAddr(const char *kp_ethname, const uint k_loaclport = 0, bool b_readdr = true);

    // 获取该socket的SO_RCVBUF参数
    int GetRevBuf();
    // 获取该socket的SO_SNDBUF参数
    int GetSndBuf();
    // 设置该socket的SO_RCVBUF参数
    bool SetRevBuf(const int recvbuffsize);
    // 设置该socket的SO_SNDBUF参数
    bool SetSndBuf(const int sendbuffsize);
    // 获取client地址结构体
    struct sockaddr_in GetClientSockaddr();

private:
    // 设置非阻塞socket
    bool setnonblocking();

private:
    int socket_;                    // 创建的udp套接字
    bool bconnect_;                 // 是否使用有连接的udp
    socklen_t socklen_;             // sockaddr_in结构体的长度
    struct ip_mreq multicastreq_;   // 组播地址结构体
    struct sockaddr_in serveraddr_; // server地址结构体
    struct sockaddr_in clientaddr_; // client地址结构体

};

#endif // UDPSOCKET_H
