#import <Foundation/Foundation.h>
#import "PrinterZebraTech.h"
#include "common/app_build_capabilities.h"

extern void Init_PrinterZebra_API();

void Init_PrintingZebra_extension()
{
    Init_PrinterZebra_API();
    // create tech singletone and register it inside PrinterTechManagerSingleton
    [PrinterZebraTechSingleton getPrinterZebraWorkerInstance];
}
