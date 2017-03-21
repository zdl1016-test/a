#include "stat_simple.h"

int main()
{
    StatSimple stat("stat_sample", 10, StatSimple::ENUM_WORK_MODE_SVR);

    if (stat.Init())
    {
        printf("stat init failed. msg : %s\n", stat.GetErrMsg());
        return -1;
    }

    for(;;)
    {
        stat.Inc(9);
        sleep(1);
    }
}
