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

using Microsoft.Scripting.Utils;
using Microsoft.Scripting.Runtime;
using IronRuby.Runtime;
using IronRuby.Builtins;
using System;
using System.Runtime.InteropServices;

namespace rho.rubyext
{
    [RubyModule("NativeBar")]
    public static class RhoNativeBar
    {

        #region Private Implementation Details

        #endregion

        #region Private Instance & Singleton Methods

        [RubyMethodAttribute("create", RubyMethodAttributes.PublicSingleton)]
        public static void Create(RubyModule/*!*/ self, int barType, Object args)
        {

        }

        [RubyMethodAttribute("remove", RubyMethodAttributes.PublicSingleton)]
        public static void Remove(RubyModule/*!*/ self)
        {

        }

        [RubyMethodAttribute("switch_tab", RubyMethodAttributes.PublicSingleton)]
        public static void switchTab(RubyModule/*!*/ self, int index)
        {

        }

        [RubyMethodAttribute("started", RubyMethodAttributes.PublicSingleton)]
        public static Boolean isStarted(RubyModule/*!*/ self)
        {
            return true;
        }

        [RubyMethodAttribute("set_tab_badge", RubyMethodAttributes.PublicSingleton)]
        public static void setTabBadge(RubyModule/*!*/ self, int index, MutableString/*!*/ val)
        {

        }

        [RubyMethodAttribute("create_native_toolbar", RubyMethodAttributes.PublicSingleton)]
        public static void createNativeToolBar(RubyModule/*!*/ self, int barType, Object args)
        {

        }


        [RubyMethodAttribute("remove_native_toolbar", RubyMethodAttributes.PublicSingleton)]
        public static void removeNativeToolBar(RubyModule/*!*/ self)
        {

        }

        [RubyMethodAttribute("create_native_tabbar", RubyMethodAttributes.PublicSingleton)]
        public static void createNativeTabBar(RubyModule/*!*/ self, int barType, Object args)
        {

        }

        [RubyMethodAttribute("remove_native_tabbar", RubyMethodAttributes.PublicSingleton)]
        public static void removeNativeTabBar(RubyModule/*!*/ self)
        {

        }

        [RubyMethodAttribute("native_tabbar_switch_tab", RubyMethodAttributes.PublicSingleton)]
        public static void nativeTabBarSwitchTab(RubyModule/*!*/ self, int index)
        {

        }

        [RubyMethodAttribute("native_tabbar_set_tab_badge", RubyMethodAttributes.PublicSingleton)]
        public static void nativeTabBarSetTabBadge(RubyModule/*!*/ self, int index, MutableString/*!*/ val)
        {

        }

        [RubyMethodAttribute("native_tabbar_get_current_tab", RubyMethodAttributes.PublicSingleton)]
        public static int nativeTabBarGetCurrentTab(RubyModule/*!*/ self)
        {
            return 0;
        }

        #endregion
    }
}
