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


class NativeView {
public:
	// that function must return native object provided view functionality :
	// UIView* for iPhone
	// jobject for Android - jobect must be android.view.View class type
	// HWND for Windows Mobile 
	virtual void* getView() = 0;
	
	virtual void navigate(const char* url) = 0;
};

class NativeViewFactory {
public:
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
};

#endif
