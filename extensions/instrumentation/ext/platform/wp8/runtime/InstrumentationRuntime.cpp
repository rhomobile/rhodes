// InstrumentationRuntime.cpp
#include "InstrumentationRuntime.h"

using namespace InstrumentationRuntime;
using namespace Platform;
using namespace rho::apiGenerator;

InstrumentationRuntimeComponent::InstrumentationRuntimeComponent(IInstrumentationImpl^ impl):
    _impl(impl)
{
    // TODO: implement runtime component constructor
}

InstrumentationSingletonComponent::InstrumentationSingletonComponent(IInstrumentationSingletonImpl^ impl):
    _impl(impl)
{
    // TODO: implement singleton component constructor
}

void InstrumentationRuntimeComponent::simulate_key_event_code(int keycode, IMethodResult^ oResult)
{
    _impl->simulate_key_event_code(keycode, oResult);
}

void InstrumentationRuntimeComponent::simulate_key_event_string(Platform::String^ str, IMethodResult^ oResult)
{
    _impl->simulate_key_event_string(str, oResult);
}

void InstrumentationRuntimeComponent::simulate_touch_event(int event_type, int x, int y, IMethodResult^ oResult)
{
    _impl->simulate_touch_event(event_type, x, y, oResult);
}

void InstrumentationRuntimeComponent::screen_capture(Platform::String^ pszFilename, IMethodResult^ oResult)
{
    _impl->screen_capture(pszFilename, oResult);
}

void InstrumentationRuntimeComponent::get_allocated_memory(IMethodResult^ oResult)
{
    _impl->get_allocated_memory(oResult);
}

void InstrumentationRuntimeComponent::delete_file(Platform::String^ str, IMethodResult^ oResult)
{
    _impl->delete_file(str, oResult);
}

void InstrumentationRuntimeComponent::file_exists(Platform::String^ str, IMethodResult^ oResult)
{
    _impl->file_exists(str, oResult);
}

void InstrumentationRuntimeComponent::re_simulate_navigation(IMethodResult^ oResult)
{
    _impl->re_simulate_navigation(oResult);
}

void InstrumentationRuntimeComponent::getProperty(Platform::String^ propertyName, IMethodResult^ oResult)
{
    _impl->getProperty(propertyName, oResult);
}

void InstrumentationRuntimeComponent::getProperties(Windows::Foundation::Collections::IVectorView<Platform::String^>^ arrayofNames, IMethodResult^ oResult)
{
    _impl->getProperties(arrayofNames, oResult);
}

void InstrumentationRuntimeComponent::getAllProperties(IMethodResult^ oResult)
{
    _impl->getAllProperties(oResult);
}

void InstrumentationRuntimeComponent::setProperty(Platform::String^ propertyName, Platform::String^ propertyValue, IMethodResult^ oResult)
{
    _impl->setProperty(propertyName, propertyValue, oResult);
}

void InstrumentationRuntimeComponent::setProperties(Windows::Foundation::Collections::IMapView<Platform::String^, Platform::String^>^ propertyMap, IMethodResult^ oResult)
{
    _impl->setProperties(propertyMap, oResult);
}

void InstrumentationRuntimeComponent::clearAllProperties(IMethodResult^ oResult)
{
    _impl->clearAllProperties(oResult);
}

void InstrumentationSingletonComponent::enumerate(IMethodResult^ oResult)
{
    _impl->enumerate(oResult);
}
