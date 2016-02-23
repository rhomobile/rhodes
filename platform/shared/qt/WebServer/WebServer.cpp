#include "WebServer.h"
#include "common/RhodesApp.h"

/*******************************************************************/
//Class Contructor
CWebServer::CWebServer(void)
{
	WSADATA	wsa={0};

	//Startup Winsock
	WSAStartup(MAKEWORD(2,2),&wsa);

	szWebFolder = L"";
	usServerPort = 0;
	hThreadId = 0;
	bAllowPublic = FALSE;
	sServerSocket = INVALID_SOCKET;
	sConnection = INVALID_SOCKET;
	content.clear();
}
/*******************************************************************/
//Class Destrutor
CWebServer::~CWebServer(void)
{
	//Make sure Server Thread is Shutdown
	Shutdown();
	WSACleanup();
}
/*******************************************************************/
//Initialise the Web Server
BOOL CWebServer::Initialise()
{
	SOCKADDR_IN	sai;

	//Set up Socket
	sServerSocket = socket(AF_INET,SOCK_STREAM,0);
	if (sServerSocket == INVALID_SOCKET) return FALSE;

	//Bind to socket
	ZeroMemory(&sai,sizeof(sai));
	sai.sin_family = AF_INET;
	sai.sin_port = htons(usServerPort);
	sai.sin_addr.s_addr = htonl(bAllowPublic ? INADDR_ANY : INADDR_LOOPBACK);
	if (bind(sServerSocket,(SOCKADDR *)&sai,sizeof(SOCKADDR_IN)) != 0)
	{
		closesocket(sServerSocket);
		sServerSocket = INVALID_SOCKET;
		return FALSE;
	}

	//Get the port number if port 0 was specified
	int iLen = sizeof(SOCKADDR_IN);
	getsockname(sServerSocket,(SOCKADDR *)&sai,&iLen);
	usServerPort = htons(sai.sin_port);

	//listen for incomming connections
	if (listen(sServerSocket,128) != 0)
	{
		closesocket(sServerSocket);
		sServerSocket = INVALID_SOCKET;
		return FALSE;
	}

	//Create a stop event
	hThreadEnd = CreateEvent(NULL,TRUE,FALSE,NULL);
	if (!hThreadEnd) 
	{
		closesocket(sServerSocket);
		sServerSocket = INVALID_SOCKET;
		return FALSE;
	}
	LOG(TRACE) + "creating network server thread";

	//Start up Web Server Thread
	hThreadId = CreateThread(NULL,NULL,CWebServer::WebServerThread,(LPVOID)this,NULL,NULL);
	if (!hThreadId)
	{
		CloseHandle(hThreadEnd);
		closesocket(sServerSocket);
		hThreadEnd = NULL;
		sServerSocket = INVALID_SOCKET;
		return FALSE;
	}

	return TRUE;
}
/*******************************************************************/
//Shutdown Web Server
void CWebServer::Shutdown()
{
	//Is Web Server Thread Running
	if (hThreadId)
	{
		//Signal Thread Closure Request
		SetEvent(hThreadEnd);

		//Closedown server socket
		closesocket(sServerSocket);
		sServerSocket = INVALID_SOCKET;

		//Wait for thread to exit
		if (WaitForSingleObject(hThreadId,WEBSERVER_TIMEOUT) == WAIT_TIMEOUT)
		{
			//Thread has failed to finish
			TerminateThread(hThreadId,0);
		}
		CloseHandle(hThreadId);
		CloseHandle(hThreadEnd);
		hThreadId = NULL;
		hThreadEnd = NULL;
	}
}
/*******************************************************************/
//Web Server Thread
DWORD WINAPI CWebServer::WebServerThread(LPVOID lpParameter)
{
	CWebServer *Server = (CWebServer *) lpParameter;
	return Server->RunServer();
}
/*******************************************************************/
//Main Web Server
DWORD CWebServer::RunServer()
{
	BOOL	bQuit = FALSE;
	while (!bQuit)
	{
		if (WaitForSingleObject(hThreadEnd,0) == WAIT_OBJECT_0) break;
		sConnection = accept(sServerSocket,NULL,NULL);
		LOG(TRACE) + "connection accepted at n/w server";
		if (sConnection == INVALID_SOCKET) break;
		ProcessRequest();
		closesocket(sConnection);
		sConnection = INVALID_SOCKET;
	}
	return 0;
}

