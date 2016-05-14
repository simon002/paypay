#include "stdafx.h"
#include "RK_Interface.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


//========================= CRK_Interface�ӿ���(����ֻҪ��ע�����) =============================
CRK_Interface::CRK_Interface()
{
	// �޸�Ĭ��ϵͳ������
	ULONG nMaxConnect = 200;
	InternetSetOption(NULL, 73, &nMaxConnect, sizeof(nMaxConnect));
	InternetSetOption(NULL, 74, &nMaxConnect, sizeof(nMaxConnect));

	//CGlobalMgr::m_SoftwareID = "1";
	//CGlobalMgr::m_SoftwareKEY = "b40ffbee5c1cf4e38028c197eb2fc751";
}


CRK_Interface::~CRK_Interface()
{
}

void CRK_Interface::RK_SoftInit(LPCTSTR lpSoftwareID, LPCTSTR lpSoftwareKEY, LPCTSTR lpQuestionType)
{
	CGlobalMgr::m_SoftwareID = CUtils::WStringToString(lpSoftwareID);
	CGlobalMgr::m_SoftwareKEY = CUtils::WStringToString(lpSoftwareKEY);
	CGlobalMgr::m_QuestionType = CUtils::WStringToString(lpQuestionType);
}

void CRK_Interface::RK_UserInit(LPCTSTR lpAccount, LPCTSTR lpPassword)
{
	CGlobalMgr::m_Account = CUtils::WStringToString(lpAccount);
	CGlobalMgr::m_Password = CUtils::WStringToString(lpPassword);
}

BOOL CRK_Interface::RK_UploadPicByByte( LPBYTE lpBytePic,  DWORD dwPicSize,  string& szResult)
{
	return AnswerUploadPic("image", lpBytePic, dwPicSize, szResult);
}

BOOL CRK_Interface::RK_UploadPicByLocal(LPCTSTR lpLocalPicPathName,  string& szResult)
{
	
	return AnswerUploadPic("image", lpLocalPicPathName, szResult);
}

BOOL CRK_Interface::RK_UploadPicByURL(LPCTSTR lpURLPic,  string& szResult)
{
	return SubmitPicURL(CUtils::WStringToString(lpURLPic), szResult);
}

BOOL CRK_Interface::RK_SubmitErrorID(LPCTSTR lpErrorID,  string& szResult)
{
	/*
	http://api.ruokuai.com/reporterror.json
	{"username"}
	{"password"}
	{"id"}
	*/

	if (string((LPSTR)lpErrorID).empty())
	{
		szResult = "����ID����Ϊ��.";
		return FALSE;
	}
	string id = "";
	string ret = "";

	char postData[256] = { 0x0 };
	sprintf(postData, "username=%s&password=%s&id=%s", CGlobalMgr::m_Account.c_str(), CGlobalMgr::m_Password.c_str(),CUtils::WStringToString(lpErrorID).c_str());

	string rspMsg = "";
	bool successful = false;
	CGlobalMgr::PostToRuoKuai("reporterror.xml", postData, rspMsg, successful);

	if (rspMsg.empty())
	{

		ret = "������û����Ӧ��";
	}
	else
	{
		ret = successful ? "" : "";
		ret += rspMsg.c_str();
	}

	szResult = ret;
	return successful;
}

BOOL CRK_Interface::RK_Register( LPCTSTR lpAccount,  LPCTSTR lpPassword,  LPCTSTR lpEmail,  string& szResult)
{
	/*
	http://api.ruokuai.com/register.xml
	{"username"}
	{"password"}
	{"email"}
	*/
	string ret = "";

	if (string((LPSTR)lpAccount).empty() || string((LPSTR)lpPassword).empty() || string((LPSTR)lpEmail).empty())
	{
		szResult = "�û����������ע�����䲻��Ϊ��.";
		return FALSE;
	}

	char postData[256] = { 0x0 };
	sprintf(postData, "username=%s&password=%s&email=%s", CUtils::WStringToString(lpAccount).c_str(), CUtils::WStringToString(lpPassword).c_str(), CUtils::WStringToString(lpEmail).c_str());

	string rspMsg = "";
	bool successful = false;
	CGlobalMgr::PostToRuoKuai("register.xml", postData, rspMsg, successful);

	if (rspMsg.empty())
	{
		ret = "������û����Ӧ��";
	}
	else
	{
		ret = successful ? "" : "";
		ret += rspMsg.c_str();
	}

	szResult = ret;
	return successful;
}

BOOL CRK_Interface::RK_Queryacct( LPCTSTR lpAccount,  LPCTSTR lpPassword,  string& szResult)
{
	/*
	http://api.ruokuai.com/info.xml
	{"username"}
	{"password"}
	*/

	if (string((LPSTR)lpAccount).empty() || string((LPSTR)lpPassword).empty())
	{
		szResult = "�û��������벻��Ϊ��.";
		return FALSE;
	}

	BOOL bSuccess = FALSE;
	char postData[256] = { 0x0 };
	sprintf(postData, "username=%s&password=%s", lpAccount, lpPassword);

	string response = "";
	CUtils::PostRequest("api.ruokuai.com", "info.xml", postData, response);

	string msg = "";
	if (response.find("<Error>") != string::npos)
	{
		CUtils::GetXMLFirstTagValue(response, "Error", msg);
		bSuccess = FALSE;
	}
	else
	{
		CUtils::GetXMLFirstTagValue(response, "Root", msg);
		bSuccess = TRUE;
	}

	szResult = msg.c_str();
	return bSuccess;
}

BOOL CRK_Interface::RK_Recharge( LPCTSTR lpAccount,  LPCTSTR lpCardID,  LPCTSTR lpCardPassword,  string& szResult)
{
	/*
	http://api.ruokuai.com/recharge.xml
	{"username"}
	{"id"}
	{"password"}
	*/
	string ret = "";

	if (string((LPSTR)lpAccount).empty() || string((LPSTR)lpCardID).empty() || string((LPSTR)lpCardPassword).empty())
	{
		szResult = "�û������ֵ���Ż��ֵ���벻��Ϊ��";
		return FALSE;
	}

	char postData[256] = { 0x0 };
	sprintf(postData, "username=%s&id=%s&password=%s", CUtils::WStringToString(lpAccount).c_str(), CUtils::WStringToString(lpCardID).c_str(), CUtils::WStringToString(lpCardPassword).c_str());

	string rspMsg = "";
	bool successful = false;
	CGlobalMgr::PostToRuoKuai("recharge.xml", postData, rspMsg, successful);

	if (rspMsg.empty())
	{
		ret = "������û����Ӧ��";
	}
	else
	{
		ret = successful ? "" : "";
		ret += rspMsg.c_str();
	}

	szResult = ret;
	return successful;
}

