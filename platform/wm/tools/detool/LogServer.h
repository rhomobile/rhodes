#pragma once

#include <ws2tcpip.h>
#include <winsock2.h>

class LogServer
{
	SOCKET m_listenSocket;
	TCHAR* m_logFilePath;
	HANDLE m_hLogFile;
public:
	LogServer(TCHAR* logFilePath);
	~LogServer(void);
	//
	bool init();
	//
	void run();

private:
	bool createFile();

	bool writeToFile(const char*, const int );
};
