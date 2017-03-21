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
#include "stat_simple.h"
#include <string>

using namespace std;

#define MAX_LENGTH_CONNECT_QUEUE    1024 // 该参数最好是可配置的, apigw的配置的值是:2048
#define CRLF    "\r\n"

// 转换timestamp为yyyy-mm-dd格式字符串
std::string GetDateFromTimestamp( unsigned int timer )                                                                                                        
{ 
    time_t curtime = timer;
    struct tm *tblock;
    tblock = localtime(&curtime);  
  
    // format date info
    char szBuf[64] = {0};
    snprintf(szBuf, sizeof(szBuf), 
        "%4d-%02d-%02d",              // yyyy-mm-dd:2010-08-18       
        tblock->tm_year+1900,           // tm_year: curr_year-1900     
        tblock->tm_mon+1,               // tm_mon : [0~11]             
        tblock->tm_mday);               // tm_day : [1~31]             
  
    return std::string(szBuf);
} 

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s port\n", argv[0]);
        return -1;
    }

    int port = atoi(argv[1]);

    int ret = 0;

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
    svr_addr.sin_port = htons(port);

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

    printf("listen at port : %d ...\n", port);

    StatSimple stat("stat_iterative_svr", 100, StatSimple::ENUM_WORK_MODE_SVR);
    if (stat.Init())
    {
        printf("stat Init faild. msg : %s\n", stat.GetErrMsg());
    }

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
            return -1;
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
            unsigned int now = time(NULL);
            string now_time = GetDateFromTimestamp (now);

            recv_buf[recv_buf_len] = '\0';
            printf("[%s] recv : [%s]\n", now_time.c_str(), recv_buf);
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
                return -1;
            }

            send_total += ret;
        }

        // 请求统计+1
        stat.Inc(0);

        close(connfd);
    }
}