BOOL CRK_Interface::AnswerUploadPic(const string& fileName, const LPBYTE lpPicByte, DWORD dwPicSize,  string& szResult)
{
	if (lpPicByte == NULL)
	{
		szResult = "ָ��ͼƬ�ֽ������ָ��Ϊ��.";
		return FALSE;
	}

	if (dwPicSize == 0)
	{
		szResult = "ͼƬ�ֽ����鳤��Ϊ0.";
		return FALSE;
	}

	//���巢����
	CSendData	MySend;
	TCHAR		*pRecvData = NULL;	//�������ݵ�ָ��
	DWORD		dwRecvSize = 0;		//�������ݵĴ�С
	DWORD		dwRetSend = 0;		//���ͺ�������ֵ
	int			SendCode, RecvCode;	//��������յı���

	/*SendCode = HTTP_POST_GB2312;
	RecvCode = HTTP_POST_GB2312;*/
	SendCode = HTTP_POST_UTF8;
	RecvCode = HTTP_POST_UTF8;

	//���÷���������
	MySend.SetServerParam("api.ruokuai.com","create.xml", INTERNET_DEFAULT_HTTP_PORT);

	//�����ȹ������ݰ�
	MySend.MakePostData(CUtils::StringToWString("username").c_str(), CUtils::StringToWString(CGlobalMgr::m_Account).c_str());
	MySend.MakePostData(CUtils::StringToWString("password").c_str(), CUtils::StringToWString(CGlobalMgr::m_Password).c_str());
	MySend.MakePostData(CUtils::StringToWString("typeid").c_str(), CUtils::StringToWString(CGlobalMgr::m_QuestionType).c_str());
	MySend.MakePostData(CUtils::StringToWString("timeout").c_str(), CUtils::StringToWString("90").c_str());
	MySend.MakePostData(CUtils::StringToWString("softid").c_str(), CUtils::StringToWString(CGlobalMgr::m_SoftwareID).c_str());
	MySend.MakePostData(CUtils::StringToWString("softkey").c_str(), CUtils::StringToWString(CGlobalMgr::m_SoftwareKEY).c_str());

	//����
	dwRetSend = MySend.PostPicDataMethod(NULL, CUtils::StringToWString(fileName).c_str(), &pRecvData, dwRecvSize, SendCode, RecvCode, TRUE, lpPicByte, dwPicSize);
	string recvStr = (pRecvData == NULL) ? "" : CUtils::WStringToString(pRecvData);
	CUtils::ProcessChineseMessyCodeW(recvStr);
	return ProcAnswerResult(recvStr, szResult);
}

BOOL CRK_Interface::AnswerUploadPic(const string& fileName, const wstring& imagePath,  string& szResult)
{
	/*
	http://api.ruokuai.com/create.xml
	{"username"}
	{"password"}
	{"typeid"}
	{"timeout"}
	{"softid"}
	{"softkey"}
	*/

	if (imagePath.empty())
	{
		szResult = "ͼƬ·������Ϊ��.";
		return FALSE;
	}

	//���巢����
	CSendData	MySend;
	TCHAR		*pRecvData = NULL;	//�������ݵ�ָ��
	DWORD		dwRecvSize = 0;		//�������ݵĴ�С
	DWORD		dwRetSend = 0;		//���ͺ�������ֵ
	int			SendCode, RecvCode;	//��������յı���

	/*SendCode = HTTP_POST_GB2312;
	RecvCode = HTTP_POST_GB2312;*/
	SendCode = HTTP_POST_UTF8;
	RecvCode = HTTP_POST_UTF8;

	//���÷���������
	MySend.SetServerParam("api.ruokuai.com","create.xml", INTERNET_DEFAULT_HTTP_PORT);

	//�����ȹ������ݰ�
	MySend.MakePostData(CUtils::StringToWString("username").c_str(), CUtils::StringToWString(CGlobalMgr::m_Account).c_str());
	MySend.MakePostData(CUtils::StringToWString("password").c_str(), CUtils::StringToWString(CGlobalMgr::m_Password).c_str());
	MySend.MakePostData(CUtils::StringToWString("typeid").c_str(), CUtils::StringToWString(CGlobalMgr::m_QuestionType).c_str());
	MySend.MakePostData(CUtils::StringToWString("timeout").c_str(), CUtils::StringToWString("90").c_str());
	MySend.MakePostData(CUtils::StringToWString("softid").c_str(), CUtils::StringToWString(CGlobalMgr::m_SoftwareID).c_str());
	MySend.MakePostData(CUtils::StringToWString("softkey").c_str(), CUtils::StringToWString(CGlobalMgr::m_SoftwareKEY).c_str());
	//����
		dwRetSend = MySend.PostPicDataMethod((LPSTR)(LPCSTR)CUtils::WStringToString(imagePath).c_str(),CUtils::StringToWString(fileName).c_str(), &pRecvData, dwRecvSize, SendCode, RecvCode);
		string recvStr = (pRecvData == NULL) ? "" : CUtils::WStringToString(pRecvData);
	CUtils::ProcessChineseMessyCodeW(recvStr);
	return ProcAnswerResult(recvStr, szResult);
}

BOOL CRK_Interface::SubmitPicURL(const string& imageURL,  string& szResult)
{
	/*
	http://api.ruokuai.com/create.xml
	{"username"}
	{"password"}
	{"typeid"}
	{"timeout"}
	{"softid"}
	{"softkey"}
	{"imageurl"}
	*/

	if (imageURL.c_str()=="")
	{
		szResult = "URL·������Ϊ��.";
		return FALSE;
	}

	char postData[256] = { 0x0 };
	sprintf(postData, "username=%s&password=%s&typeid=%s&timeout=%d&softid=%s&softkey=%s&imageurl=%s",
		CGlobalMgr::m_Account.c_str(), CGlobalMgr::m_Password.c_str(), CGlobalMgr::m_QuestionType.c_str(), 90, CGlobalMgr::m_SoftwareID.c_str(), CGlobalMgr::m_SoftwareKEY.c_str(), imageURL.c_str());

	string response;
	CUtils::PostRequest("api.ruokuai.com", "create.xml", postData, response);

	return ProcAnswerResult(response, szResult);
}

