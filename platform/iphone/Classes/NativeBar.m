/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/

#import "NativeBar.h"
#import "Rhodes.h"
#import "SimpleMainView.h"
#import "TabbedMainView.h"
#import "SplittedMainView.h"

//#include "common/rhoparams.h"
#include "logging/RhoLog.h"
#include "ruby/ext/rho/rhoruby.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "NativeBar"

static int started = 0;

@interface RhoNativeBarCreateTask : NSObject {}
+ (void)run:(NSValue*)value :(NSDictionary*)parameters;
@end

@implementation RhoNativeBarCreateTask
+ (void)run:(NSValue*)value :(NSDictionary*)parameters {
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
				[smv addToolbar:parameters];
			}
			else {
				view = [[SimpleMainView alloc] initWithMainView:mainView parent:w bar_info:parameters];
				[r setMainView:view];
				[view release];
			}
			started = 1;
			break;
		case TABBAR_TYPE: {
			[[Rhodes sharedInstance] hideSplash];
			view = [[TabbedMainView alloc] initWithMainView:mainView parent:w bar_info:parameters];
			started = 1;
			[r setMainView:view];
			[view release];
		}
			break;
		case VTABBAR_TYPE: {
			[[Rhodes sharedInstance] hideSplash];
			BOOL is_iPad = NO;
			{
				NSString *model = [[UIDevice currentDevice] model]; // "iPad ..."
				if ([model hasPrefix:@"iPad"]) {
					is_iPad = YES;
				}
			}
			if (is_iPad) {
				view = [[SplittedMainView alloc] initWithMainView:mainView parent:w bar_info:parameters];
			}
			else {
				view = [[TabbedMainView alloc] initWithMainView:mainView parent:w bar_info:parameters];
			}
			started = 1;
			[r setMainView:view];
			[view release];
			}
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

#define BADGE_TEXT @"badge_text"
#define TAB_INDEX @"tab_index"

@interface RhoNativeTabBarSetBadgeTask : NSObject {}
+ (void)run:(NSDictionary*)params;
@end

@implementation RhoNativeTabBarSetBadgeTask
+ (void)run:(NSDictionary*)params {
    NSValue* ns_tab_index  = [params objectForKey:TAB_INDEX];
    int tab_index = 0;
    [ns_tab_index getValue:&tab_index];
    NSString* badge_text = (NSString*)[params objectForKey:BADGE_TEXT];

    [[[Rhodes sharedInstance] mainView] setTabBarBadge:badge_text tab_index:tab_index];
}
@end


