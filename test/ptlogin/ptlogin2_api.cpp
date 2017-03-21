#include "ptlogin2_api.h"
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <time.h>
#include <stdlib.h>

static sockaddr_in 	g_sAddrServer;
static sockaddr_in 	g_sAddrServer2;
static sockaddr_in  g_sAddrLocal;
static char			g_achPkgSend[512];	
static char			g_achPkgRecv[512];


#define inaddrr(x) (*(struct in_addr *) &ifr->x[sizeof sa.sin_port])

#define MAX_INTERFACE_CNT 32

/*
 * 专门用于获取虚拟机的内网IP
 */
static char* GetSpecialIP()
{
	static char ip[20];
	ifreq		intfBuf[MAX_INTERFACE_CNT];
	int  		sockfd = -1;
	
	char 		*ipaddr = NULL;
	
	struct ifconf      ifc;
	
	struct ifreq       *ifr;
	struct sockaddr_in sa;

	sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
	if (sockfd < 0 )
	{
		return NULL; 
	}

	// init the ifc struct
	memset(&ifc, 0, sizeof(ifc));
	ifc.ifc_len = sizeof(intfBuf);
	ifc.ifc_req = intfBuf;
	if (0 != ioctl(sockfd, SIOCGIFCONF, &ifc)) 
	{
      	close(sockfd);
      	return NULL;
	}

	if (sizeof(intfBuf) <= ifc.ifc_len)
	{
		close(sockfd);
		return NULL;
	}

	ifr = ifc.ifc_req;
	for (;(char *) ifr < (char *) ifc.ifc_req + ifc.ifc_len; ++ifr) {
        if (ifr->ifr_addr.sa_data == (ifr+1)->ifr_addr.sa_data) {
              continue;  /* duplicate, skip it */
        }

	    if (ioctl(sockfd, SIOCGIFFLAGS, ifr)) {
	          continue;  /* failed to get flags, skip it */
	    }
	    
	    strcpy(ip , inet_ntoa(inaddrr(ifr_addr.sa_data)));
	    if(0==strncmp(ip, "172", 3) || 0==strncmp(ip, "192", 3) || 0==strncmp(ip, "10.", 3))
	    {
	       ipaddr=ip;
	       break;
	    }
	}
	
	close (sockfd);
    return ipaddr;
}

static char* GetNomalIP()
{
      static char ip[20];                  
      int i;
      int s = socket (PF_INET, SOCK_STREAM, 0);

      for (i=1;;i++)
      {
          struct ifreq ifr;
          struct sockaddr_in *sin = (struct sockaddr_in *) &ifr.ifr_addr;

          ifr.ifr_ifindex = i;
          if (ioctl (s, SIOCGIFNAME, &ifr) < 0)
            break;

          /* now ifr.ifr_name is set */
          if (ioctl (s, SIOCGIFADDR, &ifr) < 0)
            continue;

         memset(ip, 0, sizeof(ip));
         strcpy(ip , inet_ntoa (sin->sin_addr));
         if(0==strncmp(ip, "172", 3) || 0==strncmp(ip, "192", 3) || 0==strncmp(ip, "10.", 3))
         { 
           close (s);
           return ip;
         }
         else  
            continue;
      }

      close (s);
      return NULL;
}

static char* GetLocalIP()
{
	char *szIP = GetNomalIP();	
	if (NULL == szIP)
	{
		szIP = GetSpecialIP();
	}
	
	return szIP;
}

// 取seq, 分+秒+(微秒/10)
unsigned int GetSeq()
{
    struct timeval tvv;
    struct tm      tmm;
    char   tmp[20] = {0};
    time_t tt      = 0;

    gettimeofday(&tvv, NULL);
    tt = tvv.tv_sec;
    tmm = *localtime(&tt);

    sprintf(tmp, "%d%02d%05d", tmm.tm_min, tmm.tm_sec, (int)(tvv.tv_usec/10));

    return  atoi(tmp);
}

// 时间差, 精确到毫秒 
static int DiffTimeMsec(const struct timeval tvLast)
{
	struct timeval tvCur;
	int	   iSecDiff		= 0;
	int	   iMsecDiff 	= 0;
	
	gettimeofday(&tvCur, NULL);

	iSecDiff = tvCur.tv_sec - tvLast.tv_sec;
	iMsecDiff= (int)(((int)tvCur.tv_usec - (int)tvLast.tv_usec)/1000); // (int)保证操作在有符号下进行,
																		// 避免强制unsigned转换
	return (iSecDiff*1000 + iMsecDiff);
}

// 返回第一个随机选取的server
// 同时设置下次调用的是另一个server, 用rand做参数是为了在server1 和server2 之间负载均衡
static sockaddr_in* GetTargetServer(unsigned long rand)
{
	static int tmp = 0;
	if(tmp %2 == 0)
	{
		++tmp;
		if(rand % 2 == 0)					
			return &g_sAddrServer;
		else
			return &g_sAddrServer2;
	}
	else
	{
		++tmp;  // avoid error for no cgi but daemon server
		if(rand % 2 == 0)
			return &g_sAddrServer2;
		else
			return &g_sAddrServer;
	}

	// dummy no use for ever
	return &g_sAddrServer;
}

