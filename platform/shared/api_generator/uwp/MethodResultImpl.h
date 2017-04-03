#pragma once

#include "../MethodResult.h"
#include "IMethodResult.h"

namespace rhoruntime {

    public ref class CMethodResultImpl sealed: public IMethodResult
    {
        rho::apiGenerator::CMethodResult* oResult;
    public:
		CMethodResultImpl();
		CMethodResultImpl(int64 native);
        virtual int64 getNative();
        virtual bool hasCallback();
        virtual void set(bool res);
        virtual void set(int64 res);
        virtual void set(int res);
        virtual void set(double res);
        virtual void set(Platform::String^ res);
        virtual void set(WFC::IVectorView<Platform::String^>^ res);
        virtual void set(WFC::IVectorView<WFC::IMapView<Platform::String^, Platform::String^>^>^ res);
        virtual void set(WFC::IMapView<Platform::String^, Platform::String^>^ res);
        virtual void set(WFC::IMapView<Platform::String^, WFC::IVectorView<Platform::String^>^>^ res);
        virtual void set(WFC::IMapView<Platform::String^, WFC::IMapView<Platform::String^, Platform::String^>^>^ res);
		virtual void setRubyCallback(Platform::String^ strCallback);
		virtual Platform::String^ getRubyCallback();
	};

}
