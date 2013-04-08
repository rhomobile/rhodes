// <%= name.camel_case() %>Runtime.cpp
#include "<%= name.camel_case() %>Runtime.h"

using namespace <%= name.camel_case() %>Runtime;
using namespace Platform;
using namespace rho::apiGenerator;

<%= name.camel_case() %>RuntimeComponent::<%= name.camel_case() %>RuntimeComponent(I<%= name.camel_case() %>Impl^ impl):
    _impl(impl)
{
    // TODO: it's temporary code for testing only => commented out
    //C<%= name.camel_case() %>FactoryBase::getInstance()->createModuleByID(rho::common::convertToString(L"test"))->registerRuntime(this);
}

void <%= name.camel_case() %>RuntimeComponent::enable(Windows::Foundation::Collections::IMapView<Platform::String^, Platform::String^>^ propertyMap, IMethodResult^ oResult)
{
    _impl->enable(propertyMap, oResult);
}

void <%= name.camel_case() %>RuntimeComponent::start(IMethodResult^ oResult)
{
    _impl->start(oResult);
}

void <%= name.camel_case() %>RuntimeComponent::stop(IMethodResult^ oResult)
{
    _impl->stop(oResult);
}

void <%= name.camel_case() %>RuntimeComponent::disable(IMethodResult^ oResult)
{
    _impl->disable(oResult);
}

void <%= name.camel_case() %>RuntimeComponent::take(Windows::Foundation::Collections::IMapView<Platform::String^, Platform::String^>^ propertyMap, IMethodResult^ oResult)
{
    _impl->take(propertyMap, oResult);
}
