/******************************************************************************
  FILE: spirit_media_manager.h
  DESCRIPTION: N/A yet

                          This file is a part of Spirit Voice/Video Engine SDK.

  Copyright SPIRIT DSP 2008.                                 www.spiritdsp.com
******************************************************************************/
#ifndef _SPIRIT_MM_H_
#define _SPIRIT_MM_H_

#include "common/spirit_comtypes.h"
#include "media_manager_api/spirit_media_manager_types.h"
#include "media_manager_api/spirit_media_manager_keys.h"
#include "vocoders_api/vocoders_api.h"
#include "video_codec_api/video_codec_api.h"
#include "logging/SpiritLog.h"

//------------------------------------------------------------------------------
// module types

// declare handles
typedef void* tMMHandle;

typedef enum
{
  tr_internal,
  tr_external

} tTransportType;

typedef struct _tag_RTCP_Packet_APP
{
  uint8  Subtype;
  uint8  Name[4];
  uint32 AppDataSize;
  uint8* pAppData;

} tRTCP_Packet_APP;

typedef tResult ( *tpFnOnRecvPacketRtcpApp )( void* pHost, const tRTCP_Packet_APP* pPacket );
typedef void ( *tpFnOnRecvPacketRtcpFir )( void* pHost );

typedef  struct _tIpAddr
{
  union
  {
    uint8   i8[4];
    uint32  i32;

  } ip;            // IP address

  uint16  port;

} tIpAddr;

// RTCP Statistics
typedef struct _tRtcpStatistics
{
  struct
  {
    uint32  Age;         // describes, how old is the returned statistics, in ms
    uint32  SSRC;
    uint32  NTP_TS_HI;
    uint32  NTP_TS_LO;
    uint32  RTP_TS;
    uint32  PacketCount;
    uint32  OctetCount;

  } SenderReport;

  struct
  {
    uint32  Age;         // describes, how old is the returned statistics, in ms
    uint32  SSRC;        // our SSRC ID
    uint32  PacketsLost; // cumulative number of packets lost
    uint32  Sequence;    // extended highest sequence number received
    uint32  Jitter;      // interval jitter
    uint32  LSR;         // last SR
    uint32  DLSR;        // delay since last SR
    uint32  RoundTrip;   // SR round trip
    uint8   Loss;        // fraction lost

  } ReceiverReport;

  struct
  {
    uint32  Age;            // describes, how old is the returned statistics, in ms
    char    szCNAME[256];

  } SourceDescription;

} tRtcpStatistics;

typedef tResult ( *tpFnSendPacket )( void* pHost, uint32 nChannelId, const void* pPacket, size_t size );

typedef struct
{
  tTransportType transport_type;

  union
  {
    struct
    {
      void*          pHost;
      tpFnSendPacket pfnSendRtp;
      tpFnSendPacket pfnSendRtcp;

    } extrnl;

    // NOTE: network addresses and ports should be specified with host byte order
    struct
    {
      tIpAddr RtpSrc;
      tIpAddr RtcpSrc;
    } intrnl;

  } transport;

} tCallParam;

//-----------------------------------------------------------------------------
// Codec Description Functions

typedef union _tCodecDescription
{
  tAudioCodecDescSettings audio;
  tVideoCodecDescSettings video;

} tCodecDescription;

typedef struct _tCodecPayload
{
  tCodecDescription codec;
  uint8             Payload;

} tCodecPayload;

//-----------------------------------------------------------------------------
// Media Manager Control Functions

SPIRIT_EXTERN_C tResult MM_Create( tMMHandle* phModule, tSpiritLog* pLog );
SPIRIT_EXTERN_C tResult MM_Destroy( tMMHandle hModule );
SPIRIT_EXTERN_C tResult MM_SetLog( void* pModule, tSpiritLog* pLog );

