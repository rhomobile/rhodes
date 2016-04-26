// P2PClient.cpp : Defines the entry point for the console application.
//

#include "ThreadMessageQueue.h"
#include "Method.h"
#include "../PrinterManager.h"

#include <common/RhoStd.h>
#include <json/JSONIterator.h>

namespace rho
{
namespace printing
{
namespace ipc
{
namespace wrappers
{

//////////////////////////////////////////////////////////////////////////
// dummy command class

class EmptyCommand : public rho::printing::ipc::thread::MessageQueueCommand
{
};

//////////////////////////////////////////////////////////////////////////
// base classes for queue commands 

class MessageHashResult : public rho::printing::ipc::thread::MessageQueueCommand
{
public:
    MessageHashResult(const rho::String& id, const rho::String& funcName, const rho::Hashtable<rho::String, rho::String>& hash, rho::apiGenerator::CMethodResult& oResult)
        : MessageQueueCommand(oResult)
    {
        m_message = printing::ipc::message::constructIpcMessage(id, funcName, hash);
    }
};

class MessageStringAndResult : public rho::printing::ipc::thread::MessageQueueCommand
{
public:
    MessageStringAndResult(const rho::String& id, const rho::String& funcName, const rho::String& strParam, rho::apiGenerator::CMethodResult& oResult)
        : MessageQueueCommand(oResult)
    {
        m_message = printing::ipc::message::constructIpcMessage(id, funcName, strParam);
    }
};

class MessageStringAndHashResult : public rho::printing::ipc::thread::MessageQueueCommand
{
public:
    MessageStringAndHashResult(const rho::String& id, const rho::String& funcName, const rho::String& strParam, 
        const rho::Hashtable<rho::String, rho::String>& hash, rho::apiGenerator::CMethodResult& oResult)
        : MessageQueueCommand(oResult)
    {
        m_message = printing::ipc::message::constructIpcMessage(id, funcName, strParam, hash);
    }
};

class MessageOnlyResult : public rho::printing::ipc::thread::MessageQueueCommand
{
public:
    MessageOnlyResult(const rho::String& id, const rho::String& funcName, rho::apiGenerator::CMethodResult& oResult)
        : MessageQueueCommand(oResult)
    {
        m_message = printing::ipc::message::constructIpcMessage(id, funcName);
    }
};

class MessageIntAndResult : public rho::printing::ipc::thread::MessageQueueCommand
{
public:
    MessageIntAndResult(const rho::String& id, const rho::String& funcName, int intParam, rho::apiGenerator::CMethodResult& oResult)
        : MessageQueueCommand(oResult)
    {
        m_message = printing::ipc::message::constructIpcMessage(id, funcName, intParam);
    }
};

class MessageStringIntIntHashAndResult : public rho::printing::ipc::thread::MessageQueueCommand
{
public:
    MessageStringIntIntHashAndResult(const rho::String& id, const rho::String& funcName, 
                                     const rho::String& strParam, 
                                     int intParam1, 
                                     int intParam2, 
                                     const rho::Hashtable<rho::String, rho::String>& hash, 
                                     rho::apiGenerator::CMethodResult& oResult)
                                     : MessageQueueCommand(oResult)
    {
        m_message = printing::ipc::message::constructIpcMessage(id, funcName, strParam, intParam1, intParam2, hash);
    }
};

class MessageStringStringIntIntAndResult : public rho::printing::ipc::thread::MessageQueueCommand
{
public:
    MessageStringStringIntIntAndResult(const rho::String& id, const rho::String& funcName, 
        const rho::String& strParam, 
        const rho::String& strParam2, 
        int intParam, 
        int intParam2, 
        rho::apiGenerator::CMethodResult& oResult)
        : MessageQueueCommand(oResult)
    {
        m_message = printing::ipc::message::constructIpcMessage(id, funcName, strParam, strParam2, intParam, intParam2);
    }
};

class MessageListAndResult : public rho::printing::ipc::thread::MessageQueueCommand
{
public:
    MessageListAndResult(const rho::String& id, const rho::String& funcName, 
        const rho::Vector<rho::String>& list, rho::apiGenerator::CMethodResult& oResult) : MessageQueueCommand(oResult)
    {
        m_message = printing::ipc::message::constructIpcMessage(id, funcName, list);
    }
};

class MessageStringListAndResult : public rho::printing::ipc::thread::MessageQueueCommand
{
public:
    MessageStringListAndResult(const rho::String& id, const rho::String& funcName, const rho::String& strParam, 
        const rho::Vector<rho::String>& list, rho::apiGenerator::CMethodResult& oResult) : MessageQueueCommand(oResult)
    {
        m_message = printing::ipc::message::constructIpcMessage(id, funcName, strParam, list);
    }
};

//////////////////////////////////////////////////////////////////////////
// zebra singleton methods

template<typename PrinterType, typename PrinterImplType, typename PrinterBaseImplType>
class SearchPrintersTask : public MessageHashResult
{
public:
    SearchPrintersTask(const rho::Hashtable<rho::String, rho::String>& hash, rho::apiGenerator::CMethodResult& oResult)
        : MessageHashResult("", "searchPrinters", hash, oResult) {}

