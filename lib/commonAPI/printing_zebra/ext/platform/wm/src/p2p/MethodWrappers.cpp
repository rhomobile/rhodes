// P2PClient.cpp : Defines the entry point for the console application.
//

#include "common/ResultConverter.h"
#include "MethodWrappers.h"

namespace rho
{
namespace printing
{
namespace ipc
{
namespace wrappers
{

void ConnectWithParametersPrinterTask::resultExecute(const rho::String& resultFromServer, rho::apiGenerator::CMethodResult& apiResult)
{
    LOG(TRACE) + "ConnectWithParametersPrinterTask result: " + resultFromServer;

    apiResult.set(resultFromServer);
}

void ConnectPrinterTask::resultExecute(const rho::String& resultFromServer, rho::apiGenerator::CMethodResult& apiResult)
{
    LOG(TRACE) + "ConnectPrinterTask result: " + resultFromServer;

    apiResult.set(resultFromServer);
}

void DisconnectPrinterTask::resultExecute(const rho::String& resultFromServer, rho::apiGenerator::CMethodResult& apiResult)
{
    LOG(TRACE) + "DisconnectPrinterTask result: " + resultFromServer;

    apiResult.set(resultFromServer);
}

void GetTimeToWaitForMoreDataTask::resultExecute(const rho::String& resultFromServer, rho::apiGenerator::CMethodResult& apiResult)
{
    LOG(TRACE) + "GetTimeToWaitForMoreDataTask result: " + resultFromServer;

    ResultConverter<int>::convert(apiResult, resultFromServer);
}

void GetMaxTimeoutForReadTask::resultExecute(const rho::String& resultFromServer, rho::apiGenerator::CMethodResult& apiResult)
{
    LOG(TRACE) + "GetMaxTimeoutForReadTask result: " + resultFromServer;

    ResultConverter<int>::convert(apiResult, resultFromServer);
}

void IsConnectedTask::resultExecute(const rho::String& resultFromServer, rho::apiGenerator::CMethodResult& apiResult)
{
    LOG(TRACE) + "IsConnectedTask result: " + resultFromServer;

    ResultConverter<bool>::convert(apiResult, resultFromServer);

    m_isConnected = apiResult.getBool();
}

void PrintFileTask::resultExecute(const rho::String& resultFromServer, rho::apiGenerator::CMethodResult& apiResult)
{
    LOG(TRACE) + "PrintFileTask result: " + resultFromServer;

    apiResult.set(resultFromServer);
}

void EnumerateSupportedControlLanguagesTask::resultExecute(const rho::String& resultFromServer, rho::apiGenerator::CMethodResult& apiResult)
{
    LOG(TRACE) + "EnumerateSupportedControlLanguagesTask result: " + resultFromServer;

    rho::Vector<rho::String> result;
    
    json::CJSONEntry jsonObj(resultFromServer.c_str());

    if (jsonObj.hasName(CPrinterZebraBase::PRINTER_LANGUAGE_ZPL))
        result.push_back(CPrinterZebraBase::PRINTER_LANGUAGE_ZPL);

    if (jsonObj.hasName(CPrinterZebraBase::PRINTER_LANGUAGE_CPCL))
        result.push_back(CPrinterZebraBase::PRINTER_LANGUAGE_CPCL);

    apiResult.set(result);
}

void PrintStoredFormatWithHashTask::resultExecute(const rho::String& resultFromServer, rho::apiGenerator::CMethodResult& apiResult)
{
    LOG(TRACE) + "PrintStoredFormatWithHashTask result: " + resultFromServer;

    apiResult.set(resultFromServer);
}

void PrintStoredFormatWithArrayTask::resultExecute(const rho::String& resultFromServer, rho::apiGenerator::CMethodResult& apiResult)
{
    LOG(TRACE) + "PrintStoredFormatWithArrayTask result: " + resultFromServer;

    apiResult.set(resultFromServer);
}

void StoreImageTask::resultExecute(const rho::String& resultFromServer, rho::apiGenerator::CMethodResult& apiResult)
{
    LOG(TRACE) + "StoreImageTask result: " + resultFromServer;

    apiResult.set(resultFromServer);
}

void PrintImageFromFileTask::resultExecute(const rho::String& resultFromServer, rho::apiGenerator::CMethodResult& apiResult)
{
    LOG(TRACE) + "PrintImageFromFileTask result: " + resultFromServer;

    apiResult.set(resultFromServer);
}

void SendFileContentsTask::resultExecute(const rho::String& resultFromServer, rho::apiGenerator::CMethodResult& apiResult)
{
    LOG(TRACE) + "SendFileContentsTask result: " + resultFromServer;

    apiResult.set(resultFromServer);
}

} // wrapper
} // ipc
} // printing
} // rho

