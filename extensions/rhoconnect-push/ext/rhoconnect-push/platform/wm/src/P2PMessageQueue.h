#include <string>
#include <windows.h>
#include <Msgqueue.h>
using namespace std;
namespace RhoIPC
{
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

class P2PMessageQueue;

typedef void (*EventHandler)(P2PMessageQueue*, int);

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

class P2PMessageQueue
{
public:
	static const int TimeoutInfinite = -1;
	static const unsigned int InfiniteQueueSize = 0;
	P2PMessageQueue(bool forReading);
	P2PMessageQueue(bool forReading, wstring name);
	P2PMessageQueue(bool forReading, wstring name, int maxMessageLength, int maxMessages);
	P2PMessageQueue(bool forReading, wstring name, int maxMessageLength, int maxMessages, bool& createdNew);
    P2PMessageQueue(HANDLE h, bool forReading);
	~P2PMessageQueue();
	bool CanRead();
	bool CanWrite();
	wstring QueueName();
	int MaxMessagesAllowed();
	int MostMessagesSoFar();
	int MessagesInQueueNow();
	short CurrentReaders();
	short CurrentWriters();
	HANDLE Handle();
	int Send(Message* msg, int timeoutMillis);
	int Send(Message* msg);
	int Receive(Message* msg, int timeoutMillis);
	int Receive(Message* msg);
	void Close();
	static P2PMessageQueue OpenExisting(bool forReading, HANDLE processHandle, HANDLE queueHandle);
	int MaxMessageLength() const;
	EventHandler DataOnQueueChanged;

private:  
	static const int DefaultMaxMsgLength = 4096;
	bool m_IsForReading;          
	wstring m_Name;   
	HANDLE m_hQue;      
	char* m_ReadBuffer;  

	void Init(bool forReading, wstring name, int maxMessageLength, int maxMessages);
	void StartEventThread();
	static DWORD WINAPI ProcessThread( LPVOID lpParam );
	int MonitorHandle();
	LPMSGQUEUEINFO GetInfo() const;
};
};