    virtual void resultExecute(const rho::String& resultFromServer, rho::apiGenerator::CMethodResult& apiResult)
    {
        LOG(TRACE) + "resultExecute: " + resultFromServer;

        rho::apiGenerator::CMethodResult propResult;
        rho::Hashtable<string, string> hash;

        json::CJSONEntry jsonObj(resultFromServer.c_str());

        if (jsonObj.hasName(printerzebra::HK_PRINTER_ID))
        {
            rho::String id = jsonObj.getString(printerzebra::HK_PRINTER_ID);

            PrinterBaseImplType* printer = static_cast<PrinterBaseImplType*>(PrinterManager<PrinterType, PrinterImplType>::getInstance()->addPrinterWithID(id));

            if (jsonObj.hasName(printerzebra::PROPERTY_CONNECTION_TYPE))
                printer->setProperty(printerzebra::PROPERTY_CONNECTION_TYPE, jsonObj.getString(printerzebra::PROPERTY_CONNECTION_TYPE), propResult);

            if (jsonObj.hasName(printerzebra::PROPERTY_DEVICE_ADDRESS))
                printer->setProperty(printerzebra::PROPERTY_DEVICE_ADDRESS, jsonObj.getString(printerzebra::PROPERTY_DEVICE_ADDRESS), propResult);

            if (jsonObj.hasName(printerzebra::PROPERTY_DEVICE_PORT))
                printer->setProperty(printerzebra::PROPERTY_DEVICE_PORT, jsonObj.getString(printerzebra::PROPERTY_DEVICE_PORT), propResult);

            if (jsonObj.hasName(printerzebra::PROPERTY_PRINTER_TYPE))
                printer->setProperty(printerzebra::PROPERTY_PRINTER_TYPE, jsonObj.getString(printerzebra::PROPERTY_PRINTER_TYPE), propResult);

            if (jsonObj.hasName(printerzebra::PROPERTY_DEVICE_NAME))
                printer->setProperty(printerzebra::PROPERTY_DEVICE_NAME, jsonObj.getString(printerzebra::PROPERTY_DEVICE_NAME), propResult);

            printer->setProperty(printerzebra::PROPERTY_ID, id, propResult);

            hash.put(printerzebra::HK_PRINTER_ID, id);
        }

        if (jsonObj.hasName(printerzebra::HK_STATUS)) {
            hash.put(printerzebra::HK_STATUS, jsonObj.getString(printerzebra::HK_STATUS));
        }

        apiResult.set(hash);
    }
};

class StopSearchTask : public MessageOnlyResult
{
public:
    StopSearchTask(rho::apiGenerator::CMethodResult& oResult)
        : MessageOnlyResult("", "stopSearch", oResult) {}
};

//////////////////////////////////////////////////////////////////////////
// zebra objects methods

class ConnectPrinterTask : public MessageOnlyResult
{
private:
    rho::String m_printerID;

public:
    ConnectPrinterTask(const rho::String id, rho::apiGenerator::CMethodResult& oResult)
        : MessageOnlyResult(id, "connect", oResult), m_printerID(id) {}

