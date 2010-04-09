#ifndef __VIDEO_CHANNEL__H__
#define __VIDEO_CHANNEL__H__

#include "Channel.h"

// Default ports settings
#define VIDEO_CHANNEL_RTP_LOCAL_PORT      (9000)
#define VIDEO_CHANNEL_RTCP_LOCAL_PORT     (VIDEO_CHANNEL_RTP_LOCAL_PORT + 1)

#define VIDEO_CHANNEL_RTP_REMOTE_PORT     (9000)
#define VIDEO_CHANNEL_RTCP_REMOTE_PORT    (VIDEO_CHANNEL_RTP_REMOTE_PORT + 1)

// Default video rect
#define VIDEO_CHANNEL_RECT_X      0
#define VIDEO_CHANNEL_RECT_Y      0
#define VIDEO_CHANNEL_RECT_CX     176
#define VIDEO_CHANNEL_RECT_CY     144

typedef enum _eVideoStreamDirection
{
  VSD_BOTH = 0,
  VSD_SEND = 1,
  VSD_RECV = 2
}eVideoStreamDirection;

class CVideoChannel : public CChannel
{
  static void BuildDefaultVideoRtpMap(tCodecPayload aRtpMap[SPIRIT_MAX_CODEC_MAP_SIZE]);

  tVideoCodecDescSettings   *m_pCodec;
  sSpiritVideoRect          m_Rect;
  static sSpiritVideoRect   m_rectPreview;
  uint32                    m_VoiceChannelId; // for sync
  uint32                    m_Camera;

  eVideoStreamDirection     m_Vsd;
  //tVideoCodecDescSettings vcdsEncoder;
  //tVideoCodecDescSettings vcdsDecoder;

  uint32 m_Bitrate;
  uint32 m_width;
  uint32 m_height;
  uint32 m_frame_rate_num;
  uint32 m_frame_rate_denom;
  uint32 m_max_rtp_frame_size;

  uint32 m_channelNack;
  uint32 m_channelFreezeOnLoss;
  uint32 m_fecType;

public:
  CVideoChannel(void);
  ~CVideoChannel(void);

  static const tVideoCodecDescSettings* GetCodecList();


  // Stop video streaming
  bool Stop();

  // Prepare Start
  bool PreStart();
  // Start Tx
  bool StartTx();
  // Start Rx
  bool StartRx();

  // Set output window rect with preview
  bool SetRect(int x, int y, int cx, int cy);
  //
  bool SetLocalWindow(int x, int y, int cx, int cy);
  bool SetRemoteWindow(int x, int y, int cx, int cy);

  // Sync voice and video channel
  bool SetSyncChannel(uint32 Id);

  // Set encoder
  bool SetCodec(const tVideoCodecDescSettings *pCodec);
  // Get encoder
  const tVideoCodecDescSettings* GetCodec();

  // Set camera number (front or rear)
  bool SetCamera(int Camera);
  // Get camera number
  int GetCamera();

  // Get statistics
  bool GetRecvStats(sSpiritVideoChannelStat *pStats);
  bool GetSendStats(sSpiritVideoChannelStat *pStats);

  // Get encoder/decoder settings
  const char* GetEncoderName(char aName[64]);
  const char* GetDecoderName(char aName[64]);

  void SetFps(uint32 Fps);
  void SetVideoStreamDirection(eVideoStreamDirection Vsd);
  void SetBps(uint32 Bps);
  void SetVideoSize(uint32 Width, uint32 Height);
  void SetMaxRtpFrameSize(uint32 mfrs);

  bool SetChannelNack(uint32 val);
  bool SetChannelFreezeOnLoss(uint32 val);
  bool SetFecType(uint32 val);
};

#endif // __VIDEO_CHANNEL__H__

