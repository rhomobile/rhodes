//
//  LocationController.m
//  rhorunner
//
//  Created by vlad on 10/15/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#if TARGET_IPHONE_SIMULATOR
#import <CoreServices/CoreServices.h>
#else
#import <CFNetwork/CFNetwork.h>
#endif

#import "defs.h"
#import <CoreLocation/CoreLocation.h>
#import "LocationController.h"

char* GeoGetLocation() {
	return [[LocationController sharedInstance] getLocation];
}

//
static const CFTimeInterval kTimeOutInSeconds = 25;
static void _TimerCallBack(CFRunLoopTimerRef timer, void* context);

// This is a singleton class, see below
static LocationController *sharedLC = nil;

//Static return buffer (assuming get methods will be called from the same thread)
static char location_message[256];

@implementation LocationController

@synthesize _locationManager;

- (id) init {
	self = [super init];
	if (self != nil) {
		self._locationManager = [[[CLLocationManager alloc] init] autorelease];
		self._locationManager.desiredAccuracy = kCLLocationAccuracyBest;
		self._locationManager.delegate = self; // Tells the location manager to send updates to this object
	}
	return self;
}

- (bool)update{
	if (!_locationManager.locationServicesEnabled) {
		return false;
	}
	if (_timer==NULL) {
		_timer = CFRunLoopTimerCreate(NULL,
									  CFAbsoluteTimeGetCurrent() + kTimeOutInSeconds,
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
		CFRunLoopTimerSetNextFireDate(_timer, CFAbsoluteTimeGetCurrent() + kTimeOutInSeconds);
	}
	[_locationManager startUpdatingLocation];
	return true;
}

- (double) getLatitude{
	if (![self update]) {
		return 0.0f;
	}
	_location = _locationManager.location;
	if (_location==NULL) {
		return 0.0f;
	}
	double latitude = _location.coordinate.latitude;
	return latitude;
}

- (double) getLongitude{
	if (![self update]) {
		return 0.0f;
	}
	_location = _locationManager.location;
	if (_location==NULL) {
		return 0.0f;
	}
	return _location.coordinate.longitude;
}

- (int) isKnownLocation{
	if (![self update]) {
		return 0;
	}
	_location = _locationManager.location;
	if (_location==NULL) {
		return 0;
	}
	return 1;
}

- (char*) getLocation {
	if (![self update]) {
		return "Unavailable;Unavailable;Unavailable";
	}
	_location = _locationManager.location;
	if (_location==NULL) {
		return "Reading;Reading;Reading";
	}
	double latitude = _location.coordinate.latitude;
	double longitude = _location.coordinate.longitude;
	sprintf(location_message, "%.4f° %s, %.4f° %s;%f;%f",
			fabs(latitude),signbit(latitude) ? "South" : "North",
			fabs(longitude),signbit(longitude) ? "West" : "East",
			latitude,longitude);
	return location_message;
}

- (void) stop {
    // Get rid of the timer, if it still exists
    if (_timer != NULL) {
        CFRunLoopTimerInvalidate(_timer);
        CFRelease(_timer);
        _timer = NULL;
    }
	[_locationManager stopUpdatingLocation];
}

// Called when the location is updated
- (void)locationManager:(CLLocationManager *)manager
	didUpdateToLocation:(CLLocation *)newLocation
		   fromLocation:(CLLocation *)oldLocation
{
	DBG(("Updated location\n"));
}

- (void)locationManager:(CLLocationManager *)manager
	   didFailWithError:(NSError *)error
{
	DBG(("Error reading location\n"));
}

+ (LocationController *)sharedInstance {
    @synchronized(self) {
        if (sharedLC == nil) {
            [[self alloc] init]; // assignment not done here
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
	DBG(("Stopping location controller on timeout\n"));
	//[[LocationController sharedInstance] stop];
}

	double geo_latitude() {
		
		return [[LocationController sharedInstance] getLatitude];
	}
	
	double geo_longitude() {
		
		return [[LocationController sharedInstance] getLongitude];
	}
	
	int geo_known_position() {
		return [[LocationController sharedInstance] isKnownLocation];
	}

