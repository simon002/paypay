#include "stdafx.h"
#include "ftpmanager.h"
FtpManager::FtpManager():
m_name(L""),
m_password(L""),
m_server_ip(L""),
m_pFtpConnection(NULL),
m_pInternetSession(NULL)

{
	CString strAppName;
	strAppName.LoadString(AFX_IDS_APP_TITLE);
	m_pInternetSession = new CInternetSession(strAppName,INTERNET_OPEN_TYPE_PRECONFIG);
}

FtpManager::~FtpManager()
{
	
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
		m_password.c_str());
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

int FtpManager::downLoadFiles(CStringArray *remoteArray,CStringArray *localArray,int number_file)
{
	// init some var
	BOOL goodfile;
	int x=0;
	int nb_lost_file =0;
	int missed[10240000];
	// while loop to transfer every file in the array
	while(x)
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
