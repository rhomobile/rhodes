#pragma once

#include "rapi_helper.h"

class DeployEmuWebKitCommand : public EmuCommand
{
private:
    TCHAR *emu_name;
    TCHAR *src_path;
    bool use_shared_runtime;

public:
    DeployEmuWebKitCommand(int argc, _TCHAR* argv[]) : EmuCommand(argc, argv) {}
    virtual bool checkParameters() const;
    virtual void parseParameters();
    virtual void runObject();
};
