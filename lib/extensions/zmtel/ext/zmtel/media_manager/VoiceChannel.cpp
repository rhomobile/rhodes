#include "../stdafx.h"
#include "VoiceChannel.h"
#include "vector"

#include <string.h>

/////////////////////////////////////////////////
class tVoiceCodecList
{
public:
  tVoiceCodecList()
  {
    const tAudioCodecDescription **p = MM_Voice_GetCodecList();
    int i;

    for (i=0; p[i]!=0; i++)
    {
      if ((p[i]->pFriendlyName != 0) && _stricmp("CN", p[i]->pFriendlyName) && _stricmp("DTMF", p[i]->pFriendlyName))
      {
        tAudioCodecDescSettings tmp={0};
        tmp.pVoiceCodec = p[i];
        m_pCodecs.push_back(tmp);
      }
    }

    tAudioCodecDescSettings tmp={0};
    m_pCodecs.push_back(tmp);
  }

  tAudioCodecDescSettings *get() { return &m_pCodecs[0]; } 

private:
  std::vector<tAudioCodecDescSettings> m_pCodecs;
};
static tVoiceCodecList m_VoiceCodecList;



CVoiceChannel::CVoiceChannel(void)
  : m_pCodec(m_VoiceCodecList.get())
{
  SetLocalRtpPort(VOICE_CHANNEL_RTP_LOCAL_PORT);
  SetLocalRtcpPort(VOICE_CHANNEL_RTCP_LOCAL_PORT);
  SetRemoteRtpPort(VOICE_CHANNEL_RTP_REMOTE_PORT);
  SetRemoteRtcpPort(VOICE_CHANNEL_RTCP_REMOTE_PORT);

  // Default rtp in/out maps
  BuildDefaultVoiceRtpMap(m_aInRtpMap);
  BuildDefaultVoiceRtpMap(m_aOutRtpMap);
}

CVoiceChannel::~CVoiceChannel(void)
{
  Stop();
}

const tAudioCodecDescSettings* CVoiceChannel::GetCodecList()
{
  return m_VoiceCodecList.get();
}

void CVoiceChannel::BuildDefaultVoiceRtpMap(tCodecPayload aRtpMap[SPIRIT_MAX_CODEC_MAP_SIZE])
{
  memset(aRtpMap, 0, sizeof(tCodecPayload) * SPIRIT_MAX_CODEC_MAP_SIZE);

  int index = 0;
  uint32  pt;
  tAudioCodecDescSettings* pCodec = m_VoiceCodecList.get();

  while(pCodec && pCodec->pVoiceCodec)
  {
    pt = GetPayloadTypeByName(pCodec->pVoiceCodec->pFriendlyName);

    // Skip codec with 255 payload type
    if(255 != pt)
    {
      aRtpMap[index].codec.audio = *pCodec;
      aRtpMap[index].Payload = pt;
      index++;
    }
    pCodec++;
  }
}

bool CVoiceChannel::Start()
{
  if(NULL == m_hMM)
    return false;

  tResult res;
  uint32 channelId = INVALID_CHANNEL;

  tCallParam  CallParam;
  CallParam.transport_type = tr_internal;

  CallParam.transport.intrnl.RtpSrc.ip.i32 = m_LocalRtpAddr.ip.i32;
  CallParam.transport.intrnl.RtpSrc.port = m_LocalRtpAddr.port;

  CallParam.transport.intrnl.RtcpSrc.ip.i32 = m_LocalRtcpAddr.ip.i32;
  CallParam.transport.intrnl.RtcpSrc.port = m_LocalRtcpAddr.port;

  res = MM_Voice_Channel_Create(m_hMM, &channelId, &CallParam);
  if(SPIRIT_RESULT_OK != res || INVALID_CHANNEL == channelId)
  {
    return false;
  }

  tVariant val;

  val.u32 = m_bAutoFecEnable;
  MM_SetChannelParam(m_hMM, channelId, SVEP_CHANNEL_AUTO_FEC_ENABLE, val);

  res = MM_Voice_SetRtpPayloadMapping(m_hMM, channelId, m_aInRtpMap, m_aOutRtpMap);
  if(SPIRIT_RESULT_OK != res)
  {
    MM_Voice_Channel_Destroy(m_hMM, channelId);
    return false;
  }

  res = MM_Voice_SetEncoder(m_hMM, channelId, m_pCodec);
  if(SPIRIT_RESULT_OK != res)
  {
    MM_Voice_Channel_Destroy(m_hMM, channelId);
    return false;
  }

  // BSV

  // //VVK
  // //Dear BSV why did you disable RTCP sending??? 
  // //
  // 
  // BSV
  // Dear VVK I did disable RTCP sending because this version is uses without RTCP.
  //
  //
  res = MM_Voice_Channel_SetDestAddr(m_hMM, channelId, &m_RemoteRtpAddr, &m_RemoteRtcpAddr);
  //res = MM_Voice_Channel_SetDestAddr(m_hMM, channelId, &m_RemoteRtpAddr, NULL);
  if(SPIRIT_RESULT_OK != res)
  {
    MM_Voice_Channel_Destroy(m_hMM, channelId);
    return false;
  }

  res = MM_Voice_Channel_StartRx(m_hMM, channelId);
  if(SPIRIT_RESULT_OK != res)
  {
    MM_Voice_Channel_Destroy(m_hMM, channelId);
    return false;
  }

  res = MM_Voice_Channel_StartTx(m_hMM, channelId);
  if(SPIRIT_RESULT_OK != res)
  {
    MM_Voice_Channel_StopRx(m_hMM, channelId);
    MM_Voice_Channel_Destroy(m_hMM, channelId);
    return false;
  }

  m_Id = channelId;
  return true;
}

