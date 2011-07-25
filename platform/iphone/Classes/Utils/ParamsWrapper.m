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

#import "ParamsWrapper.h"


@implementation ParamsWrapper

@synthesize callName;
@synthesize values;
@synthesize names;

/*- (void)dealloc 
{
	[callName release];
	int cnt = [names count];
	for(int i = 0; i < cnt; i++) {
		[[names objectAtIndex:i] release];
		[[values objectAtIndex:i] release];
	}
	[values release];
	[names release];
	[super dealloc];
}*/

- (PARAMS_WRAPPER*) unwrap:(PARAMS_WRAPPER*)pw {
	pw->_callname = strdup([callName cStringUsingEncoding:[NSString defaultCStringEncoding]]);
	int cnt = [names count];
	pw->_nparams = cnt;
	pw->_names = malloc(cnt*sizeof(char*));
	pw->_values = malloc(cnt*sizeof(char*));
	for(int i = 0; i < cnt; i++) {
		pw->_names[i] = strdup([[names objectAtIndex:i] cStringUsingEncoding:[NSString defaultCStringEncoding]]);
		pw->_values[i] = strdup([[values objectAtIndex:i] cStringUsingEncoding:[NSString defaultCStringEncoding]]);
	}
	return pw;
}

+ (ParamsWrapper*) wrap:(PARAMS_WRAPPER*) params {
	ParamsWrapper* pw = [[ParamsWrapper alloc] init]; 
	pw->callName = [NSString stringWithCString:params->_callname encoding:[NSString defaultCStringEncoding]];
	pw->values = [NSMutableArray arrayWithCapacity:params->_nparams];
	pw->names = [NSMutableArray arrayWithCapacity:params->_nparams];
	for (int i=0;i<params->_nparams;i++) {
		[pw->names addObject:[NSString stringWithCString:params->_names[i] encoding:[NSString defaultCStringEncoding]]];
		[pw->values addObject:[NSString stringWithCString:params->_values[i] encoding:[NSString defaultCStringEncoding]]];
	}		
	return pw;
}
@end
