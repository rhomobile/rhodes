
#import "<%= $cur_module.name %>Base.h"


<% if $cur_module.property_aliases.size > 0
%>
static NSDictionary* ourPropertyAliases= nil;
<%
   end
%>


@implementation <%= $cur_module.name %>Base


- (id) init {
    self = [super init];
<% if $cur_module.is_template_propertybag %>
    mProperties = [NSMutableDictionary dictionary];
    <% $cur_module.properties.each do |prop|
          if prop.default_value != nil
             line = '[self setProperty:@"'+prop.native_name+'" propertyValue:@"'+prop.default_value.to_s+'"];' %>
    <%= line %><%
          end
       end
     %>
<% end %>
    return self;
}

<% if $cur_module.is_template_propertybag %>

+(NSString*) applyAliasesToPropertyName:(NSString*)prop_name {
<% if $cur_module.property_aliases.size > 0
%>
    if (ourPropertyAliases == nil) {
        ourPropertyAliases = [NSDictionary dictionaryWithObjectsAndKeys:<%
                               $cur_module.property_aliases.each do |palias|
                                  line = '@"'+palias.existing_name+'", @"'+palias.new_name+'",' %>
                              <%= line %><%
                               end %>
                              nil];
    }
    NSObject* resNameObj = [ourPropertyAliases objectForKey:prop_name];
    if (resNameObj != nil) {
        return (NSString*)resNameObj;
    }
    return prop_name;
<%
    else

%>return prop_name;
<%
   end
%>
}

+(NSDictionary*) applyAliasesToDictionary:(NSDictionary*)dict {
<% if $cur_module.property_aliases.size > 0
%>
    NSArray* keys = [dict allKeys];
    int i;
    NSMutableDictionary* resDict = [NSMutableDictionary dictionaryWithCapacity:[keys count]];
    for (i = 0; i < [keys count]; i++) {
        NSString* key = (NSString*)[keys objectAtIndex:i];
        NSObject* value = [dict objectForKey:key];
        [resDict setObject:value forKey:[<%= $cur_module.name %>Base applyAliasesToPropertyName:key]];
    }
    return resDict;
<%
    else

%>return dict;
<%
   end
%>
}




-(void) getProperty:(NSString*)propertyName methodResult:(id<IMethodResult>)methodResult {
    [methodResult setResult:[mProperties objectForKey:[<%= $cur_module.name %>Base applyAliasesToPropertyName:propertyName]]];
}

-(void) setProperty:(NSString*)propertyName propertyValue:(NSString*)propertyValue {
   [mProperties setObject:propertyValue forKey:[<%= $cur_module.name %>Base applyAliasesToPropertyName:propertyName]];
}

-(void) getProperties:(NSArray*)arrayofNames methodResult:(id<IMethodResult>)methodResult {
    NSMutableDictionary* dict = [NSMutableDictionary dictionaryWithCapacity:[arrayofNames count]];
    NSArray* keys = arrayofNames;
    CMethodResult_SimpleHolder* resultHolder = [CMethodResult_SimpleHolder makeEmptyHolder];
    int i;
    for (i = 0; i < [keys count]; i++) {
        NSString* key = (NSString*)[keys objectAtIndex:i];
        [resultHolder setResult:nil];
        [self getProperty:key methodResult:resultHolder];
        if ([resultHolder getResult] != nil) {
            NSString* value = (NSString*)[resultHolder getResult];
            [dict setObject:value forKey:key];
        }
    }
    [methodResult setResult:dict];
}

-(void) getAllProperties:(id<IMethodResult>)methodResult {
    [self getProperties:[mProperties allKeys] methodResult:methodResult];
}


-(void) setProperties:(NSDictionary*)propertyMap {
    NSArray* keys = [propertyMap allKeys];
    int i;
    for (i = 0; i < [keys count]; i++) {
        NSString* key = (NSString*)[keys objectAtIndex:i];
        NSString* value = (NSString*)[propertyMap objectForKey:key];
        [self setProperty:key propertyValue:value];
    }
}

-(void) clearAllProperties {
   [mProperties removeAllObjects];
}




<% end %>


<%

$iphone_pb_getter_conversion = {}
$iphone_pb_getter_conversion["BOOLEAN"] = 'NSNumber* typedResult = [NSNumber numberWithBool:(([@"true" caseInsensitiveCompare:strResult] == NSOrderedSame)?YES:NO)];'
$iphone_pb_getter_conversion["INTEGER"] = 'NSNumber* typedResult = [NSNumber numberWithInt:[strResult intValue]];'
$iphone_pb_getter_conversion["FLOAT"] = 'NSNumber* typedResult = [NSNumber numberWithFloat:[strResult floatValue]];'
$iphone_pb_getter_conversion["STRING"] = 'NSString* typedResult = strResult;'

$iphone_pb_setter_conversion_pre = {}
$iphone_pb_setter_conversion_pre["BOOLEAN"] = 'NSString* strValue = '
$iphone_pb_setter_conversion_pre["INTEGER"] = 'NSString* strValue = [NSString stringWithFormat:@"%@", [NSNumber numberWithInt:'
$iphone_pb_setter_conversion_pre["FLOAT"] = 'NSString* strValue = [NSString stringWithFormat:@"%@", [NSNumber numberWithFloat:'
$iphone_pb_setter_conversion_pre["STRING"] = 'NSString* strValue = [NSString stringWithFormat:@"%@", '

$iphone_pb_setter_conversion_post = {}
$iphone_pb_setter_conversion_post["BOOLEAN"] = '?@"true":@"false";'
$iphone_pb_setter_conversion_post["INTEGER"] = ']];'
$iphone_pb_setter_conversion_post["FLOAT"] = ']];'
$iphone_pb_setter_conversion_post["STRING"] = '];'

%>


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
%><%= module_method.cached_data["iphone_line"] %> {
<% if module_method.special_behaviour == ModuleMethod::SPECIAL_BEHAVIOUR_GETTER %>
    CMethodResult_SimpleHolder* resultHolder = [CMethodResult_SimpleHolder makeEmptyHolder];
    [self getProperty:@"<%= module_method.linked_property.native_name %>" methodResult:resultHolder];
    if ([resultHolder getResult] != nil) {
        NSString* strResult = (NSString*)[resultHolder getResult];
        <%= $iphone_pb_getter_conversion[module_method.linked_property.type] %>
        [methodResult setResult:typedResult];
    }
    else {
        [methodResult setResult:nil];
    }
<% end
 if module_method.special_behaviour == ModuleMethod::SPECIAL_BEHAVIOUR_SETTER %>
    <%= $iphone_pb_setter_conversion_pre[module_method.linked_property.type] + module_method.linked_property.native_name + $iphone_pb_setter_conversion_post[module_method.linked_property.type] %>
    [self setProperty:@"<%= module_method.linked_property.native_name %>" propertyValue:strValue];
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