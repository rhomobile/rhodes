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

#import "BarcodeViewController.h"
#import "BarcodeViewManager.h"

//#import "AppManager.h"
//#import "Rhodes.h"
#import "common/RhodesApp.h"

#include "common/RhoConf.h"
#include "common/RhodesApp.h"
#include "logging/RhoLog.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "BarcodeViewController"

#include "BarcodeCallbackHelper.h"

static BarcodeViewController* bv = nil;



@interface RhoCreateBarcodeViewTask : NSObject {}

+ (void)run:(NSString*)value;

@end

@implementation RhoCreateBarcodeViewTask

static RhoCreateBarcodeViewTask* instance_create = nil;

-(void)runCommand:(NSString*)callback {
    //if (mc) {
    //    [mc close];
    //    mc = nil;
    //}
    BarcodeViewController* bvc = [[BarcodeViewController alloc] initWithCallback:callback];
	bv = bvc;
	[BarcodeViewManager openView];
    
}

+(void) run:(NSString*)value {
	if (instance_create == nil) {
		instance_create = [[RhoCreateBarcodeViewTask alloc] init];
	}
    [instance_create performSelectorOnMainThread:@selector(runCommand:) withObject:value waitUntilDone:NO];
}

@end







@implementation BarcodeViewController


+(void)createBarcodeView:(NSString*) callback {
    [RhoCreateBarcodeViewTask run:callback];
}


- (id)initWithCallback:(NSString*)callback {
	[self initWithRect:CGRectMake(0,0,320,460)];
    callback_url = callback;
	return self;
}



- (id)initWithRect:(CGRect)rect{
	[self init];
	
	self.view.frame = rect;

	//content.backgroundColor = [UIColor redColor];
	//1ontent.backgroundColor = [UIColor groupTableViewBackgroundColor];
	
	toolbar = [[UIToolbar alloc] init];
	toolbar.barStyle = UIBarStyleBlack;
	
	toolbar.autoresizingMask = UIViewAutoresizingFlexibleLeftMargin | UIViewAutoresizingFlexibleTopMargin | UIViewAutoresizingFlexibleWidth;
	toolbar.autoresizesSubviews = YES;
	
	{
		UIBarButtonItem *btn_fixed = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemFixedSpace target:nil action:nil];
		UIBarButtonItem* btn_cancel = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemCancel target:self action:@selector(doCancel:)];
		UIBarButtonItem* btn_space = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemFlexibleSpace target:nil action:nil];
		
		NSMutableArray *btns = [NSMutableArray arrayWithCapacity:6];
		[btns addObject:btn_fixed];
		[btns addObject:btn_cancel];
		[btns addObject:btn_fixed];
		[btns addObject:btn_space];
		
		[btn_fixed release];
		[btn_cancel release];
		[btn_space release];
		
		[toolbar setItems:btns];
		
	}
	
	
	[toolbar sizeToFit];
	CGRect srect = self.view.frame;
	CGRect trect = toolbar.frame;
	srect.size.height -= trect.size.height;
	srect.origin.y = 0;
	trect.origin.x = 0;
	trect.origin.y = srect.origin.y+srect.size.height;
	trect.size.width = srect.size.width;
	toolbar.frame = trect;
	toolbar.autoresizingMask = UIViewAutoresizingFlexibleLeftMargin | UIViewAutoresizingFlexibleTopMargin | UIViewAutoresizingFlexibleWidth;
	
	CGRect rrect = srect;
    srect.size.height -= 40;
    rrect.size.height = 40;
    rrect.origin.y += srect.size.height;
    
    resultText = [[UITextView alloc] initWithFrame:rrect];
	resultText.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleLeftMargin | UIViewAutoresizingFlexibleTopMargin;
	resultText.autoresizesSubviews = YES;
	resultText.frame = rrect;
    resultText.textAlignment = UITextAlignmentCenter;
    resultText.font = [resultText.font fontWithSize:22];
    
    readerView = [[ZBarReaderView alloc] initWithImageScanner:[[ZBarImageScanner alloc] init]];//initWithFrame:srect];//CGRectZero];
    
	readerView.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
	readerView.autoresizesSubviews = YES;
	readerView.frame = srect;

	//signatureView = [[SignatureView alloc] initWithFrame:CGRectZero];
	//signatureView.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
	//signatureView.autoresizesSubviews = YES;
	//signatureView.frame = srect;
	
	[self.view addSubview:readerView];
	[self.view addSubview:resultText];
	[self.view addSubview:toolbar];

	//[readerView start];
    
    // the delegate receives decode results
    readerView.readerDelegate = self;
    
    // you can use this to support the simulator
    if(TARGET_IPHONE_SIMULATOR) {
        cameraSim = [[ZBarCameraSimulator alloc]
                     initWithViewController: self];
        cameraSim.readerView = readerView;
    }
    
    [self.view layoutSubviews];
    
    
	return self;

}


// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
	UIView* content = [[UIView alloc] initWithFrame:CGRectZero];
	content.autoresizesSubviews = YES;

	self.view = content;
	[content release];
    
    
    
    
}

- (void)doDone:(id)sender {
	if (callback_url == nil) {
		return;
	}
    
    Barcode_executeRhoCallBackWithParams(callback_url, nil,0,
                                         @"status", @"ok",
                                         @"barcode", resultText.text,
                                         nil, nil, nil, nil);
                                         
    
    /*
	NSString* strBody = @"";
	strBody = [strBody stringByAppendingString:@"&rho_callback=1&status=ok"];
	strBody = [strBody stringByAppendingString:@"&barcode="];
	strBody = [strBody stringByAppendingString:resultText.text];
    char* norm_url = rho_http_normalizeurl([callback_url UTF8String]);
	rho_net_request_with_data(norm_url, [strBody UTF8String]);
    rho_http_free(norm_url);
     */
    [BarcodeViewManager closeView];
}

- (void)doCancel:(id)sender {
	if (callback_url == nil) {
		return;
	}
    Barcode_executeRhoCallBackWithParams(callback_url, nil,0,
                                         @"status", @"cancel",
                                         nil, nil,
                                         nil, nil, nil, nil);

    [BarcodeViewManager closeView];
}



///*
// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
    
    
    
    
	//[self.view 
}
//*/


- (void)didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation {
}

- (void) willRotateToInterfaceOrientation: (UIInterfaceOrientation) orient
                                 duration: (NSTimeInterval) duration
{
    // compensate for view rotation so camera preview is not rotated
    [readerView willRotateToInterfaceOrientation: orient
                                        duration: duration];
}

- (void) viewDidAppear: (BOOL) animated
{
    // run the reader when the view is visible
    [readerView start];
}

- (void) viewWillDisappear: (BOOL) animated
{
    [readerView stop];
}

- (void) readerView: (ZBarReaderView*) view
     didReadSymbols: (ZBarSymbolSet*) syms
          fromImage: (UIImage*) img
{
    // do something useful with results
    for(ZBarSymbol *sym in syms) {
        resultText.text = sym.data;
        
        {
            UIBarButtonItem *btn_fixed = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemFixedSpace target:nil action:nil];
            UIBarButtonItem* btn_cancel = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemCancel target:self action:@selector(doCancel:)];
            UIBarButtonItem* btn_space = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemFlexibleSpace target:nil action:nil];
            UIBarButtonItem* btn_done = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemDone target:self action:@selector(doDone:)];
            
            NSMutableArray *btns = [NSMutableArray arrayWithCapacity:6];
            [btns addObject:btn_fixed];
            [btns addObject:btn_cancel];
            [btns addObject:btn_fixed];
            [btns addObject:btn_space];
            [btns addObject:btn_done];
            
            [btn_fixed release];
            [btn_cancel release];
            [btn_space release];
            [btn_done release];
            
            [toolbar setItems:btns];
            
        }
        
        
        
        
        break;
    }
}


+(UIView*)getCurrentView
{
    return bv.view;
}



- (void)didReceiveMemoryWarning {
	// Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
	
	// Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
	// Release any retained subviews of the main view.
	// e.g. self.myOutlet = nil;
}


- (void)dealloc {
	[readerView removeFromSuperview];
	[toolbar removeFromSuperview];
    [resultText removeFromSuperview];
	[readerView release];
	[toolbar release];
    [resultText release];
    [super dealloc];
}



@end
