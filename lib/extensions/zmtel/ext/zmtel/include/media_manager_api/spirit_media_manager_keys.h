/******************************************************************************
  FILE: spirit_media_manager_keys.h
  DESCRIPTION: N/A yet

                          This file is a part of Spirit Voice/Video Engine SDK.

  Copyright SPIRIT DSP 2008.                                 www.spiritdsp.com
******************************************************************************/
#ifndef _SPIRIT_MEDIA_MANAGER_KEYS_H_
#define _SPIRIT_MEDIA_MANAGER_KEYS_H_

///////////////////////////////////////////////////////////////////////
//"ViE:Camera" - changes/returns video camera ID used to capture video
#define SVEP_CAMERA "ViE:Camera"

//win32 only
//"ViE:ParentHWND" - sets parent window for viewport
//The same parameter should be used to set parent window
//for local camera video using SetParam function
//and  for the channel video using ChannelSetParam function
//Setting parent window cause video to be clipped (device dependent feature)
#define SVEP_VIEWPORT_HWND "ViE:ParentHWND"

//////////////////////////////////////////////////////////////
//"ViE:Preview" - enables/disables captured video preview
#define SVEP_ENABLE_PREVIEW  "ViE:Preview"

//////////////////////////////////////////////////////////////
//"ViE:HidePreview" - hides decoded video view
#define SVEP_HIDE_PREVIEW "ViE:HidePreview"

//////////////////////////////////////////////////////////////
//"ViE:MirrorPreview" - enables/disables horizontal mirror
//    captured video preview mode
// 1 => mirror video; 0 => otherwise
#define SVEP_MIRROR_PREVIEW "ViE:MirrorPreview"

//////////////////////////////////////////////////////////////
//Specifies captured video preview overlay color key
//This parameter is used only if overlay mode is supported
//by the underlying hrdware
//val.u32 specifies the RGB color value
#define SVEP_PREVIEW_OVERLAY_COLOR "ViE:PreviewOverlayColor"

//////////////////////////////////////////////////////////////
//HW specific parameter (Intel Olo platform)
//Used to enable HW scaling on Linux Olo platforms (uses direct FB library to render video)
//should be used with "ViE:SourceRect" parameter
//val.pData should be a valid pointer to the sSpiritVideoRect object
//Note that changing layer rect takes effect on to the all viewports created by the engine
//SetParam function
#define SVEP_SET_LAYER_RECT  "ViE:LayerRect"

//////////////////////////////////////////////////////////////
//HW specific parameter (Intel Olo platform)
//Used to enable HW scaling on Linux Olo platforms (uses direct FB library to render video)
//should be used with "ViE:LayerRect" parameter
//val.pData should be a valid pointer to the sSpiritVideoRect object
#define SVEP_SOURCE_RECT "ViE:SourceRect"

//////////////////////////////////////////////////////////////
//HW specific parameter (Intel Olo platform)
//changes z-order of the local camera video preview window
//val.u32 should be one of the values from the eViewportZOrder enumeration
//SetParam function
#define SVEP_PREVIEW_Z_ORDER  "ViE:PreviewZOrder"

//////////////////////////////////////////////////////////////
//HW specific parameter (Intel Olo platform)
//changes z-order of the video window
//val.u32 should be one of the values from the eViewportZOrder enumeration
//SetChannelParam function
#define SVEP_CHANNEL_Z_ORDER  "ViE:ChannelZOrder"


//////////////////////////////////////////////////////////////
//Enables or disables mute captured video mode.
//!!!this paramter works only on HTC Hermes device!!!
//  This parameter is used only if mute mode is supported
//  by the underlying capture middleware
//val.pData should be a valid pointer to the tMuteCaptureParams structure.
#define SVEP_MUTE_CAPTURED_VIDEO "ViE:MuteCapturedVideo"

