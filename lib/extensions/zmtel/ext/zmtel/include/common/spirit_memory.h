/******************************************************************************
  FILE: spirit_memory.h
  DESCRIPTION: N/A yet

                          This file is a part of Spirit Embedded Media Engine.

  Copyright SPIRIT DSP 2006.                                 www.spiritdsp.com
******************************************************************************/
#ifndef _SPIRIT_MEMORY_H_
#define _SPIRIT_MEMORY_H_

#include <stddef.h>  // for size_t
#include "common/spirit_comtypes.h"

EXTERN_C void* SpiritMalloc(size_t Size);
EXTERN_C void  SpiritFree(void* pMem);

#endif // _SPIRIT_MEMORY_H_