//-----------------------------------------------------------------------------
// Voice/Video Channel Management Functions
SPIRIT_EXTERN_C tResult MM_Voice_Channel_Create( tMMHandle hModule, uint32* pChannelId, const tCallParam* pParams );
SPIRIT_EXTERN_C tResult MM_Voice_Channel_StartRx( tMMHandle hModule, uint32 ChannelId);
SPIRIT_EXTERN_C tResult MM_Voice_Channel_StopRx( tMMHandle hModule, uint32 ChannelId);
SPIRIT_EXTERN_C tResult MM_Voice_Channel_StartTx( tMMHandle hModule, uint32 ChannelId);
SPIRIT_EXTERN_C tResult MM_Voice_Channel_StopTx( tMMHandle hModule, uint32 ChannelId);
SPIRIT_EXTERN_C tResult MM_Voice_Channel_Destroy( tMMHandle hModule, uint32 ChannelId);

SPIRIT_EXTERN_C tResult MM_Voice_Channel_SetDestAddr( tMMHandle hModule, uint32 ChannelId, const tIpAddr* pRtpDest, const tIpAddr* pRtcpDest );
SPIRIT_EXTERN_C tResult MM_Voice_Channel_GetDestAddr( tMMHandle hModule, uint32 ChannelId, tIpAddr *pRtpDest, tIpAddr *pRtcpDest );

SPIRIT_EXTERN_C tResult MM_Video_Channel_Create( tMMHandle hModule, uint32* pChannelId, const tCallParam* pParams );
SPIRIT_EXTERN_C tResult MM_Video_Channel_StartRx( tMMHandle hModule, uint32 ChannelId);
SPIRIT_EXTERN_C tResult MM_Video_Channel_StopRx( tMMHandle hModule, uint32 ChannelId);
SPIRIT_EXTERN_C tResult MM_Video_Channel_StartTx( tMMHandle hModule, uint32 ChannelId);
SPIRIT_EXTERN_C tResult MM_Video_Channel_StopTx( tMMHandle hModule, uint32 ChannelId);
SPIRIT_EXTERN_C tResult MM_Video_Channel_Destroy( tMMHandle hModule, uint32 ChannelId);

SPIRIT_EXTERN_C tResult MM_Video_Channel_SetDestAddr( tMMHandle hModule, uint32 ChannelId, const tIpAddr* pRtpDest, const tIpAddr* pRtcpDest );
SPIRIT_EXTERN_C tResult MM_Video_Channel_GetDestAddr( tMMHandle hModule, uint32 ChannelId, tIpAddr *pRtpDest, tIpAddr *pRtcpDest );

SPIRIT_EXTERN_C tResult MM_Video_Channel_Sync( tMMHandle hModule, uint32 VideoChannelId, uint32 AudioChannelId );

//-----------------------------------------------------------------------------
// RTP/RTCP functions

SPIRIT_EXTERN_C tResult MM_Voice_GetRtcpStat( tMMHandle hModule, uint32 ChannelId, tRtcpStatistics* pInfo );
SPIRIT_EXTERN_C tResult MM_Video_GetRtcpStat( tMMHandle hModule, uint32 ChannelId, tRtcpStatistics* pInfo );

SPIRIT_EXTERN_C tResult MM_Voice_SetRtpPayloadMapping( tMMHandle hModule, uint32 ChannelId, const tCodecPayload* pInRtpMap, const tCodecPayload* pOutRtpMap );
SPIRIT_EXTERN_C tResult MM_Video_SetRtpPayloadMapping( tMMHandle hModule, uint32 ChannelId, const tCodecPayload* pInRtpMap, const tCodecPayload* pOutRtpMap );

//-----------------------------------------------------------------------------
// Encoder/Decoder Management Functions

// TODO: switch to get/set param by Key?

SPIRIT_EXTERN_C tResult MM_Voice_SetEncoder( tMMHandle hModule, uint32 ChannelId, const tAudioCodecDescSettings* pDescr );
SPIRIT_EXTERN_C tResult MM_Voice_GetEncoder( tMMHandle hModule, uint32 ChannelId, tAudioCodecDescSettings* pDescr );
SPIRIT_EXTERN_C tResult MM_Voice_GetDecoder( tMMHandle hModule, uint32 ChannelId, tAudioCodecDescSettings* pDescr );

