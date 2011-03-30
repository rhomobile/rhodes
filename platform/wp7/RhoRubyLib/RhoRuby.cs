using System;
using Microsoft.Scripting.Hosting;
using Microsoft.Scripting;
using IronRuby.Builtins;
using IronRuby.Runtime;
using System.Windows.Resources;
using System.Windows;
using System.IO;
using Microsoft.Phone.Controls;
using rho.common;

namespace rho
{
    public class CRhoRuby
    {
        RhoConf RHOCONF() { return RhoConf.getInstance(); }

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
            createRhoFramework();
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

        private void createRhoFramework()
        {
            string code = "def foo; 'haha'; end; foo()";
            //string code = "class MyClass; def initialize(arg1); end; end; MyClass.new('');";
            //m_engine.Execute("class MyClass < Exception; def initialize(arg1); end; end; MyClass.new('');");
            //m_engine.Execute("def test; while false; end; end; test();");
            //m_engine.Execute("class RecordNotFound < StandardError;end; raise RecordNotFound;");
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
        }

        public void InitApp()
        {
            if (m_rhoframework == null)
                return;

            m_engine.Operations.InvokeMember(m_rhoframework, "init_app");
            m_engine.Operations.InvokeMember(m_rhoframework, "ui_created");
        }

        public void Stop()
        {
            m_runtime.Shutdown();
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

        public String getStringFromObject(Object body)
        {
            if (body != null && body.GetType() == typeof(MutableString))
                return ((MutableString)body).ToString();

            return String.Empty;
        }

        public object createString(String str)
        {
            MutableString res = new MutableString();
            res.Append(str);

            return res;
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

            if (value != null && value is MutableString)
                return ((MutableString)value).ToString();

            return String.Empty;
        }

        public Vector<String> makeVectorStringFromArray(RubyArray ar)
        {
            Vector<String> arRes = new Vector<String>();
            for (int i = 0; ar != null && i < ar.Count; i++)
            {
                Object item = ar[i];
                if (item != null && item is MutableString)
                    arRes.Add( ((MutableString)item).ToString() );
                else
                    arRes.Add(String.Empty);
            }

            return arRes;
        }

        public bool isMainRubyThread()
        {
            //TODO: isMainRubyThread
            return true;
        }

        public String getRhoDBVersion()
        {
            String strVer = "";

            object val = m_engine.Execute("Rhodes::DBVERSION");

            if (val != null && val.GetType() == typeof(MutableString))
                strVer = ((MutableString)val).ToString();
		
	        return strVer;        
        }

        public void call_config_conflicts()
        {
            //TODO: call_config_conflicts
            /*RubyHash hashConflicts = RHOCONF().getRubyConflicts();
            if (hashConflicts.size().toInt() == 0)
                return;

            m_engine.Operations.InvokeMember(m_rhoframework, "on_config_conflicts", hashConflicts);*/
        }

        public void raise_RhoError(int errCode)
        {
            m_engine.Operations.InvokeMember(m_rhoframework, "raise_rhoerror", errCode);
        }

        public void loadServerSources(String strData)
        {
            MutableString strParam = new MutableString();
            strParam.Append(strData);
            m_engine.Operations.InvokeMember(m_rhoframework, "load_server_sources", strParam);
        }

        public void loadAllSyncSources()
        {
            m_engine.Operations.InvokeMember(m_rhoframework, "load_all_sync_sources");
        }

        public void resetDBOnSyncUserChanged()
        {
            m_engine.Operations.InvokeMember(m_rhoframework, "reset_db_on_sync_user_changed");
        }
    }
}
