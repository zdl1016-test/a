#include "stat_simple.h"

int main()
{
    StatSimple stat("stat_sample", 10, StatSimple::ENUM_WORK_MODE_CLI);

    if (stat.Init())
    {
        printf("stat init failed. msg : %s\n", stat.GetErrMsg());
        return -1;
    }

    for(;;)
    {
        printf("%d\n", stat.Get(9));
        sleep(1);
    }

    return 0;
}
