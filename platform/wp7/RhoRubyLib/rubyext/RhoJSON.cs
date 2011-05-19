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
    public static class Rho
    {
        [RubyModule("JSON")]
        public static class RhoJSON
        {
            #region Private Implementation Details

            private static RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() :
                new RhoLogger("RhoJSON");

            #endregion

            #region Private Instance & Singleton Methods

            [RubyMethodAttribute("parse", RubyMethodAttributes.PublicSingleton)]
            public static object parse(RubyModule/*!*/ self, [NotNull]string/*!*/ strData)
            {
                object res = null;
                try
                {
                    res = fastJSON.RJSONTokener.JsonDecode(strData);
                }
                catch (Exception ex)
                {
                    Exception rubyEx = self.Context.CurrentException;
                    if (rubyEx == null)
                    {
                        rubyEx = RubyExceptionData.InitializeException(new RuntimeError(ex.Message.ToString()), ex.Message);
                    }
                    LOG.ERROR("parse", ex);
                    throw rubyEx;
                }

                return res;
            }

            [RubyMethodAttribute("quote_value", RubyMethodAttributes.PublicSingleton)]
            public static object quote_value(RubyModule/*!*/ self, [NotNull]string/*!*/ strData)
            {
                object res = null;
                try
                {
                    String strRes = rho.json.JSONEntry.quoteValue(strData);
                    res = CRhoRuby.Instance.createString(strRes);
                }
                catch (Exception ex)
                {
                    Exception rubyEx = self.Context.CurrentException;
                    if (rubyEx == null)
                    {
                        rubyEx = RubyExceptionData.InitializeException(new RuntimeError(ex.Message.ToString()), ex.Message);
                    }
                    LOG.ERROR("quote_value", ex);
                    throw rubyEx;
                }

                return res;
            }

            #endregion
        }
    }
}
