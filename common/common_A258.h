#ifndef COMMON_A258_H
#define COMMON_A258_H

#define SZ_IPADDR       (24)
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>        /* ftok() etc. */
#include <sys/msg.h>        /* msgget() etc. */
#include <pthread.h>
#include <errno.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <sys/stat.h>

#include <singleton.h> // 线程安全的单例类实现


#define HDPATH      "/version/hardware_info"
#define BUFF_SIZE_A258 1024


template<typename T>
std::string int2str(const T &dst_temp)
{
    std::stringstream stream;
    stream << dst_temp;
    return stream.str();
}
bool readHdVersion(char *hd_version);

#endif // COMMON_H
