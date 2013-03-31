#pragma once

#include "../../../shared/generated/cpp/I<%= name.camel_case %>.h"

namespace <%= name.camel_case() %>Runtime
{
    public interface class IMethodResult
    {
    public:
        [Windows::Foundation::Metadata::DefaultOverloadAttribute]
        void set(Platform::String^ res);
        void set(Windows::Foundation::Collections::IVectorView<Platform::String^>^ res);
        void set(Windows::Foundation::Collections::IMapView<Platform::String^, Platform::String^>^ res);
    };

    public interface class I<%= name.camel_case() %>Impl
    {
    public:
        void enable(Windows::Foundation::Collections::IMapView<Platform::String^, Platform::String^>^ propertyMap, IMethodResult^ oResult);
        void start(IMethodResult^ oResult);
        void stop(IMethodResult^ oResult);
        void disable(IMethodResult^ oResult);
        void take(Windows::Foundation::Collections::IMapView<Platform::String^, Platform::String^>^ propertyMap, IMethodResult^ oResult);
    };

    public ref class <%= name.camel_case() %>RuntimeComponent sealed: public I<%= name.camel_case() %>Impl
    {
    public:
        <%= name.camel_case() %>RuntimeComponent(I<%= name.camel_case() %>Impl^ impl);

        virtual void enable(Windows::Foundation::Collections::IMapView<Platform::String^, Platform::String^>^ propertyMap, IMethodResult^ oResult);
        virtual void start(IMethodResult^ oResult);
        virtual void stop(IMethodResult^ oResult);
        virtual void disable(IMethodResult^ oResult);
        virtual void take(Windows::Foundation::Collections::IMapView<Platform::String^, Platform::String^>^ propertyMap, IMethodResult^ oResult);

    private:
        I<%= name.camel_case() %>Impl^ _impl;
    };
}