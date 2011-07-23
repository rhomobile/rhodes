//
//  syncEngine.h
//  store
//
//  Created by Vlad on 8/30/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "../../../../ObjectiveC/RhoConnectClient.h"

enum LoginState {
	logged_out,
	logged_in,
	in_progress,
	failed
};

@interface SyncEngine : NSObject {
	enum LoginState loginState;
	
	RhomModel* customer;	
	RhomModel* product;	
	RhoConnectClient* sclient;
}

+ (void)create;
+ (void)destroy;

+ (SyncEngine *)sharedInstance;

@property enum LoginState loginState;

@property(readonly, getter=getSClient) RhoConnectClient* syncClient;

@property(readonly) RhomModel* product;

- (void)dealloc;

@end


