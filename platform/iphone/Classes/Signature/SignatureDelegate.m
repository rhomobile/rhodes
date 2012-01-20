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
#include "ruby/ext/rho/rhoruby.h"


@implementation SignatureDelegate

-(id) init
{
    if (self = [super init])
    {
        signatureViewController = nil;
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
    if (![fileManager fileExistsAtPath:folder])
        [fileManager createDirectoryAtPath:folder attributes:nil];

    NSString *now = [[[NSDate date] descriptionWithLocale:nil]
             stringByReplacingOccurrencesOfString: @":" withString: @"."];
    now = [now stringByReplacingOccurrencesOfString: @" " withString: @"_"];
    now = [now stringByReplacingOccurrencesOfString: @"+" withString: @"_"];

    NSString *filename;
    NSString *fullname;
    NSData *pngImage;
    
    if (imageFormat == @"png") {
	
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
	
    [self useImage:image]; 
    [signatureViewController.view removeFromSuperview];
    [signatureViewController release];
    signatureViewController = nil;
	[parentView addSubview:prevView];
	[prevView release];
    prevView = nil;
	[image release];
}

-(void)doCancel {
    rho_rhodesapp_callSignatureCallback([postUrl UTF8String], "", "", 1);
    [signatureViewController.view removeFromSuperview];
    [signatureViewController release];
    signatureViewController = nil;
	[parentView addSubview:prevView];
	[prevView release];
    prevView = nil;
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
        penColor = "0xFF66009A";
    if (!penWidth)
        penWidth = "3";
    if (!bgColor)
        bgColor = "0xFFFFFFFF";
    
    
    
    NSString* ns_penColor = [NSString stringWithUTF8String:penColor];
    NSString* ns_penWidth = [NSString stringWithUTF8String:penWidth];
    NSString* ns_bgColor = [NSString stringWithUTF8String:bgColor];
    
    NSString *iformat = [NSString stringWithUTF8String:image_format];
	Rhodes* rho = [Rhodes sharedInstance];
	SignatureDelegate* deleg = rho.signatureDelegate; 
	[deleg setImageFormat:iformat];
    [deleg setPenColor:(unsigned int)[ns_penColor longLongValue]];
    [deleg setPenWidth:[ns_penWidth floatValue]];
    [deleg setBgColor:(unsigned int)[ns_bgColor longLongValue]];
    [[Rhodes sharedInstance] performSelectorOnMainThread:@selector(takeSignature:)
                                              withObject:url waitUntilDone:NO];
}

void rho_signature_visible(bool b, rho_param* p)
{
    //TODO: rho_signature_visible
}

void rho_signature_capture(const char* callback_url) 
{
    //TODO: rho_signature_capture
}

void rho_signature_clear() 
{
    //TODO: rho_signature_clear
}
