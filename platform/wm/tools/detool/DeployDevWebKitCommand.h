#pragma once

#include "rapi_helper.h"

class DeployDevWebKitCommand : public EmuCommand
{
private:
    TCHAR *src_path;
    bool use_shared_runtime;

public:
    DeployDevWebKitCommand(int argc, _TCHAR* argv[]) : EmuCommand(argc, argv), use_shared_runtime(false) {}

    virtual bool checkParameters() const;
    virtual void parseParameters();
    virtual void runObject();
};
