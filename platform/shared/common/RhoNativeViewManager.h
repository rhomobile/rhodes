/*
 *  RhoNativeViewManager.h
 *  rhorunner
 *
 *  Created by Dmitry Soldatenkov on 8/25/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _RHO_NATIVE_VIEW_MANAGER_
#define _RHO_NATIVE_VIEW_MANAGER_


#include "common/RhoStd.h"
#include "ruby/ext/rho/rhoruby.h"


class NativeView {
public:
	virtual ~NativeView(){}
	
        // that function must return native object provided view functionality :
	// UIView* for iPhone
	// jobject for Android - jobect must be android.view.View class type
	// HWND for Windows Mobile 
	virtual void* getView() = 0;
	
	virtual void navigate(const char* url) = 0;


        // that function must return native object provided view functionality :
	// UIView* for iPhone
	// jobject for Android - jobect must be android.view.View class type
	// HWND for Windows Mobile 
        // this function executed when we make native view by Ruby NativeViewManager (not by URL prefix) 
        virtual void* createView(VALUE params) {return getView();}
};

class NativeViewFactory {
public:
	virtual ~NativeViewFactory(){}
	virtual NativeView* getNativeView(const char* viewType) = 0;
	virtual void destroyNativeView(NativeView* nativeView) = 0;
};



class RhoNativeViewManager {
public: 
	static void registerViewType(const char* viewType, NativeViewFactory* factory);
	static void unregisterViewType(const char* viewType);

	// that function return native object used for display Web content :
	// UIWebView* for iPhone
	// jobject for Android - jobect is android.webkit.WebView class type
	// HWND for Windows Mobile 
	static void* getWebViewObject(int tab_index);


        // destroy native view (opened with URL prefix or in separated full-screen window)
        // this function can executed from your native code (from NativeView code, for example)
        // instead of this function you can execute destroy() for Ruby NativeView object
        static void destroyNativeView(NativeView* nativeView);
};


class RhoNativeViewRunnable {
public:
	virtual ~RhoNativeViewRunnable(){}
	virtual void run() = 0;
};


class RhoNativeViewUtil {
public: 
	static void executeInUIThread_WM(RhoNativeViewRunnable* command);
};

#endif
