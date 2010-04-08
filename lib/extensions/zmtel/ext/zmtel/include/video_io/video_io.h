
#ifndef SPIRIT_VIDEO_IO_H_INCLUDED
#define SPIRIT_VIDEO_IO_H_INCLUDED

#include "common/spirit_comtypes.h"
#include "video_io/video_types.h"

#define HTC_CAMERA_ID_FRONT 2
#define HTC_CAMERA_ID_REAR  1


typedef tResult (*tfnSpiritVideoCodecAlloc) (void *pObject, void **ppBuffer,size_t n);
typedef tResult (*tfnSpiritVideoCodecFree) (void *pObject, void  *pBuffer,size_t n);

typedef struct tagVideoCodecAllocatorDesc {
  tfnSpiritVideoCodecAlloc Alloc;
  tfnSpiritVideoCodecFree Free;
  void *pOwner;
}tVideoCodecAllocatorDesc ;

//
//"ViE:VideoCodecAllocator" used to set custom memory allocator.
//DaVinci TI codec needs memory to ba allocated by the internal memory alllocation routines.
//
//val.pData should be a valid pointer to the tVideoCodecAllocatorDesc structure
#define SVEP_VIDEOCODEC_ALLOCATOR "ViE:VideoCodecAllocator"

//
//returns capture rate
//This parameter is read only
//val.i32 can contain the following values
//0 - means infinite rate
//-1 - means the rate should be auto detected
//>0 - mean delay between 2 captured frames
#define SVEP_CAPTURE_RATE "ViE:CaptureRate"

//sets Frame Rate Manager pointer
//val.pData points to tFrameratemanager instance
#define SVEP_SET_RATE_MGR "ViE:SetRateMgr"

typedef struct tagVideoDriverHTCParams{
  //used to select front or rear camera
  //can be HTC_CAMERA_ID_REAR or HTC_CAMERA_ID_FRONT
  int32 cam_no;
}tVideoDriverHTCParams;
typedef struct tagVideoDriverDM642Params{
	int32 mHeapSegId; //used by the driver for memorty allocation
} tVideoDriverDM642Params;

typedef struct tagVideoStreamParams {
	teVideoFrameSize frameSize;
	//this is a placeholder for device dependent video capture driver settings
	//for DM642 samsung board this should be set to the valid tVideoDriverDM642Params structure pointer
  union {
    tVideoDriverDM642Params *pDM642;
    tVideoDriverHTCParams *pHTC;
  } DevParams;
} tVideoStreamParams;


typedef tResult (*tfnVideoFrameCaptured) (void *pModule,tSpiritVideoFrame* pFrame);

typedef struct tagVideoCaptureParams
{
	//user callback pointer. Video capture module returns I420 planar video frame.
	tfnVideoFrameCaptured fnVideoFrameCaptured;
	//user parameter for the user callback function.
	void *pFrameOwner;
} tVideoCaptureParams;

//Create -
//used to initialize video driver and to set video frame callback pointer
typedef tResult (*tfnSpiritVideoCaptureCreate) (void **ppObject, tVideoCaptureParams *pParams);
//Release
//used to release all resources allocated by the video driver
typedef tResult (*tfnSpiritVideoCaptureRelease) (void *pObject);
//Start
//starts actual video capture. Sets the resolution of the the captured video stream.
typedef tResult (*tfnSpiritVideoCaptureStart) (void *pObject,tVideoStreamParams *pParams);
//Stop
//stops video capture
typedef tResult (*tfnSpiritVideoCaptureStop) (void *pObject);

//Set Param
//sets video capture param
typedef tResult (*tpfnSpiritVideoIOSetParam)(void *pObject,const char *pszKey,tVariant value);
//
//gets video capture param
typedef tResult (*tpfnSpiritVideoIOGetParam)(void *pObject,const char *pszKey,tVariant *pValue);

typedef struct tagISpiritVideoCapture
{
	const char *strVersion;
	tfnSpiritVideoCaptureCreate Create;
	tfnSpiritVideoCaptureRelease Release;
	tfnSpiritVideoCaptureStart 	Start;
	tfnSpiritVideoCaptureStop 	Stop;

  //Implementation can set this pointer to NULL
  tpfnSpiritVideoIOSetParam SetParam;
  //Implementation can set this pointer to NULL
  tpfnSpiritVideoIOGetParam GetParam;
} iSpiritVideoCapture;



//////////////////////////////////
//Video out

typedef struct tagImageFormat
{
  uint32 Width;
  uint32 Height;
  tSpiritVideoFrameFormat ColorFormat;
} tImageFormat;

typedef struct tagDM642GraphicLibParam
{
	int32 extHeapSegId;
} tDM642GraphicLibParam;

typedef enum _eSpiritHermesViewportMode{
  spirit_viewport_preview,
  spirit_viewport_decoder
} eSpiritHermesViewportMode;

typedef struct tagSpiritHermesViewportParams
{
  uint32 ColorKey;
  eSpiritHermesViewportMode mode;
} tSpiritHermesViewportParams;

typedef union tagSpiritViewportParams {
  tSpiritHermesViewportParams *pHTCHermes;
  void *pFake;
} uSpiritViewportParams;


typedef tResult (*tSpiritGraphicsLibInit) (void **ppGraphics,void *pParam);
typedef tResult (*tSpiritGraphicsLibRelease) (void *pGraphics);

typedef tResult (*tSpiritGraphicsViewportCreate) (void *pGraphics,void **ppObject, int32 X, int32 Y,int32 Width, int32 Height, uSpiritViewportParams *pParams);
typedef tResult (*tSpiritGraphicsViewportDestroy) (void *pGraphics,void *pObject);

//typedef tResult (*tSpiritGraphicsBitBlt) (void *pObject,void *pFrame,size_t szFrameSizeBytes);
typedef tResult (*tSpiritGraphicsBitBlt) (void *pViewport,tSpiritVideoFrame *pFrame);

typedef tResult (*tSpiritGraphicsGetNativeFormat)(void* pGraphics,tSpiritVideoFrameFormat *pFormat);

typedef tResult (*tSpiritGraphicsViewportSetParam)(void* pGraphics,void *pViewport,const char *pszKey,tVariant value);
typedef tResult (*tSpiritGraphicsViewportGetParam)(void* pGraphics,void *pViewport,const char *pszKey,tVariant *pValue);

typedef struct tagSpritGraphics {

  const char *szFriendlyName;
  tSpiritGraphicsLibInit    fxnLibInit;
  tSpiritGraphicsLibRelease fxnLibRelease;

  tSpiritGraphicsViewportCreate  fxnViewportCreate;
  tSpiritGraphicsViewportDestroy fxnViewportDestroy;
  tSpiritGraphicsBitBlt fxnBitBlt;

  tSpiritGraphicsGetNativeFormat fxnGetNativeFormat;

  //Implementation can set this pointer to NULL
  tSpiritGraphicsViewportSetParam fxnViewportSetParam;
  //Implementation can set this pointer to NULL
  tSpiritGraphicsViewportGetParam fxnViewportGetParam;
} iSpiritGraphics;

#endif // SPIRIT_VIDEO_IO_H_INCLUDED
