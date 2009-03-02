//
//  LocationController.h
//  rhorunner
//
//  Created by vlad on 10/15/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface LocationController : NSObject<CLLocationManagerDelegate> {
	CLLocationManager* _locationManager;
	CFRunLoopTimerRef  _timer;
	CLLocation*		_location;
}

@property (nonatomic, retain) CLLocationManager *_locationManager;

- (char*) getLocation;
- (void)  stop; 

- (double) getLatitude;
- (double) getLongitude;
- (int) isKnownLocation;

- (void)locationManager:(CLLocationManager *)manager
	didUpdateToLocation:(CLLocation *)newLocation
		   fromLocation:(CLLocation *)oldLocation;

- (void)locationManager:(CLLocationManager *)manager
	   didFailWithError:(NSError *)error;

+ (LocationController *)sharedInstance;

@end