void rho_create_nativebar_inner(int bar_type, NSArray* p_items, NSDictionary* p_properties, id<IMethodResult>callback)
{
    if (!rho_rhodesapp_check_mode())
        return;

    int bar_t = bar_type;
    
	const char* background_color = NULL;
	const char* background_color_enable = NULL;
    
    const char* on_change_tab_callback = NULL;

	const char* ios_7_icon_color = NULL;
	const char* ios_7_selected_color = NULL;

	
    
    NSObject* obj = nil;
    
    if ([p_properties isKindOfClass:[NSDictionary class]]) {
        obj = [p_properties objectForKey:NATIVE_BAR_ITEM_WEB_BACKGROUND_COLOR];
        if (obj != nil) {
            NSNumber* obj_num = (NSNumber*)obj;
            background_color = [[NSString stringWithFormat:@"%@", obj_num] UTF8String];
            background_color_enable = "true";
        }
        obj = [p_properties objectForKey:@"verticalOrientation"];
        if (obj != nil) {
            NSNumber* obj_num = (NSNumber*)obj;
            if ([obj_num boolValue]) {
                bar_t = VTABBAR_TYPE;
            }
        }
        obj = [p_properties objectForKey:NATIVE_BAR_ICON_COLOR];
        if (obj != nil) {
            NSNumber* obj_num = (NSNumber*)obj;
            ios_7_icon_color = [[NSString stringWithFormat:@"%@", obj_num] UTF8String];
        }
        obj = [p_properties objectForKey:NATIVE_BAR_SELECTED_COLOR];
        if (obj != nil) {
            NSNumber* obj_num = (NSNumber*)obj;
            ios_7_selected_color = [[NSString stringWithFormat:@"%@", obj_num] UTF8String];
        }
    }
    

    
    int size = [p_items count];//params->v.array->size;

	NSMutableDictionary* main_properties = [NSMutableDictionary dictionaryWithCapacity:2];
	
	NSMutableDictionary* properties = [NSMutableDictionary dictionaryWithCapacity:1];
	[main_properties setObject:properties forKey:NATIVE_BAR_PROPERTIES];
	
	NSMutableArray* items = [NSMutableArray arrayWithCapacity:size];
	[main_properties setObject:items forKey:NATIVE_BAR_ITEMS];
	
    for (int i = 0; i < size; ++i) {
        NSObject* obj = [p_items objectAtIndex:i];
        NSDictionary* hash = nil;
        if ([obj isKindOfClass:[NSDictionary class]]) {
            hash = (NSDictionary*)obj;
        }
        else {
            RAWLOG_ERROR("Unexpected type of array item for create_nativebar, should be Hash");
            return;
        }
        
        const char *label = NULL;
        const char *action = NULL;
        const char *icon = NULL;
        const char *reload = NULL;
        const char *colored_icon = NULL;
		
		const char *selected_color = NULL;
		//const char *selected_color_enable = NULL;
		const char *disabled = NULL;
		const char* web_bkg_color = NULL;
		const char* use_current_view_for_tab = NULL;
		
        BOOL skip_item = NO;
        
        //obj = [hash objectForKey:@"backgroundColor"];
        //if (obj != nil) {
        //    NSNumber* obj_num = (NSNumber*)obj;
        //    background_color = [[NSString stringWithFormat:@"%@", obj_num] UTF8String];
        //    skip_item = YES;
        //}
        obj = [hash objectForKey:@"label"];
        if (obj != nil) {
            NSString* obj_str = (NSString*)obj;
            label = [obj_str UTF8String];
        }
        obj = [hash objectForKey:@"action"];
        if (obj != nil) {
            NSString* obj_str = (NSString*)obj;
            action = [obj_str UTF8String];
        }
        obj = [hash objectForKey:@"icon"];
        if (obj != nil) {
            NSString* obj_str = (NSString*)obj;
            icon = [obj_str UTF8String];
        }
        obj = [hash objectForKey:@"reload"];
        if (obj != nil) {
            NSNumber* obj_num = (NSNumber*)obj;
            if ([obj_num boolValue]) {
                reload = "true";
            }
            else {
                reload = "false";
            }
        }
        obj = [hash objectForKey:@"coloredIcon"];
        if (obj != nil) {
            NSNumber* obj_num = (NSNumber*)obj;
            if ([obj_num boolValue]) {
                colored_icon = "true";
            }
            else {
                colored_icon = "false";
            }
        }
        obj = [hash objectForKey:@"selectedColor"];
        if (obj != nil) {
            NSNumber* obj_num = (NSNumber*)obj;
            selected_color = [[NSString stringWithFormat:@"%@", obj_num] UTF8String];
        }
        obj = [hash objectForKey:@"disabled"];
        if (obj != nil) {
            NSNumber* obj_num = (NSNumber*)obj;
            if ([obj_num boolValue]) {
                disabled = "true";
            }
            else {
                disabled = "false";
            }
        }
        obj = [hash objectForKey:@"backgroundColor"];
        if (obj != nil) {
            NSNumber* obj_num = (NSNumber*)obj;
            web_bkg_color = [[NSString stringWithFormat:@"%@", obj_num] UTF8String];
        }
        obj = [hash objectForKey:@"useCurrentViewForTab"];
        if (obj != nil) {
            NSNumber* obj_num = (NSNumber*)obj;
            if ([obj_num boolValue]) {
                use_current_view_for_tab = "true";
                action = "none";
            }
            else {
                use_current_view_for_tab = "false";
            }
        }
        
        
        if (label == NULL && bar_type == TOOLBAR_TYPE)
            label = "";
        
        if ((label == NULL || (action == NULL)) && (!skip_item)) {
            RAWLOG_ERROR("Illegal argument for create_nativebar");
            return;
        }
		if (!skip_item) {
			NSMutableDictionary* item = [NSMutableDictionary dictionaryWithCapacity:10];	
			
			[item setObject:[NSString stringWithUTF8String:label] forKey:NATIVE_BAR_ITEM_LABEL];
			[item setObject:[NSString stringWithUTF8String:action] forKey:NATIVE_BAR_ITEM_ACTION];
			[item setObject:[NSString stringWithUTF8String:(icon ? icon : "")] forKey:NATIVE_BAR_ITEM_ICON];
			[item setObject:[NSString stringWithUTF8String:(reload ? reload : "false")] forKey:NATIVE_BAR_ITEM_RELOAD];
			[item setObject:[NSString stringWithUTF8String:(colored_icon ? colored_icon : "false")] forKey:NATIVE_BAR_ITEM_COLORED_ICON];
			if (selected_color != nil) {
				[item setObject:[NSString stringWithUTF8String:selected_color] forKey:NATIVE_BAR_ITEM_SELECTED_COLOR];
			}
			[item setObject:[NSString stringWithUTF8String:(disabled ? disabled : "false")] forKey:NATIVE_BAR_ITEM_DISABLED];
			if (web_bkg_color != NULL) {
				[item setObject:[NSString stringWithUTF8String:web_bkg_color] forKey:NATIVE_BAR_ITEM_WEB_BACKGROUND_COLOR];
			}
			[item setObject:[NSString stringWithUTF8String:(use_current_view_for_tab ? use_current_view_for_tab : "false")] forKey:NATIVE_BAR_ITEM_USE_CURRENT_VIEW_FOR_TAB];
			
			[items addObject:item];
		}
		
	}
	if (background_color != NULL) {
		[properties setObject:[NSString stringWithUTF8String:background_color] forKey:NATIVE_BAR_BACKGOUND_COLOR];	
	}
    if (ios_7_icon_color != NULL) {
		[properties setObject:[NSString stringWithUTF8String:ios_7_icon_color] forKey:NATIVE_BAR_ICON_COLOR];
    }
    if (ios_7_selected_color != NULL) {
		[properties setObject:[NSString stringWithUTF8String:ios_7_selected_color] forKey:NATIVE_BAR_SELECTED_COLOR];
    }
    
    //COMMONAPI
    if (callback != NULL) {
		[properties setObject:[callback retain] forKey:NATIVE_BAR_ON_CHANGE_TAB_CALLBACK];
    }
	
    id runnable = [RhoNativeBarCreateTask class];
    id arg1 = [NSValue valueWithBytes:&bar_t objCType:@encode(int)];
    [Rhodes performOnUiThread:runnable arg:arg1 arg:main_properties wait:NO];
}

