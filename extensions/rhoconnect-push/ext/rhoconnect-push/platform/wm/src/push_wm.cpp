//#include "stdafx.h"
#include <common/RhoConf.h>
#include <logging/RhoLogConf.h>
#include "AnsSync.h"

#include <stdlib.h>
#include <windows.h>
#include <commctrl.h>

////#include <RhoNativeViewManager.h>
#include "rubyext/WebView.h"

#include "ruby/ext/rho/rhoruby.h"

#include "PushClient.h"

extern "C" void anspush_install_service() {
	//TODO: implement ANS service cab installation
}

extern "C" void anspush_ans_register(const char* username, const char* password, const char* session) 
{
	PushClient::setAppName(RHOCONF().getString("Push.rhoconnect.pushAppName").c_str());
	PushClient::setUserName(username);
	PushClient::setPassword(password);
	PushClient::setSession(session);
	CAnsSync::Create()->sendCommand(Register);
}

extern "C" void anspush_ans_unregister(const char* session) 
{
	PushClient::setSession(session);
	CAnsSync::Create()->sendCommand(Unregister);
}

extern "C" void anspush_check_ans_registration(const char* username, const char* password)
{
}

