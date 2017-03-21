#include <sys/time.h>
#include <netinet/in.h> // for sockaddr_in

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#include "stat_simple.h"

using namespace std;

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        printf("usage : %s srv_ip srv_port is_recv(defaut:0) is_show_recv(default:0) timeout_ms(100)\n", argv[0]);
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

    // udp是无连接的, 所以即使svrip port不存在, connect也不会阻塞 (测试通过)
    // 所以, 这里connect的意义在于, 内核在收回包时, 会把对应srv的包交给上层进程, 其他不认识的srv的回包则不会交给上层. 即使回包的目标是同样的本地端口
    int ret = connect(sockfd, (const struct sockaddr *)&srv_addr, sizeof(srv_addr));
    if (ret)
    {
        printf("connect failed. %s\n", strerror(errno));
    }
    else
    {
        printf("connect succ. \n");
    }

    char send_buf[128] = {0};
    snprintf(send_buf, sizeof(send_buf), "%s", "nemozhang");
    char send_buf_len = strlen(send_buf);

    char recv_buf[128] = {0};
    char recv_buf_len = sizeof(recv_buf) - 1; // 保留一个字节, 用来填充\0
    int  recv_len = 0;

    printf("connect to srv_ip:%s, srv_port:%u\n", srv_ip, srv_port);

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
