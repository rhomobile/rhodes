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
CMainWindow* Rhodes_getMainWindow();


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

static int ourGlobalID = 1;

class RhoOpenedNativeView {
public:
	RhoOpenedNativeView() {
		next = NULL;
		id = ourGlobalID++;
		tab_index = 0;
		factory_holder = NULL;
		n_view = NULL;
	}
	RhoOpenedNativeView* next;
	int id;
	int tab_index;
	RhoNativeViewHolder* factory_holder;
	NativeView* n_view;
};


class RhoNativeViewRunnable_OpenViewCommand : public RhoNativeViewRunnable {
public:
	RhoNativeViewRunnable_OpenViewCommand(RhoOpenedNativeView* view) {
		mView = view;
	}
	virtual void run() {
		CMainWindow* mw = Rhodes_getMainWindow();
		String sn(mView->factory_holder->viewtype);
#ifndef RHODES_EMULATOR
		mw->openNativeView(mView->factory_holder->factory, mView->n_view, sn);
#endif
		//delete this;
	}

private:
	RhoOpenedNativeView* mView;
};

class RhoNativeViewRunnable_CloseViewCommand : public RhoNativeViewRunnable {
public:
	RhoNativeViewRunnable_CloseViewCommand() {
	}
	virtual void run() {
		CMainWindow* mw = Rhodes_getMainWindow();
#ifndef RHODES_EMULATOR
		mw->closeNativeView();
#endif
		//delete this;
	}
};





static RhoNativeViewHolder* first = NULL;
static RhoOpenedNativeView* first_opened = NULL;









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







static void addRhoNativeOpenedView(RhoOpenedNativeView* view) {
	if (first_opened == NULL) {
		first_opened = view;
		view->next = NULL;
	}
	else {
		view->next = first_opened;
		first_opened = view;
	}
}

static void removeRhoNativeOpenedView(RhoOpenedNativeView* view) {
	RhoOpenedNativeView* p = first_opened;
	RhoOpenedNativeView* prev = NULL;
	while (p != NULL) {
		if (p == view) {
			RhoOpenedNativeView* next = p->next;
			if (prev != NULL) {
				prev->next = next;
			}
			if (first_opened == p) {
				first_opened = NULL;
			}
			delete p;
		}
		prev = p;
		p = p->next;
	}
}



static RhoOpenedNativeView* getOpenedViewByID(int v_id) {
	RhoOpenedNativeView* p = first_opened;
	while (p != NULL) {
		if (p->id == v_id) {
			return p;
		}
		p = p->next;
	}
	return NULL;
}


static RhoOpenedNativeView* getOpenedViewByNativeView(NativeView* nv) {
	RhoOpenedNativeView* p = first_opened;
	while (p != NULL) {
		if (p->n_view == nv) {
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
	RhoNativeViewHolder* h = getHolderByViewTypeName(viewtype);
	if (h == NULL) {
		return -1;
	}
	NativeViewFactory* factory = h->factory;
	NativeView* nv = factory->getNativeView(viewtype);
	if (nv == NULL) {
		return -1;
	}
	//String vtype = new String(viewtype);
	
	RhoOpenedNativeView* opened_view = new RhoOpenedNativeView();
	opened_view->factory_holder = h;
	opened_view->n_view = nv;
	opened_view->tab_index = tab_index;

	addRhoNativeOpenedView(opened_view);

	RhoNativeViewRunnable_OpenViewCommand* open_command = new RhoNativeViewRunnable_OpenViewCommand(opened_view);

	RhoNativeViewUtil::executeInUIThread_WM(open_command);

	return opened_view->id;
}

extern "C" void rho_native_view_manager_navigate_native_view(int native_view_id, const char* url) {
	RhoOpenedNativeView* opened_view = getOpenedViewByID(native_view_id);
	if (opened_view != NULL) {
		opened_view->n_view->navigate(url);
	}
}

extern "C" void rho_native_view_manager_destroy_native_view(int native_view_id) {
	RhoOpenedNativeView* opened_view = getOpenedViewByID(native_view_id);
	if (opened_view != NULL) {
		removeRhoNativeOpenedView(opened_view);
		RhoNativeViewRunnable_CloseViewCommand* close_command = new RhoNativeViewRunnable_CloseViewCommand();
		RhoNativeViewUtil::executeInUIThread_WM(close_command);
	}
}




// destroy native view (opened with URL prefix or in separated full-screen window)
// this function can executed from your native code (from NativeView code, for example)
// instead of this function you can execute destroy() for Ruby NativeView object
void RhoNativeViewManager::destroyNativeView(NativeView* nativeView) {
	RhoOpenedNativeView* opened_view = getOpenedViewByNativeView(nativeView);
	if (opened_view != NULL) {
		RhoNativeViewRunnable_CloseViewCommand* close_command = new RhoNativeViewRunnable_CloseViewCommand();
		RhoNativeViewUtil::executeInUIThread_WM(close_command);
	}
}

int RhoNativeViewManager::openNativeView(const char* viewType, int tab_index, VALUE params) {
	return rho_native_view_manager_create_native_view(viewType, tab_index, params);
}


void RhoNativeViewManager::closeNativeView(int v_id) {
	rho_native_view_manager_destroy_native_view(v_id);
}