BOOL CRK_Interface::ProcAnswerResult(const string& response,  string& szResult)
{
	string shownStr = "";

	if (response.empty())
	{
		shownStr = "������û����Ӧ��";
	}
	else if (response.find("<Error>") != string::npos)
	{
		;
		CUtils::GetXMLFirstTagValue(response, "Error", shownStr);
	}
	else
	{
		string tempStr, resultMsg, id;
		CUtils::GetXMLFirstTagValue(response, "Root", tempStr);
		CUtils::GetXMLFirstTagValue(tempStr, "Result", resultMsg);
		CUtils::GetXMLFirstTagValue(tempStr, "Id", id);
		szResult = tempStr.c_str();

		return TRUE;
	}

	szResult = shownStr.c_str();
	return FALSE;
}

//========================= CUtils������ =============================
CUtils::CUtils(void)
{
}

CUtils::~CUtils(void)
{
}

/*
//��http://current.sinaapp.com/post.php��������
PostHttpPage("current.sinaapp.com","post.php","name=rain&age=12");
*/
bool CUtils::PostHttpPage(const string& hostName, const string& pathName, const string& postData, string& response)
{ 
	INTERNET_PORT nPort = 80;
	DWORD dwRet = 0;
	HINTERNET hInternet = InternetOpen(CUtils::StringToWString("RuoKuaiCPP").c_str(),INTERNET_OPEN_TYPE_PRECONFIG,NULL, NULL,NULL);   
	//���Ծ��
	if(hInternet < 0 )
	{
		return false;
	}
	//��������Ự
    HINTERNET hSession = InternetConnect(hInternet,CUtils::StringToWString(hostName).c_str(),nPort,NULL,NULL,INTERNET_SERVICE_HTTP,0,0);   
	if(hSession < 0 )
	{
		return false;
	}
    char* szAccept[] = {"*/*",NULL};
    string strVerb; 
	//������������
	HINTERNET hRequest = HttpOpenRequest(hSession,CUtils::StringToWString("POST").c_str(),CUtils::StringToWString(pathName).c_str(),NULL,NULL,(LPCWSTR*)(LPCSTR*)szAccept,0,0); 
	if(hRequest < 0 )
      {
          return false;
      }
	string strHeaders = "Content-Type: application/x-www-form-urlencoded";
	//��������
	 if(!HttpSendRequest(hRequest,CUtils::StringToWString(strHeaders.c_str()).c_str(),-1,(LPVOID)postData.c_str(),postData.size()))
	{
		return false;
	}
    //��������
    LPSTR lpszData;        //buffer for the data   
    DWORD dwSize;        //  
    DWORD dwDownloaded; //size of the downloaded data 
	while(1)   
    {          
        // ѯ���Ƿ��п�������
        if (!InternetQueryDataAvailable(hRequest,&dwSize,0,0))   
        {   
			AfxMessageBox(_T("afdfasd"));
            break;   
        }   
        else   
        {     
            // ������ȡ�ļ���������   
            lpszData = new char[dwSize+1]; 
            
            // ��ȡ�ļ�
            if(!InternetReadFile(hRequest,(LPVOID)lpszData,dwSize,&dwDownloaded))   
            {  
                delete[] lpszData;   
                break;   
            }   
            else   
            {   
                //��ӽ�����
                lpszData[dwDownloaded]='\0';   
                response = lpszData;             
	            //ɾ����������
                delete[] lpszData;   
                break; 
                if (dwDownloaded == 0)   
                {
                    break;   
                }
            }
        }   
    } 
	 // �ر�����Ự
	InternetCloseHandle(hRequest);   
    InternetCloseHandle(hSession);   
    InternetCloseHandle(hInternet); 

	return true;
}

void CUtils::PostRequest(const string& hostName, const string& pathName, const string& postData, string& response)
{
	PostHttpPage(hostName, pathName, postData, response);

	ProcessChineseMessyCode(response);
}

void CUtils::ProcessChineseMessyCode(string& input)
{
	if (input.empty()) return;
	string str=input;
	str.resize(str.size()); //ȡ��str�����ԭʼ�ַ���
	int nBufferSize = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0); //ȡ�����軺��Ķ���
	wchar_t *pBuffer = (wchar_t*)malloc(nBufferSize * sizeof(wchar_t));//���뻺��ռ�
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, pBuffer, nBufferSize*sizeof(wchar_t));//ת��
	char *psText;
	psText = new char[nBufferSize + 1];
	memset(psText, 0, nBufferSize + 1);
	WideCharToMultiByte(CP_OEMCP, NULL, pBuffer, -1, psText, nBufferSize, NULL, FALSE);
	input="";
	input = psText;

	delete[]psText;
	free(pBuffer); //�ͷŻ���
}
//////////////////////////////////////////////////////////////////////////////
//���������ϴ�ͼƬת��
//////////////////////////////////////////////////////////////////////////////
void CUtils::ProcessChineseMessyCodeW(string& input)
{
	if (input.empty()) return;
	string str=input;
	str.resize(str.size()); //ȡ��str�����ԭʼ�ַ���
	int nBufferSize = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0); //ȡ�����軺��Ķ���
	wchar_t *pBuffer = (wchar_t*)malloc(nBufferSize * sizeof(wchar_t));//���뻺��ռ�
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, pBuffer, nBufferSize*sizeof(wchar_t));//ת��
	free(pBuffer); //�ͷŻ���
}

void CUtils::ParseResponse(const string& response, string& rspMsg, bool& successful)
{
	if (response.empty())
	{
		successful = false;
		return;
	}

	string targetTag = "";
	//string::size_type pos;
	if (response.find("<Result>") != string::npos)
	{
		successful = true;
		targetTag = "Result";
	}
	else if (response.find("<Error>") != string::npos)
	{
		successful = false;
		targetTag = "Error";
	}
	GetXMLFirstTagValue(response, targetTag, rspMsg);
}

//xmlStr = ....<tag>tagValue</tag>....
void CUtils::GetXMLFirstTagValue(const string& xmlStr, const string& tag, string& tagValue)
{
	string tagStart = "";
	tagStart.append("<");
	tagStart.append(tag);
	tagStart.append(">");

	string tagEnd = "";
	tagEnd.append("</");
	tagEnd.append(tag);
	tagEnd.append(">");

	string::size_type pos = xmlStr.find(tagStart);
	if (pos != string::npos)
	{
		pos += tagStart.length();
		string tempStr = xmlStr.substr(pos, xmlStr.length() - pos);
		pos = tempStr.find(tagEnd);
		tagValue = tempStr.substr(0, pos);
	}
}

