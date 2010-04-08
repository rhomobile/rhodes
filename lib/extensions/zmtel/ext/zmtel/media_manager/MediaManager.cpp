#include "../stdafx.h"
#include "MediaManager.h"

#include "../storage_manager/storage_manager.h"

#define COUNT(x) (sizeof(x)/sizeof((x)[0]))

struct tAecModeNames
{
  teSpiritRDMode mode;
  const char *name;
};
static const tAecModeNames g_AecModeNames[] =
{
  { rd_DISABLED   , "Disabled"   },
  { rd_HANDSFREE  , "Handsfree"  },
  { rd_HANDSET    , "Handset"    },
  { rd_HEADSET    , "Headset"    },
  { rd_HEADPHONES , "Headphones" },
  { rd_CUSTOM     , "Custom"     },
};

static const teSpiritRDMode g_EnabledAecModes[] =
{
  rd_HANDSET    ,
  rd_HANDSFREE  ,
  //rd_HEADSET    ,
  rd_HEADPHONES ,
  rd_CUSTOM   ,
  rd_DISABLED   ,
};

static const char* find_mode_name_by_id(teSpiritRDMode id)
{
  for (int i=0; i<COUNT(g_AecModeNames); i++)
  {
    if (g_AecModeNames[i].mode == id)
      return g_AecModeNames[i].name;
  }
  return 0;
}

static teSpiritRDMode find_mode_id_by_name(const char* name)
{
  for (int i=0; i<COUNT(g_AecModeNames); i++)
  {
    if (!_stricmp(name,g_AecModeNames[i].name))
      return g_AecModeNames[i].mode;
  }
  return g_EnabledAecModes[0];
}

char* CMediaManager::m_paAECModeList[16] = { 0 };

CMediaManager::CMediaManager(void)
: m_hMM(0)
, m_eAECMode(g_EnabledAecModes[0])
, m_pAecParams(0)
{
  params.other.count = 0;

  if (!m_paAECModeList[0])
  {
    for (int i=0; i<COUNT(g_EnabledAecModes); i++)
    {
      m_paAECModeList[i] = (char*)find_mode_name_by_id(g_EnabledAecModes[i]);
    }
  }
}

CMediaManager::~CMediaManager(void)
{
  params.other.count = 0;
  if(m_pAecParams)
    delete [] m_pAecParams;
  Destroy();
}

#define ENABLE_SPIRIT_LOG 1

//
bool CMediaManager::Create()
{
  tResult  res;
  tVariant val;

  pLog = NULL;

  // TODO: Full path to utils
  // Get full file path
  CStringA strParamsPath = "settings.ini";
  wchar_t path[1024] = {0};
  if(0 != GetModuleFileName(NULL, path, sizeof(path) / sizeof(wchar_t)))
  {
    size_t len = wcslen(path);
    for(int i = len; i >= 0; i--)
      if(path[i] == '\\')
      {
        path[i] = 0;
        strParamsPath.Format("%S\\%s", path, strParamsPath);
        break;
      }
  }
  //------------------------------

  SetDefaultSettings(strParamsPath.GetBuffer());

#if ENABLE_SPIRIT_LOG
  if(params.log.type >= 0)
  {
    res = SpiritLogCreate(
      &pLog,
      "p2p_demo.log",
      (tSpiritLogType)params.log.type,
      params.log.size
      );
    if (res != SPIRIT_RESULT_OK)
      pLog = NULL;
  }
#endif

  // Create media_magaer
  res = MM_Create(&m_hMM, pLog);
  if (SPIRIT_RESULT_OK != res)
    return false;


  //  Set
  //  teSpiritRDMode    m_eAECMode;
  //  tSpiritLog        *pLog;

  //val.i = -1;
  //MM_SetParam(m_hMM, "SIO:LogLevel", val);

  // net
  SetLocalPortRtpVoice(params.net.local.voiceRtpPort);
  //params.net.local.voiceRtcpPort
  SetLocalPortRtpVideo(params.net.local.videoRtpPort);
  //params.net.local.videoRtcpPort

  SetRemotePortRtpVoice(params.net.remote.voiceRtpPort);
  //params.net.remote.voiceRtcpPort
  SetRemotePortRtpVideo(params.net.remote.videoRtpPort);
  //params.net.remote.videoRtcpPort

  // voice
  SetAECMode(find_mode_name_by_id((teSpiritRDMode)params.voice.aecMode));
  SetVoiceCodec(params.voice.codec);
  m_VoiceChannel.SetAutoFecEnable(params.voice.bAutoFecEnable);

  // video
  SetCamera(params.video.camera);
  SetVsd(params.video.vsd);
  SetFps(params.video.framerate);
  SetBps(params.video.bitrate);
  SetVideoSize(params.video.width, params.video.height);
  SetMaxRtpFrameSize(params.video.mrfs);
  SetVideoCodec(params.video.codec);

  m_VideoChannel.SetChannelNack(params.video.channleNack);
  m_VideoChannel.SetChannelFreezeOnLoss(params.video.channleFreezeOnLoss);
  m_VideoChannel.SetAutoFecEnable(params.video.bAutoFecEnable);
  m_VideoChannel.SetFecType(params.video.fecType);

  //
  val.i = 0;
  MM_SetParam(m_hMM, "VoE:HiSysLoadCtrl", val);

  // other
  for(int i = 0; i < params.other.count; i++)
  {
    val.u32 = params.other.value[i].u32;
    MM_SetParam(m_hMM, params.other.name[i].c_str(), val);
  }

  return true;
}

