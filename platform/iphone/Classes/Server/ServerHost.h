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
	SEL onStartSuccess;
	//SEL onStartFailure;
	SEL onRefreshView;
	SEL onNavigateTo;
	SEL onExecuteJs;
	//SEL onSetViewHomeUrl;
	SEL onTakePicture;
	SEL onChoosePicture;
	SEL onChooseDateTime;
	SEL onCreateNativeBar;
    SEL onRemoveNativeBar;
    SEL onSwitchTab;
	//SEL onSetViewOptionsUrl;
	SEL onShowPopup;
	SEL onVibrate;
	SEL onPlayFile;
	SEL onSysCall;
	SEL onMapLocation;
	SEL onCreateMap;
	SEL onActiveTab;
}

// callbacks
@property (assign) id  actionTarget;
@property (assign) SEL onStartSuccess;
//@property (assign) SEL onStartFailure;
@property (assign) SEL onRefreshView;
@property (assign) SEL onNavigateTo;
@property (assign) SEL onExecuteJs;
//@property (assign) SEL onSetViewHomeUrl;
@property (assign) SEL onTakePicture;
@property (assign) SEL onChoosePicture;
@property (assign) SEL onChooseDateTime;
@property (assign) SEL onCreateNativeBar;
@property (assign) SEL onRemoveNativeBar;
@property (assign) SEL onSwitchTab;
//@property (assign) SEL onSetViewOptionsUrl;
@property (assign) SEL onShowPopup;
@property (assign) SEL onVibrate;
@property (assign) SEL onPlayFile;
@property (assign) SEL onSysCall;
@property (assign) SEL onMapLocation;
@property (assign) SEL onCreateMap;
@property (assign) SEL onActiveTab;

- (void) start;
//TODO - implement pause and stop operations and graceful termination of the thread...
- (void) stop;
//Sync all sources
//- (void) doSync;
//- (void) doSyncFor:(NSString*)url;

+ (ServerHost *)sharedInstance;

@end