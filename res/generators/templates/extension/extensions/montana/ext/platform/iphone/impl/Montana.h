
#import "I<%= name.camel_case %>.h"
#import "<%= name.camel_case %>Base.h"

@interface <%= name.camel_case %> : <%= name.camel_case %>Base<I<%= name.camel_case %>> {
}

-(void) enable:(NSDictionary*)propertyMap methodResult:(id<IMethodResult>)methodResult;
-(void) start;
-(void) stop;
-(void) disable;
-(void) take:(NSDictionary*)propertyMap;



@end