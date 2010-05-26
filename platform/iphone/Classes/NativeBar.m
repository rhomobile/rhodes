//
//  NativeBar.m
//  rhorunner
//
//  Created by lars on 8/21/09.
//  Copyright 2009 Home. All rights reserved.
//

#include "common/rhoparams.h"
#import "NativeBar.h"
#import "Rhodes.h"
#import "SimpleMainView.h"
#import "TabbedMainView.h"

#import "logging/RhoLog.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "NativeBar"

@interface RhoNativeBarCreateTask : NSObject {}
+ (void)run:(NSValue*)value :(NSArray*)items;
@end

@implementation RhoNativeBarCreateTask
+ (void)run:(NSValue*)value :(NSArray*)items {
    int type;
    [value getValue:&type];
    
    id view = nil;
   
    Rhodes *r = [Rhodes sharedInstance];
    //UIWindow *window = [[Rhodes sharedInstance] rootWindow];
    //CGRect frame = [Rhodes applicationFrame];
    
    switch (type) {
    case NOBAR_TYPE:
        //view = [[SimpleMainView alloc] initWithParentView:window frame:frame];
        view = [[SimpleMainView alloc] initWithMainView:r.mainView];
        break;
    case TOOLBAR_TYPE:
        //view = [[SimpleMainView alloc] initWithParentView:window frame:frame toolbar:items];
        view = [[SimpleMainView alloc] initWithMainView:r.mainView toolbar:items];
        break;
    case TABBAR_TYPE:
        //view = [[TabbedMainView alloc] initWithParentView:window frame:frame items:items];
        view = [[TabbedMainView alloc] initWithMainView:r.mainView tabs:items];
        break;
    default:
        RAWLOG_ERROR1("Unknown bar type passed: %d", type);
        return;
    }
    
    r.mainView = view;
    [view release];
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
    if (p->type != RHO_PARAM_ARRAY) {
        RAWLOG_ERROR("Unexpected parameter type for create_nativebar, should be Array");
        return;
    }
    
    int size = p->v.array->size;
    NSMutableArray *items = [NSMutableArray arrayWithCapacity:size];
    for (int i = 0; i < size; ++i) {
        rho_param *hash = p->v.array->value[i];
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
    [Rhodes performOnUiThread:runnable arg:arg1 arg:items wait:YES];
}

void remove_nativebar() {
    int bar_type = NOBAR_TYPE;
    id runnable = [RhoNativeBarCreateTask class];
    id arg1 = [NSValue valueWithBytes:&bar_type objCType:@encode(int)];
    [Rhodes performOnUiThread:runnable arg:arg1 arg:nil wait:YES];
}

void nativebar_switch_tab(int index) {
    id runnable = [RhoNativeBarSwitchTabTask class];
    id arg = [NSValue valueWithBytes:&index objCType:@encode(int)];
    [Rhodes performOnUiThread:runnable arg:arg wait:YES];
}
