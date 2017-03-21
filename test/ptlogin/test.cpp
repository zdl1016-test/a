#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "ptlogin2_api.h"

//#define MY_APPID		10000001

// pick UIN from ptlogin cookie
unsigned long PickUin(const char* szUin)
{
	char* p = (char*)szUin;
	if(!p || *p != 'o')
		return 0;

	p++;
	while(*p== '0')
		p++;
	return atol(p);
}

void Usage()
{
	printf("Usage:\n");
	printf("./test local_ip ptlogin_ip1 ptlogin_port1 ptlogin_ip2 ptlogin_port2 app_id uin user_ip cookie timeout_ms\n");
	printf("Example:\n");
	printf("./test 0.0.0.0 172.23.19.191 18891 172.23.19.192 18891 1000001 22078888 10.6.59.63 @yRBIW7z3J 500\n");
	printf("\n");
	exit(1);
}


int main(int argc, char* argv[])
{
	if (argc != 11)
		Usage();

	int iRet = -1;
	int iTmp = 0;
	int iArg = 0;
	
	char *szLocalIP = argv[++iArg];
	unsigned short ushLocalPort = 0;

	char *szServIP1 = argv[++iArg];
	iTmp = atol(argv[++iArg]); 
	if (iTmp<0 || iTmp>65535) {
		printf("ptlogin_port1 format error\n");
		return -1;
	}
	unsigned short ushPort1 = (unsigned short)(iTmp);

	char *szServIP2 = argv[++iArg];
	iTmp = atol(argv[++iArg]); 
	if (iTmp<0 || iTmp>65535) {
		printf("ptlogin_port2 format error\n");
		return -1;
	}
	unsigned short ushPort2 = (unsigned short)(iTmp);
	
	iRet = api_ses_init(szLocalIP, 0, szServIP1, ushPort1, szServIP2, ushPort2 );
	if(iRet !=0)
	{
		printf("api_ses_init error %d\n", iRet);
		return -1;
	}

	int iAppID = atol(argv[++iArg]);
	
	unsigned long ulUin = atol(argv[++iArg]);
	unsigned long ulUserIP = ntohl(inet_addr(argv[++iArg]));
	char *szKey = argv[++iArg];

	unsigned int seq = GetSeq(); 
	const    int iTimeout = atol(argv[++iArg]);
	char	 res  = VERIFY_FAIL;
	char 	 szErrMsg[256] = {0};
	const    int iErrMsgLen =  sizeof(szErrMsg)-1;
	iRet = api_ses_verify(seq, iAppID, ulUin, ulUserIP, szKey, iTimeout, &res, szErrMsg, iErrMsgLen);
	if(iRet == 0 && res == VERIFY_OK)	
	{
		printf("api_ses_verify ok\n");
	}
	else
	{
		printf("api_ses_verify failed %d %s \n", iRet, szErrMsg);
	}

	return 0;
}