void rho_create_toolbar(NSArray* items, NSDictionary* properties) {
    rho_create_nativebar_inner(TOOLBAR_TYPE, items, properties, nil);
}

void rho_create_tabbar(NSArray* items, NSDictionary* properties, id<IMethodResult>callback) {
    rho_create_nativebar_inner(TABBAR_TYPE, items, properties, callback);
}


void remove_nativebar_innner() {
    if (!rho_rhodesapp_check_mode())
        return;
    int bar_type = NOBAR_TYPE;
    id runnable = [RhoNativeBarCreateTask class];
    id arg1 = [NSValue valueWithBytes:&bar_type objCType:@encode(int)];
    [Rhodes performOnUiThread:runnable arg:arg1 arg:nil wait:NO];
}

void nativebar_switch_tab_innner(int index) {
    if (!rho_rhodesapp_check_mode())
        return;
    id runnable = [RhoNativeBarSwitchTabTask class];
    id arg = [NSValue valueWithBytes:&index objCType:@encode(int)];
    [Rhodes performOnUiThread:runnable arg:arg wait:NO];
}

BOOL nativebar_started() {
    return (started != 0);
}





void remove_native_toolbar() {
	remove_nativebar_innner();
}



void remove_native_tabbar() {
	remove_nativebar_innner();
}

void native_tabbar_switch_tab(int index) {
	nativebar_switch_tab_innner(index);
}



void native_tabbar_set_tab_badge(int index,char *val) {
    NSString* badge_text = [NSString stringWithUTF8String:val];
    
    NSMutableDictionary* params = [NSMutableDictionary dictionaryWithCapacity:2];
    NSValue* tab_index = [NSValue valueWithBytes:&index objCType:@encode(int)];
    [params setObject:tab_index forKey:TAB_INDEX];	
    [params setObject:badge_text forKey:BADGE_TEXT];	
    
    id runnable = [RhoNativeTabBarSetBadgeTask class];
    id arg = params;
    [Rhodes performOnUiThread:runnable arg:arg wait:NO];
    
}

int native_tabbar_get_current_tab() {
    Rhodes *r = [Rhodes sharedInstance];
    return [[r mainView] activeTab];
}


