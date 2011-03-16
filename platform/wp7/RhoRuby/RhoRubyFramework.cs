using System;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Ink;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Shapes;
using Microsoft.Scripting.Hosting;
using Microsoft.Scripting;
using IronRuby.Builtins;
using IronRuby.Runtime;
using System.Windows.Resources;
using System.IO;
using Microsoft.Phone.Controls;

namespace RhoRuby
{
    public class RhoRubyFramework
    {

        private static WebBrowser _webBrowser;

        public static WebBrowser WebBrowser
        {
            get { return _webBrowser; }
            set { _webBrowser = value; }
        }

        public RhoRubyFramework()
        {
            initRuby();
            RhoRubyStart();
        }
        
        public class RhoHost : ScriptHost
        {
            private readonly PlatformAdaptationLayer/*!*/ _pal;

            public RhoHost()
            {
                _pal = new WP_PlatformAdaptationLayer();
            }

            public override PlatformAdaptationLayer PlatformAdaptationLayer
            {
                get { return _pal; }
            }
        }

        private ScriptRuntime _runtime;
        private ScriptEngine _engine;
        private object _rhoframework;


        private void initRuby()
        {
            ScriptRuntimeSetup runtimeSetup = ScriptRuntimeSetup.ReadConfiguration();
            var languageSetup = IronRuby.RubyHostingExtensions.AddRubySetup(runtimeSetup);

            runtimeSetup.DebugMode = false;
            runtimeSetup.PrivateBinding = false;
            runtimeSetup.HostType = typeof(RhoHost);

            languageSetup.Options["NoAdaptiveCompilation"] = false;
            languageSetup.Options["CompilationThreshold"] = 0;
            languageSetup.Options["Verbosity"] = 2;

            _runtime = IronRuby.Ruby.CreateRuntime(runtimeSetup);
            _engine = IronRuby.Ruby.GetEngine(_runtime);


            RubyContext _context = (RubyContext)Microsoft.Scripting.Hosting.Providers.HostingHelpers.GetLanguageContext(_engine);

            _context.ObjectClass.SetConstant("RHO_WP7", 1);
            _context.Loader.LoadAssembly("RhoRuby", "RhoRuby.RhoRubyLibraryInitializer", true, true);
            _runtime.Globals.SetVariable("RHO_FRAMEWORK", "");

            System.Collections.ObjectModel.Collection<string> paths = new System.Collections.ObjectModel.Collection<string>();
            paths.Add("lib");
            paths.Add("apps/app");
            _engine.SetSearchPaths(paths);
        }

        private void RhoRubyStart()
        {
            string code = "def foo; 'haha'; end; foo()";
            //string code = "class MyClass; def initialize(arg1); end; end; MyClass.new('');";

            StreamResourceInfo sr = Application.GetResourceStream(new Uri("rho/lib/rhoframework.rb", UriKind.Relative));
            using (System.IO.BinaryReader br = new BinaryReader(sr.Stream))
            {
                char[] str = br.ReadChars((int)sr.Stream.Length);
                code = new string(str);
            }

            ScriptSource src = _engine.CreateScriptSourceFromString(code);
            if (src == null)
                return;

            _rhoframework = src.Execute(_engine.CreateScope());
            if (_rhoframework == null)
                return;
            _engine.Execute("RHO_FRAMEWORK.ui_created");

            ////_engine.Operations.InvokeMember(_rhoframework, "ui_created");
        }
    }
}
