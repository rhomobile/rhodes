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
	NSString *gapikey;  
	//IBOutlet UISegmentedControl *mapType;
@public
	id  actionTarget;
	SEL onNavigate;

}

// 
@property (assign) id  actionTarget;
@property (assign) SEL onNavigate;
@property (copy) NSString* gapikey; 

//- (IBAction)changeType:(id) sender;
- (void)setParams:(NSMutableArray*)params;
- (void)setAnnotations:(NSMutableArray*)annotations;

@end

NSMutableArray* parse_annotations(int nannotations, char** annotation);
NSMutableArray* parse_settings(int nparams, char** params);

#endif