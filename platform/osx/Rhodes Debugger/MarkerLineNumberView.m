//
//  MarkerTextView.m
//  Line View Test
//
//  Created by Paul Kim on 10/4/08.
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

#import "MarkerLineNumberView.h"
#import "NoodleLineNumberMarker.h"

#define CORNER_RADIUS	3.0
#define MARKER_HEIGHT	13.0

@implementation MarkerLineNumberView
@synthesize oldMarker;
@synthesize debugger;


- (void)dealloc
{
	[markerImage release];
	
	[super dealloc];
}

- (void)setRuleThickness:(float)thickness
{
	[super setRuleThickness:thickness];
	
	// Overridden to reset the size of the marker image forcing it to redraw with the new width.
	// If doing this in a non-subclass of NoodleLineNumberView, you can set it to post frame 
	// notifications and listen for them.
	[markerImage setSize:NSMakeSize(thickness, MARKER_HEIGHT)];	
}

- (void)drawMarkerImageIntoRep:(id)rep
{
	NSBezierPath	*path;
	NSRect			rect;
	
	rect = NSMakeRect(1.0, 2.0, [rep size].width - 2.0, [rep size].height - 3.0);
	
	path = [NSBezierPath bezierPath];
	[path moveToPoint:NSMakePoint(NSMaxX(rect), NSMinY(rect) + NSHeight(rect) / 2)];
	[path lineToPoint:NSMakePoint(NSMaxX(rect) - 5.0, NSMaxY(rect))];
	
	[path appendBezierPathWithArcWithCenter:NSMakePoint(NSMinX(rect) + CORNER_RADIUS, NSMaxY(rect) - CORNER_RADIUS) radius:CORNER_RADIUS startAngle:90 endAngle:180];
	
	[path appendBezierPathWithArcWithCenter:NSMakePoint(NSMinX(rect) + CORNER_RADIUS, NSMinY(rect) + CORNER_RADIUS) radius:CORNER_RADIUS startAngle:180 endAngle:270];
	[path lineToPoint:NSMakePoint(NSMaxX(rect) - 5.0, NSMinY(rect))];
	[path closePath];
	
	[[NSColor colorWithCalibratedRed:0.003 green:0.56 blue:0.85 alpha:1.0] set];
	[path fill];
	
	[[NSColor colorWithCalibratedRed:0 green:0.44 blue:0.8 alpha:1.0] set];
	
	[path setLineWidth:2.0];
	[path stroke];
}

- (NSImage *)markerImageWithSize:(NSSize)size
{
	if (markerImage == nil)
	{
		NSCustomImageRep	*rep;
		
		markerImage = [[NSImage alloc] initWithSize:size];
		rep = [[NSCustomImageRep alloc] initWithDrawSelector:@selector(drawMarkerImageIntoRep:) delegate:self];
		[rep setSize:size];
		[markerImage addRepresentation:rep];
		[rep release];
	}
	return markerImage;
}

- (void)mouseDown:(NSEvent *)theEvent
{
	NSPoint					location;
	unsigned				line;
	
	location = [self convertPoint:[theEvent locationInWindow] fromView:nil];
	line = [self lineNumberForLocation:location.y];
	
	if (line != NSNotFound)
	{
		NoodleLineNumberMarker		*marker;
		
		marker = [self markerAtLine:line];
		
		
		if (marker != nil)
		{
			[self removeMarker:marker];
			if(debugger != nil) {
				[debugger removeBreakpoint];
			}
			
		}
		else
		{
			if(oldMarker != nil) {
				[self removeMarker:[self markerAtLine:[oldMarker lineNumber]]];
			}
			marker = [[NoodleLineNumberMarker alloc] initWithRulerView:self
															 lineNumber:line
																  image:[self markerImageWithSize:NSMakeSize([self ruleThickness], MARKER_HEIGHT)]
														   imageOrigin:NSMakePoint(0, MARKER_HEIGHT / 2)];
			[self setOldMarker: marker];
			[self addMarker:marker];
			[marker release];
			
			if(debugger != nil) {
				[debugger setBreakPoint:nil atLine:line];
			}
		}
		[self setNeedsDisplay:YES];
	}
}

@end