//////////////////////////////////////////////////////////////
//Enables or disables mute captured video mode.
//In this mode captured video replaced with specified frame on the specified background
//val.pData should be a valid pointer to the tSpiritVideoMute structure or NULL
//in case of NULL val.pData pointer mute mode is disabled
//this parameter can be used only to enable or disable mute mode (SetParam functions)
//to get mute state please use "ViE:GetVideoMuteState" parameter
#define SVEP_MUTE_VIDEO "ViE:MuteVideo"

//////////////////////////////////////////////////////////////
//Gets current mute state can be used only with GetParam function
//val.u32 set to 1 in case of enabled mute and 0 in case of normal video processing
//
#define SVEP_GET_VIDEO_MUTE_STATE "ViE:GetVideoMuteState"

//////////////////////////////////////////////////////////////
//Sets video mixer background fill color
//val.u32 should be set to RGB color value in the following format
//  (R&0xFF<<16)|((G&0xFF)<<8) | (B&0xFF)
#define SVEP_MIXER_BACKGROUND_COLOR "ViE:MixerBkgColor"

//////////////////////////////////////////////////////////////
//Sets video mixer background picture
//this picture will be added to the mixed frame.
//val.pData should be set to valid tSpiritVideoFrame pointer
#define SVEP_MIXER_BACKGROUND_PICTURE "ViE:MixerBkgPicture"

//////////////////////////////////////////////////////////////
//Sets video mixer background picture
//this picture will be added to the mixed frame.
//val.u32 should be set to required mode (PREVIEW_MODE_CAMERA_VIDEO or PREVIEW_MODE_MIXED_VIDEO)
#define SVEP_PREVIEW_MODE "ViE:PreviewMode"

//////////////////////////////////////////////////////////////
#define SVEP_CHANNEL_RECV_STAT "ViE:ChannelRecvStat"

#define SVEP_CHANNEL_SEND_STAT "ViE:ChannelSendStat"

//returns the input/output statistics: actual encoder bitrate and capture frame rate values
//variant.pData should be a valid pointer to sSpiritChannelStat structure
#define SVEP_CHANNEL_STAT "VxE:ChannelStat"

//////////////////////////////////////////////////////////////
//"ViE:Output" - sets decoded video view parameters or returns current values
#define SVEP_CHANNEL_RECT  "ViE:ChannelVideoRect"

//////////////////////////////////////////////////////////////
//"ViE:HideOutput" - hides/shows decoded video view
//val.u32 should be set to 1 to hide video, 0 otherwise
#define SVEP_CHANNEL_HIDE  "ViE:ChannelVideoHide"

//////////////////////////////////////////////////////////////
//ViE:ChannelDisableSync - disables video syncchronization
//val.u32 should be set to 1 to disable audio/video sync
#define SVEP_CHANNEL_DISABLE_SYNC "ViE:ChannelDisableSync"


//////////////////////////////////////////////////////////////
//Specifies uncompressed video view (remote video) overlay color key
//This parameter is used only if overlay mode is supported
//by the underlying hrdware
//val.u32 specifies the RGB color value
#define SVEP_CHANNEL_VIDEO_OVERLAY_COLOR "ViE:ChannelVideoOverlayColor"

//////////////////////////////////////////////////////////////
//Sets (or gets) channel state
//val.u32 should be one of the values from the teSpiritVideoChannelState enumeration.
//except of vcs_SOLO (not supported fro video channels)
//vcs_MIX - means the both Rx and Tx are enabled
//vcs_SNDONLY - means that only Tx is enabled
//vcs_RCVONLY - means that only Rx is enabled
//vcs_HOLD - means tht both Rx and Tx are disabled
#define SVEP_VIDEO_CHANNEL_STATE "ViE:ChannelState"

//////////////////////////////////////////////////////////////
//Set channel idetificator (id) for logging
//val.u32 id of channel
#define SVEP_CHANNEL_SET_ID "ViE:ChannelID"


