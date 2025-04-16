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

#import "SimpleMainView.h"
#import "AppManager.h"
#import "Rhodes.h"
#import "RhoWebViewFabrique.h"


#include "common/RhoConf.h"
#include "common/RhodesApp.h"
#include "logging/RhoLog.h"

#include "TabbedMainView.h"

#include "NativeBar.h"

#import "Signature/SignatureDelegate.h"

#include "statistic/RhoProfiler.h"

#import "RhoExtManager/RhoExtManagerSingletone.h"


#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "SimpleMainView"



int rho_sys_get_screen_width();
int rho_sys_get_screen_height();

@interface RhoToolbarButtonItemAction : NSObject
{
    NSString *url;
}

@property (nonatomic,copy) NSString *url;

- (id)init:(NSString*)u;
- (void)dealloc;
- (void)onAction:(id)sender;

@end

@implementation RhoToolbarButtonItemAction

@synthesize url;

- (id)init:(NSString*)u {
    self.url = u;
    return self;
}

- (void)dealloc {
    self.url = nil;
    [super dealloc];
}

- (void)onAction:(id)sender {
    const char *s = [url UTF8String];
    rho_rhodesapp_load_url(s);
}

@end

@implementation SimpleMainView

@synthesize rhoWebView, toolbar, navbar, nativeViewType, nativeViewView, mTabBarCallback, urlBasedNativeView, url_after_set_background, isBackgroundSetted, is_url_after_set_background_redirect, thisTabIndex;


static BOOL makeHiddenUntilLoadContent = YES;


+ (void) disableHiddenOnStart {
	makeHiddenUntilLoadContent = NO;
}

+ (void) enableHiddenOnStart {
	makeHiddenUntilLoadContent = YES;
}

-(CGRect)getContentBounds {
	if (nativeViewView != nil) {
		return nativeViewView.bounds;
	}
	return [rhoWebView view].bounds;
}

-(CGRect)getContentRect {
	if (nativeViewView != nil) {
		return nativeViewView.frame;
	}
	return [rhoWebView view].frame;
}

-(void)setContentRect:(CGRect)rect {
	if (nativeViewView != nil) {
		nativeViewView.frame = rect;
		return;
	}
	[rhoWebView view].frame = rect;
}

- (UIBarButtonItem*)makeUIBarButtonWithCustomImage:(UIImage*)image name:(NSString*)name target:(id)target action:(SEL)action colored_icon:(BOOL)colored_icon {
    UIBarButtonItem *btn = nil;
	//load the image
	if (image == nil) {
		image = [UIImage imageNamed:name];
	}
	
	if (!colored_icon) {
		btn = [[UIBarButtonItem alloc] initWithImage:image style:UIBarButtonItemStylePlain target:target action:action];
		return btn;
	}
	
	//create the button and assign the image
	UIButton *button = [UIButton buttonWithType:UIButtonTypeCustom];
	[button setImage:image forState:UIControlStateNormal];
	[button addTarget:target action:action forControlEvents:UIControlEventTouchDown];	
	
	//set the frame of the button to the size of the image (see note below)
	button.frame = CGRectMake(0, 0, image.size.width, image.size.height);
	
	//create a UIBarButtonItem with the button as a custom view
	btn = [[UIBarButtonItem alloc] initWithCustomView:button];	
	
	btn.target = target;
	btn.action = action;
	
	return btn;
}

- (UIBarButtonItem*)newButton:(NSString*)url label:(NSString*)label icon:(NSString*)icon  colored_icon:(BOOL)colored_icon{
    UIImage *img = nil;
    if ([icon length] > 0) {
        NSString *imagePath = [[AppManager getApplicationsRootPath] stringByAppendingPathComponent:icon];
        img = [UIImage imageWithContentsOfFile:imagePath];
    }
    
    UIBarButtonItem *btn = nil;
    
	
    if ([url compare:@"back"] == NSOrderedSame) {
        btn = [self makeUIBarButtonWithCustomImage:img name:@"back_btn.png" target:self action:@selector(goBack:) colored_icon:colored_icon];
    }
    else if ([url compare:@"forward"] == NSOrderedSame) 
	{
		if ( !rho_conf_getBool("jqtouch_mode") )
			btn = [self makeUIBarButtonWithCustomImage:img name:@"forward_btn.png" target:self action:@selector(goForward:) colored_icon:colored_icon];
    }
    else if ([url compare:@"home"] == NSOrderedSame) {
        btn = [self makeUIBarButtonWithCustomImage:img name:@"home_btn.png" target:self action:@selector(goHome:) colored_icon:colored_icon];
    }
    else if ([url compare:@"options"] == NSOrderedSame) {
        btn = [self makeUIBarButtonWithCustomImage:img name:@"gears.png" target:self action:@selector(goOptions:) colored_icon:colored_icon];
    }
    else if ([url compare:@"refresh"] == NSOrderedSame) {
        if (img)
            btn = [self makeUIBarButtonWithCustomImage:img name:nil target:self action:@selector(onRefresh:) colored_icon:colored_icon];
        else
            btn = [[UIBarButtonItem alloc]
                   initWithBarButtonSystemItem:UIBarButtonSystemItemRefresh
                   target:self action:@selector(onRefresh:)];
    }
    else if ([url compare:@"separator"] == NSOrderedSame) {
        btn = [[UIBarButtonItem alloc]
               initWithBarButtonSystemItem:UIBarButtonSystemItemFlexibleSpace
               target:nil action:nil];
    }
    else {
        id action = [[RhoToolbarButtonItemAction alloc] init:url];
        if (img) {
            btn = [self makeUIBarButtonWithCustomImage:img name:nil target:action action:@selector(onAction:) colored_icon:colored_icon];
        }
        else if ([label length] > 0) {
            btn = [[UIBarButtonItem alloc]
                   initWithTitle:label style:UIBarButtonItemStylePlain
                   target:action action:@selector(onAction:)];
        }
    }
    
    return btn;
}