SPIRIT_EXTERN_C tResult MM_Video_SetEncoder( tMMHandle hModule, uint32 ChannelId, const tVideoCodecDescSettings* pDescr);
SPIRIT_EXTERN_C tResult MM_Video_GetEncoder( tMMHandle hModule, uint32 ChannelId, tVideoCodecDescSettings* pDescr );
SPIRIT_EXTERN_C tResult MM_Video_GetDecoder( tMMHandle hModule, uint32 ChannelId, tVideoCodecDescSettings* pDescr );

SPIRIT_EXTERN_C tResult MM_Video_EncoderForceRefFrame( tMMHandle hModule, uint32 ChannelId );

//-----------------------------------------------------------------------------
// Send/Receive RTCP APP packets
SPIRIT_EXTERN_C tResult MM_Voice_SendRtcpAPP( tMMHandle hModule, uint32 ChannelId, const tRTCP_Packet_APP* pPacket );
SPIRIT_EXTERN_C tResult MM_Video_SendRtcpAPP( tMMHandle hModule, uint32 ChannelId, const tRTCP_Packet_APP* pPacket );

SPIRIT_EXTERN_C tResult MM_Voice_SetRtcpAppPacketRecvCallback( tMMHandle hModule, uint32 ChannelId, void* pHost, tpFnOnRecvPacketRtcpApp fxn );
SPIRIT_EXTERN_C tResult MM_Video_SetRtcpAppPacketRecvCallback( tMMHandle hModule, uint32 ChannelId, void* pHost, tpFnOnRecvPacketRtcpApp fxn );

//-----------------------------------------------------------------------------
// Send/Receive RTCP FIR packets
SPIRIT_EXTERN_C tResult MM_Video_SendRtcpFIR( tMMHandle hModule, uint32 ChannelId );
SPIRIT_EXTERN_C tResult MM_Video_SetRtcpFirPacketRecvCallback( tMMHandle hModule, uint32 ChannelId, void* pHost, tpFnOnRecvPacketRtcpFir fxn );

//-----------------------------------------------------------------------------
// External Transport
SPIRIT_EXTERN_C tResult MM_Voice_PushRtpPacket( tMMHandle hModule, uint32 ChannelId, const void* pPacket, size_t size );
SPIRIT_EXTERN_C tResult MM_Voice_PushRtcpPacket( tMMHandle hModule, uint32 ChannelId, const void* pPacket, size_t size );

SPIRIT_EXTERN_C tResult MM_Video_PushRtpPacket( tMMHandle hModule, uint32 ChannelId, const void* pPacket, size_t size );
SPIRIT_EXTERN_C tResult MM_Video_PushRtcpPacket( tMMHandle hModule, uint32 ChannelId, const void* pPacket, size_t size );

struct _tRTP_Packet; // forward declaration of struct _tRTP_Packet
SPIRIT_EXTERN_C tResult MM_Voice_PushRtpPacketEx(tMMHandle hModule, uint32 ChannelId, const struct _tRTP_Packet* pPacket);
SPIRIT_EXTERN_C tResult MM_Video_PushRtpPacketEx(tMMHandle hModule, uint32 ChannelId, const struct _tRTP_Packet* pPacket);

SPIRIT_EXTERN_C tResult MM_Voice_GetRtpSocketHandle( tMMHandle hModule, uint32 ChannelId, int* socket );
SPIRIT_EXTERN_C tResult MM_Voice_GetRtcpSocketHandle( tMMHandle hModule, uint32 ChannelId, int* socket );

SPIRIT_EXTERN_C tResult MM_Video_GetRtpSocketHandle( tMMHandle hModule, uint32 ChannelId, int* socket );
SPIRIT_EXTERN_C tResult MM_Video_GetRtcpSocketHandle( tMMHandle hModule, uint32 ChannelId, int* socket );

