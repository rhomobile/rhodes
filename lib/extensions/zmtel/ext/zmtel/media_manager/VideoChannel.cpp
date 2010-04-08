#include "../stdafx.h"
#include "VideoChannel.h"
#include "vector"

/////////////////////////////////////////////////
class tVideoCodecList
{
public:
  tVideoCodecList()
  {
    const tVideoCodecDescription **p = MM_Video_GetCodecList();
    int i;

    for (i=0; p[i]!=0; i++)
    {
      tVideoCodecDescSettings tmp={0};
      tmp.pVideoCodec = p[i];
      m_pCodecs.push_back(tmp);
    }

    tVideoCodecDescSettings tmp={0};
    m_pCodecs.push_back(tmp);
  }

  tVideoCodecDescSettings *get() { return &m_pCodecs[0]; } 

private:
  std::vector<tVideoCodecDescSettings> m_pCodecs;
};
static tVideoCodecList m_VideoCodecList;

/////////////////////////////////////////////////

sSpiritVideoRect CVideoChannel::m_rectPreview = {
  VIDEO_CHANNEL_RECT_X, 
  VIDEO_CHANNEL_RECT_Y, 
  VIDEO_CHANNEL_RECT_CX / 2,
  VIDEO_CHANNEL_RECT_CY / 2
};

CVideoChannel::CVideoChannel(void)
  : m_pCodec(m_VideoCodecList.get())
  , m_VoiceChannelId(INVALID_CHANNEL)
  , m_Camera(0)
  , m_Vsd(VSD_BOTH)
  , m_Bitrate(128)
  , m_width(176)
  , m_height(144)
  , m_frame_rate_num(10)
  , m_frame_rate_denom(1)
  , m_max_rtp_frame_size(1400)
  , m_channelNack(1)
  , m_channelFreezeOnLoss(1)
  , m_fecType(0)//((5<<8)|(2))
{
  SetLocalRtpPort(VIDEO_CHANNEL_RTP_LOCAL_PORT);
  SetLocalRtcpPort(VIDEO_CHANNEL_RTCP_LOCAL_PORT);
  SetRemoteRtpPort(VIDEO_CHANNEL_RTP_REMOTE_PORT);
  SetRemoteRtcpPort(VIDEO_CHANNEL_RTCP_REMOTE_PORT);

  m_Rect.X = VIDEO_CHANNEL_RECT_X;
  m_Rect.Y = VIDEO_CHANNEL_RECT_Y;
  m_Rect.Width = VIDEO_CHANNEL_RECT_CX;
  m_Rect.Height = VIDEO_CHANNEL_RECT_CY;

  // Default video rtp in/out maps
  BuildDefaultVideoRtpMap(m_aInRtpMap);
  BuildDefaultVideoRtpMap(m_aOutRtpMap);
}

CVideoChannel::~CVideoChannel(void)
{
  Stop();
}

void CVideoChannel::BuildDefaultVideoRtpMap(tCodecPayload aRtpMap[SPIRIT_MAX_CODEC_MAP_SIZE])
{
  memset(aRtpMap, 0, sizeof(tCodecPayload) * SPIRIT_MAX_CODEC_MAP_SIZE);

  int index = 0;
  uint32  pt;
  tVideoCodecDescSettings* pCodec = m_VideoCodecList.get();

  while(pCodec && pCodec->pVideoCodec)
  {
    pt = GetPayloadTypeByName(pCodec->pVideoCodec->pFriendlyName);

    // All codecs with payload type 255 are skipped
    if(255 != pt)
    {
      aRtpMap[index].codec.video = *pCodec;
      aRtpMap[index].Payload = pt;
      index++;
    }
    pCodec++;
  }
}

const tVideoCodecDescSettings* CVideoChannel::GetCodecList()
{
  return m_VideoCodecList.get();
}

// Prepare Start
bool CVideoChannel::PreStart()
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

  res = MM_Video_Channel_Create(m_hMM, &channelId, &CallParam);
  if(SPIRIT_RESULT_OK != res || INVALID_CHANNEL == channelId)
  {
    return false;
  }

  tVariant val;

  val.u32 = m_channelNack;
  MM_SetChannelParam(m_hMM,channelId,SVEP_CHANNEL_NACK,val);

