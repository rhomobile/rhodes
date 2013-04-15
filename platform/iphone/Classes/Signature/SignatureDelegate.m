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
        imageFormat = nil;
        callbackHolder = nil;
        penColor = 0;
        penWidth = 0;
        bgColor = 0;

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
    if (![fileManager fileExistsAtPath:folder])
        [fileManager createDirectoryAtPath:folder attributes:nil];

    NSString *now = [[[NSDate date] descriptionWithLocale:nil]
             stringByReplacingOccurrencesOfString: @":" withString: @"."];
    now = [now stringByReplacingOccurrencesOfString: @" " withString: @"_"];
    now = [now stringByReplacingOccurrencesOfString: @"+" withString: @"_"];

    NSString *filename;
    NSString *fullname;
    NSData *pngImage;
    
    if (imageFormat == CF_PNG) {
        filename = [NSString stringWithFormat:@"Image_%@.png", now]; 	
        fullname = [folder stringByAppendingPathComponent:filename];
        pngImage = UIImagePNGRepresentation(theImage);
    } else {
        filename = [NSString stringWithFormat:@"Image_%@.jpg", now]; 	
        fullname = [folder stringByAppendingPathComponent:filename];
        pngImage = UIImageJPEGRepresentation(theImage, 1.0);
    }

    int isError = ![pngImage writeToFile:fullname atomically:YES];
    
    if (callbackHolder)
    {
        NSMutableDictionary* result;
        
        if (isError) {
            result = [NSMutableDictionary dictionaryWithObjectsAndKeys: @"error", @"status", @"" , @"imageUri", @"Can't write image to the storage.", @"message", nil];
        } else{
            NSString* extpath = [NSString stringWithFormat:@"db%%2Fdb-files%%2F%@", filename];
            // imageUri for new commonAPI, signature_uri for legacy support
            result = [NSMutableDictionary dictionaryWithObjectsAndKeys: @"ok", @"status", extpath , @"imageUri", extpath, @"signature_uri", nil];
        }

        [callbackHolder setResult:result];
        callbackHolder = nil;
    }
    else
    {
        rho_rhodesapp_callSignatureCallback([postUrl UTF8String], [filename UTF8String],
            isError ? "Can't write image to the storage." : "", 0 );
    }
} 

-(void)doDone:(UIImage*)image {
	[[[[Rhodes sharedInstance] mainView] getMainViewController] dismissModalViewControllerAnimated:YES]; 
    [self useImage:image]; 
    //[signatureViewController.view removeFromSuperview];
    signatureViewController = nil;
	//[parentView addSubview:prevView];
	//[prevView release];
    //prevView = nil;
	[image release];
}

