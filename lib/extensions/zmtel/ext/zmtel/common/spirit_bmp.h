#ifndef SPIRIT_BMP_H_INCLUDED
#define SPIRIT_BMP_H_INCLUDED

#include "common/spirit_comtypes.h"
#include "media_manager_api/spirit_media_manager_types.h"

///////////////////////////////////////////////////////////////////////////////
//Loads bitmap from the specified file
//Note: Function allocates video bufer.
//This buffer should be freed by SpiritBitmap_FreeFrame function
//NOTE this function loads RGB32 bitmaps only
SPIRIT_EXTERN_C tBool SpiritBitmap_LoadFromFile(tSpiritVideoFrame *pFrame,const char *filename);

//
//frees video buffer allocated by the SpiritBitmap_LoadFromFile function
SPIRIT_EXTERN_C void SpiritBitmap_FreeFrame(tSpiritVideoFrame *pFrame);

#endif
