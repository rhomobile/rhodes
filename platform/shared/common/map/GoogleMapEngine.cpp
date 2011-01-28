#include "common/map/GoogleMapEngine.h"

namespace rho
{
namespace map
{

GoogleGeoCoding::GoogleGeoCoding()
{
    // TODO:
}

void GoogleGeoCoding::stop()
{
    // TODO:
}

void GoogleGeoCoding::resolve(String const &address, GeoCodingCallback *cb)
{
    std::auto_ptr<GeoCodingCallback> callback(cb);
    // TODO:
}

} // namespace map
} // namespace rho