void CUtils::ProcessRspMsg(const string& rspMsg, bool rspResult)
{
	string ret = "";

	if (rspMsg.empty())
	{
		ret = "������û����Ӧ��";
	}
	else
	{
		ret = rspResult ? "" : "ʧ��: ";
		ret.append(rspMsg);
	}
}

wstring  CUtils::StringToWString(const string& s)
{
     wstring wszStr;
 
     int nLength = MultiByteToWideChar( CP_ACP, 0, s.c_str(), -1, NULL, NULL );
     wszStr.resize(nLength);
     LPWSTR lpwszStr = new wchar_t[nLength];
     MultiByteToWideChar( CP_ACP, 0, s.c_str(), -1, lpwszStr, nLength );
     wszStr = lpwszStr;
     delete [] lpwszStr;
 
     return wszStr;
}

string CUtils::WStringToString(const wstring& ws)
{
    string curLocale = setlocale(LC_ALL, NULL);        // curLocale = "C";
    setlocale(LC_ALL, "chs");
    const wchar_t* _Source = ws.c_str();
    size_t _Dsize = 2 * ws.size() + 1;
    char *_Dest = new char[_Dsize];
    memset(_Dest,0,_Dsize);
    wcstombs(_Dest,_Source,_Dsize);
    string result = _Dest;
    delete []_Dest;
    setlocale(LC_ALL, curLocale.c_str());
    return result;
}

//========================= CGlobalMgr ���ͻ��������� =============================
string CGlobalMgr::m_SoftwareID = "";
string CGlobalMgr::m_SoftwareKEY = "";
string CGlobalMgr::m_QuestionType = "";
string CGlobalMgr::m_Account = "";
string CGlobalMgr::m_Password = "";

CGlobalMgr::CGlobalMgr(void)
{
}

CGlobalMgr::~CGlobalMgr(void)
{
}

void CGlobalMgr::PostToRuoKuai(const string& targetPage, const string& postData, string& rspMsg, bool& successful)
{
	string response;
	CUtils::PostRequest("api.ruokuai.com", targetPage, postData, response);
	CUtils::ParseResponse(response, rspMsg, successful);
}


//========================= CSendData����ͼƬ������ =============================
CSendData::CSendData()
{
	m_lpUTF8 = NULL;		//UTF8���뻺��ָ��
	m_lpMultiByte = NULL;		//
	m_wpWideChar = NULL;		//Widechar���뻺��ָ��
	//������Ĭ��ֵ
	strcpy(m_tzServerName, "api.ruokuai.com");
	strcpy(m_tzObjectName, "/create.xml");
	m_nPort = INTERNET_DEFAULT_HTTP_PORT;

	m_strBoundary = "-------------RuoKuaiCPP";
}

CSendData::~CSendData()
{
	//���ԭ�ȵ�����û��ɾ��
	if (m_lpUTF8)
	{
		delete[] m_lpUTF8;
		m_lpUTF8 = NULL;
	}
	//���ԭ�ȵ�����û��ɾ��
	if (m_lpMultiByte)
	{
		delete[] m_lpMultiByte;
		m_lpMultiByte = NULL;
	}
	//���ֽڴ���ָ��
	if (m_wpWideChar)
	{
		delete[]m_wpWideChar;
		m_wpWideChar = NULL;
	}

}
//���÷���������
void CSendData::SetServerParam(CHAR ServerName[], CHAR ObjectName[], WORD Port)
{
	strcpy(m_tzServerName, ServerName);
	strcpy(m_tzObjectName, ObjectName);
	m_nPort = Port;
}
//��������
//���������1��ʾGB2312��2��ʾUTF8
DWORD CSendData::PostDataMethod(LPCTSTR strDataSend,
	int DataSize,
	LPTSTR *pRecvData,
	DWORD &RecvSize,
	int SendCode,	//����ʱ�ı���
	int RecvCode	//���շ���ʱ�ı���
	)
{
	//���ش�������б�
	//100�������ɹ�
	//101���������޷�����
	//102���ύҳ���޷���
	//103�����ݷ���ʧ��
	//104������������ʧ��
	//500���쳣����

	HINTERNET hInternet;
	HINTERNET hSession;
	HINTERNET hRequest;
	DWORD				nRetCode = 100;	//����ֵ
	TCHAR				tzErrMsg[MAX_PATH] = { 0 };	//	��׽�쳣
	string				strResponse;	//���յ�����
	//��������
	hInternet = InternetOpen((LPCWSTR)"RuoKuaiCPP",INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,NULL);   
	if(hInternet < 0 )
	{
		return 101;
	}
	//�����Ự
	hSession = InternetConnect(hInternet,(LPCWSTR)m_tzServerName,m_nPort,NULL,NULL,INTERNET_SERVICE_HTTP,0,0);   
	if(hSession < 0 )
	{
		return 101;
	}
	char* szAccept[] = {"*/*",NULL};
	//��������
	hRequest = HttpOpenRequest(hSession,(LPCWSTR)"POST",(LPCWSTR)m_tzObjectName,NULL,NULL,(LPCWSTR*)(LPCSTR*)szAccept,0,0); 
	if(hRequest < 0 )
	{
		return 102;
	}
	//��������ͷ
	HttpAddRequestHeaders(hRequest,(LPCWSTR)m_strHeaderData.c_str(),
		m_strHeaderData.size(),HTTP_ADDREQ_FLAG_ADD);

	string	strHeader = "Content-Type: application/x-www-form-urlencoded";
	//////////////////////////////////////////////////////////////////////////
	//����ʱ����GB2312
	if (SendCode == 1)
	{
#ifdef UNICODE
		WideToMulti((LPCWSTR)(LPCTSTR)strDataSend, DataSize);
#else
		MultiToMulti((LPCSTR)(LPCTSTR)strDataSend, DataSize);
#endif
		if(!HttpSendRequest(
			hRequest,
			(LPCWSTR)strHeader.c_str(),
			-1,
			m_lpMultiByte,
			m_nMultiByteLen
			))
		{
			return 103;
		}

	}
	//����ʱѡ��UTF8
	else if (SendCode == 2)
	{
#ifdef UNICODE
		WideCharToUTF8((LPCWSTR)(LPCTSTR)strDataSend, DataSize);
#else
		MultiByteToUTF8((LPCSTR)(LPCTSTR)strDataSend, DataSize);
#endif
		//��������
		if(!HttpSendRequest(hRequest,(LPCWSTR)strHeader.c_str(),-1,m_lpUTF8,m_nUTF8Len))
		{
			return 103;
		}
	}

	//��ѯ״̬
	//DWORD	dwStatusCode = 0;
	////pFile->QueryInfoStatusCode(dwStatusCode);
	//HttpQueryInfo(
	//	hRequest,
	//	dwStatusCode,
	//	NULL,
	//	NULL,
	//	0
	//	);
	//if (dwStatusCode != 200)
	//{
	//	return 104;
	//}

	//////////////////////////////////////////////////////////////////////////
	//��������
    LPSTR lpszData;        //buffer for the data   
    DWORD dwSize;        //size of the data available   
    DWORD dwDownloaded; //size of the downloaded data 
	while(1)   
    {   
        // ѯ���Ƿ������ݿ���
        if (!InternetQueryDataAvailable(hRequest,&dwSize,0,0))   
        {   
            break;   
        }   
        else   
        {     
            // ���仺���ڴ�  
            lpszData = new char[dwSize+1]; 
            
            // ��ȡ�ļ� 
            if(!InternetReadFile(hRequest,(LPVOID)lpszData,dwSize,&dwDownloaded))   
            {  
                delete[] lpszData;   
                break;   
            }   
            else   
            {   
                // ��ӽ�����ʶ��
                lpszData[dwDownloaded]='\0';   
                strResponse = lpszData;             
                //ɾ��������  
                delete[] lpszData;   
                break;
                // ����Ϊ0ֱ�ӷ���
                if (dwDownloaded == 0)   
                {
                    break;   
                }
            }
        }   
    }
		//ת�����յ�����
		//���ղ���GB2312����
		if (RecvCode == 1)
		{
#ifdef UNICODE
			MultiToWide(strResponse.c_str(), strResponse.length());
			*pRecvData = m_wpWideChar;
			RecvSize = m_nWideCharLen;
#else
			MultiToMulti(strResponse.c_str(), strResponse.length());
			*pRecvData = (LPTSTR)m_lpMultiByte;
			RecvSize = m_nMultiByteLen;
#endif
		}
		//���ղ���UTF8����
		else if (RecvCode == 2)
		{
#ifdef UNICODE
			UTF8ToWideChar(strResponse.c_str(), strResponse.length());
			*pRecvData = m_wpWideChar;
			RecvSize = m_nWideCharLen;
#else
			UTF8ToMultiByte(strResponse.c_str(), strResponse.length());
			*pRecvData = (LPTSTR)m_lpMultiByte;
			RecvSize = m_nMultiByteLen;
#endif
		}
		//����ֵ
		nRetCode = 100;
	//�رջỰ
	InternetCloseHandle(hRequest);   
    InternetCloseHandle(hSession);   
    InternetCloseHandle(hInternet); 
	return nRetCode;
}

