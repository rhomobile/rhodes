/*
 *  RhoNativeViewManagerWM.h
 *  rhorunner
 *
 *  Created by Dmitry Soldatenkov on 10/08/2010.
 *  Copyright 2010 Rhomobile. All rights reserved.
 *
 */

#ifndef _RHO_NATIVE_VIEW_MANAGER_WM_
#define _RHO_NATIVE_VIEW_MANAGER_WM_

#include "common/RhoNativeViewManager.h"

class RhoNativeViewManagerWM {
public: 
	static NativeViewFactory* getFactoryByViewType(const char* viewtype);
};

#endif