static int AddChar(char **p, int *piLenLeft, char cValue)
{
    int iLenLeft = 100; 
    char *pCur;

    pCur = *p;
    if (piLenLeft) iLenLeft = *piLenLeft;

    if ((size_t)iLenLeft < sizeof(cValue)) return -1;

    *pCur = cValue;
    pCur += sizeof(cValue);
    iLenLeft -= sizeof(cValue);

    *p = pCur;
    if (piLenLeft) *piLenLeft = iLenLeft;

    return 0;
}


static int AddWord(char **p, int *piLenLeft, unsigned short shValue)
{
    int iLenLeft = 100;
    char *pCur;
    unsigned short shNValue;

    pCur = *p;
    if (piLenLeft) iLenLeft = *piLenLeft;

    if ((size_t)iLenLeft < sizeof(shValue)) return -1;

    shNValue = htons(shValue);
    memcpy(pCur, &shNValue, sizeof(shNValue));
    pCur += sizeof(shValue);
    iLenLeft -= sizeof(shValue);

    *p = pCur;
    if (piLenLeft) *piLenLeft = iLenLeft;

    return 0;
}


static int AddDWord(char **p, int *piLenLeft, unsigned long lValue)
{
    int iLenLeft = 100;
    char *pCur;
    unsigned long lNValue;

    pCur = *p;
    if (piLenLeft) iLenLeft = *piLenLeft;

    if ((size_t)iLenLeft < sizeof(lValue)) return -1;

    lNValue = htonl(lValue);
    memcpy(pCur, &lNValue, sizeof(lNValue));
    pCur += sizeof(lValue);
    iLenLeft -= sizeof(lValue);

    *p = pCur;
    if (piLenLeft) *piLenLeft = iLenLeft;

    return 0;
}
// 长度两个字节
static int AddString(char **p, int *piLenLeft, char *pBuf, unsigned short shBufLen)
{
    int iLenLeft = 5000;
    char *pCur;

    if (AddWord(p, piLenLeft, shBufLen) < 0) return -1;

    pCur = *p;
    if (piLenLeft) iLenLeft = *piLenLeft;

    if (iLenLeft < shBufLen) return -2;

    memcpy(pCur, pBuf, shBufLen);
    pCur += shBufLen;
    iLenLeft -= shBufLen;

    *p = pCur;
    if (piLenLeft) *piLenLeft = iLenLeft;

    return 0;
}

// 长度为一个字节
static int AddString2(char **p, int *piLenLeft, char *pBuf, unsigned short shBufLen)
{
    int iLenLeft = 5000;
    char *pCur;

    if (AddChar(p, piLenLeft, shBufLen) < 0) return -1;

    pCur = *p;
    if (piLenLeft) iLenLeft = *piLenLeft;

    if (iLenLeft < shBufLen) return -2;

    memcpy(pCur, pBuf, shBufLen);
    pCur += shBufLen;
    iLenLeft -= shBufLen;

    *p = pCur;
    if (piLenLeft) *piLenLeft = iLenLeft;

    return 0;
}


static int GetChar(char **p, int *piLen, char *cValue)
{
    int iLen;
    char *pCur;

    pCur = *p;
    if (piLen) iLen = *piLen;
    else iLen = sizeof(char);

    if ((size_t)iLen < sizeof(char)) return -1;

    *cValue = *pCur;

    pCur += sizeof(char);
    iLen -= sizeof(char);

    *p = pCur;
    if (piLen) *piLen = iLen;

    return 0;
}

static int GetWord(char **p, int *piLen, unsigned short *pshValue)
{
    unsigned short shNValue;
    int iLen;
    char *pCur;

    pCur = *p;
    if (piLen) iLen = *piLen;
    else iLen = sizeof(short);

    if ((size_t)iLen < sizeof(shNValue)) return -1;

    memcpy(&shNValue, pCur, sizeof(shNValue));
    *pshValue = ntohs(shNValue);
    pCur += sizeof(shNValue);
    iLen -= sizeof(shNValue);

    *p = pCur;
    if (piLen) *piLen = iLen;

    return 0;
}



static int GetDWord(char **p, int *piLen, unsigned long *plValue)
{
    unsigned long lNValue;
    int iLen;
    char *pCur;

    pCur = *p;
    if (piLen) iLen = *piLen;
    else iLen = sizeof(long);

    if ((size_t)iLen < sizeof(lNValue)) return -1;

    memcpy(&lNValue, pCur, sizeof(lNValue));
    *plValue = ntohl(lNValue);
    pCur += sizeof(lNValue);
    iLen -= sizeof(lNValue);

    *p = pCur;
    if (piLen) *piLen = iLen;

    return 0;
}