bool CMediaManager::Destroy()
{
  if(m_hMM)
  {
    // Destroy media manager
    tResult res = MM_Destroy(m_hMM);
    if(SPIRIT_RESULT_OK != res)
    {
      if (pLog != NULL)
      {
        SpiritLogDestroy(pLog);
        pLog = NULL;
      }
      return false;
    }
    m_hMM = 0;
  }

  if (pLog != NULL)
  {
    SpiritLogDestroy(pLog);
  }
  return true;
}

const char** CMediaManager::GetAECModeList()
{
  return (const char**)m_paAECModeList;
}

bool CMediaManager::SetAECMode(const char* pMode)
{
  if(!pMode || !m_hMM)
    return false;

  tVariant val;
  tResult res;

  m_eAECMode = find_mode_id_by_name(pMode);
  if(m_eAECMode == rd_CUSTOM)
  {
    if(!m_pAecParams)
      return false;

    val.i = rd_HANDSFREE;
    res = MM_SetParam(m_hMM, "VoE:RD:Mode", val);
    if (SPIRIT_RESULT_OK != res)
      return false;

    val.pData = m_pAecParams;
    res = MM_SetParam(m_hMM, "VoE:RD:TunningParams", val);
    if (SPIRIT_RESULT_OK != res)
      return false;

  }
  else
  {
    val.i = m_eAECMode;
    res = MM_SetParam(m_hMM, "VoE:RD:Mode", val);
    if (SPIRIT_RESULT_OK != res)
      return false;
  }
  return true;
}

const char* CMediaManager::GetAECMode()
{
  if(m_hMM)
  {
    tVariant val;
    tResult res = MM_GetParam(m_hMM, "VoE:RD:Mode", &val);
    if (SPIRIT_RESULT_OK != res)
      return 0;
    m_eAECMode = (teSpiritRDMode)val.i;
  }
  return find_mode_name_by_id(m_eAECMode);
}

bool CMediaManager::SetLocalIp(const char* pIp)
{
  if(!pIp)
    return false;

  m_VoiceChannel.SetLocalIp(pIp);
  m_VideoChannel.SetLocalIp(pIp);
  return true;
}

bool CMediaManager::SetVoiceCodec(const char* pCodec)
{
  if(!pCodec)
    return false;

  tAudioCodecDescSettings *pACDS = (tAudioCodecDescSettings*)CVoiceChannel::GetCodecList();
  if(!pACDS)
    return false;

  while(pACDS && pACDS->pVoiceCodec)
  {
    if(!_stricmp(pCodec, pACDS->pVoiceCodec->pFriendlyName))
      return m_VoiceChannel.SetCodec(pACDS);
    pACDS++;
  }
  return false;
}

