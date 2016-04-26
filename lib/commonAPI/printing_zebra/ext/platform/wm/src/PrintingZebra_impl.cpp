#include "PrintingZebra_impl.h"
#include "PrinterManager.h"

#include "p2p/ThreadMessageQueue.h"
#include "p2p/MethodWrappers.h"


namespace rho {

using namespace apiGenerator;
using namespace printing::ipc::wrappers;
using namespace printing::ipc::thread;

void CPrinterZebraImpl::connect(rho::apiGenerator::CMethodResult& oResult) 
{
    LOG(TRACE) + "[zebra] connect send";

    ConnectPrinterTask *task = new ConnectPrinterTask(m_ID, oResult);

    if (oResult.hasCallback())
    {
        ThreadMessageQueue::getInstance()->addQueueCommand(task);
    }
    else
    {        
        ThreadMessageQueue::getInstance()->processCommand(task);
        rho::String str = task->getResult().getString();
        oResult.set(str);
        delete task;
    }
}

void CPrinterZebraImpl::connectWithOptions( const rho::Hashtable<rho::String, rho::String>& options, rho::apiGenerator::CMethodResult& oResult)
{    
    LOG(TRACE) + "[zebra] connectWithOptions send";

    rho::apiGenerator::CMethodResult propResult;

    rho::Hashtable<rho::String, rho::String> fullOptions = options;

    ConnectWithParametersPrinterTask *task = new ConnectWithParametersPrinterTask(m_ID, fullOptions, oResult);

    if (oResult.hasCallback())
    {
        ThreadMessageQueue::getInstance()->addQueueCommand(task);
    }
    else
    {        
        ThreadMessageQueue::getInstance()->processCommand(task);
        rho::String str = task->getResult().getString();
        oResult.set(str);
        delete task;
    }
}

void CPrinterZebraImpl::disconnect(rho::apiGenerator::CMethodResult& oResult) 
{
    LOG(TRACE) + "[zebra] disconnect send";

    ThreadMessageQueue::getInstance()->addQueueCommand(new DisconnectPrinterTask(m_ID, oResult));
}

void CPrinterZebraImpl::printFile( const rho::String& fileURI,  const rho::Hashtable<rho::String, rho::String>& options, rho::apiGenerator::CMethodResult& oResult) 
{
    LOG(TRACE) + "[zebra] printFile send";

    ThreadMessageQueue::getInstance()->addQueueCommand(new PrintFileTask(m_ID, fileURI, options, oResult));
}

void CPrinterZebraImpl::printRawString( const rho::String& command,  const rho::Hashtable<rho::String, rho::String>& options, rho::apiGenerator::CMethodResult& oResult) 
{
    LOG(TRACE) + "[zebra] printRawString send";

    ThreadMessageQueue::getInstance()->addQueueCommand(new PrintRawStringTask(m_ID, command, options, oResult));
}

void CPrinterZebraImpl::getDeviceName(rho::apiGenerator::CMethodResult& oResult) 
{
    LOG(TRACE) + "[zebra] getDeviceName send";

    ThreadMessageQueue::getInstance()->addQueueCommand(new MessageOnlyResult(m_ID, "getDeviceName", oResult));
}

void CPrinterZebraImpl::sendFileContents( const rho::String& filePath, rho::apiGenerator::CMethodResult& oResult) 
{
    LOG(TRACE) + "[zebra] sendFileContents send";

    ThreadMessageQueue::getInstance()->addQueueCommand(new SendFileContentsTask(m_ID, filePath, oResult));
}

void CPrinterZebraImpl::printImageFromFile( const rho::String& path,  int x,  int y,  const rho::Hashtable<rho::String, rho::String>& options, rho::apiGenerator::CMethodResult& oResult) 
{
    LOG(TRACE) + "[zebra] printImageFromFile send";

    ThreadMessageQueue::getInstance()->addQueueCommand(new PrintImageFromFileTask(m_ID, path, x, y, options, oResult));
}

void CPrinterZebraImpl::getIsConnected(rho::apiGenerator::CMethodResult& oResult) 
{
    LOG(TRACE) + "[zebra] getIsConnected send";

    IsConnectedTask *task = new IsConnectedTask(m_ID, oResult);
    ThreadMessageQueue::getInstance()->processCommand(task);

    rho::apiGenerator::CMethodResult dumbResult;
    setProperty(printerzebra::PROPERTY_IS_CONNECTED, task->isConnected() ? "true" : "false", dumbResult);

    oResult.set(task->isConnected());
}

void CPrinterZebraImpl::enumerateSupportedControlLanguages(rho::apiGenerator::CMethodResult& oResult)
{
    LOG(TRACE) + "[zebra] enumerateSupportedControlLanguages send";

    EnumerateSupportedControlLanguagesTask *task = new EnumerateSupportedControlLanguagesTask(m_ID, oResult);

    if (oResult.hasCallback())
    {
        ThreadMessageQueue::getInstance()->addQueueCommand(task);
    }
    else
    {        
        ThreadMessageQueue::getInstance()->processCommand(task);
        oResult.set(task->getResult().getStringArray());
        delete task;
    }
}

void CPrinterZebraImpl::retrieveFileNames(rho::apiGenerator::CMethodResult& oResult) 
{
    LOG(TRACE) + "[zebra] retrieveFileNames send";

    ThreadMessageQueue::getInstance()->addQueueCommand(new RetrieveFileNamesTask(m_ID, oResult));
}

void CPrinterZebraImpl::retrieveFileNamesWithExtensions( const rho::Vector<rho::String>& extensions, rho::apiGenerator::CMethodResult& oResult) 
{
    LOG(TRACE) + "[zebra] retrieveFileNamesWithExtensions send";

    ThreadMessageQueue::getInstance()->addQueueCommand(new RetrieveFileNamesWithExtensionsTask(m_ID, extensions, oResult));
}

void CPrinterZebraImpl::printStoredFormatWithHash( const rho::String& formatPathOnPrinter,  const rho::Hashtable<rho::String, rho::String>& vars, rho::apiGenerator::CMethodResult& oResult) 
{
    LOG(TRACE) + "[zebra] printStoredFormatWithHash send";

    ThreadMessageQueue::getInstance()->addQueueCommand(new PrintStoredFormatWithHashTask(m_ID, formatPathOnPrinter, vars, oResult));
}

void CPrinterZebraImpl::printStoredFormatWithArray( const rho::String& formatPathOnPrinter,  const rho::Vector<rho::String>& vars, rho::apiGenerator::CMethodResult& oResult) 
{
    LOG(TRACE) + "[zebra] printStoredFormatWithArray send";

    ThreadMessageQueue::getInstance()->addQueueCommand(new PrintStoredFormatWithArrayTask(m_ID, formatPathOnPrinter, vars, oResult));
}

void CPrinterZebraImpl::storeImage( const rho::String& printerDriveAndFileName,  const rho::String& imageFilePathOnDevice,  int width,  int height, rho::apiGenerator::CMethodResult& oResult) 
{
    LOG(TRACE) + "[zebra] storeImage send";

    ThreadMessageQueue::getInstance()->addQueueCommand(new StoreImageTask(m_ID, printerDriveAndFileName, imageFilePathOnDevice, width, height, oResult));
}

void CPrinterZebraImpl::requestState( const rho::Vector<rho::String>& listOfParameters, rho::apiGenerator::CMethodResult& oResult) 
{
    LOG(TRACE) + "[zebra] requestState send";

    ThreadMessageQueue::getInstance()->addQueueCommand(new RequestStateTask(m_ID, listOfParameters, oResult));
}

//////////////////////////////////////////////////////////////////////////

class CPrinterZebraSingleton: public CPrinterZebraSingletonBase
{
public:
    virtual rho::String getInitialDefaultID()
    {
        CMethodResult oRes;
        enumerate(oRes);

        rho::Vector<rho::String>& arIDs = oRes.getStringArray();

        return ""; // arIDs[0];
    }

