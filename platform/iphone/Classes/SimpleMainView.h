//
//  SimpleMainView.h
//  rhorunner
//
//  Created by Dmitry Moskalchuk on 07.03.10.
//  Copyright 2010 Rhomobile Inc. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "RhoMainView.h"

@interface SimpleMainView : UIViewController <RhoMainView> {
    UIWindow *window;
    UIWebView *webView;
}

@property (retain) UIWebView *webView;

- (id)initWithParentWindow:(UIWindow *)w andDelegate:(id)delegate;

@end