#define SYSTEM_VERSION_GREATER_THAN_OR_EQUAL_TO(v)  ([[[UIDevice currentDevice] systemVersion] compare:v options:NSNumericSearch] != NSOrderedAscending)




- (UIToolbar*)newToolbar:(NSDictionary*)bar_info frame:(CGRect)mainFrame {
    
    UIToolbar *tb = [UIToolbar new];
    if (!(SYSTEM_VERSION_GREATER_THAN_OR_EQUAL_TO(@"7.0"))) {
         tb.barStyle = UIBarStyleBlack;//Opaque;
    }

	NSString *background_color = nil;
    NSString *icon_color = nil;

	NSDictionary* global_properties = (NSDictionary*)[bar_info objectForKey:NATIVE_BAR_PROPERTIES];
	if (global_properties != nil) {
		background_color = (NSString*)[global_properties objectForKey:NATIVE_BAR_BACKGOUND_COLOR];
		icon_color = (NSString*)[global_properties objectForKey:NATIVE_BAR_ICON_COLOR];
	}

#ifdef __IPHONE_7_0
    if (SYSTEM_VERSION_GREATER_THAN_OR_EQUAL_TO(@"7.0")) {
        tb.translucent = NO;
    }
#endif
	
	if (background_color != nil) {
		tb.barStyle = UIBarStyleDefault;
		int c = [background_color intValue];
		int cR = (c & 0xFF0000) >> 16;
		int cG = (c & 0xFF00) >> 8;
		int cB = (c & 0xFF);
#ifdef __IPHONE_7_0
        if (SYSTEM_VERSION_GREATER_THAN_OR_EQUAL_TO(@"7.0")) {
            tb.barTintColor = [UIColor colorWithRed:( ((float)(cR)) / 255.0) green:(((float)(cG)) / 255.0) blue:(((float)(cB)) / 255.0) alpha:1.0];
        }
        else {
            tb.tintColor = [UIColor colorWithRed:( ((float)(cR)) / 255.0) green:(((float)(cG)) / 255.0) blue:(((float)(cB)) / 255.0) alpha:1.0];
        }
#else
		tb.tintColor = [UIColor colorWithRed:( ((float)(cR)) / 255.0) green:(((float)(cG)) / 255.0) blue:(((float)(cB)) / 255.0) alpha:1.0];
#endif
	}
    
	if (icon_color != nil) {
		int c = [icon_color intValue];
		int cR = (c & 0xFF0000) >> 16;
		int cG = (c & 0xFF00) >> 8;
		int cB = (c & 0xFF);
#ifdef __IPHONE_7_0
        if (SYSTEM_VERSION_GREATER_THAN_OR_EQUAL_TO(@"7.0")) {
            tb.tintColor = [UIColor colorWithRed:( ((float)(cR)) / 255.0) green:(((float)(cG)) / 255.0) blue:(((float)(cB)) / 255.0) alpha:1.0];
        }
#endif
	}
	
	
    
    [tb sizeToFit];
    
    CGFloat tbHeight = [tb frame].size.height;
	// hack for do not reduce height of toolbar in Landscape mode
	if (tbHeight < 44) {
		tbHeight = 44;
	}
	
    CGRect tbFrame = CGRectMake(CGRectGetMinX(mainFrame),
                                CGRectGetHeight(mainFrame) - tbHeight,
                                CGRectGetWidth(mainFrame),
                                tbHeight);
    [tb setFrame:tbFrame];
    
    UIBarButtonItem *fixed = [[UIBarButtonItem alloc]
                              initWithBarButtonSystemItem:UIBarButtonSystemItemFixedSpace
                              target:nil action:nil];
    
	NSArray* items = (NSArray*)[bar_info objectForKey:NATIVE_BAR_ITEMS];
	if (items == nil) {
		RAWLOG_ERROR("Illegal arguments for createNewToolbar - array of items not found");
		[tb release];
		[fixed release];
		return nil;
	}

    NSMutableArray *btns = [NSMutableArray arrayWithCapacity:[items count]];
	
    for(int i = 0, lim = (int)[items count]; i < lim; i++) {
		NSDictionary* item = (NSDictionary*)[items objectAtIndex:i];
        
		
		NSString *label = (NSString*)[item objectForKey:NATIVE_BAR_ITEM_LABEL];
        NSString *url = (NSString*)[item objectForKey:NATIVE_BAR_ITEM_ACTION];
        NSString *icon = (NSString*)[item objectForKey:NATIVE_BAR_ITEM_ICON];
		NSString *colored_icon = (NSString*)[item objectForKey:NATIVE_BAR_ITEM_COLORED_ICON];  
        
        if ([url length] == 0) {
            RAWLOG_ERROR("Illegal arguments for createNewToolbar");
            [tb release];
            [fixed release];
            return nil;
        }
        
        UIBarButtonItem *btn = [self newButton:url label:label icon:icon colored_icon:[colored_icon isEqualToString:@"true"]];
        
        if (btn) {
            [btns addObject:fixed];
            [btns addObject:btn];
            [btn release];
        }
    }
    
    [tb setItems:btns];
    
    [fixed release];
    
    tb.hidden = NO;
    tb.userInteractionEnabled = YES;
    tb.autoresizesSubviews = YES;
    tb.autoresizingMask = UIViewAutoresizingFlexibleLeftMargin | UIViewAutoresizingFlexibleTopMargin | UIViewAutoresizingFlexibleWidth;
 	
	//assert([tb retainCount] == 1);
    return tb;
}

