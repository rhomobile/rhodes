

#import "I<%= $cur_module.name %>.h"

@interface <%= $cur_module.name %>Base : NSObject {
<% if $cur_module.is_template_propertybag %>
    NSMutableDictionary* mProperties;
<% end %>
}

- (id) init;

<% if $cur_module.is_template_propertybag %>

+(NSString*) applyAliasesToPropertyName:(NSString*)prop_name;
+(NSDictionary*) applyAliasesToDictionary:(NSDictionary*)dict;
-(void) getProperty:(NSString*)propertyName methodResult:(id<IMethodResult>)methodResult;
-(void) getProperties:(NSArray*)arrayofNames methodResult:(id<IMethodResult>)methodResult;
-(void) getAllProperties:(id<IMethodResult>)methodResult;
-(void) setProperty:(NSString*)propertyName propertyValue:(NSString*)propertyValue;
-(void) setProperties:(NSDictionary*)propertyMap;
-(void) clearAllProperties;
<% end %>

<% $cur_module.methods.each do |module_method|
  add_method = true
  add_method = add_method && (module_method.access != ModuleMethod::ACCESS_STATIC)
  add_method = add_method && $cur_module.is_template_propertybag
  if (module_method.special_behaviour == ModuleMethod::SPECIAL_BEHAVIOUR_GETTER) || (module_method.special_behaviour == ModuleMethod::SPECIAL_BEHAVIOUR_SETTER)
     if module_method.linked_property.use_property_bag_mode != ModuleProperty::USE_PROPERTY_BAG_MODE_ACCESSORS_VIA_PROPERTY_BAG
        add_method = false
     end
  else
     add_method = false
  end
  if add_method
%><%= module_method.cached_data["iphone_line"] %>;
<% end
end %>

-(void) dealloc;

@end