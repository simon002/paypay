#pragma comment(lib, "wininet.lib") 
#include "stdafx.h"
#include <WinInet.h>
#include <ATLBASE.H>
#include <mshtml.h>
#define WM_MY_OWN_MSG WM_USER + 100

#include "../WinHttpClient/WinHttpClient.h"
#include "cookieprocess.h"
#include "../PaypayDlg.h"
HANDLE cookieMutex = CreateMutex(NULL,FALSE,NULL);
HANDLE OpenUrlMutex = CreateMutex(NULL,FALSE,NULL);
extern HANDLE hMutex;
extern unsigned int g_proxy_index;
extern std::vector<std::wstring> g_all_daili;
std::deque<ProxyCookie> CookieProcess::proxyCookieQueue;
std::deque<std::wstring> CookieProcess::m_can_use_proxy;
CookieProcess::CookieProcess():
m_can_visit(true),
m_first(true),
m_tag(0)
{ 
}

CookieProcess::~CookieProcess()
{

}

void CookieProcess::setTag(int _tag)
{
	m_tag = _tag;
}

void CookieProcess::setProxy(std::wstring _proxy_address)
{
	m_proxy_address = _proxy_address;
}

bool  CookieProcess::setConnectionOptions(std::wstring _proxy_full_addr)
{
	LPWSTR proxy_full_addr = const_cast<LPWSTR>(_proxy_full_addr.c_str());
	INTERNET_PER_CONN_OPTION_LIST list;  
	INTERNET_PER_CONN_OPTION Option[3];  
	unsigned long nSize = sizeof(INTERNET_PER_CONN_OPTION_LIST);   
	Option[0].dwOption = INTERNET_PER_CONN_PROXY_SERVER;  
	Option[0].Value.pszValue = proxy_full_addr;  

	// This option sets all the possible connection types for the client.   
	// This case specifies that the proxy can be used or direct connection is possible.  
	Option[1].dwOption = INTERNET_PER_CONN_FLAGS;  
	Option[1].Value.dwValue = PROXY_TYPE_PROXY | PROXY_TYPE_DIRECT;  

	Option[2].dwOption = INTERNET_PER_CONN_PROXY_BYPASS;  
	Option[2].Value.pszValue = L"local";  
	//fill the list struct  
	list.dwSize = nSize;  
	list.pszConnection = NULL;  
	list.dwOptionCount = 3;  
	list.dwOptionError = 0;  
	list.pOptions = Option;  

	if(!InternetSetOption(NULL,INTERNET_OPTION_PER_CONNECTION_OPTION,&list,nSize))  
		return FALSE;
	//The connection settings for other instances of Internet Explorer.   
	InternetSetOption(NULL, INTERNET_OPTION_SETTINGS_CHANGED, NULL, 0);  
	InternetSetOption(NULL, INTERNET_OPTION_REFRESH , NULL, 0);  
	return TRUE;
}

bool CookieProcess::disableConnectionProxy()
{
	INTERNET_PER_CONN_OPTION_LIST list;  
	INTERNET_PER_CONN_OPTION Option[1];  
	unsigned long nSize = sizeof(INTERNET_PER_CONN_OPTION_LIST);  

	Option[0].dwOption = INTERNET_PER_CONN_FLAGS;  
	Option[0].Value.dwValue = PROXY_TYPE_DIRECT;  

	list.dwSize = nSize;  
	list.pszConnection = NULL;  
	list.dwOptionCount = 1;  
	list.dwOptionError = NULL;  
	list.pOptions = Option;  

	if(!InternetSetOption(NULL,INTERNET_OPTION_PER_CONNECTION_OPTION,&list,nSize))  
		return FALSE;
	//The connection settings for other instances of Internet Explorer.   
	InternetSetOption(NULL, INTERNET_OPTION_SETTINGS_CHANGED, NULL, 0);  
	InternetSetOption(NULL, INTERNET_OPTION_REFRESH , NULL, 0);  
	return TRUE;
}
#define WM_MY_OWN_MSG WM_USER + 100
void CookieProcess::visitExplorerByProxy()  //ͨ���������
{
	//setConnectionOptions(L"202.100.167.182:80");
	try
	{
//		setConnectionOptions(m_proxy_address.c_str());
//		CString str;
//		str = "https://connect.secure.wellsfargo.com/auth/login/do";
//		m_explorer->Navigate(str, NULL, NULL, NULL, NULL);

		wstring s = m_proxy_address;	
		WaitForSingleObject(OpenUrlMutex, INFINITE);
		SendMessage(getPayDlg()->GetSafeHwnd(), WM_MY_OWN_MSG, (WPARAM)&s, (LPARAM)&m_tag);
		ReleaseMutex(OpenUrlMutex);

	}
	catch (...)
	{
		
	}


}

