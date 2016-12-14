
#import "ClipboardSingleton.h"
#import <UIKit/UIKit.h>

@implementation ClipboardSingleton



-(void) getText:(id<IMethodResult>)methodResult {
    UIPasteboard *pasteboard = [UIPasteboard generalPasteboard];
    if (pasteboard.hasStrings) {
        NSString* text = pasteboard.string;
        [methodResult setResult:text];
    }
    else {
        [methodResult setResult:[NSNull null]];
    }
}

-(void) hasText:(id<IMethodResult>)methodResult {
    UIPasteboard *pasteboard = [UIPasteboard generalPasteboard];
    if (pasteboard.hasStrings) {
        [methodResult setResult:[NSNumber numberWithBool:YES]];
    }
    else {
        [methodResult setResult:[NSNumber numberWithBool:NO]];
    }
}

-(void) setText:(NSString*)text methodResult:(id<IMethodResult>)methodResult {
    UIPasteboard *pasteboard = [UIPasteboard generalPasteboard];
    pasteboard.string = text;
}

-(void) clear:(id<IMethodResult>)methodResult {
    UIPasteboard *pasteboard = [UIPasteboard generalPasteboard];
    [pasteboard setValue:@"" forPasteboardType:UIPasteboardNameGeneral];
}




@end