/*
static int GetString(char **p, int *piLen, char *pBuf, int iBufLen)
{
    unsigned short shLen, shCopyLen;
    int iLen;
    char *pCur;

    if (GetWord(p, piLen, &shLen) < 0) return -1;

    pCur = *p;
    if (piLen) iLen = *piLen;
    else iLen = iBufLen;

    if (shLen > iLen) return -2;
    if (iBufLen > shLen) shCopyLen = shLen;
    else shCopyLen = iBufLen-1;

    memcpy(pBuf, pCur, shCopyLen);
    *(pBuf + shCopyLen) = 0;
    pCur += shLen;
    iLen -= shLen;

    *p = pCur;
    if (piLen) *piLen = iLen;

    return 0;
}
*/
static int GetString2(char **p, int *piLen, char *pBuf, int iBufLen)
{
    unsigned short shCopyLen;
    int iLen;
    char *pCur;
    unsigned char cLen;

    if (GetChar(p, piLen, (char*)&cLen) < 0) return -1;

    pCur = *p;
    if (piLen) iLen = *piLen;
    else iLen = iBufLen;

    if (cLen > iLen) return -2;
    if (iBufLen > cLen) shCopyLen = cLen;
    else shCopyLen = iBufLen-1;

    memcpy(pBuf, pCur, shCopyLen);
    *(pBuf + shCopyLen) = 0;
    pCur += cLen;
    iLen -= cLen;

    *p = pCur;
    if (piLen) *piLen = iLen;

    return 0;
}

/*
 * 设置非阻塞模式
 */
static int SetNBlock(int iSock)
{
	int iFlags;

	iFlags = fcntl(iSock, F_GETFL, 0);
	iFlags |= O_NONBLOCK;
	iFlags |= O_NDELAY;
	fcntl(iSock, F_SETFL, iFlags);
	return 0;
}

/*
 * 创建UDP套接字 
 * 输入: 本地主机的 地址信息
 * 输出: 无
 * 返回: 套接字，若<0，错误
 */
static int CreateUDPSocket(struct sockaddr_in* pstAddr)
{
	int s = 0;
	int flags = 1; //reusaddr
	//int iRcvBufLen = 256*1024;

	if(pstAddr == NULL)
	{
		return -1;
	}

	s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(s == -1)    return -1;

	setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &flags, sizeof(flags));
				
	if(bind(s, (struct sockaddr*)pstAddr, sizeof(struct sockaddr_in)) == -1)
	{
		close(s);
		return -2;
	}

	return s;
}

static bool IsVerifyHead(const char* pPkg, int iPkgLen)
{
    char* pCur = (char*)pPkg;
    unsigned short ushLen = 0;
    char* pTail = NULL;
    unsigned short  ushCmd = 0x00;

    pTail = pCur + iPkgLen -1;	
    
// 检查包头
    if(*pCur++ != STX) //  包头
    {
    
        return false;
    }
    
    // 校验长度: 
    GetWord(&pCur, NULL, &ushLen);
    if((int)ushLen != iPkgLen)
    {
  
        return false;
    }
   
    // 校验包尾
   
    if(*pTail != ETX)
    {
    
        return false;
    }
 
    // 校验命令号
    GetWord(&pCur, NULL, &ushCmd);
    if(CMD_VERIFYCODE != ushCmd)
    {
    	
        return false;
    }
    return true;
}


int api_vc_init(const char* szLocalIP, unsigned short ushLocalPort, 
	const char* szServerIP, unsigned short ushServerPort,
	const char* szServerIP2, unsigned short ushServerPort2)
{
	if(NULL == szServerIP  || '\0' == szServerIP[0]
	 ||NULL == szServerIP2 || '\0' == szServerIP2[0])
	{	
		return -1;
	}

	memset(&g_sAddrLocal,   0, sizeof(g_sAddrLocal));
	memset(&g_sAddrServer,  0, sizeof(g_sAddrServer));
	memset(&g_sAddrServer2, 0, sizeof(g_sAddrServer2));
	//memset(g_achPkg, 0, sizeof(g_achPkg));
    
	// local
	g_sAddrLocal.sin_family		= AF_INET;
	g_sAddrLocal.sin_port		= htons(ushLocalPort);
    if(szLocalIP)
    {
        g_sAddrLocal.sin_addr.s_addr = inet_addr(szLocalIP);
    }    
	else	// 如果localip 为空, 自己取内网IP
    {
        char* ip = GetLocalIP();
        if(!ip) 
            return -2;
        g_sAddrLocal.sin_addr.s_addr = inet_addr(ip);
    }    
	// server
	g_sAddrServer.sin_family 		= AF_INET;
	g_sAddrServer.sin_port 			= htons(ushServerPort);
	g_sAddrServer.sin_addr.s_addr 	= inet_addr(szServerIP);

	// server2
	g_sAddrServer2.sin_family 		= AF_INET;
	g_sAddrServer2.sin_port 		= htons(ushServerPort2);
	g_sAddrServer2.sin_addr.s_addr 	= inet_addr(szServerIP2);	
	
	return 0;
}


