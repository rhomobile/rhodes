#include "..\Common\js_helpers.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Barcode1"

rho::String js_barcode1_enumerate(const rho::String& strID, rho::json::CJSONEntry& oParams);
rho::String js_barcode1_getProps(const rho::String& strID, rho::json::CJSONEntry& oParams);

extern "C" void Init_JSAPI_Barcode1(void)
{
    js_define_method("Barcode1:enumerate", js_barcode1_enumerate );
    js_define_method("Barcode1:getProps", js_barcode1_getProps );

    //tests
    js_entry_point("{'Module':'Barcode1', 'Method':'enumerate'}");
    js_entry_point("{'Module':'Barcode1', 'Method':'getProps', 'ObjID':'SC1'}");

/*    rb_mRho = rb_define_module("Rho");
	rb_cBarcode1 = rb_define_class_under(rb_mRho, "Barcode1", rb_cObject);
	
    //Constructor should be not available
	//rb_define_alloc_func(rb_cBarcode1, rb_barcode1_allocate);
    rb_undef_alloc_func(rb_cBarcode1);

    //Class fabric
    rb_define_singleton_method(rb_cBarcode1, "enumerate", rb_barcode1_s_enumerate, 0);

    rb_define_singleton_method(rb_cBarcode1, "default", rb_barcode1_s_default, 0);
    rb_define_singleton_method(rb_cBarcode1, "set_default", rb_barcode1_s_set_default, 1);

    rb_define_singleton_method(rb_cBarcode1, "getProps", rb_barcode1_s_getprops, -1);
    rb_define_method(rb_cBarcode1, "getProps", rb_barcode1_getprops, -1);*/
}

