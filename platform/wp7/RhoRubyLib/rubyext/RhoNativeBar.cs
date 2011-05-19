using Microsoft.Scripting.Utils;
using Microsoft.Scripting.Runtime;
using IronRuby.Runtime;
using IronRuby.Builtins;
using System;
using System.Runtime.InteropServices;
using rho.common;

namespace rho.rubyext
{
    [RubyModule("NativeBar")]
    public static class RhoNativeBar
    {

        #region Private Implementation Details

        private static RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() :
            new RhoLogger("RhoNativeBar");
        private static CRhodesApp RhodesApp { get { return CRhodesApp.Instance; } }

        #endregion

        #region Private Instance & Singleton Methods

        [RubyMethodAttribute("create", RubyMethodAttributes.PublicSingleton)]
        public static void Create(RubyModule/*!*/ self, int barType, RubyArray args)
        {
            try
            {
                if (args != null && args.Count > 0)
                    RhodesApp.createToolBar(barType, (Object)args);

                if (barType == 2)
                    RhodesApp.removeToolBar();
            }
            catch (Exception ex)
            {
                Exception rubyEx = self.Context.CurrentException;
                if (rubyEx == null)
                {
                    rubyEx = RubyExceptionData.InitializeException(new RuntimeError(ex.Message.ToString()), ex.Message);
                }
                LOG.ERROR("create", ex);
                throw rubyEx;
            }
        }

        [RubyMethodAttribute("create", RubyMethodAttributes.PublicSingleton)]
        public static void Create(RubyModule/*!*/ self, int barType, Hash args)
        {
            try
            {
                if (args != null && args.Count > 0)
                    RhodesApp.createToolBar(barType, (Object)args);

                if (barType == 2)
                    RhodesApp.removeToolBar();
            }
            catch (Exception ex)
            {
                Exception rubyEx = self.Context.CurrentException;
                if (rubyEx == null)
                {
                    rubyEx = RubyExceptionData.InitializeException(new RuntimeError(ex.Message.ToString()), ex.Message);
                }
                LOG.ERROR("create", ex);
                throw rubyEx;
            }
        }

        [RubyMethodAttribute("remove", RubyMethodAttributes.PublicSingleton)]
        public static void Remove(RubyModule/*!*/ self)
        {
            try
            {
                RhodesApp.removeToolBar();
            }
            catch (Exception ex)
            {
                Exception rubyEx = self.Context.CurrentException;
                if (rubyEx == null)
                {
                    rubyEx = RubyExceptionData.InitializeException(new RuntimeError(ex.Message.ToString()), ex.Message);
                }
                LOG.ERROR("remove", ex);
                throw rubyEx;
            }
        }

        [RubyMethodAttribute("switch_tab", RubyMethodAttributes.PublicSingleton)]
        public static void switchTab(RubyModule/*!*/ self, int index)
        {

        }

        [RubyMethodAttribute("started", RubyMethodAttributes.PublicSingleton)]
        public static Boolean isStarted(RubyModule/*!*/ self)
        {
            Boolean res = false;
            try
            {
                res = RhodesApp.barIsStarted;
            }
            catch (Exception ex)
            {
                Exception rubyEx = self.Context.CurrentException;
                if (rubyEx == null)
                {
                    rubyEx = RubyExceptionData.InitializeException(new RuntimeError(ex.Message.ToString()), ex.Message);
                }
                LOG.ERROR("started", ex);
                throw rubyEx;
            }

            return res;
        }

        [RubyMethodAttribute("set_tab_badge", RubyMethodAttributes.PublicSingleton)]
        public static void setTabBadge(RubyModule/*!*/ self, int index, MutableString/*!*/ val)
        {

        }

        [RubyMethodAttribute("create_native_toolbar", RubyMethodAttributes.PublicSingleton)]
        public static void createNativeToolBar(RubyModule/*!*/ self, int barType, RubyArray args)
        {
            try
            {
                Create(self, barType, args);
            }
            catch (Exception ex)
            {
                Exception rubyEx = self.Context.CurrentException;
                if (rubyEx == null)
                {
                    rubyEx = RubyExceptionData.InitializeException(new RuntimeError(ex.Message.ToString()), ex.Message);
                }
                LOG.ERROR("create_native_toolbar", ex);
                throw rubyEx;
            }
        }

        [RubyMethodAttribute("create_native_toolbar", RubyMethodAttributes.PublicSingleton)]
        public static void createNativeToolBar(RubyModule/*!*/ self, int barType, Hash args)
        {
            try
            {
                Create(self, barType, args);
            }
            catch (Exception ex)
            {
                Exception rubyEx = self.Context.CurrentException;
                if (rubyEx == null)
                {
                    rubyEx = RubyExceptionData.InitializeException(new RuntimeError(ex.Message.ToString()), ex.Message);
                }
                LOG.ERROR("create_native_toolbar", ex);
                throw rubyEx;
            }
        }

        [RubyMethodAttribute("remove_native_toolbar", RubyMethodAttributes.PublicSingleton)]
        public static void removeNativeToolBar(RubyModule/*!*/ self)
        {
            try
            {
                Remove(self);
            }
            catch (Exception ex)
            {
                Exception rubyEx = self.Context.CurrentException;
                if (rubyEx == null)
                {
                    rubyEx = RubyExceptionData.InitializeException(new RuntimeError(ex.Message.ToString()), ex.Message);
                }
                LOG.ERROR("remove_native_toolbar", ex);
                throw rubyEx;
            }
        }

        [RubyMethodAttribute("create_native_tabbar", RubyMethodAttributes.PublicSingleton)]
        public static void createNativeTabBar(RubyModule/*!*/ self, int barType, RubyArray args)
        {

        }

        [RubyMethodAttribute("remove_native_tabbar", RubyMethodAttributes.PublicSingleton)]
        public static void removeNativeTabBar(RubyModule/*!*/ self)
        {

        }

        [RubyMethodAttribute("native_tabbar_switch_tab", RubyMethodAttributes.PublicSingleton)]
        public static void nativeTabBarSwitchTab(RubyModule/*!*/ self, int index)
        {

        }

        [RubyMethodAttribute("native_tabbar_set_tab_badge", RubyMethodAttributes.PublicSingleton)]
        public static void nativeTabBarSetTabBadge(RubyModule/*!*/ self, int index, MutableString/*!*/ val)
        {

        }

        [RubyMethodAttribute("native_tabbar_get_current_tab", RubyMethodAttributes.PublicSingleton)]
        public static void nativeTabBarGetCurrentTab(RubyModule/*!*/ self)
        {
      
        }

        #endregion
    }
}
