#pragma once

#include "../IBarcode1.h"

namespace Barcode1Runtime
{
	public interface class IMethodResult
	{
	public:
		[Windows::Foundation::Metadata::DefaultOverloadAttribute]
	    void set(Platform::String^ res);
		void set(Windows::Foundation::Collections::IVectorView<Platform::String^>^ res);
	    void set(Windows::Foundation::Collections::IMapView<Platform::String^, Platform::String^>^ res);
	};

	public interface class IBarcode1Impl
	{
	public:
		void enable(Windows::Foundation::Collections::IMapView<Platform::String^, Platform::String^>^ propertyMap, IMethodResult^ oResult);
		void start(IMethodResult^ oResult);
		void stop(IMethodResult^ oResult);
		void disable(IMethodResult^ oResult);
		void take(Windows::Foundation::Collections::IMapView<Platform::String^, Platform::String^>^ propertyMap, IMethodResult^ oResult);
	};

	public ref class Barcode1RuntimeComponent sealed: public IBarcode1Impl
	{
	public:
		Barcode1RuntimeComponent(IBarcode1Impl^ impl);

		virtual void enable(Windows::Foundation::Collections::IMapView<Platform::String^, Platform::String^>^ propertyMap, IMethodResult^ oResult);
		virtual void start(IMethodResult^ oResult);
		virtual void stop(IMethodResult^ oResult);
		virtual void disable(IMethodResult^ oResult);
		virtual void take(Windows::Foundation::Collections::IMapView<Platform::String^, Platform::String^>^ propertyMap, IMethodResult^ oResult);

	private:
		IBarcode1Impl^ _impl;
	};
}