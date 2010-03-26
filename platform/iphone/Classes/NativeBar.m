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

@interface RhoNativeBarCreateTask : NSObject<RhoRunnable>
{
    int type;
    NSArray *items;
}

@property (nonatomic, assign) int type;
@property (nonatomic, copy) NSArray *items;

- (id)initWithType:(int)type andItems:(NSArray*)items;
- (void)dealloc;
- (void)run;

@end

@implementation RhoNativeBarCreateTask

@synthesize type, items;

- (id)initWithType:(int)t andItems:(NSArray*)its {
    self.type = t;
    self.items = its;
    return self;
}

- (void)dealloc {
    [items release];
    [super dealloc];
}

- (void)run {
    id view = nil;
    
    UIWindow *window = [[Rhodes sharedInstance] rootWindow];
    id delegate = [Rhodes sharedInstance];
    
    switch (type) {
    case NOBAR_TYPE:
        view = [[SimpleMainView alloc] initWithParentWindow:window andDelegate:delegate];
        break;
    case TOOLBAR_TYPE:
        view = [[SimpleMainView alloc] initWithParentWindow:window toolbar:items andDelegate:delegate];
        break;
    case TABBAR_TYPE:
        view = [[TabbedMainView alloc] initWithParentWindow:window items:items andDelegate:delegate];
        break;
    default:
        RAWLOG_ERROR1("Unknown bar type passed: %d", type);
    }
    
    if (view) {
        [Rhodes sharedInstance].mainView = view;
        [view release];
    }
}

@end

@interface RhoNativeBarSwitchTabTask : NSObject<RhoRunnable>
{
    int index;
}

- (id)initWithIndex:(int)idx;
- (void)run;

@end

@implementation RhoNativeBarSwitchTabTask

- (id)initWithIndex:(int)idx {
    index = idx;
    return self;
}

- (void)run {
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
    
    id task = [[[RhoNativeBarCreateTask alloc] initWithType:bar_type andItems:items] autorelease];
    [Rhodes performOnUiThread:task wait:NO];
}

void remove_nativebar() {
    id task = [[[RhoNativeBarCreateTask alloc] initWithType:NOBAR_TYPE andItems:nil] autorelease];
    [Rhodes performOnUiThread:task wait:NO];
}

void nativebar_switch_tab(int index) {
    id task = [[[RhoNativeBarSwitchTabTask alloc] initWithIndex:index] autorelease];
    [Rhodes performOnUiThread:task wait:NO];
}
