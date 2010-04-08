/******************************************************************************

FILE: SpiritLog.h

DESCRIPTION: In this file the interface of work with a broad gully is described.



This file is a part of Spirit Embedded Media Engine.



Copyright SPIRIT DSP 2006.                                 www.spiritdsp.com

******************************************************************************/

#ifndef _SPIRIT_LOG_H_
#define _SPIRIT_LOG_H_

//#include <stdlib.h>
#include "common/spirit_comtypes.h"
#include "logging/SpiritLogType.h"

// Functions working with SpiritLog

//----------------------Create log--------------------------------------
//  Parameters:
//      (out) ppLog - pointer of log
//      (in) pFileName - name of a file for preservation
//      (in) LogType - type of log, can be
//                          eSPIRIT_LOG_FAST – log data is stored in the memory buffer and
//                              written to the file storage after completion of realtime
//                              processing. Spirit recommends to use this mode always.
//
//                          eSPIRIT_LOG_CACHED, eSPIRIT_LOG_SYNC – both of these modes
//                              perform direct write to the file. These modes are intended
//                              for debugging of engine internals. Please avoid using of these modes.
//
//      (in) LogMaxSize - maximum size of log,
//                          if LogType equal eSPIRIT_LOG_FAST this is the size in memory,
//                          if LogType equal eSPIRIT_LOG_CACHED this parameter ignore
//                          if LogType equal eSPIRIT_LOG_SYNC this parameter ignore
//  Return:
//          SPIRIT_LOG_SUCCESS if success unitialization,
//          differently returned SPIRIT_LOG_FAIL, SPIRIT_LOG_LACK_OF_MEMORY
//          or SPIRIT_LOG_INVALID_PARAMETERS
SPIRIT_EXTERN_C uint32 SpiritLogCreate(tSpiritLog **ppLog, const char *pFileName, tSpiritLogType LogType, size_t LogMaxSize);

//----------------------Create log--------------------------------------
//  Parameters:
//      (in) pLog - log
//  Return:
//          SPIRIT_LOG_SUCCESS if success unitialization,
//          differently returned SPIRIT_LOG_FAIL or SPIRIT_LOG_INVALID_PARAMETERS
SPIRIT_EXTERN_C uint32 SpiritLogDestroy(tSpiritLog *pLog);
//----------------------Create log--------------------------------------
//  Parameters:
//      (out) ppVersion - pointer to char. (version format xx.xx.xx.xx)
//  Return:
//          SPIRIT_LOG_SUCCESS if success unitialization,
//          differently returned SPIRIT_LOG_FAIL
SPIRIT_EXTERN_C uint32 SpiritLogGetVersion(const char** ppVersion);

#endif // _SPIRIT_LOG_H_
