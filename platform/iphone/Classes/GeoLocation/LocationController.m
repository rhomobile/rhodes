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
#include "ruby/ext/rho/rhoruby.h"


#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Location"

int rho_rhodesapp_check_mode();
int rho_conf_getInt(const char *);

//#define RHO_GEO_LOCATION_INACTIVITY_TIMEOUT 25
//static CFTimeInterval timeOutInSeconds = RHO_GEO_LOCATION_INACTIVITY_TIMEOUT;
//static void _TimerCallBack(CFRunLoopTimerRef timer, void* context);

// This is a singleton class, see below
static LocationController *sharedLC = nil;


/*
@interface LocationManagerInit : NSObject {}
+ (void)run;
@end

@implementation LocationManagerInit
+ (void)run {
    [[LocationController sharedInstance] initLocationManager];
}
@end
*/

@implementation LocationController

@synthesize _locationManager;
//@synthesize onUpdateLocation; 

- (bool)update{
    @synchronized(self) {
        if (!_locationManager)
            return false;
        if (!_locationManager.locationServicesEnabled)
            return false;
        if (![CLLocationManager locationServicesEnabled])    
            return false;
        if (([CLLocationManager authorizationStatus] != kCLAuthorizationStatusAuthorizedWhenInUse) &&
            ([CLLocationManager authorizationStatus] != kCLAuthorizationStatusNotDetermined) ) 
            return false;
        isEnabled = YES;
        _locationManager.delegate = self; // Tells the location manager to send updates to this object
        if (_bMinDistanceMode) {
            _locationManager.distanceFilter = _dMinDistance;
            _locationManager.desiredAccuracy = _dMinDistance;
            [_locationManager startUpdatingLocation];
        }
        else {
            _locationManager.distanceFilter = kCLDistanceFilterNone;
            _locationManager.desiredAccuracy = kCLLocationAccuracyBest;
            [_locationManager startUpdatingLocation];
        }
    }
    return true;
}

- (void)onTimerFired:(NSTimer*)theTimer {
    if (isEnabled) {
        isFirstUpdateFromPlatform = NO;
        rho_geo_callcallback();
    }
}

- (void)resetTimerCommand:(NSNumber*)value {
    @synchronized(self){
        if (timer != nil) {
            [timer invalidate];
            timer = nil;
        }
        gps_callback_interval = [value integerValue];
        isFirstUpdateFromPlatform = YES;
        _bMinDistanceMode = false;
        if (gps_callback_interval > 0) {
            timer = [NSTimer scheduledTimerWithTimeInterval:gps_callback_interval target:self selector:@selector(onTimerFired:) userInfo:nil repeats:YES];
        }
    }
}

- (void)resetTimerWithNewInterval:(int)interval {
    [self performSelectorOnMainThread:@selector(resetTimerCommand:) withObject:[NSNumber numberWithInt:interval] waitUntilDone:NO];
}


-(void) stopCommand:(NSNumber*)number {
    @synchronized(self) {
        isEnabled = NO;
        if (!_locationManager)
            return;
        [_locationManager stopUpdatingLocation];
        _locationManager.delegate = nil;
        
        if (timer != nil) {
            [timer invalidate];
            timer = nil;
        }
    }
}

- (void)resetUpdateWithMinDistanceCommand:(NSNumber*)value {
    @synchronized(self){
        _dMinDistance = [value doubleValue];
        if (_dMinDistance <= 0) {
            _dMinDistance = kCLDistanceFilterNone;
        }
        _bMinDistanceMode = true;
    }
    [self stopCommand:nil];
    [self update];
}

- (void)resetUpdateWithMinDistance:(double)minDistance {
    [self performSelectorOnMainThread:@selector(resetUpdateWithMinDistanceCommand:) withObject:[NSNumber numberWithDouble:minDistance] waitUntilDone:NO];
}



- (void)doUpdateLocation {
	[self update];
}

- (void) initLocationManager:(NSObject*)param {

    self._locationManager = [[[CLLocationManager alloc] init] autorelease];
    self._locationManager.desiredAccuracy = kCLLocationAccuracyBest;
    self._locationManager.delegate = self; // Tells the location manager to send updates to this object
    
    if ([self._locationManager respondsToSelector:@selector(requestWhenInUseAuthorization)]) {
        [self._locationManager requestWhenInUseAuthorization];
    }
    
    
    
}

- (id) init {
	self = [super init];
	if (self != nil) {
		[self performSelectorOnMainThread:@selector(initLocationManager:) withObject:nil waitUntilDone:YES];
        
        timer = nil;
		_dLatitude = 0;
		_dLongitude = 0;
		_dAltitude = 0;
        _dAccuracy = 0;
        _iSatellites = 0;
        _dMinDistance = 0;
        _dSpeed = 0;
		_bKnownPosition = false;
        _bMinDistanceMode = false;
		isErrorState = false;
        gps_callback_interval = 0;
        isFirstUpdateFromPlatform = YES;
        isEnabled = YES;
    	RAWLOG_INFO("init");
	}
	return self;
}



