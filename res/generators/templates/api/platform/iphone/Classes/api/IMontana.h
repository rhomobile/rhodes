
#import <Foundation/Foundation.h>

#import "api_generator/iphone/IMethodResult.h"

@protocol I<%= $cur_module.name %> <NSObject>

<% if $cur_module. %>
-(void) getProps:(id<IMethodResult>)methodResult;
-(void) getPropsWithString:(NSString*)param1 methodResult:(id<IMethodResult>)methodResult;
-(void) getPropsWithArray:(NSArray*)param1 methodResult:(id<IMethodResult>)methodResult;
<% end %>
/*
-(void) setProps:(NSDictionary*)props;
-(void) setProperty:(NSString*)propertyName propertyValue:(NSObject*)propertyValue;
*/

// NOTE: if you want to hold methodResult(for periodically call callback for example) you should release it buself when you stop using it !

@end


@protocol I<%= $name %>Singleton <NSObject>

<% if $xml_class_attributes_singletone_id %>
-(void) enumerate:(id<IMethodResult>)methodResult;
<% end %>


<% if $xml_class_attributes_default_instance %>
-(NSString*) getDefaultID;
-(void) setDefaultID:(NSString*)defaultID;
-(NSString*)getInitialDefaultID;
<% end %>
@end


@protocol I<%= $name %>Factory <NSObject>
-(id<I<%= $name %>Singleton>) get<%= $name %>Singleton;
-(id<I<%= $name %>>) get<%= $name %>ByID:(NSString*)ID;
@end



@interface <%= $name %>FactorySingleton : NSObject {
}
+(id<I<%= $name %>Factory>) get<%= $name %>FactoryInstance;
@end


