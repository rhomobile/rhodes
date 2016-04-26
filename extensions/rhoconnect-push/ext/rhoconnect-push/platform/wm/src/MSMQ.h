#include <string>
#include <windows.h>
#include <Mq.h>
using namespace std;
namespace RhoIPC
{
const int BUFLEN = 256;

enum ReadWriteResult
{
	OK,
	BufferFail,
	Disconnected,
	Timeout,
	InvalidHandle
};

enum EventArgs
{
	Empty,
	NonEmpty
};

class MSMQ;

typedef void (*EventHandler)(MSMQ*, int);

class Message
{
public :
	Message() : m_Buffer(0)
	{
	}

	Message(const char* buf, bool isAlert) : m_Buffer(0)
	{
		int size = strlen(buf);
		Init(buf, size);
		m_Length = size;
	}
	Message(const char* buf, int size) : m_Buffer(0)
	{
		Init(buf, size);
		m_Length = size;
	}
	Message(const char* buf,int size, bool isAlert) : m_IsAlert(isAlert)
	{
		Init(buf, size);
	}
	~Message()
	{
		if(m_Buffer)
			delete m_Buffer;
	}

	void setMessageBytes(char* buf, int size)
	{
		if(m_Buffer)
			delete m_Buffer;
		Init(buf, size);
	}

	bool IsAlert ()  const {return m_IsAlert;}
	unsigned int Length()  const {return m_Length;}
	char* MessageBytes () const {return m_Buffer;}
	void setAlert(bool isAlert)
	{
		m_IsAlert = isAlert;
	}
private:
	char* m_Buffer;
	bool m_IsAlert;
	unsigned int m_Length;

	void Init(const char* buf, int size)
	{
		m_Buffer = new char[size];
		memcpy(m_Buffer, buf, size);
		m_Length = size;
	}
};

class MSMQ
{
public:
	static const int TimeoutInfinite = -1;
	static const unsigned int InfiniteQueueSize = 0;
	MSMQ(bool forReading);
	MSMQ(bool forReading, wstring name);
	MSMQ(bool forReading, wstring name, int maxMessageLength, int maxMessages);
	MSMQ(bool forReading, wstring name, int maxMessageLength, int maxMessages, bool& createdNew);
    MSMQ(HANDLE h, bool forReading);
	~MSMQ();
	bool CanRead();
	bool CanWrite();
	int Send(Message* msg, int timeoutMillis);
	int Receive(Message* msg, int timeoutMillis);
	int Receive(Message* msg);
	void Close();
	int MessagesInQueueNow();
	Message* getMessage();

	EventHandler DataOnQueueChanged;
	void StartEventThread();
	void StopEventThread();
	void Init(bool forReading, wstring name, int maxMessageLength);

private:  
    static const int DefaultMaxMsgLength = 4096;
	bool m_isCanceled;
	bool m_IsForReading;          
	wstring m_Name;   
	HANDLE m_hQue;      
	char* m_ReadBuffer;  
	Message* m_hMessage;
	HANDLE m_tHandle;

	static DWORD WINAPI ProcessThread( LPVOID lpParam );
	int MonitorHandle();
};
};