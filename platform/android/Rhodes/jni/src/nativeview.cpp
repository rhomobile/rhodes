#include <rhodes.h>
#include <android/log.h>
#include <common/RhodesApp.h>
#include <logging/RhoLogConf.h>
#include "rhodes/JNIRhodes.h"


#include <stdlib.h>
#include <strings.h>


#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "NativeView"

#define logging_enable false



#include <RhoNativeViewManager.h> 


class RhoNativeViewHolder{
  public :
	RhoNativeViewHolder() {
		factory = NULL;
		next = NULL;
		viewtype = NULL;
	}
	virtual ~RhoNativeViewHolder() {
		if (viewtype != NULL) {
			delete viewtype;
		}
	}
	void setViewType(const char* viewtypename) {
		viewtype = new char[strlen(viewtypename)+2];
		strcpy(viewtype, viewtypename);
	}
	bool isApplicable(const char* viewtypename) {
		return (strcmp(viewtype, viewtypename) == 0);
	}
	char* viewtype;
	NativeViewFactory* factory;
	RhoNativeViewHolder* next;
};

static RhoNativeViewHolder* first = NULL;


static void addRhoNativeViewHolder(RhoNativeViewHolder* holder) {
	if (first == NULL) {
		first = holder;
		holder->next = NULL;
	}
	else {
		holder->next = first;
		first = holder;
	}
}

static void removeRhoNativeViewHolder(RhoNativeViewHolder* holder) {
	RhoNativeViewHolder* p = first;
	RhoNativeViewHolder* prev = NULL;
	while (p != NULL) {
		if (p == holder) {
			RhoNativeViewHolder* next = p->next;
			if (prev != NULL) {
				prev->next = next;
			}
			if (first == p) {
				first = NULL;
			}
			delete p;
		}
		prev = p;
		p = p->next;
	}
}

static RhoNativeViewHolder* getHolderByViewTypeName(const char* name) {
	RhoNativeViewHolder* p = first;
	while (p != NULL) {
		if (p->isApplicable(name)) {
			return p;
		}
		p = p->next;
	}
	return NULL;
}


void RhoNativeViewManager::registerViewType(const char* viewType, NativeViewFactory* factory) {
	RhoNativeViewHolder* holder = new RhoNativeViewHolder();
	holder->factory = factory;
	holder->setViewType(viewType);
	addRhoNativeViewHolder(holder);
}

void RhoNativeViewManager::unregisterViewType(const char* viewType) {
	RhoNativeViewHolder* holder = getHolderByViewTypeName(viewType);
	if (holder != NULL) {
		removeRhoNativeViewHolder(holder);
	}
}

// that function return native object used for display Web content :
// UIWebView* for iPhone
// jobject for Android - jobect is android.webkit.WebView class type
// HWND for Windows Mobile 
void* RhoNativeViewManager::getWebViewObject(int tab_index) {
    JNIEnv *env = jnienv();
    jclass cls = rho_find_class(env, "com/rhomobile/rhodes/nativeview/RhoNativeViewManager");
    if (!cls) return null;
    jmethodID mid = env->GetStaticMethodID( cls, "getWebViewObject", "(I)Landroid/webkit/WebView;");
    if (!mid) return null;
    return env->CallStaticObjectMethod(cls, mid, tab_index);
}

// destroy native view (opened with URL prefix or in separated full-screen window)
// this function can executed from your native code (from NativeView code, for example)
// instead of this function you can execute destroy() for Ruby NativeView object
void RhoNativeViewManager::destroyNativeView(NativeView* nativeView) {
   //TODO

}



RHO_GLOBAL jobject JNICALL Java_com_rhomobile_rhodes_nativeview_RhoNativeViewManager_getViewByHandle
(JNIEnv *env, jclass, jlong handle) {
	NativeView* p = (NativeView*)handle;
	if (p == NULL) {
		return NULL;
	}
	return (jobject)(p->getView());
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_nativeview_RhoNativeViewManager_navigateByHandle
(JNIEnv *env, jclass, jlong handle, jstring url) {
    	if (logging_enable) RAWLOG_INFO("navigateByHandle() START");

	NativeView* p = (NativeView*)handle;
	if (p != NULL) {
    		if (logging_enable) RAWLOG_INFO("navigateByHandle() handle not NULL");
		p->navigate(rho_cast<std::string>(url).c_str());
	}
    	if (logging_enable) RAWLOG_INFO("navigateByHandle() FINISH");
}

RHO_GLOBAL jlong JNICALL Java_com_rhomobile_rhodes_nativeview_RhoNativeViewManager_getFactoryHandleByViewType
(JNIEnv *env, jclass, jstring viewtype) {
	const char* s_viewtype = rho_cast<std::string>(viewtype).c_str();
	RhoNativeViewHolder* nvh = getHolderByViewTypeName(s_viewtype);
	return (jlong)((unsigned long int)nvh);
}

RHO_GLOBAL jlong JNICALL Java_com_rhomobile_rhodes_nativeview_RhoNativeViewManager_getViewHandleByFactoryHandle
(JNIEnv *env, jclass, jlong factory_h) {
	RhoNativeViewHolder* holder = (RhoNativeViewHolder*)factory_h;
	if (holder == NULL) {
		return 0;
	}
	return (jlong)((unsigned long int)holder->factory->getNativeView((const char*)holder->viewtype));
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_nativeview_RhoNativeViewManager_destroyByHandle
(JNIEnv *env, jclass, jlong factory_h, jlong view_h) {
	RhoNativeViewHolder* holder = (RhoNativeViewHolder*)factory_h;
	NativeView* nv = (NativeView*)view_h;
	if ((holder == NULL) || (nv == NULL)) {
		return;
	}
	holder->factory->destroyNativeView(nv);
}

extern "C" int rho_native_view_manager_create_native_view(const char* viewtype, int tab_index, VALUE params) {
	return -1;
}

extern "C" void rho_native_view_manager_navigate_native_view(int native_view_id, const char* url) {
}

extern "C" void rho_native_view_manager_destroy_native_view(int native_view_id) {
}






