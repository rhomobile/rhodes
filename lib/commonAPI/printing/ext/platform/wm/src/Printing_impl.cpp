#include "Printing_impl.h"

#include "../../../shared/generated/cpp/PrinterBase.h"

#include "../../../../../printing_zebra/ext/platform/wm/src/PrinterManager.h"
#include "../../../../../printing_zebra/ext/platform/wm/src/p2p/ThreadMessageQueue.h"
#include "../../../../../printing_zebra/ext/platform/wm/src/p2p/MethodWrappers.h"

namespace rho 
{

using namespace apiGenerator;
using namespace printing::ipc::wrappers;
using namespace printing::ipc::thread;

void CPrinterImpl::setID(const rho::String& ID) 
{ 
    m_ID = ID; 
}

void CPrinterImpl::getID(rho::apiGenerator::CMethodResult& oResult) 
{
    oResult.set(m_ID);
}

void CPrinterImpl::getDeviceName(rho::apiGenerator::CMethodResult& oResult) 
{
    LOG(TRACE) + "[generic] getDeviceName send";

    ThreadMessageQueue::getInstance()->addQueueCommand(new MessageOnlyResult(m_ID, "getDeviceName", oResult));
}

void CPrinterImpl::getIsConnected(rho::apiGenerator::CMethodResult& oResult) 
{
    LOG(TRACE) + "[generic] getIsConnected send";

    IsConnectedTask *task = new IsConnectedTask(m_ID, oResult);
    ThreadMessageQueue::getInstance()->processCommand(task);

    rho::apiGenerator::CMethodResult dumbResult;
    setProperty(printerzebra::PROPERTY_IS_CONNECTED, task->isConnected() ? "true" : "false", dumbResult);

    oResult.set(task->isConnected());
}

void CPrinterImpl::connect(rho::apiGenerator::CMethodResult& oResult) 
{
    LOG(TRACE) + "[generic] connect send";

    if (oResult.hasCallback())
    {
        ThreadMessageQueue::getInstance()->addQueueCommand(new ConnectPrinterTask(m_ID, oResult));
    }
    else
    {
        ConnectPrinterTask *task = new ConnectPrinterTask(m_ID, oResult);
        ThreadMessageQueue::getInstance()->processCommand(task);
        rho::String str = task->getResult().getString();
        oResult.set(str);
        delete task;
    }
}

void CPrinterImpl::connectWithOptions( const rho::Hashtable<rho::String, rho::String>& options, rho::apiGenerator::CMethodResult& oResult) 
{
    LOG(TRACE) + "[generic] connectWithOptions send";

    rho::apiGenerator::CMethodResult propResult;

    rho::Hashtable<rho::String, rho::String> fullOptions = options;

    if (oResult.hasCallback())
    {
        ThreadMessageQueue::getInstance()->addQueueCommand(new ConnectWithParametersPrinterTask(m_ID, fullOptions, oResult));
    }
    else
    {
        ConnectWithParametersPrinterTask *task = new ConnectWithParametersPrinterTask(m_ID, fullOptions, oResult);
        ThreadMessageQueue::getInstance()->processCommand(task);
        rho::String str = task->getResult().getString();
        oResult.set(str);
        delete task;
    }
}

void CPrinterImpl::disconnect(rho::apiGenerator::CMethodResult& oResult) 
{
    LOG(TRACE) + "[generic] disconnect send";

    ThreadMessageQueue::getInstance()->addQueueCommand(new DisconnectPrinterTask(m_ID, oResult));
}

void CPrinterImpl::printFile( const rho::String& path,  const rho::Hashtable<rho::String, rho::String>& options, rho::apiGenerator::CMethodResult& oResult) 
{
    LOG(TRACE) + "[generic] printFile send";

    ThreadMessageQueue::getInstance()->addQueueCommand(new PrintFileTask(m_ID, path, options, oResult));
}

void CPrinterImpl::printImageFromFile( const rho::String& path,  int x,  int y,  const rho::Hashtable<rho::String, rho::String>& options, rho::apiGenerator::CMethodResult& oResult) 
{
    LOG(TRACE) + "[generic] printImageFromFile send";

    ThreadMessageQueue::getInstance()->addQueueCommand(new PrintImageFromFileTask(m_ID, path, x, y, options, oResult));
}

void CPrinterImpl::printRawString( const rho::String& command,  const rho::Hashtable<rho::String, rho::String>& options, rho::apiGenerator::CMethodResult& oResult) 
{
    LOG(TRACE) + "[generic] printRawString send";

    ThreadMessageQueue::getInstance()->addQueueCommand(new PrintRawStringTask(m_ID, command, options, oResult));
}

void CPrinterImpl::enumerateSupportedControlLanguages(rho::apiGenerator::CMethodResult& oResult) 
{
    LOG(TRACE) + "[generic] enumerateSupportedControlLanguages send";

    if (oResult.hasCallback())
    {
        ThreadMessageQueue::getInstance()->addQueueCommand(new EnumerateSupportedControlLanguagesTask(m_ID, oResult));
    }
    else
    {
        EnumerateSupportedControlLanguagesTask *task = new EnumerateSupportedControlLanguagesTask(m_ID, oResult);
        ThreadMessageQueue::getInstance()->processCommand(task);
        oResult.set(task->getResult().getStringArray());
        delete task;
    }
}

void CPrinterImpl::requestState( const rho::Vector<rho::String>& listOfParameters, rho::apiGenerator::CMethodResult& oResult) 
{
    LOG(TRACE) + "[generic] requestState send";

    ThreadMessageQueue::getInstance()->addQueueCommand(new RequestStateTask(m_ID, listOfParameters, oResult));
}

//////////////////////////////////////////////////////////////////////////

class CPrinterSingleton: public CPrinterSingletonBase
{
    ~CPrinterSingleton(){}
    virtual rho::String getInitialDefaultID()
    {
        CMethodResult oRes;
        enumerate(oRes);

        rho::Vector<rho::String>& arIDs = oRes.getStringArray();

        return ""; //arIDs[0];
    }

