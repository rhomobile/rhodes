#include "I<%= $cur_module.name %>.h"

rho::common::CAutoPtr<<%= api_generator_cpp_MakeNamespace($cur_module.parents)%>C<%= $cur_module.name %>FactoryBase> <%= api_generator_cpp_MakeNamespace($cur_module.parents)%>C<%= $cur_module.name %>FactoryBase::m_pInstance;