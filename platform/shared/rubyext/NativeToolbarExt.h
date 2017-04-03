/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/

#ifndef _NATIVETOOLBAREXT_H_
#define _NATIVETOOLBAREXT_H_

#include "common/RhoStd.h"

#if defined(OS_WP8) || defined(OS_UWP)
#define TOOLBAR_TYPE		0
#define TABBAR_TYPE			1
#define NOBAR_TYPE			2
#define VTABBAR_TYPE		3
#endif

#if defined(OS_MACOSX)
bool rho_osximpl_toolbar_isStarted();
void rho_osximpl_toolbar_create(const rho::Vector<rho::String>& toolbarElements, const rho::Hashtable<rho::String, rho::String>& toolBarProperties);
void rho_osximpl_toolbar_remove();

bool rho_osximpl_tabbar_isStarted();
void rho_osximpl_tabbar_create(const rho::Vector<rho::String>& tabbarElements, const rho::Hashtable<rho::String, rho::String>& tabBarProperties, rho::apiGenerator::CMethodResult& oResult);
void rho_osximpl_tabbar_remove();
int rho_osximpl_tabbar_currentTabIndex();
void rho_osximpl_tabbar_switchTab(int tabIndex);
void rho_osximpl_tabbar_setTabBadge(int tabIndex, const rho::String& badge);
#else
bool rho_wmimpl_toolbar_isStarted();
void rho_wmimpl_toolbar_create( const rho::Vector<rho::String>& toolbarElements,  const rho::Hashtable<rho::String, rho::String>& toolBarProperties);
void rho_wmimpl_toolbar_remove();

bool rho_wmimpl_tabbar_isStarted();
void rho_wmimpl_tabbar_create( const rho::Vector<rho::String>& tabbarElements,  const rho::Hashtable<rho::String, rho::String>& tabBarProperties, rho::apiGenerator::CMethodResult& oResult);
void rho_wmimpl_tabbar_remove();
void rho_wmimpl_tabbar_removeTab(int tabIndex);
int rho_wmimpl_tabbar_currentTabIndex();
void rho_wmimpl_tabbar_switchTab(int tabIndex);
void rho_wmimpl_tabbar_setTabBadge(int tabIndex,  const rho::String& badge);
#endif

#endif //_NATIVETOOLBAREXT_H_