const char* CMediaManager::GetVoiceCodec()
{
  const tAudioCodecDescSettings* pCodec = m_VoiceChannel.GetCodec();
  if(!pCodec || !pCodec->pVoiceCodec)
    return 0;

  return pCodec->pVoiceCodec->pFriendlyName;
}

bool CMediaManager::SetVideoCodec(const char* pCodec)
{
  if(!pCodec)
    return false;

  tVideoCodecDescSettings *pVCDS = (tVideoCodecDescSettings*)CVideoChannel::GetCodecList();
  if(!pVCDS)
    return false;

  while(pVCDS && pVCDS->pVideoCodec)
  {
    if(!_stricmp(pCodec, pVCDS->pVideoCodec->pFriendlyName))
      return m_VideoChannel.SetCodec(pVCDS);
    pVCDS++;
  }
  return false;
}

const char* CMediaManager::GetVideoCodec()
{
  const tVideoCodecDescSettings* pCodec = m_VideoChannel.GetCodec();
  if(!pCodec || !pCodec->pVideoCodec)
    return 0;

  return pCodec->pVideoCodec->pFriendlyName;
}

bool CMediaManager::SetLocalPortRtpVoice(unsigned short Port)
{
  m_VoiceChannel.SetLocalRtpPort(Port);
  m_VoiceChannel.SetLocalRtcpPort(Port + 1);
  return true;
}

bool CMediaManager::SetLocalPortRtpVideo(unsigned short Port)
{
  m_VideoChannel.SetLocalRtpPort(Port);
  m_VideoChannel.SetLocalRtcpPort(Port + 1);
  return true;
}

bool CMediaManager::SetRemoteIp(const char* pIp)
{
  if(!pIp)
    return false;

  m_VoiceChannel.SetRemoteIp(pIp);
  m_VideoChannel.SetRemoteIp(pIp);
  return true;
}

bool CMediaManager::SetRemotePortRtpVoice(unsigned short Port)
{
  m_VoiceChannel.SetRemoteRtpPort(Port);
  m_VoiceChannel.SetRemoteRtcpPort(Port + 1);
  return true;
}

bool CMediaManager::SetRemotePortRtpVideo(unsigned short Port)
{
  m_VideoChannel.SetRemoteRtpPort(Port);
  m_VideoChannel.SetRemoteRtcpPort(Port + 1);
  return true;
}

bool CMediaManager::Start(bool bVideo)
{
  if (!BeginStart(bVideo))
    return false;

  return EndStart(bVideo);
}

bool CMediaManager::BeginStart(bool bVideo)
{
  m_VoiceChannel.Init(m_hMM);
  if(bVideo)
  {
    m_VideoChannel.Init(m_hMM);

  }
  m_VoiceChannel.PreStart();
  m_VoiceChannel.StartRx();

  if(bVideo)
  {
    m_VideoChannel.PreStart();

    m_VideoChannel.SetSyncChannel(m_VoiceChannel.Id());

    m_VideoChannel.StartRx();
  }
  return true;
}

bool CMediaManager::EndStart(bool bVideo)
{
  m_VoiceChannel.StartTx();
  if(bVideo)
    m_VideoChannel.StartTx();
  return true;
}

bool CMediaManager::Stop()
{
  m_VideoChannel.Stop();
  m_VoiceChannel.Stop();
  return true;
}

