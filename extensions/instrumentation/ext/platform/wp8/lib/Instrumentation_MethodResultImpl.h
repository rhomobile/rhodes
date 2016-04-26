#pragma once

#include "../runtime/InstrumentationRuntime.h"

namespace InstrumentationRuntime
{
    public ref class CInstrumentationMethodResultImpl sealed: public IMethodResult
    {
        rho::apiGenerator::CMethodResult* oResult;
    public:
        CInstrumentationMethodResultImpl(int64 native);

        virtual void set(Platform::String^ res);
        virtual void set(Windows::Foundation::Collections::IVectorView<Platform::String^>^ res);
        virtual void set(Windows::Foundation::Collections::IMapView<Platform::String^, Platform::String^>^ res);
    };
}
