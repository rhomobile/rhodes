// P2PClient.cpp : Defines the entry point for the console application.
//


#include <common/RhoStd.h>
#include "api_generator/MethodResult.h"

#include <algorithm>

namespace rho
{
namespace printing
{

template<typename T>
class ResultConverter
{
public:
    static void convert(rho::apiGenerator::CMethodResult& result, const rho::String& value)
    {
        result.set(value);
    }
};

template<>
class ResultConverter<bool>
{
public:
    static void convert(rho::apiGenerator::CMethodResult& result, const rho::String& value)
    {
        rho::String resultFromServer;
        std::transform(value.begin(), value.end(), std::back_inserter(resultFromServer), &tolower);

        if (resultFromServer == "false")
        {
            result.set(false);
        }
        else if (resultFromServer == "true")
        {
            result.set(true);
        }
        else
        {
            result.set(resultFromServer);
        }
    }
};

template<>
class ResultConverter<int>
{
public:
    static void convert(rho::apiGenerator::CMethodResult& result, const rho::String& value)
    {
        int intResult = atoi(value.c_str());
        result.set(intResult);
    }
};

} // printing
} // rho