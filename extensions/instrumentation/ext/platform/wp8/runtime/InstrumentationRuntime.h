#pragma once

#include "../../../shared/generated/cpp/IInstrumentation.h"

namespace InstrumentationRuntime
{
    public interface class IMethodResult
    {
    public:
        [Windows::Foundation::Metadata::DefaultOverloadAttribute]
        void set(Platform::String^ res);
        void set(Windows::Foundation::Collections::IVectorView<Platform::String^>^ res);
        void set(Windows::Foundation::Collections::IMapView<Platform::String^, Platform::String^>^ res);
    };

    public interface class IInstrumentationImpl
    {
    public:
        void simulate_key_event_code(int keycode, IMethodResult^ oResult);
        void simulate_key_event_string(Platform::String^ str, IMethodResult^ oResult);
        void simulate_touch_event(int event_type, int x, int y, IMethodResult^ oResult);
        void screen_capture(Platform::String^ pszFilename, IMethodResult^ oResult);
        void get_allocated_memory(IMethodResult^ oResult);
        void delete_file(Platform::String^ str, IMethodResult^ oResult);
        void file_exists(Platform::String^ str, IMethodResult^ oResult);
        void re_simulate_navigation(IMethodResult^ oResult);
        void getProperty(Platform::String^ propertyName, IMethodResult^ oResult);
        void getProperties(Windows::Foundation::Collections::IVectorView<Platform::String^>^ arrayofNames, IMethodResult^ oResult);
        void getAllProperties(IMethodResult^ oResult);
        void setProperty(Platform::String^ propertyName, Platform::String^ propertyValue, IMethodResult^ oResult);
        void setProperties(Windows::Foundation::Collections::IMapView<Platform::String^, Platform::String^>^ propertyMap, IMethodResult^ oResult);
        void clearAllProperties(IMethodResult^ oResult);
    };

    public interface class IInstrumentationSingletonImpl
    {
    public:
        void enumerate(IMethodResult^ oResult);
    };

    public ref class InstrumentationRuntimeComponent sealed: public IInstrumentationImpl
    {
    public:
        InstrumentationRuntimeComponent(IInstrumentationImpl^ impl);
        virtual void simulate_key_event_code(int keycode, IMethodResult^ oResult);
        virtual void simulate_key_event_string(Platform::String^ str, IMethodResult^ oResult);
        virtual void simulate_touch_event(int event_type, int x, int y, IMethodResult^ oResult);
        virtual void screen_capture(Platform::String^ pszFilename, IMethodResult^ oResult);
        virtual void get_allocated_memory(IMethodResult^ oResult);
        virtual void delete_file(Platform::String^ str, IMethodResult^ oResult);
        virtual void file_exists(Platform::String^ str, IMethodResult^ oResult);
        virtual void re_simulate_navigation(IMethodResult^ oResult);
        virtual void getProperty(Platform::String^ propertyName, IMethodResult^ oResult);
        virtual void getProperties(Windows::Foundation::Collections::IVectorView<Platform::String^>^ arrayofNames, IMethodResult^ oResult);
        virtual void getAllProperties(IMethodResult^ oResult);
        virtual void setProperty(Platform::String^ propertyName, Platform::String^ propertyValue, IMethodResult^ oResult);
        virtual void setProperties(Windows::Foundation::Collections::IMapView<Platform::String^, Platform::String^>^ propertyMap, IMethodResult^ oResult);
        virtual void clearAllProperties(IMethodResult^ oResult);
    private:
        IInstrumentationImpl^ _impl;
    };

    public ref class InstrumentationSingletonComponent sealed: public IInstrumentationSingletonImpl
    {
    public:
        InstrumentationSingletonComponent(IInstrumentationSingletonImpl^ impl);
        virtual void enumerate(IMethodResult^ oResult);
    private:
        IInstrumentationSingletonImpl^ _impl;
    };
}