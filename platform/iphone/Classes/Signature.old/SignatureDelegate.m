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

-(void)setImageFormat:(NSString*)format {
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
    
    if ([@"png" compare:imageFormat options:NSCaseInsensitiveSearch] == NSOrderedSame) {
	
        filename = [NSString stringWithFormat:@"Image_%@.png", now]; 	
        fullname = [folder stringByAppendingPathComponent:filename];
        pngImage = UIImagePNGRepresentation(theImage);
    }
    else {
        filename = [NSString stringWithFormat:@"Image_%@.jpg", now]; 	
        fullname = [folder stringByAppendingPathComponent:filename];
        pngImage = UIImageJPEGRepresentation(theImage, 1.0);
    }

    int isError = ![pngImage writeToFile:fullname atomically:YES];
    rho_rhodesapp_callSignatureCallback([postUrl UTF8String], [filename UTF8String],
            isError ? "Can't write image to the storage." : "", 0 );
} 

-(void)doDone:(UIImage*)image {
	[[[[Rhodes sharedInstance] mainView] getMainViewController] dismissViewControllerAnimated:YES completion:nil];
    [self useImage:image]; 
    //[signatureViewController.view removeFromSuperview];
    [signatureViewController release];
    signatureViewController = nil;
	//[parentView addSubview:prevView];
	//[prevView release];
    //prevView = nil;
	[image release];
}

