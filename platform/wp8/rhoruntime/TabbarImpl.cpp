#include "rhoruntime.h"
#include "common/RhoStd.h"
#include "common/RhodesApp.h"
#include "common/rhoparams.h"
#include "common/RhoFilePath.h"
#include "logging/RhoLog.h"
#include "rubyext/NativeToolbarExt.h"

using namespace rhoruntime;
using namespace Platform;

bool charToBool(const char* str)
{
    return str && ((strcasecmp(str,"true")==0) || (strcasecmp(str,"yes")==0) || (atoi(str)==1));
}

extern "C" {

void remove_native_tabbar()
{
	CRhoRuntime::getInstance()->getMainPage()->tabbarHide();
}

void create_native_tabbar(int bar_type, rho_param *p)
{
    if (!rho_rhodesapp_check_mode())
        return;

	// check for iPad SplitTabBar type -> redirect to TabBar
    if (bar_type == VTABBAR_TYPE) {
        bar_type = TABBAR_TYPE;
    }

    if ( bar_type == NOBAR_TYPE ) {
        remove_native_tabbar();
		return;
	}
    else if ( bar_type != TABBAR_TYPE )
    {
        RAWLOGC_ERROR("NativeTabbar", "Only Tabbar control is supported.");
		return;
    }

    const char* on_change_tab_callback = NULL;
    const char* background_color = NULL;
    
    rho_param *params = NULL;
    switch (p->type)
	{
        case RHO_PARAM_ARRAY:
            params = p;
            break;
        case RHO_PARAM_HASH:
			{
                for (int i = 0, lim = p->v.hash->size; i < lim; ++i)
			    {
                    const char *name = p->v.hash->name[i];
                    rho_param *value = p->v.hash->value[i];
                    if (strcasecmp(name, "background_color") == 0) {
                        background_color = value->v.string;
                    } else if (strcasecmp(name, "on_change_tab_callback") == 0) {
                        on_change_tab_callback = value->v.string;
                    } else if (strcasecmp(name, "buttons") == 0 || strcasecmp(name, "tabs") == 0) {
                        params = value;
                    }
                }
            }
            break;
        default: {
            RAWLOG_ERROR("Unexpected parameter type for create_nativebar, should be Array or Hash");
            return;
        }
    }
    
    if (!params) {
        RAWLOG_ERROR("Wrong parameters for create_tabbar");
        return;
    }
    
    CRhoRuntime::getInstance()->getMainPage()->tabbarInitialize();

    int size = params->v.array->size;

    for (int i = 0; i < size; ++i) {
        rho_param *hash = params->v.array->value[i];
        if (hash->type != RHO_PARAM_HASH) {
            RAWLOG_ERROR("Unexpected type of array item for create_nativebar, should be Hash");
            return;
        }
        
        const char *label = NULL;
        const char *action = NULL;
        const char *icon = NULL;
        const char *reload = NULL;
        const char *colored_icon = NULL;
    	const char* selected_color = NULL;
        const char *disabled = NULL;
		const char* web_bkg_color = NULL;
        const char* use_current_view_for_tab = NULL;
        
        bool skip_item = false;
        for (int j = 0, lim = hash->v.hash->size; j < lim; ++j) {
            const char *name = hash->v.hash->name[j];
            rho_param *value = hash->v.hash->value[j];
            if (value->type != RHO_PARAM_STRING) {
                RAWLOGC_ERROR("Unexpected '%s' type, should be String", name);
                return;
            }
            if (strcasecmp(name, "background_color") == 0) {
                background_color = value->v.string;
                skip_item = true;
            }
            
            if (strcasecmp(name, "label") == 0)
                label = value->v.string;
            else if (strcasecmp(name, "action") == 0)
                action = value->v.string;
            else if (strcasecmp(name, "icon") == 0)
                icon = value->v.string;
            else if (strcasecmp(name, "reload") == 0)
                reload = value->v.string;
            else if (strcasecmp(name, "colored_icon") == 0)
                colored_icon = value->v.string;
            else if (strcasecmp(name, "selected_color") == 0){
                selected_color = value->v.string;
            }    
            else if (strcasecmp(name, "disabled") == 0)
                disabled = value->v.string;
            else if (strcasecmp(name, "web_bkg_color") == 0)
                web_bkg_color = value->v.string;
            else if (strcasecmp(name, "use_current_view_for_tab") == 0) {
                use_current_view_for_tab = value->v.string;
                if (strcasecmp(use_current_view_for_tab, "true") == 0) {
                    action = "none";
                }
            }
        }
        
        if (label == NULL && bar_type == TOOLBAR_TYPE)
            label = "";
        
        if ((label == NULL || (action == NULL)) && (!skip_item)) {
            RAWLOG_ERROR("Illegal argument for create_nativebar");
            return;
        }
        if (!skip_item) {
			rho::StringW labelW;
			rho::common::convertToStringW(label, labelW);
			rho::StringW actionW;
			rho::common::convertToStringW(action, actionW);
			rho::StringW bgcolor = background_color ? rho::common::convertToStringW(background_color) : rho::StringW();
			rho::StringW selcolor = selected_color ? rho::common::convertToStringW(selected_color) : rho::StringW();
			rho::StringW callback = on_change_tab_callback ? rho::common::convertToStringW(on_change_tab_callback) : rho::StringW();
			rho::String strIconPath = icon ? rho::common::CFilePath::join( RHODESAPP().getAppRootPath(), icon) : rho::String();
			rho::StringW iconW = rho::common::convertToStringW(strIconPath);
			CRhoRuntime::getInstance()->getMainPage()->tabbarAddTab(
				ref new String(labelW.c_str()), ref new String(iconW.c_str()), ref new String(actionW.c_str()),
				charToBool(disabled), ref new String(bgcolor.c_str()), ref new String(selcolor.c_str()),
				charToBool(reload), charToBool(use_current_view_for_tab));
        }
    }

    CRhoRuntime::getInstance()->getMainPage()->tabbarShow();
}

void native_tabbar_switch_tab(int index)
{
	CRhoRuntime::getInstance()->getMainPage()->tabbarSwitch(index);
}

void native_tabbar_set_tab_badge(int index, char *val)
{
	rho::StringW valW;
	rho::common::convertToStringW(val, valW);
	CRhoRuntime::getInstance()->getMainPage()->tabbarSetBadge(index, ref new String(valW.c_str()));
}

void nativebar_set_tab_badge(int index, char* val)
{
    RAWLOGC_INFO("NativeBar", "NativeBar.set_tab_badge() is DEPRECATED. Use Rho::NativeTabbar.set_tab_badge().");
    native_tabbar_set_tab_badge(index, val);
}

int native_tabbar_get_current_tab()
{
	return 	CRhoRuntime::getInstance()->getMainPage()->tabbarGetCurrent();
}

void nativebar_switch_tab(int index)
{
    RAWLOGC_INFO("NativeBar", "NativeBar.switch_tab() is DEPRECATED. Use Rho::NativeTabbar.switch_tab().");
    native_tabbar_switch_tab(index);
}

} //extern "C"
