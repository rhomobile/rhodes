using Microsoft.Scripting.Utils;
using Microsoft.Scripting.Runtime;
using IronRuby.Runtime;
using IronRuby.Builtins;
using System;
using System.Runtime.InteropServices;

namespace rho.rubyext
{
    [RubyModule("Rho")]
    public static class Rho
    {
        [RubyModule("JSON")]
        public static class RhoJSON
        {
            #region Private Implementation Details

            #endregion

            #region Private Instance & Singleton Methods

            [RubyMethodAttribute("parse", RubyMethodAttributes.PublicSingleton)]
            public static object parse(RubyModule/*!*/ self, [NotNull]string/*!*/ strData)
            {
                return null;
            }

            [RubyMethodAttribute("quote_value", RubyMethodAttributes.PublicSingleton)]
            public static object quote_value(RubyModule/*!*/ self, [NotNull]string/*!*/ strData)
            {
                return null;
            }

            #endregion
        }
    }
}
