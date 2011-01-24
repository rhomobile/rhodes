
// rhodes/platform/shared/common/RhoNativeViewManager.h
#include "RhoNativeViewManager.h"

#include "ESRIViewFactory.h"


	
//extern "C" void Init_ESRIRuby(void);	
extern "C" void register_ESRI_mapViewProvider();





extern "C" void Init_ESRI(void) {
    NativeViewFactory* factory = ESRIViewFactorySingletone::instance();
    RhoNativeViewManager::registerViewType("ESRI_View", factory);
	register_ESRI_mapViewProvider();
	//Init_ESRIRuby();
}
	