// val.u32 = m_fecEnable;
// MM_SetChannelParam(m_hMM,channelId,SVEP_CHANNEL_FEC_ENABLE,val);

  val.u32 = m_bAutoFecEnable;
  MM_SetChannelParam(m_hMM, channelId, SVEP_CHANNEL_AUTO_FEC_ENABLE, val);
  
  val.u32 = m_fecType;
  MM_SetChannelParam(m_hMM,channelId,SVEP_CHANNEL_FEC_TYPE,val);

  val.u32 = m_channelFreezeOnLoss;
  MM_SetChannelParam(m_hMM,channelId,SVEP_CHANNEL_FREEZE_ON_LOSS,val);

  // BSV
  /*
  if(INVALID_CHANNEL != m_VoiceChannelId)
  {
    res = MM_Video_Channel_Sync(m_hMM, channelId, m_VoiceChannelId);
    if(SPIRIT_RESULT_OK != res)
    {
      MM_Video_Channel_Destroy(m_hMM, channelId);
      return false;
    }
  }
  */

  res = MM_Video_SetRtpPayloadMapping(m_hMM, channelId, m_aInRtpMap, m_aOutRtpMap);
  if(SPIRIT_RESULT_OK != res)
  {
    MM_Video_Channel_Destroy(m_hMM, channelId);
    return false;
  }

  // BSV
  //VVK
  //Dear BSV why did you disable RTCP sending??? 
  //
  res = MM_Video_Channel_SetDestAddr(m_hMM, channelId, &m_RemoteRtpAddr, &m_RemoteRtcpAddr);
  //res = MM_Video_Channel_SetDestAddr(m_hMM, channelId, &m_RemoteRtpAddr, NULL);
  if(SPIRIT_RESULT_OK != res)
  {
    MM_Video_Channel_Destroy(m_hMM, channelId);
    return false;
  }

  val.u32 = 0xFFFFFF;
  res = MM_SetChannelParam(m_hMM, channelId, SVEP_CHANNEL_VIDEO_OVERLAY_COLOR, val);
  if(SPIRIT_RESULT_OK != res)
  {
    MM_Video_Channel_Destroy(m_hMM, channelId);
    return false;
  }

  val.pData = &m_Rect;
  res = MM_SetChannelParam(m_hMM, channelId, SVEP_CHANNEL_RECT, val);
  if(SPIRIT_RESULT_OK != res)
  {
    MM_Video_Channel_Destroy(m_hMM, channelId);
    return false;
  }
  val.u32 = 0;
  res = MM_SetChannelParam(m_hMM, channelId, SVEP_CHANNEL_HIDE, val);
  if(SPIRIT_RESULT_OK != res)
  {
    MM_Video_Channel_Destroy(m_hMM, channelId);
    return false;
  }

  // Codec settings

  //cpk_video_bitrate, 128 * 1024
  //cpk_video_width, 176
  //cpk_video_height, 144
  //cpk_video_frame_rate_num, 10
  //cpk_video_frame_rate_denom, 1
  //cpk_video_max_rtp_frame_size, 1440

  m_pCodec->count = 6;

  m_pCodec->pParam[0].key = cpk_video_bitrate;
  m_pCodec->pParam[0].u.value = m_Bitrate * 1024;

  m_pCodec->pParam[1].key = cpk_video_width;
  m_pCodec->pParam[1].u.value = m_width;

  m_pCodec->pParam[2].key = cpk_video_height;
  m_pCodec->pParam[2].u.value = m_height;

  m_pCodec->pParam[3].key = cpk_video_frame_rate_num;
  m_pCodec->pParam[3].u.value = m_frame_rate_num;

  m_pCodec->pParam[4].key = cpk_video_frame_rate_denom;
  m_pCodec->pParam[4].u.value = m_frame_rate_denom;

  m_pCodec->pParam[5].key = cpk_video_max_rtp_frame_size;
  m_pCodec->pParam[5].u.value = m_max_rtp_frame_size;

  res = MM_Video_SetEncoder(m_hMM, channelId, m_pCodec);
  if(SPIRIT_RESULT_OK != res)
  {
    MM_Video_Channel_Destroy(m_hMM, channelId);
    return false;
  }

  val.u32 = 0;
  res = MM_SetParam(m_hMM, SVEP_HIDE_PREVIEW, val);
  if(SPIRIT_RESULT_OK != res)
  {
    MM_Video_Channel_Destroy(m_hMM, channelId);
    return false;
  }

  val.pData = &m_rectPreview;
  res = MM_SetParam(m_hMM, SVEP_ENABLE_PREVIEW, val);
  if(SPIRIT_RESULT_OK != res)
  {
    MM_Video_Channel_Destroy(m_hMM, channelId);
    return false;
  }

  val.u32 = m_Camera;
  res = MM_SetParam(m_hMM, SVEP_CAMERA, val);
  if(SPIRIT_RESULT_OK != res)
  {
    MM_Video_Channel_Destroy(m_hMM, channelId);
    return false;
  }
  m_Id = channelId;
  return true;
}

// Start Tx
bool CVideoChannel::StartTx()
{
  if(NULL == m_hMM || INVALID_CHANNEL == m_Id)
    return false;

  if(VSD_RECV == m_Vsd)
    return true;

  return (SPIRIT_RESULT_OK == MM_Video_Channel_StartTx(m_hMM, m_Id));
}

