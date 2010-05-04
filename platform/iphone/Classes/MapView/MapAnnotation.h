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
    NSString *type;
    NSString *title, *subtitle, *address, *url;
    NSString *coordinateString,*resolvedAddress; 
    CLLocationCoordinate2D coordinate;
}

@property (nonatomic,copy) NSString *type;
@property (nonatomic,copy) NSString *title;
@property (nonatomic,copy) NSString *subtitle;
@property (nonatomic,copy) NSString *address;
@property (nonatomic,copy) NSString *url;
@property (nonatomic,copy) NSString *coordinateString;
@property (nonatomic,copy) NSString *resolvedAddress;
@property (nonatomic,assign) CLLocationCoordinate2D coordinate;

@end

#endif