- (void)removeToolbar {
    if (toolbar) {
        CGRect wFrame = [self getContentRect];
        CGRect tbFrame = toolbar.frame;
        wFrame.size.height += tbFrame.size.height;
        [self setContentRect:wFrame];
    }
    
    [toolbar removeFromSuperview];
    //assert(!toolbar || [toolbar retainCount] == 1);
    self.toolbar = nil;
}

- (void)addToolbar:(NSDictionary*)bar_info {
    [self removeToolbar];
    //assert(!toolbar);
    
    if (!bar_info)
        return;
    
    CGRect wFrame = [self getContentRect];
	wFrame.size.height += wFrame.origin.y;
	wFrame.origin.y = 0;
    
    toolbar = [self newToolbar:bar_info frame:wFrame];
    //assert([toolbar retainCount] == 1);
    toolbar.tag = RHO_TAG_TOOLBAR;
    UIView* root = self.view;
    [root addSubview:toolbar];
    //assert([toolbar retainCount] == 2);
    
    CGRect tbFrame = toolbar.frame;
	wFrame = [self getContentRect];
    wFrame.size.height -= tbFrame.size.height;
    [self setContentRect:wFrame];
}

- (id<RhoWebView,NSObject>)newRhoWebView:(CGRect)frame {
    id<RhoWebView,NSObject> w = [RhoWebViewFabrique createRhoWebViewWithFrame:frame];
    return w;
}

- (id)init:(UIView*)p rhowebview:(id<RhoWebView,NSObject>)w frame:(CGRect)frame bar_info:(NSDictionary*)bar_info web_bkg_color:(UIColor*)web_bkg_color {
	[self init];
	
	self.mTabBarCallback = nil;
    
    rootFrame = frame;
	
	self.urlBasedNativeView = NO;
    
    self.thisTabIndex = 0;
    
	self.url_after_set_background = nil;
	self.isBackgroundSetted = YES;
	self.is_url_after_set_background_redirect = NO;
	
    UIView* root = self.view;
	if (web_bkg_color != nil) {
		self.view.backgroundColor = web_bkg_color;
	}
    
    //assert(!webView || [webView retainCount] == 2);
    //[webView removeFromSuperview];
    [[w view] removeFromSuperview];
    
    //assert(!webView || [webView retainCount] == 1);
    self.rhoWebView = nil;
    rhoWebView = w;
    //Assertion Failed, fix for ios9 app crashed
    //assert(!webView || [webView retainCount] == 1);
    if (rhoWebView == nil) {
        rhoWebView = [self newRhoWebView:frame];
    }
    [rhoWebView setupDelegate:self];
    //Assertion Failed, fix for ios9 app crashed
    //assert(webView && [webView retainCount] == 1);
    
	CGRect wFrame = frame;
    wFrame.origin.y = 0;
    
#ifdef __IPHONE_7_0
    
    if (SYSTEM_VERSION_GREATER_THAN_OR_EQUAL_TO(@"7.0")) {
        const char fsc[] = "0";
        const char* fs = get_app_build_config_item("iphone_use_new_ios7_status_bar_style");
        if (fs == NULL) {
            fs = fsc;
        }
        
        if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone) {
            CGSize screenSize = [[UIScreen mainScreen] bounds].size;
            if (screenSize.height == 812.0f)
                //NSLog(@"iPhone X");
                fs = "1";
        }
        
        if ((fs[0] == '0')) {
            wFrame.origin.y += 20;
            wFrame.size.height -= 20;
            root.backgroundColor = [UIColor grayColor];
        }
    }
    
#endif
    
    
    [rhoWebView view].frame = wFrame;
    
    [rhoWebView view].autoresizesSubviews = YES;
    [rhoWebView view].autoresizingMask = /*UIViewAutoresizingFlexibleLeftMargin | UIViewAutoresizingFlexibleTopMargin |*/ UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleWidth;

	if (web_bkg_color != nil) {
		[self.rhoWebView view].backgroundColor = web_bkg_color;
		int cr = (int)(CGColorGetComponents([web_bkg_color CGColor])[0] * 255);
		int cg = (int)(CGColorGetComponents([web_bkg_color CGColor])[1] * 255);
		int cb = (int)(CGColorGetComponents([web_bkg_color CGColor])[2] * 255);
		int c = (cr << 16) | (cg << 8) | cb;
        
        //self.isBackgroundSetted = NO;
        //webView.hidden = YES;
		[self setWebBackgroundColor:c];

		//assert([webView retainCount] == 1);
        
        
        UIView* bkg_v = [[UIView alloc] initWithFrame:wFrame];
        bkg_v.backgroundColor = web_bkg_color;
        [root addSubview:bkg_v];
        
        
	}
	else {
		[root addSubview:[rhoWebView view]];
        //Assertion Failed, fix for ios9 app crashed
		//assert([webView retainCount] == 2);
    }
	

    [self addToolbar:bar_info];
    self.navbar = nil;
	nativeView = nil;
	nativeViewType = nil;
	nativeViewView = nil;
    
    // DO NOT REMOVE THIS LINE!!!
    // First call of self.view (when self.view is nil) trigger loadView
    // and viewDidLoad which add all our subviews to the root view
    NSString* s = [NSString stringWithFormat:@"root view: %@", self.view];
    RAWLOG_INFO([s UTF8String]);

    return self;
}

- (void)loadView {
    UIView* root = [[UIView alloc] init];
    root.frame = rootFrame;
	if (makeHiddenUntilLoadContent) {
		root.hidden = YES;
	}
    root.autoresizingMask = UIViewAutoresizingFlexibleLeftMargin | UIViewAutoresizingFlexibleTopMargin | UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleWidth;
    root.autoresizesSubviews = YES;

    self.view = root;

    [root release];
    //assert([root retainCount] == 1);
}

