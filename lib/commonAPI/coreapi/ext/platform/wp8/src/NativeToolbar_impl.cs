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

namespace NativeToolbarImpl
{
    public class NativeToolbar : INativeToolbarImpl
    {
        public NativeToolbar()
        {
            NativeToolbarRuntimeComponent _runtime = new NativeToolbarRuntimeComponent(this);
        }
    }

    public class NativeToolbarSingleton : INativeToolbarSingletonImpl
    {
        public NativeToolbarSingleton()
        {
            NativeToolbarSingletonComponent _runtime = new NativeToolbarSingletonComponent(this);
        }

        static private MainPage getMainPage()
        {
            MainPage mainPage = MainPage.getInstance();
            return mainPage;
        }

        public void create(IReadOnlyList<string> toolbarElements, IReadOnlyDictionary<string, string> toolBarProperties, IMethodResult oResult)
        {
            MainPage mp = getMainPage();
            if (mp != null)
            {
                mp.toolbarRemoveAllButtons();
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
                    if ((action != null) && !action.Equals("separator", StringComparison.InvariantCultureIgnoreCase))
                    {
                        if ((icon != null) && (icon.Length > 0))
                        {
                            icon = CRhoRuntime.getInstance().getRootPath(icon);
                        }
                        else
                        {
                            if (action.Equals("options", StringComparison.InvariantCultureIgnoreCase))
                                icon = "res/options_btn.png";
                            else if (action.Equals("home", StringComparison.InvariantCultureIgnoreCase))
                                icon = "res/home_btn.png";
                            else if (action.Equals("refresh", StringComparison.InvariantCultureIgnoreCase))
                                icon = "res/refresh_btn.png";
                            else if (action.Equals("back", StringComparison.InvariantCultureIgnoreCase))
                                icon = "res/back_btn.png";
                            else if (action.Equals("forward", StringComparison.InvariantCultureIgnoreCase))
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

        public void remove(IMethodResult oResult)
        {
            MainPage mp = getMainPage();
            if (mp != null)
            {
                mp.toolbarRemoveAllButtons();
                CRhoRuntime.getInstance().logEvent("Toolbar removed");
            }
        }

        public void isCreated(IMethodResult oResult)
        {
            MainPage mp = getMainPage();
            oResult.set(mp != null ? (mp.toolbarIsStarted() != 0) : false);
        }
    }

    public class NativeToolbarFactory : INativeToolbarFactoryImpl
    {
        public INativeToolbarImpl getImpl() {
            return new NativeToolbar();
        }
        public INativeToolbarSingletonImpl getSingletonImpl() {
            return new NativeToolbarSingleton();
        }
    }
}

}