// Prepare Start
bool CVoiceChannel::PreStart()
{
  if(NULL == m_hMM)
    return false;

  tResult res;
  uint32 channelId = INVALID_CHANNEL;

  tCallParam  CallParam;
  CallParam.transport_type = tr_internal;

  CallParam.transport.intrnl.RtpSrc.ip.i32 = m_LocalRtpAddr.ip.i32;
  CallParam.transport.intrnl.RtpSrc.port = m_LocalRtpAddr.port;

  CallParam.transport.intrnl.RtcpSrc.ip.i32 = m_LocalRtcpAddr.ip.i32;
  CallParam.transport.intrnl.RtcpSrc.port = m_LocalRtcpAddr.port;

  res = MM_Voice_Channel_Create(m_hMM, &channelId, &CallParam);
  if(SPIRIT_RESULT_OK != res)
  {
    return false;
  }

  tVariant val;

  val.u32 = m_bAutoFecEnable;
  MM_SetChannelParam(m_hMM, channelId, SVEP_CHANNEL_AUTO_FEC_ENABLE, val);

  res = MM_Voice_SetRtpPayloadMapping(m_hMM, channelId, m_aInRtpMap, m_aOutRtpMap);
  if(SPIRIT_RESULT_OK != res)
  {
    MM_Voice_Channel_Destroy(m_hMM, channelId);
    return false;
  }

  res = MM_Voice_SetEncoder(m_hMM, channelId, m_pCodec);
  if(SPIRIT_RESULT_OK != res)
  {
    MM_Voice_Channel_Destroy(m_hMM, channelId);
    return false;
  }

  // BSV
  //VVK
  //Dear BSV why did you disable RTCP sending??? 
  //
  res = MM_Voice_Channel_SetDestAddr(m_hMM, channelId, &m_RemoteRtpAddr, &m_RemoteRtcpAddr);
  //res = MM_Voice_Channel_SetDestAddr(m_hMM, channelId, &m_RemoteRtpAddr, NULL);
  if(SPIRIT_RESULT_OK != res)
  {
    MM_Voice_Channel_Destroy(m_hMM, channelId);
    return false;
  }
  m_Id = channelId;
  return true;
}

// Start Tx
bool CVoiceChannel::StartTx()
{
  if(NULL == m_hMM || INVALID_CHANNEL == m_Id)
    return false;

  return (SPIRIT_RESULT_OK == MM_Voice_Channel_StartTx(m_hMM, m_Id));
}
// Start Rx
bool CVoiceChannel::StartRx()
{
  if(NULL == m_hMM || INVALID_CHANNEL == m_Id)
    return false;

  return (SPIRIT_RESULT_OK == MM_Voice_Channel_StartRx(m_hMM, m_Id));
}
// Stop voice channel
bool CVoiceChannel::Stop()
{
  if(INVALID_CHANNEL != m_Id && NULL != m_hMM)
  {
    tResult res;
    res = MM_Voice_Channel_StopTx(m_hMM, m_Id);
    res = MM_Voice_Channel_StopRx(m_hMM, m_Id);
    res = MM_Voice_Channel_Destroy(m_hMM, m_Id);
    m_Id = INVALID_CHANNEL;
  }
  return true;
}

// Get statistics
bool CVoiceChannel::GetStatistics(sSpiritChannelStat *pStats)
{
  if(!pStats)
    return false;

  memset(pStats, 0, sizeof(sSpiritChannelStat));

  if(INVALID_CHANNEL == m_Id || NULL == m_hMM)
    return false;

  tVariant val;
  val.pData = pStats;
  tResult res = MM_GetChannelParam(m_hMM, m_Id, SVEP_CHANNEL_STAT, &val);
  return (SPIRIT_RESULT_OK == res);
}

// Set encoder
bool CVoiceChannel::SetCodec(const tAudioCodecDescSettings *pCodec)
{  
  if(!pCodec)
    return false;
  if(INVALID_CHANNEL != m_Id && NULL != m_hMM)
  {
    tResult res = MM_Voice_SetEncoder(m_hMM, m_Id, pCodec);
    if(SPIRIT_RESULT_OK != res)
      return false;
  }
  m_pCodec = (tAudioCodecDescSettings*)pCodec;
  return true;
}

const tAudioCodecDescSettings* CVoiceChannel::GetCodec()
{
  return (const tAudioCodecDescSettings*)m_pCodec;
}

// Get encoder name from channel
const char* CVoiceChannel::GetEncoderName(char aName[64])
{
  tAudioCodecDescSettings acds;
  memset(&acds, 0, sizeof(acds));

  aName[0] = 0;
  if(!m_hMM || INVALID_CHANNEL == m_Id)
    return aName;

  if(SPIRIT_RESULT_OK != MM_Voice_GetEncoder(m_hMM, m_Id, &acds))
    return aName;

  if(!acds.pVoiceCodec)
    return aName;

  strcpy(aName, acds.pVoiceCodec->pFriendlyName);
  return aName;
}

// Get decoder name from channel
const char* CVoiceChannel::GetDecoderName(char aName[64])
{
  tAudioCodecDescSettings acds;
  memset(&acds, 0, sizeof(acds));

  aName[0] = 0;
  if(!m_hMM || INVALID_CHANNEL == m_Id)
    return aName;

  if(SPIRIT_RESULT_OK != MM_Voice_GetDecoder(m_hMM, m_Id, &acds))
    return aName;

  if(!acds.pVoiceCodec)
    return aName;

  strcpy(aName, acds.pVoiceCodec->pFriendlyName);
  return aName;
}