- (void) didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation
{
    UIInterfaceOrientation current_orientation = [[UIApplication sharedApplication] statusBarOrientation];
	if (current_orientation == fromInterfaceOrientation) {
		return;
	}
	int width = rho_sys_get_screen_width();
	int height = rho_sys_get_screen_height();
	// send after rotate message
	//CGRect wFrame = [webView frame];
	int angle = 0;
	switch (fromInterfaceOrientation) {
		case UIInterfaceOrientationPortrait: {
			switch (current_orientation) {
				case UIInterfaceOrientationLandscapeLeft: {
					angle = 90;
				}
					break;
				case UIInterfaceOrientationPortraitUpsideDown: {
					angle = 180;
				}
					break;
				case UIInterfaceOrientationLandscapeRight: {
					angle = -90;
				}
					break;
			}
		}
		break;
		case UIInterfaceOrientationLandscapeLeft: {
			switch (current_orientation) {
				case UIInterfaceOrientationPortrait: {
					angle = -90;
				}
					break;
				case UIInterfaceOrientationPortraitUpsideDown: {
					angle = 90;
				}
					break;
				case UIInterfaceOrientationLandscapeRight: {
					angle = 180;
				}
					break;
			}
		}
		break;
		case UIInterfaceOrientationPortraitUpsideDown: {
			switch (current_orientation) {
				case UIInterfaceOrientationPortrait: {
					angle = 180;
				}
					break;
				case UIInterfaceOrientationLandscapeLeft: {
					angle = -90;
				}
					break;
				case UIInterfaceOrientationLandscapeRight: {
					angle = 90;
				}
					break;
			}
		}
		break;
		case UIInterfaceOrientationLandscapeRight: {
			switch (current_orientation) {
				case UIInterfaceOrientationPortrait: {
					angle = 90;
				}
					break;
				case UIInterfaceOrientationLandscapeLeft: {
					angle = 180;
				}
					break;
				case UIInterfaceOrientationPortraitUpsideDown: {
					angle = -90;
				}
					break;
			}
		}
		break;
	}
	//if ((current_orientation == UIInterfaceOrientationLandscapeLeft) || (current_orientation == UIInterfaceOrientationLandscapeRight)) {
	//	int t = width;
	//	width = height;
	//	height = t;
	//}
	//rho_rhodesapp_callScreenRotationCallback((int)wFrame.size.width, (int)wFrame.size.height, angle);
	rho_rhodesapp_callScreenRotationCallback(width, height, angle);
}

- (id)initWithParentView:(UIView *)p frame:(CGRect)frame bar_info:(NSDictionary*)bar_info {
    return [self init:p rhowebview:nil frame:frame bar_info:bar_info web_bkg_color:nil];
}

- (id)initWithParentView:(UIView *)p frame:(CGRect)frame bar_info:(NSDictionary*)bar_info web_bkg_color:(UIColor*)web_bkg_color {
    return [self init:p rhowebview:nil frame:frame bar_info:bar_info web_bkg_color:web_bkg_color];
}

- (id)initWithParentView:(UIView *)p frame:(CGRect)frame {
    return [self initWithParentView:p frame:frame bar_info:nil];
}

- (id)initWithParentView:(UIView *)p frame:(CGRect)frame web_bkg_color:(UIColor*)web_bkg_color {
    return [self initWithParentView:p frame:frame bar_info:nil web_bkg_color:web_bkg_color];
}

- (id)initWithMainView:(id<RhoMainView>)v parent:(UIWindow*)p {
    return [self initWithMainView:v parent:p bar_info:nil];
}

- (id)initWithParentView:(UIView *)p frame:(CGRect)frame rhowebview:(id<RhoWebView,NSObject>)rhowebview {
    id result = [self init:p rhowebview:rhowebview frame:frame bar_info:nil web_bkg_color:nil];
    return result;
}


- (id)initWithMainView:(id<RhoMainView>)v parent:(UIWindow*)p bar_info:(NSDictionary*)bar_info {
    CGRect frame = [[v view] frame];
	frame.origin.x = 0;
    //UIWebView *w = (UIWebView*)[Rhodes subviewWithTag:RHO_TAG_WEBVIEW ofView:[v view]];
    //UIWebView *w = [v detachWebView];
    
    id<RhoWebView,NSObject> w = [v detachRhoWebView];
    
    id result = [self init:p rhowebview:w frame:frame bar_info:bar_info web_bkg_color:nil];
    return result;
}

- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
}

- (void)viewDidLoad {
    UIView *root = self.view;

    root.userInteractionEnabled = YES;
 	root.autoresizingMask = UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleWidth;
	root.autoresizesSubviews = YES;

	if (nativeView) {
          [root addSubview:[nativeView getView]];
	}
	else if (rhoWebView != nil) {
        [root addSubview:[rhoWebView view]];
        [rhoWebView setupDelegate:self];
	}
	//assert(!nativeView || [nativeView retainCount] == 2);
	//assert(!webView || [webView retainCount] == 2);
    if (toolbar)
        [root addSubview:toolbar];
    //assert(!toolbar || [toolbar retainCount] == 2);
    if (navbar)
        [root addSubview:navbar];
    //assert(!navbar || [navbar retainCount] == 2);
}

- (void)viewDidUnload {
    [super viewDidUnload];
    //assert(!nativeView || [nativeView retainCount] == 1);
    //assert(!webView || [webView retainCount] == 1);
    //assert(!toolbar || [toolbar retainCount] == 1);
    //assert(!navbar || [navbar retainCount] == 1);
}

- (void)dealloc {
    [rhoWebView setupDelegate:nil];
    nativeView = nil;
    nativeViewView = nil;
    self.rhoWebView = nil;
    self.toolbar = nil;
    self.navbar = nil;
    [super dealloc];
}

