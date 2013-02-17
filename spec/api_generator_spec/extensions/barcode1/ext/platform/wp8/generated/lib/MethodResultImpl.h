#pragma once

#include "../runtime/Barcode1Runtime.h"

namespace Barcode1Runtime
{

	public ref class CMethodResultImpl sealed: public IMethodResult
	{
		rho::apiGenerator::CMethodResult* oResult;
	public:
		CMethodResultImpl(int64 native);

		virtual void set(Platform::String^ res);
		virtual void set(Windows::Foundation::Collections::IVectorView<Platform::String^>^ res);
		virtual void set(Windows::Foundation::Collections::IMapView<Platform::String^, Platform::String^>^ res);
	};

}
