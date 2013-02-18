
#import "<%= $cur_module.name %>.h"


@implementation <%= $cur_module.name %>



<% $cur_module.methods.each do |module_method|
  add_method = true
  add_method = add_method && (module_method.access != ModuleMethod::ACCESS_STATIC) && (module_method.generated_by_template != TEMPLATE_PROPERTY_BAG)
  if (module_method.special_behaviour == ModuleMethod::SPECIAL_BEHAVIOUR_GETTER) || (module_method.special_behaviour == ModuleMethod::SPECIAL_BEHAVIOUR_SETTER)
     if module_method.linked_property.use_property_bag_mode == ModuleProperty::USE_PROPERTY_BAG_MODE_ACCESSORS_VIA_PROPERTY_BAG
        add_method = false
     end
  end
  if add_method
%><%= module_method.cached_data["iphone_line"] %> {

}

<% end
end %>


@end