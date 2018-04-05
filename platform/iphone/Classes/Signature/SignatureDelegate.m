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

#import "Rhodes.h"
#import "SignatureDelegate.h"
#import "AppManager.h"
#import "common/RhodesApp.h"
#import "logging/RhoLog.h"
#include "SignatureParam.h"
#include "ruby/ext/rho/rhoruby.h"

SignatureDelegate* ourSD = nil;


@implementation SignatureDelegate

+(id) getSharedInstance
{
    if (ourSD == nil) {
        ourSD = [[SignatureDelegate alloc] init];
    }
    return ourSD;
}


-(id) init
{
    if (self = [super init])
    {
        signatureViewController = nil;
        signatureInlineView = nil;
        parentView = nil;
        prevView = nil;
        imageFormat = 0;
        callbackHolder = nil;
        penColor = 0;
        penWidth = 0;
        bgColor = 0;
        postUrl = nil;

    }
    return self;
}

-(void)setSignatureViewControllerValue:(SignatureViewController *)svc {
	signatureViewController = svc;
    [signatureViewController setPenColor:penColor];
    [signatureViewController setPenWidth:penWidth];
    [signatureViewController setBgColor:bgColor];
}

-(void)setParentView:(UIView*)parent_view {
	parentView = parent_view;
}

-(void)setPrevView:(UIView*)prev_view {
	prevView = prev_view;
}

-(void)setImageFormat:(unsigned int)format {
	imageFormat = format;
}

-(void)setPenColor:(unsigned int)value
{
    penColor = value;
}

-(void)setFileName:(NSString*)fName {
    fileName = fName;
}

-(void)setPenWidth:(float)value
{
    penWidth = value;
}

-(void)setBgColor:(unsigned int)value
{
    bgColor = value;
}

-(void)setCallback:(NSObject<IMethodResult>*)value;
{
    callbackHolder = value;
}



- (void)useImage:(UIImage*)theImage { 
    NSString *folder = [[AppManager getDbPath] stringByAppendingPathComponent:@"/db-files"];

    NSFileManager *fileManager = [NSFileManager defaultManager];
    if (![fileManager fileExistsAtPath:folder]) {
        NSError* error;
        [fileManager createDirectoryAtPath:folder withIntermediateDirectories:YES attributes:nil error:&error];
    }

    NSString *now = [[[NSDate date] descriptionWithLocale:nil]
             stringByReplacingOccurrencesOfString: @":" withString: @"."];
    now = [now stringByReplacingOccurrencesOfString: @" " withString: @"_"];
    now = [now stringByReplacingOccurrencesOfString: @"+" withString: @"_"];

    NSString *filename;
    NSString *fullname;
    NSData *pngImage;
    
    if (imageFormat == CF_PNG) {
        filename = [NSString stringWithFormat:@"%@", fileName];
        if (![filename hasSuffix:@".png"]) {
            filename = [filename stringByAppendingString:@".png"];
        }
        //fullname = [folder stringByAppendingPathComponent:filename];
        pngImage = UIImagePNGRepresentation(theImage);
    } else {
        filename = [NSString stringWithFormat:@"%@", fileName];
        if (![filename hasSuffix:@".jpg"]) {
            filename = [filename stringByAppendingString:@".jpg"];
        }
        //fullname = [folder stringByAppendingPathComponent:filename];
        pngImage = UIImageJPEGRepresentation(theImage, 1.0);
    }
    
    NSString *appDirectory = [NSString stringWithUTF8String:rho_native_rhopath()];
    NSString *dbDirectory = [NSString stringWithUTF8String:rho_native_rhodbpath()];
    
    if ((![filename hasPrefix:appDirectory])   && (![filename hasPrefix:dbDirectory])) {
        NSString* userfolder = [NSString stringWithUTF8String:rho_native_rhouserpath()];
        
        filename = [userfolder stringByAppendingPathComponent:filename];
    }
    
    NSError* er = nil;
    //[fileManager removeItemAtPath:fullname error:&er];
    [fileManager removeItemAtPath:filename error:&er];

    int isError = ![pngImage writeToFile:filename atomically:YES];
    
    if (callbackHolder)
    {
        NSMutableDictionary* result;
        
        if (isError) {
            result = [NSMutableDictionary dictionaryWithObjectsAndKeys: @"error", @"status", @"" , @"imageUri", @"Can't write image to the storage.", @"message", nil];
        } else{
            //NSString* extpath = [NSString stringWithFormat:@"db/db-files/%@", filename];
            // imageUri for new commonAPI, signature_uri for legacy support
            result = [NSMutableDictionary dictionaryWithObjectsAndKeys: @"ok", @"status", filename , @"imageUri", filename, @"signature_uri", nil];
        }

        [callbackHolder setResult:result];
        callbackHolder = nil;
    }
    else
    {
        if (postUrl != nil) {
        rho_rhodesapp_callSignatureCallback([postUrl UTF8String], [filename UTF8String],
            isError ? "Can't write image to the storage - no callback defined." : "", 0 );
        }
    }
} 

-(void)doDone:(UIImage*)image {
	[[[[Rhodes sharedInstance] mainView] getMainViewController] dismissViewControllerAnimated:YES completion:nil];
    [self useImage:image]; 
    //[signatureViewController.view removeFromSuperview];
    signatureViewController = nil;
	//[parentView addSubview:prevView];
	//[prevView release];
    //prevView = nil;
	[image release];
}

