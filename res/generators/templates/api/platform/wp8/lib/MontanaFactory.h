#pragma once

#include "../../../../shared/generated/cpp/<%= $cur_module.name %>Base.h"
#include "<%= $cur_module.name %>Runtime.h"

<% $cur_module.parents.each do |parent| %>
namespace <%= parent.downcase() %> {<%
end%>
class C<%= $cur_module.name %>Factory: public C<%= $cur_module.name %>FactoryBase
{
private:
    static rhoruntime::I<%= $cur_module.name %>FactoryImpl^ _impl;
public:
    static void setImpl(rhoruntime::I<%= $cur_module.name %>FactoryImpl^ impl) { _impl = impl; }
    ~C<%= $cur_module.name %>Factory(){}
    virtual I<%= $cur_module.name %>Singleton* createModuleSingleton();
    virtual I<%= $cur_module.name %>* createModuleByID(const rho::String& strID);
};
<% $cur_module.parents.each do |parent| %>
}<%
end %>
