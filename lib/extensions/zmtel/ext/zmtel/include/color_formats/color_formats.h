
#ifndef FORMAT_CONVERTER_H_INCLUDED
#define FORMAT_CONVERTER_H_INCLUDED

#include "common/spirit_comtypes.h"
#include "video_io/video_io.h"

/*
  Spirit color format converter library
  Supports following convertions
  YUV420p --> RGB555, Same Resolution   (supports horz mirror)
  YUV420p --> RGB555, Qurter Resolution (supports horz mirror)
  YUV420p --> RGB555, 1/16 Resolution   (supports horz mirror)
  YUV420p --> RGB565, Same Resolution
  YUV420p --> RGB565, Qurter Resolution
  YUV420p --> RGB565, 1/16 Resolution

  YUV422p --> RGB565, Same Resolution
  YUV422p --> RGB565, Qurter Resolution
  YUV422p --> RGB565, 1/16 Resolution

  YUV422p --> YUV420p, Same Resolution
  YUV422p --> YUV420p, Qurter Resolution
  YUV422p --> YUV420p, 1/16 Resolution

  RGB555-->RGB555, Qurter Resolution
  RGB565-->RGB565, Qurter Resolution

  YUV420p-->YUV420p, Same Resolution   (supports horz mirror,  vert mirror)
  YUV420p-->YUV420p, Qurter Resolution (supports horz mirror,  vert mirror)
  YUV420p-->YUV420p, 1/16 Resolution

  YVU420i-->YUV420p, Same Resolution   (supports   vert mirror)
  YVU420i-->YUV420p, Qurter Resolution (supports   vert mirror)

  YV12-->YUV420p, Same Resolution     (supports horz mirror,  vert mirror)
  YV12-->YUV420p, Qurter Resolution   (supports horz mirror,  vert mirror)
  

  YUV422i-->YUV420p, Same Resolution
  YUV422i-->YUV420p, Qurter Resolution

  RGB565-->YUV420p, Same Resolution, cropping
  RGB565-->YUV420p, Quarter Resolution

  RGB555-->YUV420p, Same Resolution, cropping
  RGB555-->YUV420p, Quarter Resolution

*/

//typedef uint8* (*tFormatConverter)(uint8*,int32,uint8 *,int32,int32);
//SPIRIT_EXTERN_C tFormatConverter FormatConverterGetFuncPtr(tSpiritVideoFrameFormat sourceFmt,tSpiritVideoFrameFormat destFmt,tBool bResizeSmall);

typedef enum
{
  s_noscale      = 0,
  s_downscale_x2 = 1,
  s_downscale_x4 = 4
} teScaleFactor;

typedef tSpiritVideoFrame* (*tfnColorFormatConverter)(tSpiritVideoFrame* pInFrame,tSpiritVideoFrame* pOutFrame);
SPIRIT_EXTERN_C tfnColorFormatConverter ColorFormatConverterGetFuncPtr(tSpiritVideoFrameFormat sourceFmt,tSpiritVideoFrameFormat destFmt,teScaleFactor nScaleFactor);
SPIRIT_EXTERN_C tfnColorFormatConverter ColorFormatConverterGetFuncPtr2(tSpiritVideoFrameFormat sourceFmt,tSpiritVideoFrameFormat destFmt,teScaleFactor nScaleFactor);

//ColorFromatBufferToVideoFrame
//!!!supports negative width and height!!!
//pFrame - continuous video frame buffer 
//tSpiritVideoFrame - object to initialize
//int32 width,int32 height - frame size
//fmt - frame  color format
//returns Null if error or pFrame ptr 
SPIRIT_EXTERN_C tSpiritVideoFrame* ColorFromat_Buffer2VideoFrame(tSpiritVideoFrame /*IN/OUT*/* pFrame,uint8 /*IN*/*pFrameBuffer, int32 width,int32 height,tSpiritVideoFrameFormat fmt);

//ColorFromat_GetFrameBufferSize
//!!!supports negative width and height!!!
//returns frame buffer size (bytes).
//in case of error returns 0
SPIRIT_EXTERN_C uint32 ColorFromat_GetFrameBufferSize(tSpiritVideoFrameFormat fmt,int32 width,int32 height);

//!!!supports negative width
SPIRIT_EXTERN_C uint32 ColorFormat_GetStride(tSpiritVideoFrameFormat fmt,int32 width,int32 plane_no);

//returns plane size stride[i]*plane_height
SPIRIT_EXTERN_C uint32 ColorFormats_GetPlaneSize(tSpiritVideoFrame *pFrame,int32 plane_no);

//ColorFormats_VideoFrameSize2Height ColorFormats_VideoFrameSize2Width
//converts teVideoFrameSize to width and height (pixels).
SPIRIT_EXTERN_C int32 ColorFormats_VideoFrameSize2Height(teVideoFrameSize eVideoSize);
SPIRIT_EXTERN_C int32 ColorFormats_VideoFrameSize2Width(teVideoFrameSize eVideoSize);


//verticaly flips video frame
SPIRIT_EXTERN_C tResult ColorFormats_FlipVertical(tSpiritVideoFrame *pFrame);

//rotates YUV420 frame
SPIRIT_EXTERN_C tSpiritVideoFrame* ColorFormats_Rotate90(tSpiritVideoFrame *pInFrame,tSpiritVideoFrame *pOutFrame);

//return offset in bytes to the specified pixel from the first byte of the plane
//!!!supports negative width and height!!!
SPIRIT_EXTERN_C int32 ColorFormats_GetPixelOffsetBytes(tSpiritVideoFrame*pFrame ,int32 plane_no,int32 X,int32 Y);

typedef struct tagVideoMarkDesc {
  uint32 R,G,B; //color of the mark 
  uint32 x,y; //top left corner of the mark
  uint32 w,h; //width and height of the mark
} tVideoMarkDesc;

//!!!supports negative width and height!!!
SPIRIT_EXTERN_C int32 ColorFormats_AddMarkToFrame(tSpiritVideoFrame*pDestFrame,tVideoMarkDesc *pMarkDesc);
#endif
