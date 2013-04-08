#include "<%= $cur_module.name %>_MethodResultImpl.h"

using namespace <%= $cur_module.name %>Runtime;

CMethodResultImpl::CMethodResultImpl(int64 native):
    oResult((rho::apiGenerator::CMethodResult*)native)
{
}

void CMethodResultImpl::set(Platform::String^ res)
{
    oResult->set(rho::common::convertToStringW(res->Data()));
}

void CMethodResultImpl::set(Windows::Foundation::Collections::IVectorView<Platform::String^>^ res)
{
    // TODO: implement types/class conversion
}

void CMethodResultImpl::set(Windows::Foundation::Collections::IMapView<Platform::String^, Platform::String^>^ res)
{
    // TODO: implement types/class conversion
}
