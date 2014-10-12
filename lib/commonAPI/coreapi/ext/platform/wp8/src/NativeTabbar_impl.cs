using System;
using System.Collections.Generic;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Ink;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Shapes;
using rhoruntime;
using rhodes;

namespace rho {

namespace NativeTabbarImpl
{
    public class NativeTabbar : INativeTabbarImpl
    {
        private long _nativeImpl = 0;
        NativeTabbarRuntimeComponent _runtime;

        public NativeTabbar()
        {
            _runtime = new NativeTabbarRuntimeComponent(this);
        }

        public long getNativeImpl()
        {
            return _nativeImpl;
        }

        public void setNativeImpl(long native)
        {
            _nativeImpl = native;
        }
    }

    public class NativeTabbarSingleton : INativeTabbarSingletonImpl
    {
        public NativeTabbarSingleton()
        {
            NativeTabbarSingletonComponent _runtime = new NativeTabbarSingletonComponent(this);
        }

        static private MainPage getMainPage()
        {
            MainPage mainPage = MainPage.getInstance();
            return mainPage;
        }

        private static bool charToBool(string value)
        {
            if ((value == null) || (value.Length <= 0))
                return false;

            try
            {
                return Convert.ToBoolean(value);
            }
            catch (FormatException)
            {
            }

            try
            {
                return Convert.ToInt64(value) == 1;
            }
            catch (FormatException)
            {
                return false;
            }
        }

        public void create(IReadOnlyList<string> tabElements, IReadOnlyDictionary<string, string> tabBarProperties, IMethodResult oResult)
        {
            MainPage mp = getMainPage();
            if (mp != null)
            {
                mp.tabbarHide();
                mp.toolbarRemoveAllButtons();
                string tab_background_color = null;
                if (tabBarProperties.ContainsKey("backgroundColor"))
                    tab_background_color = tabBarProperties["backgroundColor"];
                for (int i = 0; i < tabElements.Count; ++i)
                {
                    string tb = tabElements[i];
                    CJSONEntryProxy oEntry = new CJSONEntryProxy(tb);

                    string label = null;
                    string action = null;
                    string icon = null;
                    string reload = null;
    	            string selected_color = null;
                    string disabled = null;
                    string background_color = null;
                    string use_current_view_for_tab = null;

                    if (oEntry.hasName("label"))
                        label = oEntry.getString("label");
                    if (oEntry.hasName("action"))
                        action = oEntry.getString("action");
                    if (oEntry.hasName("icon"))
                        icon = oEntry.getString("icon");
                    if (oEntry.hasName("reload"))
                        reload = oEntry.getString("reload");
                    if (oEntry.hasName("selectedColor"))
                        selected_color = oEntry.getString("selectedColor");
                    if (oEntry.hasName("disabled"))
                        disabled = oEntry.getString("disabled");
                    if (oEntry.hasName("backgroundColor"))
                        background_color = oEntry.getString("backgroundColor");
                    if (oEntry.hasName("useCurrentViewForTab"))
                        use_current_view_for_tab = oEntry.getString("useCurrentViewForTab");

                    if ((icon != null) && (icon.Length > 0))
                        icon = CRhoRuntime.getInstance().getAppRootPath(icon);

                    CRhoRuntime.getInstance().logEvent("AddTab: " + (label==null?"(null)":label) + "; " +
                        (icon==null?"(null)":icon) + "; " + (action==null?"(null)":action) + "; " +
                        charToBool(disabled) + "; " + (background_color==null?"(null)":background_color) + "; " +
                        (selected_color==null?"(null)":selected_color) + "; " +
                        (tab_background_color==null?"(null)":tab_background_color) + "; " +
                        charToBool(reload) + "; " + charToBool(use_current_view_for_tab));
                    mp.tabbarAddTab(label, icon, action, charToBool(disabled),
                        background_color, selected_color, tab_background_color,
                        charToBool(reload), charToBool(use_current_view_for_tab), oResult.hasCallback(), oResult);                    
                }
                mp.tabbarSwitch(0);
                mp.tabbarShow();
            }
        }

        public void currentTabIndex(IMethodResult oResult)
        {
            MainPage mp = getMainPage();
            oResult.set(mp != null ? mp.tabbarGetCurrent() : 0);
        }

        public void remove(IMethodResult oResult)
        {
            MainPage mp = getMainPage();
            if (mp != null)
            {
                //mp.tabbarRemoveAllTabs();
                mp.tabbarHide();
                CRhoRuntime.getInstance().logEvent("Tabbar removed");
            }
        }

        public void removeTab(int tabIndex, IMethodResult oResult)
        {
            MainPage mp = getMainPage();
            if (mp != null)
            {
                mp.tabbarRemove(tabIndex);
                CRhoRuntime.getInstance().logEvent("Tabbar #" + tabIndex + " removed");
            }
        }

        public void setTabBadge(int tabIndex, string badge, IMethodResult oResult)
        {
            MainPage mp = getMainPage();
            if (mp != null)
                mp.tabbarSetBadge(tabIndex, badge);
        }

        public void switchTab(int tabIndex, IMethodResult oResult)
        {
            MainPage mp = getMainPage();
            if (mp != null)
            {
                mp.tabbarSwitch(tabIndex);
                CRhoRuntime.getInstance().logEvent("Tabbar switched to tab #" + tabIndex);
            }
        }

        public void isCreated(IMethodResult oResult)
        {
            MainPage mp = getMainPage();
            oResult.set(mp != null ? (mp.tabbarIsStarted() != 0) : false);
        }
    }

    public class NativeTabbarFactory : INativeTabbarFactoryImpl
    {
        public INativeTabbarImpl getImpl() {
            return new NativeTabbar();
        }
        public INativeTabbarSingletonImpl getSingletonImpl() {
            return new NativeTabbarSingleton();
        }
    }
}

}
