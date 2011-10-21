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

#include "rhodes/JNIRhodes.h"

#include "rhodes/jni/com_rhomobile_rhodes_RhoMenu.h"

#include <common/RhodesApp.h>
#include <common/AppMenu.h>

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Menu"

typedef rho::Vector<rho::common::CAppMenuItem> items_t;

static items_t &menu_items(jlong menu)
{
    return *reinterpret_cast<items_t*>(menu);
}

RHO_GLOBAL jlong JNICALL Java_com_rhomobile_rhodes_RhoMenu_allocMenu
  (JNIEnv *, jobject)
{
    std::auto_ptr<items_t> items(new items_t());
    RHODESAPP().getAppMenu().copyMenuItems(*items);
    return reinterpret_cast<jlong>(items.release());
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_RhoMenu_deallocMenu
  (JNIEnv *, jobject, jlong menu)
{
    delete &menu_items(menu);
}

RHO_GLOBAL jint JNICALL Java_com_rhomobile_rhodes_RhoMenu_getMenuSize
  (JNIEnv *, jobject, jlong menu)
{
    return menu_items(menu).size();
}

RHO_GLOBAL jstring JNICALL Java_com_rhomobile_rhodes_RhoMenu_getMenuItemLabel
  (JNIEnv *env, jobject, jlong menu, jint idx)
{
    return rho_cast<jhstring>(env, menu_items(menu)[idx].m_strLabel.c_str()).release();
}

RHO_GLOBAL jstring JNICALL Java_com_rhomobile_rhodes_RhoMenu_getMenuItemType
  (JNIEnv *env, jobject, jlong menu, jint idx)
{
    const char *s = "unknown";
    namespace r = rho::common;
    r::CAppMenuItem &item = menu_items(menu)[idx];
    switch (item.m_eType)
    {
        case r::CAppMenuItem::emtUrl: s = "url"; break;
        case r::CAppMenuItem::emtRefresh: s = "refresh"; break;
        case r::CAppMenuItem::emtHome: s = "home"; break;
        case r::CAppMenuItem::emtBack: s = "back"; break;
        case r::CAppMenuItem::emtSync: s = "sync"; break;
        case r::CAppMenuItem::emtOptions: s = "options"; break;
        case r::CAppMenuItem::emtLog: s = "log"; break;
        case r::CAppMenuItem::emtExit: s = "exit"; break;
        case r::CAppMenuItem::emtClose: s = "close"; break;
    }
    return rho_cast<jhstring>(env, s).release();
}

RHO_GLOBAL jstring JNICALL Java_com_rhomobile_rhodes_RhoMenu_getMenuItemUrl
  (JNIEnv *env, jobject, jlong menu, jint idx)
{
    return rho_cast<jhstring>(env, menu_items(menu)[idx].m_strLink.c_str()).release();
}

