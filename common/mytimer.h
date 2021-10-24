#ifndef MYTIMER_H
#define MYTIMER_H

#include <stdio.h>
#ifdef __WIN32__
#include <windows.h>
#else
#include <string.h>
#include <time.h>
#include <sys/time.h>
#endif // __WIN32__


class MyTimer
{
public:
    typedef struct tagTimeval
    {
        int  sec; //s
        long msec; //ms
        long usec; //us
    } Timeval;

public:
    MyTimer(){
        memset(&stCostTime, 0, sizeof(stCostTime));
#ifdef __WIN32__
        //用QueryPerformanceCounter()来计时  精度:微秒
        LARGE_INTEGER tmp;
        QueryPerformanceFrequency(&tmp);
        freq_ = tmp.QuadPart;
#else

#endif // __WIN32__
    }

    void Start(){
#ifdef __WIN32__
        QueryPerformanceCounter(&begin_);
#else
        gettimeofday(&begin_, 0);
#endif // __WIN32__
    }

    void Stop(){
#ifdef __WIN32__
        QueryPerformanceCounter(&end_);
        long result = (long)(((end_.QuadPart - begin_.QuadPart) * 1000000) / freq_); // total us
        stCostTime.sec  = (int)(result / 1000000);
        stCostTime.msec = (long)((result / 1000) % 1000);
        stCostTime.usec = (long)(result % 1000);
#else
        gettimeofday(&end_, 0);
        tim_subtract(&begin_, &end_);
#endif // __WIN32__
    }

    void Reset(){
        memset(&stCostTime, 0, sizeof(stCostTime));
    }

    void ShowCostTime(){
        printf("diff: %03ds. %03ldms. %03ldus\n",
               this->stCostTime.sec,
               this->stCostTime.msec,
               this->stCostTime.usec );
    }

private:
#ifndef __WIN32__
    int tim_subtract(struct timeval *begin, struct timeval *end){
        if(begin->tv_sec > end->tv_sec){
            memset(&stCostTime, -1, sizeof(stCostTime));
            return -1;
        }

        if((begin->tv_sec == end->tv_sec) && (begin->tv_usec > end->tv_usec)){
            memset(&stCostTime, -2, sizeof(stCostTime));
            return -2;
        }

        stCostTime.sec  = (end->tv_sec - begin->tv_sec);
        stCostTime.usec = (end->tv_usec - begin->tv_usec);

        if (stCostTime.usec < 0){
            stCostTime.sec--;
            stCostTime.usec += 1000000;
        }

        stCostTime.msec = stCostTime.usec / 1000;
        stCostTime.usec = stCostTime.usec % 1000;

        return 0;
    }
#endif // __WIN32__

public:
    Timeval stCostTime; // 花费的时间(精确到微秒)

private:
#ifdef __WIN32__
    LONGLONG freq_;
    LARGE_INTEGER begin_;
    LARGE_INTEGER end_;
#else
    struct timeval begin_, end_;
#endif // __WIN32__

};

#endif // MYTIMER_H
