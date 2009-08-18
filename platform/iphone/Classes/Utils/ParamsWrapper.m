//
//  ParamsWrapper.m
//  rhorunner
//
//  Created by Vlad on 8/7/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "ParamsWrapper.h"


@implementation ParamsWrapper

@synthesize callName;
@synthesize values;
@synthesize names;

/*- (void)dealloc 
{
	[callName release];
	int cnt = [names count];
	for(int i = 0; i < cnt; i++) {
		[[names objectAtIndex:i] release];
		[[values objectAtIndex:i] release];
	}
	[values release];
	[names release];
	[super dealloc];
}*/

- (PARAMS_WRAPPER*) unwrap:(PARAMS_WRAPPER*)pw {
	pw->_callname = strdup([callName cStringUsingEncoding:[NSString defaultCStringEncoding]]);
	int cnt = [names count];
	pw->_nparams = cnt;
	pw->_names = malloc(cnt*sizeof(char*));
	pw->_values = malloc(cnt*sizeof(char*));
	for(int i = 0; i < cnt; i++) {
		pw->_names[i] = strdup([[names objectAtIndex:i] cStringUsingEncoding:[NSString defaultCStringEncoding]]);
		pw->_values[i] = strdup([[values objectAtIndex:i] cStringUsingEncoding:[NSString defaultCStringEncoding]]);
	}
	return pw;
}

+ (ParamsWrapper*) wrap:(PARAMS_WRAPPER*) params {
	ParamsWrapper* pw = [[ParamsWrapper alloc] init]; 
	pw->callName = [NSString stringWithCString:params->_callname encoding:[NSString defaultCStringEncoding]];
	pw->values = [NSMutableArray arrayWithCapacity:params->_nparams];
	pw->names = [NSMutableArray arrayWithCapacity:params->_nparams];
	for (int i=0;i<params->_nparams;i++) {
		[pw->names addObject:[NSString stringWithCString:params->_names[i] encoding:[NSString defaultCStringEncoding]]];
		[pw->values addObject:[NSString stringWithCString:params->_values[i] encoding:[NSString defaultCStringEncoding]]];
	}		
	return pw;
}
@end
