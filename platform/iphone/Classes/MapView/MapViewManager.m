//
//  Created by DmitryS .
//  Copyright 2011 rhomobile, inc. All rights reserved.
//


#import "MapAnnotation.h"
#import "MapViewController.h"
//#import "MapViewControllerESRI.h"
#import "Rhodes.h"
#import "RhoMainView.h"

#include "logging/RhoLog.h"
#include "ruby/ext/rho/rhoruby.h"

#import "MapViewManager.h"


#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "MapViewCore"


static NSMutableDictionary* map_providers = nil;

@implementation RhoMapViewProvidersManager


+(void)registerMapViewProvider:(NSString*)name provider:(id)provider {
	if (map_providers == nil) {
		map_providers = [NSMutableDictionary dictionaryWithCapacity:5];
	}
	[map_providers setObject:provider forKey:name];
}

@end




@interface MapEngine_Google : NSObject <RhoMapViewProvider>
{
}

- (void)createMap:(rho_param*)params;
- (void)closeMap;
- (BOOL)isStarted;
- (double)centerLatitude;
- (double)centerLongitude;

@end

@implementation MapEngine_Google

- (void)createMap:(rho_param*)params {
	[MapViewController createMap:params];
}

- (void)closeMap {
	[MapViewController closeMap];
}

- (BOOL)isStarted {
	return [MapViewController isStarted];
}

- (double)centerLatitude {
	return [MapViewController centerLatitude];
}

- (double)centerLongitude {
	return [MapViewController centerLongitude];
}

@end




static id<RhoMapViewProvider,NSObject> engine_p = nil;


//extern void register_ESRI_mapViewProvider();
//extern void Init_ESRI();

void mapview_create(rho_param *p) {
    if (!rho_rhodesapp_check_mode())
        return;
#ifdef __IPHONE_3_0
	if (map_providers == nil) {
		[RhoMapViewProvidersManager registerMapViewProvider:@"Google" provider:[[MapEngine_Google alloc] init]];
	}
	else {
		if ([map_providers objectForKey:@"Google"] == nil) {
			[RhoMapViewProvidersManager registerMapViewProvider:@"Google" provider:[[MapEngine_Google alloc] init]];
		}
	}
	
	NSString* engine = @"Google";
    if (p && p->type == RHO_PARAM_HASH) {
		rho_param *eng_p = NULL;
        for (int i = 0, lim = p->v.hash->size; i < lim; ++i) {
            char *name = p->v.hash->name[i];
            rho_param *value = p->v.hash->value[i];
            if (strcasecmp(name, "provider") == 0)
                eng_p = value;
        }
		if (eng_p && eng_p->type == RHO_PARAM_STRING) {
			engine = [NSString stringWithUTF8String:eng_p->v.string];
		}
    }
	
	id<RhoMapViewProvider,NSObject> eng_p = [map_providers objectForKey:engine];
	if (eng_p != nil ) {
		engine_p = eng_p;
	}
	/*
	if ([engine compare:@"Google"] == NSOrderedSame) {
		engine_p = [[MapEngine_Google alloc] init];
	}
	else if ([engine compare:@"ESRI"] == NSOrderedSame) {
		engine_p = [[MapEngine_ESRI alloc] init];
	}
	*/
	if (engine_p != nil) {
		[engine_p createMap:rho_param_dup(p)];
	}
#endif	
}

void mapview_close() {
    if (!rho_rhodesapp_check_mode())
        return;
#ifdef __IPHONE_3_0
	if (engine_p != nil) {
		[engine_p closeMap];
	}
#endif
}

VALUE mapview_state_started() {
#ifdef __IPHONE_3_0
    BOOL started = NO;
	if (engine_p != nil) {
		started = [engine_p isStarted];
	}
    return rho_ruby_create_boolean(started);
#else
    return 0;
#endif
}

double mapview_state_center_lat() {
#ifdef __IPHONE_3_0
	if (engine_p != nil) {
		return [engine_p centerLatitude];
	}
	return 0;
#else
    return 0;
#endif
}

double mapview_state_center_lon() {
#ifdef __IPHONE_3_0
	if (engine_p != nil) {
		return [engine_p centerLongitude];
	}
	return 0;
#else
    return 0;
#endif
}
