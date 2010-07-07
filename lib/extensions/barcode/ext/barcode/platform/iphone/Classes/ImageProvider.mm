//
//  ImageProvider.mm
//  rhorunner
//
//  Created by Dmitry Soldatenkov on 7/2/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "ImageProvider.h"
#import <UIKit/UIKit.h>
#import <strings.h>


class IP_Inner_Helper {
public:

static CGContextRef CreateRGBABitmapContext (CGImageRef inImage)
{
    CGContextRef    context = NULL;
    CGColorSpaceRef colorSpace;
    void *          bitmapData;
    int             bitmapByteCount;
    int             bitmapBytesPerRow;
	
	// Get image width, height. We'll use the entire image.
    size_t pixelsWide = CGImageGetWidth(inImage);
    size_t pixelsHigh = CGImageGetHeight(inImage);
	
    // Declare the number of bytes per row. Each pixel in the bitmap in this
    // example is represented by 4 bytes; 8 bits each of red, green, blue, and
    // alpha.
    bitmapBytesPerRow   = (pixelsWide) << 2;
    bitmapByteCount     = (bitmapBytesPerRow * pixelsHigh);
	
    // Use the generic RGB color space.
    colorSpace = CGColorSpaceCreateDeviceRGB();//CGColorSpaceCreateDeviceGray();//(kCGColorSpaceGenericRGB);
    if (colorSpace == NULL)
    {
        fprintf(stderr, "Error allocating color space\n");
        return NULL;
    }
	
    // Allocate memory for image data. This is the destination in memory
    // where any drawing to the bitmap context will be rendered.
    bitmapData = malloc( bitmapByteCount );
    if (bitmapData == NULL) 
    {
        fprintf (stderr, "Memory not allocated!");
        CGColorSpaceRelease( colorSpace );
        return NULL;
    }
	
    // Create the bitmap context. We want pre-multiplied ARGB, 8-bits 
    // per component. Regardless of what the source image format is 
    // (CMYK, Grayscale, and so on) it will be converted over to the format
    // specified here by CGBitmapContextCreate.
    context = CGBitmapContextCreate (bitmapData,
									 pixelsWide,
									 pixelsHigh,
									 8,      // bits per component
									 bitmapBytesPerRow,
									 colorSpace,
									 kCGImageAlphaNoneSkipLast);
    if (context == NULL)
    {
        free (bitmapData);
        fprintf (stderr, "Context not created!");
    }
	
    // Make sure and release colorspace before returning
    CGColorSpaceRelease( colorSpace );
	
    return context;
}

};




void CImageProvider::rho_platform_image_load_grayscale(const char* url, void** image_pixels, int* pwidth, int* pheight) {
	*image_pixels = 0;
	*pwidth = 0;
	*pheight = 0;
	
	NSString* ns_path = [NSString stringWithCString:url encoding:1];
	UIImage* image = [UIImage imageWithContentsOfFile:ns_path];
	CGImageRef imageRef = [image CGImage];
    if (imageRef == NULL) 
    { 
        // error creating img
        return;
    }	
	
	// Create the bitmap context
    CGContextRef cgctx = IP_Inner_Helper::CreateRGBABitmapContext(imageRef);
    if (cgctx == NULL) 
    { 
        // error creating context
        return;
    }
	
	// Get image width, height. We'll use the entire image.
    size_t w = CGImageGetWidth(imageRef);
    size_t h = CGImageGetHeight(imageRef);
    CGRect rect = {{0,0},{w,h}}; 
	
    // Draw the image to the bitmap context. Once we draw, the memory 
    // allocated for the context for rendering will then contain the 
    // raw image data in the specified color space.
    CGContextDrawImage(cgctx, rect, imageRef); 
	
    // Now we can get a pointer to the image data associated with the bitmap
    // context.
    void *data = CGBitmapContextGetData (cgctx);
    if (data != NULL)
    {
		
		unsigned char* gray_buf = new unsigned char[w*h];
		if (gray_buf != 0) {
			*image_pixels = gray_buf;
			*pwidth = w;
			*pheight = h;
		
			// now we have image in int_32(ARGB) array
			{
				// make gray image
				unsigned char* dst = gray_buf;
				int* src = (int*)data;
				int tk = (1<<16)/3;
				int count = w*h;
				int i;
				for (i = count; i > 0; i--) {
					int c = *src++;
					//dst = (R+G+B)/3;
					*dst++ = (unsigned char)((((c & 0xFF) + ((c & 0xFF00)>>8)	+ ((c & 0xFF0000)>>16))*tk)>>16);
				}
			}
		}
		
    }
	
    // When finished, release the context
    CGContextRelease(cgctx); 
    // Free image data memory for the context
    if (data)
    {
        free(data);
    }
	
}

void CImageProvider::rho_platform_image_free(void* image_pixels) {
	if (image_pixels != 0) {
	  delete image_pixels;
	}
}