//�ύͼƬ���Լ��������ݰ�
DWORD CSendData::PostPicDataMethod(CHAR PicPath[], LPCTSTR pPostName,
	LPTSTR *pRecvData, DWORD &RecvSize,
	int	SendCode, int RecvCode, BOOL bIsByte, LPBYTE lpPicByte, DWORD dwPicByteSize)
{

	//99��ͼƬ������
	//100�������ɹ�
	//101���������޷�����
	//102���ύҳ���޷���
	//103�����ݷ���ʧ��
	//104������������ʧ��
	//500���쳣����
	DWORD	dwRetCode = 100;
	DWORD	dwPostSize = 0;		//Ҫ���͵����ݴ�С
	DWORD	dwPicSize = 0;	//ͼƬ��С
	DWORD	dwPostEndSize = 0;	//Э��β�����ݳ���

	TCHAR				tzErrMsg[MAX_PATH] = { 0 };	//	��׽�쳣
	string				strResponse;	//���յ�����

	//��ȡͼƬ��С
	if (bIsByte)
		dwPicSize = dwPicByteSize;
	else
		dwPicSize = GetExistPic(PicPath);

	//�������ݰ�
	MakeHeaderData();
	if (bIsByte)
		MakePostPicData(pPostName, CUtils::StringToWString("code.jpg").c_str());
	else
		MakePostPicData(pPostName, CUtils::StringToWString(PicPath).c_str());	//��ԭ�����ݰ��������ͼƬǰ׺��
	MakeEndData();
	//���ݰ��������

	HINTERNET hInternet;
	HINTERNET hSession;
	HINTERNET hRequest;
	//��������
	hInternet = InternetOpen(CUtils::StringToWString("RuoKuaiCPP").c_str(),INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,NULL);   
	if(hInternet < 0 )
	{
		return 101;
	}
    //�����Ự
	hSession = InternetConnect(hInternet,CUtils::StringToWString(m_tzServerName).c_str(),m_nPort,NULL,NULL,INTERNET_SERVICE_HTTP,0,0);   
	if(hSession < 0 )
	{
		return 101;
	}
	char* szAccept[] = {"*/*",NULL};
	hRequest = HttpOpenRequest(hSession,CUtils::StringToWString("POST").c_str(),CUtils::StringToWString(m_tzObjectName).c_str(),NULL,NULL,(LPCWSTR*)szAccept,0,0); 
	if(hRequest < 0 )
	{
		return 102;
	}
	//����ͷ
	if(!HttpAddRequestHeaders(hRequest,CUtils::StringToWString(m_strHeaderData).c_str(),
		CUtils::StringToWString(m_strHeaderData).length(),HTTP_ADDREQ_FLAG_ADD))
	{
		return 103;
	}
		//////////////////////////////////////////////////////////////////////////
		//����ʱ����GB2312
		if (SendCode == 1)
		{
#ifdef UNICODE
			WideToMulti((LPCWSTR)(LPCTSTR)CUtils::StringToWString(m_strPostData).c_str(), m_strPostData.size());
			//����β�����ݳ���
			dwPostEndSize = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)(LPCTSTR)CUtils::StringToWString(m_strEndData).c_str(), m_strEndData.size(), NULL, 0, NULL, NULL);
#else
			MultiToMulti((LPCSTR)(LPCTSTR)m_strPostData.c_str(), m_strPostData.size());
			dwPostEndSize = m_strEndData.size();