    virtual void resultExecute(const rho::String& resultFromServer, rho::apiGenerator::CMethodResult& apiResult);
};

class DisconnectPrinterTask : public MessageOnlyResult
{
public:
    DisconnectPrinterTask(const rho::String id, rho::apiGenerator::CMethodResult& oResult)
        : MessageOnlyResult(id, "disconnect", oResult) {}

    virtual void resultExecute(const rho::String& resultFromServer, rho::apiGenerator::CMethodResult& apiResult);
};

class ConnectWithParametersPrinterTask : public MessageHashResult
{
public:
    ConnectWithParametersPrinterTask(const rho::String id, const rho::Hashtable<rho::String, rho::String>& hash, rho::apiGenerator::CMethodResult& oResult)
        : MessageHashResult(id, "connectWithOptions", hash, oResult) {}

    virtual void resultExecute(const rho::String& resultFromServer, rho::apiGenerator::CMethodResult& apiResult);
};

class PrintRawStringTask : public MessageStringAndHashResult
{
public:
    PrintRawStringTask(const rho::String id, const rho::String command, const rho::Hashtable<rho::String, rho::String>& hash, rho::apiGenerator::CMethodResult& oResult)
        : MessageStringAndHashResult(id, "printRawString", command, hash, oResult) {}
};

class PrintFileTask : public MessageStringAndHashResult
{
public:
    PrintFileTask(const rho::String id, const rho::String fileUri, const rho::Hashtable<rho::String, rho::String>& hash, rho::apiGenerator::CMethodResult& oResult)
        : MessageStringAndHashResult(id, "printFile", fileUri, hash, oResult) {}

    virtual void resultExecute(const rho::String& resultFromServer, rho::apiGenerator::CMethodResult& apiResult);
};

class SendFileContentsTask : public MessageStringAndResult
{
public:
    SendFileContentsTask(const rho::String id, const rho::String filePath, rho::apiGenerator::CMethodResult& oResult)
        : MessageStringAndResult(id, "sendFileContents", filePath, oResult) {}

    virtual void resultExecute(const rho::String& resultFromServer, rho::apiGenerator::CMethodResult& apiResult);
};

class SetMaxTimeoutForReadTask : public MessageIntAndResult
{
public:
    SetMaxTimeoutForReadTask(const rho::String id, int maxTimeoutForRead, rho::apiGenerator::CMethodResult& oResult)
        : MessageIntAndResult(id, "setMaxTimeoutForRead", maxTimeoutForRead, oResult) {}
};

class GetMaxTimeoutForReadTask : public MessageOnlyResult
{
public:
    GetMaxTimeoutForReadTask(const rho::String id, rho::apiGenerator::CMethodResult& oResult)
        : MessageOnlyResult(id, "getMaxTimeoutForRead", oResult) {}

    virtual void resultExecute(const rho::String& resultFromServer, rho::apiGenerator::CMethodResult& apiResult);
};

class SetTimeToWaitForMoreDataTask : public MessageIntAndResult
{
public:
    SetTimeToWaitForMoreDataTask(const rho::String id, int timeToWaitForMoreData, rho::apiGenerator::CMethodResult& oResult)
        : MessageIntAndResult(id, "setTimeToWaitForMoreData", timeToWaitForMoreData, oResult) {}
};

class GetTimeToWaitForMoreDataTask : public MessageOnlyResult
{
public:
    GetTimeToWaitForMoreDataTask(const rho::String id, rho::apiGenerator::CMethodResult& oResult)
        : MessageOnlyResult(id, "getTimeToWaitForMoreData", oResult) {}

    virtual void resultExecute(const rho::String& resultFromServer, rho::apiGenerator::CMethodResult& apiResult);
};

class IsConnectedTask : public MessageOnlyResult
{
private:
    bool m_isConnected;
public:
    IsConnectedTask(const rho::String id, rho::apiGenerator::CMethodResult& oResult)
        : MessageOnlyResult(id, "getIsConnected", oResult), m_isConnected(false) {}

    virtual void resultExecute(const rho::String& resultFromServer, rho::apiGenerator::CMethodResult& apiResult);

