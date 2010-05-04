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
#import "common/rhoparams.h"

@interface MapViewController : UIViewController <MKReverseGeocoderDelegate,MKMapViewDelegate> {
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
    //IBOutlet UISegmentedControl *mapType;
}

// 
@property (copy) NSString *region_center;
@property (copy) NSString *gapikey; 

+ (void)createMap:(rho_param*)params;
- (void)setParams:(rho_param*)params;

@end

#endif