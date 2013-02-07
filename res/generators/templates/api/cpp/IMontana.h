#pragma once

#include "common/RhoStd.h"
#include "api_generator/MethodResult.h"
#include "api_generator/BaseClasses.h"

///////////////////////////////////////////////////////////
struct I<%= $cur_module.name %>
{
    virtual ~I<%= $cur_module.name %>(){}

<% $cur_module.methods.each do |module_method|
    next if module_method.access == ModuleMethod::ACCESS_STATIC

    params = ''
    module_method.params.each do |param|
        params += " #{api_generator_cpp_makeNativeTypeArg(param.type)} #{param.name}, "
    end

    params += 'CMethodResult& oResult'

%>    virtual void <%= module_method.native_name%>(<%= params%>) = 0;
<% end %>
};

struct I<%= $cur_module.name %>Singleton
{
    virtual ~I<%= $cur_module.name %>Singleton(){}

<% $cur_module.methods.each do |module_method|
    next if module_method.access != ModuleMethod::ACCESS_STATIC

    params = ''
    module_method.params.each do |param|
        params += " #{api_generator_cpp_makeNativeTypeArg(param.type)} #{param.name}, "
    end

    params += 'CMethodResult& oResult'

%>    virtual void <%= module_method.native_name%>(<%= params%>) = 0;
<% end %>
<% if $cur_module.is_template_default_instance %>
    virtual rho::StringW getDefaultID() = 0;
    virtual rho::StringW getInitialDefaultID() = 0;
    virtual void setDefaultID(const rho::StringW& strID) = 0;
<% end %>
    virtual void addCommandToQueue(rho::common::IRhoRunnable* pFunctor) = 0;
    virtual void callCommandInThread(rho::common::IRhoRunnable* pFunctor) = 0;
};

struct I<%= $cur_module.name %>Factory
{
    virtual ~I<%= $cur_module.name %>Factory(){}

    virtual I<%= $cur_module.name %>Singleton* getModuleSingleton() = 0;

<% if $cur_module.is_template_default_instance %>
    virtual I<%= $cur_module.name %>* getModuleByID(const rho::StringW& strID) = 0;
<% end %>
};

class C<%= $cur_module.name %>FactoryBase : public CModuleFactoryBase<I<%= $cur_module.name %>, I<%= $cur_module.name %>Singleton, I<%= $cur_module.name %>Factory>
{
protected:
    static rho::common::CAutoPtr<C<%= $cur_module.name %>FactoryBase> m_pInstance;

public:

    static void setInstance(C<%= $cur_module.name %>FactoryBase* pInstance){ m_pInstance = pInstance; }
    static C<%= $cur_module.name %>FactoryBase* getInstance(){ return m_pInstance; }

    static I<%= $cur_module.name %>Singleton* get<%= $cur_module.name %>SingletonS(){ return getInstance()->getModuleSingleton(); }
};

extern "C" void Init_<%= $cur_module.name %>_API();
