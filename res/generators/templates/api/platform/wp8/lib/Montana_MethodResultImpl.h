#pragma once

#include "../runtime/<%= $cur_module.name %>Runtime.h"

namespace rhoruntime {
    public ref class C<%= $cur_module.name %>MethodResultImpl sealed: public I<%= $cur_module.name %>MethodResult
    {
        rho::apiGenerator::CMethodResult* oResult;
    public:
        C<%= $cur_module.name %>MethodResultImpl(int64 native);

        virtual void set(bool res);
        virtual void set(int64 res);
        virtual void set(int res);
        virtual void set(double res);
        virtual void set(Platform::String^ res);
        virtual void set(Windows::Foundation::Collections::IVectorView<Platform::String^>^ res);
        virtual void set(Windows::Foundation::Collections::IMapView<Platform::String^, Platform::String^>^ res);
    };
}
