/******************************************************************************
  FILE: spirit_media_manager_ext.h
  DESCRIPTION: N/A yet

                          This file is a part of Spirit Voice/Video Engine SDK.

  Copyright SPIRIT DSP 2008.                                 www.spiritdsp.com
******************************************************************************/
#ifndef _SPIRIT_MM_EXT_H_
#define _SPIRIT_MM_EXT_H_

#include "media_manager_api/spirit_media_manager.h"

//------------------------------------------------------------------------------
// module types

//-----------------------------------------------------------------------------
// stream recording

struct _tRTP_Packet; // forward declaration of struct _tRTP_Packet
struct _tRCTP_Packet; // forward declaration of struct _tRTCP_Packet
typedef tResult (*tpfnOnRtpPacket)(void *, const struct _tRTP_Packet* );
typedef tResult (*tpfnOnRtcpPacket)(void *, const struct _tRTCP_Packet* );

typedef struct _tPacketDump
{
  void*            pUserData;
  tpfnOnRtpPacket  onRtp;
  tpfnOnRtcpPacket onRtcp;

} tPacketDump;

SPIRIT_EXTERN_C tResult MM_Voice_SetOnIncomingPacketCb( tMMHandle hModule, uint32 ChannelId, const tPacketDump * params);
SPIRIT_EXTERN_C tResult MM_Video_SetOnIncomingPacketCb( tMMHandle hModule, uint32 ChannelId, const tPacketDump * params);
SPIRIT_EXTERN_C tResult MM_Voice_SetOnOutgoingPacketCb( tMMHandle hModule, uint32 ChannelId, const tPacketDump * params);
SPIRIT_EXTERN_C tResult MM_Video_SetOnOutgoingPacketCb( tMMHandle hModule, uint32 ChannelId, const tPacketDump * params);

//-----------------------------------------------------------------------------
// stream recording

typedef tResult (*tpfnOnRawPacket)(void *, const void*, size_t );

typedef struct _tRawPacketDump
{
  void*           pUserData;
  tpfnOnRawPacket onRtp;
  tpfnOnRawPacket onRtcp;

} tRawPacketDump;

SPIRIT_EXTERN_C tResult MM_Voice_SetOnIncomingRawPacketCb( tMMHandle hModule, uint32 ChannelId, const tRawPacketDump * params);
SPIRIT_EXTERN_C tResult MM_Video_SetOnIncomingRawPacketCb( tMMHandle hModule, uint32 ChannelId, const tRawPacketDump * params);
SPIRIT_EXTERN_C tResult MM_Voice_SetOnOutgoingRawPacketCb( tMMHandle hModule, uint32 ChannelId, const tRawPacketDump * params);
SPIRIT_EXTERN_C tResult MM_Video_SetOnOutgoingRawPacketCb( tMMHandle hModule, uint32 ChannelId, const tRawPacketDump * params);

SPIRIT_EXTERN_C tResult MM_Channel_SetOnRawPacketCb(  tMMHandle hModule, uint32 ChannelId, const tRawPacketDump * params, tBool bIncoming);
//-----------------------------------------------------------------------------

//
//External transport helpers for RTP rw
SPIRIT_EXTERN_C tResult MM_Channel_PushRtpPacket( tMMHandle hModule, uint32 ChannelId, const void* pPacket, size_t size );
SPIRIT_EXTERN_C tResult MM_Channel_PushRtcpPacket( tMMHandle hModule, uint32 ChannelId, const void* pPacket, size_t size );

#endif // #endif // _SPIRIT_MM_EXT_H_
