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

#import "LeftViewController.h"
#import "SimpleMainView.h"
#import "Rhodes.h"
#import "AppManager.h"

#include "common/RhodesApp.h"
#include "logging/RhoLog.h"

#include "NativeBar.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "LeftViewController"



@interface RhoLeftItem : NSObject {
@public
    NSString *url;
    UIImage* image;
    NSString *title;
    NSString *detail;
    UIColor *selectColor;
    UIImage* backgroundImage;
    UIImage* backgroundSelectedImage;
    int nIndentWidth;
    
}

@property (retain) NSString *url;
@property (retain) UIImage *image;
@property (retain) NSString *title;
@property (retain) NSString *detail;
@property (retain) UIColor *selectColor;
@property (retain) UIImage* backgroundImage;
@property (retain) UIImage* backgroundSelectedImage;
@property (nonatomic,assign) int nIndentWidth;
@property (retain) UIColor *labelSelColor;
@property (retain) UIColor *detailSelColor;

- (id)init;
- (void)dealloc;

@end

@implementation RhoLeftItem

@synthesize url, image, title, detail, selectColor, backgroundImage, backgroundSelectedImage, nIndentWidth, labelSelColor, detailSelColor;

- (id)init {
    url = nil;
    image = nil;
    title = nil;
    detail = nil;
    selectColor = nil;
    labelSelColor = nil;
    detailSelColor = nil;
    backgroundImage = nil;
    backgroundSelectedImage = nil;
    nIndentWidth = 0;
    return self;
}

- (void)dealloc {
    [url release];
    [image release];
    [title release];
    [detail release];
    [selectColor release];
    [detailSelColor release];
    [labelSelColor release];
    [backgroundImage release];
    [backgroundSelectedImage release];
    [super dealloc];
}

@end



@implementation LeftViewController

@synthesize itemsData, preferredSize, myFont, iconColor, detailColor, backgroundColor, groupBottomIndex, groupBottomHeight;

