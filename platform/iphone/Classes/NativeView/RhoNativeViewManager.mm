/*
 *  RhoNativeViewManager.mm
 *  rhorunner
 *
 *  Created by Dmitry Soldatenkov on 8/25/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#import <UIKit/UIKit.h>
#import <strings.h>

#import "RhoNativeViewManagerOC.h"
#import "RhoNativeViewManager.h" 
#import "Rhodes.h"
#include "logging/RhoLog.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "RhoNativeViewManager"


static int currentID = 1;

@interface RhoNativeViewItem : NSObject {
@public
    NSString *type_name;
	NativeView* nv_view;
	NativeViewFactory* nv_factory;
	int tab_indes;
    int nv_id;
	VALUE start_params;
    NSString *message;
}

@property (retain) NSString *type_name;
@property (assign) NativeView* nv_view;
@property (assign) NativeViewFactory* nv_factory;
@property (assign) int tab_index;
@property (assign) int nv_id;
@property (assign) VALUE start_params;
@property (retain) NSString *message;

- (id)init;
- (void)dealloc;

@end

@implementation RhoNativeViewItem

@synthesize type_name, nv_view, nv_factory, tab_index, nv_id, start_params, message;

- (id)init {
    type_name = nil;
	nv_view = NULL;
	nv_factory = NULL;
	tab_index = 0;
	nv_id = currentID++;
	start_params = NULL;
	message = nil;
    return self;
}

- (void)dealloc {
    [type_name release];
    [super dealloc];
}

@end



	
@interface NativeViewOCImpl:NSObject<NativeViewOC> {
	NativeView* mNativeView;
	id mFactory;
}

//@property (nonatomic, copy) id mFactory;

- (id)init;
- (void)dealloc;
- (UIView*)getView;
- (void)navigate:(NSString*)url;
- (void)setNativeView:(NativeView*)nativeView;
- (NativeView*)getNativeView;
- (void)setFactory:(id)factory;
- (id)getFactory;
+ (NativeViewOCImpl*)makeNativeView:(NativeView*)nativeView;

@end

@implementation NativeViewOCImpl

//@synthesize mFactory;

- (id)init {
    self = [super init];
    if (self) {
    }
    return self;
}

- (void)dealloc {
    [super dealloc];
}

- (UIView*)getView {
	return ((UIView*)mNativeView->getView());
}
- (void)navigate:(NSString*)url {
	mNativeView->navigate([url UTF8String]);
}

- (void)setNativeView:(NativeView*)nativeView {
	mNativeView = nativeView;
}

- (void)setFactory:(id)factory {
	mFactory = factory;
}

- (id)getFactory {
	return mFactory;
}


- (NativeView*)getNativeView {
	return mNativeView;
}

+ (NativeViewOCImpl*)makeNativeView:(NativeView*)nativeView factory:(id)factory  {
	NativeViewOCImpl* nv = [[NativeViewOCImpl alloc] init];
	[nv setNativeView:nativeView];
	[nv setFactory:factory];
	return nv;
}


@end

@interface NativeViewFactoryOCImpl : NSObject<NativeViewFactoryOC> {

	NativeViewFactory* mFactory;
}


- (id)init;
- (void)dealloc;
-(id)getNativeView:(NSString*)viewType;
-(void)destroyNativeView:(id)nativeView;
-(void)setFactory:(NativeViewFactory*)factory;
-(NativeViewFactory*)getFactory;
+(NativeViewFactoryOCImpl*)makeFactory:(NativeViewFactory*)factory;

@end


@implementation NativeViewFactoryOCImpl

- (id)init {
    self = [super init];
    if (self) {
    }
    return self;
}

- (void)dealloc {
    [super dealloc];
}

-(id)getNativeView:(NSString*)viewType {
	NativeView* nv = mFactory->getNativeView([viewType UTF8String]);
	if (nv != NULL) {
		NativeViewOCImpl* nvoc = [NativeViewOCImpl makeNativeView:nv factory:self];
		return nvoc;
	}
	return nil;
}

-(void)destroyNativeView:(id)nativeView {
	mFactory->destroyNativeView([nativeView getNativeView]);
}

-(void)setFactory:(NativeViewFactory*)factory {
	mFactory = factory;
}

-(NativeViewFactory*)getFactory {
	return mFactory;
}


+(NativeViewFactoryOCImpl*)makeFactory:(NativeViewFactory*)factory {
	NativeViewFactoryOCImpl* f = [[NativeViewFactoryOCImpl alloc] init];
	[f setFactory:factory];
	return f;
}
@end


@implementation RhoNativeViewManagerOC

@synthesize mProviders, mOpenedViews;

static RhoNativeViewManagerOC *instance = NULL;

+ (RhoNativeViewManagerOC*)sharedInstance {
	if (instance == NULL) {
		instance = [[RhoNativeViewManagerOC alloc] init];
	}
    return instance;
}

- (id)init {
    self = [super init];
    if (self) {
		mProviders = [[NSMutableDictionary alloc] initWithCapacity:0];
		mOpenedViews = [[NSMutableArray alloc] initWithCapacity:0];
    }
    return self;
}

- (void)dealloc {
    [super dealloc];
}

+ (id)getNativeView:(NSString*)viewType {
	RhoNativeViewManagerOC* sharedInstance = [RhoNativeViewManagerOC sharedInstance];
	NativeViewFactoryOCImpl* nvf = [[sharedInstance mProviders] objectForKey:viewType];
	if (nvf != nil) {
		return [nvf getNativeView:viewType];
	}
	return nil;
}

+ (void)destroyNativeView:(id)nativeView {
	NativeViewOCImpl* v = nativeView;
	[[v getFactory] destroyNativeView:v];
}

+(void)registerViewType:(NSString*)viewType factory:(id)factory {
	RhoNativeViewManagerOC* sharedInstance = [RhoNativeViewManagerOC sharedInstance];
	[sharedInstance.mProviders setObject:factory forKey:viewType];
}

+(void)unregisterViewType:(NSString*)viewType {
	RhoNativeViewManagerOC* sharedInstance = [RhoNativeViewManagerOC sharedInstance];
	[sharedInstance.mProviders removeObjectForKey:viewType];
}

+(UIWebView*)getWebViewObject:(int)tab_index {
	return [[[Rhodes sharedInstance] mainView] getWebView:tab_index];
}



-(void)create_native_view_command:(RhoNativeViewItem*)item {
	RhoNativeViewManagerOC* sharedInstance = [RhoNativeViewManagerOC sharedInstance];
	
	NativeViewOCImpl* nvoc = (NativeViewOCImpl*)[RhoNativeViewManagerOC getNativeView:item.type_name];
	if (nvoc == nil) {
		RAWLOG_ERROR("NativeViewManager do not found registered NativeView type !");
		return;
	}
	NativeView* nv = [nvoc getNativeView];
	if (nv == NULL) {
		RAWLOG_ERROR("NativeViewManager do not get NativeView !");
		return;
	}
	
	item.nv_view = nv;
	item.nv_factory = NULL;
	NativeViewFactoryOCImpl* nvf = [[sharedInstance mProviders] objectForKey:item.type_name];
	if (nvf != nil) {
		item.nv_factory = [nvf getFactory];
	}
	
	
	UIView* v = ((UIView*)item.nv_view->createView(item.start_params));
	if (!rho_ruby_is_NIL(item.start_params)) {
		rho_ruby_releaseValue(item.start_params);
	}
	if (v == nil) {
		RAWLOG_ERROR("NativeView do not make UIView* object !");
		return;
	}
	Rhodes* rho = [Rhodes sharedInstance];
	if (item.tab_index == OPEN_IN_MODAL_FULL_SCREEN_WINDOW) {
		[rho openFullScreenNativeView:v];
	}
	else {
		[[rho mainView] openNativeView:v tab_index:item.tab_index];		
	}
}

+(int)create_native_view:(NSString*)viewType tab_index:(int)tab_index params:(VALUE)params {
	RhoNativeViewManagerOC* sharedInstance = [RhoNativeViewManagerOC sharedInstance];

	RhoNativeViewItem* item = [[RhoNativeViewItem alloc] init];
	
	item.type_name = viewType;
	item.tab_index = tab_index;
	item.start_params = params;
	
	if (!rho_ruby_is_NIL(item.start_params)) {
		rho_ruby_holdValue(item.start_params);
	}
	[sharedInstance.mOpenedViews addObject:item];

	[sharedInstance performSelectorOnMainThread:@selector(create_native_view_command:) withObject:item waitUntilDone:NO];
	
	return item.nv_id;
}


-(RhoNativeViewItem*)found_opened_view_by_id:(int)nv_id {
	int i;
	for (i = 0; i < [self.mOpenedViews count]; i++) {
		RhoNativeViewItem* item = (RhoNativeViewItem*)[self.mOpenedViews objectAtIndex:i];
		if (item.nv_id == nv_id) {
			return item;
		}
	}
	return nil;
}

-(RhoNativeViewItem*)found_opened_view_by_nview:(NativeView*)nv_view {
	int i;
	for (i = 0; i < [self.mOpenedViews count]; i++) {
		RhoNativeViewItem* item = (RhoNativeViewItem*)[self.mOpenedViews objectAtIndex:i];
		if (item.nv_view == nv_view) {
			return item;
		}
	}
	return nil;
}


-(void)navigate_native_view_command:(RhoNativeViewItem*)item {
	item.nv_view->navigate([item.message UTF8String]);
}

+(void)navigate_native_view:(int)nv_id message:(NSString*)message {
	RhoNativeViewManagerOC* sharedInstance = [RhoNativeViewManagerOC sharedInstance];
	RhoNativeViewItem* item = [sharedInstance found_opened_view_by_id:nv_id];
	if (item != nil) {
		item.message = message;
		[sharedInstance performSelectorOnMainThread:@selector(navigate_native_view_command:) withObject:item waitUntilDone:NO];
	}
}

-(void)destroy_native_view_command:(RhoNativeViewItem*)item {
	Rhodes* rho = [Rhodes sharedInstance];
	if (item.tab_index == OPEN_IN_MODAL_FULL_SCREEN_WINDOW) {
		[rho closeFullScreenNativeView];
	}
	else {
		[[rho mainView] closeNativeView:item.tab_index];		
	}
	item.nv_factory->destroyNativeView(item.nv_view);
	[item release];
}

+(void)destroy_native_view:(int)nv_id {
	RhoNativeViewManagerOC* sharedInstance = [RhoNativeViewManagerOC sharedInstance];
	RhoNativeViewItem* item = [sharedInstance found_opened_view_by_id:nv_id];
	if (item != nil) {
		[item retain];
		[sharedInstance.mOpenedViews removeObject:item];
		[sharedInstance performSelectorOnMainThread:@selector(destroy_native_view_command:) withObject:item waitUntilDone:NO];
	}
}

+(void)destroy_native_view_by_nview:(void*)nv_view {
	RhoNativeViewManagerOC* sharedInstance = [RhoNativeViewManagerOC sharedInstance];
	RhoNativeViewItem* item = [sharedInstance found_opened_view_by_nview:(NativeView*)nv_view];
	if (item != nil) {
		[item retain];
		[sharedInstance.mOpenedViews removeObject:item];
		[sharedInstance performSelectorOnMainThread:@selector(destroy_native_view_command:) withObject:item waitUntilDone:NO];
	}
}



@end




void RhoNativeViewManager::registerViewType(const char* viewType, NativeViewFactory* factory) {
	NativeViewFactoryOCImpl* f = [NativeViewFactoryOCImpl makeFactory:factory];
	NSString* s = [[NSString alloc] initWithUTF8String:viewType];
	[RhoNativeViewManagerOC registerViewType:s factory:f];
}

void RhoNativeViewManager::unregisterViewType(const char* viewType) {
	NSString* s = [[NSString alloc] initWithUTF8String:viewType];
	[RhoNativeViewManagerOC unregisterViewType:s];
}

// that function return native object used for display Web content :
// UIWebView* for iPhone
// jobject for Android - jobect is android.webkit.WebView class type
// HWND for Windows Mobile 
void* RhoNativeViewManager::getWebViewObject(int tab_index) {
	return [RhoNativeViewManagerOC getWebViewObject:tab_index];
}

// destroy native view (opened with URL prefix or in separated full-screen window)
// this function can executed from your native code (from NativeView code, for example)
// instead of this function you can execute destroy() for Ruby NativeView object
void RhoNativeViewManager::destroyNativeView(NativeView* nativeView) {
   //TODO

}



extern "C" int rho_native_view_manager_create_native_view(const char* viewtype, int tab_index, VALUE params) {
	NSString *nsviewtype = [NSString stringWithUTF8String:viewtype];
	return [RhoNativeViewManagerOC create_native_view:nsviewtype tab_index:tab_index params:params];
}

extern "C" void rho_native_view_manager_navigate_native_view(int native_view_id, const char* url) {
	if (url != NULL) {
		NSString *message = [NSString stringWithUTF8String:url];
		[RhoNativeViewManagerOC navigate_native_view:native_view_id message:message];
	}
}

extern "C" void rho_native_view_manager_destroy_native_view(int native_view_id) {
	[RhoNativeViewManagerOC destroy_native_view:native_view_id];
}