static int api_vc_composepkg(const char* szCode, int iCodeLen, const char* szVerifySession, 
	int iSessionLen, int iValidTime, unsigned int uiUserIP,unsigned short ushUserPort, 
	int iAppID, unsigned int uiSeq, char chSendTimes)
{
	short	ushLen		= 0;
	short   ushNetLen 	= 0; // 网络字节序长度

	char*	pCur		= g_achPkgSend;
	memset(g_achPkgSend, 0, sizeof(g_achPkgSend));
	
	// stx
    AddChar(&pCur, NULL, STX);
    // skip pkg len
    pCur+=2; // 
    // cmd no
    AddWord(&pCur, NULL, CMD_VERIFYCODE);
    // ServerID
    AddWord(&pCur, NULL, 0);
    //  client ip
    AddDWord(&pCur, NULL, uiUserIP);
    // client port
    AddWord(&pCur, NULL, ushUserPort);
    // conn ip
    AddDWord(&pCur, NULL, 0); // 这里应该填本地内网IP
    // conn port
    AddWord(&pCur, NULL, 0);  // 同上，端口 
    // interface ip
    AddDWord(&pCur, NULL, 0); // 
    // interface port
    AddWord(&pCur, NULL, 0);  // 同上，端口 
    // process seq
    AddChar(&pCur, NULL, 0);  // 这里可以填进程ID
    // skip DbID
    pCur++;
    // pad
    pCur+=2;
    // iSocketID ==>
    //AddDWord(&pCur, NULL, uiUserIP);
    memcpy(pCur, szCode, 4);
	pCur += 4;
    // seq
    AddDWord(&pCur, NULL, uiSeq);
	// appid
	AddDWord(&pCur, NULL, iAppID);
    // validtime
    AddDWord(&pCur, NULL, iValidTime);
    // request times
    AddChar(&pCur, NULL, chSendTimes);

    // code
    AddString2(&pCur, NULL, (char*)szCode, iCodeLen);
    // session
    AddString(&pCur, NULL, (char*)szVerifySession, iSessionLen);
     
    // etx
    AddChar(&pCur, NULL, ETX);

    // 计算包长
    ushLen	 = pCur - g_achPkgSend;
    ushNetLen = htons(ushLen);

    memcpy(g_achPkgSend+1, &ushNetLen, sizeof(ushNetLen));

	return ushLen;
}

static int api_vc_parsepkg(const char* szCode, unsigned int uiSeq, char* pResult,
		char* szErrMsg, int iErrMsgLen)
{
	char* pCur = (char*)g_achPkgRecv;
	char	szBuf[10] = {0}; 
	unsigned int seq = 0;
	
    pCur++; 	// stx
    pCur += sizeof(MYDBPkgHead);

    //  socketid -->code
	memcpy(szBuf, pCur, 4); 
	if( 0 != memcmp(szBuf, szCode, 4))
	{
		return -1;
	}
	pCur += 4;
    //  seq
	//pCur += 4;
	GetDWord(&pCur, NULL, (unsigned long*)&seq);
	if(seq != uiSeq)
	{
		return -1;
	}
    
    GetChar(&pCur, NULL, pResult); // iao

	pCur += 1;
    //GetChar(&pCur, NULL, pReqNums ); // 后续有用。先留着

	pCur += 4;  //GetDWord(&pCur, NULL, pGetImgTime); // 拉取时间
    
    GetString2(&pCur, NULL, szErrMsg, iErrMsgLen);
	
	return 0;
}

