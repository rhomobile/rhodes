#include "stdafx.h"

#include "MainWindow.h"
#include "NativeToolbarExt.h"

extern CMainWindow& getAppWindow();

extern "C"
{
int rho_wmsys_has_touchscreen();
void remove_native_toolbar();
void create_native_toolbar(int bar_type, rho_param *p) 
{
    if ( bar_type == NOBAR_TYPE )
        remove_native_toolbar();
    else if ( bar_type == TOOLBAR_TYPE )
    {
        getAppWindow().performOnUiThread(new CNativeToolbar::CCreateTask(p) );
    }else
    {
    	RAWLOGC_ERROR("NativeBar", "Only Toolbar control is supported.");
    }
}

void create_nativebar(int bar_type, rho_param *p) 
{
	RAWLOGC_INFO("NativeBar", "NativeBar.create() is DEPRECATED. Use Rho::NativeToolbar.create() or Rho::NativeTabbar.create().");
    create_native_toolbar(bar_type, p);
}

void remove_native_toolbar() 
{
    getAppWindow().performOnUiThread(new CNativeToolbar::CRemoveTask() );
}

void remove_nativebar() 
{
	RAWLOGC_INFO("NativeBar", "NativeBar.remove() is DEPRECATED API ! Please use Rho::NativeToolbar.remove() or Rho::NativeTabbar.remove().");
	remove_native_toolbar();
}

VALUE nativebar_started() 
{
    bool bStarted = CNativeToolbar::getInstance().isStarted();
    return rho_ruby_create_boolean(bStarted?1:0);
}

//Tabbar
void create_native_tabbar(int bar_type, rho_param *p)
{
#ifdef RHODES_EMULATOR
    if (!rho_rhodesapp_check_mode() || !rho_wmsys_has_touchscreen() )
        return;

    // check for iPad SplitTabBar type -> redirect to TabBar
    if (bar_type == VTABBAR_TYPE) {
        bar_type = TABBAR_TYPE;
    }

    const char* background_color = NULL;
    const char* background_color_enable = NULL;
    const char* on_change_tab_callback = NULL;
    
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
                        background_color_enable = "true";
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
        RAWLOG_ERROR("Wrong parameters for create_nativebar");
        return;
    }
    
    int size = params->v.array->size;

    //NSMutableDictionary* main_properties = [NSMutableDictionary dictionaryWithCapacity:2];
    //NSMutableDictionary* properties = [NSMutableDictionary dictionaryWithCapacity:1];
    //[main_properties setObject:properties forKey:NATIVE_BAR_PROPERTIES];
    //NSMutableArray* items = [NSMutableArray arrayWithCapacity:size];
    //[main_properties setObject:items forKey:NATIVE_BAR_ITEMS];
    
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
        
        const char *selected_color = NULL;
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
            //NSMutableDictionary* item = [NSMutableDictionary dictionaryWithCapacity:10];    
            //[item setObject:[NSString stringWithUTF8String:label] forKey:NATIVE_BAR_ITEM_LABEL];
            //[item setObject:[NSString stringWithUTF8String:action] forKey:NATIVE_BAR_ITEM_ACTION];
            //[item setObject:[NSString stringWithUTF8String:(icon ? icon : "")] forKey:NATIVE_BAR_ITEM_ICON];
            //[item setObject:[NSString stringWithUTF8String:(reload ? reload : "false")] forKey:NATIVE_BAR_ITEM_RELOAD];
            //[item setObject:[NSString stringWithUTF8String:(colored_icon ? colored_icon : "false")] forKey:NATIVE_BAR_ITEM_COLORED_ICON];
            if (selected_color != NULL) {
                //[item setObject:[NSString stringWithUTF8String:selected_color] forKey:NATIVE_BAR_ITEM_SELECTED_COLOR];
            }
            //[item setObject:[NSString stringWithUTF8String:(disabled ? disabled : "false")] forKey:NATIVE_BAR_ITEM_DISABLED];
            if (web_bkg_color != NULL) {
                //[item setObject:[NSString stringWithUTF8String:web_bkg_color] forKey:NATIVE_BAR_ITEM_WEB_BACKGROUND_COLOR];
            }
            //[item setObject:[NSString stringWithUTF8String:(use_current_view_for_tab ? use_current_view_for_tab : "false")] forKey:NATIVE_BAR_ITEM_USE_CURRENT_VIEW_FOR_TAB];
            //[items addObject:item];
        }
        
    }
    if (background_color != NULL) {
        //[properties setObject:[NSString stringWithUTF8String:background_color] forKey:NATIVE_BAR_BACKGOUND_COLOR];    
    }
    if (on_change_tab_callback != NULL) {
        //[properties setObject:[NSString stringWithUTF8String:on_change_tab_callback] forKey:NATIVE_BAR_ON_CHANGE_TAB_CALLBACK];    
    }
    //id runnable = [RhoNativeBarCreateTask class];
    //id arg1 = [NSValue valueWithBytes:&bar_type objCType:@encode(int)];
    //[Rhodes performOnUiThread:runnable arg:arg1 arg:main_properties wait:NO];
#endif
}

void remove_native_tabbar()
{
#ifdef RHODES_EMULATOR
    if (!rho_rhodesapp_check_mode())
        return;
    int bar_type = NOBAR_TYPE;
    //id runnable = [RhoNativeBarCreateTask class];
    //id arg1 = [NSValue valueWithBytes:&bar_type objCType:@encode(int)];
    //[Rhodes performOnUiThread:runnable arg:arg1 arg:nil wait:NO];
#endif
}

void native_tabbar_switch_tab(int index)
{
#ifdef RHODES_EMULATOR
    if (!rho_rhodesapp_check_mode())
        return;
    //id runnable = [RhoNativeBarSwitchTabTask class];
    //id arg = [NSValue valueWithBytes:&index objCType:@encode(int)];
    //[Rhodes performOnUiThread:runnable arg:arg wait:NO];
#endif
}

void native_tabbar_set_tab_badge(int index,char *val)
{
#ifdef RHODES_EMULATOR
    //RAWLOG_INFO2("set_tab_badge called: %d : %s",index,val);
    //id runnable = [RhoNativeBarSetTabBadgeTask class];
    //id arg1 = [NSValue valueWithBytes:&index objCType:@encode(int)];
    //id arg2 = [NSValue valueWithBytes:&val objCType:@encode(char*)];
    //[Rhodes performOnUiThread:runnable arg:arg1 arg:arg2 wait:YES];
    //RAWLOG_INFO("set_tab_badge done");
#endif
}

void nativebar_set_tab_badge(int index,char* val)
{
}

int native_tabbar_get_current_tab() 
{
#ifdef RHODES_EMULATOR
    //Rhodes *r = [Rhodes sharedInstance];
    //return [[r mainView] activeTab];
	return 0;
#else
	return 0;
#endif
}

void nativebar_switch_tab(int index) {
	//TODO: Implement me!
}

//NavBar - iphone only
void create_navbar(rho_param *p)
{
}

void remove_navbar()
{
}

VALUE navbar_started()
{
    return rho_ruby_create_boolean(0);
}

}