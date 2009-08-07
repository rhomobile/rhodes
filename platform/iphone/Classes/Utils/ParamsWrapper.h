//
//  ParamsWrapper.h
//  rhorunner
//
//  Created by Vlad on 8/7/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "syscall.h"


@interface ParamsWrapper : NSObject {
	NSString*  callName;
	NSMutableArray* names;
	NSMutableArray* values;
}

@property (assign) NSString *callName;
@property (assign) NSMutableArray* names;
@property (assign) NSMutableArray* values;

- (PARAMS_WRAPPER*) unwrap:(PARAMS_WRAPPER*)pw;
+ (ParamsWrapper*) wrap:(PARAMS_WRAPPER*) params;

@end
