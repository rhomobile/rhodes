#include "Instrumentation_MethodResultImpl.h"

using namespace InstrumentationRuntime;

CInstrumentationMethodResultImpl::CInstrumentationMethodResultImpl(int64 native):
    oResult((rho::apiGenerator::CMethodResult*)native)
{
}

void CInstrumentationMethodResultImpl::set(Platform::String^ res)
{
    oResult->set(rho::common::convertToStringW(res->Data()));
}

void CInstrumentationMethodResultImpl::set(Windows::Foundation::Collections::IVectorView<Platform::String^>^ res)
{
    // TODO: implement types/class conversion
}

void CInstrumentationMethodResultImpl::set(Windows::Foundation::Collections::IMapView<Platform::String^, Platform::String^>^ res)
{
    // TODO: implement types/class conversion
}
