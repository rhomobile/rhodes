//#include "stdafx.h"
//#include "json/json_object.h"
//#include "json/json_tokener.h"
#include "net/INetRequest.h"
#include <common/RhodesApp.h>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include "PushClient.h"
#include "sync/RhoconnectClientManager.h"
#include "common/RhoConf.h"
#include "json/JSONIterator.h"
#include "Winuser.h"
#include "Push.h"
using namespace rho::sync;
#ifdef OS_WINDOWS_DESKTOP
#include "objbase.h"
const wstring SERVICE_MQ_NAME = L".\\Private$\\FEF15A2B-CDF1-45e3-B1C6-D71E3718AFB8";
#else
const wstring SERVICE_MQ_NAME = L"FEF15A2B-CDF1-45e3-B1C6-D71E3718AFB6";
#endif
const string RPS_CLIENT_NAME="rhoconnect";
const char* RPS_APP_NAME = "Push.rhoconnect.pushAppName";
const char* RPS_SERVER = "Push.rhoconnect.pushServer";
//const string APPLICATION_SERVER = "http://win-appserver.herokuapp.com/token";
extern "C" struct json_object* json_tokener_parse(char *str);
extern "C" void json_object_object_add(struct json_object* obj, char *key,
struct json_object *val);
extern "C" struct json_object* json_object_new_string(char *s);
extern "C" struct json_object* json_object_new_object(void);
extern "C" char* json_object_to_json_string(struct json_object *obj);
extern "C" char* json_object_get_string(struct json_object *obj, /*OUT*/int* len);
extern "C" int64 json_object_get_int(struct json_object *obj);
extern "C" struct json_object* json_object_object_get(struct json_object* obj,
													  char *key);
extern "C" void rho_clientregister_create(const char* szDevicePin);
extern "C" void alert_show_status(const char* szTitle, const char* szMessage, const char* szHide);
extern "C" void alert_vibrate(int duration_ms);
extern "C" void alert_play_file(char* file_name, char* media_type);
extern "C" HWND getMainWnd();
//CMainWindow* Rhodes_getMainWindow();

string PushClient::m_callbackUri = "";
string PushClient::m_userName = "";
string PushClient::m_pass = "";
string PushClient::m_token = "";
string PushClient::m_appName = "";
string PushClient::m_session = "";
bool PushClient::m_isDisabled = false;
#ifdef OS_WINDOWS_DESKTOP
wstring queueName = L".\\Private$\\";
MSMQ* PushClient::m_to = 0;
MSMQ* PushClient::m_from = 0;
#else
P2PMessageQueue* PushClient::m_to = 0;
P2PMessageQueue* PushClient::m_from = 0;
#endif
bool PushClient::m_isRegistered = false;

std::wstring StringToWString(const std::string& s)
{
	std::wstring temp(s.length(),L' ');
	std::copy(s.begin(), s.end(), temp.begin());
	return temp;
}

std::string WStringToString(const std::wstring& s)
{
	std::string temp(s.length(), ' ');
	std::copy(s.begin(), s.end(), temp.begin());
	return temp;
}

void Split(vector<std::string>& strings, string data, char delim)
{
	istringstream f(data);
	string s;    
	while (getline(f, s, delim)) {
		strings.push_back(s);
	}
}

bool CompareCaseInsensitive(string strFirst, string strSecond)
{
	transform(strFirst.begin(), strFirst.end(), strFirst.begin(), toupper);
	transform(strSecond.begin(), strSecond.end(), strSecond.begin(), toupper);
	if(strFirst == strSecond) return true; else return false;
}

void Trim(string& str)
{
	string::size_type pos = str.find_last_not_of(' ');
	if(pos != string::npos) {
		str.erase(pos + 1);
		pos = str.find_first_not_of(' ');
		if(pos != string::npos) str.erase(0, pos);
	}
	else str.erase(str.begin(), str.end());
}

template<class T>
int inline findAndReplace(T& source, const T& find, const T& replace)
{
	int num=0;
	typename size_t fLen = find.size();
	typename size_t rLen = replace.size();
	for (size_t pos=0; (pos=source.find(find, pos))!=T::npos; pos+=rLen)
	{
		num++;
		source.replace(pos, fLen, replace);
	}
	return num;
}

