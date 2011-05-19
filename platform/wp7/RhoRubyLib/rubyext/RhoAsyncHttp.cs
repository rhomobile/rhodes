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
