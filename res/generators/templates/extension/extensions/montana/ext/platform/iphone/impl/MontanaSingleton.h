
#import "I<%= name.camel_case %>.h"
#import "<%= name.camel_case %>SingletonBase.h"

@interface <%= name.camel_case %>Singleton : <%= name.camel_case %>SingletonBase<I<%= name.camel_case %>Singleton> {
}


-(NSString*)getInitialDefaultID;


-(void) enumerate:(id<IMethodResult>)methodResult;




@end