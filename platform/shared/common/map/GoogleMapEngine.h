#ifndef _RHOGOOGLEMAPENGINE_H_
#define _RHOGOOGLEMAPENGINE_H_

#include "common/map/MapEngine.h"
#include "common/ThreadQueue.h"
#include "net/INetRequest.h"

namespace rho
{
namespace common
{
namespace map
{

class GoogleGeoCoding : public IGeoCoding, public CThreadQueue
{
private:
    struct Command : public IQueueCommand
    {
        String address;
        std::auto_ptr<GeoCodingCallback> callback;

        Command(String const &a, GeoCodingCallback *cb)
            :address(a), callback(cb)
        {}

        bool equals(IQueueCommand const &) {return false;}
        String toString();
    };

public:
    GoogleGeoCoding();
    ~GoogleGeoCoding();

    void stop();
    void resolve(String const &address, GeoCodingCallback *cb);

private:
    bool fetchData(String const &url, void **data, size_t *datasize);

    void processCommand(IQueueCommand* cmd);

private:
    std::auto_ptr<net::INetRequest> m_net_request;
};

} // namespace map
} // namespace common
} // namespace rho

#endif