- (id)initWithItems:(NSDictionary*)bar_info parent:(SplittedMainView*)parent  {
	self = [self initWithStyle:UITableViewStylePlain];
	
	splittedView = parent;
	
	
	NSString *background_color = nil;
    NSString *icon_color = nil;
    NSString *detail_color = nil;
    int nMaxWidth = 1000000, nFixedWidth = -1;
	NSDictionary* global_properties = (NSDictionary*)[bar_info objectForKey:NATIVE_BAR_PROPERTIES];
	if (global_properties != nil) {
		background_color = (NSString*)[global_properties objectForKey:NATIVE_BAR_BACKGOUND_COLOR];
		icon_color = (NSString*)[global_properties objectForKey:NATIVE_BAR_ICON_COLOR];
		detail_color = (NSString*)[global_properties objectForKey:NATIVE_BAR_DETAIL_COLOR];
        if ([global_properties objectForKey:NATIVE_BAR_MAX_WIDTH])
            nMaxWidth = [(NSNumber*)[global_properties objectForKey:NATIVE_BAR_MAX_WIDTH] intValue];
        if ([global_properties objectForKey:NATIVE_BAR_FIXED_WIDTH])
            nFixedWidth = [(NSNumber*)[global_properties objectForKey:NATIVE_BAR_FIXED_WIDTH] intValue];
	}
    
    if (background_color != nil) {
        int c = [background_color intValue];
        int cR = (c & 0xFF0000) >> 16;
        int cG = (c & 0xFF00) >> 8;
        int cB = (c & 0xFF);
        self.backgroundColor = [UIColor colorWithRed:( ((float)(cR)) / 255.0) green:(((float)(cG)) / 255.0) blue:(((float)(cB)) / 255.0) alpha:1.0];
    }
    
    if (icon_color != nil) {
        int c = [icon_color intValue];
        int cR = (c & 0xFF0000) >> 16;
        int cG = (c & 0xFF00) >> 8;
        int cB = (c & 0xFF);
        self.iconColor = [UIColor colorWithRed:( ((float)(cR)) / 255.0) green:(((float)(cG)) / 255.0) blue:(((float)(cB)) / 255.0) alpha:1.0];
    }

    if (detail_color != nil) {
        int c = [detail_color intValue];
        int cR = (c & 0xFF0000) >> 16;
        int cG = (c & 0xFF00) >> 8;
        int cB = (c & 0xFF);
        self.detailColor = [UIColor colorWithRed:( ((float)(cR)) / 255.0) green:(((float)(cG)) / 255.0) blue:(((float)(cB)) / 255.0) alpha:1.0];
    }
    
	NSArray* items = (NSArray*)[bar_info objectForKey:NATIVE_BAR_ITEMS];

    int count = [items count];

    NSMutableArray *tabs = [[NSMutableArray alloc] initWithCapacity:count];
    
    NSString *initUrl = nil;

    UIFont* font = [UIFont fontWithName:@"Helvetica-Bold" size:16.0];
	self.myFont = font;
	[font release];
	
	self.preferredSize = 0;
	self.groupBottomIndex = -1;
    self.groupBottomHeight = 0;
    
    for (int i = 0; i < count; ++i) {
		NSDictionary* item = (NSDictionary*)[items objectAtIndex:i];
        
        NSString *label = (NSString*)[item objectForKey:NATIVE_BAR_ITEM_LABEL];
        NSString *detailLabel = (NSString*)[item objectForKey:NATIVE_BAR_ITEM_DETAIL_LABEL];
        NSString *url = (NSString*)[item objectForKey:NATIVE_BAR_ITEM_ACTION];
        NSString *icon = (NSString*)[item objectForKey:NATIVE_BAR_ITEM_ICON];
        NSString *selected_color = (NSString*)[item objectForKey:NATIVE_BAR_ITEM_SELECTED_COLOR];
        NSString *label_sel_color = (NSString*)[item objectForKey:NATIVE_BAR_ITEM_LABEL_SEL_COLOR];
        NSString *detail_sel_color = (NSString*)[item objectForKey:NATIVE_BAR_ITEM_DETAIL_SEL_COLOR];
        NSString *background_image = (NSString*)[item objectForKey:NATIVE_BAR_ITEM_BACK_IMAGE];
        NSString *background_sel_image = (NSString*)[item objectForKey:NATIVE_BAR_ITEM_BACK_SEL_IMAGE];
        NSString *group_bottom = (NSString*)[item objectForKey:NATIVE_BAR_ITEM_GROUP_BOTTOM];
        
        if (group_bottom && [group_bottom isEqual: @"true"])
            self.groupBottomIndex = i;
        
        if (!initUrl)
            initUrl = url;
        
        if (label && url && icon) {
            RhoLeftItem *td = [[RhoLeftItem alloc] init];
            td.url = url;
			td.title = label;
            if (detailLabel != nil) {
                td.detail = detailLabel;
            }
            if (selected_color != nil) {
                int c = [selected_color intValue];
                int cR = (c & 0xFF0000) >> 16;
                int cG = (c & 0xFF00) >> 8;
                int cB = (c & 0xFF);
                td.selectColor = [UIColor colorWithRed:( ((float)(cR)) / 255.0) green:(((float)(cG)) / 255.0) blue:(((float)(cB)) / 255.0) alpha:1.0];
            }
            if (label_sel_color != nil) {
                int c = [label_sel_color intValue];
                int cR = (c & 0xFF0000) >> 16;
                int cG = (c & 0xFF00) >> 8;
                int cB = (c & 0xFF);
                td.labelSelColor = [UIColor colorWithRed:( ((float)(cR)) / 255.0) green:(((float)(cG)) / 255.0) blue:(((float)(cB)) / 255.0) alpha:1.0];
            }
            if (detail_sel_color != nil) {
                int c = [detail_sel_color intValue];
                int cR = (c & 0xFF0000) >> 16;
                int cG = (c & 0xFF00) >> 8;
                int cB = (c & 0xFF);
                td.detailSelColor = [UIColor colorWithRed:( ((float)(cR)) / 255.0) green:(((float)(cG)) / 255.0) blue:(((float)(cB)) / 255.0) alpha:1.0];
            }
            
			NSString *imagePath = [[AppManager getApplicationsRootPath] stringByAppendingPathComponent:icon];
			td.image = [UIImage imageWithContentsOfFile:imagePath];
            
            NSString *backImagePath = [[AppManager getApplicationsRootPath] stringByAppendingPathComponent:background_image];
			td.backgroundImage = [UIImage imageWithContentsOfFile:backImagePath];
            NSString *backSelImagePath = [[AppManager getApplicationsRootPath] stringByAppendingPathComponent:background_sel_image];
			td.backgroundSelectedImage = [UIImage imageWithContentsOfFile:backSelImagePath];

            if ([item objectForKey:NATIVE_BAR_ITEM_INDENT_WIDTH])
                td.nIndentWidth = [(NSNumber*)[item objectForKey:NATIVE_BAR_ITEM_INDENT_WIDTH] intValue];
            
            [tabs addObject:td];

            if (nFixedWidth > 0 )
                self.preferredSize = nFixedWidth;
            else
            {
                CGSize textSize = [label sizeWithFont:myFont];
                int pref_size = td.image.size.width + (textSize.width > 0 ? textSize.width + 32 : 0);// + 16;
                if (self.preferredSize < pref_size ) {
                    if ( pref_size < nMaxWidth)
                        self.preferredSize = pref_size;
                    else
                        self.preferredSize = nMaxWidth;
                }
            }
			[td release];
		}
    }

    self.itemsData = tabs;
    [tabs release];

	self.view.autoresizingMask = UIViewAutoresizingFlexibleLeftMargin | UIViewAutoresizingFlexibleTopMargin | UIViewAutoresizingFlexibleHeight;
    self.tableView.separatorStyle = UITableViewCellSeparatorStyleNone;

    if (self.backgroundColor != nil) {
        self.tableView.backgroundView.backgroundColor = self.backgroundColor;
        self.tableView.backgroundColor = self.backgroundColor;
    }
    
	[self.tableView reloadData];
    //[splittedView switchTab:1];
    //[self setSelection:1];
	
	return self;
}

