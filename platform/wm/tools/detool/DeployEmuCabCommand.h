#pragma once

#include "rapi_helper.h"

class DeployEmuCabCommand : public EmuCommand
{
private:
    TCHAR *emu_name;
    TCHAR *cab_file;
    TCHAR *app_name;
    TCHAR *log_port;
    TCHAR *lcdll_path;
    bool use_shared_runtime;

public:
    DeployEmuCabCommand(int argc, _TCHAR* argv[]) : EmuCommand(argc, argv), use_shared_runtime(false) {}
    virtual bool checkParameters() const;
    virtual void parseParameters();
    virtual void runObject();
};
