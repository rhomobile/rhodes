#ifndef __CHANNEL__H__
#define __CHANNEL__H__

#include "spirit_headers.h"

#define SPIRIT_MAX_CODEC_MAP_SIZE   32
#define INVALID_CHANNEL             (-1)

class CChannel
{
protected:
  tMMHandle   m_hMM;
  uint32      m_Id;

  tIpAddr     m_LocalRtpAddr;
  tIpAddr     m_LocalRtcpAddr;

  tIpAddr     m_RemoteRtpAddr;
  tIpAddr     m_RemoteRtcpAddr;

  tCodecPayload     m_aInRtpMap[SPIRIT_MAX_CODEC_MAP_SIZE];
  tCodecPayload     m_aOutRtpMap[SPIRIT_MAX_CODEC_MAP_SIZE];

  uint32      m_bAutoFecEnable;

public:
  CChannel(void);
  ~CChannel(void);

  bool Init(tMMHandle hMM);
  bool Deinit();

  bool SetLocalIp(const char* pIp);
  bool SetLocalRtpPort(unsigned short Port);
  bool SetLocalRtcpPort(unsigned short Port);

  bool SetRemoteIp(const char* pIp);
  bool SetRemoteRtpPort(unsigned short Port);
  bool SetRemoteRtcpPort(unsigned short Port);

  uint32 Id();

  bool SetAutoFecEnable(uint32 val);
};

#endif // __CHANNEL__H__
