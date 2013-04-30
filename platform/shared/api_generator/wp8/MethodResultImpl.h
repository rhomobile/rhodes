#pragma once

#include "../MethodResult.h"
#include "IMethodResult.h"

namespace rhoruntime {

    public ref class CMethodResultImpl sealed: public IMethodResult
    {
        rho::apiGenerator::CMethodResult* oResult;
    public:
        CMethodResultImpl(int64 native);

        virtual void set(bool res);
        virtual void set(int64 res);
        virtual void set(int res);
        virtual void set(double res);
        virtual void set(Platform::String^ res);
        virtual void set(Windows::Foundation::Collections::IVectorView<Platform::String^>^ res);
        virtual void set(Windows::Foundation::Collections::IMapView<Platform::String^, Platform::String^>^ res);
    };

}
