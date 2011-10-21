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

#include "common/RhoPort.h"
#include "ext/rho/rhoruby.h"
#include "common/RhodesApp.h"
#include "logging/RhoLog.h"

extern "C" {

int rho_native_view_manager_create_native_view(const char* viewtype, int tab_index, VALUE params)
{
    //TODO: rho_native_view_manager_create_native_view

    //RhoNativeViewHolder* h = getHolderByViewTypeName(viewtype);
    //if (h == NULL) {
    //    return -1;
    //}
    //NativeViewFactory* factory = h->factory;
    //NativeView* nv = factory->getNativeView(viewtype);
    //if (nv == NULL) {
    //    return -1;
    //}

    //RhoOpenedNativeView* opened_view = new RhoOpenedNativeView();
    //opened_view->factory_holder = h;
    //opened_view->n_view = nv;
    //opened_view->tab_index = tab_index;

    //addRhoNativeOpenedView(opened_view);

    //RhoNativeViewRunnable_OpenViewCommand* open_command = new RhoNativeViewRunnable_OpenViewCommand(opened_view);

    //RhoNativeViewUtil::executeInUIThread_WM(open_command);

    //return opened_view->id;

    return -1;
}

void rho_native_view_manager_navigate_native_view(int native_view_id, const char* url)
{
    //TODO: rho_native_view_manager_navigate_native_view

    //RhoOpenedNativeView* opened_view = getOpenedViewByID(native_view_id);
    //if (opened_view != NULL) {
    //    opened_view->n_view->navigate(url);
    //}
}

void rho_native_view_manager_destroy_native_view(int native_view_id)
{
    //TODO: rho_native_view_manager_destroy_native_view

    //RhoOpenedNativeView* opened_view = getOpenedViewByID(native_view_id);
    //if (opened_view != NULL) {
    //    removeRhoNativeOpenedView(opened_view);
    //    RhoNativeViewRunnable_CloseViewCommand* close_command = new RhoNativeViewRunnable_CloseViewCommand();
    //    RhoNativeViewUtil::executeInUIThread_WM(close_command);
    //}
}

} //extern "C"
