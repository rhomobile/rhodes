#include "SecurityTokenGeneratorImpl.h"
#include <random>
#include <stdint.h>
#include <sstream>


namespace rho
{
namespace common
{

    SecurityTokenGenerator::SecurityTokenGenerator()
    {
        std::random_device device_engine;
        std::ostringstream ss;
        std::uniform_int_distribution<int> dist(0, 254);
        for(int i = 0; i < 32; i++)
        {
            int value = dist(device_engine);
            ss << std::hex << value;
        }

         currentTokens = ss.str();

    }

    SecurityTokenGenerator::~SecurityTokenGenerator()
    {
    }

    const rho::String& SecurityTokenGenerator::getSecurityToken() const
    {
        return currentTokens;
    }

}
}
