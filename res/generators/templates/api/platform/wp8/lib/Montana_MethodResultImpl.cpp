#include "<%= $cur_module.name %>_MethodResultImpl.h"

using namespace <%= $cur_module.name %>Runtime;

C<%= $cur_module.name %>MethodResultImpl::C<%= $cur_module.name %>MethodResultImpl(int64 native):
    oResult((rho::apiGenerator::CMethodResult*)native)
{
}

void C<%= $cur_module.name %>MethodResultImpl::set(Platform::String^ res)
{
    oResult->set(rho::common::convertToStringW(res->Data()));
}

void C<%= $cur_module.name %>MethodResultImpl::set(Windows::Foundation::Collections::IVectorView<Platform::String^>^ res)
{
    // TODO: implement types/class conversion
}

void C<%= $cur_module.name %>MethodResultImpl::set(Windows::Foundation::Collections::IMapView<Platform::String^, Platform::String^>^ res)
{
    // TODO: implement types/class conversion
}
