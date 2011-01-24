#include "stdafx.h"

//#include <common/RhodesApp.h>
//#include <logging/RhoLogConf.h>


#include <stdlib.h>
//#include <strings.h>

#include "MainWindow.h"
#include "common/RhodesApp.h"

#include "logging/RhoLog.h"
#include "Utils.h"


#include "RhoNativeViewManagerWM.h" 

extern "C" HWND getWebViewWnd();
extern "C" HWND getMainWnd();

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
	HWND main_wnd = getMainWnd();//getWebViewWnd();
    return main_wnd;
}

// destroy native view (opened with URL prefix or in separated full-screen window)
// this function can executed from your native code (from NativeView code, for example)
// instead of this function you can execute destroy() for Ruby NativeView object
void RhoNativeViewManager::destroyNativeView(NativeView* nativeView) {
   //TODO

}



NativeViewFactory* RhoNativeViewManagerWM::getFactoryByViewType(const char* viewtype) {
	RhoNativeViewHolder* h = getHolderByViewTypeName(viewtype);
	if (h != NULL) {
		return h->factory;
	}
	return NULL;
}

void RhoNativeViewUtil::executeInUIThread_WM(RhoNativeViewRunnable* command) {
	HWND main_wnd = getMainWnd();
	::PostMessage(main_wnd, WM_EXECUTE_COMMAND, (WPARAM)command, 0);

}

extern "C" int rho_native_view_manager_create_native_view(const char* viewtype, int tab_index, VALUE params) {
	return -1;
}

extern "C" void rho_native_view_manager_navigate_native_view(int native_view_id, const char* url) {
}

extern "C" void rho_native_view_manager_destroy_native_view(int native_view_id) {
}

int RhoNativeViewManager::openNativeView(const char* viewType, int tab_index, VALUE params) {
	return rho_native_view_manager_create_native_view(viewType, tab_index, params);
}


void RhoNativeViewManager::closeNativeView(int v_id) {
	rho_native_view_manager_destroy_native_view(v_id);
}



