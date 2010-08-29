#ifndef _RHOAPPADAPTER_H_
#define _RHOAPPADAPTER_H_

#include "common/RhoStd.h"
#include "net/INetRequest.h"

namespace rho {
struct _CRhoAppAdapter {

static const int ERR_NONE = 0;
static const int ERR_NETWORK = 1;
static const int ERR_REMOTESERVER = 2;
static const int ERR_RUNTIME = 3;
static const int ERR_UNEXPECTEDSERVERRESPONSE = 4;
static const int ERR_DIFFDOMAINSINSYNCSRC = 5;
static const int ERR_NOSERVERRESPONSE = 6;
static const int ERR_CLIENTISNOTLOGGEDIN = 7;
static const int ERR_CUSTOMSYNCSERVER = 8;
static const int ERR_UNATHORIZED = 9;
static const int ERR_CANCELBYUSER = 10;
static const int ERR_SYNCVERSION = 11;
static const int ERR_GEOLOCATION = 12;

static String getMessageText(const char* szName);
static String getErrorText(int nError);
static int    getErrorFromResponse(net::INetResponse& resp);
static void   loadServerSources(const String& strSources);
static String getRhoDBVersion();
	
};
extern const _CRhoAppAdapter& RhoAppAdapter;
}

#endif //_RHOAPPADAPTER_H_
