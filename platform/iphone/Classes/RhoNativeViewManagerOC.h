//
//  RhoNativeViewManagerOC.h
//  rhorunner
//
//  Created by Dmitry Soldatenkov on 8/25/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
//#import <NSMapTable.h>


@protocol NativeViewOC

- (UIView*)getView;
- (void)navigate:(NSString*)url;

@end

@protocol NativeViewFactoryOC 

-(id)getNativeView:(NSString*)viewType;	
-(void)destroyNativeView:(id)nativeView;

@end

@interface RhoNativeViewManagerOC : NSObject {
	NSMutableDictionary* mProviders;
	
}

@property (nonatomic, retain) NSMutableDictionary *mProviders;

- (id)init;
- (void)dealloc;	
	
+ (id)getNativeView:(NSString*)viewType;
+ (void)destroyNativeView:(id)nativeView;


+(void)registerViewType:(NSString*)viewType factory:(id)factory;
+(void)unregisterViewType:(NSString*)viewType;
+(UIWebView*)getWebViewObject:(int)tab_index;

@end
