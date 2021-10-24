#include "common_A258.h"
#include "common.h"
int c_netmsgid = -1;
int c_osdmsgid = -1;
int c_netmsgtype = 1;
int c_osdmsgtype = 2;
const char *c_netmsgpath = ".";
const char *c_osdmsgpath = "..";

bool readHdVersion(char *hd_version)
{
    FILE *hdFp = NULL;
    int rdLen = 0;
    char buff[256] = {0};

    char *st = NULL, *end = NULL;

    if(NULL == (hdFp = fopen(HDPATH,"r"))) {
        printf("open hardware_info error\n");
        return false;
    }
    memset(buff,0,256);
    memset(hd_version,0,32);

    rdLen = fread(buff,1,256,hdFp);
    if(rdLen > 0) {
        printf("read hardware_info:%s\n",buff);
        st = strstr(buff,"\"board\": \"");
        st += strlen("\"board\": \"");
        end = strchr(st,'\"');
        strncpy(hd_version,st,end-st);
        printf("get hdVersion:%s\n",hd_version);
    }
    else{
        fclose(hdFp);
        return false;
    }
    fclose(hdFp);
    return true;
}
