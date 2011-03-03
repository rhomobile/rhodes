    //
//  TabbedMainView.m
//  rhorunner
//
//  Created by Dmitry Moskalchuk on 26.03.10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "TabbedMainView.h"
#import "SimpleMainView.h"
#import "Rhodes.h"
#import "AppManager.h"

#include "common/RhodesApp.h"
#include "logging/RhoLog.h"

#include "NativeBar.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "TabbedMainView"

@interface RhoTabBarData : NSObject {
@public
    NSString *url;
    BOOL loaded;
    BOOL reload;
}

@property (retain) NSString *url;
@property (assign) BOOL loaded;
@property (assign) BOOL reload;

- (id)init;
- (void)dealloc;

@end

@implementation RhoTabBarData

@synthesize url, loaded, reload;

- (id)init {
    url = nil;
    loaded = NO;
    reload = NO;
    return self;
}

- (void)dealloc {
    [url release];
    [super dealloc];
}

@end



@implementation RhoUITabBarController

@synthesize bkgColor;

- (UIImage*)makeUIImageWithGradient:(int)height colorR:(int)colorR colorG:(int)colorG colorB:(int)colorB {
	// firstly make context
    CGContextRef    context = NULL;
    CGColorSpaceRef colorSpace;
    void *          bitmapData;
    int             bitmapByteCount;
    int             bitmapBytesPerRow;
	
    size_t pixelsWide = 1;
    size_t pixelsHigh = height;
	
    bitmapBytesPerRow   = (pixelsWide) << 2;
    bitmapByteCount     = (bitmapBytesPerRow * pixelsHigh);
	
    colorSpace = CGColorSpaceCreateDeviceRGB();//CGColorSpaceCreateDeviceGray();//(kCGColorSpaceGenericRGB);
	
    bitmapData = malloc( bitmapByteCount );
	
    context = CGBitmapContextCreate (bitmapData,
									 pixelsWide,
									 pixelsHigh,
									 8,      // bits per component
									 bitmapBytesPerRow,
									 colorSpace,
									 kCGImageAlphaNoneSkipLast);
	
    CGColorSpaceRelease( colorSpace );
	CGRect rect;
	
	float color12_r = ((float)colorR)/255.0;
	float color12_g = ((float)colorG)/255.0;
	float color12_b = ((float)colorB)/255.0;

	float color21_r = color12_r;
	float color21_g = color12_g;
	float color21_b = color12_b;
	
	
	float color11_r = color12_r + 0.3;
	float color11_g = color12_g + 0.3;
	float color11_b = color12_b + 0.3;
	if (color11_r > 1) color11_r = 1.0;
	if (color11_g > 1) color11_g = 1.0;
	if (color11_b > 1) color11_b = 1.0;
	
	float color22_r = color21_r - 0.2;
	float color22_g = color21_g - 0.2;
	float color22_b = color21_b - 0.2;
	if (color22_r < 0) color22_r = 0.0;
	if (color22_g < 0) color22_g = 0.0;
	if (color22_b < 0) color22_b = 0.0;

	color12_r = color12_r+0.07;
	color12_g = color12_g+0.07;
	color12_b = color12_b+0.07;
	if (color12_r > 1) color12_r = 1;
	if (color12_g > 1) color12_g = 1;
	if (color12_b > 1) color12_b = 1;
	
	int  y0 =0;
	int  y1 = ((float)height)*0.5;
	int  y2 = ((float)height)*0.5;
	int  y3 = (float)height;
	
	rect.origin.x = 0;
	rect.origin.y = y0;
	rect.size.width = pixelsWide;
	rect.size.height = y1-y0;
	
	
	int i;
			
	for (i = y0; i <= y1; i++ ) {
		float k = ((float)i - y0)/((float)(y1 - y0));
		k = sqrt(k);
		float curR = color22_r*(1-k) + color21_r*k; 
		float curG = color22_g*(1-k) + color21_g*k; 
		float curB = color22_b*(1-k) + color21_b*k; 
		rect.origin.y = i;
		rect.size.height = 1;
		CGContextSetRGBFillColor(context, curR, curG, curB, 1);
		CGContextFillRect(context, rect);

	
	}

	for (i = y2; i <= y3; i++ ) {
		float k = ((float)i - y2)/((float)(y3 - y2));
		k= k*k;
		float curR = color12_r*(1-k) + color11_r*k; 
		float curG = color12_g*(1-k) + color11_g*k; 
		float curB = color12_b*(1-k) + color11_b*k; 
		rect.origin.y = i;
		rect.size.height = 1;
		CGContextSetRGBFillColor(context, curR, curG, curB, 1);
		CGContextFillRect(context, rect);
		
		
	}

	rect.origin.y = y3-1;
	rect.size.height = 1;
	CGContextSetRGBFillColor(context, 0, 0, 0, 1);
	CGContextFillRect(context, rect);
	
	CGImageRef cgImage = CGBitmapContextCreateImage(context);
	
	UIImage* ui = [UIImage imageWithCGImage:cgImage];
	
	CGContextRelease(context);
	
	return ui;
}

