//
//  storeAppDelegate.m
//  store
//
//  Created by Vlad on 8/30/10.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import "storeAppDelegate.h"
#import "RootViewController.h"
#import "RhoConnectEngine.h"

@implementation storeAppDelegate

@synthesize window;
@synthesize navigationController;


#pragma mark -
#pragma mark Application lifecycle

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {    
    
    // Override point for customization after application launch.
    
    // Add the navigation controller's view to the window and display.
    [window addSubview:navigationController.view];
    [window makeKeyAndVisible];

    return YES;
}








- (void)applicationWillResignActive:(UIApplication *)application { 
}

- (void)applicationDidBecomeActive:(UIApplication *)application { 
    /* 
     Restart any tasks that were paused (or not yet started) while the application was inactive. 
     */ 
    if([RhoConnectEngine sharedInstance].syncClient) 
    { 
        if([[RhoConnectEngine sharedInstance].syncClient is_syncing]) 
        { 
            [[RhoConnectEngine sharedInstance].syncClient stop_sync]; 
        } 
        if([RhoConnectEngine sharedInstance].syncClient.threaded_mode == FALSE) 
        { 
            [RhoConnectEngine sharedInstance].syncClient.threaded_mode = TRUE; 
        } 
    } 
}

- (void)applicationDidEnterBackground:(UIApplication *)app 
{ 
    //Check if our iOS version supports multitasking I.E iOS 4 
    if ([[UIDevice currentDevice] respondsToSelector:@selector(isMultitaskingSupported)]) { 
        if ([[UIDevice currentDevice] isMultitaskingSupported]) { //Check if device supports mulitasking 
            UIApplication *application = [UIApplication sharedApplication]; //Get the shared application instance 
            
            __block UIBackgroundTaskIdentifier background_task; //Create a task object 
            
            background_task = [application beginBackgroundTaskWithExpirationHandler: ^ { 
                NSLog(@"$$$ Background task is terminated by System !!!");
                [application endBackgroundTask: background_task]; //Tell the system that we are done with the tasks 
                background_task = UIBackgroundTaskInvalid; //Set the task to be invalid 
                
                //System will be shutting down the app at any point in time now 
            }]; 
            
            
            dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{ 
                
                NSLog(@"Background task started");
                //Perform your tasks that your application requires 
                
                [RhoConnectEngine sharedInstance].syncClient.poll_interval = 0;
                if([[RhoConnectEngine sharedInstance].syncClient is_syncing]) 
                { 
                    [[RhoConnectEngine sharedInstance].syncClient stop_sync]; 
                } 

                [RhoConnectEngine sharedInstance].syncClient.threaded_mode = FALSE; 
                
                RhoConnectNotify* notify = [[RhoConnectEngine sharedInstance].syncClient syncAll]; 
                
                NSLog(@"Background task syncAll finished");
                
                NSString* status = notify.status; 
                NSString* error = notify.error_message; 
                int err_code = notify.error_code; 
                NSString* source = notify.source_name; 
                
                NSLog(@"syncAll NOTIFY, source: '%s', status: '%s' , total_count: %d, processed_count: %d, cumulative_count: %d, error_msg: '%s' , error_code: %d, callback_params: '%s'", 
                      [source cStringUsingEncoding:NSUTF8StringEncoding], 
                      [status cStringUsingEncoding: NSUTF8StringEncoding], 
                      notify.total_count, 
                      notify.processed_count, 
                      notify.cumulative_count, 
                      [error cStringUsingEncoding: NSUTF8StringEncoding], 
                      err_code, 
                      [notify.callback_params cStringUsingEncoding:NSUTF8StringEncoding] 
                      ); 
                
                [application endBackgroundTask: background_task]; //End the task so the system knows that you are done with what you need to perform 
                background_task = UIBackgroundTaskInvalid; //Invalidate the background_task 
            }); 
        } 
    } 
}






















- (void)applicationWillEnterForeground:(UIApplication *)application {
    /*
     Called as part of  transition from the background to the inactive state: here you can undo many of the changes made on entering the background.
     */
}



- (void)applicationWillTerminate:(UIApplication *)application 
{
	[RhoConnectEngine destroy];	
}


#pragma mark -
#pragma mark Memory management

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application {
    /*
     Free up as much memory as possible by purging cached data objects that can be recreated (or reloaded from disk) later.
     */
}


- (void)dealloc {
	[navigationController release];
	[window release];
	[super dealloc];
}


@end

