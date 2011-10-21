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
    [RubyModule("WebView")]
    public static class RhoWebView
    {
        #region Private Implementation Details

        #endregion

        #region Private Instance & Singleton Methods

        [RubyMethodAttribute("navigate", RubyMethodAttributes.PublicSingleton)]
        public static void Navigate(RubyModule/*!*/ self, [NotNull]String/*!*/ url, int index = 0)
        {
        }

        [RubyMethodAttribute("refresh", RubyMethodAttributes.PublicSingleton)]
        public static void Refresh(RubyModule/*!*/ self, int index = 0)
        {
        }

        [RubyMethodAttribute("execute_js", RubyMethodAttributes.PublicSingleton)]
        public static void execute_js(RubyModule/*!*/ self, [NotNull]String/*!*/ strScript, int index = 0)
        {
        }

        [RubyMethodAttribute("current_location", RubyMethodAttributes.PublicSingleton)]
        public static String currentLocation(RubyModule/*!*/ self)
        {
            return null;
        }

        [RubyMethodAttribute("set_menu_items", RubyMethodAttributes.PublicSingleton)]
        public static void setMenuItems(RubyModule/*!*/ self, Hash valMenu)
        {
        }

        #endregion
    }
}