    virtual void searchPrinters( const rho::Hashtable<rho::String, rho::String>& options, rho::apiGenerator::CMethodResult& oResult) 
    {
        LOG(TRACE) + "[zebra] searchPrinters send";

        ThreadMessageQueue::getInstance()->addQueueCommand(new SearchPrintersTask<IPrinterZebra, CPrinterZebraImpl, CPrinterZebraBase>(options, oResult));
    }

    virtual void enumerateSupportedTypes(rho::apiGenerator::CMethodResult& oResult) 
    {
        LOG(TRACE) + "[zebra] enumerateSupportedTypes send";

        rho::Vector<rho::String> supportedTypes;
        supportedTypes.push_back(IPrinterZebra::PRINTER_TYPE_ZEBRA);
        oResult.set(supportedTypes);
    }

    virtual void stopSearch(rho::apiGenerator::CMethodResult& oResult) 
    {
        LOG(TRACE) + "[zebra] stopSearch send";

        ThreadMessageQueue::getInstance()->addQueueCommand(new StopSearchTask(oResult));
    }

    virtual void getPrinterByID( const rho::String& printerId, rho::apiGenerator::CMethodResult& oResult)
    {
        oResult.set(printerId);
    }

    virtual void enumerate(rho::apiGenerator::CMethodResult& oResult) 
    {
        LOG(TRACE) + "[zebra] enumerate send";

        rho::Vector<rho::String> ids;
        printing::PrinterManager<IPrinterZebra, CPrinterZebraImpl>::getInstance()->enumerate(ids);
        oResult.set(ids);
    }
};

class CPrinterZebraFactory: public CPrinterZebraFactoryBase
{
    ~CPrinterZebraFactory()
    {
        ThreadMessageQueue::getInstance()->deInit();
    }

    virtual IPrinterZebraSingleton* createModuleSingleton()
    {
        return new CPrinterZebraSingleton();
    }

    virtual IPrinterZebra* createModuleByID(const rho::String& strID)
    {
        return printing::PrinterManager<IPrinterZebra, CPrinterZebraImpl>::getInstance()->getPrinterByID(strID);
    }
};

/************************************************************************/

extern "C" void Init_PrintingZebra_extension()
{
    CPrinterZebraFactory::setInstance( new CPrinterZebraFactory() );
    Init_PrinterZebra_API();
}

}