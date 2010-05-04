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

@synthesize type, title, subtitle, address, url, coordinateString, resolvedAddress, coordinate;

-(id) init {
    [super init];
    type = [[NSString alloc] initWithFormat:@"ann"];
    title = [[NSString alloc] initWithFormat:@""];
    subtitle = [[NSString alloc] initWithFormat:@""];
    address = [[NSString alloc] initWithFormat:@""];
    coordinateString = [[NSMutableString alloc] initWithFormat:@""]; 
    resolvedAddress = [[NSMutableString alloc] initWithFormat:@""];
    url = [[NSString alloc] initWithFormat:@""];
    return self;
}

-(void) dealloc {
    [type release];
    [title release];
    [subtitle release];
    [address release];
    [coordinateString release];
    [resolvedAddress release];
    [url release];
    [super dealloc];
}

- (void)setUrl:(NSString*)u {
    const char *surl = [u UTF8String];
    const char *norm_url = rho_http_normalizeurl(surl);
    [url release];
    url = [[NSString stringWithUTF8String:norm_url] copy];
}

@end

#endif
