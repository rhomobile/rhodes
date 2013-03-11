#include "common/RhoPort.h"
extern "C" void Init_System();
extern "C" void Init_Network();
extern "C" void Init_SQLite3();

extern "C" void Init_CoreAPI_Extension()
{
    Init_System();
	Init_Network();
    Init_SQLite3();
}