int api_vc_verify(const char* szCode, int iCodeLen, const char* szVerifySession, 
	int iSessionLen, int iValidTime, unsigned int uiUserIP,unsigned short ushUserPort, 
	int iAppID, unsigned int uiSeq, char chSendTimes, int iTimeout, char* pResutl,
    char* szErrMsg, int iErrMsgLen)
{
	int		iRet			= -1;
	int		sock 			= -1;
	short	shPkgLen		= 0;
	int		iRecvLen		= 0;
	int 	iSend1 			= 0;		// send pkg length
    struct timeval tv, tvStart;			// 开始调用时间
	fd_set	rfds;						// 
	bool	bTimeout		= true;		// 是否超时flag
	int 	iSendTimes		= 0;    	// 发包次数
	int		maxsock 		= -1;
	struct sockaddr_in*		pCurServer;	// 指向当前使用的server
	
	//memset(g_achPkg, 0, sizeof(g_achPkg));

	if(NULL==szCode || '\0'==szCode[0] 
		|| NULL== szVerifySession || '\0'==szVerifySession[0])
	{
		snprintf(szErrMsg, iErrMsgLen-1, "Code or VerifySession is not right!");
		return -1;
	}
	
	shPkgLen  = api_vc_composepkg(szCode, iCodeLen, szVerifySession, iSessionLen,
		iValidTime, uiUserIP, ushUserPort, iAppID, uiSeq, chSendTimes);

	sock = CreateUDPSocket(&g_sAddrLocal);
	if(sock < 0)
	{
		snprintf(szErrMsg, iErrMsgLen-1, "CreateUDPSocket Failed:%s\n", strerror(errno));
		return -1;
	}
	
#ifdef PTLOGIN2_SET_BLOCK
	;	// dummy
#else // 非阻塞
	int iFlags = fcntl(sock, F_GETFL, 0);
	fcntl(sock, F_SETFL, iFlags | O_NONBLOCK);
	SetNBlock(sock);
#endif	
	
	do
	{
		memset(&tvStart, 0, sizeof(tvStart));
		gettimeofday(&tvStart, NULL);
		
		pCurServer = GetTargetServer((int)szCode[0]); // 利用szCode[0]做随机数
		iSend1 = sendto(sock, g_achPkgSend, shPkgLen, 0, (struct sockaddr*)pCurServer, sizeof(struct sockaddr_in));		
		if(iSend1!= shPkgLen)
		{			
			if(iSendTimes==0) // 还有第二次机会.
			{	
				
				++iSendTimes; // 发送包次数递增
				continue; 
			}
			else	
			{
				snprintf(szErrMsg, iErrMsgLen-1, "sendto() error: %s\n", strerror(errno));
				close(sock);  // avoid leak socket resources
				return -1;
			}
		}
		++iSendTimes; // 发送包次数递增
		
		// 除以2是因为2台server,每个server分配一半的时间
		while( DiffTimeMsec(tvStart) < (iTimeout/2) )
		{
			memset(g_achPkgRecv, 0, sizeof(g_achPkgRecv));
			maxsock = sock+1;
			FD_ZERO(&rfds);
			FD_SET(sock, &rfds);
			tv.tv_sec 	= 0;
			tv.tv_usec 	= 50 * 1000; // 每次select 50 ms

			iRet = select(maxsock, &rfds, NULL, NULL, &tv);
			if(iRet <= 0)
			{
				continue;
			}
			if(FD_ISSET(sock, &rfds))
			{
				iRecvLen = recvfrom(sock, g_achPkgRecv, sizeof(g_achPkgRecv), 0, NULL, 0);

				if(iRecvLen < 0 )
				{
					continue;
				}
				if(!IsVerifyHead(g_achPkgRecv, iRecvLen))
				{
					continue;
				}
				iRet = api_vc_parsepkg(szCode, uiSeq, pResutl, szErrMsg, iErrMsgLen);
				if(0!= iRet)
				{
					continue;
				}

				bTimeout = false;
				break;
			}
			
		}
	}while( (iSendTimes < 2) && bTimeout);
	
	close(sock);

	if(bTimeout == true)
	{
		snprintf(szErrMsg, iErrMsgLen-1, "Recv timeout");
		return -1;
	}
	
	return 0;
}


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


int 
api_ses_init(const char* szLocalIP, unsigned short ushLocalPort, 
	const char* szServerIP, unsigned short ushServerPort,
	const char* szServerIP2, unsigned short ushServerPort2)
{
 	return api_vc_init(szLocalIP, ushLocalPort, szServerIP,
		ushServerPort, szServerIP2, ushServerPort2);
}


/*  判断是否是有效的访问session server的数据包
 *  输入: 包起始指针 和 长度
 *  输出: 该数据包对应的命令号 	
 *  返回:  bool 值
 */
bool IsValidPkg(const char* pPkg, int iPkgLen, unsigned short ushCmd)
{
	char* pCur 				= (char*)pPkg;
	char* pTail				= NULL;
	char 	 	    chSTX	= 0x0;
	unsigned short	ushLen	= 0;
//    unsigned long   ulUin   = 0;
    unsigned short  cmd     = 0;
	
	pTail = pCur + iPkgLen -1;		//  包尾指针	
		
	GetChar(&pCur, NULL, &chSTX);   // 取第一个字节
	if(STX != chSTX)				// stx ?
	{
		return false;
	}
	
	if(ETX != *pTail)				// etx ?
	{
		return false;
	}
	
	// 包长 相等 or 包长符合要求?
	GetWord(&pCur, NULL, &ushLen);
	if((int)ushLen != iPkgLen || ushLen > 4096 )		
	{
		return false;
	}

	// 命令号
	GetWord(&pCur, NULL, &cmd);
	if( cmd == ushCmd )    // 命令号 相等 ?
	{
         return true;
	}

	// 命令号不符合
	return false;
	
}

