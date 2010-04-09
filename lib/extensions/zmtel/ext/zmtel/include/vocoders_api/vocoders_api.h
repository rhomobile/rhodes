#ifndef _SPIRIT_VOCODERS_API_H_
#define _SPIRIT_VOCODERS_API_H_

#include "common/spirit_comtypes.h"
/******************************************************************************
                               AUDIO Codecs
******************************************************************************/

#define MAX_AUDIO_CODEC_SETTINGS 10
// audio codec keys
typedef enum _teAudioCodecParamKey
{
  // codec MUST support them as READ even for NULL instance
  cpk_samplerate,             // should be set only for codecs that supports multiple sampling rates
  cpk_rtp_samplerate,         // rtp samplerate
  cpk_frame_size,
  cpk_bin_frame_size_max,
  // temporary parameters, READ access even for NULL instance
  cpk_def_payload,            // default rtp payload number, 255 for dynamic payload codecs
  cpk_sdp_name,               // sdp name

  // optional, it is not guaranteed codec supports this parameter
  cpk_bitrate,
  cpk_VAD,
  cpk_DTX,
  
  cpk_flags,
  cpk_mode,
  
  cpk_max_rtp_packet_size,    // max packet size
  
  cpk_user_key_start = 100,

} teAudioCodecParamKey;

// key value struct
typedef struct _tAudioCodecParam
{
  uint32 key;
  union
  {
    uint32 value;
    void*  p;
  } u;
} tAudioCodecParam;

//------------------------------------------------------------------------------
// voice codec functions

typedef tResult (*tVoiceCodec_Create)(void** pObject, size_t count, const tAudioCodecParam *pParam);
typedef tResult (*tVoiceCodec_Delete)(void* pObject);

typedef tResult (*tVoiceCodec_Encode)(void* pObject,
                                      const int16* pIn,
                                      size_t *pInSize,
                                      OUT void* pOut,
                                      OUT size_t* pOutSize);

typedef tResult (*tVoiceCodec_Decode)(void* pObject,
                                      const void* pInput,
                                      size_t InputSize,
                                      OUT int16* pOut,
                                      OUT size_t* pSize);

typedef tResult (*tVoiceCodec_Recover)(void* pObject,
                                       OUT int16* pOut,
                                       OUT size_t* pSize);

typedef tResult (*tVoiceCodec_SetParam)(void* pObject, size_t count, const tAudioCodecParam *pParam);
typedef tResult (*tVoiceCodec_GetParam)(void* pObject, tAudioCodecParam *pParam);

typedef struct tAudioCodecDescription
{
  const char* pFriendlyName;    // User-displayable codec name

  //virtual table:
  struct
  {
    tVoiceCodec_Create   fnCreate;
    tVoiceCodec_Delete   fnDelete;

    tVoiceCodec_SetParam fnSetParam; // OPTIONAL
    tVoiceCodec_GetParam fnGetParam; // OPTIONAL

    tVoiceCodec_Decode   fnDecode;
    tVoiceCodec_Recover  fnPLC;      // OPTIONAL if PLC is implemented 

  } decoder;

  struct
  {
    tVoiceCodec_Create   fnCreate;
    tVoiceCodec_Delete   fnDelete;

    tVoiceCodec_SetParam fnSetParam; // OPTIONAL
    tVoiceCodec_GetParam fnGetParam; // OPTIONAL

    tVoiceCodec_Encode   fnEncode;

  } encoder;

} tAudioCodecDescription;

// audio codec description and settings
typedef struct _tAudioCodecDescSettings
{
  const tAudioCodecDescription * pVoiceCodec;
  size_t count;
  tAudioCodecParam pParam[MAX_AUDIO_CODEC_SETTINGS]; 
} tAudioCodecDescSettings;

#endif // _SPIRIT_VOCODERS_API_H_