// Toolbar handlers

- (void)goBack:(id)sender {
    rho_rhodesapp_navigate_back();
}

- (void)goForward:(id)sender {
	if ((nativeViewView != nil) && (self.urlBasedNativeView == YES)) {
		[self restoreWebView];
	}
    [self forward:0];
}

- (void)goHome:(id)sender {
    const char* url = rho_rhodesapp_getstarturl();
    [self navigate:[NSString stringWithUTF8String:url] tab:0];
}

- (void)goOptions:(id)sender {
    const char *url = rho_rhodesapp_getoptionsurl();
    [self navigate:[NSString stringWithUTF8String:url] tab:0];
}

- (void)onRefresh:(id)sender {
    [self reload:0];
}

- (id<RhoWebView,NSObject>)detachRhoWebView {
	[self restoreWebView];
	id<RhoWebView,NSObject> w = [rhoWebView retain];
	[[w view] removeFromSuperview];
    [rhoWebView setupDelegate:nil];
    self.rhoWebView = nil;
    
    //Assertion Failed, fix for ios9 app crashed
    //assert(w && [w retainCount] == 1);
    return w;
}

-(void)setWebBackgroundColor:(int)bkg_color {
	//self.isBackgroundSetted = NO;
	int cR = (bkg_color & 0xFF0000) >> 16;
	int cG = (bkg_color & 0xFF00) >> 8;
	int cB = (bkg_color & 0xFF);
	UIColor* bc = [UIColor colorWithRed:( ((float)(cR)) / 255.0) green:(((float)(cG)) / 255.0) blue:(((float)(cB)) / 255.0) alpha:1.0];
	
	[self.rhoWebView view].backgroundColor = bc;
	//self.view.backgroundColor = bc;
	
	//NSString* datas = [NSString stringWithFormat:@"<body bgcolor=\"#%6X\"></body>", bkg_color];
	//NSString* datas = [NSString stringWithFormat:@"<body><script type=\"text/javascript\">document.body.style.backgroundColor = \"#%6X\";</script></body>", bkg_color];
    
	
	//[self loadHTMLString:datas];
    NSString* jscode = [NSString stringWithFormat:@"document.body.style.backgroundColor = \"#%6X\";", bkg_color];
    [self.rhoWebView stringByEvaluatingJavaScriptFromString:jscode wantAnswer:NO];

	//self.webView.hidden = YES;
}


- (void)loadHTMLString:(NSString *)data {
	[self restoreWebView];
    [rhoWebView stopLoading];
    [rhoWebView loadHTMLString:data baseURL:[NSURL URLWithString:@""]];
}

- (void)back:(int)index {
	//if (nativeViewView != nil) {
	if ((nativeViewView != nil) && (self.urlBasedNativeView == YES)) {
		[self restoreWebView];
	}
	//[webView setNeedsDisplay];
	//}
	//else {
        [[SignatureDelegate getSharedInstance] hideSignatureInlineView];
		[rhoWebView goBack];
	//}
}

- (void)forward:(int)index {
	if ((nativeViewView != nil) && (self.urlBasedNativeView == YES)) {
		[self restoreWebView];
	}
    [rhoWebView goForward];
}

- (NSString*)encodeUrl:(NSString*)url {
    // This decode/encode trick allow to work properly with urls which are already encoded
    // in the same manner as with those which are not. In case if 'url' is already encoded,
    // encodedUrl will be exactly the same as original one whereas if original url was not
    // encoded, encodedUrl will contain correct encoded version
    NSString *decodedUrl = [url stringByRemovingPercentEncoding];
    NSString *encodedUrl = [decodedUrl stringByAddingPercentEncodingWithAllowedCharacters:[NSCharacterSet URLFragmentAllowedCharacterSet]];
    
    // additional check for anchor for file:// prefix
    if ([encodedUrl hasPrefix:@"file://"]) {
        encodedUrl = [encodedUrl stringByReplacingOccurrencesOfString:@"%23" withString:@"#"];
    }
    
    return encodedUrl;
}

-(void)restoreWebView {
	UIView *root = self.view;
	self.nativeViewType = nil;
	if (nativeViewView != nil) {
		CGRect rect = [nativeViewView frame];
		[nativeViewView removeFromSuperview];
		nativeViewView = nil;
		
		[rhoWebView view].frame = rect;
		[root addSubview:[rhoWebView view]];
        
        //[[rhoWebView view] setNeedsDisplay];
	}
    [rhoWebView setupDelegate:self];
	if (nativeView != nil) {
		[RhoNativeViewManagerOC destroyNativeView:nativeView];
		nativeView = nil;
	}
}


-(void)openNativeView:(UIView*)nv_view tab_index:(int)tab_index {
	nativeViewView = nv_view;
	if (nativeViewView != nil) {
		CGRect rect = [[rhoWebView view] frame];
		[[rhoWebView view] removeFromSuperview];
		nativeViewView.frame = rect;
		nativeViewView.autoresizesSubviews = YES;
		nativeViewView.clipsToBounds = NO;
		nativeViewView.autoresizingMask = UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleWidth;
		[self.view addSubview:nativeViewView];
		self.urlBasedNativeView = NO;
	}
}

-(void)closeNativeView:(int)tab_index {
	[self restoreWebView];
}

-(UIViewController*)getMainViewController {
    return self;
}


