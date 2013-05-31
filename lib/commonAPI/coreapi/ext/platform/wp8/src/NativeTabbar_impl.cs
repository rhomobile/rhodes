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

namespace rho {

namespace NativeTabbarImpl
{
    public class NativeTabbar : INativeTabbarImpl
    {
        public NativeTabbar()
        {
            NativeTabbarRuntimeComponent _runtime = new NativeTabbarRuntimeComponent(this);
        }
    }

    public class NativeTabbarSingleton : INativeTabbarSingletonImpl
    {
        public NativeTabbarSingleton()
        {
            NativeTabbarSingletonComponent _runtime = new NativeTabbarSingletonComponent(this);
        }

        public void create(IReadOnlyList<string> tabElements, IReadOnlyDictionary<string, string> tabBarProperties, IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void currentTabIndex(IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void remove(IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void removeTab(int tabIndex, IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void setTabBadge(int tabIndex, string badge, IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void switchTab(int tabIndex, IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void isCreated(IMethodResult oResult)
        {
            // implement this method in C# here
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
