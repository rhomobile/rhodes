
#import "I<%= namecamelcase %>.h"
#import "<%= namecamelcase %>Base.h"

@interface <%= namecamelcase %> : <%= namecamelcase %>Base<I<%= namecamelcase %>> {
}

-(void) getPlatformName:(id<IMethodResult>)methodResult;
-(void) calcSumm:(int)a b:(int)b methodResult:(id<IMethodResult>)methodResult;
-(void) joinStrings:(NSString*)a b:(NSString*)b methodResult:(id<IMethodResult>)methodResult;



@end
