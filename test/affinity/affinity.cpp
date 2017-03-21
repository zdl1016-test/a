#include <stdio.h>
#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/sysinfo.h>
#include<unistd.h>
 
//#define __USE_GNU
#include<sched.h>
#include<ctype.h>
#include<string.h>
#include<errno.h>
int main(int argc, char* argv[])
{
    // 查询系统的CPU个数
    int cpu_num = sysconf(_SC_NPROCESSORS_CONF);
    printf("cpu num : %d\n", cpu_num);

    if (argc < 2)
    {
        printf("usage : %s cpu_idx1 [cpu_idx2] ...\n", argv[0]);
        return 0;
    }
  
    // 绑定CPU
    cpu_set_t mask;

    CPU_ZERO(&mask);

    // 设置要绑定的CPU (可以绑定多个)
    for (int i=1; i<argc; ++i)
    {
        // 获取预绑定的CPU索引
        int cpu_idx = atoi(argv[i]);
        if (cpu_idx >= cpu_num)
        {
            printf("invalid cpu_idx : %d. ignore, should be in [0 ~ %d]\n", cpu_idx, cpu_num-1);
            //return 0;
        }

        CPU_SET(cpu_idx, &mask); // 如果索引超过cpu的实际个数,简单忽略
    }

    // 开始绑定
    if (sched_setaffinity(0, sizeof(mask), &mask) == -1) // 第一个参数是进程ID, 0表示当前进程
    {
        printf("warning: could not set CPU affinity, msg:%s \n", strerror(errno));
        return 0;
    }

    // 执行计算密集型任务, 然后在另外的终端里查看cpu的占用率
    int count = 1;
    while(1)
    {
        float ret = count / 3.0f;
        if (count % 10000000 == 0)
        {
            printf("count : %d, ret:%f \n", count, ret);
            usleep(100); // 防止把机器卡死, sleep下

            cpu_set_t mask_get;
            CPU_ZERO(&mask_get);

            // 读取cpu的亲和性设置
            if (sched_getaffinity(0, sizeof(mask_get), &mask_get) == -1)
            {
                printf("warning: cound not get cpu affinity, msg:%s\n", strerror(errno));
            }

            for (int i=0; i<cpu_num; ++i)
            {
                if (CPU_ISSET(i, &mask_get))
                {
                    printf("this process %d is running processor : %d\n",getpid(), i);
                }
            }
        }
        ++count;
    }

    return 0;
}

