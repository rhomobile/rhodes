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
    [RubyModule("RhoConf")]
    public static class RhoConfig
    {
        #region Private Implementation Details

        #endregion

        #region Private Instance & Singleton Methods

        [RubyMethodAttribute("set_property_by_name", RubyMethodAttributes.PublicSingleton)]
        public static void set_property_by_name(RubyModule/*!*/ self, [NotNull]String/*!*/ name, Object value)
        {
        }

        [RubyMethodAttribute("get_property_by_name", RubyMethodAttributes.PublicSingleton)]
        public static Object get_property_by_name(RubyModule/*!*/ self, [NotNull]String/*!*/ name)
        {
            return null;
        }

        [RubyMethodAttribute("is_property_exists", RubyMethodAttributes.PublicSingleton)]
        public static Boolean is_property_exists(RubyModule/*!*/ self, [NotNull]String/*!*/ name)
        {
            return true;
        }

        [RubyMethodAttribute("show_log", RubyMethodAttributes.PublicSingleton)]
        public static void show_log(RubyModule/*!*/ self)
        {
        }

        [RubyMethodAttribute("send_log", RubyMethodAttributes.PublicSingleton)]
        public static void send_log(RubyModule/*!*/ self)
        {
        }

        [RubyMethodAttribute("clean_log", RubyMethodAttributes.PublicSingleton)]
        public static void clean_log(RubyModule/*!*/ self)
        {
        }

        [RubyMethodAttribute("read_log", RubyMethodAttributes.PublicSingleton)]
        public static void read_log(RubyModule/*!*/ self, int limit = 0)
        {
        }

        #endregion
    }
}
