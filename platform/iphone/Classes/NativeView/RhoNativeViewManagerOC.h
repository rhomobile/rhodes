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

#import <UIKit/UIKit.h>
//#import <NSMapTable.h>


#include "ruby/ext/rho/rhoruby.h"


//#define   OC_OPEN_IN_MODAL_FULL_SCREEN_WINDOW  11111


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
	NSMutableArray* mOpenedViews;

}

@property (nonatomic, retain) NSMutableDictionary *mProviders;
@property (nonatomic, retain) NSMutableArray* mOpenedViews;

- (id)init;
- (void)dealloc;

+ (id)getNativeView:(NSString*)viewType;
+ (void)destroyNativeView:(id)nativeView;


+(void)registerViewType:(NSString*)viewType factory:(id)factory;
+(void)unregisterViewType:(NSString*)viewType;
+(UIWebView*)getWebViewObject:(int)tab_index;

+(int)create_native_view:(NSString*)viewType tab_index:(int)tab_index params:(VALUE)params;
+(void)navigate_native_view:(int)nv_id message:(NSString*)message;
+(void)destroy_native_view:(int)nv_id;
+(void)destroy_native_view_by_nview:(void*)nv_view;

@end


