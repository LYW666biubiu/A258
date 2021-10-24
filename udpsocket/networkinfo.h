#ifndef NETWORKINFO_H
#define NETWORKINFO_H

#include <unistd.h>         /* close() functions */
#include <arpa/inet.h>      /* inet(3) functions */
#include <net/if.h>         /* ifreq struct */
#include <sys/ioctl.h>      /* ioctl() functions */
#include <netinet/in.h>     /* sockaddr_in{} and other Internet defns */
#include <net/route.h>      /* RTF_GATEWAY */
#include <stdio.h>
#include <cstring>

using namespace std;

#define ROUTEPATH "/proc/net/route"
#define ETH0      "eth0"
#define ETH1      "eth1"
#define ETHLO     "lo"


//    typedef enum {
//        kHWADDR  = SIOCGIFHWADDR,
//        kADDR    = SIOCGIFADDR,
//        kBRDADDR = SIOCGIFBRDADDR,
//        kNETMASK = SIOCGIFNETMASK,
//        kALL
//    }IF_CONFIG;


    /*
     * 功能：获取对应网卡的网络参数，现支持获取MAC，IP，MASK，BROADCASTADDR
     * 输入：const char *ethname：  指定获取哪一个网卡
     *      IF_CONFIG id：         自定义枚举类型，用来指定获取网卡的哪一个参数
     *      char *getch：          获取到的结果。当需要获取网卡上的所有信息，该格式为：
     *                             “mac:%s@ip:%s@mask:%s@braod:%s”
    typedef enum {
        kHWADDR  = SIOCGIFHWADDR,
        kADDR    = SIOCGIFADDR,
        kBRDADDR = SIOCGIFBRDADDR,
        kNETMASK = SIOCGIFNETMASK,
        kALL
    }IF_CONFIG;
     * 返回：bool
     */
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

    /*
     * 功能：获取对应网卡的网关
     * 输入：const char *ethname：  指定获取哪一个网卡
     *      char *pgateway：       获取到的结果。
     * 返回：bool
     */
    bool GetIPV4Gateway(const char *ethname, char *pgateway)
    {
        char buffer[200] = { 0 };
        unsigned long buflen = sizeof(buffer);

        unsigned long defaultRoutePara[4] = { 0 };
        FILE *pfd = fopen(ROUTEPATH, "r");
        if (NULL == pfd)
            return false;

        while (fgets(buffer, buflen, pfd))
        {
            sscanf(buffer, "%*s %x %x %x %*x %*x %*x %x %*x %*x %*x\n",
                    (unsigned int *)&defaultRoutePara[1],
                    (unsigned int *)&defaultRoutePara[0],
                    (unsigned int *)&defaultRoutePara[3],
                    (unsigned int *)&defaultRoutePara[2]);

            if (NULL != strstr(buffer, ethname))
            {
                //如果FLAG标志中有 RTF_GATEWAY
                if (defaultRoutePara[3] & RTF_GATEWAY)
                {
                    unsigned long ip = defaultRoutePara[0];
                    sprintf(pgateway, "%lu.%lu.%lu.%lu", (ip & 0xff), (ip >> 8) & 0xff, (ip >> 16) & 0xff, (ip >> 24) & 0xff);
                    break;
                }
            }

            memset(buffer, 0, buflen);
        }

        fclose(pfd);
        pfd = NULL;
        return true;
    }


#endif // NETWORKINFO_H