    virtual void enumerate(CMethodResult& oResult)
    {
        LOG(TRACE) + "[generic] enumerate send";

        rho::Vector<rho::String> ids;
        printing::PrinterManager<IPrinter, CPrinterImpl>::getInstance()->enumerate(ids);
        oResult.set(ids);
    }

    virtual void enumerateSupportedTypes(rho::apiGenerator::CMethodResult& oResult) 
    {
        LOG(TRACE) + "[generic] enumerate send";

        rho::Vector<rho::String> supportedTypes;
        supportedTypes.push_back(IPrinter::PRINTER_TYPE_ZEBRA);
        oResult.set(supportedTypes);
    }

    virtual void searchPrinters( const rho::Hashtable<rho::String, rho::String>& options, rho::apiGenerator::CMethodResult& oResult) 
    {
        LOG(TRACE) + "[generic] searchPrinters send";

        ThreadMessageQueue::getInstance()->addQueueCommand(new SearchPrintersTask<IPrinter, CPrinterImpl, CPrinterBase>(options, oResult));
    }
    
    virtual void stopSearch(rho::apiGenerator::CMethodResult& oResult) 
    {
        LOG(TRACE) + "[generic] stopSearch send";

        ThreadMessageQueue::getInstance()->addQueueCommand(new StopSearchTask(oResult));
    }

    virtual void getPrinterByID( const rho::String& printerId, rho::apiGenerator::CMethodResult& oResult) 
    {
        oResult.set(printerId);
    }
};

//////////////////////////////////////////////////////////////////////////

class CPrinterFactory: public CPrinterFactoryBase
{
    ~CPrinterFactory(){}
    virtual IPrinterSingleton* createModuleSingleton()
    {
        return new CPrinterSingleton();
    }

    virtual IPrinter* createModuleByID(const rho::String& strID)
    {
        return printing::PrinterManager<IPrinter, CPrinterImpl>::getInstance()->getPrinterByID(strID);
    }
};

extern "C" void Init_Printing_extension()
{
    CPrinterFactory::setInstance( new CPrinterFactory() );
    Init_Printer_API();
}

}