//-----------------------------------------------------------------------------
// Sound Playback Functions

typedef void* tPlaySoundHandle;

typedef enum _teStreamType
{
  ts_stm_callback      = 1,

  // stream format mask
  ts_stm_type          = 0x0000ffff,
  ts_stm_format        = 0xffff0000,

  // PCM
  ts_stf_pcm           = 0x00000000,
  ts_stm_pcm_callback  = ts_stf_pcm | ts_stm_callback,

  // Spirit proprietary binary format:
  ts_stf_binary        = 0x00010000,
  ts_stm_bin_callback  = ts_stf_binary | ts_stm_callback,

} teStreamType;

#define TS_CALLBACK
typedef uint32 (TS_CALLBACK *tpFnStream_GetData)(void *pInst, void *pData, uint32 Size);
typedef void (TS_CALLBACK *tpFnStream_Release)(void *pInst);

typedef enum _tePlaybackDest
{
  ts_play_local = 0,
  ts_play_remote
} tePlaybackDest;

typedef struct _tPlayStream
{
  teStreamType   Type;

  struct _tCallback
  {
    tpFnStream_GetData pGetData;
    tpFnStream_Release pRelease;
    void*              pHost;

  } Callback;

  tePlaybackDest      nPlaybackDest;
  eSpiritSoundIOFreqs nSampleRate;
  uint32              nChannelId;    // (uint32)-1 means global override

} tPlayStream;

SPIRIT_EXTERN_C tResult MM_Voice_PlaySound_Start(tMMHandle  hModule, tPlaySoundHandle *pPlayHandle, const tPlayStream *pStream, tBool bRingingDevice);
SPIRIT_EXTERN_C tResult MM_Voice_PlaySound_IsPlaying(tMMHandle  hModule, tPlaySoundHandle PlayHandle, tBool *pIsPlaying);
SPIRIT_EXTERN_C tResult MM_Voice_PlaySound_Stop(tMMHandle  hModule, tPlaySoundHandle PlayHandle);


//-----------------------------------------------------------------------------
// DTMF Functions

SPIRIT_EXTERN_C tResult MM_SendDTMF(tMMHandle hModule, uint32 ChannelId, const char* pszDTMF);

typedef void ( *tpfnOnIncomingDtmfSymbol )( /*in*/ void* pUserContext, /*in*/ unsigned int uDtmfSymbol );

SPIRIT_EXTERN_C tResult MM_SetOnIncomingDtmfCallback( tMMHandle hModule, uint32 ChannelId, void* pUserContext, tpfnOnIncomingDtmfSymbol pfnCallback );

//-----------------------------------------------------------------------------
// Configuration Functions

SPIRIT_EXTERN_C tResult MM_SetChannelParam( tMMHandle hModule, uint32 ChannelId, const char* pszKey, tVariant Value );
SPIRIT_EXTERN_C tResult MM_GetChannelParam( tMMHandle hModule, uint32 ChannelId, const char* pszKey, tVariant* pValue );

SPIRIT_INLINE tResult MM_SetParam( tMMHandle hModule, const char* pszKey, tVariant Value )
{ return MM_SetChannelParam( hModule, -1, pszKey, Value ); }
SPIRIT_INLINE tResult MM_GetParam( tMMHandle hModule, const char* pszKey, tVariant* pValue )
{ return MM_GetChannelParam( hModule, -1, pszKey, pValue ); }

//-----------------------------------------------------------------------------
// Device Enumeration

typedef struct _tag_device_list
{
  size_t uCount;       // number of devices in 'list'
  size_t iSelected;    // zero-based index of device which will be used
  tChar* list[];       // list of found devices

} tDeviceList;