-(void)doCancel {
	[[[[Rhodes sharedInstance] mainView] getMainViewController] dismissModalViewControllerAnimated:YES];
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
    
    signatureInlineView = [[[SignatureView alloc] initWithFrame:rect] autorelease];

    [signatureInlineView setPenColor:properties.params.penColor];
    [signatureInlineView setPenWidth:properties.params.penWidth];
    [signatureInlineView setBgColor:properties.params.bgColor];
    
    signatureInlineView.opaque = NO;
    signatureInlineView.backgroundColor = [UIColor colorWithWhite:1 alpha:0];

    
    UIWebView* webView = [[[Rhodes sharedInstance] mainView] getWebView:-1];
    if (webView != nil) {
        UIView* v = webView;//[[webView subviews] objectAtIndex:0];
        //UIView* v = [[webView subviews] objectAtIndex:0];
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
    sigparam->left = 0;
    sigparam->top = 0;
    sigparam->width = 100;
    sigparam->height = 100;
    sigparam->setFullscreen = false;
}

void signature_from_param( struct SignatureParam* sig, rho_param* params )
{
    char* _image_format = 0;
    char* _penColor = 0;
    char* _penWidth = 0;
    char* _bgColor = 0;
    char* _left = 0;
    char* _top = 0;
    char* _width = 0;
    char* _height = 0;
    char* _setFullscreen = 0;
    
    if (params)
    {
        rho_param* pFF = rho_param_hash_get(params, "imageFormat");
        if ( pFF )
            _image_format = pFF->v.string;
        pFF = rho_param_hash_get(params, "penColor");
        if ( pFF )
            _penColor = pFF->v.string;
        pFF = rho_param_hash_get(params, "penWidth");
        if ( pFF )
            _penWidth = pFF->v.string;
        pFF = rho_param_hash_get(params, "bgColor");
        if ( pFF )
            _bgColor = pFF->v.string;
        pFF = rho_param_hash_get(params, "left");
        if ( pFF )
            _left = pFF->v.string;
        pFF = rho_param_hash_get(params, "top");
        if ( pFF )
            _top = pFF->v.string;
        pFF = rho_param_hash_get(params, "width");
        if ( pFF )
            _width = pFF->v.string;
        pFF = rho_param_hash_get(params, "height");
        if ( pFF )
            _height = pFF->v.string;
        pFF = rho_param_hash_get(params, "setFullscreen");
        if ( pFF )
            _setFullscreen = pFF->v.string;
    }
    
    if (_image_format) {
        NSString *iformat = [NSString stringWithUTF8String:_image_format];
        if ( [iformat isEqualToString:@"jpg"])
        {
            sig->compressionFormat = CF_JPEG;
        } else if ([iformat isEqualToString:@"png"])
        {
            sig->compressionFormat = CF_PNG;
        } else if ([iformat isEqualToString:@"bmp"])
        {
            sig->compressionFormat = CF_BMP;
        }
    }
    if (_penColor) {
        NSString* ns_penColor = [NSString stringWithUTF8String:_penColor];
        sig->penColor = (unsigned int)[ns_penColor longLongValue];
    }
    if (_penWidth) {
        NSString* ns_penWidth = [NSString stringWithUTF8String:_penWidth];
        sig->penWidth = (float)[ns_penWidth floatValue];
    }
    if (_bgColor) {
        NSString* ns_bgColor = [NSString stringWithUTF8String:_bgColor];
        sig->bgColor = (unsigned int)[ns_bgColor longLongValue];
    }
    if (_left) {
        NSString* ns_left = [NSString stringWithUTF8String:_left];
        sig->left = (int)[ns_left longLongValue];
    }
    if (_top) {
        NSString* ns_top = [NSString stringWithUTF8String:_top];
        sig->top = (int)[ns_top longLongValue];
    }
    if (_width) {
        NSString* ns_width = [NSString stringWithUTF8String:_width];
        sig->width = (int)[ns_width longLongValue];
    }
    if (!_height) {
        NSString* ns_height = [NSString stringWithUTF8String:_height];
        sig->height = (int)[ns_height longLongValue];
    }
}

void rho_signature_take(char* callback_url, rho_param* p) {
    NSString *url = [NSString stringWithUTF8String:callback_url];
    
    struct SignatureParam sig_params;
    
    init_signature_param(&sig_params);
    
    signature_from_param( &sig_params, p );
  
	Rhodes* rho = [Rhodes sharedInstance];
	SignatureDelegate* deleg = rho.signatureDelegate; 
	[deleg setImageFormat:sig_params.compressionFormat];
    [deleg setPenColor:(sig_params.penColor | 0xFF000000)];
    [deleg setPenWidth:sig_params.penWidth];
    [deleg setBgColor:(sig_params.bgColor | 0xFF000000)];
    [[Rhodes sharedInstance] performSelectorOnMainThread:@selector(takeSignature:)
                                              withObject:url waitUntilDone:NO];
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
    [[Rhodes sharedInstance] performSelectorOnMainThread:@selector(takeSignature:)
                                              withObject:url waitUntilDone:NO];
}

void rho_signature_visible(bool b, rho_param* p)
{
    // check for RhoElements :
    if (!rho_is_rho_elements_extension_can_be_used(get_app_build_config_item("motorola_license"))) {
        RAWLOG_ERROR("Rho::SignatureCapture.visible() is unavailable without RhoElements ! For more information go to http://www.motorolasolutions.com/rhoelements");
    }
    
    
    if (!b) {
        SignatureDelegate* deleg = [SignatureDelegate getSharedInstance];
        [deleg hideSignatureInlineView]; 
        return;
    }
    
    SignatureViewProperties* props = [[SignatureViewProperties alloc] init];
    
    struct SignatureParam params;
    
    init_signature_param(&params);
    
    signature_from_param( &params, p );
    
    props.params = params;
    
	SignatureDelegate* deleg = [SignatureDelegate getSharedInstance]; 

    [deleg setImageFormat:params.compressionFormat];

    [deleg showSignatureInlineView:props];
}

void rho_signature_visible_ex(bool b, struct SignatureParam* p)
{
    // check for RhoElements :
    if (!rho_is_rho_elements_extension_can_be_used(get_app_build_config_item("motorola_license"))) {
        RAWLOG_ERROR("Rho::SignatureCapture.visible() is unavailable without RhoElements ! For more information go to http://www.motorolasolutions.com/rhoelements");
    }
    
    
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
    // check for RhoElements :
    if (!rho_is_rho_elements_extension_can_be_used(get_app_build_config_item("motorola_license"))) {
        RAWLOG_ERROR("Rho::SignatureCapture.capture() is unavailable without RhoElements ! For more information go to http://www.motorolasolutions.com/rhoelements");
    }
    
	SignatureDelegate* deleg = [SignatureDelegate getSharedInstance]; 
    
    [deleg setPostUrl:[NSString stringWithUTF8String:callback_url]];
        
    [deleg captureInlineSignature];
}

void rho_signature_capture_ex(id<IMethodResult> callback)
{
    // check for RhoElements :
    if (!rho_is_rho_elements_extension_can_be_used(get_app_build_config_item("motorola_license"))) {
        RAWLOG_ERROR("Rho::SignatureCapture.capture() is unavailable without RhoElements ! For more information go to http://www.motorolasolutions.com/rhoelements");
    }
    
	SignatureDelegate* deleg = [SignatureDelegate getSharedInstance];
    
    [deleg setCallback:callback];
    
    [deleg captureInlineSignature];
}

void rho_signature_clear()
{
    // check for RhoElements :
    if (!rho_is_rho_elements_extension_can_be_used(get_app_build_config_item("motorola_license"))) {
        RAWLOG_ERROR("Rho::SignatureCapture.clear() is unavailable without RhoElements ! For more information go to http://www.motorolasolutions.com/rhoelements");
    }
	SignatureDelegate* deleg = [SignatureDelegate getSharedInstance]; 
    [deleg clearSignatureInlineView];
    
}
