//
//  RainbowSmallViewManager.m
//  Rainbow
//
//  Created by Dmitry Soldatenkov on 10/15/10.
//  Copyright 2010 __Rhomobile__. All rights reserved.
//




#import "BarcodeViewManager.h"
#import "common/RhoNativeViewManager.h"



@implementation BarcodeViewManager

static int view_id = 0;


+ (void)openView {
	view_id = RhoNativeViewManager::openNativeView("Barcode_View", OPEN_IN_MODAL_FULL_SCREEN_WINDOW, NULL);
}

+ (void)closeView {
	RhoNativeViewManager::closeNativeView(view_id);
}


@end