#endif
			//����Э��β��ֻ��Ӣ�����ݣ���unicode��UTF8��ASCII�����£�
			//Ӣ������ת���󳤶Ȳ��䣬��˿�ֱ�Ӽ��ϳ��ȡ�
			//��ȡβ������תΪUTF8��ĳ���
			dwPostSize = dwPicSize + m_nMultiByteLen + dwPostEndSize;
			//���ͣ���֪�����ܳ���
//			pFile->SendRequestEx(dwPostSize);
/////////////////////////////////////////////////
//��Ҫ���ݸ�ʽ
    INTERNET_BUFFERS BufferIn;
	BufferIn.dwStructSize = sizeof( INTERNET_BUFFERS ); // Must be set or error will occur
    BufferIn.Next = NULL; 
    BufferIn.lpcszHeader = NULL;
    BufferIn.dwHeadersLength = 0;
    BufferIn.dwHeadersTotal = 0;
    BufferIn.lpvBuffer = NULL;                
    BufferIn.dwBufferLength = 0;
    BufferIn.dwBufferTotal = dwPostSize; // This is the only member used other than dwStructSize
    BufferIn.dwOffsetLow = 0;
    BufferIn.dwOffsetHigh = 0;

    if(!HttpSendRequestEx( hRequest, &BufferIn, NULL, 0, 0))
    {
        printf( "Error on HttpSendRequestEx %d\n",GetLastError() );
        return FALSE;
    }
			//д�ļ�
			if(!InternetWriteFile(hRequest,m_lpMultiByte,m_nMultiByteLen,0))
			{
	
			}

		}
		//����ʱѡ��UTF8
		else if (SendCode == 2)
		{
#ifdef UNICODE
			WideCharToUTF8((LPCWSTR)(LPCTSTR)CUtils::StringToWString(m_strPostData).c_str(), m_strPostData.size());
			dwPostEndSize = WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)(LPCTSTR)CUtils::StringToWString(m_strEndData).c_str(), m_strEndData.size(), NULL, 0, NULL, NULL);
#else
			//����鷳��Ҫת������ֱ���ú���ת���ˡ�
			MultiByteToUTF8((LPCSTR)(LPCTSTR)m_strEndData.c_str(), m_strEndData.size());
			dwPostEndSize = m_nUTF8Len;

			MultiByteToUTF8((LPCSTR)(LPCTSTR)m_strPostData.c_str(), m_strPostData.size());
#endif
			dwPostSize = dwPicSize + m_nUTF8Len + dwPostEndSize;
			//����
            INTERNET_BUFFERS BufferIn;
			BufferIn.dwStructSize = sizeof( INTERNET_BUFFERS ); // Must be set or error will occur
			BufferIn.Next = NULL; 
			BufferIn.lpcszHeader = NULL;
			BufferIn.dwHeadersLength = 0;
			BufferIn.dwHeadersTotal = 0;
			BufferIn.lpvBuffer = NULL;                
			BufferIn.dwBufferLength = 0;
			BufferIn.dwBufferTotal = dwPostSize; // This is the only member used other than dwStructSize
			BufferIn.dwOffsetLow = 0;
			BufferIn.dwOffsetHigh = 0;

			if(!HttpSendRequestEx( hRequest, &BufferIn, NULL, 0, 0))
			{
				return FALSE;
			}
			DWORD dwBytesWritten;
			if(!InternetWriteFile(
				hRequest,
				m_lpUTF8,
				m_nUTF8Len,
				&dwBytesWritten
				))
			{
			}
		}
		//////////////////////////////////////////////////////////////////////////
		//����ͼƬ
		//�����С
		int	BuffSize = 8 * 1024;
		if (dwPicSize != 0)
		{
			if (bIsByte)
			{
				DWORD dwBytesWritten;
				InternetWriteFile(
				hRequest,
				lpPicByte,
				dwPicSize,
				&dwBytesWritten
				);
			}
			else
			{
				char *pbuff = new char[BuffSize];
				FILE *pFile;
				UINT	ReadLength = -1;

				if (pbuff != NULL)
				{
					if(pFile=fopen( PicPath, "rb"))
					{
					    while (ReadLength != 0)
						{
							ReadLength=fread(pbuff,1,BuffSize,pFile);
							if (ReadLength != 0)
							{
								DWORD dwBytesWritten;
												InternetWriteFile(
													hRequest,
													pbuff,
													ReadLength,
													&dwBytesWritten
													);
							}
						}
						fclose(pFile);
					}
					delete[]pbuff;
					pbuff = NULL;
				}
			}
		}
		//////////////////////////////////////////////////////////////////////////
		//����Э��β��
		//����ʱ����GB2312
		if (SendCode == 1)
		{
#ifdef UNICODE
			WideToMulti((LPCWSTR)(LPCTSTR)CUtils::StringToWString(m_strEndData).c_str(), m_strEndData.size());
#else
			MultiToMulti((LPCSTR)(LPCTSTR)m_strEndData.c_str(), m_strEndData.size());
#endif
			DWORD dwBytesWritten;
			InternetWriteFile(
				hRequest,
				m_lpMultiByte,
				m_nMultiByteLen,
				&dwBytesWritten
				);
		}
		//����ʱѡ��UTF8
		else if (SendCode == 2)
		{
#ifdef UNICODE
			WideCharToUTF8((LPCWSTR)(LPCTSTR)CUtils::StringToWString(m_strEndData).c_str(), m_strEndData.size());
#else
			MultiByteToUTF8((LPCSTR)(LPCTSTR)m_strEndData.c_str(), m_strEndData.size());
#endif
			DWORD dwBytesWritten;
						if(!InternetWriteFile(
				hRequest,
				m_lpUTF8,
				m_nUTF8Len,
				&dwBytesWritten
				)){}
		}

		//�������
		if(!HttpEndRequest(hRequest, NULL, 0, 0))
		{
			return FALSE;
		}
		//��ѯ״̬
		DWORD	dwStatusCode = 0;
