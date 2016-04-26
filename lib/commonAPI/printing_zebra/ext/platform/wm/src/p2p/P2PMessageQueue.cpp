//#include "stdafx.h"
#include "P2PMessageQueue.h"

namespace rho
{
namespace printing
{
namespace ipc
{

P2PMessageQueue::P2PMessageQueue(HANDLE h, bool forReading)
{
	if (h = NULL)
	{
		throw "Could not create queue";
	}
	m_hQue = h;
	m_IsForReading = forReading;
	m_ReadBuffer = new char[MaxMessageLength()];
	StartEventThread();
}

P2PMessageQueue::P2PMessageQueue(bool forReading)
{ 
	Init(forReading, L"", DefaultMaxMsgLength, InfiniteQueueSize);
}

P2PMessageQueue::P2PMessageQueue(bool forReading, wstring name)
{
	Init(forReading, name, DefaultMaxMsgLength, InfiniteQueueSize);
}

P2PMessageQueue::P2PMessageQueue(bool forReading, wstring name, int maxMessageLength, int maxMessages)
{
	Init(forReading, name, maxMessageLength, maxMessages);
}

P2PMessageQueue P2PMessageQueue::OpenExisting(bool forReading, HANDLE processHandle, HANDLE queueHandle)
{
	LPMSGQUEUEOPTIONS opt = new MSGQUEUEOPTIONS;
	opt->bReadAccess = forReading;
	opt->dwSize = sizeof(MSGQUEUEOPTIONS);
	HANDLE h = OpenMsgQueue(processHandle, queueHandle, opt);
	if (h == NULL)
	{
		return NULL;
	}
	P2PMessageQueue mq = new P2PMessageQueue(h, forReading);
	return mq;
}

P2PMessageQueue::P2PMessageQueue(bool forReading, wstring name, int maxMessageLength, int maxMessages, bool& createdNew)
{
	Init(forReading, name, maxMessageLength, maxMessages);
	createdNew = (GetLastError() != ERROR_ALREADY_EXISTS);
}

P2PMessageQueue::~P2PMessageQueue()
{
	Close();
}

void P2PMessageQueue::Init(bool forReading, wstring name, int maxMessageLength, int maxMessages)
{
	DataOnQueueChanged = NULL;
	m_hQue = NULL;

	if (name != L"" && name.length() > MAX_PATH)
	{
		throw "name too long";
	}

	if (maxMessageLength < 0)
	{
		throw "maxMessageLength must be positive";
	}

	if (maxMessages < 0)
	{
		throw "maxMessages must be positive";
	}

	LPMSGQUEUEOPTIONS opt = new MSGQUEUEOPTIONS;
	opt->bReadAccess = forReading;
	opt->dwSize = sizeof(MSGQUEUEOPTIONS);
	opt->cbMaxMessage = maxMessageLength;
	opt->dwMaxMessages = maxMessages;
	opt->dwFlags = MSGQUEUE_ALLOW_BROKEN;

	m_hQue = CreateMsgQueue(name.c_str(), opt);

	if (m_hQue == NULL)
	{
		throw "Could not create queue";
	}

	m_IsForReading = forReading;
	m_Name = name;

	if (forReading)
	{
		m_ReadBuffer = new char[maxMessageLength];
	}

	StartEventThread();
}

void P2PMessageQueue::StartEventThread()
{
	DWORD tId;
	CreateThread( 
		NULL,                   
		0,                     
		ProcessThread,      
		LPVOID(this),         
		0,                   
		&tId);
}

int P2PMessageQueue::MaxMessageLength() const
{
	return GetInfo()->cbMaxMessage;
}

DWORD WINAPI P2PMessageQueue::ProcessThread( LPVOID lpParam )
{
	P2PMessageQueue* pObject = reinterpret_cast<P2PMessageQueue*>(lpParam);
	pObject->MonitorHandle();

	return 0;
}

int P2PMessageQueue::MonitorHandle()
{
	int res;
	while (m_hQue != NULL)
	{
		res = WaitForSingleObject(m_hQue, TimeoutInfinite);
		if (m_hQue == NULL)
		{
			return 0; 
		}
		if (res == WAIT_OBJECT_0)
		{
			if (EventModify(m_hQue, (int)EVENT_RESET) == 0)
			{
				return 0; 
			}

			if (DataOnQueueChanged != NULL)
			{
				if (CanRead() && MessagesInQueueNow() > 0)
				{ 
					if(DataOnQueueChanged != NULL)
						DataOnQueueChanged(this, NonEmpty);
				}
				else if (CanWrite())
				{
					if(DataOnQueueChanged != NULL)
						DataOnQueueChanged(this, Empty);
				}
			}
		}
		else
		{
			return 0; 
		}
	}
	return 0;
}

LPMSGQUEUEINFO P2PMessageQueue::GetInfo() const
{
	if (m_hQue == NULL)
	{
		throw "Invalid Handle. Please use new queue";
	}

	LPMSGQUEUEINFO qi = new MSGQUEUEINFO();
	qi->dwSize = 28;
	if (GetMsgQueueInfo(m_hQue, qi))
	{
		return qi;
	}
	throw "Failed to get queue info";
}

bool P2PMessageQueue::CanRead()
{
	return m_IsForReading;
}

bool P2PMessageQueue::CanWrite()
{
	return !m_IsForReading;
}
HANDLE P2PMessageQueue::Handle()
{
	return m_hQue;
}

wstring P2PMessageQueue::QueueName()
{
	return m_Name;
}

int P2PMessageQueue::MaxMessagesAllowed()
{
	return GetInfo()->dwMaxMessages;
}

int P2PMessageQueue::MostMessagesSoFar()
{
	return GetInfo()->dwMaxQueueMessages;
}

int P2PMessageQueue::MessagesInQueueNow()
{
	return GetInfo()->dwCurrentMessages;
}

short P2PMessageQueue::CurrentReaders()
{
	return GetInfo()->wNumReaders;
}

short P2PMessageQueue::CurrentWriters()
{
	return GetInfo()->wNumWriters;
}

void P2PMessageQueue::Close()
{
	CloseMsgQueue(m_hQue);
	m_hQue = NULL;
}

int P2PMessageQueue::Receive(Message* msg, int timeoutMillis)
{
	if (m_hQue == NULL)
	{
		throw "Invalid Handle. Please use new queue";
	}

	DWORD flags, bytesRead;

	if (ReadMsgQueue(m_hQue, m_ReadBuffer, MaxMessageLength(), &bytesRead, timeoutMillis, &flags))
	{
		char* dst = new char[bytesRead];
		memcpy(dst, m_ReadBuffer, bytesRead);
		msg->setMessageBytes(dst, bytesRead);
		msg->setAlert((flags == MSGQUEUE_MSGALERT ? true : false));
		return OK;

	}
	else
	{
		int err = GetLastError();
		switch (err)
		{
		case ERROR_INSUFFICIENT_BUFFER:
			return BufferFail;
		case ERROR_PIPE_NOT_CONNECTED:
			return Disconnected;
		case ERROR_TIMEOUT:
			return Timeout;
		case ERROR_INVALID_HANDLE:
			Close();
			return InvalidHandle;
		default:
			throw "Failed to read: ";
		}
	}
}

int P2PMessageQueue::Receive(Message* msg)
{
	return Receive(msg, TimeoutInfinite);
}

int P2PMessageQueue::Send(Message* msg)
{
	return Send(msg, TimeoutInfinite);
}

int P2PMessageQueue::Send(Message* msg, int timeoutMillis)
{
	if (m_hQue == NULL)
	{
		throw "Invalid Handle. Please use new queue";
	}

	if (msg->MessageBytes() == NULL)
	{
		throw "Message must contain bytes";
	}

	if (WriteMsgQueue(m_hQue, msg->MessageBytes(), msg->Length(), timeoutMillis, 0))
	{
		return OK;
	}
	else
	{
		int err = GetLastError();
		switch (err)
		{
		case ERROR_INSUFFICIENT_BUFFER:
			return BufferFail;
		case ERROR_PIPE_NOT_CONNECTED:
			return Disconnected;
		case ERROR_TIMEOUT: 
			return Timeout;
		case ERROR_OUTOFMEMORY: 
			return Timeout;
		case ERROR_INVALID_HANDLE:
			Close();
			return InvalidHandle;
		default:
			throw "Failed to write";
		}
	}
}

}
}
}