- (NSString*)processForNativeView:(NSString*)url {

	//stringWithUTF8String
	NSRange range = [url rangeOfString:@":"];
	NSRange callback_range = [url rangeOfString:@"call_stay_native:"];
	UIView *root = self.view;
	if ((callback_range.location == 0) && (callback_range.length > 0)) {
		NSString* cleared_url = [url substringFromIndex:callback_range.length];
		return cleared_url;
	}
	else {
		if((range.location >= 0) && (range.length > 0)) {
			NSString* protocol = [url substringToIndex:range.location];
			NSString* navto = [url substringFromIndex:(range.location+1)];
			if ([protocol compare:self.nativeViewType] == NSOrderedSame) {
				// just navigate
				if (nativeView != nil) {
					[nativeView navigate:navto];
				}
				return nil;
			}
			else {
				// try to make native view
				id<NativeViewOC,NSObject> nv = [RhoNativeViewManagerOC getNativeView:protocol];
				if (nv != nil) {
					[self restoreWebView];
					nativeView = nv;
					self.nativeViewType = [[NSString alloc] initWithString:protocol] ;
					// replace webView with NativeView
					nativeViewView = [nativeView getView];
					if (nativeViewView != nil) {
						CGRect rect = [[rhoWebView view] frame];
						[[rhoWebView view] removeFromSuperview];
						nativeViewView.frame = rect;
						//w.userInteractionEnabled = YES;
						//w.multipleTouchEnabled = YES;
						nativeViewView.autoresizesSubviews = YES;
						nativeViewView.clipsToBounds = NO;
						//w.delegate = self;
						nativeViewView.autoresizingMask = UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleWidth;
						[root addSubview:nativeViewView];
						self.urlBasedNativeView = YES;
					}
					[nativeView navigate:navto];
					return nil;
				}
				else {
					if ((nativeViewView != nil) && (self.urlBasedNativeView == YES)) {
						[self restoreWebView];
					}
					return url;
				}
			}
		}
		else {
			if ((nativeViewView != nil) && (self.urlBasedNativeView == YES)) {
				[self restoreWebView];
			}
			return url;
		}

	}
}

- (void)loadRequestToWebView:(NSURLRequest*)request {
    [rhoWebView stopLoading];
    [rhoWebView loadRequest:request];
    //[request release];
}

- (void)navigate:(NSString *)url tab:(int)index {
	if (!self.isBackgroundSetted) {
		self.url_after_set_background = url;
		self.is_url_after_set_background_redirect = NO;
		return;
	}
	[SimpleMainView disableHiddenOnStart];
#if defined(RHO_NO_RUBY_API) && defined(RHO_NO_HTTP_SERVER)
    NSString *encodedUrl = url;
#else
    NSString *encodedUrl = [self encodeUrl:url];
#endif
	NSString* cleared_url = [self processForNativeView:encodedUrl];
	if (cleared_url == nil) {
		return;
	}
	else {
#if defined(RHO_NO_RUBY_API) && defined(RHO_NO_HTTP_SERVER)
		NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:[NSURL fileURLWithPath:encodedUrl]];
#else
		NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:[NSURL URLWithString:encodedUrl]];
#endif
        [self performSelectorOnMainThread:@selector(loadRequestToWebView:) withObject:request waitUntilDone:NO];
	}
}

- (void)navigateRedirect:(NSString *)url tab:(int)index {
	if (!self.isBackgroundSetted) {
		self.url_after_set_background = url;
		self.is_url_after_set_background_redirect = YES;
		return;
	}
	[SimpleMainView disableHiddenOnStart];
#if defined(RHO_NO_RUBY_API) && defined(RHO_NO_HTTP_SERVER)
    NSString *encodedUrl = url;
#else
    NSString *encodedUrl = [self encodeUrl:url];
#endif
	NSString* cleared_url = [self processForNativeView:encodedUrl];
	if (cleared_url == nil) {
		return;
	}
	else {
		
        //MOHUS
        //if ([cleared_url hasPrefix:@"http:"]) {
        //    NSURLRequest *request = [[NSMutableURLRequest alloc] initWithURL:[NSURL URLWithString:cleared_url]];
        //    [self performSelectorOnMainThread:@selector(loadRequestToWebView:) withObject:request waitUntilDone:NO];
        //}
        //else {

        NSString *redirect = nil;
        if ([cleared_url hasPrefix:@"file:"] || rho_rhodesapp_is_nodejs_app() || rho_rhodesapp_is_rubynodejs_app()) {
            redirect = cleared_url;
        }
        else {
            NSString* homeurl = [NSString stringWithUTF8String:rho_rhodesapp_gethomeurl()];
            redirect = [NSString stringWithFormat:@"%@/system/redirect_to?url=%@", homeurl, cleared_url];
        }
        
            //[self navigate:redirect tab:index];
#if defined(RHO_NO_RUBY_API) && defined(RHO_NO_HTTP_SERVER)
        NSURLRequest *request = [[NSMutableURLRequest alloc] initWithURL:[NSURL fileURLWithPath:redirect]];
#else
            NSURLRequest *request = [[NSMutableURLRequest alloc] initWithURL:[NSURL URLWithString:redirect]];
#endif
            [self performSelectorOnMainThread:@selector(loadRequestToWebView:) withObject:request waitUntilDone:NO];
        //}
    }
}

- (void)reload:(int)index {
	if (nativeViewView != nil) {
		[nativeViewView setNeedsDisplay];
	}
	else {
        [rhoWebView stopLoading];
		[rhoWebView reload];
	}
}

- (void)executeJs:(NSString*)js tab:(int)index {
	[SimpleMainView disableHiddenOnStart];
	if (self.view.hidden) {
		[[Rhodes sharedInstance] hideSplash];
		self.view.hidden = NO;
		[self.view.superview bringSubviewToFront:self.view];
    }
    RAWLOG_INFO1("Executing JS: %s", [js UTF8String]);
    [rhoWebView stringByEvaluatingJavaScriptFromString:js wantAnswer:NO];
}

- (NSString*)currentLocation:(int)index {
    return [rhoWebView currentLocation];
}

