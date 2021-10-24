#ifndef VERSION_H
#define VERSION_H

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define VERSION_DIR  "/version"
#define VERSION_FILE "version"
#define HARDWARE_INFO   "hardware_info"//add
#define UUID_DIR     "/uuid"
#define UUID_FILE    "uuid"

#define MONTH_PER_YEAR   12   // 一年12月
#define YEAR_MONTH_DAY   20   // 年月日缓存大小
#define HOUR_MINUTES_SEC 20   // 时分秒缓存大小

#define __MAX(a,b)  (((a) > (b)) ? (a) : (b))
#define __MIN(a,b)  (((a) < (b)) ? (a) : (b))

/*
 * 功能： 创建文件夹
 * 输入： file： 文件夹地址
 * 返回： bool
 * 日期： 2018.12.30
 * 作者： zh.sun
 */
static bool MakeDir(const char* file)
{
    if (access(file, F_OK) == -1){ // 如果指定的文件夹不存在，则创建
#ifdef __WIN32__
        int flag = mkdir(file);
#endif  /* __WIN32__ */
#ifdef __LINUX__
        int flag = mkdir(file, 0777);
#endif  /* __LINUX__ */
        if (flag == 0) {
            printf("make \"%s\" successfully\n", file);
        }
        else {
            printf("make \"%s\" errorly\n", file);
            return false;
        }
    }

    return true;
}

/*
 * 功能： 获取程序编译时间
 * 输入： 无
 * 返回： char* 编译时间
 * 日期： 2018.12.14
 * 作者： zh.sun
 */
static char* GetCompileTime(void)
{
    const char year_month[MONTH_PER_YEAR][4] =
    { "Jan", "Feb", "Mar", "Apr",
      "May", "Jun", "Jul", "Aug",
      "Sep", "Oct", "Nov", "Dec"};

    char compile_date[YEAR_MONTH_DAY]   = {0};
    char compile_time[HOUR_MINUTES_SEC] = {0};
    char str_month[4] = {0};
    int  year = 0, month = 0, day = 0, hour = 0, minutes = 0, seconds = 0;

    sprintf(compile_date, "%s", __DATE__); // "Aug 23 2016"
    sprintf(compile_time, "%s", __TIME__); // "10:59:19"

    sscanf(compile_date, "%s %d %d", str_month, &day, &year);
    sscanf(compile_time, "%d:%d:%d", &hour, &minutes, &seconds);

    for(int i = 0; i < MONTH_PER_YEAR; ++i)
    {
        if(strncmp(str_month, year_month[i], 3) == 0)
        {
            month = i + 1;
            break;
        }
    }

    static char res[20] = {};
    sprintf(res, "%d%02d%02d%02d%02d%02d", year, month, day, hour, minutes, seconds);

    return res;
}

/*
 * 功能： 获取系统Kernel使用的编译器版本
 * 输入： 无
 * 返回： 系统Kernel使用的编译器版本
 * 日期： 2019.02.26
 * 作者： zh.sun
 */
static char* GetKernelVersion(void)
{
    FILE* f = fopen("/proc/version", "rb");
    if(!f) return nullptr;

    char buf[512] = {};
    fgets(buf, 512, f); //读取一行，最大读取512字节
    /*样式
     * Linux version 3.18.20 (lwx@lwx-VirtualBox) (gcc version 4.9.4 20150629 (prerelease) (Hisilicon_v500_20180120) ) #5 SMP Wed Feb 20 21:20:31 CST 2019
    */

    char *pos1 = strstr(buf, "Hisilicon"); // 获取到 Hisilicon_v500_20180120) ) #5 S....
    if(!pos1)
        return nullptr;
    else{
        char *pos2 = strchr(pos1, ')'); // 获取到 ) ) #5 SMP Wed F.....
        if(!pos2)
            return nullptr;

        static char res[32] = {};
        //取出 "v500_20180120"
        strncpy(res, pos1 + strlen("Hisilicon") + 1, pos2 - pos1 - strlen("Hisilicon") - 1);
        return res;
    }

    return nullptr;
}

/*
 * 功能： 生成程序版本号
 * 输入： const char *version:     <主版本号>.<次版本号>.<修订版本号>
 *       unsigned int stageNum：   阶段版本号
 *       char* sn：                序列号
 * 返回： bool
 * 日期： 2018.12.14
 * 作者： zh.sun
 */
typedef enum  {
    STAGE_BASE = 0,
    STAGE_ALPHA,
    STAGE_BETA,
    STAGE_RC,
    STAGE_RELEASE
}SW_StageNum;
struct SW_VERSION{
    const char *version;
    SW_StageNum stage;
};
//static bool GenerateVersion(unsigned int masterVersion, unsigned int subVersion, unsigned int revisionVersion, unsigned int stageNum, const char* pn)
static bool GenerateVersion(SW_VERSION sw_ver, unsigned int chipId)
{
    const char stage[5][8] = { "base", "alpha", "beta", "RC", "release"};

    char sw_version[128] = {};
    sprintf(sw_version, "%s.%s_%s", sw_ver.version, GetCompileTime(), stage[sw_ver.stage]);
    // <主版本号>.<次版本号>.<修订版本号>.<编译日期>_<阶段版本>, 即1.1.1.20140321_beta
    printf(">>> sw_version: %s\n", sw_version);

    char hw_version[128] = {};
    sprintf(hw_version, "%d_%s", chipId, GetKernelVersion());
    printf(">>> hw_version: %s\n", hw_version);

    if( !MakeDir(VERSION_DIR) )
        return false;

    char filepath[24];
    sprintf(filepath, "%s/%s", VERSION_DIR, VERSION_FILE);
    FILE* f = fopen( filepath, "wb" );
    if(f){
        char content[256] = {};
        sprintf(content,
                "{\n\"software_version\": \"%s\",\n\"hardware_version\": \"%s\"\n}\n",
                sw_version,
                hw_version
                );

        fwrite(content, 1, strlen(content), f);
        fclose(f);
        return true;
    }
    else
        return false;
}

#include <uuid/uuid.h>
/*
 * 功能： 生成uuid
 * 输入： 无
 * 返回： bool
 * 日期： 2018.12.30
 * 作者： zh.sun
 */
static bool GenerateUUID()
{
    if( !MakeDir(UUID_DIR) )
        return false;

    char filepath[24];
    sprintf(filepath, "%s/%s", UUID_DIR, UUID_FILE);
    if (access(filepath, F_OK) == -1){ // 如果指定的文件不存在，则创建。否则不再生成uuid
        FILE* f = fopen( filepath, "wb" );
        if(f){
            uuid_t uuid;
            uuid_generate( uuid );

            fprintf(f, "%02X", uuid[0]);
            int i;
            for(i = 1; i < 16; i++)
                fprintf(f, "-%02X", uuid[i]);
            fprintf(f, "%c", '\n');

            fclose(f);
            return true;
        }
        else
            return false;

    }

    return true;
}

#endif // VERSION_H
