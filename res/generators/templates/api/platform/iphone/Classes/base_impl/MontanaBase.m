
#import "<%= $cur_module.name %>Base.h"

@implementation <%= $cur_module.name %>Base


- (id) init {
    self = [super init];
<% if $cur_module.is_template_propertybag %>
    mProperties = [NSMutableDictionary dictionary];
<% end %>
    return self;
}

<% if $cur_module.is_template_propertybag %>

-(void) getProperty:(NSString*)propertyName methodResult:(id<IMethodResult>)methodResult {
     [methodResult setResult:[mProperties objectForKey:propertyName]];
}

-(void) getProperties:(NSArray*)arrayofNames methodResult:(id<IMethodResult>)methodResult {
     [methodResult setResult:[mProperties dictionaryWithValuesForKeys:arrayofNames]];
}

-(void) getAllProperties:(id<IMethodResult>)methodResult {
     [methodResult setResult:[mProperties dictionaryWithValuesForKeys:[[mProperties keyEnumerator] allObjects]]];
}

-(void) setProperty:(NSString*)propertyName propertyValue:(NSString*)propertyValue {
    [mProperties setObject:propertyValue forKey:propertyName];
}

-(void) setProperties:(NSDictionary*)propertyMap {
    [mProperties addEntriesFromDictionary:dict];
}

-(void) clearAllProperties {
    [mProperties removeAllObjects];
}

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
%><%= module_method.cached_data["iphone_line"] %>; {
<% if module_method.special_behaviour == ModuleMethod::SPECIAL_BEHAVIOUR_GETTER %>
    [self getProperty:@"<%= module_method.linked_property.native_name %>" methodResult:methodResult]
<% end
 if module_method.special_behaviour == ModuleMethod::SPECIAL_BEHAVIOUR_SETTER %>
    [self setProperty::@"<%= module_method.linked_property.native_name %>" propertyValue:value]
<% end %>
}

<% end
end %>


-(void) dealloc {
<% if $cur_module.is_template_propertybag %>
    [mProperties release];
<% end %>
    [super dealloc];
}

@end