
#import "I<%= $cur_module.name %>.h"
#import "<%= $cur_module.name %>SingletonBase.h"

@interface <%= $cur_module.name %>Singleton : <%= $cur_module.name %>SingletonBase<I<%= $cur_module.name %>Singleton> {
}

<% if $cur_module.is_template_default_instance %>
-(NSString*)getInitialDefaultID;
<% end %>

<% $cur_module.methods.each do |module_method|
  if module_method.access == ModuleMethod::ACCESS_STATIC
%><%= module_method.cached_data["iphone_line"] %>;
<% end
end %>



@end