#include <rhodes.h>
#include <android/log.h>
#include <common/RhodesApp.h>
#include <logging/RhoLogConf.h>
#include "rhodes/JNIRhodes.h"


#include <stdlib.h>
#include <strings.h>


#import <RhoNativeViewManager.h> 


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


RHO_GLOBAL jobject JNICALL Java_com_rhomobile_rhodes_nativeview_RhoNativeViewManager_getViewByHandle
(JNIEnv *env, jclass, jlong handle) {
	NativeView* p = (NativeView*)handle;
	if (p == NULL) {
		return NULL;
	}
	return (jobject)p->getView();
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_nativeview_RhoNativeViewManager_navigateByHandle
(JNIEnv *env, jclass, jlong handle, jstring url) {
	NativeView* p = (NativeView*)handle;
	if (p != NULL) {
		p->navigate(rho_cast<std::string>(url).c_str());
	}
}

RHO_GLOBAL jlong JNICALL Java_com_rhomobile_rhodes_nativeview_RhoNativeViewManager_getFactoryHandleByViewType
(JNIEnv *env, jclass, jstring viewtype) {
	const char* s_viewtype = rho_cast<std::string>(viewtype).c_str();
	RhoNativeViewHolder* nvh = getHolderByViewTypeName(s_viewtype);
	return (jlong)nvh;
}

RHO_GLOBAL jlong JNICALL Java_com_rhomobile_rhodes_nativeview_RhoNativeViewManager_getViewHandleByFactoryHandle
(JNIEnv *env, jclass, jlong factory_h) {
	RhoNativeViewHolder* holder = (RhoNativeViewHolder*)factory_h;
	if (holder == NULL) {
		return 0;
	}
	return (jlong)holder->factory->getNativeView((const char*)holder->viewtype);
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

