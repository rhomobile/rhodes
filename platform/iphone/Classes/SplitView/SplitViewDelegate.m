    //
//  TabbedMainView.m
//  rhorunner
//
//  Created by Dmitry Moskalchuk on 26.03.10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "SplitViewDelegate.h"
#import "SimpleMainView.h"
#import "Rhodes.h"
#import "AppManager.h"

#include "common/RhodesApp.h"
#include "logging/RhoLog.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "SplitViewDelegate"

@implementation SplitViewDelegate


@synthesize rightController;


// the master view controller will be displayed in a popover
- (void)splitViewController:(UISplitViewController*)svc 
		  popoverController:(UIPopoverController*)pc 
  willPresentViewController:(UIViewController *)aViewController {
	
	//empty for the moment
	
}


//the master view controller will be hidden
- (void)splitViewController:(UISplitViewController*)svc 
	 willHideViewController:(UIViewController *)aViewController 
		  withBarButtonItem:(UIBarButtonItem*)barButtonItem 
	   forPopoverController:(UIPopoverController*)pc {
	
	//[barButtonItem setTitle:@"my title"];
    //self.navigationItem.leftBarButtonItem = barButtonItem;	
	
	
	
	
	
	
	/*
	//if we keep master view in portrait mode we do not need
	//a extra toolbar
	if(svc.keepMasterInPortraitMode == YES)
		return;
	
	if(toolBar == nil) {
		
		//set title of master button
		barButtonItem.title = @"Show Master";
		
		//create a toolbar
		toolBar = [[UIToolbar alloc] initWithFrame:CGRectMake(0, 0, 1024, 44)];
		[toolBar setItems:[NSArray arrayWithObject:barButtonItem] animated:YES];
		
	}
	 UINavigationController
	
	//add the toolbar to the details view
	[detailController.view addSubview:toolBar];
	*/
}


//the master view will be shown again
- (void)splitViewController:(UISplitViewController*)svc 
	 willShowViewController:(UIViewController *)aViewController 
  invalidatingBarButtonItem:(UIBarButtonItem *)button {
	
   //self.navigationItem.leftBarButtonItem = nil;	
	
	
	
	//remove the toolbar
	//if(self.toolBar != nil)
	//	[toolBar removeFromSuperview];
}





@end
