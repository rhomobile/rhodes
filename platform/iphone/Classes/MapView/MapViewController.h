//
//  MapViewController.h
//
//  Created by Vlad on 9/16/09.
//  Copyright 2009 Rhomobile, Inc. All rights reserved.
//

#ifdef __IPHONE_3_0

#import <MapKit/MapKit.h>
#import <MapKit/MKAnnotation.h>
#import <MapKit/MKReverseGeocoder.h>
#import "GoogleGeocoder.h"
#import "RhoViewController.h"

#include "ruby/ext/rho/rhoruby.h"


 

@interface MapViewController : RhoViewController <MKMapViewDelegate, MKReverseGeocoderDelegate> {
    MKMapView *mapView;
    MKReverseGeocoder *geoCoder;
    GoogleGeocoder* ggeoCoder;
    UIToolbar* toolbar;
    MKMapType mapType;
    BOOL zoomEnabled;
    BOOL scrollEnabled;	
    BOOL showsUserLocation;
    BOOL region_set;	
    MKCoordinateRegion region;
    NSString *region_center;
    CLLocationDegrees region_radius;
    NSString *gapikey;
	UIView* savedMainView;
    //IBOutlet UISegmentedControl *mapType;
}

// 
@property (copy) NSString *region_center;
@property (copy) NSString *gapikey;
@property (nonatomic,retain) UIView *savedMainView;

+ (void)createMap:(rho_param*)params;
+ (void)closeMap;

- (void)setParams:(rho_param*)params;
- (void)close;

@end

#endif