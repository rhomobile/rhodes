#include "stdafx.h"
#include "LogServer.h"
#include <vector>

#define DEFAULT_BUFLEN 1024
#define DEFAULT_PORT "11000"
#define DEFAULT_HOST _T("localhost")

LogServer::LogServer(TCHAR* logFilePath, TCHAR* log_port) 
	: m_logFilePath(logFilePath)
	, m_hLogFile(INVALID_HANDLE_VALUE)
	, m_logPort(log_port)
    , m_logHost(DEFAULT_HOST)
{
}

LogServer::~LogServer(void)
{
	::CloseHandle(m_hLogFile);

	closesocket(m_listenSocket);
	m_listenSocket = INVALID_SOCKET;
}

//
bool LogServer::init()
{
	USES_CONVERSION;

	addrinfo *result = NULL, hints;

	memset(&hints, 0, sizeof (hints));
	hints.ai_family   = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags    = AI_PASSIVE;

	createFile();

	// Resolve the local address and port to be used by the server
	int iResult = getaddrinfo(T2A(m_logHost), T2A(m_logPort), &hints, &result);
	
	if (iResult != 0) 
	{
		printf("getaddrinfo failed: %d\n", iResult);
		return false;
	}

	// Create a SOCKET for the server to listen for client connections
	m_listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	if (m_listenSocket == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		return false;
	}

	    // Setup the TCP listening socket
    iResult = bind( m_listenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(m_listenSocket);
        return false;
    }

	if ( listen( m_listenSocket, SOMAXCONN ) == SOCKET_ERROR ) {
		printf( "Listen failed with error: %ld\n", WSAGetLastError() );
		closesocket(m_listenSocket);
		return false;
	}

	return true;
}

//
void LogServer::run()
{
	char recvbuf[DEFAULT_BUFLEN];
	int iResult = 0;
	int recvbuflen = DEFAULT_BUFLEN;
	SOCKET clientSocket = INVALID_SOCKET;
	recvbuf[0] = 0;

	// Accept a client socket
	clientSocket = accept(m_listenSocket, NULL, NULL);

	if (clientSocket == INVALID_SOCKET) 
	{
		printf("accept failed: %d\n", WSAGetLastError());
		closesocket(m_listenSocket);
		return;
	}

	// Receive until the peer shuts down the connection
	do 
	{
		iResult = recv(clientSocket, recvbuf, recvbuflen, 0);

		if (iResult > 0)
		{
			recvbuf[iResult - 1] = 0;
			printf("%s\n", recvbuf);
			writeToFile(recvbuf, iResult);
		}
		else if (iResult == 0)
		{
			printf("Connection closing...\n");
		}
		else 
		{
			printf("recv failed: %d\n", WSAGetLastError());
			closesocket(clientSocket);
			return;
		}

	} 
	while (iResult > 0);
}

bool LogServer::createFile()
{
	m_hLogFile = CreateFile(m_logFilePath, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, 
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == m_hLogFile) {
		_tprintf( TEXT("Unable to open host file\n"));
		return false;
	}
	
	return true;
}

bool LogServer::writeToFile(const char* dataBuf, const int dataLen)
{
	static char crlfBuf[] = "\n";

	if (m_hLogFile != INVALID_HANDLE_VALUE)
	{
		DWORD dwNumWritten = 0;
		if (!WriteFile(m_hLogFile, dataBuf, dataLen, &dwNumWritten, NULL)) {
			_tprintf( TEXT("Error !!! writing log file\n"));
			return false;
		}

		if (!WriteFile(m_hLogFile, crlfBuf, sizeof(crlfBuf), &dwNumWritten, NULL)) {
			_tprintf( TEXT("Error !!! writing log file\n"));
			return false;
		}

	}

	return true;
}