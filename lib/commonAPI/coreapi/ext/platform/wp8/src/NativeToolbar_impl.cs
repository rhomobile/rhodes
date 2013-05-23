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

        public void create(IReadOnlyList<string> toolbarElements, IReadOnlyDictionary<string, string> toolBarProperties, IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void remove(IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void isCreated(IMethodResult oResult)
        {
            // implement this method in C# here
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
