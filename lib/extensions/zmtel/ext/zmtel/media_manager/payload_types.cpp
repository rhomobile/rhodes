#include "../stdafx.h"

#include "payload_types.h"

static sPayloadType g_aPayloadTypeList[] = {
  // Voice codecs
  { "G.711 u-law"       , SPIRIT_PAYLOAD_TYPE_PCMU            },
  { "G.711 A-law"       , SPIRIT_PAYLOAD_TYPE_PCMA            },
  { "G.722"             , SPIRIT_PAYLOAD_TYPE_G722            },
  { "G.723.1"           , SPIRIT_PAYLOAD_TYPE_G723            },
  { "G.729 AB"          , SPIRIT_PAYLOAD_TYPE_G729            },
  { "AMR-NB"            , SPIRIT_PAYLOAD_TYPE_AMRNB           },
  { "AMR-WB"            , SPIRIT_PAYLOAD_TYPE_AMRWB           },
  { "IP-MR"             , SPIRIT_PAYLOAD_TYPE_IPMR            },
  { "G.722.1"           , SPIRIT_PAYLOAD_TYPE_G722_1          },
  { "iLBC"              , SPIRIT_PAYLOAD_TYPE_ILBC            },

  // Video codecs
  { "Spirit H.261"      , SPIRIT_PAYLOAD_TYPE_H261            },
  { "Spirit H.263"      , SPIRIT_PAYLOAD_TYPE_H263            },
  { "Spirit H.263+"     , SPIRIT_PAYLOAD_TYPE_H263_1998       },
  { "Spirit H.264"      , SPIRIT_PAYLOAD_TYPE_H264            },
  { "Spirit MPEG4-SP"   , SPIRIT_PAYLOAD_TYPE_MPEG4_SP        },
  { "Intel H263"        , SPIRIT_PAYLOAD_TYPE_INTEL_H263      },
  { "Intel H263-1998"   , SPIRIT_PAYLOAD_TYPE_INTEL_H263_1998 },
  { "Intel H264"        , SPIRIT_PAYLOAD_TYPE_INTEL_H264      },

  { "H.263 (WMMX2)"     , SPIRIT_PAYLOAD_TYPE_INTEL_H263      },
  { "H.263-1998 (WMMX2)", SPIRIT_PAYLOAD_TYPE_INTEL_H263_1998 },
  { "H.264 (WMMX2)"     , SPIRIT_PAYLOAD_TYPE_INTEL_H264      },
};

static const size_t g_PayloadTypeCount = sizeof(g_aPayloadTypeList) / sizeof(sPayloadType);

uint8 GetPayloadTypeByName(const char *pFriendlyCodecName)
{
  if(!pFriendlyCodecName)
    return 255;
  for(size_t i = 0; i < g_PayloadTypeCount; i++)
    if(0 == _stricmp(g_aPayloadTypeList[i].aFriendlyCodecName, pFriendlyCodecName))
      return g_aPayloadTypeList[i].PayloadType;

  return 255;
}