void CookieProcess::setPayDlg(CPaypayDlg* _payDlg)
{
    m_payDlg = _payDlg;
}

CPaypayDlg* CookieProcess::getPayDlg()
{
	return m_payDlg;
}
void CookieProcess::visitExplorerCallBack(LPDISPATCH pDisp, VARIANT FAR* URL)
{


	try
	{
	CString strUrl(URL->bstrVal);
	if(strUrl!="https://connect.secure.wellsfargo.com/auth/login/present?origin=cob" && 
		strUrl != "https://connect.secure.wellsfargo.com/auth/login/do")
		return;
	IHTMLDocument2   *objDocument=NULL;
	CComPtr<IHTMLElement> m_pBody;
	CComBSTR bstrHTMLText;
	objDocument=(IHTMLDocument2 *)m_explorer->get_Document();//m_ie��������ؼ�
	HRESULT hr=objDocument->get_body(&m_pBody);
	if (FAILED(hr))
		return;

	hr=m_pBody->get_innerText(&bstrHTMLText);
	if (FAILED(hr))
		return;
	CString html=bstrHTMLText.m_str;//��ȡ����html����

	if (html.GetLength() > 0 && strUrl == "https://connect.secure.wellsfargo.com/auth/login/do")
	{
		setCanVisit(true);
		return;
	}
	std::wstring cookie;
	LPWSTR lpszData = NULL;      
	DWORD dwSize=0;    
	lpszData=new TCHAR[1];   
	memset(lpszData,0,1);  

retry: 
	if (!InternetGetCookieEx(strUrl, 0, lpszData, &dwSize,8192,0))
	{  
		int x=GetLastError();
		if (GetLastError()== ERROR_INSUFFICIENT_BUFFER)   
		{   

			delete []lpszData;   
			lpszData = new TCHAR[dwSize+1];   
			memset(lpszData,0,dwSize+1);   
			goto retry;   
		}  
		else
		{
			cookie = _T("");
			delete []lpszData; 
			//			UpdateData(FALSE);
			return;

		}
	}   
	else  
	{   
	}
	cookie = (LPCTSTR)lpszData;
	delete []lpszData; 
	if (cookie.size() > 0 && strUrl =="https://connect.secure.wellsfargo.com/auth/login/present?origin=cob")
	{
		char mmm[10];
		itoa(m_tag, mmm, 10);
		
		string ttt = string(mmm);
		wstring ss = wstring(ttt.begin(),ttt.end());

		char m[10];
		itoa(proxyCookieQueue.size(), m, 10);
		string efws = string(m);
		wstring text = wstring(efws.begin(),efws.end());
		std::wstring msg = L"�߳�" + ss + L"cookie������:" + text + L"��ȡ��cookie" + cookie ;
		getPayDlg()->addLogMsg(msg.c_str());
		WaitForSingleObject(cookieMutex, INFINITE);
		ProxyCookie pc;
		pc.cookie = cookie;
		pc.proxy = m_proxy_address;
		proxyCookieQueue.push_front(pc);
		ReleaseMutex(cookieMutex);
		setCanVisit(true);
	}
	if (strUrl =="https://connect.secure.wellsfargo.com/auth/login/do" && cookie.size() == 0 )
	{
		setCanVisit(true);
	}
	}
	catch (...)
	{
		//ReleaseMutex(cookieMutex);
		setCanVisit(true);
		return;
	}
}

