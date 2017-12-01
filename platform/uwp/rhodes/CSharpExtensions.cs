// WARNING! THIS FILE IS GENERATED AUTOMATICALLY! DO NOT EDIT IT MANUALLY!
using rhoruntime;
namespace rhodes {
    public static class CSharpExtensions {
        public static void InitializeExtensions() {
            WebViewFactoryComponent.setImpl(new rho.WebViewImpl.WebViewFactory());
            NativeToolbarFactoryComponent.setImpl(new rho.NativeToolbarImpl.NativeToolbarFactory());
            NativeTabbarFactoryComponent.setImpl(new rho.NativeTabbarImpl.NativeTabbarFactory());
            TimerFactoryComponent.setImpl(new rho.TimerImpl.TimerFactory());
        }
    }
}
