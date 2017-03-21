#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <errno.h>
#include <sys/epoll.h>
#include <assert.h>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <map>
#include <vector>
#include "stat_simple.h"

using namespace std;

#define SVR_PORT                    8899
#define MAX_LENGTH_CONNECT_QUEUE    1024 // 该参数最好是可配置的, apigw的配置的值是:2048
#define CRLF    "\r\n"

vector<int> pids;

// 主进程退出, kill全部子进程
void signal_int(int signal)
{
    for (int i=0; i<pids.size(); ++i)
    {
        kill(pids[i], SIGTERM);
    }

    int pid = 0;
    while ((pid = waitpid(-1, NULL, 0)) > 0)
    {    
        printf("child %d exit.\n", pid); // do nothing
    }
    return ;
}

// 子进程主动退出
void signal_chld(int signal)
{
    int pid = 0;
    while ( (pid=waitpid(-1, NULL, WNOHANG)) > 0 )
    {
        printf("child %d exit.\n", pid); // do nothing
    }
    return ;
}

int child_main(int listenfd);

StatSimple g_stat("stat_prefork_svr", 100, StatSimple::ENUM_WORK_MODE_SVR);

void bind_cpu();
int main(int argc, char* argv[])
{
    int ret = 0;

    if (argc < 2)
    {
        printf("Usage %s child_num\n", argv[0]);
        return -1;
    }

    int prefork_num = atoi(argv[1]);

    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0)
    {
        printf("socket() error, error:%s", strerror(errno));
        return -1;
    }

    struct sockaddr_in svr_addr;
    memset(&svr_addr, 0, sizeof(svr_addr));

    // bind ip port
    svr_addr.sin_family = AF_INET;
    svr_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    svr_addr.sin_port = htons(SVR_PORT);

    if (bind(listenfd, (sockaddr*)&svr_addr, sizeof(svr_addr)) < 0)
    {
        close(listenfd);
        printf("bind error, error:%s", strerror(errno));
        return -1;
    }

    if (listen(listenfd, MAX_LENGTH_CONNECT_QUEUE) < 0)
    {
        close(listenfd);
        printf("listen error, error:%s", strerror(errno));
        return -1;
    }

    printf("listen at port : %d ...\n", SVR_PORT);

    if (g_stat.Init())
    {
        printf("stat Init faild. msg : %s\n", g_stat.GetErrMsg());
    }

    signal(SIGINT, signal_int);
    signal(SIGCHLD, signal_chld);


    // 如果通过循环创建子进程, 
    // 要么创建子进程后, 子进程死循环, 不返回. ==> 推荐
    // 如果返回的话, 必须break. 否则子进程会循环创建子进程, 导致死循环...
    for (int i=0; i<prefork_num; ++i)
    {
        int pid = fork();
        if (pid == 0) // child
        {
            child_main(listenfd); // never return
            exit(0); // 万一return 直接退出
        }
       
        // parent
        pids.push_back(pid);
    }

    int pid = 0;
    while ((pid = waitpid(-1, NULL, 0)) > 0)
    {    
        printf("child %d exit.\n", pid); // do nothing
    }

}

// 绑定CPU
void bind_cpu()
{                                                                                                                                                                   
    int cpu_num = sysconf(_SC_NPROCESSORS_CONF);

    if (cpu_num < 2)
    {
        return;
    }

    cpu_set_t mask;

    CPU_ZERO(&mask);

    // 根据pid随机挑选一个cpu (除了cpu0)
    int cpu_idx = getpid() % (cpu_num-1) + 1;

    CPU_SET(cpu_idx, &mask); // 如果索引超过cpu的实际个数,简单忽略

    // 开始绑定
    if (sched_setaffinity(0, sizeof(mask), &mask) == -1) // 第一个参数是进程ID, 0表示当前进程
    {   
        printf("warning: could not set CPU affinity, msg:%s \n", strerror(errno));
        return ;
    } 
    
    printf("proc %d bind cpu : %d\n", getpid(), cpu_idx);
    return;
}
int child_main(int listenfd)
{
    printf("child %d stated.\n", getpid());

    bind_cpu();
    int ret = 0;
    int connfd = 0;
    struct sockaddr_in cli_addr;
    int cli_addr_len = sizeof(cli_addr);
    char send_buf[512] = {0};
    char recv_buf[512] = {0};
    char content_buf[512] = {0};
    int send_buf_len = 0;
    int recv_buf_len = 0;
    for(;;)
    {
        send_buf_len = 0;
        recv_buf_len = 0;
        // can be block
        connfd = accept(listenfd, (struct sockaddr*)&cli_addr, (socklen_t*)&cli_addr_len);
        if (connfd < 0)
        {
            if (errno == EINTR)
            {
                continue;
            }

            close(listenfd);
            printf("accept error, error:%s", strerror(errno));
            //return -1;
        }

        // read
        // 由于tcp的数据没有边界, 由于是简单的测试, 没有约定协议,  我就读一次, 准备一个大点的buf, 否则读多次会阻塞...
        // 如果是http协议的话, 就需要按照约定的协议读, 发现一次没有读完协议没有读完, 可以继续等待读, 反正对方会再发送... 
        // gw就是读多次, 每读到一次就检查数据是否读完, 没读完就再等待. 直到3秒超时
        for (;;)
        {
            ret = read(connfd, &recv_buf[recv_buf_len], sizeof(recv_buf)-recv_buf_len);

            if (ret < 0 && errno == EINTR)
            {
                continue;
            }
        
            recv_buf_len += ret;

            // 这里可以对已收到的buf做协议解析, 如果没有读完, 就继续循环读

            recv_buf[recv_buf_len] = '\0';
            //printf("recv : [%s]\n", recv_buf);
            break;
        }
        

        //printf("will response ...\n");

        snprintf(content_buf, sizeof(content_buf), "pid : %d, time : %u", getpid(), time(NULL));
        snprintf(send_buf, sizeof(send_buf),
                "HTTP/1.1 200 OK" CRLF
                "Content-Type: text/html; charset=UTF-8" CRLF
                "Server: iterative_svr" CRLF
                "Content-Length: %d" CRLF
                CRLF
                "%s", strlen(content_buf), content_buf
                );

        send_buf_len = strlen(send_buf);

        // write
        int send_total = 0;
        while (send_total < send_buf_len)
        {
            ret = write(connfd, &send_buf[send_total], send_buf_len-send_total);
            if (ret < 0)
            {
                if (errno == EINTR)
                {
                    continue;
                }
                
                close(listenfd);
                close(connfd);
                printf("write error, error:%s", strerror(errno));
                //return -1;
                break;
            }

            send_total += ret;
        }

        // 请求统计+1
        g_stat.Inc(0);

        close(connfd);
    }

}