bool CMediaManager::SetWindowRect(int x, int y, int cx, int cy)
{

  /*
  int newCX = cx;
  int newCY = 144 * cx / 176;
  if(newCY > cy)
  {
  newCX = 176 * cy / 144;
  newCY = cy;
  }
  return m_VideoChannel.SetRect(
  x + ((cx > newCX) ? (cx - newCX) / 2 : (newCX - cx) / 2),
  y + ((cy > newCY) ? (cx - newCY) / 2 : (newCY - cy) / 2),
  newCX,
  newCY
  );
  */
  int temp;
  int margin = 0;

  if(cx >= VIDEO_CHANNEL_RECT_CX * 2 && cy >= VIDEO_CHANNEL_RECT_CY * 2)
  {
    temp = x + (cx - VIDEO_CHANNEL_RECT_CX * 2) / 2;
    x = temp > 0 ? temp : 0;

    temp = y + (cy - VIDEO_CHANNEL_RECT_CY * 2) / 2;
    y = temp > 0 ? temp : 0;
  }
  else if(cx >= VIDEO_CHANNEL_RECT_CX * 2)
  {
    temp = x + (cx / 2 - VIDEO_CHANNEL_RECT_CX) / 2;
    if(temp > 0)
    {
      x =  temp;
      margin = (cx / 2 - VIDEO_CHANNEL_RECT_CX) / 2;
    }
    else
    {
      x = 0;
      margin = 0;
    }

    temp = y + (cy - VIDEO_CHANNEL_RECT_CY) / 2;
    y = temp > 0 ? temp : 0;
  }
  else
  {
    temp = x + (cx - VIDEO_CHANNEL_RECT_CX) / 2;
    x = temp > 0 ? temp : 0;

    temp = y + (cy - VIDEO_CHANNEL_RECT_CY) / 2;
    y = temp > 0 ? temp : 0;
  }

  int remote_x = x;
  int remote_y = y;

  int remote_cx = VIDEO_CHANNEL_RECT_CX;
  int remote_cy = VIDEO_CHANNEL_RECT_CY;

  if(cx >= VIDEO_CHANNEL_RECT_CX * 2 && cy >= VIDEO_CHANNEL_RECT_CY * 2)
  {
    remote_cx *= 2;
    remote_cy *= 2;
  }

  int local_x;
  int local_y;
  int local_cx;
  int local_cy;

  if(cx >= VIDEO_CHANNEL_RECT_CX * 2 && cy >= VIDEO_CHANNEL_RECT_CY * 2)
  {
    local_x = remote_x + remote_cx / 4 * 3;
    local_y = remote_y + remote_cy / 4 * 3;

    local_cx = remote_cx / 2;
    local_cy = remote_cy / 2;
  }
  else if(cx >= VIDEO_CHANNEL_RECT_CX * 2)
  {
    local_x = remote_x + remote_cx + 2 * margin;
    local_y = remote_y;

    local_cx = remote_cx;
    local_cy = remote_cy;
  }
  else
  { // BSV
    //local_x = remote_x + remote_cx / 2;
    //local_y = remote_y + remote_cy / 2;
    local_x = remote_x + remote_cx / 4 * 3;
    local_y = remote_y + remote_cy / 4 * 3;

    local_cx = remote_cx / 4;
    local_cy = remote_cy / 4;
  }
  /*
  int remote_x, remote_y, remote_cx, remote_cy;

  remote_x = x + 4;
  remote_y = y + 4;
  remote_cx = VIDEO_CHANNEL_RECT_CX;
  remote_cy = VIDEO_CHANNEL_RECT_CY;

  int local_x, local_y, local_cx, local_cy;

  local_cx = VIDEO_CHANNEL_RECT_CX / 2;
  local_cy = VIDEO_CHANNEL_RECT_CY / 2;
  local_x = remote_x + (cx - local_cx - 8);
  local_y = remote_y + VIDEO_CHANNEL_RECT_CY / 2;
  */
  SetLocalWindow(local_x, local_y, local_cx, local_cy);
  SetRemoteWindow(remote_x, remote_y, remote_cx, remote_cy);
  return true;
}
bool CMediaManager::SetLocalWindow(int x, int y, int cx, int cy){
	m_VideoChannel.SetLocalWindow(x, y, cx, cy);
	return true;
}
bool CMediaManager::SetRemoteWindow(int x, int y, int cx, int cy){
	m_VideoChannel.SetRemoteWindow(x,y, cx,cy);
	return true;
}

bool CMediaManager::SetCamera(int Camera)
{
  return m_VideoChannel.SetCamera(Camera);
}

int CMediaManager::GetCamera()
{
  return m_VideoChannel.GetCamera();
}

