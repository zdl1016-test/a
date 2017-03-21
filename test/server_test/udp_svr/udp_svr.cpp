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
    if (argc < 2)
    {
        printf("usage : %s srv_port is_show_recv(default:1) is_send_resp(default:1)\n", argv[0]);
        return -1;
    }
    int srv_port = atoi(argv[1]);

    int is_show_recv = 1;
    if (argc >= 3)
    {
        is_show_recv = atoi(argv[2]);
    }
    
    int is_send_resp = 1;
    if (argc >= 4)
    {
        is_send_resp = atoi(argv[3]);
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
    srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    srv_addr.sin_port = htons(srv_port);

    int ret = 0;
    ret = bind(sockfd, (sockaddr*)&srv_addr, sizeof(srv_addr));
    if (ret)
    {
        printf("bind failed.ret:%d, errno:%d\n", ret, errno);
        return -1;
    }


    char send_buf[128] = {0};
    int send_buf_len = 0;
    char recv_buf[128] = {0};
    int recv_buf_len = sizeof(recv_buf) - 1;
    int recv_len = 0;


    printf("bind_port:%u\n", srv_port);

    StatSimple stat("stat_udp_srv", 100, StatSimple::ENUM_WORK_MODE_SVR);
    if (stat.Init())
    {   
        printf("stat Init faild. msg : %s\n", stat.GetErrMsg());
    }  

    struct sockaddr_in cli_addr;
    bzero(&cli_addr, sizeof(cli_addr));
    int cli_addr_len = sizeof(cli_addr);

    int flag = 0;
    while(1)
    {
        int recv_len = recvfrom(sockfd, &recv_buf[0], recv_buf_len, flag, 
                (sockaddr*)&cli_addr, (socklen_t *)&cli_addr_len);

        // note : in udp, recv 0 byte is normal. although in tcp recv 0 mean peer close the connection
        if (recv_len >= 0) 
        {
            recv_buf[recv_len] = '\0';

            if (is_show_recv)
            {
                printf("recv [%s] len:%d from %s:%d\n", recv_buf, recv_len, 
                        inet_ntoa(cli_addr.sin_addr),
                        cli_addr.sin_port);
            }
            stat.Inc(0); // succ

            if (is_send_resp)
            {
                int ret = sendto(sockfd, recv_buf, recv_len, 0/*flag*/, (sockaddr*)&cli_addr, sizeof(cli_addr));
                if (ret != recv_len)
                {
                    stat.Inc(3); // resp fail
                }
                else
                {
                    stat.Inc(2); // resp succ
                }
            }
        }
        else
        {
            stat.Inc(1); // fail
        }
    }
    return 0;
}
