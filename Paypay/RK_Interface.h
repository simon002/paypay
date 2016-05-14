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

// ����ֻҪ���������Ľӿ�
class CRK_Interface
{
public:
	CRK_Interface();
	~CRK_Interface();

public:
	// �������,�����������Ҫ����
	// ����: lpSoftwareID:���ID; lpSoftwareKEY:���KEY; lpQuestionType:��Ŀ����;
	void RK_SoftInit(LPCTSTR lpSoftwareID,LPCTSTR lpSoftwareKEY,LPCTSTR lpQuestionType);
	// �û�����, �����������Ҫ����
	// ����: lpAccount:�û���; lpPassword:����;
	void RK_UserInit(LPCTSTR lpAccount,LPCTSTR lpPassword);

	// �ϴ�ͼƬ�ֽ�����
	// ����: lpBytePic--ͼƬ�ֽ�����; dwPicSize--�ֽ����鳤��; szResult--���ؽ��
	// ����: TRUE��szResult=��ȷ��Ϣ, ����szResult=������Ϣ.
	BOOL RK_UploadPicByByte(LPBYTE lpBytePic,DWORD dwPicSize,  string& szResult);
	// �ϴ�����ͼƬ
	// ����: lpLocalPicPathName:������֤��ͼƬȫ·��; szResult:���ؽ��;
	// ����: TRUE��szResult=��ȷ��Ϣ, ����szResult=������Ϣ.
	BOOL RK_UploadPicByLocal( LPCTSTR lpLocalPicPathName,  string& szResult);
	// �ϴ�URLͼƬ
	// ����: lpURLPic:������֤��ͼƬ��URL����; szResult:���ؽ��;
	// ����: TRUE��szResult=��ȷ��Ϣ, ����szResult=������Ϣ.
	BOOL RK_UploadPicByURL( LPCTSTR lpURLPic,  string& szResult);
	// �ύ������ID
	// ����: lpErrorID:���ID;
	// ����: ���ؽ����Ϣ.
	BOOL RK_SubmitErrorID( LPCTSTR lpErrorID,  string& szResult);

	// ע��
	// ����: lpAccount:ע���û���; lpPassword:ע������; lpEmail:ע������; szResult:���ؽ��;
	BOOL RK_Register( LPCTSTR lpAccount,  LPCTSTR lpPassword,  LPCTSTR lpEmail,  string& szResult);
	// ��ѯ
	// ����: lpAccount:�û���; lpPassword:����; szResult:���ؽ��;
	BOOL RK_Queryacct( LPCTSTR lpAccount,  LPCTSTR lpPassword,  string& szResult);
	// ��ֵ
	// ����: lpAccount:�û���; lpCardID:��ֵ����; lpCardPassword:��ֵ����; szResult:���ؽ��;
	BOOL RK_Recharge( LPCTSTR lpAccount,  LPCTSTR lpCardID,  LPCTSTR lpCardPassword,  string& szResult);

private:
	BOOL AnswerUploadPic(const string& fileName, const wstring& imagePath,  string& szResult);
	BOOL AnswerUploadPic(const string& fileName, const LPBYTE lpPicByte, DWORD dwPicSize,  string& szResult);
	BOOL SubmitPicURL(const string& imageURL,  string& szResult);
	BOOL ProcAnswerResult(const string& response,  string& szResult);
};


// �ڲ�������,���߲��õ���
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

// �ڲ�������,���߲��õ���
class CGlobalMgr
{
public:
	CGlobalMgr(void);
	~CGlobalMgr(void);

	static void PostToRuoKuai(const string& targetPage, const string& postData, string& rspMsg, bool& successful);

public:
	static string m_SoftwareID;			// ���ID
	static string m_SoftwareKEY;		// ���KEY
	static string m_QuestionType;		// ��Ŀ����
	static string m_Account;				// �ʺ�
	static string m_Password;			// ����
};

// �ڲ�������,���߲��õ���
#define HTTP_POST_GB2312	1	//���ͺͽ��յ���ҳ����
#define	HTTP_POST_UTF8		2

class CSendData
{
public:
	CSendData();
	virtual ~CSendData();
	void SetServerParam(CHAR ServerName[100], CHAR ObjectName[100], WORD Port);
	//����Э����������
	BOOL MakePostData(LPCTSTR pPostName, LPCTSTR pPostValue);
	BOOL MakePostData(LPCTSTR pPostName, int iPostValue);
	//���ʹ�ͼƬ������
	DWORD PostPicDataMethod(CHAR PicPath[MAX_PATH], LPCTSTR pPostName,
		LPTSTR *pRecvData, DWORD &RecvSize,
		int	SendCode = HTTP_POST_GB2312,//���ͺͽ��յı���
		int RecvCode = HTTP_POST_GB2312,
		BOOL bIsByte = FALSE,
		LPBYTE lpPicByte = NULL,
		DWORD dwPicByteSize = 0);
	//�����ı�������
	DWORD PostDataMethod(LPCTSTR strDataSend, int DataSize,
		LPTSTR *pRecvData, DWORD &RecvSize,
		int	SendCode = 1, int RecvCode = 1);
private:
	string			m_strBoundary;	//���ݷָ���
	string			m_strHeaderData;	//Э��ͷ����
	string			m_strPostData;	//Э����������
	string			m_strEndData;	//Э��β����

	void MakeHeaderData();	//����Э��ͷ������
	void MakeEndData();		//����Э��β������
	void MakePostPicData(LPCTSTR pPostName, LPCTSTR pPostValue);	//����ͼƬǰ׺����

	//����ֵΪͼƬ��С������ֵΪ0��ʾ�����ڡ�
	DWORD GetExistPic(CHAR PicPath[MAX_PATH]);
	//����������
	INTERNET_PORT m_nPort;
	CHAR m_tzObjectName[100];
	CHAR m_tzServerName[100];
	/////����ת��/////////////////////////////////////////////////////////////
	char	* m_lpUTF8;			//UTF8���뻺��ָ��
	char	* m_lpMultiByte;	//Multil���ֽڱ���ָ��
	wchar_t	* m_wpWideChar;		//Widechar���뻺��ָ��
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
