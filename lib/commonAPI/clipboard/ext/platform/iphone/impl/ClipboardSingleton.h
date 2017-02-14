
#import "IClipboard.h"
#import "ClipboardSingletonBase.h"

@interface ClipboardSingleton : ClipboardSingletonBase<IClipboardSingleton> {
}



-(void) getText:(id<IMethodResult>)methodResult;
-(void) hasText:(id<IMethodResult>)methodResult;
-(void) setText:(NSString*)text methodResult:(id<IMethodResult>)methodResult;
-(void) clear:(id<IMethodResult>)methodResult;




@end