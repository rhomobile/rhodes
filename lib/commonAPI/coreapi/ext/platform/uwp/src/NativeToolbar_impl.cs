using System;
using System.Collections.Generic;
using rhoruntime;
using rhodes;

namespace rho {

namespace NativeToolbarImpl
{
    public class NativeToolbar : NativeToolbarBase
    {
        public NativeToolbar(string id) : base(id)
        {
        }
    }

    public class NativeToolbarSingleton : NativeToolbarSingletonBase
    {
        public NativeToolbarSingleton()
        {
        }

        static private MainPage getMainPage()
        {
            MainPage mainPage = MainPage.getInstance();
            return mainPage;
        }

        public override void create(IReadOnlyList<string> toolbarElements, IReadOnlyDictionary<string, string> toolBarProperties, IMethodResult oResult)
        {
            MainPage mp = getMainPage();
            if (mp != null)
            {
                mp.toolbarRemoveAllButtons();
                string bgCr = "";
                string mask = "";
                if (toolBarProperties.ContainsKey("backgroundColor"))
                    bgCr = toolBarProperties["backgroundColor"];
                if (toolBarProperties.ContainsKey("maskColor"))
                    mask = toolBarProperties["maskColor"];
                mp.setToolbarStyle(false, bgCr, mask);
                for (int i = 0; i < toolbarElements.Count; ++i)
                {
                    string tb = toolbarElements[i];
                    CJSONEntryProxy oEntry = new CJSONEntryProxy(tb);
                    string label = null;
                    string action = null;
                    string icon = null;
                    if (oEntry.hasName("label"))
                        label = oEntry.getString("label");
                    if (oEntry.hasName("action"))
                        action = oEntry.getString("action");
                    if (oEntry.hasName("icon"))
                        icon = oEntry.getString("icon");
                    CRhoRuntime.getInstance().logEvent("addToolbarButton: Label: '" + (label==null?"":label) + "';Action: '" + (action==null?"":action) + "'");
                    if ((action != null) && !action.Equals("separator", StringComparison.OrdinalIgnoreCase))
                    {
                        if ((icon != null) && (icon.Length > 0))
                        {
                            icon = "apps"+icon;
                            icon = CRhoRuntime.getInstance().getRootPath(icon);
                        }
                        else
                        {
                            if (action.Equals("options", StringComparison.OrdinalIgnoreCase))
                                icon = "res/options_btn.png";
                            else if (action.Equals("home", StringComparison.OrdinalIgnoreCase))
                                icon = "res/home_btn.png";
                            else if (action.Equals("refresh", StringComparison.OrdinalIgnoreCase))
                                icon = "res/refresh_btn.png";
                            else if (action.Equals("back", StringComparison.OrdinalIgnoreCase))
                                icon = "res/back_btn.png";
                            else if (action.Equals("forward", StringComparison.OrdinalIgnoreCase))
                                icon = "res/forward_btn.png";
                            if (icon != null)
                                icon = CRhoRuntime.getInstance().getRERuntimePath("lib/" + icon);
                        }
                        mp.toolbarAddAction(icon, label, action);
                    }
                }
                mp.toolbarShow();
            }
        }

        public override void remove(IMethodResult oResult)
        {
            MainPage mp = getMainPage();
            if (mp != null)
            {
                mp.toolbarRemoveAllButtons();
                CRhoRuntime.getInstance().logEvent("Toolbar removed");
            }
        }

        public override void isCreated(IMethodResult oResult)
        {
            MainPage mp = getMainPage();
            oResult.set(mp != null ? (mp.toolbarIsStarted() != 0) : false);
        }
    }

    public class NativeToolbarFactory : NativeToolbarFactoryBase
    {
    }
}

}
