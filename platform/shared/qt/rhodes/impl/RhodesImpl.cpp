extern "C" {

const char* rho_native_rhopath() 
{
    //TODO: return _AtlModule.getRhoRootPath().c_str();
}

void rho_conf_show_log()
{
    //TODO: ::PostMessage(getMainWnd(),WM_COMMAND,IDM_LOG,0);
}

//Hook for ruby call to refresh web view

void rho_net_impl_network_indicator(int active)
{
    //TODO: rho_net_impl_network_indicator
}

void rho_map_location(char* query)
{
    //TODO: rho_map_location
}

void rho_appmanager_load( void* httpContext, char* szQuery)
{
    //TODO: rho_appmanager_load
}

int rho_net_ping_network(const char* szHost)
{
    //TODO: rho_net_ping_network
    return 1;
}

} //extern "C"
