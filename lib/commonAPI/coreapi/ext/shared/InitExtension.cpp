extern "C" void Init_System();
extern "C" void Init_NetworkAccess();

extern "C" void Init_CoreAPI_Extension()
{
    Init_System();
    Init_NetworkAccess();
}