int PushClient::Register()
{
	LOG(INFO) + "Register Begin";
	if(m_isDisabled)
	{
		LOG(ERROR) + "ans service is not started";	
		return 0;
	}

	//string tmp = m_appName;
	//tmp += "-";
	//tmp += m_userName;
#ifdef OS_WINDOWS_DESKTOP
	/*GUID  guid;
	CoCreateGuid(&guid);
	LPOLESTR szGUID = new WCHAR [39];
 	StringFromGUID2(guid,szGUID,39);*/
	queueName = L".\\Private$\\";
	queueName.append(StringToWString(m_appName));//szGUID);
#else
	wstring queueName = StringToWString(m_session);
#endif
	LOG(INFO) + queueName.c_str();
	if(m_to || m_from)
	{
		LOG(ERROR) + "already registered";
		return 0;
	}

	try
	{
#ifdef OS_WINDOWS_DESKTOP
    m_to = new MSMQ(false, SERVICE_MQ_NAME);
	m_from = new MSMQ(true, queueName);
#else
	m_to = new P2PMessageQueue(false, SERVICE_MQ_NAME);
	m_from = new P2PMessageQueue(true, queueName.substr(1,100));
#endif
	}
	catch(...)
	{
		LOG(ERROR) + "can not create message queue";
		return 0;
	}

	if(!sendTokenRequest())
		sendTokenRequest();

	rho::push::CPushManager::getInstance() -> setDeviceId(RPS_CLIENT_NAME, m_token.c_str());

	LOG(INFO) + "ANS Client Registered";
	
	m_from->DataOnQueueChanged = messageHandler;

	m_isRegistered = true;

	LOG(INFO) + "Register End";

	return 0;
}	

int PushClient::sendTokenRequest()
{
	json_object* obj = json_object_new_object();
	json_object_object_add(obj, "Guid", json_object_new_string(const_cast<char*>(m_appName.c_str()))); 
/*#ifdef OS_WINDOWS_DESKTOP
	json_object_object_add(obj, "MQName", json_object_new_string(const_cast<char*>(WStringToString(queueName).c_str()))); 
#endif*/
	json_object_object_add(obj, "User", json_object_new_string(const_cast<char*>(m_userName.c_str()))); 
	json_object_object_add(obj, "Pass", json_object_new_string(const_cast<char*>(m_pass.c_str())));
	json_object_object_add(obj, "Session", json_object_new_string(const_cast<char*>(m_session.c_str())));
	json_object_object_add(obj, "ServerUrl", json_object_new_string(const_cast<char*>(RHOCONF().getString(RPS_SERVER).c_str()))); 
	if(!m_isRegistered)
	{
		TCHAR path[MAX_PATH];
		GetModuleFileName(NULL, path, MAX_PATH);
		json_object_object_add(obj, "Path", json_object_new_string(const_cast<char*>(WStringToString(path).c_str()))); 
		json_object_object_add(obj, "Args", json_object_new_string(const_cast<char*>("-rhoconnectpush"))); 
		//json_object_object_add(obj, "Args", json_object_new_string(const_cast<char*>(m_appName.c_str()))); 
		//json_object_object_add(obj, "ServerUrl", json_object_new_string(const_cast<char*>(RHOCONF().getString("rhoconnect_push_server").c_str()))); 
		json_object_object_add(obj, "Token", 0);
		json_object_object_add(obj, "Command", json_object_new_string(const_cast<char*>("Register"))); 
	}
	else {
		json_object_object_add(obj, "Command", json_object_new_string(const_cast<char*>("Unregister")));
	}

	Message* message = new Message(json_object_to_json_string(obj), false);
	int result = m_to->Send(message, 0);
	LOG(INFO) + "token request is sent , registered: " + m_isRegistered;
	result = m_from->Receive(message);
	LOG(INFO) + message->MessageBytes();
#ifdef OS_WINDOWS_DESKTOP
	m_from->StartEventThread();
#endif
	if (result == OK)
	{
		json_object* value = json_tokener_parse(message->MessageBytes());
		int len;
		string action = json_object_get_string(json_object_object_get(value, "Action"), &len);
        LOG(INFO) + "Received action: " + action;
		if(!m_isRegistered)
		{
			if(action == "Deleted")
			{
				delete obj;
				delete value;
				delete message;
				return 0;
			}
			m_token = json_object_get_string(json_object_object_get(value, "Token"), &len);
			LOG(INFO) + "Token : " + json_object_get_string(json_object_object_get(value, "Token"), &len);
		}
		else
		{
			if(action != "Deleted")
			{
				delete obj;
				delete value;	
				delete message;
				return 0;
			}
			LOG(INFO) + "Registration status: " + action.c_str();
		}
		delete value;
	}
LOG(INFO) + "token request is received5";
	delete obj;
	delete message;
	return 1;
}