-(void)doCancel {
	[[[[Rhodes sharedInstance] mainView] getMainViewController] dismissViewControllerAnimated:YES completion:nil]; 
    rho_rhodesapp_callSignatureCallback([postUrl UTF8String], "", "", 1);
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
    
    rect.origin.x = properties.left;
    rect.origin.y = properties.top;
    rect.size.width = properties.width;
    rect.size.height = properties.height;
    
    signatureInlineView = [[[SignatureView alloc] initWithFrame:rect] autorelease];

    [signatureInlineView setPenColor:properties.penColor];
    [signatureInlineView setPenWidth:properties.penWidth];
    [signatureInlineView setBgColor:properties.bgColor];
    
    signatureInlineView.opaque = NO;
    signatureInlineView.backgroundColor = [UIColor colorWithWhite:1 alpha:0];

    
    //UIWebView* webView = [[[Rhodes sharedInstance] mainView] getWebView:-1];
    //if (webView != nil) {
    //    UIView* v = webView;//[[webView subviews] objectAtIndex:0];
    //    //UIView* v = [[webView subviews] objectAtIndex:0];
    //    [v addSubview:signatureInlineView];
    //    [v bringSubviewToFront:signatureInlineView];
    //    [v setNeedsDisplay];
    //}
    
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

void rho_signature_take(char* callback_url, rho_param* p) {
    
    NSString *url = [NSString stringWithUTF8String:callback_url];
    char* image_format = 0;
    char* penColor = 0;
    char* penWidth = 0;
    char* bgColor = 0;
        
    if (p)
    {
        rho_param* pFF = rho_param_hash_get(p, "imageFormat");
        if ( pFF )
            image_format = pFF->v.string;
        pFF = rho_param_hash_get(p, "penColor");
        if ( pFF )
            penColor = pFF->v.string;
        pFF = rho_param_hash_get(p, "penWidth");
        if ( pFF )
            penWidth = pFF->v.string;
        pFF = rho_param_hash_get(p, "bgColor");
        if ( pFF )
            bgColor = pFF->v.string;
    }
    if (!image_format)
        image_format = "png";
    if (!penColor)
        penColor = "4284874906";
    if (!penWidth)
        penWidth = "3";
    if (!bgColor)
        bgColor = "4294967295";
    
    
    
    NSString* ns_penColor = [NSString stringWithUTF8String:penColor];
    NSString* ns_penWidth = [NSString stringWithUTF8String:penWidth];
    NSString* ns_bgColor = [NSString stringWithUTF8String:bgColor];
    
    NSString *iformat = [NSString stringWithUTF8String:image_format];
	Rhodes* rho = [Rhodes sharedInstance];
	SignatureDelegate* deleg = rho.signatureDelegate; 
	[deleg setImageFormat:iformat];
    [deleg setPenColor:((unsigned int)[ns_penColor longLongValue] | 0xFF000000)];
    [deleg setPenWidth:[ns_penWidth floatValue]];
    [deleg setBgColor:((unsigned int)[ns_bgColor longLongValue] | 0xFF000000)];
    [[Rhodes sharedInstance] performSelectorOnMainThread:@selector(takeSignature:)
                                              withObject:url waitUntilDone:NO];
}


void rho_signature_visible(bool b, rho_param* p)
{
   
    if (!b) {
        SignatureDelegate* deleg = [SignatureDelegate getSharedInstance]; 
        [deleg hideSignatureInlineView]; 
        return;
    }
    
    
    char* image_format = 0;
    char* penColor = 0;
    char* penWidth = 0;
    char* bgColor = 0;
    char* left = 0;
    char* top = 0;
    char* width = 0;
    char* height = 0;
    
    if (p)
    {
        rho_param* pFF = rho_param_hash_get(p, "imageFormat");
        if ( pFF )
            image_format = pFF->v.string;
        pFF = rho_param_hash_get(p, "penColor");
        if ( pFF )
            penColor = pFF->v.string;
        pFF = rho_param_hash_get(p, "penWidth");
        if ( pFF )
            penWidth = pFF->v.string;
        pFF = rho_param_hash_get(p, "bgColor");
        if ( pFF )
            bgColor = pFF->v.string;
        pFF = rho_param_hash_get(p, "left");
        if ( pFF )
            left = pFF->v.string;
        pFF = rho_param_hash_get(p, "top");
        if ( pFF )
            top = pFF->v.string;
        pFF = rho_param_hash_get(p, "width");
        if ( pFF )
            width = pFF->v.string;
        pFF = rho_param_hash_get(p, "height");
        if ( pFF )
            height = pFF->v.string;
    }

    if (!image_format)
        image_format = "png";
    if (!penColor)
        penColor = "4284874906";
    if (!penWidth)
        penWidth = "3";
    if (!bgColor)
        bgColor = "4294967295";
    if (!left)
        left = "0";
    if (!top)
        top = "0";
    if (!width)
        width = "100";
    if (!height)
        height = "100";
    
    NSString *iformat = [NSString stringWithUTF8String:image_format];
    NSString* ns_penColor = [NSString stringWithUTF8String:penColor];
    NSString* ns_penWidth = [NSString stringWithUTF8String:penWidth];
    NSString* ns_bgColor = [NSString stringWithUTF8String:bgColor];
    NSString* ns_left = [NSString stringWithUTF8String:left];
    NSString* ns_top = [NSString stringWithUTF8String:top];
    NSString* ns_width = [NSString stringWithUTF8String:width];
    NSString* ns_height = [NSString stringWithUTF8String:height];
    
    SignatureViewProperties* props = [[SignatureViewProperties alloc] init];

    props.penColor = (unsigned int)[ns_penColor longLongValue];
    props.penWidth = (float)[ns_penWidth floatValue];
    props.bgColor = (unsigned int)[ns_bgColor longLongValue];
    props.left = (int)[ns_left longLongValue];
    props.top = (int)[ns_top longLongValue];
    props.width = (int)[ns_width longLongValue];
    props.height = (int)[ns_height longLongValue];
    
	SignatureDelegate* deleg = [SignatureDelegate getSharedInstance]; 

    [deleg setImageFormat:iformat];

    [deleg showSignatureInlineView:props];
    
}

void rho_signature_capture(const char* callback_url) 
{
    
	SignatureDelegate* deleg = [SignatureDelegate getSharedInstance]; 
    
    [deleg setPostUrl:[NSString stringWithUTF8String:callback_url]];
        
    [deleg captureInlineSignature];
}

void rho_signature_clear() 
{
	SignatureDelegate* deleg = [SignatureDelegate getSharedInstance];
    [deleg clearSignatureInlineView];
    
}