#define CUR_URL_DICT_KEY @"CUR_URL_DICT_KEY"

-(void) get_current_url_command:(NSMutableDictionary*)dict {
    NSString* res = [rhoWebView stringByEvaluatingJavaScriptFromString:@"window.location.href" wantAnswer:YES];
    //NSString* res = [[webView.request mainDocumentURL] absoluteString];
    [dict setValue:res forKey:CUR_URL_DICT_KEY];
}

- (NSString*)get_current_url:(int)index {
    NSMutableDictionary* dict = [NSMutableDictionary dictionaryWithCapacity:1];
    [self performSelectorOnMainThread:@selector(get_current_url_command:) withObject:dict waitUntilDone:YES];
    NSString* res = (NSString*)[dict valueForKey:CUR_URL_DICT_KEY];
    return res;
    //return [webView stringByEvaluatingJavaScriptFromString:@"window.location.href"];
}

- (void)switchTab:(int)index {
    // Nothing
}

- (int)activeTab {
    return 0;
}

- (id<RhoWebView,NSObject>)getRhoWebView:(int)tab_index {
	return rhoWebView;
}


- (void)addNavBar:(UINavigationBar*)navb {
    [self removeNavBar];
    
    navbar = navb;
    //assert([navbar retainCount] == 1);
    RAWLOG_INFO1("navbar retain count: %d", (int)[navbar retainCount]);
	navbar.autoresizingMask = UIViewAutoresizingFlexibleWidth;
	navbar.autoresizesSubviews = YES;
	
	UIView* root = self.view;
    
    CGRect nFrame = navbar.frame;

	CGFloat nvHeight = nFrame.size.height;
	// hack for do not reduce height of navbar in Landscape mode
	if (nvHeight < 44) {
		nvHeight = 44;
	}
    CGRect wFrame = [self getContentRect];

	nFrame.size.height = nvHeight;
#ifdef __IPHONE_7_0
    nFrame.origin.y = wFrame.origin.y;
#endif
    nFrame.size.width = [rhoWebView view].bounds.size.width;
	navbar.frame = nFrame;
	
    wFrame.origin.y += nFrame.size.height;
    wFrame.size.height -= nFrame.size.height;

    [root addSubview:navbar];
    //assert([navbar retainCount] > 1);
    
    [self setContentRect:wFrame];
}

- (void)addNavBar:(NSString*)title left:(NSArray*)left right:(NSArray*)right {
    UINavigationBar *nb = [[UINavigationBar alloc] initWithFrame:CGRectZero];
    nb.tag = RHO_TAG_NAVBAR;
	nb.autoresizesSubviews = YES;
    nb.autoresizingMask = UIViewAutoresizingFlexibleWidth;
    [nb sizeToFit];
    
    UINavigationItem *ni = [[UINavigationItem alloc] initWithTitle:title];
    
    NSArray *btns[] = {left, right};
    for (int i = 0, lim = sizeof(btns)/sizeof(btns[0]); i < lim; ++i) {
        NSArray *btn = btns[i];
        if ([btn count] < 4)
            continue;
        NSString *action = [btn objectAtIndex:0];
        NSString *label = [btn objectAtIndex:1];
        NSString *icon = [btn objectAtIndex:2];
        NSString *colored_icon = [btn objectAtIndex:3];
       
        UIBarButtonItem *button = [self newButton:action label:label icon:icon colored_icon:[colored_icon isEqualToString:@"true"]];
        
        if (btn == left)
            [ni setLeftBarButtonItem:button];
        else
            [ni setRightBarButtonItem:button];
    }
    
    [nb pushNavigationItem:ni animated:NO];
	
	[self addNavBar:nb];
}

- (void)removeNavBar {
    if (navbar) {
        CGRect nFrame = navbar.frame;
        CGRect wFrame = [self getContentRect];
        wFrame.origin.y -= nFrame.size.height;
        wFrame.size.height += nFrame.size.height;
        [self setContentRect:wFrame];
    }

    [navbar removeFromSuperview];
    //assert(!navbar || [navbar retainCount] == 1);
    self.navbar = nil;
}


-(void)setTabBarBadge:(NSString*)badge_text tab_index:(int)tab_index {
    // nothing
}




- (BOOL)shouldStartLoadWithRequest:(id<RhoWebView,NSObject>)webView  request:(NSURLRequest *)request {

    NSURL *url = [request URL];
    if (!url)
        return NO;
    
    const char* curl = [[url absoluteString] UTF8String];
    RAWLOG_INFO1("WebView shouldStartLoadWithRequest( %s )", curl);
    
    
    BOOL res = [[RhoExtManagerSingletone getExtensionManager] onBeforeNavigate:[url absoluteString] tabIndex:thisTabIndex];
    if (res) {
        // one from extension block this URL
        return NO;
    }
    
    BOOL external = NO;
    
    NSString *scheme = url.scheme;
#if defined(RHO_NO_RUBY_API)
    if (![scheme isEqualToString:@"http"] && ![scheme isEqualToString:@"https"] && ![scheme isEqualToString:@"file"])
#else
        if (![scheme isEqualToString:@"http"] && ![scheme isEqualToString:@"https"] && ![scheme isEqualToString:@"rhoctp"] && ![scheme isEqualToString:@"file"])
#endif
            external = YES;
        else {
            NSString *ps = [url query];
            NSArray *parameters = [ps componentsSeparatedByString:@"&"];
            for (int i = 0, lim = [parameters count]; i < lim; ++i) {
                NSString *param = [parameters objectAtIndex:i];
                NSArray *nv = [param componentsSeparatedByString:@"="];
                int size = [nv count];
                if (size == 0 || size > 2)
                    continue;
                NSString *name = [nv objectAtIndex:0];
                NSString *value = nil;
                if (size == 2)
                    value = [nv objectAtIndex:1];

                if ([name isEqualToString:@"rho_open_target"] && [value isEqualToString:@"_blank"]) {
                    external = YES;
                    break;
                }
            }
        }

    if (external) {
        // This is not http url so try to open external application for it
        RAWLOG_INFO1("Open url in external application: %s", [[url absoluteString] UTF8String]);
        [[AppManager instance] openURL:[url absoluteString]];
        return NO;
    }

    // Retrieve cookie for http url
    //NSString *c = [[Rhodes sharedInstance] cookie:[url absoluteString]];
    //if (c && [request isKindOfClass:[NSMutableURLRequest class]]) {
    //    NSMutableURLRequest *r = (NSMutableURLRequest*)request;
    //    [r addValue:c forHTTPHeaderField:@"Cookie"];
    //}
    [[SignatureDelegate getSharedInstance] hideSignatureInlineView];
    return YES;

}