int PushClient::Unregister()
{
	LOG(INFO) + "Unregister Begin";
	if(m_isDisabled)
	{
		LOG(ERROR) + "ans service is not started";	
		return 0;
	}

	if(!isRegistered()) return 1;

#ifdef OS_WINDOWS_DESKTOP
	m_from->StopEventThread();
#endif

	m_from->DataOnQueueChanged = 0;

#ifdef OS_WINDOWS_DESKTOP
	Sleep(500);
#endif

	if(!sendTokenRequest())
		sendTokenRequest();

	if(m_to)
		delete m_to;
	if(m_from)
		delete m_from;

	m_to =0;
	m_from = 0;

	m_isRegistered = false;
	LOG(INFO) + "Unregister End";
	return 0;
}

int PushClient::SetNotificationCallback(const char* callback_uri)
{
	LOG(INFO) + "Set Notification callback";
	m_callbackUri = RHODESAPP().canonicalizeRhoUrl(callback_uri);
	return 0;
}

bool PushClient::isRegistered()
{
	return m_isRegistered;
}

void PushClient::Disable()
{
	m_isDisabled = true;
}

#ifdef OS_WINDOWS_DESKTOP
void PushClient::messageHandler(MSMQ* mq, int status)
#else
void PushClient::messageHandler(P2PMessageQueue* mq, int status)
#endif
{
	LOG(INFO) + "message handler fired";
	if(status == NonEmpty)
	{
		//if(!IsWindowVisible(getMainWnd()))
		//	ShowWindow(getMainWnd(),SW_RESTORE);
		
		Message* msg = new Message();
#ifdef OS_WINDOWS_DESKTOP
	    msg = mq->getMessage();
#else
	    mq->Receive(msg);
#endif

		LOG(INFO) + "message received from queue: " + msg -> MessageBytes();
		int len;
		json_object* value = json_tokener_parse(msg->MessageBytes());
		string data = json_object_get_string(json_object_object_get(value, "Data"), &len);
		findAndReplace<string>(data, "\\\"", "\"");
		rho::push::CPushManager::getInstance() -> callBack(RPS_CLIENT_NAME, data.c_str());
		/*
		string body = "type=message&message=";
		body += data.c_str();
		body += "&rho_callback=1";

		NetResponse resp = getNetRequest().pushData( m_callbackUri, body, NULL );
		delete msg;

		if (!resp.isOK() || strcmp(resp.getCharData(),"rho_push") == 0)
			//	if (!RHODESAPP().callPushCallback("rhoconnect_push","", body))
		{
			value = json_tokener_parse(const_cast<char*>(data.c_str()));
			string message = json_object_get_string(json_object_object_get(value, "alert"));
			int duration = static_cast<int>(json_object_get_int(json_object_object_get(value, "vibrate")));
			string sound = json_object_get_string(json_object_object_get(value, "sound"));
			string syncSources = json_object_get_string(json_object_object_get(value, "do_sync"));

			if (message != "") {
				alert_show_status( "New Message", message.c_str(), 0 );
			}    

			if (sound != "") {
				//TO DO - uncomment and test when play file support will be implemented on CE/WM
				//alert_play_file(strcat("/public/alerts/", sound.c_str()), NULL);
			} 

			if (duration > 0) {
				//TO DO - uncomment and test when vibrate support will be implemented on CE/WM
				//alert_vibrate(duration);
			} 

			//temp fix. should be redone
			if(syncSources.empty() || syncSources == "[ \"\" ]") return;
			rho::json::CJSONArrayIterator syncIt(syncSources.c_str());
			boolean syncAll = false;
			while(!syncIt.isEnd()){
				if (CompareCaseInsensitive(syncIt.getCurItem().getString(), "all")) {
					syncAll = true;
					break;
				} else {
					string arg_source = syncIt.getCurItem().getString();
					if(arg_source.length() > 0)
						rho::sync::RhoconnectClientManager::doSyncSourceByName(arg_source.c_str());
						//rho_sync_doSyncSourceByName(arg_source.c_str());
						//CSyncThread::getSyncEngine().doSyncSource(CSyncEngine::CSourceID(arg_source),"");
				}
				syncIt.next();
			}

			if (syncAll) {
				//CSyncThread::getSyncEngine().doSyncAllSources(""); 
				//rho_sync_doSyncAllSources(0,"", false);
				rho::sync::RhoconnectClientManager::doSyncAllSources(0, "", false);
			}
		}
		*/
	}
}

void PushClient::setAppName(string appname)
{
	m_appName = appname;
}
void PushClient::setUserName(string username)
{
	m_userName = username;
}
void PushClient::setPassword(string password)
{
	m_pass = password;
}
void PushClient::setSession(string session)
{
	m_session = session;
}