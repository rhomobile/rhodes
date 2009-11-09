#include <assert.h>
#include <stdlib.h>

#include <ruby.h>

#include <common/RhoDefs.h>
#include <common/IRhoClassFactory.h>
//#include <sync/SyncThread.h>
//#include <sync/ClientRegister.h>

namespace rho
{
namespace common
{

IRhoClassFactory* createClassFactory()
{
	// TODO:
	RHO_ABORT("Not implemented");
	return 0;
}

} // namespace common
} // namespace rho

RHO_GLOBAL void rho_map_location(char* query)
{
	// TODO:
	RHO_ABORT("Not implemented");
}

RHO_GLOBAL void rho_appmanager_load( void* httpContext, const char* szQuery)
{
	// TODO:
	RHO_ABORT("Not implemented");
}

RHO_GLOBAL int rho_net_has_network()
{
	// TODO:
	return 1;
}

RHO_GLOBAL void delete_files_in_folder(const char *szFolderPath)
{
	// TODO:
	RHO_ABORT("Not implemented");
}

RHO_GLOBAL VALUE rho_syscall(const char* callname, int nparams, char** param_names, char** param_values)
{
	// TODO:
	RHO_ABORT("Not implemented");
	return 0;
}
