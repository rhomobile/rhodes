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

        private static CRhodesApp RhodesApp { get { return CRhodesApp.Instance; } }

        #endregion

        #region Private Instance & Singleton Methods

        [RubyMethodAttribute("create", RubyMethodAttributes.PublicSingleton)]
        public static void Create(RubyModule/*!*/ self, int barType, RubyArray args)
        {
            if (args != null && args.Count > 0)
                RhodesApp.createToolBar(barType, (Object)args);

            if (barType == 2)
                RhodesApp.removeToolBar();
        }

        [RubyMethodAttribute("create", RubyMethodAttributes.PublicSingleton)]
        public static void Create(RubyModule/*!*/ self, int barType, Hash args)
        {
            if (args != null && args.Count > 0)
                RhodesApp.createToolBar(barType, (Object)args);

            if (barType == 2)
                RhodesApp.removeToolBar();
        }

        [RubyMethodAttribute("remove", RubyMethodAttributes.PublicSingleton)]
        public static void Remove(RubyModule/*!*/ self)
        {
            RhodesApp.removeToolBar();
        }

        [RubyMethodAttribute("switch_tab", RubyMethodAttributes.PublicSingleton)]
        public static void switchTab(RubyModule/*!*/ self, int index)
        {

        }

        [RubyMethodAttribute("started", RubyMethodAttributes.PublicSingleton)]
        public static Boolean isStarted(RubyModule/*!*/ self)
        {
            return true;
        }

        [RubyMethodAttribute("set_tab_badge", RubyMethodAttributes.PublicSingleton)]
        public static void setTabBadge(RubyModule/*!*/ self, int index, MutableString/*!*/ val)
        {

        }

        [RubyMethodAttribute("create_native_toolbar", RubyMethodAttributes.PublicSingleton)]
        public static void createNativeToolBar(RubyModule/*!*/ self, int barType, RubyArray args)
        {

        }

        [RubyMethodAttribute("remove_native_toolbar", RubyMethodAttributes.PublicSingleton)]
        public static void removeNativeToolBar(RubyModule/*!*/ self)
        {

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
