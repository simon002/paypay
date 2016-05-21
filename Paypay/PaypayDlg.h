
// PaypayDlg.h : 头文件
//

#pragma once
#import "C:\WINDOWS\system32\msscript.ocx"
#include "Wincrypt.h"
#include "windows.h"
#include <string>
#include <list>
#include <map>
#include <vector>
#include <iostream> 
#include <fstream>
#include <conio.h>
#include "afxwin.h"
#include "explorer1.h"
#include "RK_Interface.h"
#include "WinHttpClient/WinHttpClient.h" 
#include "MyGzip.h"
#include "Mshtml.h"
#include "afxcmn.h"
#include "resource.h"
#include "cookie/cookieprocess.h"
#define _CRTDBG_MAP_ALLOC
#define new  new(_NORMAL_BLOCK, __FILE__, __LINE__)
#include <crtdbg.h>
#include "Urlmon.h"
struct my_struct
{
    std::string name;
	std::string password;
	bool operator == (const my_struct& m)const
	{
		return name == m.name && password == m.password;
	}
};
// CPaypayDlg 对话框
class CPaypayDlg : public CDialog
{
// 构造
public:
	CPaypayDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_PAYPAY_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	void readAllConfig();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	void addLogMsg(LPCTSTR msg);
	afx_msg void OnBnClickedButton1();
	int Login2(string username, string password, wstring proxy, wstring cookie,string & returnstr);
	bool GetCode( std::wstring& strCode );
	LRESULT OnRecvUsermsg(WPARAM wParam, LPARAM lParam);
public:
	
	list<my_struct> m_all_name_password;
	//std::map <std::string,std::string> m_all_name_password;//缓存所有数据
	unsigned int                       m_thread_num;//线程数
public:
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedButton4();
	CEdit m_daili_address;
	CEdit m_thread_input_num;
	CEdit m_input_file_path;
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedButton3();
	CEdit m_log_record;
	CListBox m_list_box;
	CExplorer1 m_ie;
	CEdit m_time;
	afx_msg void OnBnClickedCheck3();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnStnClickedStatic7();
	bool initDaiLi();
	afx_msg void OnClose();
	CProgressCtrl m_progress;
	afx_msg void OnStnClickedStatic19();
	CExplorer1 m_iee;
	CExplorer1* run_time_ie;
	afx_msg void OnBnClickedCheck4();
	CEdit m_chongqi;
	CookieProcess m_cookieProcess;
	CookieProcess m_cookieProcess_1;
	CookieProcess m_cookieProcess_2;
	CookieProcess m_cookieProcess_3;
	CookieProcess m_cookieProcess_4;

	DECLARE_EVENTSINK_MAP()
	void DocumentCompleteExplorer1(LPDISPATCH pDisp, VARIANT* URL);
	CExplorer1 m_ie_1;
	void DocumentCompleteExplorer2(LPDISPATCH pDisp, VARIANT* URL);
	CExplorer1 m_ie_2;
	void DocumentCompleteExplorer3(LPDISPATCH pDisp, VARIANT* URL);
	CExplorer1 m_ie_3;
	void DocumentCompleteExplorer4(LPDISPATCH pDisp, VARIANT* URL);
	CExplorer1 m_ie_4;
	void DocumentCompleteExplorer5(LPDISPATCH pDisp, VARIANT* URL);
};