// Start Rx
bool CVideoChannel::StartRx()
{
  if(NULL == m_hMM || INVALID_CHANNEL == m_Id)
    return false;

  if(VSD_SEND == m_Vsd)
    return true;

  return (SPIRIT_RESULT_OK == MM_Video_Channel_StartRx(m_hMM, m_Id));
}

bool CVideoChannel::Stop()
{
  if(INVALID_CHANNEL != m_Id && NULL != m_hMM)
  {
    tResult res;
    res = MM_Video_Channel_StopTx(m_hMM, m_Id);
    res = MM_Video_Channel_StopRx(m_hMM, m_Id);
    res = MM_Video_Channel_Destroy(m_hMM, m_Id);
    m_Id = INVALID_CHANNEL;
  }
  return true;
}

bool CVideoChannel::SetRect(int x, int y, int cx, int cy)
{
  sSpiritVideoRect rect;
  rect.X = x;
  rect.Y = y;
  rect.Width = cx;
  rect.Height = cy;

  if(INVALID_CHANNEL == m_Id || NULL == m_hMM)
  {
    m_Rect.X = rect.X;
    m_Rect.Y = rect.Y;
    m_Rect.Width = rect.Width;
    m_Rect.Height = rect.Height;
    return true;
  }
  tResult res;
  tVariant  val;

  val.pData = &rect;
  res = MM_SetChannelParam(m_hMM, m_Id, SVEP_CHANNEL_RECT, val);
  if(SPIRIT_RESULT_OK != res)
    return false;

  m_Rect.X = rect.X;
  m_Rect.Y = rect.Y;
  m_Rect.Width = rect.Width;
  m_Rect.Height = rect.Height;
  return true;
}

bool CVideoChannel::SetLocalWindow(int x, int y, int cx, int cy)
{
  sSpiritVideoRect rect;
  rect.X = x;
  rect.Y = y;
  rect.Width = cx;
  rect.Height = cy;

  if(INVALID_CHANNEL == m_Id || NULL == m_hMM)
  {
    m_rectPreview.X = rect.X;
    m_rectPreview.Y = rect.Y;
    m_rectPreview.Width = rect.Width;
    m_rectPreview.Height = rect.Height;
    return true;
  }
  tResult res;
  tVariant  val;

  val.pData = &rect;
  res = MM_SetParam(m_hMM, SVEP_ENABLE_PREVIEW, val);
  if(SPIRIT_RESULT_OK != res)
    return false;

  m_rectPreview.X = rect.X;
  m_rectPreview.Y = rect.Y;
  m_rectPreview.Width = rect.Width;
  m_rectPreview.Height = rect.Height;
  return true;
}
bool CVideoChannel::SetRemoteWindow(int x, int y, int cx, int cy)
{
  sSpiritVideoRect rect;
  rect.X = x;
  rect.Y = y;
  rect.Width = cx;
  rect.Height = cy;

  if(INVALID_CHANNEL == m_Id || NULL == m_hMM)
  {
    m_Rect.X = rect.X;
    m_Rect.Y = rect.Y;
    m_Rect.Width = rect.Width;
    m_Rect.Height = rect.Height;
    return true;
  }
  tResult res;
  tVariant  val;

  val.pData = &rect;
  res = MM_SetChannelParam(m_hMM, m_Id, SVEP_CHANNEL_RECT, val);
  if(SPIRIT_RESULT_OK != res)
    return false;

  m_Rect.X = rect.X;
  m_Rect.Y = rect.Y;
  m_Rect.Width = rect.Width;
  m_Rect.Height = rect.Height;
  return true;
}

bool CVideoChannel::SetSyncChannel(uint32 Id)
{
  if(INVALID_CHANNEL == m_VoiceChannelId)
    return false;

  if(NULL != m_hMM && INVALID_CHANNEL != m_Id)
  {
    tResult res;
    res = MM_Video_Channel_Sync(m_hMM, m_Id, m_VoiceChannelId);
    if(SPIRIT_RESULT_OK != res) 
      return false;
  }

  m_VoiceChannelId = Id;
  return true;
}

bool CVideoChannel::SetCodec(const tVideoCodecDescSettings *pCodec)
{
  if(!pCodec)
    return false;
  if(INVALID_CHANNEL != m_Id && NULL != m_hMM)
  {
    tResult res = MM_Video_SetEncoder(m_hMM, m_Id, pCodec);
    if(SPIRIT_RESULT_OK != res)
      return false;
  }
  m_pCodec = (tVideoCodecDescSettings*)pCodec;
  return true;
}

