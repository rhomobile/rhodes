#pragma once

namespace WFC = Windows::Foundation::Collections;

namespace rhoruntime {

    public interface class IMethodResult
    {
    public:
        int64 getNative();
        bool hasCallback();
        [Windows::Foundation::Metadata::DefaultOverloadAttribute]
        void set(bool res);
        void set(int64 res);
        void set(int res);
        void set(double res);
        void set(Platform::String^ res);
        void set(WFC::IVectorView<Platform::String^>^ res);
        void set(WFC::IVectorView<WFC::IMapView<Platform::String^, Platform::String^>^>^ res);
        void set(WFC::IMapView<Platform::String^, Platform::String^>^ res);
        void set(WFC::IMapView<Platform::String^, WFC::IVectorView<Platform::String^>^>^ res);
		void set(WFC::IMapView<Platform::String^, WFC::IMapView<Platform::String^, Platform::String^>^>^ res);
        void setRubyCallback(Platform::String^ strCallback);
		Platform::String^ getRubyCallback();
	};

}
