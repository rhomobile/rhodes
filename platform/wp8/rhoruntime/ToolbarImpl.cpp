#include "rhoruntime.h"
#include "common/RhoStd.h"
#include "common/RhoConf.h"
#include "common/RhodesApp.h"
#include "common/rhoparams.h"
#include "common/RhoFilePath.h"
#include "logging/RhoLog.h"
#include "rubyext/NativeToolbarExt.h"
#include "common/RhoConvertWP8.h"

using namespace rhoruntime;
using namespace Platform;

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "NativeToolbar"

extern "C" {

void remove_native_toolbar() 
{
    CRhoRuntime::getInstance()->getMainPage()->toolbarRemoveAllButtons();
}

void create_native_toolbar(int bar_type, rho_param *p) 
{
    if ( bar_type == NOBAR_TYPE ) {
        remove_native_toolbar();
		return;
	} else if ( bar_type != TOOLBAR_TYPE ) {
        RAWLOGC_ERROR("NativeBar", "Only Toolbar control is supported.");
		return;
    }

    if (!rho_rhodesapp_check_mode())
        return;

    bar_type = TOOLBAR_TYPE;
	const char *rgbBackColor = NULL;
    const char *rgbMaskColor = NULL;

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
                    
                    if (strcasecmp(name, "background_color") == 0) 
                        rgbBackColor = value->v.string;
                    else if (strcasecmp(name, "mask_color") == 0) 
                        rgbMaskColor = value->v.string;
                    else if (strcasecmp(name, "buttons") == 0 || strcasecmp(name, "tabs") == 0) 
                        params = value;
                }
            }
            break;
        default: {
            LOG(ERROR) + "Unexpected parameter type for create_nativebar, should be Array or Hash";
            return;
        }
    }
    
    if (!params) {
        LOG(ERROR) + "Wrong parameters for create_nativebar";
        return;
    }

    CRhoRuntime::getInstance()->getMainPage()->toolbarRemoveAllButtons();

	int size = params->v.array->size;
    if ( size == 0 )
        return;

    for (int i = 0; i < size; ++i) 
    {
        rho_param *hash = params->v.array->value[i];
        if (hash->type != RHO_PARAM_HASH) {
            LOG(ERROR) + "Unexpected type of array item for create_nativebar, should be Hash";
            return;
        }
            
        const char *label = NULL;
        const char *action = NULL;
        const char *icon = NULL;
        const char *colored_icon = NULL;

        for (int j = 0, lim = hash->v.hash->size; j < lim; ++j) 
        {
            const char *name = hash->v.hash->name[j];
            rho_param *value = hash->v.hash->value[j];
            if (value->type != RHO_PARAM_STRING) {
                LOG(ERROR) + "Unexpected '" + name + "' type, should be String";
                return;
            }
                
            if (strcasecmp(name, "label") == 0)
                label = value->v.string;
            else if (strcasecmp(name, "action") == 0)
                action = value->v.string;
            else if (strcasecmp(name, "icon") == 0)
                icon = value->v.string;
            else if (strcasecmp(name, "colored_icon") == 0)
                colored_icon = value->v.string;
        }
            
        if (label == NULL && bar_type == TOOLBAR_TYPE)
            label = "";
            
        if ( label == NULL || action == NULL) {
            LOG(ERROR) + "Illegal argument for create_nativebar";
            return;
        }
        if ( strcasecmp(action, "forward") == 0 && rho_conf_getBool("jqtouch_mode") )
            continue;

        if (!action) action = "";

        LOG(INFO) + "addToolbarButton: Label: '"+label+"';Action: '"+action+"'";
        if (strcasecmp(action, "separator") != 0) {
            rho::String strImagePath;
            if ( icon && *icon )
                strImagePath = rho::common::CFilePath::join( RHODESAPP().getRhoRootPath(), icon );
            else {
                if ( strcasecmp(action, "options")==0 )
                    strImagePath = "res/options_btn.png";
                else if ( strcasecmp(action, "home")==0 )
                    strImagePath = "res/home_btn.png";
                else if ( strcasecmp(action, "refresh")==0 )
                    strImagePath = "res/refresh_btn.png";
                else if ( strcasecmp(action, "back")==0 )
                    strImagePath = "res/back_btn.png";
                else if ( strcasecmp(action, "forward")==0 )
                    strImagePath = "res/forward_btn.png";
                strImagePath = strImagePath.length() > 0 ? rho::common::CFilePath::join( rho_native_reruntimepath(), "lib/" + strImagePath) : rho::String();
            }
			//TODO: toolbarAddAction
			CRhoRuntime::getInstance()->getMainPage()->toolbarAddAction(rho::common::convertStringToWP8(strImagePath), rho::common::convertStringToWP8(label), rho::common::convertStringToWP8(action));
		}
    }
	//TODO: setToolbarStyle
	//TODO: toolbarShow
	CRhoRuntime::getInstance()->getMainPage()->setToolbarStyle(false, rho::common::convertStringToWP8(rgbBackColor));
    CRhoRuntime::getInstance()->getMainPage()->toolbarShow();
    //m_started = true;
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
    return rho_ruby_create_boolean(CRhoRuntime::getInstance()->getMainPage()->toolbarIsStarted() > 0 ? 1 : 0);
}

} //extern "C"
