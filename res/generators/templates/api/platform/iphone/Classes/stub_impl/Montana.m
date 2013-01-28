
#import "<%= $cur_module.name %>.h"


@implementation <%= $cur_module.name %>


<% $cur_module.methods.each do |module_method|
  if module_method.access != ModuleMethod::ACCESS_STATIC
%><%= module_method.cached_data["iphone_line"] %> {

}

<% end
end %>


@end