void CookieProcess::setExplorer(CExplorer1* _explorer)
{
	m_explorer = _explorer;
}

DWORD WINAPI cookieProcessThread(LPVOID lpParamter)
{
	CookieProcess* cookieProcess = (CookieProcess*)lpParamter;
	int tag = cookieProcess->m_tag;
	char m[10];
	itoa(tag,m,10);
	std::string tt = std::string(m);
	wstring text =  wstring(tt.begin(),tt.end());
	DWORD start = GetTickCount();
	DWORD end = GetTickCount();
	while(true)
	{
		if (cookieProcess->getCanVisit())
		{
			WaitForSingleObject(cookieMutex, INFINITE);
			if (CookieProcess::m_can_use_proxy.size() > 0)
			//if (cookieProcess->getUseProxy().size() > 0)
			{
				try
				{

					std::wstring proxy = CookieProcess::m_can_use_proxy.front();
					std::wstring msg = L"�߳�" + text + L"ͨ������" + proxy + L"ȡcookie";
					cookieProcess->getPayDlg()->addLogMsg(msg.c_str());
					cookieProcess->CookieProcess::m_can_use_proxy.pop_front();
					cookieProcess->setProxy(const_cast<LPWSTR>(proxy.c_str()));
					cookieProcess->setCanVisit(false);
					cookieProcess->visitExplorerByProxy();
					Sleep(5000);
					start = GetTickCount();
				}
				catch (...)
				{
					//ReleaseMutex(cookieMutex);
					cookieProcess->setCanVisit(true);
				}


			}
			ReleaseMutex(cookieMutex);

		}
		else
		{
			Sleep(3000);
		}
		end = GetTickCount();
		if (end - start > 2*60*1000)
		{
			cookieProcess->setCanVisit(true);
		}
		Sleep(2000);
	}
	return 1;
}

void CookieProcess::doGetCookie()
{
	HANDLE handle = CreateThread(NULL, 0, cookieProcessThread, (LPVOID)this, 0, NULL);

}

std::deque<std::wstring>& CookieProcess::getUseProxy()
{
	return m_can_use_proxy;
}

DWORD WINAPI proxyProcessThread(LPVOID lpParamter)
{
	while(true)
	{
		CookieProcess* cookieProcess = (CookieProcess*)lpParamter;
		WaitForSingleObject(hMutex, INFINITE);
		if (g_proxy_index >= g_all_daili.size())
		{
			g_proxy_index = 0;
			ReleaseMutex(hMutex);
			Sleep(3000);
			continue;
		}
		std::wstring proxy = g_all_daili[g_proxy_index];
		g_proxy_index++;
		if (g_proxy_index >= g_all_daili.size())
		{
			g_proxy_index = 0;
			//break;
		}
		ReleaseMutex(hMutex);
		WinHttpClient WinClient(L"https://connect.secure.wellsfargo.com/auth/login/do");
		WinClient.SetProxy(proxy);
		WinClient.SetRequireValidSslCertificates(false);
		WinClient.SendHttpRequest(L"GET");
		if (((int)WinClient.m_responseHeader.find(L"200 OK\r\n")) <= 0)
		{

		}
		else
		{
			WaitForSingleObject(cookieMutex, INFINITE);
			CookieProcess::m_can_use_proxy.push_front(proxy);
			//cookieProcess->getUseProxy().push_front(proxy);	
			ReleaseMutex(cookieMutex);
			//if (cookieProcess->getFirst())
			//{
			//	cookieProcess->doGetCookie();
			//	cookieProcess->setFirst(false);
			//}	
			
		}
	}
	return 0;
}

void CookieProcess::doPorxyCheck()
{
	for (int i = 0;i < 20; ++i)
	{
		HANDLE handle = CreateThread(NULL, 0, proxyProcessThread, (LPVOID)this, 0, NULL);
	}
}

void CookieProcess::stopAllThread()
{

}