bool CMediaManager::GetVoiceStatistics(
    uint32 *pInBitrate, 
    uint32 *pOutBitrate,
    uint32 *pCntRecv,
    uint32 *pCntLost,
    uint32 *pCntRecov
  )
{
  sSpiritChannelStat stats;

  if(!m_VoiceChannel.GetStatistics(&stats))
    return false;

  if (pInBitrate)
    *pInBitrate = stats.in.Bitrate;

  if (pCntRecv)
    *pCntRecv = stats.in.nRecvPackets;

  if (pCntLost)
    *pCntLost = stats.in.nLostPackets;

  if (pCntRecov)
    *pCntRecov = stats.in.nRecoveredPackets;

  if (pOutBitrate)
    *pOutBitrate = stats.out.Bitrate;

  return true;
}



bool CMediaManager::GetVideoStatistics(
                                       uint32 *pInBitrate,
                                       uint32 *pInFramerate,
                                       uint32 *pOutBitrate,
                                       uint32 *pOutFramerate,
                                       uint32 *pCntRecv,
                                       uint32 *pCntLost,
                                       uint32 *pCntRecov
                                       )
{
  sSpiritVideoChannelStat stats;

  if(!m_VideoChannel.GetRecvStats(&stats))
    return false;

  if (pInBitrate)
    *pInBitrate = stats.bitrate;

  if (pInFramerate)
    *pInFramerate = stats.framerate;

  if (pCntRecv)
    *pCntRecv = stats.nPackets;

  if (pCntLost)
    *pCntLost = stats.nLostPackets;

  if (pCntRecov)
    *pCntRecov = stats.nRecovered;

  if(!m_VideoChannel.GetSendStats(&stats))
    return false;

  if (pOutBitrate)
    *pOutBitrate = stats.bitrate;

  if (pOutFramerate)
    *pOutFramerate = stats.framerate;

  return true;
}

const char* CMediaManager::GetVoiceEncoderName(char aName[64])
{
  return m_VoiceChannel.GetEncoderName(aName);
}

const char* CMediaManager::GetVoiceDecoderName(char aName[64])
{
  return m_VoiceChannel.GetDecoderName(aName);
}

const char* CMediaManager::GetVideoEncoderName(char aName[64])
{
  return m_VideoChannel.GetEncoderName(aName);
}

const char* CMediaManager::GetVideoDecoderName(char aName[64])
{
  return m_VideoChannel.GetDecoderName(aName);
}

bool CMediaManager::CustomAecParams(void *pData, uint32 Size)
{
  if(m_pAecParams)
    delete [] m_pAecParams;

  m_pAecParams = new char[Size];
  if(!m_pAecParams)
    return false;

  memcpy(m_pAecParams, pData, Size);
  m_AecParamsSize = Size;
  return true;
}

void CMediaManager::SetFps(uint32 Fps)
{
  m_VideoChannel.SetFps(Fps);
}

bool CMediaManager::SetVsd(uint32 Vsd)
{
  if(Vsd > 2)
    return false;

  m_VideoChannel.SetVideoStreamDirection((eVideoStreamDirection)Vsd);
  return true;
}

void CMediaManager::SetBps(uint32 Bps)
{
  m_VideoChannel.SetBps(Bps);
}

void CMediaManager::SetVideoSize(uint32 Width, uint32 Height)
{
  m_VideoChannel.SetVideoSize(Width, Height);
}

void CMediaManager::SetMaxRtpFrameSize(uint32 mfrs)
{
  m_VideoChannel.SetMaxRtpFrameSize(mfrs);
}


