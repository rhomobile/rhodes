using System;
using Microsoft.Scripting.Hosting;
using Microsoft.Scripting;
using IronRuby.Builtins;
using IronRuby.Runtime;
using System.Windows.Resources;
using System.Windows;
using System.IO;
using Microsoft.Phone.Controls;

namespace rho
{
    public class CRhoRuby
    {
        private static readonly CRhoRuby m_instance = new CRhoRuby();
        public static CRhoRuby Instance { get { return m_instance; } }

        private WebBrowser m_webBrowser;
        private ScriptRuntime m_runtime;
        private ScriptEngine m_engine;
        RubyContext m_context;
        private object m_rhoframework;

        public WebBrowser WebBrowser{ get { return m_webBrowser; } }

        public void Init(WebBrowser browser)
        {
            m_webBrowser = browser;
            initRuby();
            Start();
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

            m_runtime = IronRuby.Ruby.CreateRuntime(runtimeSetup);
            m_engine = IronRuby.Ruby.GetEngine(m_runtime);
            m_context = (RubyContext)Microsoft.Scripting.Hosting.Providers.HostingHelpers.GetLanguageContext(m_engine);

            m_context.ObjectClass.SetConstant("RHO_WP7", 1);
            m_context.Loader.LoadAssembly("RhoRubyLib", "rho.rubyext.rubyextLibraryInitializer", true, true);

            System.Collections.ObjectModel.Collection<string> paths = new System.Collections.ObjectModel.Collection<string>();
            paths.Add("lib");
            paths.Add("apps/app");
            m_engine.SetSearchPaths(paths);
        }

        private void Start()
        {
            string code = "def foo; 'haha'; end; foo()";
            //string code = "class MyClass; def initialize(arg1); end; end; MyClass.new('');";
            //m_engine.Execute("class MyClass < Exception; def initialize(arg1); end; end; MyClass.new('');");
            //m_engine.Execute("def test; while false; end; end; test();");

            StreamResourceInfo sr = Application.GetResourceStream(new Uri("lib/rhoframework.rb", UriKind.Relative));

            using (System.IO.BinaryReader br = new BinaryReader(sr.Stream))
            {
                char[] str = br.ReadChars((int)sr.Stream.Length);
                code = new string(str);
            }

            ScriptSource src = m_engine.CreateScriptSourceFromString(code);
            if (src == null)
                return;

            m_rhoframework = src.Execute(m_engine.CreateScope());
            if (m_rhoframework == null)
                return;
            //m_engine.Execute("RHO_FRAMEWORK.ui_created");

            m_engine.Operations.InvokeMember(m_rhoframework, "ui_created");
        }

        public Object callServeIndex(String indexPath, Object req)
        {
            m_context.ObjectClass.SetConstant("RHO__wp_index_path", indexPath);
            m_context.ObjectClass.SetConstant("RHO__wp_headers", req);

            return m_engine.Execute("RHO_FRAMEWORK.serve_index_hash(RHO__wp_index_path, RHO__wp_headers)");
        }

        public Object callServe(Object req)
        {
            m_context.ObjectClass.SetConstant("RHO__wp_headers", req);

            return m_engine.Execute("RHO_FRAMEWORK.serve_hash(RHO__wp_headers)");
        }

        public byte[] getBytesFromString(Object body)
        {
            if (body != null && body.GetType() == typeof(MutableString))
                return ((MutableString)body).ToByteArray();

            return new byte[0];
        }

        public Hash createHash()
        {
            return new Hash(m_context);
        }

        public void hashAdd(Object hash, Object key, Object value)
        {
            ((Hash)hash).Add(key, value);
        }

        public Object hashGet(Object hash, Object key)
        {
            return ((Hash)hash)[key];
        }

        public int hashGetInt(Object hash, Object key)
        {
            Object value = hashGet(hash, key);
            if (value != null && value.GetType() == typeof(System.Int32))
                return ((System.Int32)value);

            return 0;
        }

        public String hashGetString(Object hash, Object key)
        {
            Object value = hashGet(hash, key);

            if (value != null && value.GetType() == typeof(MutableString))
                return ((MutableString)value).ToString();

            return String.Empty;
        }

        public bool isMainRubyThread()
        {
            //TODO: isMainRubyThread
            return true;
        }
    }
}
