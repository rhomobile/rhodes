/*
 *  SyncManager.m
 *  RhoSyncClient
 *
 *  Copyright (C) 2008 Rhomobile, Inc. All rights reserved.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#import <UIKit/UIKit.h>
#import <sys/socket.h>
#import <netinet/in.h>
#import <SystemConfiguration/SystemConfiguration.h>
#import <SystemConfiguration/SCNetworkReachability.h>
#import <Foundation/Foundation.h>
#include "common/RhoPort.h"
#include "ruby/ext/rho/rhoruby.h"
#import "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Net"

#define MAX_CONNECTION_TRY 64

extern int isNetworkAvailableFlags(SCNetworkReachabilityFlags *outFlags);
typedef void (*FSAVECONNDATA)(void* pThis, void* pData);

/*
 An enumeration that defines the return values of the network state
 of the device.
 */
typedef enum {
	NotReachable = 0,
	ReachableViaCarrierDataNetwork,
	ReachableViaWiFiNetwork
} NetworkStatus;

void rho_net_impl_network_indicator(int active)
{
	[UIApplication sharedApplication].networkActivityIndicatorVisible = active ? YES : NO;
}

// Determines network connectivity
VALUE rho_sys_has_network() {
	SCNetworkReachabilityFlags defaultRouteFlags;
	
	
	int defaultRouteIsAvailable = 0;
	
	int i = 0;
	while  ((!defaultRouteIsAvailable) && (i++ < MAX_CONNECTION_TRY)) {
		defaultRouteIsAvailable = isNetworkAvailableFlags(&defaultRouteFlags);
    }
	
	if (defaultRouteIsAvailable == 1) {
		if (defaultRouteFlags & kSCNetworkReachabilityFlagsIsDirect) {
			// Ad-Hoc network, not available
			return rho_ruby_create_boolean(0);
		}
		else if (defaultRouteFlags & ReachableViaCarrierDataNetwork) {
			// Cell network available
			return rho_ruby_create_boolean(1);
		}
		// WIFI available
		return rho_ruby_create_boolean(1);
	}
	return rho_ruby_create_boolean(0);
}

int isNetworkAvailableFlags(SCNetworkReachabilityFlags *outFlags)
{
	struct sockaddr_in zeroAddress;
	BOOL isReachable = FALSE;
	int reachable = 0;
	SCNetworkReachabilityRef defaultRouteReachability;
	//NSString *hostNameOrAddress; 
	bzero(&zeroAddress, sizeof(zeroAddress));
	zeroAddress.sin_len = sizeof(zeroAddress);
	zeroAddress.sin_family = AF_INET;
	
	defaultRouteReachability = SCNetworkReachabilityCreateWithAddress(NULL, (struct sockaddr *)&zeroAddress);
	//hostNameOrAddress = @"0.0.0.0";
	
	SCNetworkReachabilityFlags flags;
	BOOL gotFlags = SCNetworkReachabilityGetFlags(defaultRouteReachability, &flags);
	if (!gotFlags) {
		CFRelease(defaultRouteReachability);
        return reachable;
    }
    
	isReachable = flags & kSCNetworkReachabilityFlagsReachable;
	BOOL noConnectionRequired = !(flags & kSCNetworkReachabilityFlagsConnectionRequired);
	if ((flags & kSCNetworkReachabilityFlagsIsWWAN)) {
		noConnectionRequired = YES;
	}
	
	reachable = (isReachable && noConnectionRequired) ? 1 : 0;
	
	// Callers of this method might want to use the reachability flags, so if an 'out' parameter
	// was passed in, assign the reachability flags to it.
	if (outFlags) {
		*outFlags = flags;
	}
	CFRelease(defaultRouteReachability);
	return reachable;
}