- (void) stop {
    isEnabled = NO;
    [self performSelectorOnMainThread:@selector(stopCommand:) withObject:nil waitUntilDone:NO];
}

// Called when the location is updated
- (void)locationManager:(CLLocationManager *)manager
	didUpdateToLocation:(CLLocation *)newLocation
		   fromLocation:(CLLocation *)oldLocation
{
	RAWTRACE("Updated location");
	if (!newLocation)
		return;
	
	@synchronized(self) {
		
		_dLatitude = newLocation.coordinate.latitude;
		_dLongitude = newLocation.coordinate.longitude;
        _dAccuracy = newLocation.horizontalAccuracy;//sqrt(newLocation.horizontalAccuracy*newLocation.horizontalAccuracy + newLocation.verticalAccuracy*newLocation.verticalAccuracy);
		_dAltitude = newLocation.altitude;
        _dSpeed = newLocation.speed;
        _iSatellites = 0;
		_bKnownPosition = true;	
	}
    if (_bMinDistanceMode) {
        rho_geo_callcallback();
    }
    else {
        if (isFirstUpdateFromPlatform && isEnabled) {
            isFirstUpdateFromPlatform = NO;
            rho_geo_callcallback();
        }
    }
}

- (double) getLatitude{
	double res = 0; 
	@synchronized(self){
		res = _dLatitude;
	}
	
	return res;
}

- (double) getAccuracy {
	double res = 0; 
	@synchronized(self){
		res = _dAccuracy;
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

- (double) getAltitude{
	double res = 0; 
	@synchronized(self){
		res = _dAltitude;
	}
	
	return res;
}


- (double) getSpeed {
	double res = 0;
	@synchronized(self){
		res = _dSpeed;
	}
	
	return res;
}

- (int) getSatellites {
	int res = 0;
	@synchronized(self){
		res = _iSatellites;
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
		res =   _locationManager && 
                _locationManager.locationServicesEnabled && 
                [CLLocationManager locationServicesEnabled] && 
                ( ([CLLocationManager authorizationStatus] == kCLAuthorizationStatusAuthorized ) || ([CLLocationManager authorizationStatus] == kCLAuthorizationStatusNotDetermined )||([CLLocationManager authorizationStatus] == kCLAuthorizationStatusAuthorizedWhenInUse ) );
	}
	return res;
}

- (void)locationManager:(CLLocationManager *)manager
	   didFailWithError:(NSError *)error
{
    /* Do not remove it - this code used in debugging   
    if (error != nil) {
        const char* description = [[error localizedDescription] UTF8String];
        const char* reason = [[error localizedFailureReason] UTF8String];
        
        int o = 0;
        o = 9;
    }
     */    
    isErrorState = true;
    _bKnownPosition = false;
	RAWLOG_ERROR("Error in GeoLocation");
	rho_geo_callcallback_error();
    isErrorState = false;
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

- (void) runUpdateInMainThread
{
    if (!isErrorState) {
        [self performSelectorOnMainThread:@selector(doUpdateLocation) withObject:nil waitUntilDone:NO];
    }
}

@end



void geo_update() {
    if (!rho_rhodesapp_check_mode())
        return;
    LocationController *loc = [LocationController sharedInstance];
    [loc runUpdateInMainThread];
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

double rho_geo_altitude() {
	geo_update();
	return [[LocationController sharedInstance] getAltitude];
}

float rho_geo_accuracy() {
	geo_update();
	return (float)[[LocationController sharedInstance] getAccuracy ];
}

double rho_geo_speed() {
	geo_update();
	return (float)[[LocationController sharedInstance] getSpeed ];
}

int rho_geo_satellites() {
	geo_update();
	return (float)[[LocationController sharedInstance] getSatellites ];
}

void rho_geo_set_notification_ex(const char *url, rho_param* p, char* params) {
    double minDistance = kCLDistanceFilterNone;

    // setup callback url
    rho_geo_set_notification(url, params, 60*60*24*365);
    
    if (p && p->type == RHO_PARAM_HASH) {
        for (int i = 0, lim = p->v.hash->size; i < lim; ++i) {
            char *name = p->v.hash->name[i];
            rho_param *value = p->v.hash->value[i];
            if (!name || !value)
                continue;
            
            if (strcasecmp(name, "minimumDistance") == 0)
                if (value->type != RHO_PARAM_STRING)
                    continue;
            char *strMinDistance = value->v.string;
            minDistance = strtod(strMinDistance, NULL);
        }
    }
    [[LocationController sharedInstance] resetUpdateWithMinDistance:minDistance];
}


int rho_geo_known_position() {
	geo_update();
	return [[LocationController sharedInstance] isKnownPosition] ? 1 : 0;
}

void rho_geoimpl_settimeout(int nTimeoutSec)
{
    [[LocationController sharedInstance] resetTimerWithNewInterval:nTimeoutSec];
}

void rho_geoimpl_turngpsoff()
{
	RAWLOG_INFO("Explicit request to stop location controller");
	[[LocationController sharedInstance] stop];
}

