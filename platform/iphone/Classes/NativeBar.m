//
//  NativeBar.m
//  rhorunner
//
//  Created by lars on 8/21/09.
//  Copyright 2009 Home. All rights reserved.
//

#import "NativeBar.h"
#import "Rhodes.h"
#import "SimpleMainView.h"
#import "TabbedMainView.h"

//#include "common/rhoparams.h"
#include "logging/RhoLog.h"
#include "ruby/ext/rho/rhoruby.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "NativeBar"

static int started = 0;

@interface RhoNativeBarCreateTask : NSObject {}
+ (void)run:(NSValue*)value :(NSArray*)items;
@end

@implementation RhoNativeBarCreateTask
+ (void)run:(NSValue*)value :(NSArray*)items {
    int type;
    [value getValue:&type];
    
    id view = nil;
   
    Rhodes *r = [Rhodes sharedInstance];
    
    id mainView = [r mainView];
    UIWindow* w = r.window;

	SimpleMainView* smv = nil;
    if ([mainView isKindOfClass:[SimpleMainView class]]) {
	    smv = (SimpleMainView*)mainView;
	}
    switch (type) {
		case NOBAR_TYPE:
			if (smv != nil) {
				[smv removeToolbar];
			}
			else {
				view = [[SimpleMainView alloc] initWithMainView:mainView parent:w ];
				[r setMainView:view];
				[view release];
			}
			started = 0;
			break;
		case TOOLBAR_TYPE:
			if (smv != nil) {
				[smv addToolbar:items];
			}
			else {
				view = [[SimpleMainView alloc] initWithMainView:mainView parent:w toolbar:items];
				[r setMainView:view];
				[view release];
			}
			started = 1;
			break;
		case TABBAR_TYPE:
			view = [[TabbedMainView alloc] initWithMainView:mainView parent:w tabs:items];
			started = 1;
			[r setMainView:view];
			[view release];
			break;
		default:
			RAWLOG_ERROR1("Unknown bar type passed: %d", type);
        return;
    }
 
}
@end

@interface RhoNativeBarSwitchTabTask : NSObject {}
+ (void)run:(NSValue*)value;
@end

@implementation RhoNativeBarSwitchTabTask
+ (void)run:(NSValue*)value {
    int index;
    [value getValue:&index];
    [[[Rhodes sharedInstance] mainView] switchTab:index];
}
@end

void create_nativebar(int bar_type, rho_param *p)
{
    if (!rho_rhodesapp_check_mode())
        return;
    rho_param *params = NULL;
    switch (p->type) {
        case RHO_PARAM_ARRAY:
            params = p;
            break;
        case RHO_PARAM_HASH: {
            for (int i = 0, lim = p->v.hash->size; i < lim; ++i) {
                const char *name = p->v.hash->name[i];
                rho_param *value = p->v.hash->value[i];
                
                // TODO: implement color settings
                if (strcasecmp(name, "buttons") == 0 || strcasecmp(name, "tabs") == 0) {
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
    NSMutableArray *items = [NSMutableArray arrayWithCapacity:size];
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
        
        for (int j = 0, lim = hash->v.hash->size; j < lim; ++j) {
            const char *name = hash->v.hash->name[j];
            rho_param *value = hash->v.hash->value[j];
            if (value->type != RHO_PARAM_STRING) {
                RAWLOG_ERROR1("Unexpected '%s' type, should be String", name);
                return;
            }
            
            if (strcasecmp(name, "label") == 0)
                label = value->v.string;
            else if (strcasecmp(name, "action") == 0)
                action = value->v.string;
            else if (strcasecmp(name, "icon") == 0)
                icon = value->v.string;
            else if (strcasecmp(name, "reload") == 0)
                reload = value->v.string;
        }
        
        if (label == NULL && bar_type == TOOLBAR_TYPE)
            label = "";
        
        if (label == NULL || action == NULL) {
            RAWLOG_ERROR("Illegal argument for create_nativebar");
            return;
        }
        
        [items addObject:[NSString stringWithUTF8String:label]];
        [items addObject:[NSString stringWithUTF8String:action]];
        [items addObject:[NSString stringWithUTF8String:(icon ? icon : "")]];
        [items addObject:[NSString stringWithUTF8String:(reload ? reload : "false")]];
    }
    
    id runnable = [RhoNativeBarCreateTask class];
    id arg1 = [NSValue valueWithBytes:&bar_type objCType:@encode(int)];
    [Rhodes performOnUiThread:runnable arg:arg1 arg:items wait:NO];
}

void remove_nativebar() {
    if (!rho_rhodesapp_check_mode())
        return;
    int bar_type = NOBAR_TYPE;
    id runnable = [RhoNativeBarCreateTask class];
    id arg1 = [NSValue valueWithBytes:&bar_type objCType:@encode(int)];
    [Rhodes performOnUiThread:runnable arg:arg1 arg:nil wait:NO];
}

void nativebar_switch_tab(int index) {
    if (!rho_rhodesapp_check_mode())
        return;
    id runnable = [RhoNativeBarSwitchTabTask class];
    id arg = [NSValue valueWithBytes:&index objCType:@encode(int)];
    [Rhodes performOnUiThread:runnable arg:arg wait:NO];
}

VALUE nativebar_started() {
    return rho_ruby_create_boolean(started);
}
