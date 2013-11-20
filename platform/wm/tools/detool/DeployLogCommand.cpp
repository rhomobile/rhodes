#include "StdAfx.h"
#include "DeployLogCommand.h"

extern void startLogServer(TCHAR * log_file, TCHAR* log_port);

bool DeployLogCommand::checkParameters() const
{
    USES_CONVERSION;
    return (m_vecArgv.size() == 5) && (strcmp(T2A(m_vecArgv[2]), "log") == 0);
}

void DeployLogCommand::parseParameters()
{
    log_file = m_vecArgv[3];
    log_port = m_vecArgv[4];
}

void DeployLogCommand::runObject()
{
	if (log_file != NULL) 
    {
		startLogServer(log_file, log_port);
	}
}