/*******************************************************************/
//Send a packet of data
BOOL CWebServer::SendBuffer(LPSTR szBuffer, DWORD dwLength)
{
	DWORD	dwSent = 0;
	DWORD	dwResult = 0;

	//If the connection still valid
	if (sConnection == INVALID_SOCKET) return FALSE;

	while(dwSent != dwLength)
	{
		dwResult = send(sConnection,(LPSTR) szBuffer+dwSent,dwLength - dwSent,0);
		if ((dwResult == SOCKET_ERROR) || (dwResult == 0)) return FALSE;
		dwSent += dwResult;
	}
	return TRUE;
}

/*******************************************************************/
//Process HTTP Request
void CWebServer::ProcessRequest()
{
CHAR	szRequest[MAX_HEADER_REQ] ={0};
CHAR	szReqPage[MAX_HEADER_REQ] = {0};
WCHAR	szFile[MAX_PATH] = {0};
DWORD	dwReceived = 0;
LPSTR	p;
int		ret;

	for (;;)
	{
		ret = recv(sConnection,szRequest+dwReceived,MAX_HEADER_REQ-dwReceived,0);
		if (ret < 0) return;
		if(0 == ret)
		{
			//client closed the socket
			LOG(TRACE) + "client closed the connection";
			return;
		}
		dwReceived += ret;

		//Header Terminator Found ?
		if ((p=strstr(szRequest,HTTP_TERMINATE))) break;

		if (dwReceived == MAX_HEADER_REQ) dwReceived -= 4;
		
	}
	LOG(TRACE) + szRequest;

	//Get Requested Page
	p = strstr(szRequest,"HTTP/1.");
	if (p == NULL)
	{
        LOG(TRACE) + "invalid request received";

		SendBuffer(HTTP400_HEADER,strlen(HTTP400_HEADER));			//Send 400 Error (Invalid Request)
	}
	else
	{
		LOG(TRACE) + "valid request received";
		//Get the request page name from HTTP request
		if(szRequest[0] =='G')
		{
			//for GET request eg: GET /public/resources/hello.json HTTP/1.1
		  strncpy(szReqPage,(szRequest+4),p-szRequest-5);
		}
		else
		{
			//for POST request eg: POST /public/resources/getTabs.json HTTP/1.1
			strncpy(szReqPage,(szRequest+5),p-szRequest-5);
			
		}

		//Concat Page and Working Folder to produce a file
		if (szReqPage[0] == '\0' || szReqPage[1] == '\0') strcpy(szReqPage,HTTP_DEFAULT_PAGE);	
		wsprintf(szFile,L"%ls%hs",szWebFolder.c_str(), szReqPage);		
		LPWSTR param = wcsstr(szFile,L"?");
		if (param) *param = L'\0';
		
		LOG(TRACE) + "Requested page is : " + szFile;

		//Does the File Exists
		if (GetFileAttributes(szFile) == MAXDWORD)
		{
			LOG(TRACE) + "file not found";
			SendBuffer(HTTP404_HEADER,strlen(HTTP404_HEADER));			//Send 404 Error (Not Found)
		}
		else
		{
			//Send The Requested Page
			SendPage(szFile);
			return;
		}
	}
	//Send Closing Headers
	SendBuffer(HTTP_CACHE_CONTROL,strlen(HTTP_CACHE_CONTROL));	//Send Cache Control Header
	SendBuffer(HTTP_CONNECTION,strlen(HTTP_CONNECTION));		//Send Connection Close
	SendBuffer(HTTP_TERMINATE,strlen(HTTP_TERMINATE));			//Terminate HTTP Header
}

