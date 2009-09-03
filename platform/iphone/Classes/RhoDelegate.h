//
//  RhoDelegate.h
//  rhorunner
//
//  Created by lars on 8/11/09.
//  Copyright 2009 Home. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface RhoDelegate : NSObject {
	NSString* postUrl; 
}

@property (readwrite, copy) NSString *postUrl;

- (void)doCallback:(NSString*)message;

@end