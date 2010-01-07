//
//  MapAnnotation.m
//
//  Created by Vlad on 9/22/09.
//  Copyright 2009 rhomobile, inc. All rights reserved.
//

#ifdef __IPHONE_3_0

#import "MapAnnotation.h"
#include "common/RhodesApp.h"

@implementation MapAnnotation

@synthesize coordinate;

-(id)init{
	[super init];
	_title = [[NSString alloc] initWithFormat:@""];
	_subtitle = [[NSString alloc] initWithFormat:@""];
	_street_address = [[NSString alloc] initWithFormat:@""];
	_coordinate_string = [[NSMutableString alloc] initWithFormat:@""]; 
	_resolved_address = [[NSMutableString alloc] initWithFormat:@""];
	_url = [[NSString alloc] initWithFormat:@""];
	return self;
}

- (void) dealloc {
/*	self._title = nil;
	self._subtitle = nil;
	self._street_address = nil;
	self._coordinate_string = nil; 
	self._resolved_address = nil; 
*/	[super dealloc];
}

- (void)setCoordinate:(CLLocationCoordinate2D)c {
	coordinate = c;
}

- (void)setTitle:(NSString*)title {
	_title = [title copy];
}

- (NSString *)title {
	return _title;
}

- (void)setSubtitle:(NSString*)subtitle {
	_subtitle = [subtitle copy];
}

- (NSString *)subtitle {
	return _subtitle;
}

- (void)setAddress:(NSString*)street_address {
	_street_address = [street_address copy];
}

- (NSString*)street_address {
	return _street_address;
}

- (void)setCoordinateString:(NSString*)coordinate_string {
	[_coordinate_string appendString:coordinate_string];
}

- (NSString*)coordinate_string {
	return _coordinate_string;
}

- (void)setResolvedAddress:(NSString*)resolved_address {
	[_resolved_address appendString:resolved_address];
}


- (NSString*)resolved_address {
	return _resolved_address;
}

- (void)setUrl:(NSString*)url {
    const char *norm_url = rho_http_normalizeurl([url UTF8String]);
    _url = [NSString stringWithUTF8String:norm_url];
}

- (NSString*)url {
	return _url;
}

@end

#endif
