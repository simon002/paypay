#pragma once
#include <afxinet.h>
#include <string>
using namespace std;
class FtpManager
{
public:
	FtpManager();
    ~FtpManager();
	int openConnection(wstring _server_ip);
	bool closeConnection();
	int downLoadFiles(CStringArray *remoteArray,CStringArray *localArray,int number_file);
private:
	CInternetSession* m_pInternetSession;
	CFtpConnection* m_pFtpConnection;
	wstring m_name;
	wstring m_password;
	wstring m_server_ip;

};