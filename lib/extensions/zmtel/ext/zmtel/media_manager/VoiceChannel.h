#ifndef __VOICE_CHANNEL__H__
#define __VOICE_CHANNEL__H__

#include "Channel.h"

// Default port settings
#define VOICE_CHANNEL_RTP_LOCAL_PORT    (8000)
#define VOICE_CHANNEL_RTCP_LOCAL_PORT   (VOICE_CHANNEL_RTP_LOCAL_PORT + 1)

#define VOICE_CHANNEL_RTP_REMOTE_PORT    (8000)
#define VOICE_CHANNEL_RTCP_REMOTE_PORT   (VOICE_CHANNEL_RTP_REMOTE_PORT + 1)

class CVoiceChannel : public CChannel
{
  static void BuildDefaultVoiceRtpMap(tCodecPayload aRtpMap[SPIRIT_MAX_CODEC_MAP_SIZE]);

  tAudioCodecDescSettings *m_pCodec;

  //tAudioCodecDescSettings vcdsEncoder;
  //tAudioCodecDescSettings vcdsDecoder;
public:
  CVoiceChannel(void);
  ~CVoiceChannel(void);

  static const tAudioCodecDescSettings* GetCodecList();

  // Start voice streaming
  bool Start();
  // Stop voice streaming
  bool Stop();

  // Prepare Start
  bool PreStart();
  // Start Tx
  bool StartTx();
  // Start Rx
  bool StartRx();

  // Get statistics
  bool GetStatistics(sSpiritChannelStat *pStats);

  // Set encoder
  bool SetCodec(const tAudioCodecDescSettings *pCodec);
  // Get encoder
  const tAudioCodecDescSettings* GetCodec();

  // Get encoder/decoder settings
  const char* GetEncoderName(char aName[64]);
  const char* GetDecoderName(char aName[64]);
};

#endif // __VOICE_CHANNEL__H__
