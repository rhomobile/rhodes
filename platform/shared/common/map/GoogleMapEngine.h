#ifndef _RHOGOOGLEMAPENGINE_H_
#define _RHOGOOGLEMAPENGINE_H_

#include "common/map/MapEngine.h"

namespace rho
{
namespace map
{

class GoogleGeoCoding : public IGeoCoding
{
public:
    GoogleGeoCoding();
    void stop();
    void resolve(String const &address, GeoCodingCallback *cb);
};

} // namespace map
} // namespace rho

#endif
