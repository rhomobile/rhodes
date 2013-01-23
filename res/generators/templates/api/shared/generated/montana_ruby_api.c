
#include "ruby.h"

static VALUE rb_mParent;
static VALUE rb_c<%= $cur_module.name %>;

<% $cur_module.methods.each do |module_method|
         method_name = 'rb_'
         if module_method.access == ModuleMethod::ACCESS_STATIC
            method_name = method_name + 's_'
         end

         method_name = method_name + module_method.name

         params = ''
         if module_method.params.size == 0
            if module_method.access == ModuleMethod::ACCESS_STATIC
               params = 'VALUE klass'
            else
               params = 'VALUE instance'
            end
         else
             if (module_method.params.size == 1) && (!module_method.params[0].can_be_nil)
                if module_method.access == ModuleMethod::ACCESS_STATIC
                   params = 'VALUE klass, VALUE obj'
                else
                   params = 'VALUE instance, VALUE obj'
                end
             else
                if module_method.access == ModuleMethod::ACCESS_STATIC
                   params = 'int argc, VALUE *argv, VALUE klass'
                else
                   params = 'int argc, VALUE *argv, VALUE instance'
                end
             end
         end
      %>VALUE <%= method_name%>(<%= params%>);
<% end %>



void Init_RubyAPI_<%= $cur_module.name %>(void)
{
    rb_mParent = rb_define_module("<%= $cur_module.parent %>");
	rb_c<%= $cur_module.name %> = rb_define_class_under(rb_mParent, "<%= $cur_module.name %>", rb_cObject);
	
    //Constructor should be not available
	//rb_define_alloc_func(rb_cBarcode1, rb_barcode1_allocate);
    rb_undef_alloc_func(rb_c<%= $cur_module.name %>);

<% $cur_module.methods.each do |module_method|
         method_name = 'rb_'
         if module_method.access == ModuleMethod::ACCESS_STATIC
            method_name = method_name + 's_'
         end

         method_name = method_name + module_method.name

         params = '-1'
         if module_method.params.size == 0
            params = '0'
         else
             if (module_method.params.size == 1) && (!module_method.params[0].can_be_nil)
                params = '1'
             end
         end
      if module_method.access == ModuleMethod::ACCESS_STATIC
%>    rb_define_singleton_method(rb_c<%= $cur_module.name %>, "<%= module_method.name %>", <%= method_name %>, <%= params %>);
<% else
%>    rb_define_method(rb_c<%= $cur_module.name %>, "<%= module_method.name %>", <%= method_name %>, <%= params %>);
<% end
end %>



}

