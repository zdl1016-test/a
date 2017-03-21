/*
 * @desc    : 简单的计时器类
 * @author  : nemozhang
 * @history :
 *
 *   2012-02-02_12:03:12 Create
 *
 * @copyright: Shenzhen Tencent Co.Ltd
 *
 */
#ifndef _CTIMER_H_
#define _CTIMER_H_


#include <sys/time.h>
#include <stdio.h>
#include <memory.h>

class CMyTimer
{
public:
    CMyTimer()
    {
        memset(&tpStart, 0, sizeof(tpStart));
        memset(&tpEnd, 0, sizeof(tpEnd));
    }
    void Begin()
    {
        gettimeofday(&tpStart, NULL);
    }
    float GetElapseTimeSec()
    {

        gettimeofday(&tpEnd, NULL);

        float timecost = 0.0f;
        timecost = tpEnd.tv_sec - tpStart.tv_sec + (float)(tpEnd.tv_usec-tpStart.tv_usec)/1000000;

        return timecost;
    }
    float GetElapseTimeMS()
    {
        return GetElapseTimeSec()*1000;
    }
private:
    struct timeval tpStart;
    struct timeval tpEnd;
};
#endif