int 
api_ses_composepkg(unsigned int uiSeq, int iAppID, unsigned long ulUin, unsigned long ulUserIP, 
	const char* szKey, unsigned short ushCmd)
{
	short	ushLen		= 0;
	short   ushNetLen 	= 0; // 网络字节序长度
	char*	pCur		= g_achPkgSend;

	memset(g_achPkgSend, 0, sizeof(g_achPkgSend));
	pCur = (char*)g_achPkgSend;

	*pCur++ = STX;	// stx flag
	pCur += 2;		// pkg len
	AddWord(&pCur, NULL, ushCmd);	// cmd no
	pCur += 2;
	AddDWord(&pCur, NULL, ulUserIP); // client ip
	pCur += 2;		// client port
	pCur += 4; 		// conn ip
	pCur += 2; 		// conn port
	pCur += 4; 		// interface ip
	pCur += 2; 		// interface port
	pCur += 1;		// process seq
	pCur += 1;		// dbid
	pCur += 2;		// pad

//	pCur += 4; 		// socketid -> uluin
	AddDWord(&pCur, NULL, ulUin);

	//pCur += 4;		// seq no 
	AddDWord(&pCur, NULL, uiSeq);
	
	AddDWord(&pCur, NULL, ulUin);		// uin
	AddDWord(&pCur, NULL, ulUserIP);	// ip
	AddDWord(&pCur, NULL, iAppID);		// app id
	AddString(&pCur, NULL, (char*)szKey, strlen(szKey));	// verify key
	AddChar(&pCur, NULL, ETX);

	ushLen = pCur - g_achPkgSend;
	ushNetLen = htons(ushLen);

	memcpy(g_achPkgSend+1, &ushNetLen, 2);	

	return ushLen;
}

int 
api_ses_parsepkg(unsigned int uiSeq, unsigned long ulUin, char* pResult, char* szErrMsg, int iErrMsgLen)
{
	char* pCur = (char*)g_achPkgRecv;
	unsigned long uin = 0;
	unsigned int  seq = 0;
	pCur += 1;
	pCur += sizeof(MYDBPkgHead);

	//pCur += 4;
	GetDWord(&pCur, NULL, &uin);
	if(uin != ulUin)
		return -1;
	//pCur += 4; // seq
	GetDWord(&pCur, NULL, (unsigned long*)&seq);
	if(uiSeq != seq)
		return -1;
	
	GetChar(&pCur, NULL, pResult);

	GetString2(&pCur, NULL, szErrMsg, iErrMsgLen);
	
	return 0;
}

int 
api_ses_verify(unsigned int uiSeq, int iAppID, unsigned long ulUin, unsigned long ulUserIP, 
	const char* szKey, int iTimeout, char* pResult, char* szErrMsg, int iErrMsgLen)
{
	int		iRet			= -1;
	int		sock 			= -1;
//	char*	pCur 			= NULL;
	short	shPkgLen		= 0;
	int		iRecvLen		= 0;
	int 	iSend1 			= 0;	// send pkg length
    struct timeval tv, tvStart;		// 开始调用时间
	fd_set	rfds;
	bool	bTimeout		= true;	// 是否超时flag
	int 	iSendTimes		= 0;    // 发包次数
	int		maxsock 		= -1;
	struct sockaddr_in*		pCurServer;
	
	shPkgLen = api_ses_composepkg(uiSeq, iAppID, ulUin, ulUserIP, szKey, CMD_SESSION_VERIFY);

	sock = CreateUDPSocket(&g_sAddrLocal);
	if(sock < 0)
	{
		snprintf(szErrMsg, iErrMsgLen-1, "CreateUDPSocket Failed:%s\n", strerror(errno));
		return -1;
	}
	
#ifdef PTLOGIN2_SET_BLOCK	
	;	// dummy
#else	// 默认非阻塞
	int iFlags = fcntl(sock, F_GETFL, 0);
	fcntl(sock, F_SETFL, iFlags | O_NONBLOCK);
	SetNBlock(sock);
#endif	

	do
	{
		memset(&tvStart, 0, sizeof(tvStart));
		gettimeofday(&tvStart, NULL);
		pCurServer = GetTargetServer(ulUin);
	
		// 发包	
		iSend1 = sendto(sock, g_achPkgSend, shPkgLen, 0, (struct sockaddr*)pCurServer, sizeof(struct sockaddr_in));
		if(iSend1!= shPkgLen)
		{			
			if(iSendTimes==0) // 还有第二次机会.
			{	
				++iSendTimes; // 发送包次数递增
				continue; 
			}
			else
			{
				snprintf(szErrMsg, iErrMsgLen-1, "sendto() error: %s\n", strerror(errno));
				close(sock);  // avoid leak socket resources
				return -1;
			}
		}
		++iSendTimes; // 发送包次数递增
			
		while( DiffTimeMsec(tvStart) < (iTimeout/2) )
		{
			memset(g_achPkgRecv, 0, sizeof(g_achPkgRecv));
			maxsock = sock+1;
			FD_ZERO(&rfds);
			FD_SET(sock, &rfds);
			tv.tv_sec 	= 0;
			tv.tv_usec 	= 50 * 1000; // 每次select 50 ms

			iRet = select(maxsock, &rfds, NULL, NULL, &tv);
			if(iRet <= 0)
			{
				continue;
			}
			if(FD_ISSET(sock, &rfds))
			{
				iRecvLen = recvfrom(sock, g_achPkgRecv, sizeof(g_achPkgRecv), 0, NULL, 0);

				if(iRecvLen < 0 )
				{
					continue;
				}
				if(!IsValidPkg(g_achPkgRecv, iRecvLen, CMD_SESSION_VERIFY))
				{
					continue;
				}
				iRet = api_ses_parsepkg(uiSeq, ulUin, pResult, szErrMsg, iErrMsgLen); 
				if(0!= iRet)
				{
					continue;
				}

				bTimeout = false;
				break;
			}
			
		}
	}while((iSendTimes < 2) && bTimeout);
	
	close(sock);

	if(bTimeout)
	{
		snprintf(szErrMsg, iErrMsgLen-1, "Recv timeout");
		return -1;
	}
	
	return 0;
}