//////////////////////////////////////////////////////////////
//Gets hardware video codec virtual function table pointer (tSpiritVideoCodecDescription)
//val.pData should be a valid tSpiritVideoCodecInfo pointer
//    strSDPName and  nSDPPayload should be initialized with the required values
//return: fills pSpiritVideoCodec with the valid virtual function table pointer or NULL
//    if the specified hardware codec doesnot exist
#define SVEP_GET_HW_CODEC "ViE:HardwareCodec"

//////////////////////////////////////////////////////////////
//Sets camera brightness
//val.u32 should specify the required brightness value from the [1..5] range
//
#define SVEP_BRIGHTNESS "ViE:Brightness"

//////////////////////////////////////////////////////////////
//Enables/disables local camera video preview for the HTC Hermes device
//This parameter is implemented only on HTC Hermes devices
//value.pData should be a valid pointer to the VT_DISPLAYAREA structure
#define SVEP_HERMES_PREVIEW "ViE:HermesPreview"

//////////////////////////////////////////////////////////////
//Enables/disables remote video view for the HTC Hermes device
//This parameter is implemented only on HTC Hermes devices
//value.pData should be a valid pointer to the VT_DISPLAYAREA structure
#define SVEP_CHANNEL_HERMES_OUTPUT  "ViE:HermesOutput"

/////////////////////////////////////////////////////////////
//Enables/disables channel synchronization marks
//1 - enables marks
//0 - disables marks
#define SVEP_CHANNEL_MARKS "ViE:ChannelEnableMarks"

/////////////////////////////////////////////////////////////
//Starts HW video decoder without initializing rendering subsystem
//This parameter should be used only on systems with integrated HW decoder
//and rendering subsystem instead of SVEP_CHANNEL_RECT param
//value.pData should be valid tSize object pointer.
//tSize specifies the resolution of the decompressed video returned by decoder
#define SVEP_CHANNEL_DECODER_START "ViE:ChannelDecoderStart"

/////////////////////////////////////////////////////////////
//Starts HW video decoder without initializing rendering subsystem
//This parameter should be used only on systems with integrated HW decoder
//and rendering subsystem instead of SVEP_CHANNEL_RECT param with NULL value.pData parameter
//Value parameter is not used
#define SVEP_CHANNEL_DECODER_STOP "ViE:ChannelDecoderStop"


/////////////////////////////////////////////////////////////
//Enables or desables RTCP FIR packets sending in case if input video packet lost
//val.u32 should be 1 to enable FIR sending and 0 to disable this feature.
#define SVEP_CHANNEL_FIR "ViE:ChannelFIR"

/////////////////////////////////////////////////////////////
//Enables or desables RTCP NACK packets sending in case if input video packet lost
//val.u32 should be 1 to enable FIR sending and 0 to disable this feature.
#define SVEP_CHANNEL_NACK "ViE:ChannelNACK"

/////////////////////////////////////////////////////////////
//Enables or disables freeze decoder on packet loss mode
//In this mode if the packet loss detected decoder stops 
//  input packets processing until the reference frame received
//val.u32 should be 1 to enable FIRdecoder freezing and 0 to disable this feature.
#define SVEP_CHANNEL_FREEZE_ON_LOSS "ViE:ChannelFreezeOnLoss"

/////////////////////////////////////////////////////////////
// Spirit Voice Engine SetParam keys:

/////////////////////////////////////////////////////////////
//Acceptable values:
//rd_DISABLED - speech enhancement is disabled, only wave io is active;
//rd_HANDSFREE - handsfree mode;  rd_HANDSET - handset mode;
//rd_HEADSET - headset mode;
//rd_HEADPHONES - headphones mode;
//rd_CUSTOM -  isn't supported in this control
#define SVEP_RD_MODE                      "VoE:RD:mode"

/////////////////////////////////////////////////////////////
//The parameter is reserved. Do not use.
#define SVEP_RD_CUSTOM                    "VoE:RD:Custom"