//		pFile->QueryInfoStatusCode(dwStatusCode);

		//HttpQueryInfo(
		//	hRequest,
		//	dwStatusCode,
		//	NULL,
		//	NULL,
		//	0
		//	);
		//if (dwStatusCode != 200)
		//{
		//	return 104;
		//}

		//////////////////////////////////////////////////////////////////////////
		//��������
    LPSTR lpszData;        //buffer for the data   
    DWORD dwSize;        //size of the data available   
    DWORD dwDownloaded; //size of the downloaded data 

	while(1)   
	{   
		// The call to InternetQueryDataAvailable determines the amount of
		// data available to download.   
		if (!InternetQueryDataAvailable(hRequest,&dwSize,0,0))   
		{   	
			break;   
		}   
		else   
		{     
			// Allocates a buffer of the size returned by InternetQueryDataAvailable
			lpszData = new char[dwSize+1]; 
			// Reads the data from the HINTERNET handle.   
			if(!InternetReadFile(hRequest,(LPVOID)lpszData,dwSize,&dwDownloaded))   
			{  
				delete[] lpszData;   
				break;   
			}   
			else   
			{   
				// Adds a null terminator to the end of the data buffer 
				lpszData[dwDownloaded]='\0';   
				strResponse = lpszData;     
				// Delete the two buffers   
				delete[] lpszData;   
				break;
				// Check the size of the remaining data.   If it is zero, break.   
				if (dwDownloaded == 0)   
				{
					break;   
				}
			}
		}   
	}
		//ת�����յ�����
		//���ղ���GB2312����
		if (RecvCode == 1)
		{
#ifdef UNICODE
			MultiToWide(strResponse.c_str(), strResponse.length());
			*pRecvData = (LPTSTR)m_wpWideChar;
			RecvSize = m_nWideCharLen;
#else
			MultiToMulti(strResponse.c_str(), strResponse.length());
			*pRecvData = (LPTSTR)m_lpMultiByte;
			RecvSize = m_nMultiByteLen;
#endif
		}
		//���ղ���UTF8����
		else if (RecvCode == 2)
		{
#ifdef UNICODE
			UTF8ToWideChar(strResponse.c_str(), strResponse.length());
			*pRecvData = (LPTSTR)m_wpWideChar;
			RecvSize = m_nWideCharLen;
#else
			UTF8ToMultiByte(strResponse.c_str(), strResponse.length());
			*pRecvData = (LPTSTR)m_lpMultiByte;
			RecvSize = m_nMultiByteLen;
#endif
		}
		//����ֵ
		dwRetCode = 100;



	//////////////////////////////////////////////////////////////////////////
	//�ر�
	InternetCloseHandle(hRequest);   
    InternetCloseHandle(hSession);   
    InternetCloseHandle(hInternet); 
	return dwRetCode;
}
//����ֵΪͼƬ��С������ֵΪ0��ʾ�����ڡ�
DWORD CSendData::GetExistPic(CHAR PicPath[])
{
//	TCHAR*  PicPath1;
//	int iLength ;   
 //   iLength = MultiByteToWideChar (CP_ACP, 0, PicPath, strlen (PicPath) + 1, NULL, 0) ;  
//    MultiByteToWideChar (CP_ACP, 0, PicPath, strlen (PicPath) + 1, PicPath1, iLength) ;  
	TCHAR Name[100];  
    MultiByteToWideChar(CP_ACP, 0, PicPath, -1, Name, 100);  

	DWORD	dwRetCode = 0;
	WIN32_FIND_DATA	FindData;
	HANDLE	hFile = INVALID_HANDLE_VALUE;

	hFile = FindFirstFile(Name, &FindData);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		dwRetCode = 0;
	}
	else
	{
		dwRetCode = FindData.nFileSizeLow;
		FindClose(hFile);
		hFile = INVALID_HANDLE_VALUE;
	}
	return dwRetCode;
}

//UTF-8תΪ���ַ�
void CSendData::UTF8ToWideChar(LPCSTR pUTF8, int UTF8Len)
{

	if (m_wpWideChar)
	{
		delete[] m_wpWideChar;
		m_wpWideChar = NULL;
	}

	m_nWideCharLen = MultiByteToWideChar(CP_UTF8, 0, pUTF8, UTF8Len, NULL, 0);

	m_wpWideChar = new WCHAR[m_nWideCharLen + 1];
	m_wpWideChar[m_nWideCharLen] = 0;

	MultiByteToWideChar(CP_UTF8, 0, pUTF8, UTF8Len, m_wpWideChar, m_nWideCharLen);

}
//��UTF8תΪ���ֽ�
void CSendData::UTF8ToMultiByte(LPCSTR pUTF8, int UTF8Len)
{
	//���ת��ҲҪ���������Ƚ�UTF8תΪ���ַ���
	//�ٽ����ַ�תΪ���ֽڡ�

	if (m_lpMultiByte)
	{
		delete[] m_lpMultiByte;
		m_lpMultiByte = NULL;
	}

	//UTF8ת���ַ�
	UTF8ToWideChar(pUTF8, UTF8Len);
	//���ַ�תΪ���ֽ�
	m_nMultiByteLen = WideCharToMultiByte(CP_ACP, 0, m_wpWideChar, m_nWideCharLen, NULL, 0, NULL, NULL);
	m_lpMultiByte = new char[m_nMultiByteLen + 1];
	m_lpMultiByte[m_nMultiByteLen] = 0;

	WideCharToMultiByte(CP_UTF8, 0, m_wpWideChar, m_nWideCharLen, m_lpMultiByte, m_nMultiByteLen, NULL, NULL);

}
//�����ַ�תΪUTF-8
void CSendData::WideCharToUTF8(LPCWSTR wpWideData, int WideLen)
{
	//���ԭ�ȵ�����û��ɾ��
	if (m_lpUTF8)
	{
		delete[] m_lpUTF8;
		m_lpUTF8 = NULL;
	}

	m_nUTF8Len = WideCharToMultiByte(CP_UTF8, 0, wpWideData, WideLen, NULL, 0, NULL, NULL);


	m_lpUTF8 = new char[m_nUTF8Len + 1];
	m_lpUTF8[m_nUTF8Len] = 0;


	WideCharToMultiByte(CP_UTF8, 0, wpWideData, WideLen, m_lpUTF8, m_nUTF8Len, NULL, NULL);

}


