
#include "ruby.h"

static VALUE rb_mRho;
static VALUE rb_c<%= $name %>;

<% if $xml_class_attributes_singletone_id %>
VALUE rb_<%= namefixed.downcase %>_s_enumerate(VALUE klass);
<% end %>

<% if $xml_class_attributes_propertybag %>
VALUE rb_<%= namefixed.downcase %>_s_getprops(int argc, VALUE *argv);
VALUE rb_<%= namefixed.downcase %>_getprops(int argc, VALUE *argv, VALUE obj);
VALUE rb_<%= namefixed.downcase %>_s_setprops(int argc, VALUE *argv);
VALUE rb_<%= namefixed.downcase %>_setprops(int argc, VALUE *argv, VALUE obj);
<% end %>

<% if $xml_class_attributes_default_instance %>
VALUE rb_<%= namefixed.downcase %>_s_default(VALUE klass);
VALUE rb_<%= namefixed.downcase %>_s_set_default(VALUE klass, VALUE obj);
<% end %>

void Init_RubyAPI_<%= $name %>(void)
{
    rb_mRho = rb_define_module("Rho");
	rb_c<%= $name %> = rb_define_class_under(rb_mRho, "<%= $name %>", rb_cObject);
	
    //Constructor should be not available
	//rb_define_alloc_func(rb_cBarcode1, rb_barcode1_allocate);
    rb_undef_alloc_func(rb_c<%= $name %>);

    //Class fabric
<% if $xml_class_attributes_singletone_id %>
    rb_define_singleton_method(rb_c<%= $name %>, "enumerate", rb_<%= namefixed.downcase %>_s_enumerate, 0);
<% end %>

<% if $xml_class_attributes_default_instance %>
    rb_define_singleton_method(rb_c<%= $name %>, "default", rb_<%= namefixed.downcase %>_s_default, 0);
    rb_define_singleton_method(rb_c<%= $name %>, "set_default", rb_<%= namefixed.downcase %>_s_set_default, 1);
<% end %>

<% if $xml_class_attributes_propertybag %>
    rb_define_singleton_method(rb_c<%= $name %>, "getProps", rb_<%= namefixed.downcase %>_s_getprops, -1);
    rb_define_method(rb_c<%= $name %>, "getProps", rb_<%= namefixed.downcase %>_getprops, -1);
    rb_define_singleton_method(rb_c<%= $name %>, "setProps", rb_<%= namefixed.downcase %>_s_setprops, -1);
    rb_define_method(rb_c<%= $name %>, "setProps", rb_<%= namefixed.downcase %>_setprops, -1);
<% end %>

}

