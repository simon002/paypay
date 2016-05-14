// RK_Interface.h: interface for the CRK_Interface class.
//
//////////////////////////////////////////////////////////////////////

//#if !defined(AFX_RK_INTERFACE_H__45EF8663_0A4D_4187_AF01_DFC790A5BF5C__INCLUDED_)
//#define AFX_RK_INTERFACE_H__45EF8663_0A4D_4187_AF01_DFC790A5BF5C__INCLUDED_
//
//#if _MSC_VER > 1000
//#pragma once
//#endif // _MSC_VER > 1000
#pragma once
#include <iostream>
#include <Windows.h>
//#include "winsock2.h"
#include "stdio.h"
#include "Wininet.h"
#pragma comment (lib,"Wininet.lib")
#include<algorithm>
using namespace std;

#pragma warning(disable:4996)

class CUtils;
class CGlobalMgr; 
class CSendData;

// 作者只要调用这个类的接口
class CRK_Interface
{
public:
	CRK_Interface();
	~CRK_Interface();

public:
	// 软件设置,这个函数必须要调用
	// 参数: lpSoftwareID:软件ID; lpSoftwareKEY:软件KEY; lpQuestionType:题目类型;
	void RK_SoftInit(LPCTSTR lpSoftwareID,LPCTSTR lpSoftwareKEY,LPCTSTR lpQuestionType);
	// 用户设置, 这个函数必须要调用
	// 参数: lpAccount:用户名; lpPassword:密码;
	void RK_UserInit(LPCTSTR lpAccount,LPCTSTR lpPassword);

	// 上传图片字节数组
	// 参数: lpBytePic--图片字节数组; dwPicSize--字节数组长度; szResult--返回结果
	// 返回: TRUE则szResult=正确信息, 否则szResult=错误信息.
	BOOL RK_UploadPicByByte(LPBYTE lpBytePic,DWORD dwPicSize,  string& szResult);
	// 上传本地图片
	// 参数: lpLocalPicPathName:本地验证码图片全路径; szResult:返回结果;
	// 返回: TRUE则szResult=正确信息, 否则szResult=错误信息.
	BOOL RK_UploadPicByLocal( LPCTSTR lpLocalPicPathName,  string& szResult);
	// 上传URL图片
	// 参数: lpURLPic:网络验证码图片的URL链接; szResult:返回结果;
	// 返回: TRUE则szResult=正确信息, 否则szResult=错误信息.
	BOOL RK_UploadPicByURL( LPCTSTR lpURLPic,  string& szResult);
	// 提交错误结果ID
	// 参数: lpErrorID:结果ID;
	// 返回: 返回结果信息.
	BOOL RK_SubmitErrorID( LPCTSTR lpErrorID,  string& szResult);

	// 注册
	// 参数: lpAccount:注册用户名; lpPassword:注册密码; lpEmail:注册邮箱; szResult:返回结果;
	BOOL RK_Register( LPCTSTR lpAccount,  LPCTSTR lpPassword,  LPCTSTR lpEmail,  string& szResult);
	// 查询
	// 参数: lpAccount:用户名; lpPassword:密码; szResult:返回结果;
	BOOL RK_Queryacct( LPCTSTR lpAccount,  LPCTSTR lpPassword,  string& szResult);
	// 充值
	// 参数: lpAccount:用户名; lpCardID:充值卡号; lpCardPassword:充值密码; szResult:返回结果;
	BOOL RK_Recharge( LPCTSTR lpAccount,  LPCTSTR lpCardID,  LPCTSTR lpCardPassword,  string& szResult);

private:
	BOOL AnswerUploadPic(const string& fileName, const wstring& imagePath,  string& szResult);
	BOOL AnswerUploadPic(const string& fileName, const LPBYTE lpPicByte, DWORD dwPicSize,  string& szResult);
	BOOL SubmitPicURL(const string& imageURL,  string& szResult);
	BOOL ProcAnswerResult(const string& response,  string& szResult);
};


// 内部功能类,作者不用调用
class CUtils
{
public:
	CUtils(void);
	~CUtils(void);

