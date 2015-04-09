
#import "<%= $cur_module.name %>Base.h"
#import "api_generator/iphone/CMethodResult.h"


<% if $cur_module.property_aliases.size > 0
%>
static NSDictionary* ourPropertyAliases= nil;
<%
   end
%>


@implementation <%= $cur_module.name %>Base

- (void) resetAllPropertiesToDefault {
<% if $cur_module.is_template_propertybag 
%>    mDeclaredProperties = [[NSSet setWithObjects:<% 
        $cur_module.properties.each do |prop|
          line = '@"'+prop.native_name+'", ' %><%= line %><%
        end
     %>nil] retain];

    mProperties = [[NSMutableDictionary dictionaryWithCapacity:<%= $cur_module.properties.size + 2 %>] retain];
    <% $cur_module.properties.each do |prop|
          if prop.default_value != nil
             line = '[self setProperty:@"'+prop.native_name+'" propertyValue:@"'+prop.default_value.to_s+'" methodResult:nil];' %>
    <%= line %><%
          end
       end
     %>
<% end %>
}

- (id) init {
    self = [super init];
    [self resetAllPropertiesToDefault];
    return self;
}

<% if $cur_module.is_template_propertybag %>

+(NSString*) applyAliasesToPropertyName:(NSString*)prop_name {
<% if $cur_module.property_aliases.size > 0
%>
    if (ourPropertyAliases == nil) {
        ourPropertyAliases = [[NSDictionary dictionaryWithObjectsAndKeys:<%
                               $cur_module.property_aliases.each do |palias|
                                  line = '@"'+palias.existing_name+'", @"'+palias.new_name+'",' %>
                              <%= line %><%
                               end %>
                              nil] retain];
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
    NSObject* value = [mProperties objectForKey:[<%= $cur_module.name %>Base applyAliasesToPropertyName:propertyName]];
    [methodResult setResult:value];
}

-(void) setProperty:(NSString*)propertyName propertyValue:(NSString*)propertyValue methodResult:(id<IMethodResult>)methodResult {
    NSObject* value = propertyValue;
    NSString* strValue = propertyValue;
    if ([propertyName isEqualToString:@"ID"] && [value isKindOfClass:[NSString class]] && [strValue isEqualToString:@""]) {
        NSLog(@"Clearing ID!");
    }
    if ([value isKindOfClass:[NSNumber class]]) {
        NSNumber* numValue = (NSNumber*)value;
        if ([CMethodResult isBoolInsideNumber:numValue]) {
            BOOL boolValue = [numValue boolValue];
            if (boolValue) {
                strValue = @"true";
            }
            else {
                strValue = @"false";
            }
        }
        else if ([CMethodResult isIntInsideNumber:numValue]) {
            strValue =  [NSString stringWithFormat:@"%@", numValue];
        }
        else if ([CMethodResult isFloatInsideNumber:numValue]) {
            strValue =  [NSString stringWithFormat:@"%@", numValue];
        }
        else {
            // error !
            NSLog(@"<%= $cur_module.name %>.setProperty(\"%@\", value) UNSUPPORTED VALUE TYPE ! MUST BE STRING !!!", propertyName);
        }
    }
    else if ([value isKindOfClass:[NSString class]]) {
        //is OK!
    }
    else {
        // error !
        NSLog(@"<%= $cur_module.name %>.setProperty(\"%@\", value) UNSUPPORTED VALUE TYPE ! MUST BE STRING !!!", propertyName);

    }

   [mProperties setObject:strValue forKey:[<%= $cur_module.name %>Base applyAliasesToPropertyName:propertyName]];
}

-(void) getProperties:(NSArray*)arrayofNames methodResult:(id<IMethodResult>)methodResult {
    NSMutableDictionary* dict = [[NSMutableDictionary dictionaryWithCapacity:[arrayofNames count]] autorelease];
    CMethodResult_SimpleHolder* resultHolder = [CMethodResult_SimpleHolder makeEmptyHolder];
    for (NSString* key in arrayofNames) {        
      [resultHolder setResult:nil];
      [self getProperty:key methodResult:resultHolder];
      if ([resultHolder getResult] != nil) {
          NSString* value = (NSString*)[resultHolder getResult];
          [dict setObject:value forKey:key];
      } <% 
if !$cur_module.is_property_bag_limit_to_only_declared_properties && $cur_module.is_template_propertybag %>
      else {
        [dict setObject:@"" forKey:key];
      }<% end %>
    }

    [methodResult setResult:dict];
}

-(void) getAllProperties:(id<IMethodResult>)methodResult {
    [self getProperties:[mProperties allKeys] methodResult:methodResult];
}


-(void) setProperties:(NSDictionary*)propertyMap methodResult:(id<IMethodResult>)methodResult {
    for (NSString* key in propertyMap) {    
        NSString* value = (NSString*)[propertyMap objectForKey:key];
        [self setProperty:key propertyValue:value methodResult:methodResult];
    }
}

-(void) clearAllProperties:(id<IMethodResult>)methodResult {
   [mProperties removeAllObjects];
   [self resetAllPropertiesToDefault];
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
    [self setProperty:@"<%= module_method.linked_property.native_name %>" propertyValue:strValue methodResult:methodResult];
<% end %>
}

<% end
end %>


-(void) dealloc {
<% if $cur_module.is_template_propertybag %>
    [mProperties release];
    [mDeclaredProperties release];
<% end %>
    [super dealloc];
}

@end