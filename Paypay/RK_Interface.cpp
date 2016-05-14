#include "stdafx.h"
#include "RK_Interface.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


//========================= CRK_Interface接口类(作者只要关注这个类) =============================
CRK_Interface::CRK_Interface()
{
	// 修改默认系统连接数
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
		szResult = "错误ID不能为空.";
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

		ret = "服务器没有响应！";
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
		szResult = "用户名或密码或注册邮箱不能为空.";
		return FALSE;
	}

	char postData[256] = { 0x0 };
	sprintf(postData, "username=%s&password=%s&email=%s", CUtils::WStringToString(lpAccount).c_str(), CUtils::WStringToString(lpPassword).c_str(), CUtils::WStringToString(lpEmail).c_str());

	string rspMsg = "";
	bool successful = false;
	CGlobalMgr::PostToRuoKuai("register.xml", postData, rspMsg, successful);

	if (rspMsg.empty())
	{
		ret = "服务器没有响应！";
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
		szResult = "用户名或密码不能为空.";
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
		szResult = "用户名或充值卡号或充值密码不能为空";
		return FALSE;
	}

	char postData[256] = { 0x0 };
	sprintf(postData, "username=%s&id=%s&password=%s", CUtils::WStringToString(lpAccount).c_str(), CUtils::WStringToString(lpCardID).c_str(), CUtils::WStringToString(lpCardPassword).c_str());

	string rspMsg = "";
	bool successful = false;
	CGlobalMgr::PostToRuoKuai("recharge.xml", postData, rspMsg, successful);

	if (rspMsg.empty())
	{
		ret = "服务器没有响应！";
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
		szResult = "指向图片字节数组的指针为空.";
		return FALSE;
	}

	if (dwPicSize == 0)
	{
		szResult = "图片字节数组长度为0.";
		return FALSE;
	}

	//定义发送类
	CSendData	MySend;
	TCHAR		*pRecvData = NULL;	//接收数据的指针
	DWORD		dwRecvSize = 0;		//接收数据的大小
	DWORD		dwRetSend = 0;		//发送函数返回值
	int			SendCode, RecvCode;	//发送与接收的编码

	/*SendCode = HTTP_POST_GB2312;
	RecvCode = HTTP_POST_GB2312;*/
	SendCode = HTTP_POST_UTF8;
	RecvCode = HTTP_POST_UTF8;

	//设置服务器参数
	MySend.SetServerParam("api.ruokuai.com","create.xml", INTERNET_DEFAULT_HTTP_PORT);

	//必须先构造数据包
	MySend.MakePostData(CUtils::StringToWString("username").c_str(), CUtils::StringToWString(CGlobalMgr::m_Account).c_str());
	MySend.MakePostData(CUtils::StringToWString("password").c_str(), CUtils::StringToWString(CGlobalMgr::m_Password).c_str());
	MySend.MakePostData(CUtils::StringToWString("typeid").c_str(), CUtils::StringToWString(CGlobalMgr::m_QuestionType).c_str());
	MySend.MakePostData(CUtils::StringToWString("timeout").c_str(), CUtils::StringToWString("90").c_str());
	MySend.MakePostData(CUtils::StringToWString("softid").c_str(), CUtils::StringToWString(CGlobalMgr::m_SoftwareID).c_str());
	MySend.MakePostData(CUtils::StringToWString("softkey").c_str(), CUtils::StringToWString(CGlobalMgr::m_SoftwareKEY).c_str());

	//发送
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
		szResult = "图片路径不能为空.";
		return FALSE;
	}

	//定义发送类
	CSendData	MySend;
	TCHAR		*pRecvData = NULL;	//接收数据的指针
	DWORD		dwRecvSize = 0;		//接收数据的大小
	DWORD		dwRetSend = 0;		//发送函数返回值
	int			SendCode, RecvCode;	//发送与接收的编码

	/*SendCode = HTTP_POST_GB2312;
	RecvCode = HTTP_POST_GB2312;*/
	SendCode = HTTP_POST_UTF8;
	RecvCode = HTTP_POST_UTF8;

	//设置服务器参数
	MySend.SetServerParam("api.ruokuai.com","create.xml", INTERNET_DEFAULT_HTTP_PORT);

	//必须先构造数据包
	MySend.MakePostData(CUtils::StringToWString("username").c_str(), CUtils::StringToWString(CGlobalMgr::m_Account).c_str());
	MySend.MakePostData(CUtils::StringToWString("password").c_str(), CUtils::StringToWString(CGlobalMgr::m_Password).c_str());
	MySend.MakePostData(CUtils::StringToWString("typeid").c_str(), CUtils::StringToWString(CGlobalMgr::m_QuestionType).c_str());
	MySend.MakePostData(CUtils::StringToWString("timeout").c_str(), CUtils::StringToWString("90").c_str());
	MySend.MakePostData(CUtils::StringToWString("softid").c_str(), CUtils::StringToWString(CGlobalMgr::m_SoftwareID).c_str());
	MySend.MakePostData(CUtils::StringToWString("softkey").c_str(), CUtils::StringToWString(CGlobalMgr::m_SoftwareKEY).c_str());
	//发送
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
		szResult = "URL路径不能为空.";
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
		shownStr = "服务器没有响应！";
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

