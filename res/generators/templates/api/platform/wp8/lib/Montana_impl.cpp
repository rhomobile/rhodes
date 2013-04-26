#include "common/RhodesApp.h"
#include "../../wp8/rhoruntime/common/RhoConvertWP8.h"
#include "../../../../shared/generated/cpp/<%= $cur_module.name %>Base.h"
#include "<%= $cur_module.name %>_MethodResultImpl.h"

using namespace rho::apiGenerator;
using namespace rhoruntime;
<% $cur_module.parents.each do |parent| %>
namespace <%= parent.downcase() %> {<%
end%>
<%
  dynamic_methods = ''
  static_methods = ''

  $cur_module.methods.each do |module_method|
    next if !module_method.generateNativeAPI

    params = ''
    call_params = ''
    covert_params = ''
    module_method.params.each do |param|
        params += "#{api_generator_cpp_makeNativeTypeArg(param.type)} #{param.name}, "
        param_name = param.name
        param_conv = api_generator_cli_conversion(param.type, param.name)
        if param_conv.length() > 0
          param_name = '_' + param_name
          covert_params += "        #{param_conv}"
        end
        call_params += "#{param_name}, "
    end

    params += 'CMethodResult& oResult'
    call_params += "ref new C#{$cur_module.name}MethodResultImpl((int64)&oResult)"
    module_method.cached_data["cpp2cs_params"] = params
    module_method.cached_data["cpp2cs_call_params"] = call_params

    method_def = "\n    virtual void #{module_method.native_name}(#{params})\n    {\n#{covert_params}        _runtime->#{module_method.native_name}(#{call_params});\n    }\n"
    if module_method.access == ModuleMethod::ACCESS_STATIC
      static_methods += method_def
    else
      dynamic_methods += method_def
    end
  end
%>
class C<%= $cur_module.name %>Impl: public C<%= $cur_module.name %>Base
{
    I<%= $cur_module.name %>Impl^ _runtime;
public:
    C<%= $cur_module.name %>Impl(const rho::String& strID): C<%= $cur_module.name %>Base()
    {
        // TODO: implement implementation constructor
    }

    virtual void registerRuntime(I<%= $cur_module.name %>Impl^ runtime)
    {
        _runtime = runtime;
    }
<%= dynamic_methods%>};

class C<%= $cur_module.name %>Singleton: public C<%= $cur_module.name %>SingletonBase
{
    I<%= $cur_module.name %>SingletonImpl^ _runtime;
public:
    ~C<%= $cur_module.name %>Singleton(){}

    virtual void registerRuntime(I<%= $cur_module.name %>SingletonImpl^ runtime)
    {
        _runtime = runtime;
    }
<%= static_methods%>
<% if $cur_module.is_template_default_instance %>
    virtual rho::String getDefaultID(){return "1";} // TODO: implement getDefaultID
    virtual rho::String getInitialDefaultID(){return "1";} // TODO: implement getInitialDefaultID
    virtual void setDefaultID(const rho::String& strID){} // TODO: implement setDefaultID
<% end %>    virtual void addCommandToQueue(rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor){} // TODO: implement addCommandToQueue
    virtual void callCommandInThread(rho::common::IRhoRunnable* pFunctor){} // TODO: implement callCommandInThread
};

class C<%= $cur_module.name %>Factory: public C<%= $cur_module.name %>FactoryBase
{
    ~C<%= $cur_module.name %>Factory(){}
    virtual I<%= $cur_module.name %>Singleton* createModuleSingleton();
    virtual I<%= $cur_module.name %>* createModuleByID(const rho::String& strID);
};

I<%= $cur_module.name %>* C<%= $cur_module.name %>Factory::createModuleByID(const rho::String& strID)
{
    return new C<%= $cur_module.name %>Impl(strID);
}

I<%= $cur_module.name %>Singleton* C<%= $cur_module.name %>Factory::createModuleSingleton()
{
    return new C<%= $cur_module.name %>Singleton();
}
<% $cur_module.parents.each do |parent| %>
}<%
end %>

extern "C" void Init_<%= $cur_module.name %>_extension()
{
    rho::C<%= $cur_module.name %>Factory::setInstance( new rho::C<%= $cur_module.name %>Factory() );
    rho::Init_<%= $cur_module.name %>_API();

    RHODESAPP().getExtManager().requireRubyFile("Rho<%= $cur_module.name %>Api");
}
