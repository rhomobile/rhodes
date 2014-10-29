
#import "I<%= namecamelcase %>.h"
#import "<%= namecamelcase %>SingletonBase.h"

@interface <%= namecamelcase %>Singleton : <%= namecamelcase %>SingletonBase<I<%= namecamelcase %>Singleton> {
}


-(NSString*)getInitialDefaultID;


-(void) enumerate:(id<IMethodResult>)methodResult;




@end