//
//  NoodleLineNumberMarker.m
//  Line View Test
//
//  Created by Paul Kim on 9/30/08.
//  Copyright (c) 2008 Noodlesoft, LLC. All rights reserved.
//
// Permission is hereby granted, free of charge, to any person
// obtaining a copy of this software and associated documentation
// files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use,
// copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following
// conditions:
// 
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
//

#import "NoodleLineNumberMarker.h"


@implementation NoodleLineNumberMarker

- (id)initWithRulerView:(NSRulerView *)aRulerView lineNumber:(float)line image:(NSImage *)anImage imageOrigin:(NSPoint)imageOrigin
{
	if ((self = [super initWithRulerView:aRulerView markerLocation:0.0 image:anImage imageOrigin:imageOrigin]) != nil)
	{
		lineNumber = line;
	}
	return self;
}

- (void)setLineNumber:(unsigned)line
{
	lineNumber = line;
}

- (unsigned)lineNumber
{
	return lineNumber;
}

#pragma mark NSCoding methods

#define NOODLE_LINE_CODING_KEY		@"line"

- (id)initWithCoder:(NSCoder *)decoder
{
	if ((self = [super initWithCoder:decoder]) != nil)
	{
		if ([decoder allowsKeyedCoding])
		{
			lineNumber = [[decoder decodeObjectForKey:NOODLE_LINE_CODING_KEY] unsignedIntValue];
		}
		else
		{
			lineNumber = [[decoder decodeObject] unsignedIntValue];
		}
	}
	return self;
}

- (void)encodeWithCoder:(NSCoder *)encoder
{
	[super encodeWithCoder:encoder];
	
	if ([encoder allowsKeyedCoding])
	{
		[encoder encodeObject:[NSNumber numberWithUnsignedInt:lineNumber] forKey:NOODLE_LINE_CODING_KEY];
	}
	else
	{
		[encoder encodeObject:[NSNumber numberWithUnsignedInt:lineNumber]];
	}
}


#pragma mark NSCopying methods

- (id)copyWithZone:(NSZone *)zone
{
	id		copy;
	
	copy = [super copyWithZone:zone];
	[copy setLineNumber:lineNumber];
	
	return copy;
}


@end
