#include "tcp_server.h"


tcp_server::tcp_server()
{
    int ret = 0;
    sv_port = global_data.tcp_port;
    max_connect = global_data.tcp_link_max;
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(sock_fd < 0) {
        perror("set tcp socket error");
        return ;
    }

    sv_addr.sin_family  = AF_INET;
    sv_addr.sin_addr.s_addr = htonl (INADDR_ANY);
    sv_addr.sin_port = htons(sv_port);

   if(tcp_pid == 0){
        if(bind(sock_fd, (struct sockaddr*)(&sv_addr), sizeof(struct sockaddr_in)) < 0) {
            perror("bind addr error");
            sock_fd = -1;
            return ;
        }
    }

    ret = listen(sock_fd, max_connect);
    if(ret < 0 ) {
        sock_fd = -1;
        perror("listen error");
        return ;
    }
    FD_ZERO(&fd_rd);
    FD_SET(sock_fd, &fd_rd);
    max_fd = max_fd > sock_fd ? max_fd : sock_fd;

    cur_fd = (int*)malloc(max_connect * sizeof(int));
    cur_add = (struct sockaddr_in*)malloc(max_connect * sizeof(struct sockaddr_in));
    printf("tcp server init ok\n");
}


tcp_server::~tcp_server()
{
    is_finish = true;

    for(uint i = 0; i < cur_connect; i++) {
        FD_CLR(cur_fd[i], &fd_rd);
        close(cur_fd[i]);
    }
    FD_ZERO(&fd_rd);
    free(cur_add);

    close(sock_fd);
    sock_fd = -1;
    if(cur_fd)
        free(cur_fd);
}

bool tcp_server::tcp_send(char *data, int len)
{
    int s_len = 0;
    for(uint i = 0; i < cur_connect; i++) {
        s_len = send(cur_fd[i], data, len, 0);
    }

    return true;
}


bool tcp_server::tcp_running(int dev)
{
    int tmp_fd = -1;
    unsigned int i = 0;
    socklen_t cur_addr_len;
    int len = 0;
    char buf[512];
    fd_set fd_rd_use;

    while( !is_finish ) {
        fd_rd_use = fd_rd;
        printf("start select\n");
        if( select(max_fd + 1, &fd_rd_use, NULL, NULL, NULL) == -1){
            perror("select error");
            break;
        }

        if( FD_ISSET(sock_fd, &fd_rd_use) ) {
            tmp_fd = -1;
            cur_addr_len = sizeof(struct sockaddr_in);
            tmp_fd = accept(sock_fd, (struct sockaddr*)(&client_addr), &cur_addr_len);
            if(tmp_fd < 0) {
                perror("accpet error");
                continue;
            }
            else if(cur_connect < max_connect) {
                cur_fd[cur_connect] = tmp_fd;
                memcpy(&cur_add[cur_connect], &client_addr, sizeof(struct sockaddr_in));
                FD_SET(cur_fd[cur_connect], &fd_rd);
                max_fd = max_fd > cur_fd[cur_connect] ? max_fd : cur_fd[cur_connect];
                cur_connect++;

                printf("current connect tcp client ip :%s  port :%d, current link number :%d \n",
                       inet_ntoa(cur_add[cur_connect - 1].sin_addr), ntohl(cur_add[cur_connect - 1].sin_port), cur_connect);
            }
            else {
                printf("current connect tcp client number :%d\n", cur_connect);
            }
        }

        for(i = 0; i < cur_connect; i++) {
            if( FD_ISSET(cur_fd[i], &fd_rd_use) ) {
                len = 0;
                memset(buf, 0, sizeof(buf));
                len = read(cur_fd[i], buf, sizeof(buf));

                if(len == 0) {
                    register unsigned int j;
                    printf("disconnect ip:%s port:%d \n",
                           inet_ntoa(cur_add[i].sin_addr), ntohl(cur_add[i].sin_port));
                    close(cur_fd[i]);
                    FD_CLR(cur_fd[i], &fd_rd);
                    cur_connect--;

                    for(j = i; j < cur_connect; j++) {
                        cur_fd[j] = cur_fd[j+1];
                        memset(&cur_add[j], 0, sizeof(struct sockaddr_in));
                        memcpy(&cur_add[j], &cur_add[j+1], sizeof(struct sockaddr_in));
                    }
                }
                else {
                    if(!strncmp(buf, HEAD_485, strlen(HEAD_485))) {

                    }
                    else if(!strncmp(buf, HEAD_RELAY, strlen(HEAD_RELAY))) {

                    }
                    else if(!strncmp(buf, HEAD_IO, strlen(HEAD_IO))) {

                    }
                    else if(!strncmp(buf, HEAD_IR, strlen(HEAD_IR))) {

                    }
                    else if(!strncmp(buf, HEAD_232, strlen(HEAD_232))) {
                        if(dev != -1) {
                            write(dev, &buf[strlen(HEAD_232)], len - strlen(HEAD_232));
                        }
                    }
                    else {
                        if(-1 != dev) {
                            write(dev, buf, len);
                        }
                    }
                }
            }
        }
    }
    return true;
}
