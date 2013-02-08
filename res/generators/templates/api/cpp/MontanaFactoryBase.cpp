#include "I<%= $cur_module.name %>.h"

rho::common::CAutoPtr< <%= api_generator_cpp_MakeNamespace($cur_module.parents)%>C<%= $cur_module.name %>FactoryBase> <%= api_generator_cpp_MakeNamespace($cur_module.parents)%>C<%= $cur_module.name %>FactoryBase::m_pInstance;

//string constants definiton <%
$cur_module.constants.each do |module_constant|
    if module_constant.type == MethodParam::TYPE_STRING %>
const wchar_t <%= api_generator_cpp_MakeNamespace($cur_module.parents)%>I<%= $cur_module.name %>::<%= module_constant.name %>[] = L"<%= module_constant.value %>"; <%
end; end %>
