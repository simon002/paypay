#pragma once
#include "../explorer1.h"
#include <string>
#include <queue>
class CPaypayDlg;
struct ProxyCookie
{
	std::wstring proxy;
	std::wstring cookie;
};
class CookieProcess
{
public:
	CookieProcess();
	~CookieProcess();
	bool setConnectionOptions(std::wstring _proxy_full_addr);
	bool disableConnectionProxy();
	void visitExplorerByProxy();
	void visitExplorerCallBack(LPDISPATCH pDisp, VARIANT FAR* URL);
	void setProxy(std::wstring _proxy_address);
	void setExplorer(CExplorer1* _explorer);
	void doGetCookie();
	void doPorxyCheck();
	bool getCanVisit(){return m_can_visit;}
	void setCanVisit(bool _can_visit){m_can_visit = _can_visit;}
	std::wstring getProxy(){return m_proxy_address;}
	std::deque<std::wstring>& getUseProxy();
	bool getFirst(){return m_first;}
	void setFirst(bool _first){m_first = _first;}
	void setPayDlg(CPaypayDlg* _payDlg);
	void stopAllThread();
	CPaypayDlg* getPayDlg();
	void setTag(int _tag);
	static std::deque<ProxyCookie> proxyCookieQueue;
private:
	CExplorer1* m_explorer;
	CString m_cookie;
	std::wstring m_proxy_address;
	bool m_can_visit;
	bool m_first;
	int  m_tag;
	CPaypayDlg* m_payDlg;
	std::deque<std::wstring> m_can_use_proxy;
};