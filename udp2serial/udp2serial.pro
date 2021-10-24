#最新带GPIO红外
TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

#指定生成的应用程序名
TARGET = udpSerial
#指定目标文件(obj)的存放目录
OBJECTS_DIR += ../obj



#20DV200
QMAKE_CXXFLAGS +=  -march=armv7-a\
                  -mcpu=cortex-a9


QMAKE_CFLAGS += -march=armv7-a\
                -mcpu=cortex-a9


COMMON_PATH = /home/9_my_code_tool/public/common
BASE_PATH = /home/ywli/sdk/Hi3520D_SDK_V1.0.5.0/mpp

INCLUDEPATH += /home/ywli/A258/A258_2.0/common
INCLUDEPATH += /home/ywli/sdk/Hi3520D_SDK_V1.0.5.0/mpp/include
INCLUDEPATH += /home/9_my_code_tool/public/common/udpsocket
INCLUDEPATH += /home/9_my_code_tool/public/common/cJSON
INCLUDEPATH += /home/9_my_code_tool/public/common/cSCODe
INCLUDEPATH += /home/9_my_code_tool/public/common/md5sum
INCLUDEPATH += /home/9_my_code_tool/public/common
INCLUDEPATH += /home/9_my_code_tool/public/common/easylogger/inc

LIBS += $${BASE_PATH}/lib/libmpi.a \
        $${BASE_PATH}/lib/libVoiceEngine.a\
        $${BASE_PATH}/lib/libvqev2.a\
        $${BASE_PATH}/lib/libjpeg.a\
        $${BASE_PATH}/lib/libaec.a\
        $${BASE_PATH}/lib/libanr.a\
        $${BASE_PATH}/lib/libhdmi.a\
        $${BASE_PATH}/lib/libjpeg6b.a\
        $${BASE_PATH}/lib/libmem.a\
        $${BASE_PATH}/lib/libresampler.a\
        $${BASE_PATH}/lib/libtde.a\


#DEFINES += __31DV200__

#定义编译选项
DEFINES += __LINUX__
DEFINES += __DEBUG__
DEFINES += __test__

#DEFINES += __GPIO1_5__     #700k
#DEFINES += __GPIO1_6__
#DEFINES += __7000__
#DEFINES += __7000_NJ__      #21A
#DEFINES += __8000__

#DEFINES += __AUTO_TEST__
#DEFINES += __31DV200__

SOURCES += main.cpp \
$${COMMON_PATH}/cJSON/cJSON.c \
$${COMMON_PATH}/cSCODe/SCODe.cpp \
$${COMMON_PATH}/common.cpp \
$${COMMON_PATH}/easylogger/port/elog_port.c \
$${COMMON_PATH}/easylogger/src/elog.c \
$${COMMON_PATH}/easylogger/src/elog_async.c \
$${COMMON_PATH}/easylogger/src/elog_buf.c \
$${COMMON_PATH}/easylogger/src/elog_utils.c \
$${COMMON_PATH}/md5sum/our_md5.cpp \
$${COMMON_PATH}/md5sum/our_md5hl.cpp \
$${COMMON_PATH}/udpsocket/udpsocket.cpp \
    ../common/commandmap_scode.cpp \
    ../common/common_A258.cpp \
    software_config.cpp \
    global.cpp \
    rs232.cpp \
    uart.cpp \
    rs485.cpp \
    hi_reg.c\
    relay.cpp \
    infrared.cpp \
    io.cpp \
    umpset.cpp \
    io_edc.cpp \
    tcp_server.cpp

HEADERS += \
$${COMMON_PATH}/cJSON/cJSON.h \
$${COMMON_PATH}/cSCODe/SCODe.h \
$${COMMON_PATH}/md5sum/our_md5.h \
$${COMMON_PATH}/udpsocket/udpsocket.h \
$${COMMON_PATH}/version.h \
$${COMMON_PATH}/singleton.h \
    software_config.h \
    global.h \
    ../common/commandmap_scode.h \
    ../common/common_A258.h \
#    ../common/mytimer.h \
    rs232.h \
    uart.h \
    rs485.h \
    hi_reg.h\
    relay.h \
    infrared.h \
    io.h \
    umpset.h \
    io_edc.h \
    tcp_server.h

LIBS += -lpthread -lm -ldl -lrt \
        -lstdc++

#DEFINES += __SEND_485__