- (void)updateViewHeight:(int) height{
    
    if (groupBottomIndex >= 0 && groupBottomIndex+1 < [itemsData count])
    {
        NSIndexPath* pathStart0 = [NSIndexPath indexPathForRow:0 inSection:0];
        NSIndexPath* pathStart = [NSIndexPath indexPathForRow:groupBottomIndex inSection:0];
        NSIndexPath* pathEnd = [NSIndexPath indexPathForRow:[itemsData count]-1 inSection:0];
        
        CGRect rcStart0 = [self.tableView rectForRowAtIndexPath:pathStart0];
        CGRect rcStart = [self.tableView rectForRowAtIndexPath:pathStart];
        CGRect rcEnd = [self.tableView rectForRowAtIndexPath:pathEnd];
        
        int nBottomHeight = height - rcStart.origin.y;
        
        groupBottomHeight = nBottomHeight - ( (rcEnd.origin.y+rcEnd.size.height) - rcStart.origin.y );
        
        if (groupBottomHeight < 0 )
            groupBottomHeight = 0;
    }
}

/*
- (void)loadView {
	UITableView* tv = [[UITableView alloc] initWithFrame:CGRectMake(0,0, 100, 100) style:UITableViewStylePlain];
	self.view = tv;
}
 */


- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    // Return the number of sections.
    return 1;
}


- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    // Return the number of rows in the section.
    return [self.itemsData count];
}


