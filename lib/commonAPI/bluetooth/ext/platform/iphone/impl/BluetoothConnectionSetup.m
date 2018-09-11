#import <Foundation/Foundation.h>
#include "common/app_build_capabilities.h"

extern void Init_BluetoothConnection_API();

void Init_Bluetooth_extension()
{
    Init_BluetoothConnection_API();
}
