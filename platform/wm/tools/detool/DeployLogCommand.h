#pragma once

#include "rapi_helper.h"

class DeployLogCommand : public EmuCommand
{
private:
    TCHAR *log_port;
    TCHAR *log_file;

public:
    DeployLogCommand(int argc, _TCHAR* argv[]) : EmuCommand(argc, argv) {}
    virtual bool checkParameters() const;
    virtual void parseParameters();
    virtual void runObject();
};
