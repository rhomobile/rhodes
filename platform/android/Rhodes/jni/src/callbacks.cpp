#include "JNIRhodes.h"

#include "RhoClassFactory.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Callbacks"


namespace rho
{
namespace common
{

IRhoClassFactory* createClassFactory()
{
    return new CRhoClassFactory();
}

} // namespace common
} // namespace rho

RHO_GLOBAL void rho_map_location(char* query)
{
    // TODO:
}

RHO_GLOBAL void rho_appmanager_load( void* httpContext, const char* szQuery)
{
    // TODO:
}

RHO_GLOBAL int rho_net_has_network()
{
    // TODO:
    return 1;
}

RHO_GLOBAL void delete_files_in_folder(const char *szFolderPath)
{
    // TODO:
}

RHO_GLOBAL VALUE rho_syscall(const char* callname, int nparams, char** param_names, char** param_values)
{
    // TODO:
    return 0;
}

RHO_GLOBAL void rho_net_impl_network_indicator(int enable)
{
    // TODO
}

RHO_GLOBAL void *rho_nativethread_start()
{
    JNIEnv *env;
    jvm()->AttachCurrentThread(&env, NULL);
    store_thr_jnienv(env);
    return NULL;
}

RHO_GLOBAL void rho_nativethread_end(void *)
{
    jvm()->DetachCurrentThread();
}
