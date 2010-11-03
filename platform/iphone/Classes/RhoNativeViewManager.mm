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

+(NativeViewFactoryOCImpl*)makeFactory:(NativeViewFactory*)factory {
	NativeViewFactoryOCImpl* f = [[NativeViewFactoryOCImpl alloc] init];
	[f setFactory:factory];
	return f;
}
@end



@implementation RhoNativeViewManagerOC

@synthesize mProviders;

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
	//NSString* mytype = @"rainbow_view";
	//NSComparisonResult cr = [mytype compare:viewType];
	//if (cr != NSOrderedSame) {
	//	return nil;
	//}
	//id<NativeViewOC,NSObject> nv = [[NativeViewOCImpl alloc] init];
	//return nv;
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

