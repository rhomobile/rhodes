#pragma once

#include <common/RhoStd.h>
#include "../../../shared/generated/cpp/IPrinterZebra.h"

namespace rho
{
namespace printing
{

template<typename PrinterType, typename PrinterImplType>
class PrinterManager
{
public:
    typedef rho::String PrinterID;

private:
    static PrinterManager<PrinterType, PrinterImplType>* g_managerInstance;

    std::map<PrinterID, PrinterType*> m_priterCache;

private:
    PrinterManager() {}

public:

    PrinterType* addPrinterWithID(const PrinterID& printerID)
    {
        PrinterType* printer = getPrinterByID(printerID);

        if (!printer)
        {
            PrinterImplType* newPrinter = new PrinterImplType();
            newPrinter->setID(printerID);
            m_priterCache[printerID] = newPrinter;
            return newPrinter;
        }

        return printer;
    }

    PrinterType* getPrinterByID(const PrinterID& printerID)
    {
        try
        {
            return m_priterCache[printerID];
        }
        catch (std::out_of_range& e)
        {
            return 0;
        }
    }

    void enumerate(rho::Vector<rho::String>& printers)
    {
        std::map<PrinterID, PrinterType*>::const_iterator cit = m_priterCache.begin();
        std::map<PrinterID, PrinterType*>::const_iterator eit = m_priterCache.end();

        for (;cit != eit; ++cit)
        {
            printers.push_back(cit->first);
        }
    }

    static PrinterManager<PrinterType, PrinterImplType>* getInstance()
    {
        if (g_managerInstance == NULL)
        {
            g_managerInstance = new PrinterManager<PrinterType, PrinterImplType>();
        }

        return g_managerInstance;
    }
};

template<typename PrinterType, typename PrinterImplType>
PrinterManager<PrinterType, PrinterImplType>* PrinterManager<PrinterType, PrinterImplType>::g_managerInstance = 0;

}
}