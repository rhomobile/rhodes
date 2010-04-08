#ifndef _VIDEO_CODEC_API_H_INCLUDED_
#define _VIDEO_CODEC_API_H_INCLUDED_

#include "common/spirit_comtypes.h"

#define MAX_VIDEO_CODEC_SETTINGS 20

typedef enum
{
  // codec MUST support them as READ even for NULL instance
  cpk_video_samplerate,             // should be set only for codecs that supports multiple sampling rates
  cpk_video_rtp_samplerate,         // rtp samplerate
  cpk_video_frame_size,
  // temporary parameters, READ access even for NULL instance
  cpk_video_def_payload,            // default rtp payload number, 255 for dynamic payload codecs
  cpk_video_sdp_name,               // sdp name

  cpk_video_mode,

  // init param
  cpk_video_bitrate,
  cpk_video_frame_format,
  cpk_video_width,
  cpk_video_height,
  cpk_video_Stride1,
  cpk_video_Stride2,
  cpk_video_Stride3,
  cpk_video_frame_rate_num,
  cpk_video_frame_rate_denom,
  cpk_video_flags,
  cpk_video_codec_flags,
  cpk_video_max_rtp_frame_size,
  //
  cpk_video_codec_allocator,
  // debug or temp
  cpk_video_spirit_log,
  cpk_video_custom_string_param, // instead of GetParam(char*,tVariant*)
  //Run time parameters
  cpk_video_freeze_until_reference
} tCodecParamKey;

/* RTP callback function type */
typedef void (*tSpiritVideoEncoderSendCallback)(void*  pModule,
                                                const void*   PacketData,
                                                size_t        PacketSizeBytes,
                                                uint32 TimestampInMsec,
                                                uint32 PacketNumber,
                                                int           IsLastPacket );

//this flag is used both for encoder and decoder
//forces CodecFlags to overwrite default codec values
#define VCODEC_FLAG_FORCE_CODEC_FLAGS 0x1
#define SPIRIT_VIDEO_FLAG_FORCE_CODEC_FLAGS VCODEC_FLAG_FORCE_CODEC_FLAGS
#define VCODEC_FLAG_CODEC_PROFILE_DEFAULT 0x2
#define VCODEC_FLAG_CODEC_PROFILE_FAST    0x4
#define VCODEC_FLAG_CODEC_PROFILE_QUALITY 0x8

typedef struct
{
  uint32 key;
  union
  {
    uint32 value;
    void*  p;
  } u;
} tVideoCodecParam;


//Transport stack interface

typedef void (*tVideoEncoder_Send)(void* pHost,
                                   const void* pData,
                                   size_t Size,
                                   uint32 TimestampInMsec,
                                   uint32 PacketNumber,
                                   int IsLastPacket);

typedef struct
{
  void              *pHost;         //callback user parameter
  tVideoEncoder_Send fnSend;        //callback function beeing invoked after packet completion
  size_t             MaxPacketSize; //Max size of outgoing video RTP packet
} tVideoCodec_TransportParams;

typedef tResult (*tVideoEncoder_Create)(void** pObject, tVideoCodec_TransportParams *pTrParam,
                                      size_t count, const tVideoCodecParam *pParam);

typedef tResult (*tVideoDecoder_Create)(void** pObject, size_t count, const tVideoCodecParam *pParam);

typedef tResult (*tVideoCodec_Delete)(void* pObject);

typedef tResult (*tVideoCodec_Encode)(void* pObject,
                                      uint32 bForceIFrame,
                                      const void* pIn, size_t InSize,
                                      uint32 *pTimeStampMs,
                                      void* pOut, size_t* pOutSize);

typedef tResult (*tVideoCodec_Decode)(void* pObject,
                                      const uint8 *pSrc, size_t  SizeSrc,
                                      void *pDst, size_t *pSizeDst,
                                      size_t *pDecodedSrcBufSizeBytes,
                                      uint32 *TimeStampMs,tBool bMarkerBit);

typedef tResult (*tVideoCodec_SetParam)(void* pObject, size_t count, const tVideoCodecParam *pParam);
typedef tResult (*tVideoCodec_GetParam)(void* pObject, tVideoCodecParam *pParam);

typedef struct _tVideoCodecDescription
{
  const char* pFriendlyName;    // User-displayable codec name

  //virtual table:
  struct
  {
    tVideoDecoder_Create fnCreate;
    tVideoCodec_Delete   fnDelete;

    tVideoCodec_SetParam fnSetParam; // OPTIONAL
    tVideoCodec_GetParam fnGetParam; // OPTIONAL

    tVideoCodec_Decode   fnDecode;

  } decoder;

  struct
  {
    tVideoEncoder_Create fnCreate;
    tVideoCodec_Delete   fnDelete;

    tVideoCodec_SetParam fnSetParam; // OPTIONAL
    tVideoCodec_GetParam fnGetParam; // OPTIONAL

    tVideoCodec_Encode   fnEncode;

  } encoder;

} tVideoCodecDescription;

// video codec description and settings
typedef struct _tVideoCodecDescSettings
{
  const tVideoCodecDescription * pVideoCodec;
  size_t count;
  tVideoCodecParam pParam[MAX_VIDEO_CODEC_SETTINGS];
} tVideoCodecDescSettings;

#endif // _VIDEO_CODEC_API_H_INCLUDED_
