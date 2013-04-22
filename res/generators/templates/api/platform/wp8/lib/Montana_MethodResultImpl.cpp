#include "../../wp8/rhoruntime/common/RhoConvertWP8.h"
#include "<%= $cur_module.name %>_MethodResultImpl.h"
<% $cur_module.parents.each do |parent| %>
namespace <%= parent.downcase() %> {<%
end %>

using namespace <%= $cur_module.name %>Runtime;

C<%= $cur_module.name %>MethodResultImpl::C<%= $cur_module.name %>MethodResultImpl(int64 native):
    oResult((rho::apiGenerator::CMethodResult*)native)
{
}

void C<%= $cur_module.name %>MethodResultImpl::set(bool res)
{
    oResult->set(res);
}

void C<%= $cur_module.name %>MethodResultImpl::set(int64 res)
{
    oResult->set(res);
}

void C<%= $cur_module.name %>MethodResultImpl::set(int res)
{
    oResult->set(res);
}

void C<%= $cur_module.name %>MethodResultImpl::set(double res)
{
    oResult->set(res);
}

void C<%= $cur_module.name %>MethodResultImpl::set(Platform::String^ res)
{
    oResult->set(rho::common::convertStringWFromWP8(res));
}

void C<%= $cur_module.name %>MethodResultImpl::set(Windows::Foundation::Collections::IVectorView<Platform::String^>^ res)
{
    oResult->set(rho::common::convertArrayFromWP8(res));
}

void C<%= $cur_module.name %>MethodResultImpl::set(Windows::Foundation::Collections::IMapView<Platform::String^, Platform::String^>^ res)
{
    oResult->set(rho::common::convertHashFromWP8(res));
}

<% $cur_module.parents.each do |parent| %>
}<%
end %>
