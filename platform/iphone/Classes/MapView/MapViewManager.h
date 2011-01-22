//
//  Created by DmitryS .
//  Copyright 2011 rhomobile, inc. All rights reserved.
//
#ifdef __IPHONE_3_0

#endif

#include "ruby/ext/rho/rhoruby.h"

@protocol RhoMapViewProvider <NSObject>

- (void)createMap:(rho_param*)params;
- (void)closeMap;
- (BOOL)isStarted;
- (double)centerLatitude;
- (double)centerLongitude;

@end

@interface RhoMapViewProvidersManager : NSObject {
	
}
+(void)registerMapViewProvider:(NSString*)name provider:(id)provider;

@end