//========================= CUtils基础类 =============================
CUtils::CUtils(void)
{
}

CUtils::~CUtils(void)
{
}

/*
//向http://current.sinaapp.com/post.php发送数据
PostHttpPage("current.sinaapp.com","post.php","name=rain&age=12");
*/
bool CUtils::PostHttpPage(const string& hostName, const string& pathName, const string& postData, string& response)
{ 
	INTERNET_PORT nPort = 80;
	DWORD dwRet = 0;
	HINTERNET hInternet = InternetOpen(CUtils::StringToWString("RuoKuaiCPP").c_str(),INTERNET_OPEN_TYPE_PRECONFIG,NULL, NULL,NULL);   
	//测试句柄
	if(hInternet < 0 )
	{
		return false;
	}
	//建立网络会话
    HINTERNET hSession = InternetConnect(hInternet,CUtils::StringToWString(hostName).c_str(),nPort,NULL,NULL,INTERNET_SERVICE_HTTP,0,0);   
	if(hSession < 0 )
	{
		return false;
	}
    char* szAccept[] = {"*/*",NULL};
    string strVerb; 
	//建立网络请求
	HINTERNET hRequest = HttpOpenRequest(hSession,CUtils::StringToWString("POST").c_str(),CUtils::StringToWString(pathName).c_str(),NULL,NULL,(LPCWSTR*)(LPCSTR*)szAccept,0,0); 
	if(hRequest < 0 )
      {
          return false;
      }
	string strHeaders = "Content-Type: application/x-www-form-urlencoded";
	//发送请求
	 if(!HttpSendRequest(hRequest,CUtils::StringToWString(strHeaders.c_str()).c_str(),-1,(LPVOID)postData.c_str(),postData.size()))
	{
		return false;
	}
    //接受数据
    LPSTR lpszData;        //buffer for the data   
    DWORD dwSize;        //  
    DWORD dwDownloaded; //size of the downloaded data 
	while(1)   
    {          
        // 询问是否还有可用数据
        if (!InternetQueryDataAvailable(hRequest,&dwSize,0,0))   
        {   
			AfxMessageBox(_T("afdfasd"));
            break;   
        }   
        else   
        {     
            // 申明读取文件缓冲数组   
            lpszData = new char[dwSize+1]; 
            
            // 读取文件
            if(!InternetReadFile(hRequest,(LPVOID)lpszData,dwSize,&dwDownloaded))   
            {  
                delete[] lpszData;   
                break;   
            }   
            else   
            {   
                //添加结束符
                lpszData[dwDownloaded]='\0';   
                response = lpszData;             
	            //删除缓冲数组
                delete[] lpszData;   
                break; 
                if (dwDownloaded == 0)   
                {
                    break;   
                }
            }
        }   
    } 
	 // 关闭网络会话
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
	str.resize(str.size()); //取得str对象的原始字符串
	int nBufferSize = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0); //取得所需缓存的多少
	wchar_t *pBuffer = (wchar_t*)malloc(nBufferSize * sizeof(wchar_t));//申请缓存空间
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, pBuffer, nBufferSize*sizeof(wchar_t));//转码
	char *psText;
	psText = new char[nBufferSize + 1];
	memset(psText, 0, nBufferSize + 1);
	WideCharToMultiByte(CP_OEMCP, NULL, pBuffer, -1, psText, nBufferSize, NULL, FALSE);
	input="";
	input = psText;

	delete[]psText;
	free(pBuffer); //释放缓存
}
//////////////////////////////////////////////////////////////////////////////
//单独处理上传图片转码
//////////////////////////////////////////////////////////////////////////////
void CUtils::ProcessChineseMessyCodeW(string& input)
{
	if (input.empty()) return;
	string str=input;
	str.resize(str.size()); //取得str对象的原始字符串
	int nBufferSize = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0); //取得所需缓存的多少
	wchar_t *pBuffer = (wchar_t*)malloc(nBufferSize * sizeof(wchar_t));//申请缓存空间
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, pBuffer, nBufferSize*sizeof(wchar_t));//转码
	free(pBuffer); //释放缓存
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
		ret = "服务器没有响应！";
	}
	else
	{
		ret = rspResult ? "" : "失败: ";
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

//========================= CGlobalMgr 发送基础数据类 =============================
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


//========================= CSendData发送图片数据类 =============================
CSendData::CSendData()
{
	m_lpUTF8 = NULL;		//UTF8编码缓存指针
	m_lpMultiByte = NULL;		//
	m_wpWideChar = NULL;		//Widechar编码缓存指针
	//服务器默认值
	strcpy(m_tzServerName, "api.ruokuai.com");
	strcpy(m_tzObjectName, "/create.xml");
	m_nPort = INTERNET_DEFAULT_HTTP_PORT;

	m_strBoundary = "-------------RuoKuaiCPP";
}

CSendData::~CSendData()
{
	//如果原先的数据没有删除
	if (m_lpUTF8)
	{
		delete[] m_lpUTF8;
		m_lpUTF8 = NULL;
	}
	//如果原先的数据没有删除
	if (m_lpMultiByte)
	{
		delete[] m_lpMultiByte;
		m_lpMultiByte = NULL;
	}
	//宽字节代码指针
	if (m_wpWideChar)
	{
		delete[]m_wpWideChar;
		m_wpWideChar = NULL;
	}

}
//设置服务器参数
void CSendData::SetServerParam(CHAR ServerName[], CHAR ObjectName[], WORD Port)
{
	strcpy(m_tzServerName, ServerName);
	strcpy(m_tzObjectName, ObjectName);
	m_nPort = Port;
}
//发送数据
//编码参数：1表示GB2312，2表示UTF8
DWORD CSendData::PostDataMethod(LPCTSTR strDataSend,
	int DataSize,
	LPTSTR *pRecvData,
	DWORD &RecvSize,
	int SendCode,	//发送时的编码
	int RecvCode	//接收返回时的编码
	)
{
	//返回错误代码列表：
	//100：正常成功
	//101：服务器无法连接
	//102：提交页面无法打开
	//103：数据发送失败
	//104：服务器处理失败
	//500：异常错误

	HINTERNET hInternet;
	HINTERNET hSession;
	HINTERNET hRequest;
	DWORD				nRetCode = 100;	//返回值
	TCHAR				tzErrMsg[MAX_PATH] = { 0 };	//	捕捉异常
	string				strResponse;	//接收的数据
	//开启连接
	hInternet = InternetOpen((LPCWSTR)"RuoKuaiCPP",INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,NULL);   
	if(hInternet < 0 )
	{
		return 101;
	}
	//建立会话
	hSession = InternetConnect(hInternet,(LPCWSTR)m_tzServerName,m_nPort,NULL,NULL,INTERNET_SERVICE_HTTP,0,0);   
	if(hSession < 0 )
	{
		return 101;
	}
	char* szAccept[] = {"*/*",NULL};
	//建立请求
	hRequest = HttpOpenRequest(hSession,(LPCWSTR)"POST",(LPCWSTR)m_tzObjectName,NULL,NULL,(LPCWSTR*)(LPCSTR*)szAccept,0,0); 
	if(hRequest < 0 )
	{
		return 102;
	}
	//附加请求头
	HttpAddRequestHeaders(hRequest,(LPCWSTR)m_strHeaderData.c_str(),
		m_strHeaderData.size(),HTTP_ADDREQ_FLAG_ADD);

	string	strHeader = "Content-Type: application/x-www-form-urlencoded";
	//////////////////////////////////////////////////////////////////////////
	//发送时采用GB2312
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
	//发送时选择UTF8
	else if (SendCode == 2)
	{
#ifdef UNICODE
		WideCharToUTF8((LPCWSTR)(LPCTSTR)strDataSend, DataSize);
#else
		MultiByteToUTF8((LPCSTR)(LPCTSTR)strDataSend, DataSize);
#endif
		//发送请求
		if(!HttpSendRequest(hRequest,(LPCWSTR)strHeader.c_str(),-1,m_lpUTF8,m_nUTF8Len))
		{
			return 103;
		}
	}

	//查询状态
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
	//接收数据
    LPSTR lpszData;        //buffer for the data   
    DWORD dwSize;        //size of the data available   
    DWORD dwDownloaded; //size of the downloaded data 
	while(1)   
    {   
        // 询问是否还有数据可用
        if (!InternetQueryDataAvailable(hRequest,&dwSize,0,0))   
        {   
            break;   
        }   
        else   
        {     
            // 分配缓冲内存  
            lpszData = new char[dwSize+1]; 
            
            // 读取文件 
            if(!InternetReadFile(hRequest,(LPVOID)lpszData,dwSize,&dwDownloaded))   
            {  
                delete[] lpszData;   
                break;   
            }   
            else   
            {   
                // 添加结束标识符
                lpszData[dwDownloaded]='\0';   
                strResponse = lpszData;             
                //删除缓冲区  
                delete[] lpszData;   
                break;
                // 数据为0直接返回
                if (dwDownloaded == 0)   
                {
                    break;   
                }
            }
        }   
    }
		//转换接收的数据
		//接收采用GB2312编码
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
		//接收采用UTF8编码
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
		//返回值
		nRetCode = 100;
	//关闭会话
	InternetCloseHandle(hRequest);   
    InternetCloseHandle(hSession);   
    InternetCloseHandle(hInternet); 
	return nRetCode;
}

//提交图片，自己构造数据包
DWORD CSendData::PostPicDataMethod(CHAR PicPath[], LPCTSTR pPostName,
	LPTSTR *pRecvData, DWORD &RecvSize,
	int	SendCode, int RecvCode, BOOL bIsByte, LPBYTE lpPicByte, DWORD dwPicByteSize)
{

	//99：图片不存在
	//100：正常成功
	//101：服务器无法连接
	//102：提交页面无法打开
	//103：数据发送失败
	//104：服务器处理失败
	//500：异常错误
	DWORD	dwRetCode = 100;
	DWORD	dwPostSize = 0;		//要传送的数据大小
	DWORD	dwPicSize = 0;	//图片大小
	DWORD	dwPostEndSize = 0;	//协议尾部数据长度

	TCHAR				tzErrMsg[MAX_PATH] = { 0 };	//	捕捉异常
	string				strResponse;	//接收的数据

	//获取图片大小
	if (bIsByte)
		dwPicSize = dwPicByteSize;
	else
		dwPicSize = GetExistPic(PicPath);

	//构造数据包
	MakeHeaderData();
	if (bIsByte)
		MakePostPicData(pPostName, CUtils::StringToWString("code.jpg").c_str());
	else
		MakePostPicData(pPostName, CUtils::StringToWString(PicPath).c_str());	//在原有数据包后面加上图片前缀包
	MakeEndData();
	//数据包构造完毕

	HINTERNET hInternet;
	HINTERNET hSession;
	HINTERNET hRequest;
	//连接网络
	hInternet = InternetOpen(CUtils::StringToWString("RuoKuaiCPP").c_str(),INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,NULL);   
	if(hInternet < 0 )
	{
		return 101;
	}
    //建立会话
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
	//附加头
	if(!HttpAddRequestHeaders(hRequest,CUtils::StringToWString(m_strHeaderData).c_str(),
		CUtils::StringToWString(m_strHeaderData).length(),HTTP_ADDREQ_FLAG_ADD))
	{
		return 103;
	}
		//////////////////////////////////////////////////////////////////////////
		//发送时采用GB2312
		if (SendCode == 1)
		{
#ifdef UNICODE
			WideToMulti((LPCWSTR)(LPCTSTR)CUtils::StringToWString(m_strPostData).c_str(), m_strPostData.size());
			//计算尾部数据长度
			dwPostEndSize = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)(LPCTSTR)CUtils::StringToWString(m_strEndData).c_str(), m_strEndData.size(), NULL, 0, NULL, NULL);
#else
			MultiToMulti((LPCSTR)(LPCTSTR)m_strPostData.c_str(), m_strPostData.size());
			dwPostEndSize = m_strEndData.size();
#endif
			//由于协议尾部只有英文数据，在unicode、UTF8、ASCII编码下，
			//英文数据转换后长度不变，因此可直接加上长度。
			//获取尾部数据转为UTF8后的长度
			dwPostSize = dwPicSize + m_nMultiByteLen + dwPostEndSize;
			//发送，告知数据总长度
//			pFile->SendRequestEx(dwPostSize);
/////////////////////////////////////////////////
//重要数据格式
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
			//写文件
			if(!InternetWriteFile(hRequest,m_lpMultiByte,m_nMultiByteLen,0))
			{
	
			}

		}
		//发送时选择UTF8
		else if (SendCode == 2)
		{
#ifdef UNICODE
			WideCharToUTF8((LPCWSTR)(LPCTSTR)CUtils::StringToWString(m_strPostData).c_str(), m_strPostData.size());
			dwPostEndSize = WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)(LPCTSTR)CUtils::StringToWString(m_strEndData).c_str(), m_strEndData.size(), NULL, 0, NULL, NULL);
