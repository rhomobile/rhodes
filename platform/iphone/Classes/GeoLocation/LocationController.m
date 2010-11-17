//
//  LocationController.m
//  rhorunner
//
//  Created by vlad on 10/15/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import <CFNetwork/CFNetwork.h>

#import <CoreLocation/CoreLocation.h>
#import "Rhodes.h"
#import "LocationController.h"
#import "logging/RhoLog.h"
#include "rubyext/GeoLocation.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Location"

int rho_rhodesapp_check_mode();
int rho_conf_getInt(const char *);

#define RHO_GEO_LOCATION_INACTIVITY_TIMEOUT 25
static CFTimeInterval timeOutInSeconds = RHO_GEO_LOCATION_INACTIVITY_TIMEOUT;
static void _TimerCallBack(CFRunLoopTimerRef timer, void* context);

// This is a singleton class, see below
static LocationController *sharedLC = nil;

@interface LocationManagerInit : NSObject {}
+ (void)run;
@end

@implementation LocationManagerInit
+ (void)run {
    [[LocationController sharedInstance] initLocationManager];
}
@end


@implementation LocationController

@synthesize _locationManager;
@synthesize onUpdateLocation; 

- (bool)update{
    if (!_locationManager)
        return false;
	if (!_locationManager.locationServicesEnabled) {
		return false;
	}
	if (_timer==NULL) {
		_timer = CFRunLoopTimerCreate(NULL,
									  CFAbsoluteTimeGetCurrent() + timeOutInSeconds,
									  0,		// interval
									  0,		// flags
									  0,		// order
									  (CFRunLoopTimerCallBack)_TimerCallBack,
									  NULL);
		// Fail if unable to create the timer.
		if (_timer == NULL)
			return false;
        CFRunLoopAddTimer(CFRunLoopGetCurrent(), _timer, kCFRunLoopCommonModes);
	} else {
		CFRunLoopTimerSetNextFireDate(_timer, CFAbsoluteTimeGetCurrent() + timeOutInSeconds);
	}
	
	[_locationManager startUpdatingLocation];
	return true;
}

- (void)doUpdateLocation {
	[self update];
}

- (void) initLocationManager {
    int timeout = rho_conf_getInt("geo_location_inactivity_timeout");
    if (timeout == 0)
        timeout = RHO_GEO_LOCATION_INACTIVITY_TIMEOUT;
    timeOutInSeconds = timeout;
    self._locationManager = [[[CLLocationManager alloc] init] autorelease];
    self._locationManager.desiredAccuracy = kCLLocationAccuracyBest;
    self._locationManager.delegate = self; // Tells the location manager to send updates to this object
}

- (id) init {
	self = [super init];
	if (self != nil) {
		[Rhodes performOnUiThread:[LocationManagerInit class] wait:NO];
		
		self.onUpdateLocation = @selector(doUpdateLocation);	
		_dLatitude = 0;
		_dLongitude = 0;
		_bKnownPosition = false;
		
    	RAWLOG_INFO("init");
		
	}
	
	return self;
}

- (void) stop {
    if (!_locationManager)
        return;
	[_locationManager stopUpdatingLocation];
	
    // Get rid of the timer, if it still exists
    if (_timer != NULL) {
        CFRunLoopTimerInvalidate(_timer);
        CFRelease(_timer);
        _timer = NULL;
    }
	
}

// Called when the location is updated
- (void)locationManager:(CLLocationManager *)manager
	didUpdateToLocation:(CLLocation *)newLocation
		   fromLocation:(CLLocation *)oldLocation
{
	RAWTRACE("Updated location");
	if (!newLocation)
		return;
	
    bool bNotify = false; 
	
	@synchronized(self){
		
		bNotify = _bKnownPosition==0 || _dLatitude != newLocation.coordinate.latitude || _dLongitude != newLocation.coordinate.longitude;
	
		_dLatitude = newLocation.coordinate.latitude;
		_dLongitude = newLocation.coordinate.longitude;
		_bKnownPosition = true;	
	}
	
    if ( bNotify )
        rho_geo_callcallback();
	
}

- (double) getLatitude{
	double res = 0; 
	@synchronized(self){
		res = _dLatitude;
	}
	
	return res;
}

- (double) getLongitude{
	double res = 0; 
	@synchronized(self){
		res = _dLongitude;
	}
	return res;	
}

- (bool) isKnownPosition{
	bool res = false;
	@synchronized(self){
		res = _bKnownPosition;
	}
	return res;
}

- (bool) isAvailable{
	bool res = false;
	@synchronized(self){
		res = _locationManager &&_locationManager.locationServicesEnabled;
	}
	return res;
}

- (void)locationManager:(CLLocationManager *)manager
	   didFailWithError:(NSError *)error
{
	RAWLOG_ERROR("Error reading location");
	rho_geo_callcallback_error();
}

+ (LocationController *)sharedInstance {
    @synchronized(self) {
        if (sharedLC == nil) {
            [[LocationController alloc] init]; // assignment not done here
        }
    }
    return sharedLC;
}

+ (id)allocWithZone:(NSZone *)zone {
    @synchronized(self) {
        if (sharedLC == nil) {
            sharedLC = [super allocWithZone:zone];
            return sharedLC;  // assignment and return on first allocation
        }
    }
    return nil; // on subsequent allocation attempts return nil
}

- (id)copyWithZone:(NSZone *)zone
{
    return self;
}

- (id)retain {
    return self;
}

- (unsigned)retainCount {
    return UINT_MAX;  // denotes an object that cannot be released
}

- (void)release {
    //do nothing
}

- (id)autorelease {
    return self;
}

@end

/* static */ void
_TimerCallBack(CFRunLoopTimerRef timer, void* context) {
	RAWLOG_INFO("Stopping location controller on timeout");
	[[LocationController sharedInstance] stop];
}

void geo_update() {
    if (!rho_rhodesapp_check_mode())
        return;
    LocationController *loc = [LocationController sharedInstance];
    [loc performSelectorOnMainThread:[loc onUpdateLocation] withObject:nil waitUntilDone:NO];
}

double rho_geo_latitude() {
	geo_update();
	return [[LocationController sharedInstance] getLatitude ];
}

int rho_geo_is_available()
{
	return [[LocationController sharedInstance] isAvailable] ? 1 : 0;	
}

void rho_geoimpl_init()
{
	[LocationController sharedInstance];
}

double rho_geo_longitude() {
	geo_update();
	return [[LocationController sharedInstance] getLongitude];
}
	
int rho_geo_known_position() {
	geo_update();
	return [[LocationController sharedInstance] isKnownPosition] ? 1 : 0;
}

void rho_geoimpl_settimeout(int nTimeoutSec)
{
}
