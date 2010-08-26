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

class UIView;

class NativeView {
public:
	virtual UIView* getView() = 0;
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
};

#endif
