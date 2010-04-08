#ifndef __MEDIA_MANAGER__H__
#define __MEDIA_MANAGER__H__

#include "spirit_headers.h"

#include "VoiceChannel.h"
#include "VideoChannel.h"

#include <string>


#define MEDIA_MANAGER_INVALID_CHANNEL     (-1)

class CMediaManager
{
private:
  tMMHandle         m_hMM;
  teSpiritRDMode    m_eAECMode;
  static char*      m_paAECModeList[16];

  CVoiceChannel     m_VoiceChannel;
  CVideoChannel     m_VideoChannel;

  uint32            m_AecParamsSize;
  void              *m_pAecParams;
  tSpiritLog *pLog;

  struct tParams
  {
    struct tLog
    {
      int type; // -1 = disable
      int size;
    }log;
    struct tNet
    {
      struct tPort
      {
        int voiceRtpPort;
        int voiceRtcpPort;
        int videoRtpPort;
        int videoRtcpPort;
      }local, remote;

    }net;
    struct tVoice
    {
      int aecMode;
      char codec[32];
      uint32 bAutoFecEnable;
    }voice;
    struct tVideo
    {
      int camera;
      int vsd;
      int framerate;
      int bitrate;
      int width;
      int height;
      int mrfs; 

      uint32 channleNack;
      uint32 channleFreezeOnLoss;
      uint32 bAutoFecEnable;
      uint32 fecType;

      char codec[32];

    }video;
    struct tOther
    {
      int count;
      std::string name[20];
      tVariant value[20];
    }other;
    struct tGui
    {
      int show_voice_line[4];
      int show_video_line[4];
    }gui;
  }params;

public:
  CMediaManager(void);
  ~CMediaManager(void);

  bool Create();
  bool Destroy();

  // AEC Settings
  static const char** GetAECModeList();

  bool SetAECMode(const char* pMode);
  const char* GetAECMode();

  bool SetVoiceCodec(const char* pCodec);
  const char* GetVoiceCodec();
  bool SetVideoCodec(const char* pCodec);
  const char* GetVideoCodec();

  bool SetLocalIp(const char* pIp);
  // With rtcp
  bool SetLocalPortRtpVoice(unsigned short Port);
  // With rtcp
  bool SetLocalPortRtpVideo(unsigned short Port);

  bool SetRemoteIp(const char* pIp);
  // With rtcp
  bool SetRemotePortRtpVoice(unsigned short Port);
  // With rtcp
  bool SetRemotePortRtpVideo(unsigned short Port);

  // Start media streaming
  bool Start(bool bVideo = false);
  // Stop media streaming
  bool Stop();

  // Separating start
  bool BeginStart(bool bVideo);
  bool EndStart(bool bVideo);

  // Set video rect (with preview)
  bool SetWindowRect(int x, int y, int cx, int cy);
  bool SetRemoteWindow(int x, int y, int cx, int cy);
  bool SetLocalWindow(int x, int y, int cx, int cy);

  bool SetCamera(int Camera);
  int GetCamera();


  bool GetVoiceStatistics(
    uint32 *pInBitrate, 
    uint32 *pOutBitrate,
    uint32 *pCntRecv,
    uint32 *pCntLost,
    uint32 *pCntRecov
  );

  bool GetVideoStatistics(
    uint32 *pInBitrate, 
    uint32 *pInFramerate,
    uint32 *pOutBitrate,
    uint32 *pOutFramerate,
    uint32 *pCntRecv,
    uint32 *pCntLost,
    uint32 *pCntRecov
  );

  const char* GetVoiceEncoderName(char aName[64]);
  const char* GetVoiceDecoderName(char aName[64]);
  const char* GetVideoEncoderName(char aName[64]);
  const char* GetVideoDecoderName(char aName[64]);

  //BSV: Must be static
  bool CustomAecParams(void *pData, uint32 Size);

  void SetFps(uint32 Fps);
  bool SetVsd(uint32 Vsd);
  void SetBps(uint32 Bps);
  void SetVideoSize(uint32 Width, uint32 Height);
  void SetMaxRtpFrameSize(uint32 mfrs);

  bool SetDefaultSettings(const char* pFileName);
  bool IsVisibleLine(int numb, int &mode, bool bIsVideo);

  // DD_CHANGES_BEGIN
  bool Mute();
  bool Unmute();
  
  bool PauseVideo();
  bool ResumeVideo();
  // DD_CHANGES_END
};

#endif // __MEDIA_MANAGER__H__