#else
			//这个麻烦，要转两道，直接用函数转算了。
			MultiByteToUTF8((LPCSTR)(LPCTSTR)m_strEndData.c_str(), m_strEndData.size());
			dwPostEndSize = m_nUTF8Len;

			MultiByteToUTF8((LPCSTR)(LPCTSTR)m_strPostData.c_str(), m_strPostData.size());
#endif
			dwPostSize = dwPicSize + m_nUTF8Len + dwPostEndSize;
			//发送
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
		//发送图片
		//缓存大小
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
		//发送协议尾部
		//发送时采用GB2312
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
		//发送时选择UTF8
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

		//发送完毕
		if(!HttpEndRequest(hRequest, NULL, 0, 0))
		{
			return FALSE;
		}
		//查询状态
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
		//接收数据
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
		//转换接收的数据
		//接收采用GB2312编码
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
		//接收采用UTF8编码
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
		//返回值
		dwRetCode = 100;



	//////////////////////////////////////////////////////////////////////////
	//关闭
	InternetCloseHandle(hRequest);   
    InternetCloseHandle(hSession);   
    InternetCloseHandle(hInternet); 
	return dwRetCode;
}
//返回值为图片大小，返回值为0表示不存在。
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

//UTF-8转为宽字符
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
//将UTF8转为多字节
void CSendData::UTF8ToMultiByte(LPCSTR pUTF8, int UTF8Len)
{
	//这个转换也要分两步，先将UTF8转为宽字符，
	//再将宽字符转为多字节。

	if (m_lpMultiByte)
	{
		delete[] m_lpMultiByte;
		m_lpMultiByte = NULL;
	}

	//UTF8转宽字符
	UTF8ToWideChar(pUTF8, UTF8Len);
	//宽字符转为多字节
	m_nMultiByteLen = WideCharToMultiByte(CP_ACP, 0, m_wpWideChar, m_nWideCharLen, NULL, 0, NULL, NULL);
	m_lpMultiByte = new char[m_nMultiByteLen + 1];
	m_lpMultiByte[m_nMultiByteLen] = 0;

	WideCharToMultiByte(CP_UTF8, 0, m_wpWideChar, m_nWideCharLen, m_lpMultiByte, m_nMultiByteLen, NULL, NULL);

}
//将宽字符转为UTF-8
void CSendData::WideCharToUTF8(LPCWSTR wpWideData, int WideLen)
{
	//如果原先的数据没有删除
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


//将多字节转为UTF8
void CSendData::MultiByteToUTF8(LPCSTR lpMultiData, int MultiLen)
{
	//这个转换要麻烦点，要先将多字节转为宽字符，再将宽字符转为UTF8
	//其实如果全部是英文的话，是没有必要进行转换了，如果有中文，就必须转换
	//如果原先的数据没有删除
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
	//先转为宽字符	
	m_nWideCharLen = MultiByteToWideChar(CP_ACP, 0, lpMultiData, MultiLen, NULL, 0);

	m_wpWideChar = new WCHAR[m_nWideCharLen + 1];
	m_wpWideChar[m_nWideCharLen] = 0;


	MultiByteToWideChar(CP_ACP, 0, lpMultiData, MultiLen, m_wpWideChar, m_nWideCharLen);
	//再将宽字符转为UTF8
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
//构造头数据
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

//构造发送的数据包
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
	//增加一个新的数据包
	m_strPostData += strTemp;

	return TRUE;
}
//构造协议数据内容
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
	//增加一个新的数据包
	m_strPostData += strTemp;

	return TRUE;

}

//构造图片数据前缀包
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
	string	strExtName;	//扩展名
	char ch[200];

	sprintf(ch,"--%s\r\nContent-Disposition: form-data; name=\"%s\"; filename=\"%s\"\r\n",
		m_strBoundary.c_str(), sPostName.c_str(), sPostValue.c_str());
	strTemp=ch;
	//根据扩展名设定content-type
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
	//二进制数据流
	else
	{
		strTemp += "Content-Type: application/octet-stream\r\n\r\n";
	}
	//增加一个新的数据包
	m_strPostData += strTemp;
	return;

}
//构造结束数据
void CSendData::MakeEndData()
{
	char ch[200];
	sprintf(ch,"\r\n--%s\r\nContent-Disposition: form-data; name=\"Submit\"\r\n\r\nsubmit\r\n--%s--",
		m_strBoundary.c_str(), m_strBoundary.c_str());
	m_strEndData=ch;
}