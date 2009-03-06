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

@implementation RhoRunnerAppDelegate

@synthesize window;
@synthesize webViewController;

NSString *localhost = @"http://localhost:8080/";

- (NSString*)normalizeUrl:(NSString*)url {
	if([url hasPrefix:@"http://"]) {
		return url;
	}
	return [localhost stringByAppendingPathComponent:url];
}

- (void)onServerStarted:(NSString*)data {
	printf("Server Started notification is recived\n");
	NSString* location = [localhost stringByAppendingPathComponent:(NSString*)data];
	[webViewController navigate:location];
}

- (void)onRefreshView {
	[webViewController refresh];
}

- (void)onNavigateTo:(NSString *)url {
	[webViewController navigate:[self normalizeUrl:url]];
}

- (void)onSetViewHomeUrl:(NSString *)url {
	[webViewController setViewHomeUrl:[localhost stringByAppendingPathComponent:url]];
}

-(BOOL)startCameraPickerFromViewController:(UIViewController*)controller 
							 usingDelegate:(id<UINavigationControllerDelegate, UIImagePickerControllerDelegate>)delegateObject 
							 sourceType:(UIImagePickerControllerSourceType)type
{ 
    if ( (![UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypeCamera]) 
		|| (delegateObject == nil) || (controller == nil)) 
        return NO; 
    UIImagePickerController* picker = [[UIImagePickerController alloc] init]; 
    picker.sourceType = type;
    picker.delegate = delegateObject; 
    picker.allowsImageEditing = YES; 
	if ( type == UIImagePickerControllerSourceTypePhotoLibrary )
		[window addSubview:picker.view];
	else
		[controller presentModalViewController:picker animated:YES]; 
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
	[webViewController setViewOptionsUrl:[localhost stringByAppendingPathComponent:url]];
}

- (void)applicationDidFinishLaunching:(UIApplication *)application {	
	//Camera delegate
	pickImageDelegate = [[PickImageDelegate alloc] init];
	
    //Create local server and start it
    //serverHost = [[ServerHost alloc] init];
	serverHost = [ServerHost sharedInstance];
	serverHost->actionTarget = self;
	serverHost->onStartSuccess = @selector(onServerStarted:);
	serverHost->onRefreshView = @selector(onRefreshView);
	serverHost->onNavigateTo = @selector(onNavigateTo:);
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
