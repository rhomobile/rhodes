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

@implementation SignatureDelegate


-(void)setSignatureViewControllerValue:(SignatureViewController *)svc {
	signatureViewController = svc;
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


- (void)useImage:(UIImage*)theImage { 
    NSString *folder = [[AppManager getApplicationsRootPath] stringByAppendingPathComponent:@"/../db/db-files"];

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
	[parentView addSubview:prevView];
	[prevView release];
	[image release];
}

-(void)doCancel {
    rho_rhodesapp_callSignatureCallback([postUrl UTF8String], "", "", 1);
    [signatureViewController.view removeFromSuperview];
    [signatureViewController release];
	[parentView addSubview:prevView];
	[prevView release];
}



@end

void rho_signature_take(char* callback_url, rho_param* p) {
    NSString *url = [NSString stringWithUTF8String:callback_url];
    char* image_format = 0;
    if (p)
    {
        rho_param* pFF = rho_param_hash_get(p, "imageFormat");
        if ( pFF )
            image_format = pFF->v.string;
    }
    if (!image_format)
        image_format = "";
    
    NSString *iformat = [NSString stringWithUTF8String:image_format];
	Rhodes* rho = [Rhodes sharedInstance];
	SignatureDelegate* deleg = rho.signatureDelegate; 
	[deleg setImageFormat:iformat];
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
