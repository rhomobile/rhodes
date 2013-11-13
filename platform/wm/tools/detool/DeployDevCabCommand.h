#pragma once

#include "rapi_helper.h"

class DeployDevCabCommand : public EmuCommand
{
private:
    TCHAR *cab_file;
    bool use_shared_runtime;

public:
    DeployDevCabCommand(int argc, _TCHAR* argv[]) : EmuCommand(argc, argv), use_shared_runtime(false) {}
    virtual bool checkParameters() const;
    virtual void parseParameters();
    virtual void runObject();
};
