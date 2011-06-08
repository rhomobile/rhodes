using Microsoft.Scripting.Utils;
using Microsoft.Scripting.Runtime;
using IronRuby.Runtime;
using IronRuby.Builtins;
using System;
using System.Runtime.InteropServices;
using rho.common;

namespace rho.rubyext
{
    [RubyModule("RhoConf")]
    public static class RhoConfig
    {
        private static RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() :
            new RhoLogger("RhoConfig");
        private static CRhoRuby RhoRuby { get { return CRhoRuby.Instance; } }
        private static CRhodesApp RHODESAPP() { return CRhodesApp.Instance; }

        #region Private Implementation Details

        #endregion

        #region Private Instance & Singleton Methods

        [RubyMethodAttribute("set_property_by_name", RubyMethodAttributes.PublicSingleton)]
        public static void set_property_by_name(RubyModule/*!*/ self, [NotNull]String/*!*/ name, Object value)
        {
            try
            {
                RhoConf.getInstance().setString(name, value as String, true);

                RhoLogger.getLogConf().loadFromConf(RhoConf.getInstance());
            }
            catch (Exception ex)
            {
                Exception rubyEx = self.Context.CurrentException;
                if (rubyEx == null)
                {
                    rubyEx = RubyExceptionData.InitializeException(new RuntimeError(ex.Message.ToString()), ex.Message);
                }
                LOG.ERROR("set_property_by_name", ex);
                throw rubyEx;
            }
        }

        [RubyMethodAttribute("get_property_by_name", RubyMethodAttributes.PublicSingleton)]
        public static Object get_property_by_name(RubyModule/*!*/ self, [NotNull]String/*!*/ name)
        {
            try
            {
                return RhoConf.getInstance().getString(name);
            }
            catch (Exception ex)
            {
                Exception rubyEx = self.Context.CurrentException;
                if (rubyEx == null)
                {
                    rubyEx = RubyExceptionData.InitializeException(new RuntimeError(ex.Message.ToString()), ex.Message);
                }
                LOG.ERROR("get_property_by_name", ex);
                throw rubyEx;
            }
        }

        [RubyMethodAttribute("is_property_exists", RubyMethodAttributes.PublicSingleton)]
        public static Boolean is_property_exists(RubyModule/*!*/ self, [NotNull]String/*!*/ name)
        {
            try
            {
                return RhoConf.getInstance().isExist(name);
            }
            catch (Exception ex)
            {
                Exception rubyEx = self.Context.CurrentException;
                if (rubyEx == null)
                {
                    rubyEx = RubyExceptionData.InitializeException(new RuntimeError(ex.Message.ToString()), ex.Message);
                }
                LOG.ERROR("is_property_exists", ex);
                throw rubyEx;
            }
        }

        [RubyMethodAttribute("show_log", RubyMethodAttributes.PublicSingleton)]
        public static void show_log(RubyModule/*!*/ self)
        {
            try
            {
                //TODO
            }
            catch (Exception ex)
            {
                Exception rubyEx = self.Context.CurrentException;
                if (rubyEx == null)
                {
                    rubyEx = RubyExceptionData.InitializeException(new RuntimeError(ex.Message.ToString()), ex.Message);
                }
                LOG.ERROR("show_log", ex);
                throw rubyEx;
            }
        }

        [RubyMethodAttribute("send_log", RubyMethodAttributes.PublicSingleton)]
        public static void send_log(RubyModule/*!*/ self)
        {
            try
            {
                //TODO
            }
            catch (Exception ex)
            {
                Exception rubyEx = self.Context.CurrentException;
                if (rubyEx == null)
                {
                    rubyEx = RubyExceptionData.InitializeException(new RuntimeError(ex.Message.ToString()), ex.Message);
                }
                LOG.ERROR("send_log", ex);
                throw rubyEx;
            }
        }

        [RubyMethodAttribute("clean_log", RubyMethodAttributes.PublicSingleton)]
        public static void clean_log(RubyModule/*!*/ self)
        {
            try
            {
                RhoLogger.clearLog();
            }
            catch (Exception ex)
            {
                Exception rubyEx = self.Context.CurrentException;
                if (rubyEx == null)
                {
                    rubyEx = RubyExceptionData.InitializeException(new RuntimeError(ex.Message.ToString()), ex.Message);
                }
                LOG.ERROR("clean_log", ex);
                throw rubyEx;
            }
        }

        [RubyMethodAttribute("read_log", RubyMethodAttributes.PublicSingleton)]
        public static void read_log(RubyModule/*!*/ self, int limit = 0)
        {
            try
            {
                //TODO
            }
            catch (Exception ex)
            {
                Exception rubyEx = self.Context.CurrentException;
                if (rubyEx == null)
                {
                    rubyEx = RubyExceptionData.InitializeException(new RuntimeError(ex.Message.ToString()), ex.Message);
                }
                LOG.ERROR("read_log", ex);
                throw rubyEx;
            }
        }

        #endregion
    }
}
