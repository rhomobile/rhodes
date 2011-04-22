using Microsoft.Scripting.Utils;
using Microsoft.Scripting.Runtime;
using IronRuby.Runtime;
using IronRuby.Builtins;
using System;
using System.Runtime.InteropServices;

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

            [RubyMethodAttribute("cancel", RubyMethodAttributes.PublicSingleton)]
            public static void Cancel(RubyModule/*!*/ self)
            {

            }

            [RubyMethodAttribute("do_request", RubyMethodAttributes.PublicSingleton)]
            public static MutableString doRequest(RubyModule/*!*/ self, [NotNull]String command, Hash args)
            {
                return null;
            }

            #endregion
        }
    }
}
