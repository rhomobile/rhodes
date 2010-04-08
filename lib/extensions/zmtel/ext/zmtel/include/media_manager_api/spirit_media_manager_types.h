/******************************************************************************
  FILE: spirit_media_manager_types.h
  DESCRIPTION: N/A yet

                          This file is a part of Spirit Voice/Video Engine SDK.

  Copyright SPIRIT DSP 2008.                                 www.spiritdsp.com
******************************************************************************/
#ifndef _SPIRIT_MEDIA_MANAGER_TYPES_H_
#define _SPIRIT_MEDIA_MANAGER_TYPES_H_

#include "common/spirit_comtypes.h"

typedef enum _teSpiritRDMode
{
  rd_DISABLED = 0,
  rd_HANDSFREE,
  rd_HANDSET,
  rd_HEADSET,
  rd_HEADPHONES,
  rd_CUSTOM,
} teSpiritRDMode;

typedef enum _teSpiritRDTracingMode
{
  rd_TRACE_DISABLED,
  rd_TRACE_RDC_INPUTS,
  rd_TRACE_EC_INPUTS
} teSpiritRDTracingMode;

typedef enum _eSpiritSoundIOFreqs
{
  sio_8000Hz = 0,
  sio_11025Hz,
  sio_16000Hz,
  sio_22050Hz,
  sio_44100Hz,
  sio_48000Hz
} eSpiritSoundIOFreqs;

// video
typedef struct _tSize {
  uint32 w;
  uint32 h;
} tSize;

typedef struct tagSpiritVideoRect {
  uint32 X, Y; //Specifies the top-left corner of the destination rectangle
  uint32 Width, Height; //Specifies the width and the height of the destination rectangle.
} sSpiritVideoRect;

typedef struct tagSpiritVideoViewParamsEx
{
  //specifies the decoder/capture handler
  void *pHandler;
  sSpiritVideoRect Rect;

} sSpiritVideoViewParamsEx;

typedef struct tagSpiritChannelStat
{
  struct
  {
    uint32  Jitter;
    uint32  Bitrate;
    uint32  nRecvPackets;
    uint32  nLostPackets;
    uint32  nRecoveredPackets;
    uint32  SampleRateNum;  // for video one sample-one frame
    uint32  SampleRateDenom;
  }in;

  struct
  {
    uint32  Bitrate;
    uint32  nSendPackets;
    uint32  SampleRateNum;  // for video one sample-one frame
    uint32  SampleRateDenom;
  }out;
} sSpiritChannelStat;

typedef struct tagSpiritVideoChannelStat
{
  uint32 nChannelID;
  uint32 bitrate;      // bits per second
  uint32 framerate;    // frames per 1024 seconds
  uint32 nPackets;     // number of processed packets
  uint32 nDupPackets;  // number of duplicate packets
  uint32 nLostPackets; // number of lost packets
  uint32 nRecovered;   // number of recovered packets

} sSpiritVideoChannelStat;

typedef struct tagSpiritVideoViewHide {
  uint32 nChannelID;
  tBool bHide;

} sSpiritVideoViewHide;

/////////////////////////////////////////////////
//tSpiritVideoCodecInfo is used with the Video Engine parameter
//  to get hardware video codec virtual function table pointer (tSpiritVideoCodecDescription)
//  nSDPPayload and strSDPName members should be initialized with necessary values.
typedef struct tagSpiritVideoCodecInfo {
  uint8 nSDPPayload;  //[IN] SDP payload ID
  const char* strSDPName; //[IN] SDP payload name
  const void* pSpiritVideoCodec; //[OUT] tVideoCodecDescription pointer or NULL if there is no hardware codec

} tSpiritVideoCodecInfo;

/////////////////////////////////////////////////
//tMuteCaptureParams  is used with the SVEP_MUTE_CAPTURED_VIDEO parameter
//  to mute local video and send specified bitmap to the other side.
//Warning: this structure is used only on HTC Hermes devices
typedef struct tagMuteCaptureParams
{
  tBool bEnableMuteMode; // video mute: 1 - enable, 0 - disable
  void *pBitmapPath; //path to the bitmap image that would be transmitted instead of local video

} tMuteCaptureParams;

typedef enum _tSpiritVideoFrameFormat
{
  SPIRIT_VIDEO_FORMAT_YUV420    = 0,  //planar
  SPIRIT_VIDEO_FORMAT_YUV422    = 1,  //planar
  SPIRIT_VIDEO_FORMAT_YUV444    = 2,  //planar
  SPIRIT_VIDEO_FORMAT_YUV410    = 3,  //planar
  SPIRIT_VIDEO_FORMAT_YUV411    = 4,  //planar
  SPIRIT_VIDEO_FORMAT_BGR24     = 5,  //interleaved
  SPIRIT_VIDEO_FORMAT_RGB24     = 6,  //interleaved
  SPIRIT_VIDEO_FORMAT_RGB32     = 7,  //interleaved
  SPIRIT_VIDEO_FORMAT_RGB565    = 8,  //interleaved
  SPIRIT_VIDEO_FORMAT_RGB555    = 9,  //interleaved
  SPIRIT_VIDEO_FORMAT_GRAYSCALE = 10, //planar
  SPIRIT_VIDEO_FORMAT_VYUYi     = 11, //interleaved
  SPIRIT_VIDEO_FORMAT_YUYVi     = 12, //interleaved
  SPIRIT_VIDEO_FORMAT_UYVYi     = 13, //interleaved
  SPIRIT_VIDEO_FORMAT_YV12      = 14, //planar YVU 420 Y plane, V plane, U plane
  SPIRIT_VIDEO_FORMAT_VY1UY2i   = 15, //same as VYUYi but Y are swapped
  SPIRIT_VIDEO_FORMAT_YVU420i   = 16  //Y (wXh) plane and interleaved UV plane size of (w*h/2) (U - w/2*h/2, and V - w/2*h2) format UVUVUV...
} tSpiritVideoFrameFormat;

typedef struct tagSpiritVideoFrame {
  uint8* pPlane[3];
  uint32 stride[3];
  int32 width,height;
  tSpiritVideoFrameFormat fmt;
  uint32 timestamp;
} tSpiritVideoFrame;

/////////////////////////////////////////////////
//tSpiritVideoMute is used with SVEP_MUTE_VIDEO parameter
//  to mute local video and replace captured video
//  with specified background color and bitmap
//  Video engine fills captured image with specified background color
//    and then places bitmap in the center of the of the generated frame
typedef struct tagSpiritVideoMute {

  uint32 bkg_color; // background color: bkg_color = ((Red&0xFF)<<16) | ((Green&0xFF) <<8) | (Blue&0xFF)
  tSpiritVideoFrame *pImage;   // pImage should point to valid tSpiritVideoFrame object or can be set to 0
                               // NULL pImage pointer means that there is no bitmap to show instead of captured video
                               // in this video frames will be filled with bkg_color
} tSpiritVideoMute;

typedef enum _eViewportZOrder {
  viewport_order_bottom,
  viewport_order_top
} eViewportZOrder;

typedef enum _teSpiritPreviewMode {
  PREVIEW_MODE_CAMERA_VIDEO = 0,   //shows all encoded frames
  PREVIEW_MODE_MIXED_VIDEO  = 1,   //shows mixed video frame
  PREVIEW_MODE_CAPTURE      = 2    //shows all captured frames
} teSpiritPreviewMode;

#endif // _SPIRIT_MEDIA_MANAGER_TYPES_H_
