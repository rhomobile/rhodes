#include "NetworkAvailability.h"

/**
 * Communicates with the Android layer via JNI to query whether there is any
 * data network.
 * @return -2 if a JNI error occured, -1 if the Android layer couldnt get a network,
 *        0 if there is no network data connection, 1 if there is a network data connection.
 * @author Ben Kennedy (NCVT73)
 */
int CNetworkAvailability::hasNetwork()
{
	LOG(INFO) + "NetworkAvailability hasNetwork+";
	JNIEnv *env = jnienv();

	if(!env)
	{
		RAWLOG_ERROR1("Failed to get java environment for %s", NETWORK_IMPL_CLASS);
		return -2;
	}
	if (env->PushLocalFrame( 3) < 0) 
	{
		RAWLOG_ERROR1("PushLocalFrame fails, out of memory: %s", NETWORK_IMPL_CLASS);
		return -2;
    }	
	jclass cls = rho_find_class(env, NETWORK_IMPL_CLASS);
	if(!cls)
	{
		RAWLOG_ERROR1("Failed to find java class: %s", NETWORK_IMPL_CLASS);
		return -2;
	}

	jmethodID midHasNetwork = env->GetStaticMethodID(cls, "hasNetwork", "()I");
	if(!midHasNetwork)
	{
		RAWLOG_ERROR1("Failed to find java method in %s", NETWORK_IMPL_CLASS);
		return -2;
	}

	env->PopLocalFrame(NULL);
	LOG(INFO) + "NetworkAvailability hasNetwork-";
	return env->CallStaticIntMethod(cls, midHasNetwork);
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
	LOG(INFO) + "NetworkAvailability hasWifiNetwork+";
	JNIEnv *env = jnienv();
	if(!env)
	{
		RAWLOG_ERROR1("Failed to get java environment for %s", NETWORK_IMPL_CLASS);
		return -2;
	}
	
	jclass cls = rho_find_class(env, NETWORK_IMPL_CLASS);
	if(!cls)
	{
		RAWLOG_ERROR1("Failed to find java class: %s", NETWORK_IMPL_CLASS);
		return -2;
	}

	jmethodID midHasWifiNetwork = env->GetStaticMethodID(cls, "hasWifiNetwork", "()I");
	if(!midHasWifiNetwork)
	{
		RAWLOG_ERROR1("Failed to find java method in %s", NETWORK_IMPL_CLASS);
		return -2;
	}

	LOG(INFO) + "NetworkAvailability hasCellNetwork-";
	return env->CallStaticIntMethod(cls, midHasWifiNetwork);
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
	LOG(INFO) + "NetworkAvailability hasCellNetwork+";
	JNIEnv *env = jnienv();
	if(!env)
	{
		RAWLOG_ERROR1("Failed to get java environment for %s", NETWORK_IMPL_CLASS);
		return -2;
	}
	
	jclass cls = rho_find_class(env, NETWORK_IMPL_CLASS);
	if(!cls)
	{
		RAWLOG_ERROR1("Failed to find java class: %s", NETWORK_IMPL_CLASS);
		return -2;
	}

	jmethodID midHasCellNetwork = env->GetStaticMethodID(cls, "hasCellNetwork", "()I");
	if(!midHasCellNetwork)
	{
		RAWLOG_ERROR1("Failed to find java method in %s", NETWORK_IMPL_CLASS);
		return -2;
	}

	LOG(INFO) + "NetworkAvailability hasCellNetwork-";
	return env->CallStaticIntMethod(cls, midHasCellNetwork);
}