-(void)doCancel {
	[[[[Rhodes sharedInstance] mainView] getMainViewController] dismissViewControllerAnimated:YES completion:nil];
    if (callbackHolder)
    {
        NSMutableDictionary* result = [NSMutableDictionary dictionaryWithObjectsAndKeys: @"cancel", @"status", @"", @"imageUri", @"User canceled operation.", @"message", nil];
        
        [callbackHolder setResult:result];
        callbackHolder = nil;
    }
    else
    {
        rho_rhodesapp_callSignatureCallback([postUrl UTF8String], "", "", 1);
    }
    //[signatureViewController.view removeFromSuperview];
    [signatureViewController release];
    signatureViewController = nil;
	//[parentView addSubview:prevView];
	//[prevView release];
    prevView = nil;
}

-(void)hideSignatureInlineViewCommand
{
    if (signatureInlineView != nil) {
        [signatureInlineView removeFromSuperview];
        //[signatureInlineView release];
        signatureInlineView = nil;
    }
}

-(void)hideSignatureInlineView
{
    [self performSelectorOnMainThread:@selector(hideSignatureInlineViewCommand) withObject:nil waitUntilDone:NO];
}


-(void)showSignatureInlineViewCommand:(SignatureViewProperties*)properties
{
    [self hideSignatureInlineViewCommand];
    
    CGRect rect;

    rect.origin.x = properties.params.left;
    rect.origin.y = properties.params.top;
    rect.size.width = properties.params.width;
    rect.size.height = properties.params.height;
    
    fileName = properties.params.fileName;
    
    signatureInlineView = [[[SignatureView alloc] initWithFrame:rect] autorelease];

    [signatureInlineView setPenColor:properties.params.penColor];
    [signatureInlineView setPenWidth:properties.params.penWidth];
    [signatureInlineView setBgColor:properties.params.bgColor];
    
    signatureInlineView.opaque = NO;
    signatureInlineView.backgroundColor = [UIColor colorWithWhite:1 alpha:0];


    UIView* v = [[[[Rhodes sharedInstance] mainView] getRhoWebView:-1] containerView];
    
    if (v != nil) {
        [v addSubview:signatureInlineView];
        [v bringSubviewToFront:signatureInlineView];
        [v setNeedsDisplay];
    }
}


-(void)showSignatureInlineView:(SignatureViewProperties*)properties
{
    [self performSelectorOnMainThread:@selector(showSignatureInlineViewCommand:) withObject:properties waitUntilDone:NO];
}

-(void)clearSignatureInlineViewCommand
{
    if (signatureInlineView != nil) {
        [signatureInlineView doClear];
    }
}

-(void)clearSignatureInlineView
{
    [self performSelectorOnMainThread:@selector(clearSignatureInlineViewCommand) withObject:nil waitUntilDone:NO];
}

-(void)captureInlineSignatureCommand
{
    if (signatureInlineView != nil) {
        UIImage* img = [signatureInlineView makeUIImage];
        [self hideSignatureInlineViewCommand];
        [self useImage:img];
    }
}

-(void)captureInlineSignature
{
    [self performSelectorOnMainThread:@selector(captureInlineSignatureCommand) withObject:nil waitUntilDone:NO];
}


@end

static const char* defaultOutput = "image";

void init_signature_param( struct SignatureParam* sigparam )
{
    sigparam->compressionFormat = CF_PNG;
    sigparam->outputFormat = defaultOutput;
    sigparam->border = false;
    sigparam->penColor = 0x000000FF;
    sigparam->penWidth = 3.0;
    sigparam->bgColor = 0xFFFFFFFF;
    sigparam->left = 15;
    sigparam->top = 60;
    sigparam->width = 200;
    sigparam->height = 150;
    sigparam->setFullscreen = false;
    sigparam->fileName = @"signature";
}



void rho_signature_take_ex( id<IMethodResult> callback, struct SignatureParam* sig_params) {
    NSString *url = @"";
    
	Rhodes* rho = [Rhodes sharedInstance];
	SignatureDelegate* deleg = rho.signatureDelegate;
	[deleg setImageFormat:sig_params->compressionFormat];
    [deleg setPenColor:(sig_params->penColor | 0xFF000000)];
    [deleg setPenWidth:sig_params->penWidth];
    [deleg setBgColor:(sig_params->bgColor | 0xFF000000)];
    [deleg setCallback:callback];
    [deleg setFileName:sig_params->fileName];
    [[Rhodes sharedInstance] performSelectorOnMainThread:@selector(takeSignature:)
                                              withObject:url waitUntilDone:NO];
}



void rho_signature_visible_ex(bool b, struct SignatureParam* p)
{
    
    if (!b) {
        SignatureDelegate* deleg = [SignatureDelegate getSharedInstance];
        [deleg hideSignatureInlineView];
        return;
    }
    
    SignatureViewProperties* props = [[SignatureViewProperties alloc] init];
    
    props.params = *(p);
    
	SignatureDelegate* deleg = [SignatureDelegate getSharedInstance];
    
    [deleg setImageFormat:p->compressionFormat];
    
    [deleg showSignatureInlineView:props];
}

void rho_signature_capture(const char* callback_url) 
{
    
	SignatureDelegate* deleg = [SignatureDelegate getSharedInstance]; 
    
    [deleg setPostUrl:[NSString stringWithUTF8String:callback_url]];
        
    [deleg captureInlineSignature];
}

void rho_signature_capture_ex(id<IMethodResult> callback)
{
    
	SignatureDelegate* deleg = [SignatureDelegate getSharedInstance];
    
    [deleg setCallback:callback];
    
    [deleg captureInlineSignature];
}

void rho_signature_clear()
{
	SignatureDelegate* deleg = [SignatureDelegate getSharedInstance]; 
    [deleg clearSignatureInlineView];
    
}
