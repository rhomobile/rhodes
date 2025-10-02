#include "common/IRhoClassFactory.h"
#include "common/PosixThreadImpl.h"
#include "net/linux/SSLImpl.h"
#include "net/CURLNetRequest.h"

namespace rho {
namespace common {

class CGtkRhoClassFactory : public CBaseRhoClassFactory {
public:
    IRhoThreadImpl* createThreadImpl() override { return new CPosixThreadImpl(); }
    net::ISSL* createSSLEngine() override {
        if (!m_ssl) m_ssl = new net::SSLImpl();
        return m_ssl;
    }
    IRhoCrypt* createRhoCrypt() override { return nullptr; }
    net::INetRequestImpl* createNetRequestImpl() override { return new net::CURLNetRequest(); }
    const ISecurityTokenGenerator* createSecurityTokenGenerator() override { return nullptr; }
private:
    net::ISSL* m_ssl = nullptr;
};

} // namespace common
} // namespace rho

rho::common::IRhoClassFactory* rho_get_RhoClassFactory() {
    static rho::common::CGtkRhoClassFactory s_factory;
    return &s_factory;
}
