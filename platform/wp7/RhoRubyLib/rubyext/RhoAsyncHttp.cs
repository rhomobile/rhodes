using Microsoft.Scripting.Utils;
using Microsoft.Scripting.Runtime;
using IronRuby.Runtime;
using IronRuby.Builtins;
using System;
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

            #endregion

            #region Private Instance & Singleton Methods

            [RubyMethodAttribute("cancel", RubyMethodAttributes.PublicSingleton)]
            public static void Cancel(RubyModule/*!*/ self, [NotNull]String cancelCallback)
            {
                if (CAsyncHttp.getInstance() != null)
                    CAsyncHttp.getInstance().cancelRequest(cancelCallback);
            }

            [RubyMethodAttribute("cancel", RubyMethodAttributes.PublicSingleton)]
            public static void Cancel(RubyModule/*!*/ self)
            {
                if (CAsyncHttp.getInstance() != null)
                    CAsyncHttp.getInstance().cancelRequest("*");
            }

            [RubyMethodAttribute("do_request", RubyMethodAttributes.PublicSingleton)]
            public static MutableString doRequest(RubyModule/*!*/ self, [NotNull]String command, Hash args)
            {
                CAsyncHttp.Create();
                RhoParams p = new RhoParams(args);
                return CAsyncHttp.getInstance().addHttpCommand(new CAsyncHttp.HttpCommand(command, p));
            }

            #endregion
        }
    }
}
