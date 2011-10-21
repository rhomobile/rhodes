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

#ifdef __IPHONE_3_0

#import "MapAnnotation.h"
#include "common/RhodesApp.h"

@implementation MapAnnotation

@synthesize type, title, subtitle, address, url, coordinateString, resolvedAddress, coordinate, image, image_x_offset, image_y_offset;

-(id) init {
    [super init];
    type = [[NSString alloc] initWithFormat:@"ann"];
    title = [[NSString alloc] initWithFormat:@""];
    subtitle = [[NSString alloc] initWithFormat:@""];
    address = [[NSString alloc] initWithFormat:@""];
    coordinateString = [[NSMutableString alloc] initWithFormat:@""];
    resolvedAddress = [[NSMutableString alloc] initWithFormat:@""];
    url = [[NSString alloc] initWithFormat:@""];
    image = nil;
    image_x_offset = 0;
    image_y_offset = 0;
    return self;
}

-(void) dealloc {
    [type release];
    [title release];
    [subtitle release];
    [address release];
    [coordinateString release];
    [resolvedAddress release];
    [url release];
    [image release];
    [super dealloc];
}

- (void)setUrl:(NSString*)u {
    const char *surl = [u UTF8String];
    char *norm_url = rho_http_normalizeurl(surl);
    [url release];
    url = [[NSString stringWithUTF8String:norm_url] copy];
    rho_http_free(norm_url);
}

@end

#endif
