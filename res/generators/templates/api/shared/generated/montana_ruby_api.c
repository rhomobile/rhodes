
#include "ruby.h"

static VALUE rb_mParent;
static VALUE rb_c<%= $cur_module.name %>;

<% $cur_module.methods.each do |module_method|
%><%= api_generator_MakeRubyMethodDecl($cur_module.name, module_method, module_method.access == ModuleMethod::ACCESS_STATIC)%>;
<% if $cur_module.is_template_default_instance && module_method.access == ModuleMethod::ACCESS_INSTANCE
%><%= api_generator_MakeRubyMethodDecl($cur_module.name + "_def", module_method, true)%>;
<% end
   end %>

<% if $cur_module.is_template_default_instance %>
VALUE rb_<%= $cur_module.name %>_s_default(VALUE klass);
VALUE rb_<%= $cur_module.name %>_s_setDefault(VALUE klass, VALUE obj);
<% end %>

VALUE getRuby_<%= $cur_module.name %>_Module(){ return rb_c<%= $cur_module.name %>; }

<%
def api_generator_MakeRubyMethodDef(module_name, module_method, is_static, method_suffix)
    method_name = 'rb_'
    method_name += 's_' if is_static
    method_name += module_name + "_"
    method_name += method_suffix + "_" if method_suffix.length() > 0
    method_name += module_method.native_name

    "rb_define_#{(is_static) ? 'singleton_':''}method(rb_c#{module_name}, \"#{module_method.name}\", #{method_name}, -1);"
end
%>

void Init_RubyAPI_<%= $cur_module.name %>(void)
{
<% if $cur_module.parents.size > 0 %>
    VALUE tmpParent = Qnil;
    rb_mParent = rb_define_module("<%= $cur_module.parents[0] %>");
    <% for i in 1..($cur_module.parents.size-1) %>
    tmpParent = rb_mParent;
    rb_mParent = rb_define_module_under(tmpParent, "<%= $cur_module.parents[i] %>");
    <% end %>

	rb_c<%= $cur_module.name %> = rb_define_class_under(rb_mParent, "<%= $cur_module.name %>", rb_cObject);
<% else %>
    rb_mParent = rho_ruby_get_NIL();
	rb_c<%= $cur_module.name %> = rb_define_class("<%= $cur_module.name %>", rb_cObject);
<% end %>
    //Constructor should be not available
	//rb_define_alloc_func(rb_cBarcode1, rb_barcode1_allocate);
    rb_undef_alloc_func(rb_c<%= $cur_module.name %>);

<% $cur_module.methods.each do |module_method|
%><%= api_generator_MakeRubyMethodDef($cur_module.name, module_method, module_method.access == ModuleMethod::ACCESS_STATIC, "" ) %>
<% if $cur_module.is_template_default_instance && module_method.access == ModuleMethod::ACCESS_INSTANCE
%><%= api_generator_MakeRubyMethodDef($cur_module.name, module_method, true, "def")%>
<% end
   end %>

<% if $cur_module.is_template_default_instance %>
    rb_define_singleton_method(rb_c<%= $cur_module.name %>, "default", rb_<%= $cur_module.name %>_s_default, 0);
    rb_define_singleton_method(rb_c<%= $cur_module.name %>, "setDefault", rb_<%= $cur_module.name %>_s_setDefault, 1);
<% end %>

<% $cur_module.constants.each do |module_constant| %>
    rb_const_set(rb_c<%= $cur_module.name %>, rb_intern("<%= module_constant.name %>"), <%= api_generator_CreateSimpleRubyType(module_constant.type, module_constant.value) %> );<%
end %>
<% $cur_module.method_aliases.each do |alias_item| %>
    rb_alias(rb_c<%= $cur_module.name %>, rb_intern("<%= alias_item.new_name %>"), rb_intern("<%= alias_item.existing_name %>"));<%
end %>

}

