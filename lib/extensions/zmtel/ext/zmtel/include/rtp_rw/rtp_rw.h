
#ifndef __RTP_RW__
#define __RTP_RW__

// mm and generic
#include "common/spirit_comtypes.h"
#include "logging/SpiritLogType.h"
#include "media_manager_api/spirit_media_manager_ext.h"

typedef void* tRTPPlayerHandle;
typedef void* tRTPRecorderHandle;

typedef tResult (*tpfnOnEof)(void* pModule);

#define MAX_CODEC_PARAM 10
#define MAX_CODEC_NAME_LEN 30

#pragma pack (push)
#pragma pack (1)
typedef struct _tRTPPayloadMap
{
  uint32 codecCount;                  // number of codecs
  struct _tCodec
  {
    char codecName[MAX_CODEC_NAME_LEN]; // friendly name of spirit's codec
    uint32 paramCount;                  // count of params if exists
    struct _tCodecParam           
    {
      uint32 key;
      uint32 value;
    } codecParam[MAX_CODEC_PARAM];
    uint8 payload;                      // rtp payload number
  } codecs[1];                          // 1 is fake, actually there are codecCount codecs
} tRTPPayloadMap;
#pragma pack (pop)

typedef struct _tRTPPlayerParams
{
  tMMHandle hMediaManager;
  struct _tRtpChannelParams
  {
    void * p_object;
    tpfnOnEof fnEof;
    const char * p_filename;
    tBool bUseCashedReader;
    union {
      const tVideoCodecDescription ** pcVideoCodecs;
      const tAudioCodecDescription ** pcAudioCodecs;
    } codectab;
  } audio, video;
} tRTPPlayerParams;

typedef struct _tRTPRecordParams
{
  const char * p_filename;
  tRTPPayloadMap * pCodecs;
} tRTPRecordParams;

// player
SPIRIT_EXTERN_C tResult RTPPlayer_Create(tRTPPlayerHandle *phPlay, const tRTPPlayerParams * pParams,uint32 *pVoiceChannelId,uint32 *pVideoChannelId);
SPIRIT_EXTERN_C tResult RTPPlayer_GetCodecs(tRTPPlayerHandle hPlay, tRTPPayloadMap ** ppCodecsA, tRTPPayloadMap ** ppCodecsV);
SPIRIT_EXTERN_C tResult RTPPlayer_Destroy(tRTPPlayerHandle hPlay); 
SPIRIT_EXTERN_C tResult RTPPlayer_Start(tRTPPlayerHandle hPlay);
SPIRIT_EXTERN_C tResult RTPPlayer_Stop(tRTPPlayerHandle hPlay);

// recorder
SPIRIT_EXTERN_C tResult RTPRecorder_Create(tRTPRecorderHandle *phRec, const tRTPRecordParams * pParams);
SPIRIT_EXTERN_C void    RTPRecorder_Destroy(tRTPRecorderHandle hRec);

SPIRIT_EXTERN_C tResult RTPRecorder_StartIncomingStreamRecording(tRTPRecorderHandle hRec,tMMHandle hModule,uint32 ChannelId);
SPIRIT_EXTERN_C tResult RTPRecorder_StopIncomingStreamRecording(tRTPRecorderHandle hRec,tMMHandle hModule,uint32 ChannelId);

SPIRIT_EXTERN_C tResult RTPRecorder_StartOutgoingStreamRecording(tRTPRecorderHandle hRec,tMMHandle hModule,uint32 ChannelId);
SPIRIT_EXTERN_C tResult RTPRecorder_StopOutgoingStreamRecording(tRTPRecorderHandle hRec,tMMHandle hModule,uint32 ChannelId);

#endif // __RTP_RW__

