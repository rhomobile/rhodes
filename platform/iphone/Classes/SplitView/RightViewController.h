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
#import "RhoMainView.h"
#import "RhoViewController.h"
#import "SplittedMainView.h"

@interface RightViewController : UIViewController<RhoMainView> {
	SplittedMainView* splittedView;
    NSArray *itemsData;
    int tabindex;	
    NSString* on_change_tab_callback;
}

@property (nonatomic,retain) NSArray *itemsData;
@property (nonatomic,assign) int tabindex;
@property (nonatomic,retain) NSString *on_change_tab_callback;

- (id)initWithItems:(NSDictionary*)bar_info parent:(SplittedMainView*)parent;




//- (UIView*)view;

- (UIWebView*)detachWebView;

- (void)loadHTMLString:(NSString*)data;

- (void)back:(int)index;
- (void)forward:(int)index;
- (void)navigate:(NSString*)url tab:(int)index;
- (void)navigateRedirect:(NSString*)url tab:(int)index;
- (void)reload:(int)index;

- (void)executeJs:(NSString*)js tab:(int)index;

- (NSString*)currentLocation:(int)index;

- (void)switchTab:(int)index;
- (int)activeTab;

- (void)addNavBar:(NSString*)title left:(NSArray*)left right:(NSArray*)right;
- (void)removeNavBar;

- (UIWebView*)getWebView:(int)tab_index;

-(void)callCallback:(int)new_index;

@end
