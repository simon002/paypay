#pragma once
#include <afxinet.h>
#include <string>
#include <vector>
using namespace std;
class FtpManager
{
public:
	FtpManager(wstring _name,wstring _password,wstring _server_ip);
    ~FtpManager();
	int openConnection(wstring _server_ip);
	bool closeConnection();
	int _downLoadFilesImp(CStringArray *remoteArray,CStringArray *localArray,int number_file);
	int _pushFilesImp(CStringArray *remoteArray,CStringArray *localArray,int number_file);
	int pushFiles(vector<wstring>& files);

private:
	CInternetSession* m_pInternetSession;
	CFtpConnection* m_pFtpConnection;
	wstring m_name;
	wstring m_password;
	wstring m_server_ip;

};