//
//  AppLoader.h
//  rhorunner
//
//  Created by vlad on 9/24/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface AppLoader : NSObject {

	NSString* appName;
	NSString* loadUrl;
	id		  actionTarget;
	SEL       onLoadSuccess;
	SEL       onLoadFailure;
	BOOL      isLoading;
}

//::Public 
- (bool) loadApplication:(NSString*)applicationName;
- (bool) loadApplicationAsync:(NSString*)applicationName;


// callbacks
@property (assign) id  actionTarget;
@property (assign) SEL onLoadSuccess;
@property (assign) SEL onLoadFailure;

//::Private


@end