int 
api_ses_parsepkg2(unsigned int uiSeq, unsigned long ulUin, unsigned int* pLogintime, int* pFace, 
	char* pAge, char* pGender, char* szPassport, char* szNick, char* szMail,
	unsigned int* pLastaccesstime, char* pResult, char* szErrMsg, int iErrMsgLen)
{
	char* pCur = (char*)g_achPkgRecv;
	char	chTmp = 0;
	unsigned long 	uin = 0;
	unsigned int	seq = 0;
	
	pCur += 1;
	pCur += sizeof(MYDBPkgHead);
	
	//pCur += 4;
	GetDWord(&pCur, NULL, &uin);
	if(uin != ulUin)
		return -1;
	//pCur += 4; // seq
	GetDWord(&pCur, NULL, (unsigned long*)&seq);
	if(seq != uiSeq)
		return -1;
	
	GetChar(&pCur, NULL, pResult);
	if(*pResult != SES_VERIFY_OK)
	{
		GetString2(&pCur, NULL, szErrMsg, iErrMsgLen);
		return 0;
	}
	GetChar(&pCur, NULL, pAge);
	GetChar(&pCur, NULL, pGender);
	GetDWord(&pCur, NULL, (unsigned long*)pFace);
	GetDWord(&pCur, NULL, (unsigned long*)pLogintime);
	
	
	GetDWord(&pCur, NULL, (unsigned long*)pLastaccesstime);

	memcpy(&chTmp, pCur, 1);
	pCur++;
	memcpy(szPassport, pCur, chTmp);
	pCur += chTmp;
	
	GetString2(&pCur, NULL, szNick, 13);
	GetString2(&pCur, NULL, szMail, 51);
	GetString2(&pCur, NULL, szErrMsg, iErrMsgLen);
	return 0;
}


int 
api_ses_verify2(unsigned int uiSeq, int iAppID, unsigned long ulUin, unsigned long ulUserIP, 
	const char* szKey, int iTimeout,  unsigned int* pLogintime, int* pFace, 
	char* pAge, char* pGender, char* szPassport, char* szNick, char* szMail,
	unsigned int* pLastaccesstime, char* pResult, char* szErrMsg, int iErrMsgLen)
{
	int		iRet			= -1;
	int		sock 			= -1;
//	char*	pCur 			= NULL;
	short	shPkgLen		= 0;
	int		iRecvLen		= 0;
	int 	iSend1 			= 0;
    struct timeval tv, tvStart;		// 开始调用时间
	fd_set	rfds;
	bool	bTimeout		= true;	// 是否超时flag
	int 	iSendTimes		= 0;    // 发包次数
	int		maxsock 		= -1;
	struct sockaddr_in*		pCurServer;
	
	shPkgLen = api_ses_composepkg(uiSeq, iAppID, ulUin, ulUserIP, szKey, CMD_SESSION_VERIFY2);
	
	sock = CreateUDPSocket(&g_sAddrLocal);
	if(sock < 0)
	{
		snprintf(szErrMsg, iErrMsgLen-1, "CreateUDPSocket Failed:%s\n", strerror(errno));
		return -1;
	}
	
#ifdef PTLOGIN2_SET_BLOCK	
	;	// dummy
#else
	int iFlags = fcntl(sock, F_GETFL, 0);
	fcntl(sock, F_SETFL, iFlags | O_NONBLOCK);
	SetNBlock(sock);
#endif	

	do
	{
		memset(&tvStart, 0, sizeof(tvStart));
		gettimeofday(&tvStart, NULL);
		pCurServer = GetTargetServer(ulUin);
	
		// 发包	
		iSend1 = sendto(sock, g_achPkgSend, shPkgLen, 0, (struct sockaddr*)pCurServer, sizeof(struct sockaddr_in));
		
		if(iSend1!= shPkgLen)
		{			
			if(iSendTimes==0) // 还有第二次机会.
			{	
				++iSendTimes; // 发送包次数递增
				continue; 
			}
			else
			{
				snprintf(szErrMsg, iErrMsgLen-1, "sendto() error: %s\n", strerror(errno));
				close(sock);  // avoid leak socket resources
				return -1;
			}
		}
		++iSendTimes; // 发送包次数递增

		// 除以2是因为2台server,每个server分配一半的时间
		while( DiffTimeMsec(tvStart) < (iTimeout/2) )
		{
			memset(g_achPkgRecv, 0, sizeof(g_achPkgRecv));
			maxsock = sock+1;
			FD_ZERO(&rfds);
			FD_SET(sock, &rfds);
			tv.tv_sec 	= 0;
			tv.tv_usec 	= 50 * 1000; // 每次select 50 ms

			iRet = select(maxsock, &rfds, NULL, NULL, &tv);
			if(iRet <= 0)
			{
				continue;
			}
			if(FD_ISSET(sock, &rfds))
			{
				iRecvLen = recvfrom(sock, g_achPkgRecv, sizeof(g_achPkgRecv), 0, NULL, 0);

				if(iRecvLen < 0 )
				{
					continue;
				}
				if(!IsValidPkg(g_achPkgRecv, iRecvLen, CMD_SESSION_VERIFY2))
				{
					continue;
				}
				iRet = api_ses_parsepkg2(uiSeq, ulUin, pLogintime, pFace, pAge, pGender, szPassport, szNick, 
					szMail, pLastaccesstime, pResult, szErrMsg, iErrMsgLen); 
				if(0!= iRet)
				{
					continue;
				}

				bTimeout = false;
				break;
			}
			
		}
	}while((iSendTimes < 2) && bTimeout);
	
	close(sock);

	if(bTimeout)
	{
		snprintf(szErrMsg, iErrMsgLen-1, "Recv timeout");
		return -1;
	}
	
	return 0;
}