/////////////////////////////////////////////////////////////
//Enables audio PCM tracing.
//Value.i should contain pcm tracing buffer length in seconds.
//There will be created three PCM wav files: se_ref.wav, se_echo.wav se_res.wav.
//RDC inputs and output will be dumped to these files when RDC is destroyed or tracing is disabled.
#define SVEP_RD_TRACE_RDC                 "VoE:RD:trace_rdc"

/////////////////////////////////////////////////////////////
//Disables audio PCM tracing. If it was enabled dumps RDC data to pcm tracing files and closes them
#define SVEP_RD_PCM_TRACING_DISABLE       "VoE:RD:PCMTracingDisable"

/////////////////////////////////////////////////////////////
// Sets threshold to reset Microphone event.
//This setting makes Real Duplex to set Microphone event only when there's more data than Value.u32(in 8kHz samples)
#define SVEP_RD_CAPTURE_BUFFER_THRESHOLD  "VoE:RD:CaptureBufferThreshold"

/////////////////////////////////////////////////////////////
// Sets Render block size. This setting makes Real Duplex to get data for speaker playback by blocks of Value.u32 (in 8kHz samples)
#define SVEP_RD_RENDER_BLOCK_SIZE         "VoE:RD:RenderBlockSize"

/////////////////////////////////////////////////////////////
#define SVEP_CHANNEL_SAMPLE_RATE      "VoE:Mixer:ChannelSampleRate"
#define SVEP_MIXER_RD_SAMPLE_RATE     "VoE:Mixer:RealDuplexSampleRate"
#define SVEP_RD_USER_SAMPLE_RATE      "VoE:RD:UserSampleRate"
#define SVEP_PTIME                    "VoE:ptime"
#define SVEP_DTX                      "VoE:DTX"
#define SVEP_PROC_BUF_SIZE            "VoE:ProcBufSize"
#define SVEP_CONTROL_TX_PACKET_DELAY  "VoE:ControlTxPacketDelay"
#define SVEP_RTP_KEEP_ALIVE_MODE      "RTP:KeepAliveMode"
#define SVEP_RTP_KEEP_ALIVE_TIME      "RTP:KeepAliveTime"
#define SVEP_DTMF_MAGNITUDE           "DTMF:Magnitude"
#define SVEP_DTMF_DURATION            "DTMF:Duration"
#define SVEP_DTMF_PAUSE               "DTMF:Pause"
#define SVEP_DTMF_PAYLOAD             "DTMF:Payload"
#define SVEP_DTMF_TYPE                "DTMF:Type"
#define SVEP_DTMF_SPEAKER             "DTMF:Speaker"

//-----------------------------------------------------------------------------

#define SVEP_CHANNEL_TOS              "TR:ToS"
#define SVEP_CHANNEL_DSCP             "TR:DSCP"

//-----------------------------------------------------------------------------

// enable/disable hi system load detector and protection
#define SVEP_HISYSLOAD_CTRL           "VoE:HiSysLoadCtrl"

// read only. returns non zero if protection is currently working
#define SVEP_HISYSLOAD_STATUS         "VoE:HiSysLoadStatus"

//-----------------------------------------------------------------------------

// enable/disable Auto protection with FEC for particular voice/video channel
#define SVEP_CHANNEL_AUTO_FEC_ENABLE  "FEC:Auto"

// set type of the FEC protection
// 16 bit:
//  HB <number of informational packets>, maximum 15
//  LB <number of FEC packets>, maximum 15
#define SVEP_CHANNEL_FEC_TYPE         "FEC:Type"

// set specific payload value for FEC processor
// (affects all channels)
#define SVEP_FEC_PAYLOAD              "FEC:Payload"

//-----------------------------------------------------------------------------

#endif // _SPIRIT_MEDIA_MANAGER_PARAM_KEYS_H_
