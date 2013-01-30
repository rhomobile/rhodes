
#import "../api/I<%= $cur_module.name %>.h"
#import "../base_impl/<%= $cur_module.name %>Base.h"

@interface <%= $cur_module.name %> : <%= $cur_module.name %>Base<I<%= $cur_module.name %>> {
}

<% $cur_module.methods.each do |module_method|
  if module_method.access != ModuleMethod::ACCESS_STATIC
%><%= module_method.cached_data["iphone_line"] %>;
<% end
end %>


@end