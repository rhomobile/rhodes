#pragma once

#include <jni.h>
#include "rhodes.h"
#include "logging/RhoLog.h"
#include "common/StringConverter.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "NetworkAvailability"
#define NETWORK_IMPL_CLASS "com.motorolasolutions.rho.network.Network"

class CNetworkAvailability
{
    static jclass s_jNetworkClass;
    static JNIEnv* init();
public:
	static int hasNetwork();
	static int hasCellNetwork();
	static int hasWifiNetwork();
};
