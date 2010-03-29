//
//  ServerHost.h
//  Browser
//
//  Created by adam blum on 9/10/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "AppManager.h"
//#import "SyncEngine.h"

@interface ServerHost : NSObject
{
	AppManager * appManager;
	//sqlite3 *database;
	//NSString *homeUrl,*optionsUrl;
	
@public
    CFRunLoopRef runLoop;
	NSThread* m_geoThread;
	id  actionTarget;
	SEL onTakePicture;
	SEL onChoosePicture;
	SEL onChooseDateTime;
	SEL onCreateMap;
}

// callbacks
@property (assign) id  actionTarget;
@property (assign) SEL onTakePicture;
@property (assign) SEL onChoosePicture;
@property (assign) SEL onChooseDateTime;
@property (assign) SEL onCreateMap;

//Sync all sources
//- (void) doSync;
//- (void) doSyncFor:(NSString*)url;

+ (ServerHost *)sharedInstance;

@end