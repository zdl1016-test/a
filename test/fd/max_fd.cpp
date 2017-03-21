#include <sys/time.h>
#include <netinet/in.h> // for sockaddr_in

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>


using namespace std;

const int MAX_COUNT = 1000*1000;

int main(int argc, char* argv[])
{
    int i = 0;
    int sockfd = 0;

    //for (i=0; i<MAX_COUNT; ++i)
    for (i=0; i<202400; ++i)
    {
        sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if(sockfd < 0)
        {
            printf("socket() failed.\n");
            break;
        }

    }
    perror("reason");

    printf("max_fd : %d\n", i);

    return 0;
}
