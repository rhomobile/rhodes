using System;
using Microsoft.Scripting.Hosting;
using Microsoft.Scripting;
using IronRuby.Builtins;
using IronRuby.Runtime;
using IronRuby.Runtime.Calls;
using System.Windows.Resources;
using System.Windows;
using System.IO;
using Microsoft.Phone.Controls;
using rho.common;
using System.Collections.Generic;
using rho.rubyext;

namespace rho
{
    public class CRhoRuby
    {
        private static RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() :
            new RhoLogger("CRhoRuby");

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
            m_context.ObjectClass.AddMethod(m_context, "__rhoGetCallbackObject", new RubyLibraryMethodInfo(
                new[] { LibraryOverload.Create(new Func<System.Object, System.Int32, System.Object>(RhoKernelOps.__rhoGetCallbackObject), false, 0, 0) },
                RubyMethodVisibility.Public,
                m_context.ObjectClass
            ));
            m_context.ObjectClass.AddMethod(m_context, "__rho_exist_in_resources", new RubyLibraryMethodInfo(
                new[] { LibraryOverload.Create(new Func<System.Object, System.String, System.Object>(RhoKernelOps.__rho_exist_in_resources), false, 0, 0) },
                RubyMethodVisibility.Public,
                m_context.ObjectClass
            ));

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
            //m_engine.Execute("test = {}; test.__id__;");

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
            return MutableString.Create(str);
        }

        public RubyArray createArray()
        {
            return new IronRuby.Builtins.RubyArray();
        }

        public Hash createHash()
        {
            return new Hash(m_context);
        }

        public void hashAdd(Object hash, Object key, Object value)
        {
            if (key is String)
                key = createString(key as String);
            if (value is String)
                value = createString(value as String);

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
            MutableString strParam = MutableString.Create(strData);
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

        public String getStartPage()
        {
            return RhoConf.getInstance().getString("start_path");
        }

        public String getOptionsPage()
        {
            return RhoConf.getInstance().getString("options_path");
        }

        public static MutableString create_string(String str)
        {
            return MutableString.Create(str);
        }

        public static Hashtable<String, String> enum_strhash(Object valHash)
        {
            Hashtable<String, String> hash = new Hashtable<String, String>();

            if (valHash == null || valHash == null)
                return hash;

            Hash items = (Hash)valHash;

            foreach (KeyValuePair<object, object> kvp in items)
            {
                hash.put(kvp.Key.ToString(), kvp.Value.ToString());
            }

            return hash;
        }
    }
}