/*******************************************************************/
//Send a single page
void CWebServer::SendPage(LPWSTR szFilename)
{
	LPWSTR  szExt;
	LPSTR	szType = HTTP_DEFAULT_TYPE;
	CHAR	szReply[MAX_REPLY_BUFFER] = {0};
	BYTE	FileData[FILE_BUFFER] = {0};
	DWORD	dwFileSize;
	DWORD	dwRead;
	HANDLE	hFile;

	//Get File Extention
	szExt = &szFilename[wcslen(szFilename)];
	while (*szExt != L'.' && szExt != szFilename) szExt--;

	//Found Extention
	if (*szExt == L'.')
	{
		szExt++;
		if (content[szExt].c_str() != NULL) szType=(LPSTR) content[szExt].c_str();
	}	
	LOG(TRACE) + L"file name requested :" + szFilename;
	
	//Open the File
	hFile = CreateFile(szFilename,GENERIC_READ,NULL,NULL,OPEN_EXISTING,NULL,NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		LOG(TRACE) + "unable to open requested file";
		SendBuffer(HTTP500_HEADER,strlen(HTTP500_HEADER));			//Send 500 Internal Server Error
		SendBuffer(HTTP_CACHE_CONTROL,strlen(HTTP_CACHE_CONTROL));	//Send Cache Control Header
		SendBuffer(HTTP_CONNECTION,strlen(HTTP_CONNECTION));		//Send Connection Close
		SendBuffer(HTTP_TERMINATE,strlen(HTTP_TERMINATE));			//Terminate HTTP Header
		return;
	}
	
	SendBuffer(HTTP200_HEADER,strlen(HTTP200_HEADER));			//Send 200 Ok 

	//Get The File Size
	dwFileSize = GetFileSize(hFile,NULL);

	//Contruct Response 
	sprintf(szReply,HTTP_CONTENT,szType);
	SendBuffer(szReply,strlen(szReply));

	SendBuffer(HTTP_CACHE_CONTROL,strlen(HTTP_CACHE_CONTROL));
	SendBuffer(HTTP_CONNECTION,strlen(HTTP_CONNECTION));		//Send Connection Close
	sprintf(szReply,HTTP_CONTENT_LEN,dwFileSize);
	SendBuffer(szReply,strlen(szReply));
	SendBuffer(HTTP_TERMINATE,strlen(HTTP_TERMINATE));			//Terminate HTTP Header

	//Send the contents of the file
	while ((ReadFile(hFile,FileData,FILE_BUFFER,&dwRead,NULL) == TRUE) && (dwRead != 0))
	{
		SendBuffer((LPSTR)FileData,dwRead);
	}
	CloseHandle(hFile);
}
/*******************************************************************/
void CWebServer::SetPortNumber(USHORT PortNumber)
{
	//If Server is already running, You can't change the settings
	if (sServerSocket != INVALID_SOCKET) return;

	usServerPort = PortNumber;
}
/*******************************************************************/
USHORT CWebServer::GetPortNumber()
{
	return usServerPort;
}
/*******************************************************************/
void CWebServer::SetAllowPublic(BOOL bAllow)
{
	//If Server is already running, You can't change the settings
	if (sServerSocket != INVALID_SOCKET) return;
	bAllowPublic = bAllow;
}
/*******************************************************************/
BOOL CWebServer::GetAllowPublic()
{
	return bAllowPublic;
}
/*******************************************************************/
void CWebServer::SetWebFolder(LPCWSTR szFolder)
{
	//If Server is already running, You can't change the settings
	if (sServerSocket != INVALID_SOCKET) return;

	szWebFolder.assign(szFolder);
}
/*******************************************************************/
LPCWSTR CWebServer::GetWebFolder()
{
	return szWebFolder.c_str();
}
/*******************************************************************/
void CWebServer::AddMimeType(LPCWSTR szFileType,LPCSTR szMimeType)
{
	content[std::wstring(szFileType)] = std::string(szMimeType);
}


