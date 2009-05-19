//
//  BrowserAppDelegate.m
//  Browser
//
//  Created by adam blum on 9/4/08.
//  Copyright __MyCompanyName__ 2008. All rights reserved.
//
#import "defs.h"
#import "RhoRunnerAppDelegate.h"
#import "WebViewController.h"
#import "AppManager.h"
#import "config.h"

@implementation RhoRunnerAppDelegate

@synthesize window;
@synthesize webViewController;

- (NSString*)normalizeUrl:(NSString*)url {
	if([url hasPrefix:@"http://"]) {
		return url;
	}
	NSString* location = [@"http://localhost:8080" stringByAppendingString:[@"/" stringByAppendingPathComponent:url]];
	return location;
}

- (void)onServerStarted:(NSString*)data {
	printf("Server Started notification is recived\n");
	NSString* location = NULL;
	
	//try to restore previous location
	if ( config_getBool("KeepTrackOfLastVisitedPage") ) {
		char* lastVisitedPage = config_getString("LastVisitedPage");
		if (lastVisitedPage && strlen(lastVisitedPage)>0) {
			location = [NSString stringWithCString:lastVisitedPage
										  encoding:[NSString defaultCStringEncoding]];
		}
		config_freeString(lastVisitedPage);
	} 
	
	//if there is no previous location navigate to the default start page 
	if (!location) {
		location = [self normalizeUrl:(NSString*)data];
	}
	
	[webViewController navigateRedirect:location];
}

- (void)onRefreshView {
	[webViewController refresh];
}

- (void)onNavigateTo:(NSString *)url {
	[webViewController navigateRedirect:url];
}

- (void)onExecuteJs:(NSString *)js {
	[webViewController executeJs:js];
}

- (void)onSetViewHomeUrl:(NSString *)url {
	//[webViewController setViewHomeUrl:[localhost stringByAppendingPathComponent:url]];
	[webViewController setViewHomeUrl:url];
}

-(BOOL)startCameraPickerFromViewController:(UIViewController*)controller 
							 usingDelegate:(id<UINavigationControllerDelegate, UIImagePickerControllerDelegate>)delegateObject 
							 sourceType:(UIImagePickerControllerSourceType)type
{ 
	if ( (![UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypeCamera]) 
		|| (delegateObject == nil) || (controller == nil)) 
		return NO; 
	
	@try {
		UIImagePickerController* picker = [[UIImagePickerController alloc] init]; 
		picker.sourceType = type;
		picker.delegate = delegateObject; 
		picker.allowsImageEditing = YES; 
		if ( type == UIImagePickerControllerSourceTypePhotoLibrary ) {
			[window addSubview:picker.view];
		} else {
			[controller presentModalViewController:picker animated:YES]; 
		}
	} @catch(id theException) {
		NSLog(@"%@", theException);
		return NO;
	}
	
	return YES;
} 

- (void)onTakePicture:(NSString*) url {
	[pickImageDelegate setPostUrl:[self normalizeUrl:url]];
	[self startCameraPickerFromViewController:webViewController 
								usingDelegate:pickImageDelegate 
								sourceType:UIImagePickerControllerSourceTypeCamera];
}

- (void)onChoosePicture:(NSString*) url {
	[pickImageDelegate setPostUrl:[self normalizeUrl:url]];
	[self startCameraPickerFromViewController:webViewController 
								usingDelegate:pickImageDelegate 
								sourceType:UIImagePickerControllerSourceTypePhotoLibrary];
}

- (void)onSetViewOptionsUrl:(NSString *)url {
	//[webViewController setViewOptionsUrl:[localhost stringByAppendingPathComponent:url]];
	[webViewController setViewOptionsUrl:url];
}

- (void)onShowLog {
	if (logViewController!=NULL) {
		[window addSubview:logViewController.view];
		logViewController.view.hidden = NO;
	}
}

- (void)onShowLogOptions {
	if (logOptionsController!=NULL) {
		[window addSubview:logOptionsController.view];
		logOptionsController.view.hidden = NO;
	}
}

- (void)applicationDidFinishLaunching:(UIApplication *)application {
	
	// Log View
	logViewController = [[LogViewController alloc] init];
	logViewController->actionTarget = self;
	logViewController->onShowLogOptions = @selector(onShowLogOptions);

	logOptionsController = [[LogOptionsController alloc] init];
	
	webViewController->actionTarget = self;
	webViewController->onShowLog = @selector(onShowLog);
	
	//Camera delegate
	pickImageDelegate = [[PickImageDelegate alloc] init];
	
    //Create local server and start it
    //serverHost = [[ServerHost alloc] init];
	serverHost = [ServerHost sharedInstance];
	serverHost->actionTarget = self;
	serverHost->onStartSuccess = @selector(onServerStarted:);
	serverHost->onRefreshView = @selector(onRefreshView);
	serverHost->onNavigateTo = @selector(onNavigateTo:);
	serverHost->onExecuteJs = @selector(onExecuteJs:);
	serverHost->onSetViewHomeUrl = @selector(onSetViewHomeUrl:);
	serverHost->onTakePicture = @selector(onTakePicture:);
	serverHost->onChoosePicture = @selector(onChoosePicture:);
	serverHost->onSetViewOptionsUrl = @selector(onSetViewOptionsUrl:);
    [serverHost start];
	
    //Create View
	[window addSubview:webViewController.view];
    [window makeKeyAndVisible];
}

- (void)applicationWillTerminate:(UIApplication *)application {
    DBG(("Runner will terminate\n"));
	//Stop HTTP server host 
    [serverHost stop];
}

- (void)dealloc {
    [serverHost release];
	[webViewController release];
	[window release];
	[pickImageDelegate release];
	[super dealloc];
}

@end
