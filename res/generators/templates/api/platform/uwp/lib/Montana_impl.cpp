#include "<%= $cur_module.name %>_impl.h"
#include "<%= $cur_module.name %>Factory.h"
#include "api_generator/uwp/MethodResultImpl.h"

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
        params += "#{CppGen::native_type_arg(param)} #{param.name}, "
        param_name = param.name
        param_conv = api_generator_cli_conversion(param.type, param.name)
        if param_conv.length() > 0
          param_name = '_' + param_name
          covert_params += "        #{param_conv}"
        end
        call_params += "#{param_name}, "
    end

    params += 'CMethodResult& oResult'
    call_params += 'ref new CMethodResultImpl((int64)&oResult)'
    module_method.cached_data["cpp2cs_params"] = params
    module_method.cached_data["cpp2cs_call_params"] = call_params

    if module_method.access == ModuleMethod::ACCESS_STATIC
      static_methods += "\n    virtual void #{module_method.native_name}(#{params})\n    {\n#{covert_params}        try {\n            _runtime->#{module_method.native_name}(#{call_params});\n        } catch (Platform::Exception^ e) {\n            LOG(ERROR) + rho::common::convertStringAFromWP8(e->ToString());\n        }\n    }\n"
    else
      if /^(getProperty|getProperties|getAllProperties|setProperty|setProperties)$/ !~ module_method.native_name
        dynamic_methods += "\nvoid C#{$cur_module.name}Impl::#{module_method.native_name}(#{params})\n{\n#{covert_params}    try {\n        _runtime->#{module_method.native_name}(#{call_params});\n    } catch (Platform::Exception^ e) {\n        LOG(ERROR) + rho::common::convertStringAFromWP8(e->ToString());\n    }\n}\n"
      end
    end
  end
%>
<%= dynamic_methods%>

class C<%= $cur_module.name %>Singleton: public C<%= $cur_module.name %>SingletonBase
{
private:
    I<%= $cur_module.name %>SingletonImpl^ _runtime;
public:
    C<%= $cur_module.name %>Singleton(I<%= $cur_module.name %>SingletonImpl^ runtime): C<%= $cur_module.name %>SingletonBase(), _runtime(runtime) {}
    ~C<%= $cur_module.name %>Singleton(){}
<%= static_methods%>
<% if $cur_module.is_template_default_instance %>
    virtual rho::String getDefaultID(){return "1";} // TODO: implement getDefaultID
    virtual rho::String getInitialDefaultID(){return "1";} // TODO: implement getInitialDefaultID
    virtual void setDefaultID(const rho::String& strID){} // TODO: implement setDefaultID
<% end %>    
    //virtual void addCommandToQueue(rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor){} // TODO: implement addCommandToQueue
    //virtual void callCommandInThread(rho::common::IRhoRunnable* pFunctor){} // TODO: implement callCommandInThread
};

I<%= $cur_module.name %>FactoryImpl^ C<%= $cur_module.name %>Factory::_impl;

I<%= $cur_module.name %>* C<%= $cur_module.name %>Factory::createModuleByID(const rho::String& strID)
{
    return new C<%= $cur_module.name %>Impl(strID, _impl->getImpl(rho::common::convertStringToWP8(strID)));
}

I<%= $cur_module.name %>Singleton* C<%= $cur_module.name %>Factory::createModuleSingleton()
{
    return new C<%= $cur_module.name %>Singleton(_impl->getSingletonImpl());
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
