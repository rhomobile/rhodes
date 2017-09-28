using System;
using System.Collections.Generic;
using rhoruntime;
using rhodes;

namespace rho {

namespace NativeTabbarImpl
{
    public class NativeTabbar : NativeTabbarBase
    {
        public NativeTabbar(string id) : base(id)
        {
        }
    }

    public class NativeTabbarSingleton : NativeTabbarSingletonBase
    {
        public NativeTabbarSingleton()
        {
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

        public override void create(IReadOnlyList<string> tabElements, IReadOnlyDictionary<string, string> tabBarProperties, IMethodResult oResult)
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

        public override void currentTabIndex(IMethodResult oResult)
        {
            MainPage mp = getMainPage();
            oResult.set(mp != null ? mp.tabbarGetCurrent() : 0);
        }

        public override void remove(IMethodResult oResult)
        {
            MainPage mp = getMainPage();
            if (mp != null)
            {
                //mp.tabbarRemoveAllTabs();
                mp.tabbarHide();
                CRhoRuntime.getInstance().logEvent("Tabbar removed");
            }
        }

        public override void removeTab(int tabIndex, IMethodResult oResult)
        {
            MainPage mp = getMainPage();
            if (mp != null)
            {
                mp.tabbarRemove(tabIndex);
                CRhoRuntime.getInstance().logEvent("Tabbar #" + tabIndex + " removed");
            }
        }

        public override void setTabBadge(int tabIndex, string badge, IMethodResult oResult)
        {
            MainPage mp = getMainPage();
            if (mp != null)
                mp.tabbarSetBadge(tabIndex, badge);
        }

        public override void switchTab(int tabIndex, IMethodResult oResult)
        {
            MainPage mp = getMainPage();
            if (mp != null)
            {
                mp.tabbarSwitch(tabIndex);
                CRhoRuntime.getInstance().logEvent("Tabbar switched to tab #" + tabIndex);
            }
        }

        public override void isCreated(IMethodResult oResult)
        {
            MainPage mp = getMainPage();
            oResult.set(mp != null ? (mp.tabbarIsStarted() != 0) : false);
        }
    }

    public class NativeTabbarFactory : NativeTabbarFactoryBase
    {
    }
}

}