int
api_ses_logout(unsigned int uiSeq, int iAppID, unsigned long ulUin, unsigned long ulUserIP, const char* szKey, int iTimeout,
	char* pResult, char* szErrMsg, int iErrMsgLen)
{
	int		iRet			= -1;
	int		sock 			= -1;
//	char*	pCur 			= NULL;
	short	shPkgLen		= 0;
	int		iRecvLen		= 0;
	int 	iSend1 			= 0;
    struct timeval tv, tvStart;		// 开始调用时间
	fd_set	rfds;
	bool	bTimeout		= true;	// 是否超时flag
	int 	iSendTimes		= 0;    // 发包次数
	int		maxsock 		= -1;
	struct sockaddr_in*		pCurServer;

	shPkgLen = api_ses_composepkg(uiSeq, iAppID, ulUin, ulUserIP, szKey, CMD_SESSION_LOGOUT);
	
	sock = CreateUDPSocket(&g_sAddrLocal);
	if(sock < 0)
	{
		snprintf(szErrMsg, iErrMsgLen-1, "CreateUDPSocket Failed:%s\n", strerror(errno));
		return -1;
	}
	
#ifdef PTLOGIN2_SET_BLOCK	
	; 	// dummy
#else
	int iFlags = fcntl(sock, F_GETFL, 0);
	fcntl(sock, F_SETFL, iFlags | O_NONBLOCK);
	SetNBlock(sock);
#endif	
	
	do
	{
		memset(&tvStart, 0, sizeof(tvStart));
		gettimeofday(&tvStart, NULL);
		pCurServer = GetTargetServer(ulUin);
	
		// 发包	
		iSend1 = sendto(sock, g_achPkgSend, shPkgLen, 0, (struct sockaddr*)pCurServer, sizeof(struct sockaddr_in));
		if(iSend1!= shPkgLen)
		{			
			if(iSendTimes==0) // 还有第二次机会.
			{	
				++iSendTimes; // 发送包次数递增
				continue; 
			}
			else
			{
				snprintf(szErrMsg, iErrMsgLen-1, "sendto() error: %s\n", strerror(errno));
				close(sock);  // avoid leak socket resources
				return -1;
			}
		}
		++iSendTimes; // 发送包次数递增
			
		while( DiffTimeMsec(tvStart) < (iTimeout/2) )
		{
			memset(g_achPkgRecv, 0, sizeof(g_achPkgRecv));
			maxsock = sock+1;
			FD_ZERO(&rfds);
			FD_SET(sock, &rfds);
			tv.tv_sec 	= 0;
			tv.tv_usec 	= 50 * 1000; // 每次select 50 ms

			iRet = select(maxsock, &rfds, NULL, NULL, &tv);
			if(iRet <= 0)
			{
				continue;
			}
			if(FD_ISSET(sock, &rfds))
			{
				iRecvLen = recvfrom(sock, g_achPkgRecv, sizeof(g_achPkgRecv), 0, NULL, 0);

				if(iRecvLen < 0 )
				{
					continue;
				}
				if(!IsValidPkg(g_achPkgRecv, iRecvLen, CMD_SESSION_LOGOUT))
				{
					continue;
				}
				iRet = api_ses_parsepkg(uiSeq, ulUin, pResult, szErrMsg, iErrMsgLen) ;
				if(0!= iRet)
				{
					continue;
				}

				bTimeout = false;
				break;
			}
			
		}
	}while((iSendTimes < 2) && bTimeout);
	
	close(sock);

	if(bTimeout)
	{
		snprintf(szErrMsg, iErrMsgLen-1, "Recv timeout");
		return -1;
	}

	return 0;
}


