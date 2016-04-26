#include <string>
#include <algorithm>
#ifdef OS_WINDOWS_DESKTOP
#include "MSMQ.h"
#else
#include "P2PMessageQueue.h"
#endif
using namespace std;
using namespace RhoIPC;

std::wstring StringToWString(const std::string& s);
std::string WStringToString(const std::wstring& s);

class PushClient
{
public:
	static int Register();
    static int Unregister();
	static int SetNotificationCallback(const char* callback_uri);
	static bool isRegistered();
	static void Disable();
	static int sendTokenRequest();
	static void setAppName(string appname);
	static void setUserName(string username);
	static void setPassword(string password);
	static void setSession(string session);

private:
	static string m_appName;
	static string m_userName;
	static string m_session;
	static string m_pass;
	static string m_token;
	static bool m_isRegistered;
	static bool m_isDisabled;
#ifdef OS_WINDOWS_DESKTOP
    static MSMQ* m_to;
	static MSMQ* m_from;
    static void messageHandler(MSMQ* mq, int status);
#else
	static P2PMessageQueue* m_to;
	static P2PMessageQueue* m_from;
    static void messageHandler(P2PMessageQueue* mq, int status);
#endif
	static string m_callbackUri;
};