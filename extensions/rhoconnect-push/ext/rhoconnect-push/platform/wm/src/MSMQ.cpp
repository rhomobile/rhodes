#pragma comment(lib, "mqrt.lib")
#include "MSMQ.h"
using namespace RhoIPC;

MSMQ::MSMQ(bool forReading)
{
	m_isCanceled = false;
	Init(forReading, L"", DefaultMaxMsgLength);
}

MSMQ::MSMQ(bool forReading, wstring name)
{
	Init(forReading, name, DefaultMaxMsgLength);
}

MSMQ::~MSMQ()
{
	Close();
}

void MSMQ::Init(bool forReading, wstring name, int maxMessageLength)
{
	DataOnQueueChanged = NULL;
	m_hQue = NULL;

	if (maxMessageLength < 0)
	{
		throw "maxMessageLength must be positive";
	}

	DWORD dwFormatNameBufferLength = 256;            
	WCHAR wszFormatNameBuffer[256];                    

	DWORD mqNotExist = MQPathNameToFormatName(name.c_str(),
		wszFormatNameBuffer,
		&dwFormatNameBufferLength);

	//if(forReading)
	//{

	//	MQOpenQueue(wszFormatNameBuffer, MQ_RECEIVE_ACCESS, MQ_DENY_NONE,  &m_hQue);

	//}
	//else
	//{
		MQQUEUEPROPS   QueueProps;
		MQPROPVARIANT  aQueuePropVar[1];
		QUEUEPROPID    aQueuePropId[1];
		HRESULT        aQueueStatus[1];

		if(mqNotExist)
		{
			DWORD cPropId = 0;
			aQueuePropId[cPropId] = PROPID_Q_PATHNAME;
			aQueuePropVar[cPropId].vt = VT_LPWSTR;
			aQueuePropVar[cPropId].pwszVal = const_cast<wchar_t*>(name.c_str());
			cPropId++;

			/*aQueuePropId[cPropId] = PROPID_Q_TRANSACTION;
			aQueuePropVar[cPropId].vt = VT_UI1;
			aQueuePropVar[cPropId].bVal = MQ_TRANSACTIONAL;
			cPropId++;*/

			QueueProps.cProp = cPropId;               
			QueueProps.aPropID = aQueuePropId;       
			QueueProps.aPropVar = aQueuePropVar;      
			QueueProps.aStatus = aQueueStatus; 

			MQCreateQueue(NULL,         
				&QueueProps,                
				wszFormatNameBuffer,       
				&dwFormatNameBufferLength);
		}

		aQueuePropId[0] = PROPID_Q_PATHNAME;
		aQueuePropVar[0].vt = VT_LPWSTR;
		aQueuePropVar[0].pwszVal = wszFormatNameBuffer;

		if(forReading)
			MQOpenQueue(wszFormatNameBuffer, MQ_RECEIVE_ACCESS, MQ_DENY_RECEIVE_SHARE,  &m_hQue);
		else
			MQOpenQueue(wszFormatNameBuffer, MQ_SEND_ACCESS, MQ_DENY_NONE,  &m_hQue);
	//}

	if (m_hQue == NULL)
	{
		throw "Could not create queue";
	}

	m_IsForReading = forReading;
	m_Name = wszFormatNameBuffer;

	if (forReading)
	{
		m_ReadBuffer = new char[maxMessageLength];
	}

	//if(m_IsForReading)
	//	StartEventThread();
}

void MSMQ::StopEventThread()
{
	TerminateThread(m_tHandle, 0);
	//m_isCanceled = true;
}

void MSMQ::StartEventThread()
{
	DWORD tId;
	m_tHandle = CreateThread( 
		NULL,                   
		0,                     
		ProcessThread,      
		LPVOID(this),         
		0,                   
		&tId);
}

DWORD WINAPI MSMQ::ProcessThread( LPVOID lpParam )
{
	try{
		MSMQ* pObject = reinterpret_cast<MSMQ*>(lpParam);
		pObject->MonitorHandle();
	}
	catch(...)
	{
		return 0;
	}
	
	return 0;
}

Message* MSMQ::getMessage()
{
	return m_hMessage;
}

int MSMQ::MonitorHandle()
{
	int res;
	while (m_hQue != NULL)
	{
		m_hMessage = new Message("", false);
		int result = Receive(m_hMessage, 50);
		if(result == OK && DataOnQueueChanged != NULL)
			DataOnQueueChanged(this, NonEmpty);
		delete m_hMessage;
		/*if(m_isCanceled)
		{
			m_isCanceled = false;
			break;
		}*/
		/*res = WaitForSingleObject(m_hQue, TimeoutInfinite);
		if (m_hQue == NULL)
		{
			return 0; 
		}
		if (res == WAIT_OBJECT_0)
		{
			if (ResetEvent(m_hQue) == 0)
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
		}*/
	}

	return 0;
}

