#include <algorithm>
#include <cctype>

#include "common/RhodesApp.h"

#define Init_System_extension Init_System_stub_entry
#include "../../../../lib/commonAPI/coreapi/ext/shared/generated/stub/system_stub_impl.cpp"
#undef Init_System_extension

namespace {

bool is_platform_key(const rho::String& name) {
    rho::String lowered = name;
    std::transform(lowered.begin(), lowered.end(), lowered.begin(), [](unsigned char ch) {
        return static_cast<char>(std::tolower(ch));
    });
    return lowered == "platform";
}

class GtkSystemSingleton : public rho::CSystemSingletonImpl {
public:
    void getPlatform(rho::apiGenerator::CMethodResult& oResult) override {
        oResult.set("GTK");
    }

    void getProperty(const rho::String& propertyName, rho::apiGenerator::CMethodResult& oResult) override {
        if (is_platform_key(propertyName)) {
            getPlatform(oResult);
            return;
        }
        rho::CSystemSingletonImpl::getProperty(propertyName, oResult);
    }

    void getProperties(const rho::Vector<rho::String>& names, rho::apiGenerator::CMethodResult& oResult) override {
        rho::Hashtable<rho::String, rho::String> result;
        for (const rho::String& name : names) {
            if (is_platform_key(name)) {
                result.put(name, "GTK");
            }
        }
        if (result.size() > 0) {
            oResult.set(result);
            return;
        }
        rho::CSystemSingletonImpl::getProperties(names, oResult);
    }

    void getAllProperties(rho::apiGenerator::CMethodResult& oResult) override {
        rho::Hashtable<rho::String, rho::String> props;
        props.put("platform", "GTK");
        oResult.set(props);
    }
};

class GtkSystemFactory : public rho::CSystemFactory {
public:
    rho::ISystemSingleton* createModuleSingleton() override {
        static GtkSystemSingleton singleton;
        return &singleton;
    }
};

} // namespace

extern "C" void Init_System()
{
    Init_System_stub_entry();
    rho::CSystemFactory::setInstance(new GtkSystemFactory());
    RHODESAPP().getExtManager().requireRubyFile("RhoSystemApi");
}
