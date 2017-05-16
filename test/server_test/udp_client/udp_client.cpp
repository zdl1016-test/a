#include <sys/time.h>
#include <netinet/in.h> // for sockaddr_in

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <unistd.h>

#include "stat_simple.h"

using namespace std;

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        printf("usage : %s srv_ip srv_port is_recv(defaut:0) is_show_recv(default:0) timeout_ms(100) interval_us(0)\n", argv[0]);
        printf("note : is_recv 是否收包; timeout_ms 收包时的超时控制 毫秒\n");
        return -1;
    }
    char * srv_ip= argv[1];
    int srv_port = atoi(argv[2]);

    // 是否收回包
    int is_recv = 0;
    if (argc >= 4)
    {
        is_recv = atoi(argv[3]);
    }

    // 是否屏幕显示收到的回包
    int is_show_recv = 0;
    if (argc >= 5)
    {
        is_show_recv = atoi(argv[4]);
    }

    int timeout_ms = 100;
    if (argc >= 6)
    {
        timeout_ms = atoi(argv[5]);
    }

    int interval_us = 0;
    if (argc >= 7)
    {
        interval_us = atoi(argv[6]);
    }

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0)
    {
        printf("socket() failed.");
        return -1;
    }


    struct sockaddr_in srv_addr;
    bzero(&srv_addr, sizeof(srv_addr));

    srv_addr.sin_family = AF_INET;
    inet_aton(srv_ip, &srv_addr.sin_addr);
    srv_addr.sin_port = htons(srv_port);

    char send_buf[1024] = {0};
    //snprintf(send_buf, sizeof(send_buf), "%s", "nemozhang");
    for (int i=0; i<sizeof(send_buf)-1; i++)
    {
        send_buf[i] = 'a' + i % 26;
    }
    int send_buf_len = strlen(send_buf);
    printf("%s, \nlen:%d\n", send_buf, send_buf_len);

    char recv_buf[1024] = {0};
    int recv_buf_len = sizeof(recv_buf) - 1; // 保留一个字节, 用来填充\0
    int  recv_len = 0;

    printf("connect to srv_ip:%s, srv_port:%u, is_recv:%d, is_show_recv:%d, timeout_ms:%d, interval_us:%d\n", 
            srv_ip, srv_port, is_recv, is_show_recv, timeout_ms, interval_us);

    StatSimple stat("stat_udp_client", 100, StatSimple::ENUM_WORK_MODE_SVR);
    if (stat.Init())
    {   
        printf("stat Init faild. msg : %s\n", stat.GetErrMsg());

    }  

    // 收包的话, 为了避免服务器丢包而影响压力测试, 设置超时时间
    if (is_recv)
    {
        struct timeval timeout;
        timeout.tv_sec = timeout_ms / 1000;
        timeout.tv_usec = (timeout_ms % 1000) * 1000; 
        setsockopt(sockfd , SOL_SOCKET,SO_RCVTIMEO, &timeout, sizeof(timeout));
    }

    int flag = 0;
    while(1)
    {
        if (interval_us > 0) {
            usleep(interval_us);
        }

        // sendto 不管服务器地址是否存在, 都不会阻塞
        int ret = sendto(sockfd, &send_buf[0], send_buf_len, flag, (sockaddr*)&srv_addr, sizeof(srv_addr));
        if (ret != send_buf_len)
        {
            stat.Inc(1); // fail
        }
        else
        {
            stat.Inc(0); // succ
        }

        if (is_recv==0)
        {
            continue;
        }


        // 假设我们收到的包都是刚才udp服务器的回包. 忽略获取服务器的ip和port
        recv_len = recvfrom(sockfd, &recv_buf[0], recv_buf_len, flag, 0, 0);
        if (recv_len >=0)
        {
            stat.Inc(2); // recv succ
            if (is_show_recv)
            {
                recv_buf[recv_len] = '\0';
                printf("recv svr's resp [%s] len:%d \n", recv_buf, recv_len);
            }
        }
        else
        {
            stat.Inc(3); // recv fail
        }
    }
    return 0;
}
