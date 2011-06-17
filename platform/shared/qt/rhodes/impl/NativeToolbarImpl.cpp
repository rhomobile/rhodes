#include <time.h>
#include "ext/rho/rhoruby.h"
#include "logging/RhoLog.h"
#include "rubyext/NativeToolbarExt.h"

extern "C" {

void remove_native_toolbar() 
{
    //TODO: getAppWindow().performOnUiThread(new CNativeToolbar::CRemoveTask() );
}

void create_native_toolbar(int bar_type, rho_param *p) 
{
    if ( bar_type == NOBAR_TYPE )
        remove_native_toolbar();
    else if ( bar_type == TOOLBAR_TYPE ) {
        //TODO: getAppWindow().performOnUiThread(new CNativeToolbar::CCreateTask(p) );
    } else {
        RAWLOGC_ERROR("NativeBar", "Only Toolbar control is supported.");
    }
}

void create_nativebar(int bar_type, rho_param *p) 
{
    RAWLOGC_INFO("NativeBar", "NativeBar.create() is DEPRECATED. Use Rho::NativeToolbar.create() or Rho::NativeTabbar.create().");
    create_native_toolbar(bar_type, p);
}

void remove_nativebar() 
{
    RAWLOGC_INFO("NativeBar", "NativeBar.remove() is DEPRECATED API ! Please use Rho::NativeToolbar.remove() or Rho::NativeTabbar.remove().");
    remove_native_toolbar();
}

VALUE nativebar_started() 
{
    bool bStarted = false; //TODO: CNativeToolbar::getInstance().isStarted();
    return rho_ruby_create_boolean(bStarted?1:0);
}

//Tabbar
void remove_native_tabbar()
{
    //TODO: getAppWindow().performOnUiThread(new CNativeTabbar::CRemoveTask() );
}

void create_native_tabbar(int bar_type, rho_param *p)
{
    // check for iPad SplitTabBar type -> redirect to TabBar
    if (bar_type == VTABBAR_TYPE) {
        bar_type = TABBAR_TYPE;
    }

    if ( bar_type == NOBAR_TYPE )
        remove_native_tabbar();
    else if ( bar_type == TABBAR_TYPE )
    {
        //TODO: getAppWindow().performOnUiThread(new CNativeTabbar::CCreateTask(bar_type, p) );
    } else
    {
        RAWLOGC_ERROR("NativeTabbar", "Only Tabbar control is supported.");
    }
}

void native_tabbar_switch_tab(int index)
{
    //TODO: getAppWindow().performOnUiThread(new CNativeTabbar::CSwitchTask(index) );
}

void native_tabbar_set_tab_badge(int index,char *val)
{
    //TODO: getAppWindow().performOnUiThread(new CNativeTabbar::CBadgeTask(index, val) );
}

void nativebar_set_tab_badge(int index,char* val)
{
    RAWLOGC_INFO("NativeBar", "NativeBar.set_tab_badge() is DEPRECATED. Use Rho::NativeTabbar.set_tab_badge().");
    native_tabbar_set_tab_badge(index, val);
}

int native_tabbar_get_current_tab() 
{
    //TODO: return getAppWindow().tabbarGetCurrent();
    return 0;
}

void nativebar_switch_tab(int index)
{
    RAWLOGC_INFO("NativeBar", "NativeBar.switch_tab() is DEPRECATED. Use Rho::NativeTabbar.switch_tab().");
    native_tabbar_switch_tab(index);
}

//NavBar - iphone only
void create_navbar(rho_param *p)
{
    //TODO: create_navbar
}

void remove_navbar()
{
    //TODO: remove_navbar
}

VALUE navbar_started()
{
    //TODO: navbar_started
    return rho_ruby_create_boolean(0);
}

} //extern "C"
