using Microsoft.Scripting.Utils;
using Microsoft.Scripting.Runtime;
using IronRuby.Runtime;
using IronRuby.Builtins;
using System;
using System.Runtime.InteropServices;
using rho.common;

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

            }

            [RubyMethodAttribute("do_request", RubyMethodAttributes.PublicSingleton)]
            public static void doRequest(RubyModule/*!*/ self, [NotNull]String command, Hash args)
            {

            }

            #endregion
        }
    }
}
