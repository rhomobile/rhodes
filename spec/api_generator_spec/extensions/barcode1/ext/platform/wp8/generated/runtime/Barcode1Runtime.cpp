// Barcode1Runtime.cpp
#include "Barcode1Runtime.h"

using namespace Barcode1Runtime;
using namespace Platform;
using namespace rho::apiGenerator;

Barcode1RuntimeComponent::Barcode1RuntimeComponent(IBarcode1Impl^ impl):
	_impl(impl)
{
	// TODO: change this, it's temporary code for testing only!
	CBarcode1FactoryBase::getInstance()->createModuleByID(rho::common::convertToStringW(L"test"))->registerRuntime(this);
}

void Barcode1RuntimeComponent::enable(Windows::Foundation::Collections::IMapView<Platform::String^, Platform::String^>^ propertyMap, IMethodResult^ oResult)
{
	_impl->enable(propertyMap, oResult);
}

void Barcode1RuntimeComponent::start(IMethodResult^ oResult)
{
	_impl->start(oResult);
}

void Barcode1RuntimeComponent::stop(IMethodResult^ oResult)
{
	_impl->stop(oResult);
}

void Barcode1RuntimeComponent::disable(IMethodResult^ oResult)
{
	_impl->disable(oResult);
}

void Barcode1RuntimeComponent::take(Windows::Foundation::Collections::IMapView<Platform::String^, Platform::String^>^ propertyMap, IMethodResult^ oResult)
{
	_impl->take(propertyMap, oResult);
}