	static void PostRequest(const string& hostName, const string& pathName, const string& postData, string& response);
	static bool PostHttpPage(const string& hostName, const string& pathName, const string& postData, string& response);
	static void ParseResponse(const string& response, string& rspMsg, bool& successful);
	static void GetXMLFirstTagValue(const string& xmlStr, const string& tag, string& tagValue);
	static void ProcessRspMsg(const string& rspMsg, bool rspResult);
	static void ProcessChineseMessyCode(string& input);
	static void ProcessChineseMessyCodeW(string& input);
	static wstring  StringToWString(const string& s);
	static string WStringToString(const wstring& ws);

};

// 内部功能类,作者不用调用
class CGlobalMgr
{
public:
	CGlobalMgr(void);
	~CGlobalMgr(void);

	static void PostToRuoKuai(const string& targetPage, const string& postData, string& rspMsg, bool& successful);

public:
	static string m_SoftwareID;			// 软件ID
	static string m_SoftwareKEY;		// 软件KEY
	static string m_QuestionType;		// 题目类型
	static string m_Account;				// 帐号
	static string m_Password;			// 密码
};

// 内部功能类,作者不用调用
#define HTTP_POST_GB2312	1	//发送和接收的网页编码
#define	HTTP_POST_UTF8		2

class CSendData
{
public:
	CSendData();
	virtual ~CSendData();
	void SetServerParam(CHAR ServerName[100], CHAR ObjectName[100], WORD Port);
	//构造协议数据内容
	BOOL MakePostData(LPCTSTR pPostName, LPCTSTR pPostValue);
	BOOL MakePostData(LPCTSTR pPostName, int iPostValue);
	//发送带图片的数据
	DWORD PostPicDataMethod(CHAR PicPath[MAX_PATH], LPCTSTR pPostName,
		LPTSTR *pRecvData, DWORD &RecvSize,
		int	SendCode = HTTP_POST_GB2312,//发送和接收的编码
		int RecvCode = HTTP_POST_GB2312,
		BOOL bIsByte = FALSE,
		LPBYTE lpPicByte = NULL,
		DWORD dwPicByteSize = 0);
	//发送文本类数据
	DWORD PostDataMethod(LPCTSTR strDataSend, int DataSize,
		LPTSTR *pRecvData, DWORD &RecvSize,
		int	SendCode = 1, int RecvCode = 1);
private:
	string			m_strBoundary;	//数据分隔符
	string			m_strHeaderData;	//协议头数据
	string			m_strPostData;	//协议正文数据
	string			m_strEndData;	//协议尾数据

	void MakeHeaderData();	//构造协议头部数据
	void MakeEndData();		//构造协议尾部数据
	void MakePostPicData(LPCTSTR pPostName, LPCTSTR pPostValue);	//构造图片前缀数据

	//返回值为图片大小，返回值为0表示不存在。
	DWORD GetExistPic(CHAR PicPath[MAX_PATH]);
	//服务器参数
	INTERNET_PORT m_nPort;
	CHAR m_tzObjectName[100];
	CHAR m_tzServerName[100];
	/////编码转换/////////////////////////////////////////////////////////////
	char	* m_lpUTF8;			//UTF8编码缓存指针
	char	* m_lpMultiByte;	//Multil多字节编码指针
	wchar_t	* m_wpWideChar;		//Widechar编码缓存指针
	int		m_nUTF8Len;
	int		m_nMultiByteLen;
	int		m_nWideCharLen;

	void WideCharToUTF8(LPCWSTR wpWideData, int WideLen);
	void MultiByteToUTF8(LPCSTR lpMultiData, int MultiLen);
	void UTF8ToWideChar(LPCSTR pUTF8, int UTF8Len);
	void UTF8ToMultiByte(LPCSTR pUTF8, int UTF8Len);
	void MultiToWide(LPCSTR strDataSend, int DataSize);
	void MultiToMulti(LPCSTR strDataSend, int DataSize);
	void WideToMulti(LPCWSTR wpWideData, int WideLen);

};

//#endif // !defined(AFX_RK_INTERFACE_H__45EF8663_0A4D_4187_AF01_DFC790A5BF5C__INCLUDED_)
