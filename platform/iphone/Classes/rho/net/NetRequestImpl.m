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

int rho_net_ping_network(const char* szHost)
{
	RAWLOG_INFO("PING network.");
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	
	NSMutableURLRequest *request = [[[NSMutableURLRequest alloc] init] autorelease];
	NSString *linkString = [[NSString alloc] initWithUTF8String:szHost];

	[request setURL:[NSURL URLWithString:linkString]];
	[request setTimeoutInterval:10];
	
	NSError *error = nil;
	NSHTTPURLResponse *response;
	NSData *returnData = NULL;
	returnData = [ NSURLConnection sendSynchronousRequest: request returningResponse:&response error: &error ];
	
	if (!returnData)
		RAWLOG_ERROR2("PING network FAILED. NSError: %d. NSErrorInfo : %s", [error code], [[error localizedDescription] UTF8String]);
	else
		RAWLOG_INFO("PING network SUCCEEDED.");	
	
	[pool release];
	
	return returnData == NULL ? 0 : 1;
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
