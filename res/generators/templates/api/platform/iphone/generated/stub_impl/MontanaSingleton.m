
#import "<%= $cur_module.name %>Singleton.h"


@implementation <%= $cur_module.name %>Singleton

<% if $cur_module.is_template_default_instance %>
-(NSString*)getInitialDefaultID {

}
<% end %>

<% $cur_module.methods.each do |module_method|
  if module_method.access == ModuleMethod::ACCESS_STATIC
%><%= module_method.cached_data["iphone_line"] %> {

}

<% end
end %>


@end