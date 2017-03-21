#ifndef _PTLOGIN2_API_H
#define _PTLOGIN2_API_H

#define STX						0x02
#define ETX						0x03
#define CMD_VERIFYCODE			0x01
#define CMD_SESSION_VERIFY		0x13
#define CMD_SESSION_VERIFY2		0X16
#define CMD_SESSION_LOGOUT		0x12

                                               
#define VERIFY_FAIL 		0                        
#define VERIFY_OK		  	1                        
                                               
#define       VC_VERIFY_FAIL       VERIFY_FAIL 
#define       VC_VERIFY_OK		   VERIFY_OK   
#define       SES_VERIFY_FAIL	   VERIFY_FAIL 
#define       SES_VERIFY_OK	       VERIFY_OK   
#define 	  SES_LOGOUT_FAIL	   VERIFY_FAIL 
#define 	  SES_LOGOUT_OK		   VERIFY_OK   
#define		  SES_LOGOUT_OFFLINE   2           


#ifdef __cplusplus
extern "C"{
#endif


#pragma pack(1)

// 通用传输包头结构
typedef struct
{
	unsigned short	ushPkgLen;		//  包长
	unsigned short	ushCommand;		//  命令号
	unsigned short 	ushServerID;	
	unsigned long	ulClientIP;		// 客户IP
	unsigned short	ushClientPort;		
	unsigned long	ulConnIP;		//
	unsigned short	ushConnPort;	//
	unsigned long 	ulInterfaceIP;	//
	unsigned short	ushInterfacePort;	//
	char	        chProcessSeq;	//
	unsigned char	chDbID;			//
	char	        sPad[2];       // Pad the same length as RelayPkgHead.    
} MYDBPkgHead;

#pragma pack()

///////////////// 校验图片验证码使用 ///////////////////

/**********************************************************************
Function    : api_vc_init
Description : 初始化api
Input		: szLocalIP 	-> 本地内网IP,如果传入NULL值, 默认选择ifconfig得到的第一个内网IP值
			: ushLocalPort 	-> 本地内网端口, 可默认填0，由系统自动分配
			: szServerIP 	-> verify server 接口1 IP
			: ushServerPort -> verify server 接口1 端口
			: szServerIP2 	-> verify server 接口2 IP
			: ushServerPort2-> verify server 接口2 端口			
Output		: 无
Return		: 0 			-> ok
			: 其他			-> fail
Calls       : 
Called By   : 无
Others		: 双接口，随机发送其中一台, 若在用户指定的超时时间的1/2时间内未返回, 向另一台发送请求
**********************************************************************/
int api_vc_init(const char* szLocalIP, unsigned short ushLocalPort, 
	const char* szServerIP, unsigned short ushServerPort,
	const char* szServerIP2, unsigned short ushServerPort2);

/**********************************************************************
Function    : api_vc_verify
Description : 验证码校验
Input		: szCode		-> 用户输入的验证码
			: iCodeLen		-> 用户输入的验证码长度
			: szVerifySession -> 客户端(浏览器)带上来的verifysession串
			: iSessionLen	-> session串长
			: iValidTime	-> 验证码校验有效期(最大为20分钟)
			: uiUserIP		-> 用户连接业务CGI的IP, 主机字节序
			: ushUserPort	-> 用户连接业务CGI的端口
			: iAppID	 	-> 本业务在安全中心申请的业务ID，该ID不正确，将不能通过验证
			: uiSeq		 	-> 发送校验的seqence，每次发送完应递增 
			: chSendTimes	-> 该包发送次数，默认为0
			: iTimeout		-> 设置校验超时毫秒，eg:若超时为1毫秒，iTimeout=1, 建议值>=500
			: iErrMsgLen	-> 错误提示消息BUF长，建议 char szErrMsg[256];
Output		: pResult		-> 返回的结果,VC_VERIFY_OK | VC_VERIFY_FAIL.
			: szErrMsg		-> 返回的错误提示消息
Return		: 其他 			-> 失败  , 操作失败或参数失败
			: 0				-> 0，操作成功
Calls       : 
Called By   : 无
Others		: Return值==0表示该API调用成功和通信成功, 而(*pResult==VC_VERIFY_OK)才表示校验成功
			: 验证码是否校验成功应该同时保证 (Return值==0 )和 (*pResult==VC_VERIFY_OK)
			: 另外,这里要注意pResult是char类型, 即申明时应该char res = VC_VERIFY_FAIL;
			: 传入api_vc_verify时通过 &res 来获得其结果.
			
// ptlogin2_api默认均为udp非阻塞方式发包和收包, 如果有设置为阻塞方式的需求,
// 可以在编译时 设置 -DPTLOGIN2_SET_BLOCK 选项, e.g:
//  g++ -c -DPTLOGIN2_SET_BLOCK ptlogin2_api.cpp			
**********************************************************************/
int api_vc_verify(const char* szCode, int iCodeLen, const char* szVerifySession, 
	int iSessionLen, int iValidTime, unsigned int uiUserIP,unsigned short ushUserPort, 
	int iAppID, unsigned int uiSeq, char chSendTimes, int iTimeout, char* pResutl,
	char* szErrMsg, int iErrMsgLen);



///////////////////// 校验社区登录态使用 //////////////////////////////////

/**********************************************************************
Function    : api_ses_init
Description : 初始化api
Input		: szLocalIP 	-> 本地内网IP,如果传入NULL值, 默认选择ifconfig得到的第一个内网IP值
			: ushLocalPort 	-> 本地端口, 可默认为0
			: szServerIP 	-> session server 接口1 IP
			: ushServerPort -> session server 接口1 端口
			: szServerIP2 	-> session server 接口2 IP
			: ushServerPort2-> session server 接口2 端口			
Output		: 无
Return		: 0 			-> ok
			: 其他			-> fail
Calls       : 
Called By   : 无
Others		: 双接口，随机发送其中一台, 若在用户指定的超时时间的1/2时间内未返回, 向另一台发送请求
**********************************************************************/
int api_ses_init(const char* szLocalIP, unsigned short ushLocalPort, 
	const char* szServerIP, unsigned short ushServerPort,
	const char* szServerIP2, unsigned short ushServerPort2);


/**********************************************************************
Function    : api_ses_verify
Description : 简单session 校验
Input		: uiSeq		 	-> 发送校验的seqence，每次发送完应递增 
			: iAppID	 	-> 本业务在安全中心申请的业务ID，该ID不正确，将不能通过验证
			: ulUin		 	-> 被校验的用户号码
			: ulUserIP		-> 用户IP, 主机字节学
			: szKey		 	-> session key
			: iTimeout		-> 设置校验超时毫秒，eg:若超时为1毫秒，iTimeout=1
			: iErrMsgLen	-> 错误提示消息BUF长，建议 char szErrMsg[256];
Output		: pResult		-> 返回的结果,SES_VERIFY_OK | SES_VERIFY_FAIL
			: szErrMsg		-> 返回的错误提示消息
Return		: 0 			-> ok  , 操作成功
			: 其他			-> fail，操作失败
Calls       : 
Called By   : 无
Others		: 本函数只返回是否校验成功，不返回具体的用户信息

// ptlogin2_api默认均为udp非阻塞方式发包和收包, 如果有设置为阻塞方式的需求,
// 可以在编译时 设置 -DPTLOGIN2_SET_BLOCK 选项, e.g:
//  g++ -c -DPTLOGIN2_SET_BLOCK ptlogin2_api.cpp		
**********************************************************************/

int api_ses_verify(unsigned int uiSeq, int iAppID, unsigned long ulUin, unsigned long ulUserIP, 
	const char* szKey, int iTimeout, char* pResult, char* szErrMsg, int iErrMsgLen);


/**********************************************************************
Function    : api_ses_verify2
Description : 复杂的session 校验
Input		: uiSeq		 	-> 发送校验的seqence，每次发送完应递增 
			: iAppID	 	-> 本业务在安全中心申请的业务ID，该ID不正确，将不能通过验证
			: ulUin		 	-> 被校验的用户号码
			: ulUserIP		-> 用户访问CGI的IP, 主机字节序
			: szKey		 	-> session key
			: iTimeout		-> 设置校验超时毫秒，eg:若超时为1毫秒，iTimeout=1,建议值>=500
			: iErrMsgLen	-> 错误提示消息BUF长，建议 char szErrMsg[256];
Output		: pLogintime	-> 登录时间
			: pFace			-> 脸谱  int face;
			: pAge			-> 年龄  char age;
			: pGender		-> 性别  char gender;
			: szPassport	-> 增值中心位 char passport[20];
			: szNick		-> 昵称  char nick[12+1];
			: szMail		-> 邮箱  char mail[50+1];
			: pLastaccestime-> 最后访问时间 
			: pResult		-> 返回的结果,SES_VERIFY_OK | SES_VERIFY_FAIL
			: szErrMsg		-> 返回的错误提示消息
Return		: 0 			-> ok  , 操作成功
			: 其他			-> fail，操作失败
Calls       : 
Called By   : 无
Others		: 本函数除了返回是否校验成功外；还返回用户的基本资料信息

// ptlogin2_api默认均为udp非阻塞方式发包和收包, 如果有设置为阻塞方式的需求,
// 可以在编译时 设置 -DPTLOGIN2_SET_BLOCK 选项, e.g:
//  g++ -c -DPTLOGIN2_SET_BLOCK ptlogin2_api.cpp		
**********************************************************************/

int api_ses_verify2(unsigned int uiSeq,int iAppID, unsigned long ulUin, unsigned long ulUserIP, 
	const char* szKey, int iTimeout,  unsigned int* pLogintime, int* pFace, 
	char* pAge, char* pGender, char* szPassport, char* szNick, char* szMail,
	unsigned int* pLastaccesstime, char* pResult, char* szErrMsg, int iErrMsgLen);


/**********************************************************************
Function    : api_ses_logout
Description : 注销 或 退出
Input		: uiSeq		 	-> 发送校验的seqence，每次发送完应递增 
			: iAppID	 	-> 本业务在安全中心申请的业务ID，该ID不正确，将不能通过验证
			: ulUin		 	-> 被校验的用户号码
			: ulUserIP		-> 用户IP
			: szKey		 	-> session key
			: iTimeout		-> 设置校验超时毫秒，eg:若超时为1毫秒，iTimeout=1
			: iErrMsgLen	-> 错误提示消息BUF长，建议 char szErrMsg[256];
Output		: pResult		-> 返回的结果,SES_LOGOUT_FAIL | SES_LOGOUT_OK | SES_LOGOUT_OFFLINE
			: szErrMsg		-> 返回的错误提示消息
Return		: 0 			-> ok  , 操作成功
			: 其他			-> fail，操作失败
Calls       : 
Called By   : 无
Others		: 本函数将清除用户在server端的session

// ptlogin2_api默认均为udp非阻塞方式发包和收包, 如果有设置为阻塞方式的需求,
// 可以在编译时 设置 -DPTLOGIN2_SET_BLOCK 选项, e.g:
//  g++ -c -DPTLOGIN2_SET_BLOCK ptlogin2_api.cpp		
**********************************************************************/
int api_ses_logout(unsigned int uiSeq, int iAppID, unsigned long ulUin, unsigned long ulUserIP, const char* szKey, int iTimeout,
	char* pResult, char* szErrMsg, int iErrMsgLen);

// 应多数业务要求， 提供一个获取seq的API
// 
unsigned int GetSeq();

#ifdef __cplusplus
}
#endif

#endif
