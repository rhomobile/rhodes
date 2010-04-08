#ifndef SPIRIT_VIDEO_TYPES_H_INCLUDED
#define SPIRIT_VIDEO_TYPES_H_INCLUDED

#include "common/spirit_comtypes.h"
#include "media_manager_api/spirit_media_manager_types.h"

//Video engine uses this parameter to set Spirit Log to the underlying components
//val.pData should be a valid tSpiritLog* pointer or NULL
#define SVEP_SET_SPIRIT_LOG "ViE:SetLog"

typedef enum tagVideoFrameSize
{
  VGA              = 0,  // 640x480
  QVGA             = 1,  // 320x240
  QQVGA            = 2,  // 160x120
  QQQVGA           = 3,  // 80x60
  nVGA             = 4,  // 240x180
  CIF              = 5,  // 352x288
  QCIF             = 6,  // 176x144
  QQCIF            = 7,  // 88x72
  CUSTOM_SIZE      = 8,  //user defined - used by the video capture utilities in the some special cases 
  D1_NTSC          = 9,  //720x480
  D1_PAL           = 10, //720x576
  IMPORTEQ_304x224 = 11, //custom resolution for Importek Apollo development board, some kind of plug?!
  MAX_RESOLUTION_INDEX   //

} teVideoFrameSize;

#endif // SPIRIT_VIDEO_TYPES_H_INCLUDED
