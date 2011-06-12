#pragma once

#include <ws2tcpip.h>
#include <winsock2.h>

class LogServer
{
	SOCKET m_listenSocket;
	TCHAR* m_logFilePath;
	TCHAR* m_logPort;
	TCHAR* m_logHost;
	HANDLE m_hLogFile;
public:
	LogServer(TCHAR* logFilePath, TCHAR* log_port);
	~LogServer(void);
	//
	bool init();
	//
	void run();

private:
	bool createFile();

	bool writeToFile(const char*, const int );
};
