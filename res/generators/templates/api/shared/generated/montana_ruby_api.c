#include "ruby.h"

static VALUE rb_api_mParent;
static VALUE rb_api_m<%= $cur_module.name %>;

<% $cur_module.methods.each do |module_method|
%><%= api_generator_MakeRubyMethodDecl($cur_module.name, module_method, module_method.access == ModuleMethod::ACCESS_STATIC)%>;
<% if $cur_module.is_template_default_instance && module_method.access == ModuleMethod::ACCESS_INSTANCE && !module_method.is_constructor
%><%= api_generator_MakeRubyMethodDecl($cur_module.name + "_def", module_method, true)%>;
<% end
   end %>

<% if $cur_module.is_template_default_instance %>
VALUE rb_<%= $cur_module.name %>_s_default(VALUE klass);
VALUE rb_<%= $cur_module.name %>_s_setDefault(VALUE klass, VALUE obj);
<% end %>

VALUE getRuby_<%= $cur_module.name %>_Module(){ return rb_api_m<%= $cur_module.name %>; }

<%
def api_generator_MakeRubyMethodDef(module_name, module_method, is_static, method_suffix)
    method_name = 'rb_'
    method_name += 's_' if is_static
    method_name += module_name + "_"
    method_name += method_suffix + "_" if method_suffix.length() > 0
    method_name += module_method.native_name
    api_name = module_method.is_constructor ? "initialize" : module_method.name

    "    rb_define_#{(is_static) ? 'singleton_method':'method'}(rb_api_m#{module_name}, \"#{api_name}\", #{method_name}, -1);"
end
%>

static void _free_class_object(void *p)
{
    ruby_xfree(p);
}

static VALUE _allocate_class_object(VALUE klass)
{
    VALUE valObj = 0;
    char ** ppString = NULL;
    void* pData = ALLOC(void*);
    memset( pData, 0, sizeof(pData) );
    
	valObj = Data_Wrap_Struct(klass, 0, _free_class_object, pData);

    Data_Get_Struct(valObj, char *, ppString);
    *ppString = xmalloc(10);
    sprintf(*ppString, "%X", valObj);

    return valObj;
}

void Init_RubyAPI_<%= $cur_module.name %>(void)
{
<% if $cur_module.parents.size > 0 %>
    VALUE tmpParent = Qnil;
    rb_api_mParent = rb_define_module("<%= $cur_module.parents[0] %>");
    <% for i in 1..($cur_module.parents.size-1) %>
    tmpParent = rb_api_mParent;
    if (rb_const_defined_at(tmpParent, rb_intern("<%= $cur_module.parents[i] %>")))
        rb_api_mParent = rb_const_get_at(tmpParent, rb_intern("<%= $cur_module.parents[i] %>"));
    else
        rb_api_mParent = rb_define_module_under(tmpParent, "<%= $cur_module.parents[i] %>");
    <% end %>

	rb_api_m<%= $cur_module.name %> = rb_define_class_under(rb_api_mParent, "<%= $cur_module.name %>", rb_cObject);
<% else %>
    rb_api_mParent = rho_ruby_get_NIL();
	rb_api_m<%= $cur_module.name %> = rb_define_class_under(rb_api_mParent, "<%= $cur_module.name %>", rb_cObject);
<% end %>
	rb_define_alloc_func(rb_api_m<%= $cur_module.name %>, _allocate_class_object);
    //Constructor should be not available in case of static members
    //rb_undef_alloc_func(rb_api_m<%= $cur_module.name %>);

<% $cur_module.methods.each do |module_method|
%><%= api_generator_MakeRubyMethodDef($cur_module.name, module_method, module_method.access == ModuleMethod::ACCESS_STATIC, "" ) %>
<% if $cur_module.is_template_default_instance && module_method.access == ModuleMethod::ACCESS_INSTANCE && !module_method.is_constructor
%><%= api_generator_MakeRubyMethodDef($cur_module.name, module_method, true, "def")%>
<% end
   end %>

<% if $cur_module.is_template_default_instance %>
    rb_define_singleton_method(rb_api_m<%= $cur_module.name %>, "getDefaultInstance", rb_<%= $cur_module.name %>_s_default, 0);
    rb_define_singleton_method(rb_api_m<%= $cur_module.name %>, "setDefaultInstance", rb_<%= $cur_module.name %>_s_setDefault, 1);
    rb_define_singleton_method(rb_api_m<%= $cur_module.name %>, "defaultInstance=", rb_<%= $cur_module.name %>_s_setDefault, 1);
<% end %>

<% $cur_module.constants.each do |module_constant| %>
    rb_const_set(rb_api_m<%= $cur_module.name %>, rb_intern("<%= module_constant.name %>"), <%= api_generator_CreateSimpleRubyType(module_constant.type, module_constant.value) %> );<%
end %>
<% $cur_module.method_aliases.each do |alias_item|
   if alias_item.is_method_instance %>
    rb_define_alias(rb_api_m<%= $cur_module.name %>, "<%= alias_item.new_name %>", "<%= alias_item.existing_name %>");
    <% end
    if alias_item.is_method_static %>
    rb_define_alias(rb_singleton_class(rb_api_m<%= $cur_module.name %>), "<%= alias_item.new_name %>", "<%= alias_item.existing_name %>");<%
    end
end %>

<% $cur_module.module_aliases.each do |alias_item| %>
    rb_const_set(rb_mKernel, rb_intern("<%= alias_item.new_name %>"), rb_api_m<%= $cur_module.name %> );<%
end %>
}

