#include "rhodes/JNIRhodes.h"
#include "rhodes/JNIRhoRuby.h"
#include "rhodes.h"
#include "RhodesApp.h"

extern "C" void registerRhoExtension()
{

	RHODESAPP().getExtManager().requireRubyFile("symbolapi/elements");

	/*const char* szPlugins = get_app_build_config_item("plugins");
	if (szPlugins)
	{
		LOG(TRACE) + "Symbol plugins: " + szPlugins;

		CTokenizer oTokenizer( szPlugins, "," );
		while (oTokenizer.hasMoreTokens())
		{
			String tok = oTokenizer.nextToken();
			tok = String_trim(tok);

			String rubyFile = "symbolapi/";
			rubyFile += tok;

			rb_require(rubyFile.c_str());
		}
	}*/
}

extern "C" VALUE emmlapi_meta_proc(const char* module_name, const char* func_name, const char* params)
{
    JNIEnv *env = jnienv();
    jclass cls = rho_find_class(env, "com/rhoelementsext/RhoElementsExt");
    if (!cls)
    	return rho_ruby_get_NIL();
    jmethodID mid = env->GetStaticMethodID( cls, "metaProc", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/Object;");
    if (!mid)
    	return rho_ruby_get_NIL();

    jhstring jhModuleName = rho_cast<jstring>(env, module_name);
    jhstring jhFuncName = rho_cast<jstring>(env, func_name);
    jhstring jhParams = rho_cast<jstring>(env, params);

    jhobject jhRes = env->CallStaticObjectMethod(cls, mid, jhModuleName.get(), jhFuncName.get(), jhParams.get());
    env->DeleteLocalRef(cls);
    return (!jhRes) ? rho_ruby_get_NIL() : rho_cast<VALUE>(jhRes.get());
}

