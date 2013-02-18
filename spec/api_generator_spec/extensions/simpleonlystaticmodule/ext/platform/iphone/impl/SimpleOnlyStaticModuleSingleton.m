
#import "SimpleOnlyStaticModuleSingleton.h"
#import <UIKit/UIKit.h>


@implementation SimpleOnlyStaticModuleSingleton



-(void) calcSumm:(int)x y:(int)y methodResult:(id<IMethodResult>)methodResult {
    [methodResult setResult:[NSNumber numberWithInt:(x+y)]];
}

-(void) joinStrings:(NSArray*)strings methodResult:(id<IMethodResult>)methodResult {
    NSString* res = [NSString stringWithUTF8String:""];
    int i;
    for (i = 0; i < [strings count]; i++) {
        res = [res stringByAppendingString:((NSString*)[strings objectAtIndex:i])];
    }
    [methodResult setResult:res];
}

-(void) getPlatform:(id<IMethodResult>)methodResult {
    [methodResult setResult:@"iPhone"];
}

-(void) showAlertFromUIThread {
    // this code crashed if run from non UI thread !
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@""
                                                    message:@"Alert showed from native UI thread !"
                                                   delegate:nil
                                          cancelButtonTitle:@"OK"
                                          otherButtonTitles: nil];
    [alert show];
    [alert release];
}




@end