int MSMQ::Send(Message* msg, int timeoutMillis)
{
	if (m_hQue == NULL)
	{
		throw "Invalid Handle. Please use new queue";
	}

	if (msg->MessageBytes() == NULL)
	{
		throw "Message must contain bytes";
	}

	DWORD hr = 0;
	MQMSGPROPS msgProps;
	DWORD cPropId = 0;    
	MSGPROPID aMsgPropId[1];
	MQPROPVARIANT aMsgPropVar[1];
	HRESULT aMsgStatus[1];

	// message body
	aMsgPropId[cPropId] = PROPID_M_BODY;
	aMsgPropVar[cPropId].vt = VT_VECTOR | VT_UI1;
	aMsgPropVar[cPropId].caub.pElems = (LPBYTE)msg->MessageBytes();
	aMsgPropVar[cPropId].caub.cElems = msg->Length();
	cPropId++;

	// initialize the MQMSGPROPS structure.
	msgProps.cProp = cPropId;
	msgProps.aPropID = aMsgPropId;
	msgProps.aPropVar = aMsgPropVar;
	msgProps.aStatus = aMsgStatus;
	hr = MQSendMessage(m_hQue, &msgProps, MQ_NO_TRANSACTION);

	if (hr == MQ_OK)
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
			return Timeout;//throw "Failed to write";
		}
	}
}

int MSMQ::Receive(Message* msg, int timeoutMillis)
{
	if (m_hQue == NULL)
	{
		throw "Invalid Handle. Please use new queue";
	}

	DWORD hr = 0;
	MQMSGPROPS msgprops;
	const int NUMBEROFPROPERTIES = 2;
	DWORD cPropId = 0;
	MSGPROPID aMsgPropId[NUMBEROFPROPERTIES];
	MQPROPVARIANT aMsgPropVar[NUMBEROFPROPERTIES];
	HRESULT aMsgStatus[NUMBEROFPROPERTIES];
	ULONG ulBufferSize = DefaultMaxMsgLength;

	memset(m_ReadBuffer, 0, DefaultMaxMsgLength);
	aMsgPropId[cPropId] = PROPID_M_BODY_SIZE;           // Property ID
	aMsgPropVar[cPropId].vt = VT_NULL;                  // Type indicator
	cPropId++;

	aMsgPropId[cPropId] = PROPID_M_BODY;                // Property ID
	aMsgPropVar[cPropId].vt = VT_VECTOR | VT_UI1;       // Type indicator
	aMsgPropVar[cPropId].caub.pElems = (UCHAR*)m_ReadBuffer;  // Body buffer
	aMsgPropVar[cPropId].caub.cElems = ulBufferSize;    // Buffer size
	cPropId++;


	// Initialize the MQMSGPROPS structure.
	msgprops.cProp = cPropId;                           // Number of message properties
	msgprops.aPropID = aMsgPropId;                      // IDs of the message properties
	msgprops.aPropVar = aMsgPropVar;                    // Values of the message properties
	msgprops.aStatus = aMsgStatus; 

	hr = MQReceiveMessage(m_hQue, timeoutMillis, MQ_ACTION_RECEIVE, &msgprops, NULL, NULL, NULL, MQ_NO_TRANSACTION);


	if (hr == MQ_OK && aMsgPropVar[0].ulVal)
	{
		DWORD bytesRead = strlen(m_ReadBuffer);
		char* dst = new char[bytesRead];
		memcpy(dst, m_ReadBuffer, bytesRead);
		string tmp = dst;
		std::size_t begin = tmp.find("<string>") + 8;
		std::size_t len = tmp.find("</string>") - begin;
		tmp = tmp.substr(begin, len);
		msg->setMessageBytes(const_cast<char*>(tmp.c_str()), tmp.length());
		//msg->setAlert((flags == MSGQUEUE_MSGALERT ? true : false));
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
			return Timeout;//throw "Failed to read: ";
		}
	}
}

int MSMQ::Receive(Message* msg)
{
	return Receive(msg, INFINITE);
}

void MSMQ::Close()
{
	if(m_hQue)
		MQCloseQueue(m_hQue);
	m_hQue = NULL;
}

bool MSMQ::CanRead()
{
	return m_IsForReading;
}

bool MSMQ::CanWrite()
{
	return !m_IsForReading;
}

int MSMQ::MessagesInQueueNow()
{
	QUEUEPROPID   propId[1];
	MQPROPVARIANT propVar[1];
	HRESULT       propStatus[1];
	DWORD         cprop = 0;
	propId [cprop] = PROPID_MGMT_QUEUE_MESSAGE_COUNT;
	propVar[cprop].vt = VT_NULL;
	++cprop;
	MQMGMTPROPS mqProps;
	mqProps.cProp    = cprop;
	mqProps.aPropID  = propId;
	mqProps.aPropVar = propVar;
	mqProps.aStatus  = propStatus;
	HRESULT hr = MQMgmtGetInfo( NULL, m_Name.c_str(), &mqProps );
	if( FAILED( hr ) )
	{
		return 0;
	}
	else
	{
		return mqProps.aPropVar[0].ulVal;
	}
}
