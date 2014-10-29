#include "NetworkAvailability.h"

jclass CNetworkAvailability::s_jNetworkClass = 0;

JNIEnv* CNetworkAvailability::init()
{
    JNIEnv* env = jnienv();

    if(s_jNetworkClass == 0)
    {
        jclass cls = rho_find_class(env, NETWORK_IMPL_CLASS);
        if(!cls)
        {
            RAWLOG_ERROR1("Failed to find java class: %s", NETWORK_IMPL_CLASS);
            return 0;
        }
        s_jNetworkClass = static_cast<jclass>(env->NewGlobalRef(cls));
        env->DeleteLocalRef(cls);
        if(!s_jNetworkClass)
        {
            RAWLOG_ERROR1("Failed to create global reference to java class: %s", NETWORK_IMPL_CLASS);
            return 0;
        }
    }

    return env;
}

/**
 * Communicates with the Android layer via JNI to query whether there is any
 * data network.
 * @return -2 if a JNI error occured, -1 if the Android layer couldnt get a network,
 *        0 if there is no network data connection, 1 if there is a network data connection.
 * @author Ben Kennedy (NCVT73)
 */
int CNetworkAvailability::hasNetwork()
{
	RAWTRACE("NetworkAvailability hasNetwork+");
	JNIEnv *env = init();
    if(!env)
    {
        RAWLOG_ERROR1("Failed to get java environment for %s", NETWORK_IMPL_CLASS);
        return -2;
    }

	jmethodID midHasNetwork = env->GetStaticMethodID(s_jNetworkClass, "hasNetwork", "()I");
	if(!midHasNetwork)
	{
		RAWLOG_ERROR1("Failed to find java method in %s", NETWORK_IMPL_CLASS);
		return -2;
	}

	RAWTRACE("NetworkAvailability hasNetwork-");
	return env->CallStaticIntMethod(s_jNetworkClass, midHasNetwork);
}

/**
 * Communicates with the Android layer via JNI to query whether there is an
 * available WiFi data network.
 * @return -2 if a JNI error occured, -1 if the Android layer couldnt get a network,
 *        0 if there is no WiFi network data connection, 1 if there is a WiFi network data connection.
 * @author Ben Kennedy (NCVT73)
 */
int CNetworkAvailability::hasWifiNetwork()
{
    RAWTRACE("NetworkAvailability hasWifiNetwork+");
	JNIEnv *env = init();
    if(!env)
    {
        RAWLOG_ERROR1("Failed to get java environment for %s", NETWORK_IMPL_CLASS);
        return -2;
    }

	jmethodID midHasWifiNetwork = env->GetStaticMethodID(s_jNetworkClass, "hasWifiNetwork", "()I");
	if(!midHasWifiNetwork)
	{
		RAWLOG_ERROR1("Failed to find java method in %s", NETWORK_IMPL_CLASS);
		return -2;
	}

	RAWTRACE("NetworkAvailability hasCellNetwork-");
	return env->CallStaticIntMethod(s_jNetworkClass, midHasWifiNetwork);
}

/**
 * Communicates with the Android layer via JNI to query whether there is an
 * available cell data network.
 * @return -2 if a JNI error occured, -1 if the Android layer couldnt get a network,
 *        0 if there is no cell network data connection, 1 if there is a cell network data connection.
 * @author Ben Kennedy (NCVT73)
 */
int CNetworkAvailability::hasCellNetwork()
{
    RAWTRACE("NetworkAvailability hasCellNetwork+");
	JNIEnv *env = init();
	if(!env)
	{
		RAWLOG_ERROR1("Failed to get java environment for %s", NETWORK_IMPL_CLASS);
		return -2;
	}

	jmethodID midHasCellNetwork = env->GetStaticMethodID(s_jNetworkClass, "hasCellNetwork", "()I");
	if(!midHasCellNetwork)
	{
		RAWLOG_ERROR1("Failed to find java method in %s", NETWORK_IMPL_CLASS);
		return -2;
	}

	RAWTRACE("NetworkAvailability hasCellNetwork-");
	return env->CallStaticIntMethod(s_jNetworkClass, midHasCellNetwork);
}