//�����ֽ�תΪUTF8
void CSendData::MultiByteToUTF8(LPCSTR lpMultiData, int MultiLen)
{
	//���ת��Ҫ�鷳�㣬Ҫ�Ƚ����ֽ�תΪ���ַ����ٽ����ַ�תΪUTF8
	//��ʵ���ȫ����Ӣ�ĵĻ�����û�б�Ҫ����ת���ˣ���������ģ��ͱ���ת��
	//���ԭ�ȵ�����û��ɾ��
	if (m_lpUTF8)
	{
		delete[] m_lpUTF8;
		m_lpUTF8 = NULL;
	}
	if (m_wpWideChar)
	{
		delete[] m_wpWideChar;
		m_wpWideChar = NULL;
	}
	//��תΪ���ַ�	
	m_nWideCharLen = MultiByteToWideChar(CP_ACP, 0, lpMultiData, MultiLen, NULL, 0);

	m_wpWideChar = new WCHAR[m_nWideCharLen + 1];
	m_wpWideChar[m_nWideCharLen] = 0;


	MultiByteToWideChar(CP_ACP, 0, lpMultiData, MultiLen, m_wpWideChar, m_nWideCharLen);
	//�ٽ����ַ�תΪUTF8
	WideCharToUTF8(m_wpWideChar, m_nWideCharLen);
}

void CSendData::WideToMulti(LPCWSTR wpWideData, int WideLen)
{
	if (m_lpMultiByte)
	{
		delete[] m_lpMultiByte;
		m_lpMultiByte = NULL;
	}

	m_nMultiByteLen = WideCharToMultiByte(CP_ACP, 0, wpWideData, WideLen, NULL, 0, NULL, NULL);
	m_lpMultiByte = new char[m_nMultiByteLen + 1];
	m_lpMultiByte[m_nMultiByteLen] = '\0';

	WideCharToMultiByte(CP_ACP, 0, wpWideData, WideLen, m_lpMultiByte, m_nMultiByteLen, NULL, NULL);

}

void CSendData::MultiToMulti(LPCSTR strDataSend, int DataSize)
{
	if (m_lpMultiByte)
	{
		delete[] m_lpMultiByte;
		m_lpMultiByte = NULL;
	}

	m_nMultiByteLen = DataSize;
	m_lpMultiByte = new	char[m_nMultiByteLen + 1];
	m_lpMultiByte[m_nMultiByteLen] = '\0';

	memcpy(m_lpMultiByte, (LPCSTR)strDataSend, m_nMultiByteLen);

}

void CSendData::MultiToWide(LPCSTR strDataSend, int DataSize)
{
	if (m_wpWideChar)
	{
		delete[] m_wpWideChar;
		m_wpWideChar = NULL;
	}

	m_nWideCharLen = MultiByteToWideChar(CP_ACP, 0, strDataSend, DataSize, NULL, 0);

	m_wpWideChar = new wchar_t[m_nWideCharLen + 1];
	m_wpWideChar[m_nWideCharLen] = 0;

	MultiByteToWideChar(CP_ACP, 0, strDataSend, DataSize, m_wpWideChar, m_nWideCharLen);

}
//����ͷ����
void CSendData::MakeHeaderData()
{
	char ch[400];
		sprintf(ch,"Accept: image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, application/xaml+xml,*/*\r\n"
		"Accept-Encoding: gzip, deflate\r\n"
		"Accept-Language: zh-cn\r\n"
		"Content-Type: multipart/form-data; boundary=%s\r\n"
		"Cache-Control: no-cache\r\n\r\n",
		m_strBoundary.c_str());
	m_strHeaderData=ch;
}

//���췢�͵����ݰ�
BOOL CSendData::MakePostData(LPCTSTR pPostName, LPCTSTR pPostValue)
{
	if (pPostName == NULL)
	{
		return FALSE;
	}
	wstring wPostName=pPostName;
	wstring wPostValue=pPostValue;
	string sPostName=CUtils::WStringToString(wPostName);
	string sPostValue=CUtils::WStringToString(wPostValue);
	string	strTemp;
			char ch[200];
	sprintf(ch,"--%s\r\nContent-Disposition: form-data; name=\"%s\"\r\n\r\n%s\r\n",
		m_strBoundary.c_str(), sPostName.c_str(),sPostValue.c_str());
	strTemp=ch;
	//����һ���µ����ݰ�
	m_strPostData += strTemp;

	return TRUE;
}
//����Э����������
BOOL CSendData::MakePostData(LPCTSTR pPostName, int iPostValue)
{
	if (pPostName == NULL)
	{
		return FALSE;
	}
	wstring wPostName=pPostName;
	string sPostName=CUtils::WStringToString(wPostName);
	string	strTemp;
		char ch[200];
	sprintf(ch,"--%s\r\nContent-Disposition: form-data; name=\"%s\"\r\n\r\n%d\r\n",
		m_strBoundary.c_str(), sPostName.c_str(), iPostValue);
	strTemp=ch;
	//����һ���µ����ݰ�
	m_strPostData += strTemp;

	return TRUE;

}

//����ͼƬ����ǰ׺��
void CSendData::MakePostPicData(LPCTSTR pPostName, LPCTSTR pPostValue)
{
	if (pPostName == NULL)
	{
		return;
	}
	wstring wPostName=pPostName;
	wstring wPostValue=pPostValue;
	string sPostName=CUtils::WStringToString(wPostName);
	string sPostValue=CUtils::WStringToString(wPostValue);
	string	strTemp;
	string	strPicPath((LPSTR)pPostValue);	//
	string	strExtName;	//��չ��
	char ch[200];

	sprintf(ch,"--%s\r\nContent-Disposition: form-data; name=\"%s\"; filename=\"%s\"\r\n",
		m_strBoundary.c_str(), sPostName.c_str(), sPostValue.c_str());
	strTemp=ch;
	//������չ���趨content-type
	transform(strPicPath.begin(), strPicPath.end(), strPicPath.begin(), toupper); 
	strExtName =strPicPath.substr(strPicPath.rfind('.')+1);
	if (strExtName.c_str() == "JPG" || strExtName.c_str() == "JPEG")
	{
		strTemp += "Content-Type: image/pjpeg\r\n\r\n";
	}
	else if (strExtName.c_str() == "GIF")
	{
		strTemp += "Content-Type: image/gif\r\n\r\n";
	}
	else if (strExtName.c_str() == "PNG")
	{
		strTemp += "Content-Type: image/x-png\r\n\r\n";
	}
	//������������
	else
	{
		strTemp += "Content-Type: application/octet-stream\r\n\r\n";
	}
	//����һ���µ����ݰ�
	m_strPostData += strTemp;
	return;

}
//�����������
void CSendData::MakeEndData()
{
	char ch[200];
	sprintf(ch,"\r\n--%s\r\nContent-Disposition: form-data; name=\"Submit\"\r\n\r\nsubmit\r\n--%s--",
		m_strBoundary.c_str(), m_strBoundary.c_str());
	m_strEndData=ch;
}