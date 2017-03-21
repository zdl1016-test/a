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

// ͨ�ô����ͷ�ṹ
typedef struct
{
	unsigned short	ushPkgLen;		//  ����
	unsigned short	ushCommand;		//  �����
	unsigned short 	ushServerID;	
	unsigned long	ulClientIP;		// �ͻ�IP
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

///////////////// У��ͼƬ��֤��ʹ�� ///////////////////

/**********************************************************************
Function    : api_vc_init
Description : ��ʼ��api
Input		: szLocalIP 	-> ��������IP,�������NULLֵ, Ĭ��ѡ��ifconfig�õ��ĵ�һ������IPֵ
			: ushLocalPort 	-> ���������˿�, ��Ĭ����0����ϵͳ�Զ�����
			: szServerIP 	-> verify server �ӿ�1 IP
			: ushServerPort -> verify server �ӿ�1 �˿�
			: szServerIP2 	-> verify server �ӿ�2 IP
			: ushServerPort2-> verify server �ӿ�2 �˿�			
Output		: ��
Return		: 0 			-> ok
			: ����			-> fail
Calls       : 
Called By   : ��
Others		: ˫�ӿڣ������������һ̨, �����û�ָ���ĳ�ʱʱ���1/2ʱ����δ����, ����һ̨��������
**********************************************************************/
int api_vc_init(const char* szLocalIP, unsigned short ushLocalPort, 
	const char* szServerIP, unsigned short ushServerPort,
	const char* szServerIP2, unsigned short ushServerPort2);

/**********************************************************************
Function    : api_vc_verify
Description : ��֤��У��
Input		: szCode		-> �û��������֤��
			: iCodeLen		-> �û��������֤�볤��
			: szVerifySession -> �ͻ���(�����)��������verifysession��
			: iSessionLen	-> session����
			: iValidTime	-> ��֤��У����Ч��(���Ϊ20����)
			: uiUserIP		-> �û�����ҵ��CGI��IP, �����ֽ���
			: ushUserPort	-> �û�����ҵ��CGI�Ķ˿�
			: iAppID	 	-> ��ҵ���ڰ�ȫ���������ҵ��ID����ID����ȷ��������ͨ����֤
			: uiSeq		 	-> ����У���seqence��ÿ�η�����Ӧ���� 
			: chSendTimes	-> �ð����ʹ�����Ĭ��Ϊ0
			: iTimeout		-> ����У�鳬ʱ���룬eg:����ʱΪ1���룬iTimeout=1, ����ֵ>=500
			: iErrMsgLen	-> ������ʾ��ϢBUF�������� char szErrMsg[256];
Output		: pResult		-> ���صĽ��,VC_VERIFY_OK | VC_VERIFY_FAIL.
			: szErrMsg		-> ���صĴ�����ʾ��Ϣ
Return		: ���� 			-> ʧ��  , ����ʧ�ܻ����ʧ��
			: 0				-> 0�������ɹ�
Calls       : 
Called By   : ��
Others		: Returnֵ==0��ʾ��API���óɹ���ͨ�ųɹ�, ��(*pResult==VC_VERIFY_OK)�ű�ʾУ��ɹ�
			: ��֤���Ƿ�У��ɹ�Ӧ��ͬʱ��֤ (Returnֵ==0 )�� (*pResult==VC_VERIFY_OK)
			: ����,����Ҫע��pResult��char����, ������ʱӦ��char res = VC_VERIFY_FAIL;
			: ����api_vc_verifyʱͨ�� &res ���������.
			
// ptlogin2_apiĬ�Ͼ�Ϊudp��������ʽ�������հ�, ���������Ϊ������ʽ������,
// �����ڱ���ʱ ���� -DPTLOGIN2_SET_BLOCK ѡ��, e.g:
//  g++ -c -DPTLOGIN2_SET_BLOCK ptlogin2_api.cpp			
**********************************************************************/
int api_vc_verify(const char* szCode, int iCodeLen, const char* szVerifySession, 
	int iSessionLen, int iValidTime, unsigned int uiUserIP,unsigned short ushUserPort, 
	int iAppID, unsigned int uiSeq, char chSendTimes, int iTimeout, char* pResutl,
	char* szErrMsg, int iErrMsgLen);



///////////////////// У��������¼̬ʹ�� //////////////////////////////////

/**********************************************************************
Function    : api_ses_init
Description : ��ʼ��api
Input		: szLocalIP 	-> ��������IP,�������NULLֵ, Ĭ��ѡ��ifconfig�õ��ĵ�һ������IPֵ
			: ushLocalPort 	-> ���ض˿�, ��Ĭ��Ϊ0
			: szServerIP 	-> session server �ӿ�1 IP
			: ushServerPort -> session server �ӿ�1 �˿�
			: szServerIP2 	-> session server �ӿ�2 IP
			: ushServerPort2-> session server �ӿ�2 �˿�			
Output		: ��
Return		: 0 			-> ok
			: ����			-> fail
Calls       : 
Called By   : ��
Others		: ˫�ӿڣ������������һ̨, �����û�ָ���ĳ�ʱʱ���1/2ʱ����δ����, ����һ̨��������
**********************************************************************/
int api_ses_init(const char* szLocalIP, unsigned short ushLocalPort, 
	const char* szServerIP, unsigned short ushServerPort,
	const char* szServerIP2, unsigned short ushServerPort2);


/**********************************************************************
Function    : api_ses_verify
Description : ��session У��
Input		: uiSeq		 	-> ����У���seqence��ÿ�η�����Ӧ���� 
			: iAppID	 	-> ��ҵ���ڰ�ȫ���������ҵ��ID����ID����ȷ��������ͨ����֤
			: ulUin		 	-> ��У����û�����
			: ulUserIP		-> �û�IP, �����ֽ�ѧ
			: szKey		 	-> session key
			: iTimeout		-> ����У�鳬ʱ���룬eg:����ʱΪ1���룬iTimeout=1
			: iErrMsgLen	-> ������ʾ��ϢBUF�������� char szErrMsg[256];
Output		: pResult		-> ���صĽ��,SES_VERIFY_OK | SES_VERIFY_FAIL
			: szErrMsg		-> ���صĴ�����ʾ��Ϣ
Return		: 0 			-> ok  , �����ɹ�
			: ����			-> fail������ʧ��
Calls       : 
Called By   : ��
Others		: ������ֻ�����Ƿ�У��ɹ��������ؾ�����û���Ϣ

// ptlogin2_apiĬ�Ͼ�Ϊudp��������ʽ�������հ�, ���������Ϊ������ʽ������,
// �����ڱ���ʱ ���� -DPTLOGIN2_SET_BLOCK ѡ��, e.g:
//  g++ -c -DPTLOGIN2_SET_BLOCK ptlogin2_api.cpp		
**********************************************************************/

int api_ses_verify(unsigned int uiSeq, int iAppID, unsigned long ulUin, unsigned long ulUserIP, 
	const char* szKey, int iTimeout, char* pResult, char* szErrMsg, int iErrMsgLen);


/**********************************************************************
Function    : api_ses_verify2
Description : ���ӵ�session У��
Input		: uiSeq		 	-> ����У���seqence��ÿ�η�����Ӧ���� 
			: iAppID	 	-> ��ҵ���ڰ�ȫ���������ҵ��ID����ID����ȷ��������ͨ����֤
			: ulUin		 	-> ��У����û�����
			: ulUserIP		-> �û�����CGI��IP, �����ֽ���
			: szKey		 	-> session key
			: iTimeout		-> ����У�鳬ʱ���룬eg:����ʱΪ1���룬iTimeout=1,����ֵ>=500
			: iErrMsgLen	-> ������ʾ��ϢBUF�������� char szErrMsg[256];
Output		: pLogintime	-> ��¼ʱ��
			: pFace			-> ����  int face;
			: pAge			-> ����  char age;
			: pGender		-> �Ա�  char gender;
			: szPassport	-> ��ֵ����λ char passport[20];
			: szNick		-> �ǳ�  char nick[12+1];
			: szMail		-> ����  char mail[50+1];
			: pLastaccestime-> ������ʱ�� 
			: pResult		-> ���صĽ��,SES_VERIFY_OK | SES_VERIFY_FAIL
			: szErrMsg		-> ���صĴ�����ʾ��Ϣ
Return		: 0 			-> ok  , �����ɹ�
			: ����			-> fail������ʧ��
Calls       : 
Called By   : ��
Others		: ���������˷����Ƿ�У��ɹ��⣻�������û��Ļ���������Ϣ

// ptlogin2_apiĬ�Ͼ�Ϊudp��������ʽ�������հ�, ���������Ϊ������ʽ������,
// �����ڱ���ʱ ���� -DPTLOGIN2_SET_BLOCK ѡ��, e.g:
//  g++ -c -DPTLOGIN2_SET_BLOCK ptlogin2_api.cpp		
**********************************************************************/

int api_ses_verify2(unsigned int uiSeq,int iAppID, unsigned long ulUin, unsigned long ulUserIP, 
	const char* szKey, int iTimeout,  unsigned int* pLogintime, int* pFace, 
	char* pAge, char* pGender, char* szPassport, char* szNick, char* szMail,
	unsigned int* pLastaccesstime, char* pResult, char* szErrMsg, int iErrMsgLen);


/**********************************************************************
Function    : api_ses_logout
Description : ע�� �� �˳�
Input		: uiSeq		 	-> ����У���seqence��ÿ�η�����Ӧ���� 
			: iAppID	 	-> ��ҵ���ڰ�ȫ���������ҵ��ID����ID����ȷ��������ͨ����֤
			: ulUin		 	-> ��У����û�����
			: ulUserIP		-> �û�IP
			: szKey		 	-> session key
			: iTimeout		-> ����У�鳬ʱ���룬eg:����ʱΪ1���룬iTimeout=1
			: iErrMsgLen	-> ������ʾ��ϢBUF�������� char szErrMsg[256];
Output		: pResult		-> ���صĽ��,SES_LOGOUT_FAIL | SES_LOGOUT_OK | SES_LOGOUT_OFFLINE
			: szErrMsg		-> ���صĴ�����ʾ��Ϣ
Return		: 0 			-> ok  , �����ɹ�
			: ����			-> fail������ʧ��
Calls       : 
Called By   : ��
Others		: ������������û���server�˵�session

// ptlogin2_apiĬ�Ͼ�Ϊudp��������ʽ�������հ�, ���������Ϊ������ʽ������,
// �����ڱ���ʱ ���� -DPTLOGIN2_SET_BLOCK ѡ��, e.g:
//  g++ -c -DPTLOGIN2_SET_BLOCK ptlogin2_api.cpp		
**********************************************************************/
int api_ses_logout(unsigned int uiSeq, int iAppID, unsigned long ulUin, unsigned long ulUserIP, const char* szKey, int iTimeout,
	char* pResult, char* szErrMsg, int iErrMsgLen);

// Ӧ����ҵ��Ҫ�� �ṩһ����ȡseq��API
// 
unsigned int GetSeq();

#ifdef __cplusplus
}
#endif

#endif
