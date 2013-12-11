#include "rapi_helper.h"

class DeployEmuCommand : public EmuCommand
{
private:
    bool   exe_change;
    TCHAR *emu_name;
    TCHAR *bundle_path;
    TCHAR *app_exe;
    TCHAR *log_port;
    TCHAR *lcdll_path;
    bool use_shared_runtime;

public:
    DeployEmuCommand(int argc, _TCHAR* argv[]) : EmuCommand(argc, argv), use_shared_runtime(false) {}
    virtual bool checkParameters() const;
    virtual void parseParameters();
    virtual void runObject();
};