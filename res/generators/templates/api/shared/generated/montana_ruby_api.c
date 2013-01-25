
#include "ruby.h"

static VALUE rb_mParent;
static VALUE rb_c<%= $cur_module.name %>;

<% $cur_module.methods.each do |module_method|%>
<%= api_generator_MakeRubyMethodDecl($cur_module.name, module_method, module_method.access == ModuleMethod::ACCESS_STATIC)%>;

<% if $cur_module.is_template_default_instance && module_method.access == ModuleMethod::ACCESS_INSTANCE%>
<%= api_generator_MakeRubyMethodDecl($cur_module.name + "_def", module_method, true)%>;
<% end %>

<% end %>

VALUE getRuby_<%= $cur_module.name %>_Module(){ return rb_c<%= $cur_module.name %>; }

<% 
def api_generator_MakeRubyMethodDef(module_name, module_method, is_static, method_suffix)
    method_name = 'rb_'
    method_name += 's_' if is_static
    method_name += module_name + "_" 
    method_name += method_suffix + "_" if method_suffix.length() > 0
    method_name += module_method.name

    "rb_define_#{(module_method.access == ModuleMethod::ACCESS_STATIC) ? 'singleton_':''}method(rb_c#{module_name}, \"#{module_method.name}\", #{method_name}, -1);"
end

%>
void Init_RubyAPI_<%= $cur_module.name %>(void)
{
    rb_mParent = rb_define_module("<%= $cur_module.parent %>");
	rb_c<%= $cur_module.name %> = rb_define_class_under(rb_mParent, "<%= $cur_module.name %>", rb_cObject);
	
    //Constructor should be not available
	//rb_define_alloc_func(rb_cBarcode1, rb_barcode1_allocate);
    rb_undef_alloc_func(rb_c<%= $cur_module.name %>);

<% $cur_module.methods.each do |module_method|%>
<%= api_generator_MakeRubyMethodDef($cur_module.name, module_method, module_method.access == ModuleMethod::ACCESS_STATIC, "" ) %>

<% if $cur_module.is_template_default_instance && module_method.access == ModuleMethod::ACCESS_INSTANCE%>
<%= api_generator_MakeRubyMethodDef($cur_module.name, module_method, true, "def")%>;
<% end %>

<% end %>



}

