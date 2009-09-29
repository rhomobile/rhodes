//
//  MapAnnotation.h
//
//  Created by Vlad on 9/22/09.
//  Copyright 2009 rhomobile, inc. All rights reserved.
//
#ifdef __IPHONE_3_0

#import <MapKit/MapKit.h>
#import <MapKit/MKAnnotation.h>
#import <MapKit/MKReverseGeocoder.h>
#import <Foundation/Foundation.h>


@interface MapAnnotation : NSObject<MKAnnotation> {
	NSString *_title, *_subtitle, *_street_address, *_url;
	NSMutableString *_coordinate_string,*_resolved_address; 
	CLLocationCoordinate2D coordinate;
}

- (void)setCoordinate:(CLLocationCoordinate2D)coordinate;
- (void)setTitle:(NSString*)title;
- (void)setSubtitle:(NSString*)subtitle;
- (void)setAddress:(NSString*)street_address;
- (NSString*)street_address;
- (void)setCoordinateString:(NSString*)coordinate_string;
- (NSString*)coordinate_string;
- (void)setResolvedAddress:(NSString*)resolved_address;
- (NSString*)resolved_address;
- (void)setUrl:(NSString*)url;
- (NSString*)url;

@end

#endif