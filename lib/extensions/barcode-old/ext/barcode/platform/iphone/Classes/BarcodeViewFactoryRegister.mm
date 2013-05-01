
// rhodes/platform/shared/common/RhoNativeViewManager.h
#include "common/RhoNativeViewManager.h"

#include "BarcodeViewFactory.h"


	
//extern "C" void register_Barcode_mapViewProvider();




extern "C" void register_Barcode_View(void) {
    NativeViewFactory* factory = BarcodeViewFactorySingletone::instance();
    RhoNativeViewManager::registerViewType("Barcode_View", factory);
}
	
