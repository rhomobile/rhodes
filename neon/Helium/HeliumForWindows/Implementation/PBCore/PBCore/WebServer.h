#pragma once
#include <windows.h>
#include <string>
#include <vector>
#include <map>
//  By default WebServer used winsock2, Rhodes extension requires winsock.h
#ifndef BUILD_AS_RHODES_EXTENSION
#include <winsock2.h>

//Include WinSocket Libaray
#pragma comment(lib,"ws2.lib")
#else
#include <winsock.h>
#endif

#define WEBSERVER_TIMEOUT	5000
#define MAX_HEADER_REQ		4096
#define MAX_REPLY_BUFFER	80
#define MAX_THREADS			8
#define FILE_BUFFER			4096

static const LPSTR HTTP200_HEADER		= "HTTP/1.1 200 OK\r\nServer: RhoElements Internal Server\r\nAccept-Ranges: bytes\r\n";
static const LPSTR HTTP400_HEADER		= "HTTP/1.1 400 Bad Request\r\n";
static const LPSTR HTTP404_HEADER		= "HTTP/1.1 404 Page Not Found\r\n";
static const LPSTR HTTP500_HEADER		= "HTTP/1.1 500 Internal Server Error\r\n";
static const LPSTR HTTP_CACHE_CONTROL	= "Cache-control: no-cache,must-revalidate\r\nPragma: no-cache\r\n";
static const LPSTR HTTP_CONNECTION		= "Connection: close\r\n";
static const LPSTR HTTP_CONTENT			= "Content-Type: %hs\r\n";
static const LPSTR HTTP_CONTENT_LEN		= "Content-Length :%i\r\n";
static const LPSTR HTTP_TERMINATE		= "\r\n";
static const LPSTR HTTP_DEFAULT_PAGE	= "/index.html";
static const LPSTR HTTP_DEFAULT_TYPE	= "text/text";

class CWebServer
{
public:
	CWebServer(void);
	~CWebServer(void);

	//Initiaise Server
	BOOL Initialise();
	void Shutdown();

	//Setters
	void SetPortNumber(USHORT PortNumber);
	void SetAllowPublic(BOOL bAllow);
	void SetWebFolder(LPCWSTR szFolder);
	void AddMimeType(LPCWSTR szFileType,LPCSTR szMimeType);

	//Getters
	BOOL GetAllowPublic();
	USHORT GetPortNumber();
	LPCWSTR GetWebFolder();


protected:
	static DWORD WINAPI WebServerThread(LPVOID lpParameter);
	DWORD RunServer();

private:
	void ProcessRequest();
	BOOL SendBuffer(LPSTR szBuffer,DWORD dwLength);
	void SendPage(LPWSTR szFilename);

private:
	std::wstring		szWebFolder;					//Current Web Folder
	USHORT				usServerPort;					//Current Server Port
	BOOL				bAllowPublic;					//Allow External Access
	HANDLE				hThreadEnd;						//End Thread Signal
	HANDLE				hThreadId;						//Servers Thread ID
	SOCKET				sConnection;					//Incomming Socket
	SOCKET				sServerSocket;					//Server Socket
	std::map<std::wstring,std::string> content;			//MIME Type Contents
};