- (void)webViewDidStartLoad:(id<RhoWebView,NSObject>)webView {
    // TODO
    //[self active];
    PROF_START("BROWSER_PAGE");
}

- (void)webViewDidFinishLoad:(id<RhoWebView,NSObject>)webView {
    // Disable default context menu on touch
    [rhoWebView stringByEvaluatingJavaScriptFromString:@"document.documentElement.style.webkitTouchCallout = \"none\";" wantAnswer:NO];
    
    PROF_STOP("BROWSER_PAGE");
    
    // Set empty application cache. Otherwise memory used by UIWebView increased rapidly
    // and finally application got out of memory
    NSURLCache *sharedCache = [[NSURLCache alloc] initWithMemoryCapacity:0 diskCapacity:0 diskPath:nil];
    [NSURLCache setSharedURLCache:sharedCache];
    [sharedCache release];
    
    if (self.view.hidden) {
        [[Rhodes sharedInstance] hideSplash];
        self.view.hidden = NO;
        [self.view.superview bringSubviewToFront:self.view];
    }
    if (self.nativeViewView == nil) {
        if ([[self.rhoWebView view] superview] == nil) {
            [self.view addSubview:[self.rhoWebView view]];
        }
        if ([self.rhoWebView view].hidden) {
            [self.rhoWebView view].hidden = NO;
            [[self.rhoWebView view].superview bringSubviewToFront:[self.rhoWebView view]];
        }
    }
    
    if (!self.isBackgroundSetted) {
        self.isBackgroundSetted = YES;
        if (self.url_after_set_background != nil) {
            if (self.is_url_after_set_background_redirect) {
                [self navigateRedirect:url_after_set_background tab:0];
            }
            else {
                [self navigate:url_after_set_background tab:0];
            }
        }
    }
    self.url_after_set_background = nil;
    
    
    if (rho_rhodesapp_is_rubynodejs_app()) {
        const char* ruby_url_c = rho_rhodesapp_rubyhomeurl();
        NSString* ruby_url = [NSString stringWithUTF8String:ruby_url_c];
        NSString* jscode = [NSString stringWithFormat:@"window['__rho_apiAjaxURL']='%@/system/js_api_entrypoint'", ruby_url];
        [self executeJs:jscode tab:self.thisTabIndex];
    }


    NSString* jscode = [NSString stringWithFormat:@"window['__rhoJsVmID']='%@'", [NSNumber numberWithInt:self.thisTabIndex]];
    //[self executeJs:@"alert('hello')" tab:self.thisTabIndex];
    [self executeJs:jscode tab:self.thisTabIndex];
    
    //[self executeJs:@"alert(window['__rhoJsVmID'])" tab:self.thisTabIndex];
    
    
    
    // TODO
    //    [self inactive];
    //
    //     if ([webView canGoBack]) {
    //     backBtn.enabled = YES;
    //     } else {
    //     backBtn.enabled = NO;
    //     }
    //     if ([webView canGoForward]) {
    //     forwardBtn.enabled = YES;
    //     } else {
    //     forwardBtn.enabled = NO;
    //     }
    //
    //     //NSString* location = [webview stringByEvaluatingJavaScriptFromString:@"location.href"];
    //     //rho_rhodesapp_keeplastvisitedurl( [location cStringUsingEncoding:[NSString defaultCStringEncoding]] );
    //
    //     if ([actionTarget respondsToSelector:@selector(hideSplash)])
    //     [actionTarget performSelectorOnMainThread:@selector(hideSplash) withObject:nil waitUntilDone:NO];
    
}

- (void)didFailLoadWithError:(id<RhoWebView,NSObject>)webView error:(NSError *)error {
    
    PROF_STOP("BROWSER_PAGE");
    
    NSString* info = [error localizedDescription];
    NSString* reason = [error localizedFailureReason];
    RAWLOG_INFO2("WebView FAIL load with error: [%s] , reason: [%s]", [info UTF8String], [reason UTF8String]);
    
    if (self.view.hidden) {
        [[Rhodes sharedInstance] hideSplash];
        self.view.hidden = NO;
        [self.view.superview bringSubviewToFront:self.view];
    }
    if ([[self.rhoWebView view] superview] == nil) {
        [self.view addSubview:[self.rhoWebView view]];
    }
    if ([self.rhoWebView view].hidden) {
        [self.rhoWebView view].hidden = NO;
        [[self.rhoWebView view].superview bringSubviewToFront:[self.rhoWebView view]];
    }
    self.isBackgroundSetted = YES;
    self.url_after_set_background = nil;
    
}





- (void)viewWillAppear:(BOOL)animated {
	if (self.mTabBarCallback != nil) {
		TabbedMainView* tv = (TabbedMainView*)self.mTabBarCallback;
		[tv onViewWillActivate:self];
	}
}

@end
