#include "stdafx.h"
#include "ftpmanager.h"
FtpManager::FtpManager(wstring _name,wstring _password,wstring _server_ip):
m_name(L""),
m_password(L""),
m_server_ip(L""),
m_pFtpConnection(NULL),
m_pInternetSession(NULL)

{
	CString strAppName;
	strAppName.LoadString(AFX_IDS_APP_TITLE);
	m_pInternetSession = new CInternetSession(strAppName,INTERNET_OPEN_TYPE_PRECONFIG);
	m_name = _name;
	m_password = _password;
	m_server_ip = _server_ip;
}

FtpManager::~FtpManager()
{
	if (m_pInternetSession != NULL)
	{
		delete m_pInternetSession;
		m_pInternetSession = NULL;
	}
}

int FtpManager::openConnection(wstring _server_ip)
{
	if(_server_ip == L"")
	{
		return 0;
	}
	// put the server name in the CFtpGet class
	m_server_ip = _server_ip;
	try 
	{
		// try to connect to a ftp server
		m_pFtpConnection = m_pInternetSession->GetFtpConnection(m_server_ip.c_str(),
		m_name.c_str(),
		m_password.c_str(),21);
	}
	catch (CInternetException* pEx) 
	{
		// if failed, just show the error

		// Oops! We failed to connect!
		TCHAR szErr[1024];
		pEx->GetErrorMessage(szErr, 1024);
		TRACE(szErr);
		pEx->Delete();
		return 0;// return 1 but previous error box have been showed
	}
	return 1;
}

bool FtpManager::closeConnection()
{
	// close the connection to server, you can reconnect latter
	if(m_pFtpConnection == NULL)
		return 0;
	try
	{
		m_pFtpConnection->Close();
	}
	catch(...)
	{
		return 0;
	}
	if(m_pFtpConnection != NULL)
		delete m_pFtpConnection;

	return 1;
}

BOOL GetInternetErrorMessage(LPTSTR lpszError, UINT nMaxError)
{
	CString  strerr;
	DWORD    dwLen = 0;
	DWORD    dwError;
	if (!InternetGetLastResponseInfo(&dwError, NULL, &dwLen) &&
		GetLastError() != ERROR_INSUFFICIENT_BUFFER)
		return FALSE;

	dwLen += 2;

	if (!InternetGetLastResponseInfo(&dwError, strerr.GetBuffer(dwLen + 2), &dwLen))
	{
		strerr.ReleaseBuffer();
		return FALSE;
	}

	strerr.ReleaseBuffer();

	_tcsncpy(lpszError, strerr, nMaxError);
	lpszError[nMaxError - 1] = 0;
	return TRUE;
}

int FtpManager::_downLoadFilesImp(CStringArray *remoteArray,CStringArray *localArray,int number_file)
{
	// init some var
	BOOL goodfile;
	int x=0;
	int nb_lost_file =0;
	int missed[1024];
	// while loop to transfer every file in the array
	while(x < number_file)
	{
		// try to get file
		goodfile = m_pFtpConnection->GetFile(
			remoteArray->GetAt(x),
			localArray->GetAt(x),
			FALSE,
			FILE_ATTRIBUTE_NORMAL,
			FTP_TRANSFER_TYPE_BINARY | INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE
			);
		missed[x] = goodfile ? 0 : 1;
		// if failed, missed[x] become 1
		// if good, missed become 0
		if(missed[x])
			nb_lost_file++;
		// if the file was missed, increase the number of
		// missing file.
		// increase to the next file
		x++;
	}
	//return the number of missing file, if any.
	return nb_lost_file;
}

int FtpManager::_pushFilesImp(CStringArray *remoteArray,CStringArray *localArray,int number_file)
{
	BOOL goodfile;
	int x=0;
	int nb_lost_file =0;
	bool missed[1024];
	// while loop to transfer every file in the array
	while(x < number_file)
	{
		// try to get file
		goodfile=m_pFtpConnection->PutFile(
			localArray->GetAt(x),
			remoteArray->GetAt(x),
			FTP_TRANSFER_TYPE_BINARY,
			1
			);

		//int x = GetLastError();
		//TCHAR XXX[4096] = { 0 };
		//GetInternetErrorMessage(XXX, 4096);

		missed[x] = goodfile ? 0 : 1;
		// if failed, missed[x] become 1
		// if good, missed become 0
		if(missed[x])
			nb_lost_file++;
		// if the file was missed, increase the number of
		// missing file.
		// increase to the next file
		x++;
	}
	//return the number of missing file, if any.
	return nb_lost_file;

}

int FtpManager::pushFiles(vector<wstring>& files)
{
	if(!m_pInternetSession)
	{
		return -1;
	}
	CString tempRemote;
	//CString tempLocal;
	CString folder;
	CString server;
	CStringArray remote;
	CStringArray local;
	CString Error;

	CString strcopyFile;
	TCHAR tmp[255] = { 0 };
	GetTempPath(255, tmp);
	CTime tm;
	tm = CTime::GetCurrentTime();
	CString dir_name = tm.Format("%Y_%m_%d_%H_%M_%S");
	dir_name += L".txt";

	strcopyFile = tmp + dir_name;
	CopyFile(files[0].c_str(), strcopyFile, FALSE);

	remote.Add(dir_name);
	local.Add(strcopyFile);


	/*TCHAR szFilePath[MAX_PATH + 1] = { 0 };
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0;
	CString str_url = szFilePath;
	for (int i = 0;i < files.size(); ++i)
	{
		wstring name = files[i];
		CString tempRemote = dir_name + name.c_str();
		remote.Add(tempRemote);
		CString tempLocal = name.c_str();
		local.Add(tempLocal);
	}
*/


	//strUser="ABCD";           //用户名
	//strPass="EFGH";         //密码
	//server="192.168.1.100";    //server的IP

	//tempRemote="2.txt";        //上传之后显示的文件名
	//remote.Add(tempRemote);
	//tempLocal = "c:\\download\\1.txt";//上传文件路径及名字
	//local.Add(tempLocal);

	// open server
	bool conectOK;
	conectOK = openConnection(m_server_ip);
	// transfer multiple file
	if(conectOK)
	{
		_pushFilesImp(&remote,&local,files.size());
	}


	// close connection
	closeConnection();
}
