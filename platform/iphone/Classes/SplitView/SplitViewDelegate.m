/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/

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
