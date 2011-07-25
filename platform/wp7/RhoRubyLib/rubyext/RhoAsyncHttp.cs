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
using System.Collections.Generic;
using System.Runtime.InteropServices;
using rho.common;
using rho.net;

namespace rho.rubyext
{
    [RubyModule("Rho")]
    public static class RhoRoot
    {
        [RubyModule("AsyncHttp")]
        public static class RhoAsyncHttp
        {

            #region Private Implementation Details

            private static RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() :
            new RhoLogger("RhoAsyncHttp");

            #endregion

            #region Private Instance & Singleton Methods

            [RubyMethodAttribute("cancel", RubyMethodAttributes.PublicSingleton)]
            public static void Cancel(RubyModule/*!*/ self, [NotNull]String cancelCallback)
            {
                try
                {
                    if (CAsyncHttp.getInstance() != null)
                        CAsyncHttp.getInstance().cancelRequest(cancelCallback);
                }
                catch (Exception ex)
                {
                    Exception rubyEx = self.Context.CurrentException;
                    if (rubyEx == null)
                    {
                        rubyEx = RubyExceptionData.InitializeException(new RuntimeError(ex.Message.ToString()), ex.Message);
                    }
                    LOG.ERROR("cancel", ex);
                    throw rubyEx;
                }
            }

            [RubyMethodAttribute("cancel", RubyMethodAttributes.PublicSingleton)]
            public static void Cancel(RubyModule/*!*/ self)
            {
                try
                {
                    if (CAsyncHttp.getInstance() != null)
                        CAsyncHttp.getInstance().cancelRequest("*");
                }
                catch (Exception ex)
                {
                    Exception rubyEx = self.Context.CurrentException;
                    if (rubyEx == null)
                    {
                        rubyEx = RubyExceptionData.InitializeException(new RuntimeError(ex.Message.ToString()), ex.Message);
                    }
                    LOG.ERROR("cancel", ex);
                    throw rubyEx;
                }
            }

            [RubyMethodAttribute("do_request", RubyMethodAttributes.PublicSingleton)]
            public static MutableString doRequest(RubyModule/*!*/ self, [NotNull]String command, Hash args)
            {
                MutableString res = null;
                try
                {
                    CAsyncHttp.Create();
                    RhoParams p = new RhoParams(args);
                    res = CAsyncHttp.getInstance().addHttpCommand(new CAsyncHttp.HttpCommand(command, p));
                }
                catch (Exception ex)
                {
                    Exception rubyEx = self.Context.CurrentException;
                    if (rubyEx == null)
                    {
                        rubyEx = RubyExceptionData.InitializeException(new RuntimeError(ex.Message.ToString()), ex.Message);
                    }
                    LOG.ERROR("do_request", ex);
                    throw rubyEx;
                }
                return res;
            }

            #endregion
        }
    }
}