// Customize the appearance of table view cells.
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    
    static NSString *CellIdentifier = @"Cell";
    
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    if (cell == nil) {
        cell = [[[UITableViewCell alloc] initWithStyle:UITableViewCellStyleSubtitle reuseIdentifier:CellIdentifier] autorelease];
    }else
    {
        cell.userInteractionEnabled = YES;
        cell.indentationLevel = 0;
        cell.indentationWidth = 0;
        if ( cell.detailTextLabel.text )
        {
            cell.detailTextLabel.text = [NSString stringWithString:@""];
        }
    }
    
    RhoLeftItem* td =   [self.itemsData objectAtIndex:indexPath.row];
    
	cell.imageView.image = [td image];
    cell.imageView.clipsToBounds = YES;
	cell.textLabel.text = [NSString stringWithFormat:[td title], indexPath.section, indexPath.row];
    
    if (td.nIndentWidth > 0 )
    {
        cell.indentationLevel = 1;
        cell.indentationWidth = td.nIndentWidth;
    }
    
    if ( td.backgroundImage != nil )
    {
        UIImageView* pImgView = [[UIImageView alloc] initWithFrame:cell.frame];
        pImgView.image = td.backgroundImage;
        cell.backgroundView = pImgView;
        
        if (self.backgroundColor != nil) {
            //cell.backgroundView = [[UIView alloc] initWithFrame:cell.frame];
            cell.backgroundView.backgroundColor = self.backgroundColor;
        }

    }else
    {
        if (self.backgroundColor != nil) {
            cell.backgroundView = [[UIView alloc] initWithFrame:cell.frame];
            cell.backgroundView.backgroundColor = self.backgroundColor;
        }
    }
    
    if ( td.backgroundSelectedImage != nil )
    {
        UIImageView* pImgView2 = [[UIImageView alloc] initWithFrame:cell.frame];
        pImgView2.image = td.backgroundSelectedImage;
        cell.selectedBackgroundView = pImgView2;
        
        [[cell textLabel] setBackgroundColor:[UIColor clearColor]];
        [[cell detailTextLabel] setBackgroundColor:[UIColor clearColor]];
        
        //cell.textLabel.highlightedTextColor = self.iconColor;
        //cell.detailTextLabel.highlightedTextColor = self.detailColor;
    }
    
    if (td.labelSelColor)
        cell.textLabel.highlightedTextColor = td.labelSelColor;
    if (td.detailSelColor)
        cell.detailTextLabel.highlightedTextColor = td.detailSelColor;
    
    
    if ( [td.url isEqual: @"separator"] )
    {
        cell.userInteractionEnabled = NO;
        if ( td.backgroundImage == nil && indexPath.row != self.groupBottomIndex)
        {
            cell.backgroundView = [[UIView alloc] initWithFrame:cell.frame];
            cell.backgroundView.backgroundColor = [UIColor blackColor];
        }
    }
    else
        cell.textLabel.font = myFont;
    
    if (self.iconColor != nil) {
        cell.textLabel.textColor = self.iconColor;
    }
    if (td.detail != nil) {
        cell.detailTextLabel.text = [NSString stringWithFormat:[td detail], indexPath.section, indexPath.row];
        if (self.detailColor != nil) {
            cell.detailTextLabel.textColor = self.detailColor;
        }
    }

    if (td.selectColor != nil) {
        //cell.selectionStyle = UITableViewCellSelectionStyleNone;
        cell.selectedBackgroundView = [[UIView alloc] initWithFrame:cell.frame];
        cell.selectedBackgroundView.backgroundColor = td.selectColor;
    }
	
    return cell;
}


- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
	return YES;
}

#ifdef __IPHONE_6_0
- (BOOL)shouldAutorotate
{
	return YES;
}


- (NSUInteger)supportedInterfaceOrientations
{
    return UIInterfaceOrientationMaskAll;
}
#endif



- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
	int selectedItem = indexPath.row;
	[splittedView switchTab:selectedItem];
}


- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath {
    int index = indexPath.row;
    RhoLeftItem* td =   [self.itemsData objectAtIndex:index];
    if ( groupBottomIndex == index )
        return groupBottomHeight;

    if (td.backgroundImage != nil)
        return td.backgroundImage.size.height;

    if ( [td.url isEqual: @"separator"] )
        return 2;
    
    return tableView.rowHeight;
}

- (void)setSelectionCommand:(NSIndexPath*)index {

	[self.tableView selectRowAtIndexPath:index animated:YES scrollPosition:UITableViewScrollPositionNone];

}

- (int)getFirstActiveItem
{
    int i = 0;
    for ( ; i < [self.itemsData count]; i++ )
    {
        RhoLeftItem* td =   [self.itemsData objectAtIndex:i];
        if ( [td.url  isEqual: @"separator"] )
            continue;
        else
            break;
    }
    
    if ( i >= [self.itemsData count]) {
		return 0;
	}
    
    return i;
}

- (void)setSelection:(int)index {
	if ((index < 0) || (index >= [self.itemsData count])) {
		return;
	}
    
	NSIndexPath* path = [NSIndexPath indexPathForRow:index inSection:0];
	[self performSelectorOnMainThread:@selector(setSelectionCommand:) withObject:path waitUntilDone:NO];	
}

- (int)getPreferredWidth {
	return self.preferredSize;
}


@end