bool CMediaManager::SetDefaultSettings(const char* pFileName)
{
  params.gui.show_voice_line[0] = 1;
  params.gui.show_voice_line[1] = 1;
  params.gui.show_voice_line[2] = 1;
  params.gui.show_voice_line[3] = 1;

  params.gui.show_video_line[0] = 1;
  params.gui.show_video_line[1] = 1;
  params.gui.show_video_line[2] = 1;
  params.gui.show_video_line[3] = 0;

  params.log.size = 1024;
  params.log.type = -1;

  params.net.local.voiceRtpPort = 8000;
  params.net.local.voiceRtcpPort = 8001;
  params.net.local.videoRtpPort = 9000;
  params.net.local.videoRtcpPort = 9001;

  params.net.remote.voiceRtpPort = 8000;
  params.net.remote.voiceRtcpPort = 8001;
  params.net.remote.videoRtpPort = 9000;
  params.net.remote.videoRtcpPort = 9001;

  params.voice.aecMode = rd_HANDSET;
  strcpy(params.voice.codec, GetVoiceCodec());
  params.voice.bAutoFecEnable = 0;

  params.video.camera = 1; // front

  params.video.vsd = (int)VSD_BOTH;
  params.video.framerate = 15;
  params.video.bitrate = 128;
  params.video.width = 176;
  params.video.height = 144;
  params.video.mrfs = 1440;

  params.video.channleNack = 1;
  params.video.channleFreezeOnLoss = 1;
  params.video.bAutoFecEnable = 0;
  params.video.fecType = 0;//(5<<8)|2;

  strcpy(params.video.codec, GetVideoCodec());

  tResult res = SPIRIT_RESULT_OK;

  Spirit::tStorageReader sr;
  if(sr.LoadFile(pFileName) != SPIRIT_RESULT_OK)
    return (res != SPIRIT_RESULT_OK);

  if(sr.GetSection("settings:logging") == SPIRIT_RESULT_OK)
  {
    std::string temp = sr.GetString("log", "none");
    if(_stricmp(temp.c_str(), "fast") == 0)
      params.log.type = eSPIRIT_LOG_FAST;
    else if(_stricmp(temp.c_str(), "cached") == 0)
      params.log.type = eSPIRIT_LOG_CACHED;
    else if(_stricmp(temp.c_str(), "sync") == 0)
      params.log.type = eSPIRIT_LOG_SYNC;
    else
      params.log.type = -1;

    params.log.size = sr.GetInt("size", params.log.size) * 1024;
  }

  if(sr.GetSection("settings:net") == SPIRIT_RESULT_OK)
  {
    params.net.local.voiceRtpPort = sr.GetInt("voiceLocalRtpPort", params.net.local.voiceRtpPort);
    params.net.local.voiceRtcpPort = sr.GetInt("voiceLocalRtcpPort", params.net.local.voiceRtcpPort);
    params.net.local.videoRtpPort = sr.GetInt("videoLocalRtpPort", params.net.local.videoRtpPort);
    params.net.local.videoRtcpPort = sr.GetInt("videoLocalRtcpPort", params.net.local.videoRtcpPort);
    params.net.remote.voiceRtpPort = sr.GetInt("voiceRemoteRtpPort", params.net.remote.voiceRtpPort);
    params.net.remote.voiceRtcpPort = sr.GetInt("voiceRemoteRtcpPort", params.net.remote.voiceRtcpPort);
    params.net.remote.videoRtpPort = sr.GetInt("videoRemoteRtpPort", params.net.remote.videoRtpPort);
    params.net.remote.videoRtcpPort = sr.GetInt("videoRemoteRtcpPort", params.net.remote.videoRtcpPort);
  }
  if(sr.GetSection("settings:voice") == SPIRIT_RESULT_OK)
  {
    params.voice.aecMode = sr.GetInt("aec", rd_HANDSET);
    strcpy(params.voice.codec, sr.GetString("codec", GetVoiceCodec()).c_str());
    params.voice.bAutoFecEnable = sr.GetInt("auto_fec", params.voice.bAutoFecEnable);
  }
  if(sr.GetSection("settings:video") == SPIRIT_RESULT_OK)
  {
    if(_stricmp(sr.GetString("camera", "rear").c_str(), "front") == 0)
      params.video.camera = 1; // front
    else
      params.video.camera = 0; // rear

    params.video.vsd = sr.GetInt("vsd", params.video.vsd);
    params.video.framerate = sr.GetInt("fps", params.video.framerate);
    params.video.bitrate = sr.GetInt("kbps", params.video.bitrate);
    params.video.width = sr.GetInt("width", params.video.width);
    params.video.height = sr.GetInt("height", params.video.height);
    params.video.mrfs = sr.GetInt("max_rtp_size", params.video.mrfs);

    params.video.channleNack = sr.GetInt("NACK", params.video.channleNack);
    params.video.channleFreezeOnLoss = sr.GetInt("video_freeze", params.video.channleFreezeOnLoss);
    params.video.bAutoFecEnable = sr.GetInt("auto_fec", params.video.bAutoFecEnable);

    {
      uint32 fec_type_a = sr.GetInt("fec_type_a", 0);
      uint32 fec_type_b = sr.GetInt("fec_type_b", 0);
      params.video.fecType = 0;
      if (fec_type_a && fec_type_b)
        params.video.fecType = (fec_type_a << 8) | fec_type_b;
    }

    strcpy(params.video.codec, sr.GetString("codec", GetVideoCodec()).c_str());
  }

  if(sr.GetSection("settings:gui") == SPIRIT_RESULT_OK)
  {
    params.gui.show_voice_line[0] = sr.GetInt("my_address", params.gui.show_voice_line[0]);
    params.gui.show_voice_line[1] = sr.GetInt("voiceEncoder", params.gui.show_voice_line[1]);
    params.gui.show_voice_line[2] = sr.GetInt("voiceDecoder", params.gui.show_voice_line[2]);
    params.gui.show_voice_line[3] = sr.GetInt("AecMode", params.gui.show_voice_line[3]);

    params.gui.show_video_line[0] = sr.GetInt("my_address", params.gui.show_video_line[0]);
    params.gui.show_video_line[1] = sr.GetInt("videoEncoder", params.gui.show_video_line[1]);
    params.gui.show_video_line[2] = sr.GetInt("videoDecoder", params.gui.show_video_line[2]);
    params.gui.show_video_line[3] = sr.GetInt("Stat", params.gui.show_video_line[3]);

    // Add voice statistics
    params.gui.show_voice_line[3] |= params.gui.show_video_line[3] << 8; // 1 byte
  }

  if(sr.GetSection("settings:params") == SPIRIT_RESULT_OK)
  {
    std::string paramName;
    std::string paramValue;
    for (int i = 0; SPIRIT_RESULT_OK == sr.GetString(i, &paramName, &paramValue); i++)
    {
      if(params.other.count >= COUNT(params.other.value))
        break;

      char *pErr = NULL;
      long val = strtol(paramValue.c_str(), &pErr, 0);

      if(!pErr || (pErr && (*pErr == '\0' || *pErr == '\n')))
      {
        params.other.name[params.other.count] = paramName;
        params.other.value[params.other.count].u32 = val;
        params.other.count++;
      }
      else
      {
        // String parameters
        // not implemented
      }
    }
  }

  return (res == SPIRIT_RESULT_OK);
}

bool CMediaManager::IsVisibleLine(int numb, int &mode, bool bIsVideo)
{
  mode = -1;

  if(numb < 0)
    return false;

  if(!bIsVideo && numb < COUNT(params.gui.show_voice_line))
  {
    mode = params.gui.show_voice_line[numb];
    return (params.gui.show_voice_line[numb] != 0);
  }

  if(bIsVideo && numb < COUNT(params.gui.show_video_line))
  {
    mode = params.gui.show_video_line[numb];
    return (params.gui.show_video_line[numb] != 0);
  }

  return false;
}

// DD_CHANGES_BEGIN
bool CMediaManager::Mute()
{
	m_VoiceChannel.Stop();
	return true;
}

bool CMediaManager::Unmute()
{
	m_VoiceChannel.StartRx();
	m_VoiceChannel.StartTx();
	return true;
}

bool CMediaManager::PauseVideo()
{
	m_VideoChannel.Stop();
	return true;
}

bool CMediaManager::ResumeVideo()
{
	m_VideoChannel.StartRx();
	m_VideoChannel.StartTx();
	return true;
}

// DD_CHANGES_END
