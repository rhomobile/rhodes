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

/*------------------------------------------------------------------------
* (The MIT License)
*
* Copyright (c) 2014-2016 Symbol Technologies Inc.
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
* http://symbol.com
*------------------------------------------------------------------------*/

#import <UIKit/UIKit.h>
//#import "RhoMainView.h"
//#import "RhoViewController.h"
//#import "ZBarSDK/Headers/ZBarSDK/ZBarSDK.h"

#import "api_generator/iphone/IMethodResult.h"
#import <AudioToolbox/AudioToolbox.h>
#include "common/barcode_engine.h"

//#define ZXING 1

#ifndef ZXING
#import "ZBarSDK.h"
#else
#import "ZXingObjC.h"
#endif



#ifdef ZXING
@interface BarcodeViewController : UIViewController <ZXCaptureDelegate> {
    UITextView *resultText;
    NSString* callback_url;
    id<IMethodResult> methodResult;
    
    ZXCapture *capture;
    NSTimer*  timer_;

    UIView *signatureView; 
	UIToolbar *toolbar;

    SystemSoundID mBeep;

    CGAffineTransform _captureSizeTransform;
    CAShapeLayer* boundLayer;
}

@property (nonatomic, strong) ZXCapture *capture;

#else

@interface BarcodeViewController : UIViewController <ZBarReaderViewDelegate> {
	UIView *signatureView; 
	UIToolbar *toolbar;
    NSTimer*  timer;

    ZBarReaderView *readerView;
    UITextView *resultText;
    ZBarCameraSimulator *cameraSim;
    
    NSString* callback_url;
    id<IMethodResult> methodResult;
    
    SystemSoundID mBeep;
}

@property (nonatomic, retain) ZBarReaderView *readerView;
@property (nonatomic, retain) UITextView *resultText;
@property (nonatomic, retain) NSString* callback_url;
#endif


+(void)createBarcodeView:(NSObject*)callback;

- (id)initWithRect:(CGRect)rect;

- (void)loadView;

+(UIView*) getCurrentView;

+ (NSString*) getBeepFilePath;


- (void)onTimerFired:(NSTimer*)theTimer; 

@end
