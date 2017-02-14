
#include <sys/time.h>

#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <string>

#include <android/log.h>

#ifndef TIMER_H
#define TIMER_H

extern struct timeval tstart;
extern struct timeval tend;

inline void timeBegin()
{
    gettimeofday(&tstart, 0);
}

inline void timeEnd(std::string NAME)
{
    gettimeofday(&tend, 0);
    __android_log_print(ANDROID_LOG_INFO, "Timer", "%s: %f msec",NAME.c_str(),((tend.tv_sec - tstart.tv_sec)*1000000u +
             tend.tv_usec - tstart.tv_usec)/1e3);
    //printf("%s time : %f ms\n", TAG.c_str(), ((tend.tv_sec - tstart.tv_sec)*1000000u +
    //         tend.tv_usec - tstart.tv_usec)/1e3);
}

#define TIME_BEGIN timeBegin()
#define TIME_END(s) timeEnd(s)

#endif