    bool isConnected() const { return m_isConnected; }
};

//////////////////////////////////////////////////////////////////////////

class EnumerateSupportedControlLanguagesTask : public MessageOnlyResult
{
public:
    EnumerateSupportedControlLanguagesTask(const rho::String id, rho::apiGenerator::CMethodResult& oResult)
        : MessageOnlyResult(id, "enumerateSupportedControlLanguages", oResult) {}

    virtual void resultExecute(const rho::String& resultFromServer, rho::apiGenerator::CMethodResult& apiResult);
};

class RetrieveFileNamesTask : public MessageOnlyResult
{
public:
    RetrieveFileNamesTask(const rho::String id, rho::apiGenerator::CMethodResult& oResult)
        : MessageOnlyResult(id, "retrieveFileNames", oResult) {}
};

class RetrieveFileNamesWithExtensionsTask : public MessageListAndResult
{
public:
    RetrieveFileNamesWithExtensionsTask(const rho::String id, const rho::Vector<rho::String>& extensions, rho::apiGenerator::CMethodResult& oResult)
        : MessageListAndResult(id, "retrieveFileNamesWithExtensions", extensions, oResult) {}
};

class PrintStoredFormatWithHashTask : public MessageStringAndHashResult
{
public:
    PrintStoredFormatWithHashTask(const rho::String id,  const rho::String& formatPathOnPrinter, const rho::Hashtable<rho::String, rho::String>& vars, rho::apiGenerator::CMethodResult& oResult)
        : MessageStringAndHashResult(id, "printStoredFormatWithHash", formatPathOnPrinter, vars, oResult) {}

    virtual void resultExecute(const rho::String& resultFromServer, rho::apiGenerator::CMethodResult& apiResult);
};

class PrintStoredFormatWithArrayTask : public MessageStringListAndResult
{
public:
    PrintStoredFormatWithArrayTask(const rho::String id,  const rho::String& formatPathOnPrinter, const rho::Vector<rho::String>& vars, rho::apiGenerator::CMethodResult& oResult)
        : MessageStringListAndResult(id, "printStoredFormatWithArray", formatPathOnPrinter, vars, oResult) {}

    virtual void resultExecute(const rho::String& resultFromServer, rho::apiGenerator::CMethodResult& apiResult);
};

class StoreImageTask : public MessageStringStringIntIntAndResult
{
public:
    StoreImageTask(const rho::String id,  const rho::String& printerDriveAndFileName,  const rho::String& imageFilePathOnDevice,  
        int width,  int height, rho::apiGenerator::CMethodResult& oResult)
            : MessageStringStringIntIntAndResult(id, "storeImage", printerDriveAndFileName, imageFilePathOnDevice, width, height, oResult) {}

    virtual void resultExecute(const rho::String& resultFromServer, rho::apiGenerator::CMethodResult& apiResult);
};

class PrintImageFromFileTask : public MessageStringIntIntHashAndResult
{
public:
    PrintImageFromFileTask(const rho::String id,  const rho::String& path, int x,  int y, 
        const rho::Hashtable<rho::String, rho::String>& options, rho::apiGenerator::CMethodResult& oResult)
            : MessageStringIntIntHashAndResult(id, "printImageFromFile", path, x, y, options, oResult) {}

    virtual void resultExecute(const rho::String& resultFromServer, rho::apiGenerator::CMethodResult& apiResult);
};

class RequestStateTask : public MessageListAndResult
{
public:
    RequestStateTask(const rho::String id,  const rho::Vector<rho::String>& listOfParameters, rho::apiGenerator::CMethodResult& oResult)
        : MessageListAndResult(id, "requestState", listOfParameters, oResult) {} 
};

class RegisterClient : public MessageStringAndResult
{
public:
    RegisterClient(const rho::String& clientName, rho::apiGenerator::CMethodResult& oResult) :
        MessageStringAndResult("", "registerClient", clientName, oResult) {}
};

class UnregisterClient : public MessageStringAndResult
{
public:
    UnregisterClient(const rho::String& clientName, rho::apiGenerator::CMethodResult& oResult) :
        MessageStringAndResult("", "unregisterClient", clientName, oResult) {}
};

}
}
} // printing
} // rho