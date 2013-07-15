
#import "I<%= name.camel_case %>.h"
#import "<%= name.camel_case %>Base.h"

@interface <%= name.camel_case %> : <%= name.camel_case %>Base<I<%= name.camel_case %>> {
}

-(void) getPlatformName:(id<IMethodResult>)methodResult;
-(void) calcSumm:(int)a b:(int)b methodResult:(id<IMethodResult>)methodResult;
-(void) joinStrings:(NSString*)a b:(NSString*)b methodResult:(id<IMethodResult>)methodResult;



@end