- (void)viewDidLoad {
	
    [super viewDidLoad]; 
	
    CGRect frame = self.tabBar.frame;
	frame.origin.x = 0;
	frame.origin.y = 0;
    UIView *v = [[UIView alloc] initWithFrame:frame];
    
	int cR = (self.bkgColor & 0xFF0000) >> 16;
	int cG = (self.bkgColor & 0xFF00) >> 8;
	int cB = (self.bkgColor & 0xFF);
	
	UIImage *i = [self makeUIImageWithGradient:(int)frame.size.height colorR:cR colorG:cG colorB:cB];
	
    UIColor *c = [[UIColor alloc] initWithPatternImage:i];
    v.backgroundColor = c;
	v.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
    [c release];
    [[self tabBar] insertSubview:v atIndex:0];
    [v release];
	
}


@end


@implementation RhoCustomTabBarItem

@synthesize customHighlightedImage;
@synthesize customStdImage;

- (void) dealloc
{
    [customHighlightedImage release]; customHighlightedImage=nil;
    [customStdImage release]; customStdImage=nil;   
    [super dealloc];
}

-(UIImage *) selectedImage
{
    return self.customHighlightedImage;
}

/*
-(UIImage *) unselectedImage
{
    return self.customStdImage;
}
*/
@end



@implementation TabbedMainView

@synthesize tabbar, tabbarData, tabindex;



- (UIImage*)recolorImageWithColor:(UIImage*)image color:(UIColor *)color shadowColor:(UIColor *)shadowColor shadowOffset:(CGSize)shadowOffset shadowBlur:(CGFloat)shadowBlur
{
    CGColorRef cgColor = [color CGColor];
    CGColorRef cgShadowColor = [shadowColor CGColor];

	CGRect contextRect;
    contextRect.origin.x = 0.0f;
    contextRect.origin.y = 0.0f;
    contextRect.size = [image size];
	contextRect.size.width += 6;
	contextRect.size.height += 6;

    UIImage *itemImage = image;
    CGSize itemImageSize = [itemImage size];
    CGPoint itemImagePosition; 
    itemImagePosition.x = ceilf((contextRect.size.width - itemImageSize.width) / 2);
    itemImagePosition.y = ceilf((contextRect.size.height - itemImageSize.height) / 2);
    UIGraphicsBeginImageContext(contextRect.size);
    CGContextRef c = UIGraphicsGetCurrentContext();

    CGContextSetShadowWithColor(c, shadowOffset, shadowBlur, cgShadowColor);

    CGContextBeginTransparencyLayer(c, NULL);
    CGContextScaleCTM(c, 1.0, -1.0);
    CGContextClipToMask(c, CGRectMake(itemImagePosition.x, -itemImagePosition.y, itemImageSize.width, -itemImageSize.height), [itemImage CGImage]);

    CGContextSetFillColorWithColor(c, cgColor);
    contextRect.size.height = -contextRect.size.height;
	
	float* colorComponents = CGColorGetComponents(cgColor);
	float color0R = colorComponents[0];
	float color0G = colorComponents[1];
	float color0B = colorComponents[2];
	
	float color1R = color0R+0.6;
	float color1G = color0G+0.6;
	float color1B = color0B+0.6;
	if (color1R > 1) color1R = 1; 
	if (color1G > 1) color1G = 1; 
	if (color1B > 1) color1B = 1; 
	
	CGFloat components[16] = {	color0R, color0G, color0B, 1.0, 
								color0R, color0G, color0B, 1.0,
								color1R, color1G, color1B, 1.0,
								1.0, 1.0, 1.0, 1.0};
	CGFloat locations[4] = {	0, 0.3, 0.8, 1.0 }; 
	CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();  
	CGGradientRef colorGradient = CGGradientCreateWithColorComponents(colorSpace, components, locations, 4);
	CGContextDrawLinearGradient(c, colorGradient, CGPointMake(0,contextRect.size.height), CGPointMake(0,0), 0);	
	CGContextDrawLinearGradient(c, colorGradient, CGPointMake(contextRect.size.width*0.75,contextRect.size.height), CGPointMake(contextRect.size.width*0.25, 0), 0);	
	
    //CGContextFillRect(c, contextRect);
    CGContextEndTransparencyLayer(c);

    UIImage* resImage = UIGraphicsGetImageFromCurrentImageContext();
	UIGraphicsEndImageContext();
	return resImage;
}