SPIRIT_EXTERN_C tResult MM_Voice_GetRenderDeviceList( tMMHandle hModule, /*out*/ tDeviceList* pList );
SPIRIT_EXTERN_C tResult MM_Voice_GetCaptureDeviceList( tMMHandle hModule, /*out*/ tDeviceList* pList );
SPIRIT_EXTERN_C tResult MM_Voice_GetRenderDevice( tMMHandle hModule, /*out*/ tChar* pBuf, /*in,out*/ size_t* pLen );
SPIRIT_EXTERN_C tResult MM_Voice_GetCaptureDevice( tMMHandle hModule, /*out*/ tChar* pBuf, /*in,out*/ size_t* pLen );
SPIRIT_EXTERN_C tResult MM_Voice_SetRenderDevice( tMMHandle hModule, /*in*/ tChar* pBuf );
SPIRIT_EXTERN_C tResult MM_Voice_SetCaptureDevice( tMMHandle hModule, /*in*/ tChar* pBuf );

SPIRIT_EXTERN_C tResult MM_Video_GetRenderDeviceList( tMMHandle hModule, /*out*/ tDeviceList* pList );
SPIRIT_EXTERN_C tResult MM_Video_GetCaptureDeviceList( tMMHandle hModule, /*out*/ tDeviceList* pList );
SPIRIT_EXTERN_C tResult MM_Video_GetRenderDevice( tMMHandle hModule, /*out*/ tChar* pBuf, /*in,out*/ size_t* pLen );
SPIRIT_EXTERN_C tResult MM_Video_GetCaptureDevice( tMMHandle hModule, /*out*/ tChar* pBuf, /*in,out*/ size_t* pLen );
SPIRIT_EXTERN_C tResult MM_Video_SetRenderDevice( tMMHandle hModule, /*in*/ tChar* pBuf );
SPIRIT_EXTERN_C tResult MM_Video_SetCaptureDevice( tMMHandle hModule, /*in*/ tChar* pBuf );

SPIRIT_EXTERN_C void MM_Free_DeviceList( tMMHandle hModule, /*in*/ tDeviceList* pList );

typedef void TS_CALLBACK ( *TS_OnDeviceListChangeFunc )( void* pCtx );

SPIRIT_EXTERN_C tResult MM_Voice_SetDeviceListChangeCallback( tMMHandle hModule, TS_OnDeviceListChangeFunc Callback, void* pCtx );

//-----------------------------------------------------------------------------
// audio control functions

SPIRIT_EXTERN_C tResult MM_Voice_GetMicMute( tMMHandle hModule, tBool* pMute );
SPIRIT_EXTERN_C tResult MM_Voice_SetMicMute( tMMHandle hModule, tBool Mute );
SPIRIT_EXTERN_C tResult MM_Voice_GetSpeakerMute( tMMHandle hModule, tBool* pMute );
SPIRIT_EXTERN_C tResult MM_Voice_SetSpeakerMute( tMMHandle hModule, tBool Mute );

SPIRIT_EXTERN_C tResult MM_Voice_GetMicGain( tMMHandle hModule, uint16* pGain );
SPIRIT_EXTERN_C tResult MM_Voice_SetMicGain( tMMHandle hModule, uint16 Gain );
SPIRIT_EXTERN_C tResult MM_Voice_GetSpeakerGain( tMMHandle hModule, uint16* pGain );
SPIRIT_EXTERN_C tResult MM_Voice_SetSpeakerGain( tMMHandle hModule, uint16 Gain );

//-----------------------------------------------------------------------------
// voice/video codec enum functions

SPIRIT_EXTERN_C const tAudioCodecDescription** MM_Voice_GetCodecList(  );
SPIRIT_EXTERN_C const tAudioCodecDescription*  MM_Voice_GetCodec( const char* szCodec );

SPIRIT_EXTERN_C const tVideoCodecDescription** MM_Video_GetCodecList(  );
SPIRIT_EXTERN_C const tVideoCodecDescription*  MM_Video_GetCodec( const char* szCodec );

//-----------------------------------------------------------------------------

#endif // _SPIRIT_MM_H_
