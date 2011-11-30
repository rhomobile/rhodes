//
//  syncEngine.m
//  store
//
//  Created by Vlad on 8/30/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "RhoConnectEngine.h"

@implementation RhoConnectEngine

@synthesize loginState;
@synthesize product;

static RhoConnectEngine *sharedInst = nil;

+ (void)create
{
    @synchronized( self ) {
        if ( sharedInst == nil ) {
            /* sharedInst set up in init */
            [[self alloc] init];
        }
    }
}

+ (void)destroy
{
	if ( sharedInst ) 
	{	
		[sharedInst dealloc];
		sharedInst = nil;
	}
}

+ (id)sharedInstance
{
    return sharedInst;
}

- (id)init
{
    if ( sharedInst != nil ) {
        [NSException raise:NSInternalInconsistencyException
			format:@"[%@ %@] cannot be called; use +[%@ %@] instead",
			NSStringFromClass([self class]), NSStringFromSelector(_cmd), 
			NSStringFromClass([self class]),
			NSStringFromSelector(@selector(sharedInstance))];
	} else if ( self = [super init] ) {
		sharedInst = self;
		
    	[RhoConnectClient initDatabase];		
		
		customer = [[RhomModel alloc] init];
		customer.name = @"Customer";
		
		product = [[RhomModel alloc] init];
		product.name = @"Product";
		
		sclient = [[RhoConnectClient alloc] init];
        [sclient setLogSeverity:1];
        
		NSMutableArray* models = [NSMutableArray arrayWithObjects:customer, product, nil];	
		
		[sclient addModels:models];
		
		sclient.sync_server = @"http://rhodes-store-server.heroku.com/application";
        //sclient.sync_server = @"http://localhost:9292/application";
		sclient.threaded_mode = TRUE;
		
		loginState = [sclient is_logged_in] ? logged_in : logged_out;
	}
	return sharedInst;
}

- (void)dealloc 
{
    [customer release];
    [product release];
    [sclient release];
	
    [super dealloc];
}

- (RhoConnectClient*) getSClient
{
	return sclient;
}

/* These probably do nothing in
   a GC app.  Keeps singleton
   as an actual singleton in a
   non CG app
*/
- (NSUInteger)retainCount
{
	return NSUIntegerMax;
}
								 
- (oneway void)release
{
}
								 
- (id)retain
{
	return sharedInst;
}
								 
- (id)autorelease
{
	return sharedInst;
}
								 
@end