- (id)initWithMainView:(id<RhoMainView>)v parent:(UIWindow*)p bar_info:(NSDictionary*)bar_info {
	[SimpleMainView disableHiddenOnStart];
    CGRect frame = [[v view] frame];
    
	NSString *background_color = nil;
	
	NSDictionary* global_properties = (NSDictionary*)[bar_info objectForKey:NATIVE_BAR_PROPERTIES];
	if (global_properties != nil) {
		background_color = (NSString*)[global_properties objectForKey:NATIVE_BAR_BACKGOUND_COLOR];
	}
	
	if (background_color != nil) {
		RhoUITabBarController* rc = [RhoUITabBarController alloc];
		rc.bkgColor = [background_color intValue];
		rc = [rc initWithNibName:nil bundle:nil];
		tabbar = rc;
	}
	else {
		tabbar = [[UITabBarController alloc] initWithNibName:nil bundle:nil];
    }
	tabbar.delegate = [Rhodes sharedInstance];
    tabbar.view.frame = frame;
    tabbar.selectedIndex = 0;
    //tabbar.tabBar.autoresizingMask = UIViewAutoresizingFlexibleLeftMargin | UIViewAutoresizingFlexibleTopMargin | UIViewAutoresizingFlexibleWidth;
    //tabbar.tabBar.autoresizesSubviews = YES;
	
    
    CGRect childFrame = [[v view] bounds];
	childFrame.origin.x = 0;
	childFrame.origin.y = 0;
    CGRect tbFrame = tabbar.tabBar.frame;
	UIInterfaceOrientation orientation = [[UIApplication sharedApplication] statusBarOrientation];
	//if ((orientation == UIInterfaceOrientationLandscapeLeft)) {
		//childFrame.size.width -= tbFrame.size.height;
    //}
	//else {
		childFrame.size.height -= tbFrame.size.height;
	//}
	
	
	NSArray* items = (NSArray*)[bar_info objectForKey:NATIVE_BAR_ITEMS];
	
    int count = [items count];
    NSMutableArray *views = [NSMutableArray arrayWithCapacity:count];
    NSMutableArray *tabs = [[NSMutableArray alloc] initWithCapacity:count];
    
    NSString *initUrl = nil;
	BOOL is_load_initial_url = YES;
	int tab_to_initial_select = -1;
    
    for (int i = 0; i < count; ++i) {
		
		NSDictionary* item = (NSDictionary*)[items objectAtIndex:i];
		
        NSString *label = (NSString*)[item objectForKey:NATIVE_BAR_ITEM_LABEL];
        NSString *url = (NSString*)[item objectForKey:NATIVE_BAR_ITEM_ACTION];
        NSString *icon = (NSString*)[item objectForKey:NATIVE_BAR_ITEM_ICON];
        NSString *reload = (NSString*)[item objectForKey:NATIVE_BAR_ITEM_RELOAD];
		
		NSString *selected_color = (NSString*)[item objectForKey:NATIVE_BAR_ITEM_SELECTED_COLOR];  
		NSString *disabled = (NSString*)[item objectForKey:NATIVE_BAR_ITEM_DISABLED];
		
		NSString *web_bkg_color = (NSString*)[item objectForKey:NATIVE_BAR_ITEM_WEB_BACKGROUND_COLOR];
		
		NSString *use_current_view_for_tab = (NSString*)[item objectForKey:NATIVE_BAR_ITEM_USE_CURRENT_VIEW_FOR_TAB];
        
		
		BOOL is_use_current_view_for_tab = NO;
		if (use_current_view_for_tab != nil) {
			if ([use_current_view_for_tab caseInsensitiveCompare:@"true"] == NSOrderedSame) {
				is_use_current_view_for_tab = YES;
			}
		}
		
        if (!initUrl)
            initUrl = url;
        
        if (label && url && icon) {
            RhoTabBarData *td = [[RhoTabBarData alloc] init];
            td.url = url;
			td.reload = [reload isEqualToString:@"true"];
			td.loaded = is_use_current_view_for_tab;
			
			if (is_use_current_view_for_tab) {
				td.url = [v currentLocation:-1];
			}
			
			SimpleMainView *subController = nil;
			
			if (is_use_current_view_for_tab) {
				web_bkg_color = nil;
				is_load_initial_url = NO;
				tab_to_initial_select = i;
			}
			
			if (web_bkg_color != nil) {
				int bkgc = [web_bkg_color intValue];
				int cR = (bkgc & 0xFF0000) >> 16;
				int cG = (bkgc & 0xFF00) >> 8;
				int cB = (bkgc & 0xFF);
				UIColor* bc = [UIColor colorWithRed:( ((float)(cR)) / 255.0) green:(((float)(cG)) / 255.0) blue:(((float)(cB)) / 255.0) alpha:1.0];
				
				subController = [[SimpleMainView alloc] initWithParentView:tabbar.view frame:childFrame web_bkg_color:bc];
			}
			else {
				if (is_use_current_view_for_tab) {
					subController = [[SimpleMainView alloc] initWithParentView:tabbar.view frame:childFrame webview:[v detachWebView]];
				}
				else {
					subController = [[SimpleMainView alloc] initWithParentView:tabbar.view frame:childFrame];
				}
			}
            
			
            subController.title = label;
            NSString *imagePath = [[AppManager getApplicationsRootPath] stringByAppendingPathComponent:icon];
			
			
			if (selected_color != nil) {
				
				int sel_col = [selected_color intValue];
				int cR = (sel_col & 0xFF0000) >> 16;
				int cG = (sel_col & 0xFF00) >> 8;
				int cB = (sel_col & 0xFF);
				
				RhoCustomTabBarItem *tabItem = [[RhoCustomTabBarItem alloc]
												initWithTitle:label image:nil tag:0];
			
				tabItem.image = [UIImage imageWithContentsOfFile:imagePath];
				tabItem.badgeValue = nil;

				//subController.tabBarItem.image = [UIImage imageWithContentsOfFile:imagePath];
				//subController.tabBarItem.badgeValue = nil;
				
				UIImage* img = [UIImage imageWithContentsOfFile:imagePath];
				//img = [self recolorImageWithColor:img color:[UIColor colorWithRed:(43.0 / 255.0) green:(143.0 / 255.0) blue:(230.0 / 255.0) alpha:1.0] shadowColor:[UIColor blackColor] shadowOffset:CGSizeMake(0.5f, 1.0f) shadowBlur:3.0f];
				img = [self recolorImageWithColor:img color:[UIColor colorWithRed:( ((float)(cR)) / 255.0) green:(((float)(cG)) / 255.0) blue:(((float)(cB)) / 255.0) alpha:1.0] shadowColor:[UIColor blackColor] shadowOffset:CGSizeMake(0.5f, 1.0f) shadowBlur:3.0f];
				tabItem.customHighlightedImage = img;
				tabItem.customStdImage=nil;       
			
				subController.tabBarItem=tabItem;
				[tabItem release]; 
			}
			else {
				subController.tabBarItem.image = [UIImage imageWithContentsOfFile:imagePath];
				subController.tabBarItem.badgeValue = nil;
			}
			
			if ([disabled isEqualToString:@"true"]) {
				subController.tabBarItem.enabled = NO;
			}
			
			subController.mTabBarCallback = self;
	    //[subController navigateRedirect:url tab:0];
            
            [tabs addObject:td];
            [views addObject:subController];
            
            [td release];
            [subController release];
        }
    }
    tabbar.viewControllers = views;
    tabbar.customizableViewControllers = NO;
    tabbar.view.hidden = NO;
    tabbar.view.autoresizingMask = UIViewAutoresizingFlexibleLeftMargin | UIViewAutoresizingFlexibleTopMargin | UIViewAutoresizingFlexibleWidth;
    tabbar.view.autoresizesSubviews = YES;
    
    self.tabbarData = tabs;
    [tabs release];
    
	if (initUrl && is_load_initial_url) {
        [self navigateRedirect:initUrl tab:0];
    }
	if (tab_to_initial_select >= 0) {
		tabbar.selectedIndex = tab_to_initial_select;
	}
									 
    return self;
}

- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc that aren't in use.
}


- (void)viewDidUnload {
    [super viewDidUnload];
}

- (void)dealloc {
    //[tabbar.view removeFromSuperview];
    [tabbarData release];
	[tabbar release];
	tabbar = nil;
    [super dealloc];
}

- (SimpleMainView*)subView:(int)index {
	if (index == -1) {
		index = [self activeTab];
	}
    return (SimpleMainView*)[tabbar.viewControllers objectAtIndex:index];
}

- (RhoTabBarData*)tabData:(int)index {
	if ((index < 0) || (index >= [tabbarData count])) {
		return nil;
	}
    return (RhoTabBarData*)[tabbarData objectAtIndex:index];
}

// RhoMainView implementation

- (UIView*)view {
    return tabbar.view;
}

- (UIWebView*)detachWebView {
    int n = [self activeTab];
    return [[self subView:n] detachWebView];
}

- (void)loadHTMLString:(NSString *)data {
    [[self subView:[self activeTab]] loadHTMLString:data];
}

- (void)back:(int)index {
    [[self subView:index] back:0];
}

- (void)forward:(int)index {
    [[self subView:index] forward:0];
}

- (void)navigate:(NSString *)url tab:(int)index {
    [[self subView:index] navigate:url tab:0];
}

- (void)navigateRedirect:(NSString *)url tab:(int)index {
    [[self subView:index] navigateRedirect:url tab:0];
}

