#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#define HEAD_232    "HD232"
#define HEAD_485    "HD485"
#define HEAD_RELAY  "HDrelay"
#define HEAD_IO     "HDio"
#define HEAD_IR     "HDir"

#include "global.h"
#include "uart.h"

class tcp_server
{
public:
    tcp_server();
    ~tcp_server();
    bool tcp_running(int dev);
    bool tcp_send(char *data, int len);

public:
    unsigned int max_connect = 32;

private:
    int sv_port = 16124;
    int sock_fd = -1;
    unsigned int cur_connect = 0;
    int *cur_fd = NULL;
    struct sockaddr_in *cur_add = NULL;
    struct sockaddr_in   sv_addr, client_addr;

    fd_set fd_rd;
    int max_fd = 0;
    bool is_finish = false;
};

#endif // TCP_SERVER_H
