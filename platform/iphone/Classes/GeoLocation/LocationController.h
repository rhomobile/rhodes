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
	
@public	
	double _dLatitude, _dLongitude;
	bool _bKnownPosition;
	
	SEL onUpdateLocation;	
}

@property (nonatomic, retain) CLLocationManager *_locationManager;
@property (assign) SEL onUpdateLocation;

- (void) initLocationManager;
- (void)  stop; 

- (double) getLatitude;
- (double) getLongitude;
- (bool) isKnownPosition;
- (bool) isAvailable;

- (void)locationManager:(CLLocationManager *)manager
	didUpdateToLocation:(CLLocation *)newLocation
		   fromLocation:(CLLocation *)oldLocation;

- (void)locationManager:(CLLocationManager *)manager
	   didFailWithError:(NSError *)error;

+ (LocationController *)sharedInstance;

@end