const tVideoCodecDescSettings* CVideoChannel::GetCodec()
{
  return (const tVideoCodecDescSettings*)m_pCodec;
}
/*
bool CVideoChannel::SetCamera(int Camera)
{
  if(Camera < 0 || Camera > 1)
    return false;

  if(INVALID_CHANNEL != m_Id && NULL != m_hMM)
  {
    tDeviceList *pDevice = 0;
    tResult res = MM_Video_GetCaptureDeviceList(m_hMM, pDevice);
    if(SPIRIT_RESULT_OK != res || !pDevice)
      return false;

    if(Camera >= pDevice->uCount)
    {
      MM_Free_DeviceList(m_hMM, pDevice);
      return false;
    }

    if(Camera == pDevice->iSelected)
    {
      MM_Free_DeviceList(m_hMM, pDevice);
      return true;
    }

    res = MM_Video_SetCaptureDevice(m_hMM, pDevice->list[Camera]);
    MM_Free_DeviceList(m_hMM, pDevice);
    if(SPIRIT_RESULT_OK != res)
      return false;
  }
  m_Camera = (uint32)Camera;
  return true;
}
*/
bool CVideoChannel::SetCamera(int Camera)
{
  if(Camera < 1 || Camera > 2)
    return false;

  if(INVALID_CHANNEL != m_Id && NULL != m_hMM)
  {
    tVariant val;
    val.u32 = (uint32)Camera;

    tResult res = MM_SetParam(m_hMM, SVEP_CAMERA, val);
    if(SPIRIT_RESULT_OK != res)
      return false;
  }
  m_Camera = (uint32)Camera;
  return true;
}
int CVideoChannel::GetCamera()
{
  return m_Camera;
}

bool CVideoChannel::GetRecvStats(sSpiritVideoChannelStat *pStats)
{
  if(!pStats)
    return false;

  memset(pStats, 0, sizeof(sSpiritVideoChannelStat));

  if(INVALID_CHANNEL == m_Id || NULL == m_hMM)
    return false;

  tVariant val;
  val.pData = pStats;
  tResult res = MM_GetChannelParam(m_hMM, m_Id, SVEP_CHANNEL_RECV_STAT, &val);
  return (SPIRIT_RESULT_OK == res);
}
bool CVideoChannel::GetSendStats(sSpiritVideoChannelStat *pStats)
{
  if(!pStats)
    return false;

  memset(pStats, 0, sizeof(sSpiritVideoChannelStat));

  if(INVALID_CHANNEL == m_Id || NULL == m_hMM)
    return false;

  tVariant val;
  val.pData = pStats;
  tResult res = MM_GetChannelParam(m_hMM, m_Id, SVEP_CHANNEL_SEND_STAT, &val);
  return (SPIRIT_RESULT_OK == res);
}

const char* CVideoChannel::GetEncoderName(char aName[64])
{
  tVideoCodecDescSettings vcds;
  memset(&vcds, 0, sizeof(vcds));

  aName[0] = 0;
  if(!m_hMM || INVALID_CHANNEL == m_Id)
    return aName;

  if(SPIRIT_RESULT_OK != MM_Video_GetEncoder(m_hMM, m_Id, &vcds))
    return aName;

  if(!vcds.pVideoCodec)
    return aName;

  strcpy(aName, vcds.pVideoCodec->pFriendlyName);
  return aName;
}

const char* CVideoChannel::GetDecoderName(char aName[64])
{
  tVideoCodecDescSettings vcds;
  memset(&vcds, 0, sizeof(vcds));

  aName[0] = 0;
  if(!m_hMM || INVALID_CHANNEL == m_Id)
    return aName;

  if(SPIRIT_RESULT_OK != MM_Video_GetDecoder(m_hMM, m_Id, &vcds))
    return aName;

  if(!vcds.pVideoCodec)
    return aName;

  strcpy(aName, vcds.pVideoCodec->pFriendlyName);
  return aName;
}

void CVideoChannel::SetFps(uint32 Fps)
{
  m_frame_rate_num = Fps;
  m_frame_rate_denom = 1;
}

void CVideoChannel::SetVideoStreamDirection(eVideoStreamDirection Vsd)
{
  m_Vsd = Vsd;
}

void CVideoChannel::SetBps(uint32 Bps)
{
  m_Bitrate = Bps;
}
  
void CVideoChannel::SetVideoSize(uint32 Width, uint32 Height)
{
  m_width = Width;
  m_height = Height;
}

void CVideoChannel::SetMaxRtpFrameSize(uint32 mfrs)
{
  m_max_rtp_frame_size = mfrs;
}

bool CVideoChannel::SetChannelNack(uint32 val)
{
  m_channelNack = val;
  return true;
}
bool CVideoChannel::SetChannelFreezeOnLoss(uint32 val)
{
  m_channelFreezeOnLoss = val;
  return true;
}

bool CVideoChannel::SetFecType(uint32 val)
{
  m_fecType = val;
  return true;
}
