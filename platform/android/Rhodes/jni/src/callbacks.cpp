#include <assert.h>
#include <stdlib.h>
#include <common/RhoDefs.h>
#include <sync/SyncThread.h>
#include <sync/ClientRegister.h>


RHO_GLOBAL void webview_navigate(char* url, int index)
{
	// TODO:
	RHO_ABORT("Not implemented");
}

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

RHO_GLOBAL double geo_latitude()
{
	// TODO:
	RHO_ABORT("Not implemented");
}

RHO_GLOBAL double geo_longitude()
{
	// TODO:
	RHO_ABORT("Not implemented");
}

RHO_GLOBAL int geo_known_position()
{
	// TODO:
	RHO_ABORT("Not implemented");
}

namespace rho{
namespace common{
	IRhoClassFactory* createClassFactory()
	{
		// TODO:
		RHO_ABORT("Not implemented");
		return 0;
	}
}
}

RHO_GLOBAL void rho_sync_create()
{
	// TODO:
	RHO_ABORT("Not implemented");
}

RHO_GLOBAL void rho_sync_destroy()
{
	// TODO:
	RHO_ABORT("Not implemented");
}

// RHO_GLOBAL void rho_clientregister_create(const char* szDevicePin)
// {
// 	// TODO:
// 	RHO_ABORT("Not implemented");
// }

RHO_GLOBAL void rho_sync_doSyncAllSources(int show_status_popup)
{
	// TODO:
	RHO_ABORT("Not implemented");
}

RHO_GLOBAL void delete_files_in_folder(const char *szFolderPath)
{
	// TODO:
	RHO_ABORT("Not implemented");
}