- (void)reload:(int)index {
    [[self subView:index] reload:0];
}

- (void)executeJs:(NSString*)js tab:(int)index {
    RAWLOG_INFO1("Executing JS: %s", [js UTF8String]);
    [[self subView:index] executeJs:js tab:0];
}

- (NSString*)currentLocation:(int)index {
    return [[self subView:index] currentLocation:0];
}

- (void)switchTab:(int)index {
    tabbar.selectedIndex = index;
	[self onSwitchTab];
}

- (void)onSwitchTab {
	int new_index = tabbar.selectedIndex;
    RhoTabBarData *td = [self tabData:new_index];
	if (td != nil) {
		//if (tabindex != new_index) {
			tabindex = new_index;
			if (!td.loaded || td.reload) {
				const char *s = [td.url UTF8String];
				rho_rhodesapp_load_url(s);
				td.loaded = YES;
			}
			[[[self subView:tabindex] view] setNeedsDisplay];
		//}
	}
}


- (void)onSwitchTab:(int)tab_index {
	int new_index = tab_index;
    RhoTabBarData *td = [self tabData:new_index];
	if (td != nil) {
		tabindex = new_index;
		if (!td.loaded || td.reload) {
			const char *s = [td.url UTF8String];
			rho_rhodesapp_load_url(s);
			td.loaded = YES;
		}
		[[[self subView:tabindex] view] setNeedsDisplay];
	}
}


- (int)activeTab {
    return tabindex;
}

- (UIWebView*)getWebView:(int)tab_index {
	if (tab_index == -1) {
		tab_index = [self activeTab];
	}
	return [[self subView:tab_index] getWebView:-1];
}


- (void)addNavBar:(NSString*)title left:(NSArray*)left right:(NSArray*)right {
    [[self subView:[self activeTab]] addNavBar:title left:left right:right];
}

- (void)removeNavBar {
    [[self subView:[self activeTab]] removeNavBar];
}

-(void)openNativeView:(UIView*)nv_view tab_index:(int)tab_index {
	if (tab_index == -1) {
		tab_index = [self activeTab];
	}
	[[self subView:tab_index] openNativeView:nv_view tab_index:-1];
}

-(void)closeNativeView:(int)tab_index {
	if (tab_index == -1) {
		tab_index = [self activeTab];
	}
	[[self subView:tab_index] closeNativeView:-1];
}



- (void)onViewWillActivate:(RhoViewController*)view {
	int index = -1;
	int i;
	for (i = 0; i < [tabbar.viewControllers count]; i++) {
		if ([tabbar.viewControllers objectAtIndex:i] == view) {
			index = i;
		}
	}
	if ((index >= 0) && (index != tabindex)) {
		[self onSwitchTab:index];
	}
}

@end
