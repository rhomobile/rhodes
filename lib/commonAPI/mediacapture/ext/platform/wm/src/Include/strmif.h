
#pragma warning( disable: 4049 )  /* more than 64k source lines */

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 5.03.0286 */
/* at Sun Mar 04 17:44:04 2007
 */
/* Compiler settings for .\strmif.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32 (32b run), ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __strmif_h__
#define __strmif_h__

/* Forward Declarations */ 

#ifndef __IPin_FWD_DEFINED__
#define __IPin_FWD_DEFINED__
typedef interface IPin IPin;
#endif 	/* __IPin_FWD_DEFINED__ */


#ifndef __IEnumPins_FWD_DEFINED__
#define __IEnumPins_FWD_DEFINED__
typedef interface IEnumPins IEnumPins;
#endif 	/* __IEnumPins_FWD_DEFINED__ */


#ifndef __IEnumMediaTypes_FWD_DEFINED__
#define __IEnumMediaTypes_FWD_DEFINED__
typedef interface IEnumMediaTypes IEnumMediaTypes;
#endif 	/* __IEnumMediaTypes_FWD_DEFINED__ */


#ifndef __IFilterGraph_FWD_DEFINED__
#define __IFilterGraph_FWD_DEFINED__
typedef interface IFilterGraph IFilterGraph;
#endif 	/* __IFilterGraph_FWD_DEFINED__ */


#ifndef __IEnumFilters_FWD_DEFINED__
#define __IEnumFilters_FWD_DEFINED__
typedef interface IEnumFilters IEnumFilters;
#endif 	/* __IEnumFilters_FWD_DEFINED__ */


#ifndef __IMediaFilter_FWD_DEFINED__
#define __IMediaFilter_FWD_DEFINED__
typedef interface IMediaFilter IMediaFilter;
#endif 	/* __IMediaFilter_FWD_DEFINED__ */


#ifndef __IBaseFilter_FWD_DEFINED__
#define __IBaseFilter_FWD_DEFINED__
typedef interface IBaseFilter IBaseFilter;
#endif 	/* __IBaseFilter_FWD_DEFINED__ */


#ifndef __IReferenceClock_FWD_DEFINED__
#define __IReferenceClock_FWD_DEFINED__
typedef interface IReferenceClock IReferenceClock;
#endif 	/* __IReferenceClock_FWD_DEFINED__ */


#ifndef __IReferenceClock2_FWD_DEFINED__
#define __IReferenceClock2_FWD_DEFINED__
typedef interface IReferenceClock2 IReferenceClock2;
#endif 	/* __IReferenceClock2_FWD_DEFINED__ */


#ifndef __IMediaSample_FWD_DEFINED__
#define __IMediaSample_FWD_DEFINED__
typedef interface IMediaSample IMediaSample;
#endif 	/* __IMediaSample_FWD_DEFINED__ */


#ifndef __IMediaSample2_FWD_DEFINED__
#define __IMediaSample2_FWD_DEFINED__
typedef interface IMediaSample2 IMediaSample2;
#endif 	/* __IMediaSample2_FWD_DEFINED__ */


#ifndef __IMemAllocator_FWD_DEFINED__
#define __IMemAllocator_FWD_DEFINED__
typedef interface IMemAllocator IMemAllocator;
#endif 	/* __IMemAllocator_FWD_DEFINED__ */


#ifndef __IMemAllocator2_FWD_DEFINED__
#define __IMemAllocator2_FWD_DEFINED__
typedef interface IMemAllocator2 IMemAllocator2;
#endif 	/* __IMemAllocator2_FWD_DEFINED__ */


#ifndef __IMemInputPin_FWD_DEFINED__
#define __IMemInputPin_FWD_DEFINED__
typedef interface IMemInputPin IMemInputPin;
#endif 	/* __IMemInputPin_FWD_DEFINED__ */


#ifndef __IAMovieSetup_FWD_DEFINED__
#define __IAMovieSetup_FWD_DEFINED__
typedef interface IAMovieSetup IAMovieSetup;
#endif 	/* __IAMovieSetup_FWD_DEFINED__ */


#ifndef __IMediaSeeking_FWD_DEFINED__
#define __IMediaSeeking_FWD_DEFINED__
typedef interface IMediaSeeking IMediaSeeking;
#endif 	/* __IMediaSeeking_FWD_DEFINED__ */


#ifndef __IAudioRenderer_FWD_DEFINED__
#define __IAudioRenderer_FWD_DEFINED__
typedef interface IAudioRenderer IAudioRenderer;
#endif 	/* __IAudioRenderer_FWD_DEFINED__ */


#ifndef __IAudioRendererWaveOut_FWD_DEFINED__
#define __IAudioRendererWaveOut_FWD_DEFINED__
typedef interface IAudioRendererWaveOut IAudioRendererWaveOut;
#endif 	/* __IAudioRendererWaveOut_FWD_DEFINED__ */


#ifndef __IEnumRegFilters_FWD_DEFINED__
#define __IEnumRegFilters_FWD_DEFINED__
typedef interface IEnumRegFilters IEnumRegFilters;
#endif 	/* __IEnumRegFilters_FWD_DEFINED__ */


#ifndef __IFilterMapper_FWD_DEFINED__
#define __IFilterMapper_FWD_DEFINED__
typedef interface IFilterMapper IFilterMapper;
#endif 	/* __IFilterMapper_FWD_DEFINED__ */


#ifndef __IQualityControl_FWD_DEFINED__
#define __IQualityControl_FWD_DEFINED__
typedef interface IQualityControl IQualityControl;
#endif 	/* __IQualityControl_FWD_DEFINED__ */


#ifndef __IOverlayNotify_FWD_DEFINED__
#define __IOverlayNotify_FWD_DEFINED__
typedef interface IOverlayNotify IOverlayNotify;
#endif 	/* __IOverlayNotify_FWD_DEFINED__ */


#ifndef __IOverlay_FWD_DEFINED__
#define __IOverlay_FWD_DEFINED__
typedef interface IOverlay IOverlay;
#endif 	/* __IOverlay_FWD_DEFINED__ */


#ifndef __IMediaEventSink_FWD_DEFINED__
#define __IMediaEventSink_FWD_DEFINED__
typedef interface IMediaEventSink IMediaEventSink;
#endif 	/* __IMediaEventSink_FWD_DEFINED__ */


#ifndef __IFileSourceFilter_FWD_DEFINED__
#define __IFileSourceFilter_FWD_DEFINED__
typedef interface IFileSourceFilter IFileSourceFilter;
#endif 	/* __IFileSourceFilter_FWD_DEFINED__ */


#ifndef __IFileSinkFilter_FWD_DEFINED__
#define __IFileSinkFilter_FWD_DEFINED__
typedef interface IFileSinkFilter IFileSinkFilter;
#endif 	/* __IFileSinkFilter_FWD_DEFINED__ */


#ifndef __IFileSinkFilter2_FWD_DEFINED__
#define __IFileSinkFilter2_FWD_DEFINED__
typedef interface IFileSinkFilter2 IFileSinkFilter2;
#endif 	/* __IFileSinkFilter2_FWD_DEFINED__ */


#ifndef __IFileAsyncIO_FWD_DEFINED__
#define __IFileAsyncIO_FWD_DEFINED__
typedef interface IFileAsyncIO IFileAsyncIO;
#endif 	/* __IFileAsyncIO_FWD_DEFINED__ */


#ifndef __IGraphBuilder_FWD_DEFINED__
#define __IGraphBuilder_FWD_DEFINED__
typedef interface IGraphBuilder IGraphBuilder;
#endif 	/* __IGraphBuilder_FWD_DEFINED__ */


#ifndef __ICaptureGraphBuilder_FWD_DEFINED__
#define __ICaptureGraphBuilder_FWD_DEFINED__
typedef interface ICaptureGraphBuilder ICaptureGraphBuilder;
#endif 	/* __ICaptureGraphBuilder_FWD_DEFINED__ */


#ifndef __IAMCopyCaptureFileProgress_FWD_DEFINED__
#define __IAMCopyCaptureFileProgress_FWD_DEFINED__
typedef interface IAMCopyCaptureFileProgress IAMCopyCaptureFileProgress;
#endif 	/* __IAMCopyCaptureFileProgress_FWD_DEFINED__ */


#ifndef __ICaptureGraphBuilder2_FWD_DEFINED__
#define __ICaptureGraphBuilder2_FWD_DEFINED__
typedef interface ICaptureGraphBuilder2 ICaptureGraphBuilder2;
#endif 	/* __ICaptureGraphBuilder2_FWD_DEFINED__ */


#ifndef __IFilterGraph2_FWD_DEFINED__
#define __IFilterGraph2_FWD_DEFINED__
typedef interface IFilterGraph2 IFilterGraph2;
#endif 	/* __IFilterGraph2_FWD_DEFINED__ */


#ifndef __IStreamBuilder_FWD_DEFINED__
#define __IStreamBuilder_FWD_DEFINED__
typedef interface IStreamBuilder IStreamBuilder;
#endif 	/* __IStreamBuilder_FWD_DEFINED__ */


#ifndef __IAsyncReader_FWD_DEFINED__
#define __IAsyncReader_FWD_DEFINED__
typedef interface IAsyncReader IAsyncReader;
#endif 	/* __IAsyncReader_FWD_DEFINED__ */


#ifndef __IGraphVersion_FWD_DEFINED__
#define __IGraphVersion_FWD_DEFINED__
typedef interface IGraphVersion IGraphVersion;
#endif 	/* __IGraphVersion_FWD_DEFINED__ */


#ifndef __IResourceConsumer_FWD_DEFINED__
#define __IResourceConsumer_FWD_DEFINED__
typedef interface IResourceConsumer IResourceConsumer;
#endif 	/* __IResourceConsumer_FWD_DEFINED__ */


#ifndef __IResourceManager_FWD_DEFINED__
#define __IResourceManager_FWD_DEFINED__
typedef interface IResourceManager IResourceManager;
#endif 	/* __IResourceManager_FWD_DEFINED__ */


#ifndef __IDistributorNotify_FWD_DEFINED__
#define __IDistributorNotify_FWD_DEFINED__
typedef interface IDistributorNotify IDistributorNotify;
#endif 	/* __IDistributorNotify_FWD_DEFINED__ */


#ifndef __IAMStreamControl_FWD_DEFINED__
#define __IAMStreamControl_FWD_DEFINED__
typedef interface IAMStreamControl IAMStreamControl;
#endif 	/* __IAMStreamControl_FWD_DEFINED__ */


#ifndef __ISeekingPassThru_FWD_DEFINED__
#define __ISeekingPassThru_FWD_DEFINED__
typedef interface ISeekingPassThru ISeekingPassThru;
#endif 	/* __ISeekingPassThru_FWD_DEFINED__ */


#ifndef __IAMStreamConfig_FWD_DEFINED__
#define __IAMStreamConfig_FWD_DEFINED__
typedef interface IAMStreamConfig IAMStreamConfig;
#endif 	/* __IAMStreamConfig_FWD_DEFINED__ */


#ifndef __IConfigInterleaving_FWD_DEFINED__
#define __IConfigInterleaving_FWD_DEFINED__
typedef interface IConfigInterleaving IConfigInterleaving;
#endif 	/* __IConfigInterleaving_FWD_DEFINED__ */


#ifndef __IConfigAviMux_FWD_DEFINED__
#define __IConfigAviMux_FWD_DEFINED__
typedef interface IConfigAviMux IConfigAviMux;
#endif 	/* __IConfigAviMux_FWD_DEFINED__ */


#ifndef __IAMVideoCompression_FWD_DEFINED__
#define __IAMVideoCompression_FWD_DEFINED__
typedef interface IAMVideoCompression IAMVideoCompression;
#endif 	/* __IAMVideoCompression_FWD_DEFINED__ */


#ifndef __IAMDroppedFrames_FWD_DEFINED__
#define __IAMDroppedFrames_FWD_DEFINED__
typedef interface IAMDroppedFrames IAMDroppedFrames;
#endif 	/* __IAMDroppedFrames_FWD_DEFINED__ */


#ifndef __IAMBufferNegotiation_FWD_DEFINED__
#define __IAMBufferNegotiation_FWD_DEFINED__
typedef interface IAMBufferNegotiation IAMBufferNegotiation;
#endif 	/* __IAMBufferNegotiation_FWD_DEFINED__ */


#ifndef __IAMVideoProcAmp_FWD_DEFINED__
#define __IAMVideoProcAmp_FWD_DEFINED__
typedef interface IAMVideoProcAmp IAMVideoProcAmp;
#endif 	/* __IAMVideoProcAmp_FWD_DEFINED__ */


#ifndef __IAMCameraControl_FWD_DEFINED__
#define __IAMCameraControl_FWD_DEFINED__
typedef interface IAMCameraControl IAMCameraControl;
#endif 	/* __IAMCameraControl_FWD_DEFINED__ */


#ifndef __IAMVideoControl_FWD_DEFINED__
#define __IAMVideoControl_FWD_DEFINED__
typedef interface IAMVideoControl IAMVideoControl;
#endif 	/* __IAMVideoControl_FWD_DEFINED__ */


#ifndef __IAMCrossbar_FWD_DEFINED__
#define __IAMCrossbar_FWD_DEFINED__
typedef interface IAMCrossbar IAMCrossbar;
#endif 	/* __IAMCrossbar_FWD_DEFINED__ */


#ifndef __IAMTuner_FWD_DEFINED__
#define __IAMTuner_FWD_DEFINED__
typedef interface IAMTuner IAMTuner;
#endif 	/* __IAMTuner_FWD_DEFINED__ */


#ifndef __IAMTunerNotification_FWD_DEFINED__
#define __IAMTunerNotification_FWD_DEFINED__
typedef interface IAMTunerNotification IAMTunerNotification;
#endif 	/* __IAMTunerNotification_FWD_DEFINED__ */


#ifndef __IAMTVTuner_FWD_DEFINED__
#define __IAMTVTuner_FWD_DEFINED__
typedef interface IAMTVTuner IAMTVTuner;
#endif 	/* __IAMTVTuner_FWD_DEFINED__ */


#ifndef __IKsPropertySet_FWD_DEFINED__
#define __IKsPropertySet_FWD_DEFINED__
typedef interface IKsPropertySet IKsPropertySet;
#endif 	/* __IKsPropertySet_FWD_DEFINED__ */


#ifndef __IMediaPropertyBag_FWD_DEFINED__
#define __IMediaPropertyBag_FWD_DEFINED__
typedef interface IMediaPropertyBag IMediaPropertyBag;
#endif 	/* __IMediaPropertyBag_FWD_DEFINED__ */


#ifndef __IPersistMediaPropertyBag_FWD_DEFINED__
#define __IPersistMediaPropertyBag_FWD_DEFINED__
typedef interface IPersistMediaPropertyBag IPersistMediaPropertyBag;
#endif 	/* __IPersistMediaPropertyBag_FWD_DEFINED__ */


#ifndef __IAMPhysicalPinInfo_FWD_DEFINED__
#define __IAMPhysicalPinInfo_FWD_DEFINED__
typedef interface IAMPhysicalPinInfo IAMPhysicalPinInfo;
#endif 	/* __IAMPhysicalPinInfo_FWD_DEFINED__ */


#ifndef __IAMExtDevice_FWD_DEFINED__
#define __IAMExtDevice_FWD_DEFINED__
typedef interface IAMExtDevice IAMExtDevice;
#endif 	/* __IAMExtDevice_FWD_DEFINED__ */


#ifndef __IAMExtTransport_FWD_DEFINED__
#define __IAMExtTransport_FWD_DEFINED__
typedef interface IAMExtTransport IAMExtTransport;
#endif 	/* __IAMExtTransport_FWD_DEFINED__ */


#ifndef __IAMTimecodeReader_FWD_DEFINED__
#define __IAMTimecodeReader_FWD_DEFINED__
typedef interface IAMTimecodeReader IAMTimecodeReader;
#endif 	/* __IAMTimecodeReader_FWD_DEFINED__ */


#ifndef __IAMTimecodeGenerator_FWD_DEFINED__
#define __IAMTimecodeGenerator_FWD_DEFINED__
typedef interface IAMTimecodeGenerator IAMTimecodeGenerator;
#endif 	/* __IAMTimecodeGenerator_FWD_DEFINED__ */


#ifndef __IAMTimecodeDisplay_FWD_DEFINED__
#define __IAMTimecodeDisplay_FWD_DEFINED__
typedef interface IAMTimecodeDisplay IAMTimecodeDisplay;
#endif 	/* __IAMTimecodeDisplay_FWD_DEFINED__ */


#ifndef __IAMDevMemoryAllocator_FWD_DEFINED__
#define __IAMDevMemoryAllocator_FWD_DEFINED__
typedef interface IAMDevMemoryAllocator IAMDevMemoryAllocator;
#endif 	/* __IAMDevMemoryAllocator_FWD_DEFINED__ */


#ifndef __IAMDevMemoryControl_FWD_DEFINED__
#define __IAMDevMemoryControl_FWD_DEFINED__
typedef interface IAMDevMemoryControl IAMDevMemoryControl;
#endif 	/* __IAMDevMemoryControl_FWD_DEFINED__ */


#ifndef __IAMStreamSelect_FWD_DEFINED__
#define __IAMStreamSelect_FWD_DEFINED__
typedef interface IAMStreamSelect IAMStreamSelect;
#endif 	/* __IAMStreamSelect_FWD_DEFINED__ */


#ifndef __IAMResourceControl_FWD_DEFINED__
#define __IAMResourceControl_FWD_DEFINED__
typedef interface IAMResourceControl IAMResourceControl;
#endif 	/* __IAMResourceControl_FWD_DEFINED__ */


#ifndef __IAMClockAdjust_FWD_DEFINED__
#define __IAMClockAdjust_FWD_DEFINED__
typedef interface IAMClockAdjust IAMClockAdjust;
#endif 	/* __IAMClockAdjust_FWD_DEFINED__ */


#ifndef __IAMFilterMiscFlags_FWD_DEFINED__
#define __IAMFilterMiscFlags_FWD_DEFINED__
typedef interface IAMFilterMiscFlags IAMFilterMiscFlags;
#endif 	/* __IAMFilterMiscFlags_FWD_DEFINED__ */


#ifndef __IAMAudioRendererStats_FWD_DEFINED__
#define __IAMAudioRendererStats_FWD_DEFINED__
typedef interface IAMAudioRendererStats IAMAudioRendererStats;
#endif 	/* __IAMAudioRendererStats_FWD_DEFINED__ */


#ifndef __IAMVideoRendererMode_FWD_DEFINED__
#define __IAMVideoRendererMode_FWD_DEFINED__
typedef interface IAMVideoRendererMode IAMVideoRendererMode;
#endif 	/* __IAMVideoRendererMode_FWD_DEFINED__ */


#ifndef __ILanguageSelection_FWD_DEFINED__
#define __ILanguageSelection_FWD_DEFINED__
typedef interface ILanguageSelection ILanguageSelection;
#endif 	/* __ILanguageSelection_FWD_DEFINED__ */


#ifndef __IImageSinkFilter_FWD_DEFINED__
#define __IImageSinkFilter_FWD_DEFINED__
typedef interface IImageSinkFilter IImageSinkFilter;
#endif 	/* __IImageSinkFilter_FWD_DEFINED__ */


#ifndef __ISmartTee_FWD_DEFINED__
#define __ISmartTee_FWD_DEFINED__
typedef interface ISmartTee ISmartTee;
#endif 	/* __ISmartTee_FWD_DEFINED__ */


#ifndef __IBuffering_FWD_DEFINED__
#define __IBuffering_FWD_DEFINED__
typedef interface IBuffering IBuffering;
#endif 	/* __IBuffering_FWD_DEFINED__ */


#ifndef __IDvdControl_FWD_DEFINED__
#define __IDvdControl_FWD_DEFINED__
typedef interface IDvdControl IDvdControl;
#endif 	/* __IDvdControl_FWD_DEFINED__ */


#ifndef __IDvdInfo_FWD_DEFINED__
#define __IDvdInfo_FWD_DEFINED__
typedef interface IDvdInfo IDvdInfo;
#endif 	/* __IDvdInfo_FWD_DEFINED__ */


#ifndef __IDvdCmd_FWD_DEFINED__
#define __IDvdCmd_FWD_DEFINED__
typedef interface IDvdCmd IDvdCmd;
#endif 	/* __IDvdCmd_FWD_DEFINED__ */


#ifndef __IDvdState_FWD_DEFINED__
#define __IDvdState_FWD_DEFINED__
typedef interface IDvdState IDvdState;
#endif 	/* __IDvdState_FWD_DEFINED__ */


#ifndef __IDvdControl2_FWD_DEFINED__
#define __IDvdControl2_FWD_DEFINED__
typedef interface IDvdControl2 IDvdControl2;
#endif 	/* __IDvdControl2_FWD_DEFINED__ */


#ifndef __IDvdInfo2_FWD_DEFINED__
#define __IDvdInfo2_FWD_DEFINED__
typedef interface IDvdInfo2 IDvdInfo2;
#endif 	/* __IDvdInfo2_FWD_DEFINED__ */


#ifndef __IDvdGraphBuilder_FWD_DEFINED__
#define __IDvdGraphBuilder_FWD_DEFINED__
typedef interface IDvdGraphBuilder IDvdGraphBuilder;
#endif 	/* __IDvdGraphBuilder_FWD_DEFINED__ */


#ifndef __IDDrawExclModeVideo_FWD_DEFINED__
#define __IDDrawExclModeVideo_FWD_DEFINED__
typedef interface IDDrawExclModeVideo IDDrawExclModeVideo;
#endif 	/* __IDDrawExclModeVideo_FWD_DEFINED__ */


#ifndef __IDDrawExclModeVideoCallback_FWD_DEFINED__
#define __IDDrawExclModeVideoCallback_FWD_DEFINED__
typedef interface IDDrawExclModeVideoCallback IDDrawExclModeVideoCallback;
#endif 	/* __IDDrawExclModeVideoCallback_FWD_DEFINED__ */


/* header files for imported files */
#include "unknwn.h"
#include "objidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

/* interface __MIDL_itf_strmif_0000 */
/* [local] */ 

#ifndef NO_FILTERMAPPER2
#define NO_FILTERMAPPER2
#endif
#ifndef NO_PROPERTYPAGES
#define NO_PROPERTYPAGES
#endif
typedef DWORD DWORD_PTR;

typedef LONG LONG_PTR;

#define CHARS_IN_GUID     39
typedef struct _AMMediaType
    {
    GUID majortype;
    GUID subtype;
    BOOL bFixedSizeSamples;
    BOOL bTemporalCompression;
    ULONG lSampleSize;
    GUID formattype;
    IUnknown __RPC_FAR *pUnk;
    ULONG cbFormat;
    /* [size_is] */ BYTE __RPC_FAR *pbFormat;
    }	AM_MEDIA_TYPE;

typedef 
enum _PinDirection
    {	PINDIR_INPUT	= 0,
	PINDIR_OUTPUT	= PINDIR_INPUT + 1
    }	PIN_DIRECTION;

#define MAX_PIN_NAME     128
#define MAX_FILTER_NAME  128
typedef LONGLONG REFERENCE_TIME;

typedef double REFTIME;

typedef DWORD_PTR HSEMAPHORE;

typedef DWORD_PTR HEVENT;

typedef struct _AllocatorProperties
    {
    long cBuffers;
    long cbBuffer;
    long cbAlign;
    long cbPrefix;
    }	ALLOCATOR_PROPERTIES;















extern RPC_IF_HANDLE __MIDL_itf_strmif_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_strmif_0000_v0_0_s_ifspec;

#ifndef __IPin_INTERFACE_DEFINED__
#define __IPin_INTERFACE_DEFINED__

/* interface IPin */
/* [unique][uuid][object] */ 

typedef struct _PinInfo
    {
    IBaseFilter __RPC_FAR *pFilter;
    PIN_DIRECTION dir;
    WCHAR achName[ 128 ];
    }	PIN_INFO;


EXTERN_C const IID IID_IPin;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("56a86891-0ad4-11ce-b03a-0020af0ba770")
    IPin : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Connect( 
            /* [in] */ IPin __RPC_FAR *pReceivePin,
            /* [in] */ const AM_MEDIA_TYPE __RPC_FAR *pmt) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ReceiveConnection( 
            /* [in] */ IPin __RPC_FAR *pConnector,
            /* [in] */ const AM_MEDIA_TYPE __RPC_FAR *pmt) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Disconnect( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ConnectedTo( 
            /* [out] */ IPin __RPC_FAR *__RPC_FAR *pPin) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ConnectionMediaType( 
            /* [out] */ AM_MEDIA_TYPE __RPC_FAR *pmt) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE QueryPinInfo( 
            /* [out] */ PIN_INFO __RPC_FAR *pInfo) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE QueryDirection( 
            /* [out] */ PIN_DIRECTION __RPC_FAR *pPinDir) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE QueryId( 
            /* [out] */ LPWSTR __RPC_FAR *Id) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE QueryAccept( 
            /* [in] */ const AM_MEDIA_TYPE __RPC_FAR *pmt) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE EnumMediaTypes( 
            /* [out] */ IEnumMediaTypes __RPC_FAR *__RPC_FAR *ppEnum) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE QueryInternalConnections( 
            /* [out] */ IPin __RPC_FAR *__RPC_FAR *apPin,
            /* [out][in] */ ULONG __RPC_FAR *nPin) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE EndOfStream( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE BeginFlush( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE EndFlush( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE NewSegment( 
            /* [in] */ REFERENCE_TIME tStart,
            /* [in] */ REFERENCE_TIME tStop,
            /* [in] */ double dRate) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPinVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IPin __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IPin __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IPin __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Connect )( 
            IPin __RPC_FAR * This,
            /* [in] */ IPin __RPC_FAR *pReceivePin,
            /* [in] */ const AM_MEDIA_TYPE __RPC_FAR *pmt);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ReceiveConnection )( 
            IPin __RPC_FAR * This,
            /* [in] */ IPin __RPC_FAR *pConnector,
            /* [in] */ const AM_MEDIA_TYPE __RPC_FAR *pmt);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Disconnect )( 
            IPin __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ConnectedTo )( 
            IPin __RPC_FAR * This,
            /* [out] */ IPin __RPC_FAR *__RPC_FAR *pPin);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ConnectionMediaType )( 
            IPin __RPC_FAR * This,
            /* [out] */ AM_MEDIA_TYPE __RPC_FAR *pmt);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryPinInfo )( 
            IPin __RPC_FAR * This,
            /* [out] */ PIN_INFO __RPC_FAR *pInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryDirection )( 
            IPin __RPC_FAR * This,
            /* [out] */ PIN_DIRECTION __RPC_FAR *pPinDir);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryId )( 
            IPin __RPC_FAR * This,
            /* [out] */ LPWSTR __RPC_FAR *Id);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryAccept )( 
            IPin __RPC_FAR * This,
            /* [in] */ const AM_MEDIA_TYPE __RPC_FAR *pmt);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *EnumMediaTypes )( 
            IPin __RPC_FAR * This,
            /* [out] */ IEnumMediaTypes __RPC_FAR *__RPC_FAR *ppEnum);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInternalConnections )( 
            IPin __RPC_FAR * This,
            /* [out] */ IPin __RPC_FAR *__RPC_FAR *apPin,
            /* [out][in] */ ULONG __RPC_FAR *nPin);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *EndOfStream )( 
            IPin __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *BeginFlush )( 
            IPin __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *EndFlush )( 
            IPin __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *NewSegment )( 
            IPin __RPC_FAR * This,
            /* [in] */ REFERENCE_TIME tStart,
            /* [in] */ REFERENCE_TIME tStop,
            /* [in] */ double dRate);
        
        END_INTERFACE
    } IPinVtbl;

    interface IPin
    {
        CONST_VTBL struct IPinVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPin_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPin_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPin_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPin_Connect(This,pReceivePin,pmt)	\
    (This)->lpVtbl -> Connect(This,pReceivePin,pmt)

#define IPin_ReceiveConnection(This,pConnector,pmt)	\
    (This)->lpVtbl -> ReceiveConnection(This,pConnector,pmt)

#define IPin_Disconnect(This)	\
    (This)->lpVtbl -> Disconnect(This)

#define IPin_ConnectedTo(This,pPin)	\
    (This)->lpVtbl -> ConnectedTo(This,pPin)

#define IPin_ConnectionMediaType(This,pmt)	\
    (This)->lpVtbl -> ConnectionMediaType(This,pmt)

#define IPin_QueryPinInfo(This,pInfo)	\
    (This)->lpVtbl -> QueryPinInfo(This,pInfo)

#define IPin_QueryDirection(This,pPinDir)	\
    (This)->lpVtbl -> QueryDirection(This,pPinDir)

#define IPin_QueryId(This,Id)	\
    (This)->lpVtbl -> QueryId(This,Id)

#define IPin_QueryAccept(This,pmt)	\
    (This)->lpVtbl -> QueryAccept(This,pmt)

#define IPin_EnumMediaTypes(This,ppEnum)	\
    (This)->lpVtbl -> EnumMediaTypes(This,ppEnum)

#define IPin_QueryInternalConnections(This,apPin,nPin)	\
    (This)->lpVtbl -> QueryInternalConnections(This,apPin,nPin)

#define IPin_EndOfStream(This)	\
    (This)->lpVtbl -> EndOfStream(This)

#define IPin_BeginFlush(This)	\
    (This)->lpVtbl -> BeginFlush(This)

#define IPin_EndFlush(This)	\
    (This)->lpVtbl -> EndFlush(This)

#define IPin_NewSegment(This,tStart,tStop,dRate)	\
    (This)->lpVtbl -> NewSegment(This,tStart,tStop,dRate)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IPin_Connect_Proxy( 
    IPin __RPC_FAR * This,
    /* [in] */ IPin __RPC_FAR *pReceivePin,
    /* [in] */ const AM_MEDIA_TYPE __RPC_FAR *pmt);


void __RPC_STUB IPin_Connect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPin_ReceiveConnection_Proxy( 
    IPin __RPC_FAR * This,
    /* [in] */ IPin __RPC_FAR *pConnector,
    /* [in] */ const AM_MEDIA_TYPE __RPC_FAR *pmt);


void __RPC_STUB IPin_ReceiveConnection_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPin_Disconnect_Proxy( 
    IPin __RPC_FAR * This);


void __RPC_STUB IPin_Disconnect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPin_ConnectedTo_Proxy( 
    IPin __RPC_FAR * This,
    /* [out] */ IPin __RPC_FAR *__RPC_FAR *pPin);


void __RPC_STUB IPin_ConnectedTo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPin_ConnectionMediaType_Proxy( 
    IPin __RPC_FAR * This,
    /* [out] */ AM_MEDIA_TYPE __RPC_FAR *pmt);


void __RPC_STUB IPin_ConnectionMediaType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPin_QueryPinInfo_Proxy( 
    IPin __RPC_FAR * This,
    /* [out] */ PIN_INFO __RPC_FAR *pInfo);


void __RPC_STUB IPin_QueryPinInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPin_QueryDirection_Proxy( 
    IPin __RPC_FAR * This,
    /* [out] */ PIN_DIRECTION __RPC_FAR *pPinDir);


void __RPC_STUB IPin_QueryDirection_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPin_QueryId_Proxy( 
    IPin __RPC_FAR * This,
    /* [out] */ LPWSTR __RPC_FAR *Id);


void __RPC_STUB IPin_QueryId_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPin_QueryAccept_Proxy( 
    IPin __RPC_FAR * This,
    /* [in] */ const AM_MEDIA_TYPE __RPC_FAR *pmt);


void __RPC_STUB IPin_QueryAccept_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPin_EnumMediaTypes_Proxy( 
    IPin __RPC_FAR * This,
    /* [out] */ IEnumMediaTypes __RPC_FAR *__RPC_FAR *ppEnum);


void __RPC_STUB IPin_EnumMediaTypes_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPin_QueryInternalConnections_Proxy( 
    IPin __RPC_FAR * This,
    /* [out] */ IPin __RPC_FAR *__RPC_FAR *apPin,
    /* [out][in] */ ULONG __RPC_FAR *nPin);


void __RPC_STUB IPin_QueryInternalConnections_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPin_EndOfStream_Proxy( 
    IPin __RPC_FAR * This);


void __RPC_STUB IPin_EndOfStream_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPin_BeginFlush_Proxy( 
    IPin __RPC_FAR * This);


void __RPC_STUB IPin_BeginFlush_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPin_EndFlush_Proxy( 
    IPin __RPC_FAR * This);


void __RPC_STUB IPin_EndFlush_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPin_NewSegment_Proxy( 
    IPin __RPC_FAR * This,
    /* [in] */ REFERENCE_TIME tStart,
    /* [in] */ REFERENCE_TIME tStop,
    /* [in] */ double dRate);


void __RPC_STUB IPin_NewSegment_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPin_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_strmif_0057 */
/* [local] */ 

typedef IPin __RPC_FAR *PPIN;



extern RPC_IF_HANDLE __MIDL_itf_strmif_0057_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_strmif_0057_v0_0_s_ifspec;

#ifndef __IEnumPins_INTERFACE_DEFINED__
#define __IEnumPins_INTERFACE_DEFINED__

/* interface IEnumPins */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IEnumPins;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("56a86892-0ad4-11ce-b03a-0020af0ba770")
    IEnumPins : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Next( 
            /* [in] */ ULONG cPins,
            /* [size_is][out] */ IPin __RPC_FAR *__RPC_FAR *ppPins,
            /* [out] */ ULONG __RPC_FAR *pcFetched) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Skip( 
            /* [in] */ ULONG cPins) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Reset( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Clone( 
            /* [out] */ IEnumPins __RPC_FAR *__RPC_FAR *ppEnum) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IEnumPinsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IEnumPins __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IEnumPins __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IEnumPins __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Next )( 
            IEnumPins __RPC_FAR * This,
            /* [in] */ ULONG cPins,
            /* [size_is][out] */ IPin __RPC_FAR *__RPC_FAR *ppPins,
            /* [out] */ ULONG __RPC_FAR *pcFetched);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Skip )( 
            IEnumPins __RPC_FAR * This,
            /* [in] */ ULONG cPins);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Reset )( 
            IEnumPins __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Clone )( 
            IEnumPins __RPC_FAR * This,
            /* [out] */ IEnumPins __RPC_FAR *__RPC_FAR *ppEnum);
        
        END_INTERFACE
    } IEnumPinsVtbl;

    interface IEnumPins
    {
        CONST_VTBL struct IEnumPinsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IEnumPins_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IEnumPins_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IEnumPins_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IEnumPins_Next(This,cPins,ppPins,pcFetched)	\
    (This)->lpVtbl -> Next(This,cPins,ppPins,pcFetched)

#define IEnumPins_Skip(This,cPins)	\
    (This)->lpVtbl -> Skip(This,cPins)

#define IEnumPins_Reset(This)	\
    (This)->lpVtbl -> Reset(This)

#define IEnumPins_Clone(This,ppEnum)	\
    (This)->lpVtbl -> Clone(This,ppEnum)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IEnumPins_Next_Proxy( 
    IEnumPins __RPC_FAR * This,
    /* [in] */ ULONG cPins,
    /* [size_is][out] */ IPin __RPC_FAR *__RPC_FAR *ppPins,
    /* [out] */ ULONG __RPC_FAR *pcFetched);


void __RPC_STUB IEnumPins_Next_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IEnumPins_Skip_Proxy( 
    IEnumPins __RPC_FAR * This,
    /* [in] */ ULONG cPins);


void __RPC_STUB IEnumPins_Skip_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IEnumPins_Reset_Proxy( 
    IEnumPins __RPC_FAR * This);


void __RPC_STUB IEnumPins_Reset_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IEnumPins_Clone_Proxy( 
    IEnumPins __RPC_FAR * This,
    /* [out] */ IEnumPins __RPC_FAR *__RPC_FAR *ppEnum);


void __RPC_STUB IEnumPins_Clone_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IEnumPins_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_strmif_0058 */
/* [local] */ 

typedef IEnumPins __RPC_FAR *PENUMPINS;



extern RPC_IF_HANDLE __MIDL_itf_strmif_0058_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_strmif_0058_v0_0_s_ifspec;

#ifndef __IEnumMediaTypes_INTERFACE_DEFINED__
#define __IEnumMediaTypes_INTERFACE_DEFINED__

/* interface IEnumMediaTypes */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IEnumMediaTypes;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("89c31040-846b-11ce-97d3-00aa0055595a")
    IEnumMediaTypes : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Next( 
            /* [in] */ ULONG cMediaTypes,
            /* [size_is][out] */ AM_MEDIA_TYPE __RPC_FAR *__RPC_FAR *ppMediaTypes,
            /* [out] */ ULONG __RPC_FAR *pcFetched) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Skip( 
            /* [in] */ ULONG cMediaTypes) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Reset( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Clone( 
            /* [out] */ IEnumMediaTypes __RPC_FAR *__RPC_FAR *ppEnum) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IEnumMediaTypesVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IEnumMediaTypes __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IEnumMediaTypes __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IEnumMediaTypes __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Next )( 
            IEnumMediaTypes __RPC_FAR * This,
            /* [in] */ ULONG cMediaTypes,
            /* [size_is][out] */ AM_MEDIA_TYPE __RPC_FAR *__RPC_FAR *ppMediaTypes,
            /* [out] */ ULONG __RPC_FAR *pcFetched);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Skip )( 
            IEnumMediaTypes __RPC_FAR * This,
            /* [in] */ ULONG cMediaTypes);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Reset )( 
            IEnumMediaTypes __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Clone )( 
            IEnumMediaTypes __RPC_FAR * This,
            /* [out] */ IEnumMediaTypes __RPC_FAR *__RPC_FAR *ppEnum);
        
        END_INTERFACE
    } IEnumMediaTypesVtbl;

    interface IEnumMediaTypes
    {
        CONST_VTBL struct IEnumMediaTypesVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IEnumMediaTypes_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IEnumMediaTypes_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IEnumMediaTypes_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IEnumMediaTypes_Next(This,cMediaTypes,ppMediaTypes,pcFetched)	\
    (This)->lpVtbl -> Next(This,cMediaTypes,ppMediaTypes,pcFetched)

#define IEnumMediaTypes_Skip(This,cMediaTypes)	\
    (This)->lpVtbl -> Skip(This,cMediaTypes)

#define IEnumMediaTypes_Reset(This)	\
    (This)->lpVtbl -> Reset(This)

#define IEnumMediaTypes_Clone(This,ppEnum)	\
    (This)->lpVtbl -> Clone(This,ppEnum)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IEnumMediaTypes_Next_Proxy( 
    IEnumMediaTypes __RPC_FAR * This,
    /* [in] */ ULONG cMediaTypes,
    /* [size_is][out] */ AM_MEDIA_TYPE __RPC_FAR *__RPC_FAR *ppMediaTypes,
    /* [out] */ ULONG __RPC_FAR *pcFetched);


void __RPC_STUB IEnumMediaTypes_Next_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IEnumMediaTypes_Skip_Proxy( 
    IEnumMediaTypes __RPC_FAR * This,
    /* [in] */ ULONG cMediaTypes);


void __RPC_STUB IEnumMediaTypes_Skip_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IEnumMediaTypes_Reset_Proxy( 
    IEnumMediaTypes __RPC_FAR * This);


void __RPC_STUB IEnumMediaTypes_Reset_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IEnumMediaTypes_Clone_Proxy( 
    IEnumMediaTypes __RPC_FAR * This,
    /* [out] */ IEnumMediaTypes __RPC_FAR *__RPC_FAR *ppEnum);


void __RPC_STUB IEnumMediaTypes_Clone_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IEnumMediaTypes_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_strmif_0059 */
/* [local] */ 

typedef IEnumMediaTypes __RPC_FAR *PENUMMEDIATYPES;



extern RPC_IF_HANDLE __MIDL_itf_strmif_0059_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_strmif_0059_v0_0_s_ifspec;

#ifndef __IFilterGraph_INTERFACE_DEFINED__
#define __IFilterGraph_INTERFACE_DEFINED__

/* interface IFilterGraph */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IFilterGraph;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("56a8689f-0ad4-11ce-b03a-0020af0ba770")
    IFilterGraph : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE AddFilter( 
            /* [in] */ IBaseFilter __RPC_FAR *pFilter,
            /* [string][in] */ LPCWSTR pName) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RemoveFilter( 
            /* [in] */ IBaseFilter __RPC_FAR *pFilter) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE EnumFilters( 
            /* [out] */ IEnumFilters __RPC_FAR *__RPC_FAR *ppEnum) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE FindFilterByName( 
            /* [string][in] */ LPCWSTR pName,
            /* [out] */ IBaseFilter __RPC_FAR *__RPC_FAR *ppFilter) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ConnectDirect( 
            /* [in] */ IPin __RPC_FAR *ppinOut,
            /* [in] */ IPin __RPC_FAR *ppinIn,
            /* [in] */ const AM_MEDIA_TYPE __RPC_FAR *pmt) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Reconnect( 
            /* [in] */ IPin __RPC_FAR *ppin) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Disconnect( 
            /* [in] */ IPin __RPC_FAR *ppin) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetDefaultSyncSource( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IFilterGraphVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IFilterGraph __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IFilterGraph __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IFilterGraph __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddFilter )( 
            IFilterGraph __RPC_FAR * This,
            /* [in] */ IBaseFilter __RPC_FAR *pFilter,
            /* [string][in] */ LPCWSTR pName);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RemoveFilter )( 
            IFilterGraph __RPC_FAR * This,
            /* [in] */ IBaseFilter __RPC_FAR *pFilter);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *EnumFilters )( 
            IFilterGraph __RPC_FAR * This,
            /* [out] */ IEnumFilters __RPC_FAR *__RPC_FAR *ppEnum);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *FindFilterByName )( 
            IFilterGraph __RPC_FAR * This,
            /* [string][in] */ LPCWSTR pName,
            /* [out] */ IBaseFilter __RPC_FAR *__RPC_FAR *ppFilter);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ConnectDirect )( 
            IFilterGraph __RPC_FAR * This,
            /* [in] */ IPin __RPC_FAR *ppinOut,
            /* [in] */ IPin __RPC_FAR *ppinIn,
            /* [in] */ const AM_MEDIA_TYPE __RPC_FAR *pmt);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Reconnect )( 
            IFilterGraph __RPC_FAR * This,
            /* [in] */ IPin __RPC_FAR *ppin);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Disconnect )( 
            IFilterGraph __RPC_FAR * This,
            /* [in] */ IPin __RPC_FAR *ppin);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetDefaultSyncSource )( 
            IFilterGraph __RPC_FAR * This);
        
        END_INTERFACE
    } IFilterGraphVtbl;

    interface IFilterGraph
    {
        CONST_VTBL struct IFilterGraphVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IFilterGraph_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IFilterGraph_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IFilterGraph_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IFilterGraph_AddFilter(This,pFilter,pName)	\
    (This)->lpVtbl -> AddFilter(This,pFilter,pName)

#define IFilterGraph_RemoveFilter(This,pFilter)	\
    (This)->lpVtbl -> RemoveFilter(This,pFilter)

#define IFilterGraph_EnumFilters(This,ppEnum)	\
    (This)->lpVtbl -> EnumFilters(This,ppEnum)

#define IFilterGraph_FindFilterByName(This,pName,ppFilter)	\
    (This)->lpVtbl -> FindFilterByName(This,pName,ppFilter)

#define IFilterGraph_ConnectDirect(This,ppinOut,ppinIn,pmt)	\
    (This)->lpVtbl -> ConnectDirect(This,ppinOut,ppinIn,pmt)

#define IFilterGraph_Reconnect(This,ppin)	\
    (This)->lpVtbl -> Reconnect(This,ppin)

#define IFilterGraph_Disconnect(This,ppin)	\
    (This)->lpVtbl -> Disconnect(This,ppin)

#define IFilterGraph_SetDefaultSyncSource(This)	\
    (This)->lpVtbl -> SetDefaultSyncSource(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IFilterGraph_AddFilter_Proxy( 
    IFilterGraph __RPC_FAR * This,
    /* [in] */ IBaseFilter __RPC_FAR *pFilter,
    /* [string][in] */ LPCWSTR pName);


void __RPC_STUB IFilterGraph_AddFilter_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IFilterGraph_RemoveFilter_Proxy( 
    IFilterGraph __RPC_FAR * This,
    /* [in] */ IBaseFilter __RPC_FAR *pFilter);


void __RPC_STUB IFilterGraph_RemoveFilter_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IFilterGraph_EnumFilters_Proxy( 
    IFilterGraph __RPC_FAR * This,
    /* [out] */ IEnumFilters __RPC_FAR *__RPC_FAR *ppEnum);


void __RPC_STUB IFilterGraph_EnumFilters_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IFilterGraph_FindFilterByName_Proxy( 
    IFilterGraph __RPC_FAR * This,
    /* [string][in] */ LPCWSTR pName,
    /* [out] */ IBaseFilter __RPC_FAR *__RPC_FAR *ppFilter);


void __RPC_STUB IFilterGraph_FindFilterByName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IFilterGraph_ConnectDirect_Proxy( 
    IFilterGraph __RPC_FAR * This,
    /* [in] */ IPin __RPC_FAR *ppinOut,
    /* [in] */ IPin __RPC_FAR *ppinIn,
    /* [in] */ const AM_MEDIA_TYPE __RPC_FAR *pmt);


void __RPC_STUB IFilterGraph_ConnectDirect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IFilterGraph_Reconnect_Proxy( 
    IFilterGraph __RPC_FAR * This,
    /* [in] */ IPin __RPC_FAR *ppin);


void __RPC_STUB IFilterGraph_Reconnect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IFilterGraph_Disconnect_Proxy( 
    IFilterGraph __RPC_FAR * This,
    /* [in] */ IPin __RPC_FAR *ppin);


void __RPC_STUB IFilterGraph_Disconnect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IFilterGraph_SetDefaultSyncSource_Proxy( 
    IFilterGraph __RPC_FAR * This);


void __RPC_STUB IFilterGraph_SetDefaultSyncSource_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IFilterGraph_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_strmif_0060 */
/* [local] */ 

typedef IFilterGraph __RPC_FAR *PFILTERGRAPH;



extern RPC_IF_HANDLE __MIDL_itf_strmif_0060_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_strmif_0060_v0_0_s_ifspec;

#ifndef __IEnumFilters_INTERFACE_DEFINED__
#define __IEnumFilters_INTERFACE_DEFINED__

/* interface IEnumFilters */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IEnumFilters;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("56a86893-0ad4-11ce-b03a-0020af0ba770")
    IEnumFilters : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Next( 
            /* [in] */ ULONG cFilters,
            /* [out] */ IBaseFilter __RPC_FAR *__RPC_FAR *ppFilter,
            /* [out] */ ULONG __RPC_FAR *pcFetched) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Skip( 
            /* [in] */ ULONG cFilters) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Reset( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Clone( 
            /* [out] */ IEnumFilters __RPC_FAR *__RPC_FAR *ppEnum) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IEnumFiltersVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IEnumFilters __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IEnumFilters __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IEnumFilters __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Next )( 
            IEnumFilters __RPC_FAR * This,
            /* [in] */ ULONG cFilters,
            /* [out] */ IBaseFilter __RPC_FAR *__RPC_FAR *ppFilter,
            /* [out] */ ULONG __RPC_FAR *pcFetched);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Skip )( 
            IEnumFilters __RPC_FAR * This,
            /* [in] */ ULONG cFilters);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Reset )( 
            IEnumFilters __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Clone )( 
            IEnumFilters __RPC_FAR * This,
            /* [out] */ IEnumFilters __RPC_FAR *__RPC_FAR *ppEnum);
        
        END_INTERFACE
    } IEnumFiltersVtbl;

    interface IEnumFilters
    {
        CONST_VTBL struct IEnumFiltersVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IEnumFilters_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IEnumFilters_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IEnumFilters_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IEnumFilters_Next(This,cFilters,ppFilter,pcFetched)	\
    (This)->lpVtbl -> Next(This,cFilters,ppFilter,pcFetched)

#define IEnumFilters_Skip(This,cFilters)	\
    (This)->lpVtbl -> Skip(This,cFilters)

#define IEnumFilters_Reset(This)	\
    (This)->lpVtbl -> Reset(This)

#define IEnumFilters_Clone(This,ppEnum)	\
    (This)->lpVtbl -> Clone(This,ppEnum)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IEnumFilters_Next_Proxy( 
    IEnumFilters __RPC_FAR * This,
    /* [in] */ ULONG cFilters,
    /* [out] */ IBaseFilter __RPC_FAR *__RPC_FAR *ppFilter,
    /* [out] */ ULONG __RPC_FAR *pcFetched);


void __RPC_STUB IEnumFilters_Next_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IEnumFilters_Skip_Proxy( 
    IEnumFilters __RPC_FAR * This,
    /* [in] */ ULONG cFilters);


void __RPC_STUB IEnumFilters_Skip_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IEnumFilters_Reset_Proxy( 
    IEnumFilters __RPC_FAR * This);


void __RPC_STUB IEnumFilters_Reset_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IEnumFilters_Clone_Proxy( 
    IEnumFilters __RPC_FAR * This,
    /* [out] */ IEnumFilters __RPC_FAR *__RPC_FAR *ppEnum);


void __RPC_STUB IEnumFilters_Clone_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IEnumFilters_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_strmif_0061 */
/* [local] */ 

typedef IEnumFilters __RPC_FAR *PENUMFILTERS;



extern RPC_IF_HANDLE __MIDL_itf_strmif_0061_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_strmif_0061_v0_0_s_ifspec;

#ifndef __IMediaFilter_INTERFACE_DEFINED__
#define __IMediaFilter_INTERFACE_DEFINED__

/* interface IMediaFilter */
/* [unique][uuid][object] */ 

typedef 
enum _FilterState
    {	State_Stopped	= 0,
	State_Paused	= State_Stopped + 1,
	State_Running	= State_Paused + 1
    }	FILTER_STATE;


EXTERN_C const IID IID_IMediaFilter;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("56a86899-0ad4-11ce-b03a-0020af0ba770")
    IMediaFilter : public IPersist
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Stop( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Pause( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Run( 
            REFERENCE_TIME tStart) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetState( 
            /* [in] */ DWORD dwMilliSecsTimeout,
            /* [out] */ FILTER_STATE __RPC_FAR *State) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetSyncSource( 
            /* [in] */ IReferenceClock __RPC_FAR *pClock) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetSyncSource( 
            /* [out] */ IReferenceClock __RPC_FAR *__RPC_FAR *pClock) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMediaFilterVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IMediaFilter __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IMediaFilter __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IMediaFilter __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetClassID )( 
            IMediaFilter __RPC_FAR * This,
            /* [out] */ CLSID __RPC_FAR *pClassID);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Stop )( 
            IMediaFilter __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Pause )( 
            IMediaFilter __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Run )( 
            IMediaFilter __RPC_FAR * This,
            REFERENCE_TIME tStart);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetState )( 
            IMediaFilter __RPC_FAR * This,
            /* [in] */ DWORD dwMilliSecsTimeout,
            /* [out] */ FILTER_STATE __RPC_FAR *State);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetSyncSource )( 
            IMediaFilter __RPC_FAR * This,
            /* [in] */ IReferenceClock __RPC_FAR *pClock);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetSyncSource )( 
            IMediaFilter __RPC_FAR * This,
            /* [out] */ IReferenceClock __RPC_FAR *__RPC_FAR *pClock);
        
        END_INTERFACE
    } IMediaFilterVtbl;

    interface IMediaFilter
    {
        CONST_VTBL struct IMediaFilterVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMediaFilter_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IMediaFilter_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IMediaFilter_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IMediaFilter_GetClassID(This,pClassID)	\
    (This)->lpVtbl -> GetClassID(This,pClassID)


#define IMediaFilter_Stop(This)	\
    (This)->lpVtbl -> Stop(This)

#define IMediaFilter_Pause(This)	\
    (This)->lpVtbl -> Pause(This)

#define IMediaFilter_Run(This,tStart)	\
    (This)->lpVtbl -> Run(This,tStart)

#define IMediaFilter_GetState(This,dwMilliSecsTimeout,State)	\
    (This)->lpVtbl -> GetState(This,dwMilliSecsTimeout,State)

#define IMediaFilter_SetSyncSource(This,pClock)	\
    (This)->lpVtbl -> SetSyncSource(This,pClock)

#define IMediaFilter_GetSyncSource(This,pClock)	\
    (This)->lpVtbl -> GetSyncSource(This,pClock)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IMediaFilter_Stop_Proxy( 
    IMediaFilter __RPC_FAR * This);


void __RPC_STUB IMediaFilter_Stop_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMediaFilter_Pause_Proxy( 
    IMediaFilter __RPC_FAR * This);


void __RPC_STUB IMediaFilter_Pause_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMediaFilter_Run_Proxy( 
    IMediaFilter __RPC_FAR * This,
    REFERENCE_TIME tStart);


void __RPC_STUB IMediaFilter_Run_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMediaFilter_GetState_Proxy( 
    IMediaFilter __RPC_FAR * This,
    /* [in] */ DWORD dwMilliSecsTimeout,
    /* [out] */ FILTER_STATE __RPC_FAR *State);


void __RPC_STUB IMediaFilter_GetState_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMediaFilter_SetSyncSource_Proxy( 
    IMediaFilter __RPC_FAR * This,
    /* [in] */ IReferenceClock __RPC_FAR *pClock);


void __RPC_STUB IMediaFilter_SetSyncSource_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMediaFilter_GetSyncSource_Proxy( 
    IMediaFilter __RPC_FAR * This,
    /* [out] */ IReferenceClock __RPC_FAR *__RPC_FAR *pClock);


void __RPC_STUB IMediaFilter_GetSyncSource_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IMediaFilter_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_strmif_0062 */
/* [local] */ 

typedef IMediaFilter __RPC_FAR *PMEDIAFILTER;



extern RPC_IF_HANDLE __MIDL_itf_strmif_0062_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_strmif_0062_v0_0_s_ifspec;

#ifndef __IBaseFilter_INTERFACE_DEFINED__
#define __IBaseFilter_INTERFACE_DEFINED__

/* interface IBaseFilter */
/* [unique][uuid][object] */ 

typedef struct _FilterInfo
    {
    WCHAR achName[ 128 ];
    IFilterGraph __RPC_FAR *pGraph;
    }	FILTER_INFO;


EXTERN_C const IID IID_IBaseFilter;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("56a86895-0ad4-11ce-b03a-0020af0ba770")
    IBaseFilter : public IMediaFilter
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE EnumPins( 
            /* [out] */ IEnumPins __RPC_FAR *__RPC_FAR *ppEnum) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE FindPin( 
            /* [string][in] */ LPCWSTR Id,
            /* [out] */ IPin __RPC_FAR *__RPC_FAR *ppPin) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE QueryFilterInfo( 
            /* [out] */ FILTER_INFO __RPC_FAR *pInfo) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE JoinFilterGraph( 
            /* [in] */ IFilterGraph __RPC_FAR *pGraph,
            /* [string][in] */ LPCWSTR pName) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE QueryVendorInfo( 
            /* [string][out] */ LPWSTR __RPC_FAR *pVendorInfo) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IBaseFilterVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IBaseFilter __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IBaseFilter __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IBaseFilter __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetClassID )( 
            IBaseFilter __RPC_FAR * This,
            /* [out] */ CLSID __RPC_FAR *pClassID);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Stop )( 
            IBaseFilter __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Pause )( 
            IBaseFilter __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Run )( 
            IBaseFilter __RPC_FAR * This,
            REFERENCE_TIME tStart);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetState )( 
            IBaseFilter __RPC_FAR * This,
            /* [in] */ DWORD dwMilliSecsTimeout,
            /* [out] */ FILTER_STATE __RPC_FAR *State);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetSyncSource )( 
            IBaseFilter __RPC_FAR * This,
            /* [in] */ IReferenceClock __RPC_FAR *pClock);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetSyncSource )( 
            IBaseFilter __RPC_FAR * This,
            /* [out] */ IReferenceClock __RPC_FAR *__RPC_FAR *pClock);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *EnumPins )( 
            IBaseFilter __RPC_FAR * This,
            /* [out] */ IEnumPins __RPC_FAR *__RPC_FAR *ppEnum);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *FindPin )( 
            IBaseFilter __RPC_FAR * This,
            /* [string][in] */ LPCWSTR Id,
            /* [out] */ IPin __RPC_FAR *__RPC_FAR *ppPin);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryFilterInfo )( 
            IBaseFilter __RPC_FAR * This,
            /* [out] */ FILTER_INFO __RPC_FAR *pInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *JoinFilterGraph )( 
            IBaseFilter __RPC_FAR * This,
            /* [in] */ IFilterGraph __RPC_FAR *pGraph,
            /* [string][in] */ LPCWSTR pName);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryVendorInfo )( 
            IBaseFilter __RPC_FAR * This,
            /* [string][out] */ LPWSTR __RPC_FAR *pVendorInfo);
        
        END_INTERFACE
    } IBaseFilterVtbl;

    interface IBaseFilter
    {
        CONST_VTBL struct IBaseFilterVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IBaseFilter_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IBaseFilter_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IBaseFilter_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IBaseFilter_GetClassID(This,pClassID)	\
    (This)->lpVtbl -> GetClassID(This,pClassID)


#define IBaseFilter_Stop(This)	\
    (This)->lpVtbl -> Stop(This)

#define IBaseFilter_Pause(This)	\
    (This)->lpVtbl -> Pause(This)

#define IBaseFilter_Run(This,tStart)	\
    (This)->lpVtbl -> Run(This,tStart)

#define IBaseFilter_GetState(This,dwMilliSecsTimeout,State)	\
    (This)->lpVtbl -> GetState(This,dwMilliSecsTimeout,State)

#define IBaseFilter_SetSyncSource(This,pClock)	\
    (This)->lpVtbl -> SetSyncSource(This,pClock)

#define IBaseFilter_GetSyncSource(This,pClock)	\
    (This)->lpVtbl -> GetSyncSource(This,pClock)


#define IBaseFilter_EnumPins(This,ppEnum)	\
    (This)->lpVtbl -> EnumPins(This,ppEnum)

#define IBaseFilter_FindPin(This,Id,ppPin)	\
    (This)->lpVtbl -> FindPin(This,Id,ppPin)

#define IBaseFilter_QueryFilterInfo(This,pInfo)	\
    (This)->lpVtbl -> QueryFilterInfo(This,pInfo)

#define IBaseFilter_JoinFilterGraph(This,pGraph,pName)	\
    (This)->lpVtbl -> JoinFilterGraph(This,pGraph,pName)

#define IBaseFilter_QueryVendorInfo(This,pVendorInfo)	\
    (This)->lpVtbl -> QueryVendorInfo(This,pVendorInfo)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IBaseFilter_EnumPins_Proxy( 
    IBaseFilter __RPC_FAR * This,
    /* [out] */ IEnumPins __RPC_FAR *__RPC_FAR *ppEnum);


void __RPC_STUB IBaseFilter_EnumPins_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IBaseFilter_FindPin_Proxy( 
    IBaseFilter __RPC_FAR * This,
    /* [string][in] */ LPCWSTR Id,
    /* [out] */ IPin __RPC_FAR *__RPC_FAR *ppPin);


void __RPC_STUB IBaseFilter_FindPin_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IBaseFilter_QueryFilterInfo_Proxy( 
    IBaseFilter __RPC_FAR * This,
    /* [out] */ FILTER_INFO __RPC_FAR *pInfo);


void __RPC_STUB IBaseFilter_QueryFilterInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IBaseFilter_JoinFilterGraph_Proxy( 
    IBaseFilter __RPC_FAR * This,
    /* [in] */ IFilterGraph __RPC_FAR *pGraph,
    /* [string][in] */ LPCWSTR pName);


void __RPC_STUB IBaseFilter_JoinFilterGraph_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IBaseFilter_QueryVendorInfo_Proxy( 
    IBaseFilter __RPC_FAR * This,
    /* [string][out] */ LPWSTR __RPC_FAR *pVendorInfo);


void __RPC_STUB IBaseFilter_QueryVendorInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IBaseFilter_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_strmif_0063 */
/* [local] */ 

typedef IBaseFilter __RPC_FAR *PFILTER;



extern RPC_IF_HANDLE __MIDL_itf_strmif_0063_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_strmif_0063_v0_0_s_ifspec;

#ifndef __IReferenceClock_INTERFACE_DEFINED__
#define __IReferenceClock_INTERFACE_DEFINED__

/* interface IReferenceClock */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IReferenceClock;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("56a86897-0ad4-11ce-b03a-0020af0ba770")
    IReferenceClock : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetTime( 
            /* [out] */ REFERENCE_TIME __RPC_FAR *pTime) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AdviseTime( 
            /* [in] */ REFERENCE_TIME baseTime,
            /* [in] */ REFERENCE_TIME streamTime,
            /* [in] */ HEVENT hEvent,
            /* [out] */ DWORD_PTR __RPC_FAR *pdwAdviseCookie) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AdvisePeriodic( 
            /* [in] */ REFERENCE_TIME startTime,
            /* [in] */ REFERENCE_TIME periodTime,
            /* [in] */ HSEMAPHORE hSemaphore,
            /* [out] */ DWORD_PTR __RPC_FAR *pdwAdviseCookie) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Unadvise( 
            /* [in] */ DWORD_PTR dwAdviseCookie) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IReferenceClockVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IReferenceClock __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IReferenceClock __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IReferenceClock __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTime )( 
            IReferenceClock __RPC_FAR * This,
            /* [out] */ REFERENCE_TIME __RPC_FAR *pTime);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AdviseTime )( 
            IReferenceClock __RPC_FAR * This,
            /* [in] */ REFERENCE_TIME baseTime,
            /* [in] */ REFERENCE_TIME streamTime,
            /* [in] */ HEVENT hEvent,
            /* [out] */ DWORD_PTR __RPC_FAR *pdwAdviseCookie);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AdvisePeriodic )( 
            IReferenceClock __RPC_FAR * This,
            /* [in] */ REFERENCE_TIME startTime,
            /* [in] */ REFERENCE_TIME periodTime,
            /* [in] */ HSEMAPHORE hSemaphore,
            /* [out] */ DWORD_PTR __RPC_FAR *pdwAdviseCookie);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Unadvise )( 
            IReferenceClock __RPC_FAR * This,
            /* [in] */ DWORD_PTR dwAdviseCookie);
        
        END_INTERFACE
    } IReferenceClockVtbl;

    interface IReferenceClock
    {
        CONST_VTBL struct IReferenceClockVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IReferenceClock_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IReferenceClock_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IReferenceClock_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IReferenceClock_GetTime(This,pTime)	\
    (This)->lpVtbl -> GetTime(This,pTime)

#define IReferenceClock_AdviseTime(This,baseTime,streamTime,hEvent,pdwAdviseCookie)	\
    (This)->lpVtbl -> AdviseTime(This,baseTime,streamTime,hEvent,pdwAdviseCookie)

#define IReferenceClock_AdvisePeriodic(This,startTime,periodTime,hSemaphore,pdwAdviseCookie)	\
    (This)->lpVtbl -> AdvisePeriodic(This,startTime,periodTime,hSemaphore,pdwAdviseCookie)

#define IReferenceClock_Unadvise(This,dwAdviseCookie)	\
    (This)->lpVtbl -> Unadvise(This,dwAdviseCookie)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IReferenceClock_GetTime_Proxy( 
    IReferenceClock __RPC_FAR * This,
    /* [out] */ REFERENCE_TIME __RPC_FAR *pTime);


void __RPC_STUB IReferenceClock_GetTime_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IReferenceClock_AdviseTime_Proxy( 
    IReferenceClock __RPC_FAR * This,
    /* [in] */ REFERENCE_TIME baseTime,
    /* [in] */ REFERENCE_TIME streamTime,
    /* [in] */ HEVENT hEvent,
    /* [out] */ DWORD_PTR __RPC_FAR *pdwAdviseCookie);


void __RPC_STUB IReferenceClock_AdviseTime_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IReferenceClock_AdvisePeriodic_Proxy( 
    IReferenceClock __RPC_FAR * This,
    /* [in] */ REFERENCE_TIME startTime,
    /* [in] */ REFERENCE_TIME periodTime,
    /* [in] */ HSEMAPHORE hSemaphore,
    /* [out] */ DWORD_PTR __RPC_FAR *pdwAdviseCookie);


void __RPC_STUB IReferenceClock_AdvisePeriodic_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IReferenceClock_Unadvise_Proxy( 
    IReferenceClock __RPC_FAR * This,
    /* [in] */ DWORD_PTR dwAdviseCookie);


void __RPC_STUB IReferenceClock_Unadvise_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IReferenceClock_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_strmif_0064 */
/* [local] */ 

typedef IReferenceClock __RPC_FAR *PREFERENCECLOCK;



extern RPC_IF_HANDLE __MIDL_itf_strmif_0064_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_strmif_0064_v0_0_s_ifspec;

#ifndef __IReferenceClock2_INTERFACE_DEFINED__
#define __IReferenceClock2_INTERFACE_DEFINED__

/* interface IReferenceClock2 */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IReferenceClock2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("36b73885-c2c8-11cf-8b46-00805f6cef60")
    IReferenceClock2 : public IReferenceClock
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IReferenceClock2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IReferenceClock2 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IReferenceClock2 __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IReferenceClock2 __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTime )( 
            IReferenceClock2 __RPC_FAR * This,
            /* [out] */ REFERENCE_TIME __RPC_FAR *pTime);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AdviseTime )( 
            IReferenceClock2 __RPC_FAR * This,
            /* [in] */ REFERENCE_TIME baseTime,
            /* [in] */ REFERENCE_TIME streamTime,
            /* [in] */ HEVENT hEvent,
            /* [out] */ DWORD_PTR __RPC_FAR *pdwAdviseCookie);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AdvisePeriodic )( 
            IReferenceClock2 __RPC_FAR * This,
            /* [in] */ REFERENCE_TIME startTime,
            /* [in] */ REFERENCE_TIME periodTime,
            /* [in] */ HSEMAPHORE hSemaphore,
            /* [out] */ DWORD_PTR __RPC_FAR *pdwAdviseCookie);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Unadvise )( 
            IReferenceClock2 __RPC_FAR * This,
            /* [in] */ DWORD_PTR dwAdviseCookie);
        
        END_INTERFACE
    } IReferenceClock2Vtbl;

    interface IReferenceClock2
    {
        CONST_VTBL struct IReferenceClock2Vtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IReferenceClock2_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IReferenceClock2_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IReferenceClock2_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IReferenceClock2_GetTime(This,pTime)	\
    (This)->lpVtbl -> GetTime(This,pTime)

#define IReferenceClock2_AdviseTime(This,baseTime,streamTime,hEvent,pdwAdviseCookie)	\
    (This)->lpVtbl -> AdviseTime(This,baseTime,streamTime,hEvent,pdwAdviseCookie)

#define IReferenceClock2_AdvisePeriodic(This,startTime,periodTime,hSemaphore,pdwAdviseCookie)	\
    (This)->lpVtbl -> AdvisePeriodic(This,startTime,periodTime,hSemaphore,pdwAdviseCookie)

#define IReferenceClock2_Unadvise(This,dwAdviseCookie)	\
    (This)->lpVtbl -> Unadvise(This,dwAdviseCookie)


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IReferenceClock2_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_strmif_0065 */
/* [local] */ 

typedef IReferenceClock2 __RPC_FAR *PREFERENCECLOCK2;



extern RPC_IF_HANDLE __MIDL_itf_strmif_0065_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_strmif_0065_v0_0_s_ifspec;

#ifndef __IMediaSample_INTERFACE_DEFINED__
#define __IMediaSample_INTERFACE_DEFINED__

/* interface IMediaSample */
/* [unique][uuid][object][local] */ 


EXTERN_C const IID IID_IMediaSample;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("56a8689a-0ad4-11ce-b03a-0020af0ba770")
    IMediaSample : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetPointer( 
            /* [out] */ BYTE __RPC_FAR *__RPC_FAR *ppBuffer) = 0;
        
        virtual long STDMETHODCALLTYPE GetSize( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetTime( 
            /* [out] */ REFERENCE_TIME __RPC_FAR *pTimeStart,
            /* [out] */ REFERENCE_TIME __RPC_FAR *pTimeEnd) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetTime( 
            /* [in] */ REFERENCE_TIME __RPC_FAR *pTimeStart,
            /* [in] */ REFERENCE_TIME __RPC_FAR *pTimeEnd) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE IsSyncPoint( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetSyncPoint( 
            BOOL bIsSyncPoint) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE IsPreroll( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetPreroll( 
            BOOL bIsPreroll) = 0;
        
        virtual long STDMETHODCALLTYPE GetActualDataLength( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetActualDataLength( 
            long __MIDL_0004) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetMediaType( 
            AM_MEDIA_TYPE __RPC_FAR *__RPC_FAR *ppMediaType) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetMediaType( 
            AM_MEDIA_TYPE __RPC_FAR *pMediaType) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE IsDiscontinuity( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetDiscontinuity( 
            BOOL bDiscontinuity) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetMediaTime( 
            /* [out] */ LONGLONG __RPC_FAR *pTimeStart,
            /* [out] */ LONGLONG __RPC_FAR *pTimeEnd) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetMediaTime( 
            /* [in] */ LONGLONG __RPC_FAR *pTimeStart,
            /* [in] */ LONGLONG __RPC_FAR *pTimeEnd) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMediaSampleVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IMediaSample __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IMediaSample __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IMediaSample __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetPointer )( 
            IMediaSample __RPC_FAR * This,
            /* [out] */ BYTE __RPC_FAR *__RPC_FAR *ppBuffer);
        
        long ( STDMETHODCALLTYPE __RPC_FAR *GetSize )( 
            IMediaSample __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTime )( 
            IMediaSample __RPC_FAR * This,
            /* [out] */ REFERENCE_TIME __RPC_FAR *pTimeStart,
            /* [out] */ REFERENCE_TIME __RPC_FAR *pTimeEnd);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetTime )( 
            IMediaSample __RPC_FAR * This,
            /* [in] */ REFERENCE_TIME __RPC_FAR *pTimeStart,
            /* [in] */ REFERENCE_TIME __RPC_FAR *pTimeEnd);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsSyncPoint )( 
            IMediaSample __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetSyncPoint )( 
            IMediaSample __RPC_FAR * This,
            BOOL bIsSyncPoint);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsPreroll )( 
            IMediaSample __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetPreroll )( 
            IMediaSample __RPC_FAR * This,
            BOOL bIsPreroll);
        
        long ( STDMETHODCALLTYPE __RPC_FAR *GetActualDataLength )( 
            IMediaSample __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetActualDataLength )( 
            IMediaSample __RPC_FAR * This,
            long __MIDL_0004);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMediaType )( 
            IMediaSample __RPC_FAR * This,
            AM_MEDIA_TYPE __RPC_FAR *__RPC_FAR *ppMediaType);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetMediaType )( 
            IMediaSample __RPC_FAR * This,
            AM_MEDIA_TYPE __RPC_FAR *pMediaType);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsDiscontinuity )( 
            IMediaSample __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetDiscontinuity )( 
            IMediaSample __RPC_FAR * This,
            BOOL bDiscontinuity);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMediaTime )( 
            IMediaSample __RPC_FAR * This,
            /* [out] */ LONGLONG __RPC_FAR *pTimeStart,
            /* [out] */ LONGLONG __RPC_FAR *pTimeEnd);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetMediaTime )( 
            IMediaSample __RPC_FAR * This,
            /* [in] */ LONGLONG __RPC_FAR *pTimeStart,
            /* [in] */ LONGLONG __RPC_FAR *pTimeEnd);
        
        END_INTERFACE
    } IMediaSampleVtbl;

    interface IMediaSample
    {
        CONST_VTBL struct IMediaSampleVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMediaSample_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IMediaSample_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IMediaSample_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IMediaSample_GetPointer(This,ppBuffer)	\
    (This)->lpVtbl -> GetPointer(This,ppBuffer)

#define IMediaSample_GetSize(This)	\
    (This)->lpVtbl -> GetSize(This)

#define IMediaSample_GetTime(This,pTimeStart,pTimeEnd)	\
    (This)->lpVtbl -> GetTime(This,pTimeStart,pTimeEnd)

#define IMediaSample_SetTime(This,pTimeStart,pTimeEnd)	\
    (This)->lpVtbl -> SetTime(This,pTimeStart,pTimeEnd)

#define IMediaSample_IsSyncPoint(This)	\
    (This)->lpVtbl -> IsSyncPoint(This)

#define IMediaSample_SetSyncPoint(This,bIsSyncPoint)	\
    (This)->lpVtbl -> SetSyncPoint(This,bIsSyncPoint)

#define IMediaSample_IsPreroll(This)	\
    (This)->lpVtbl -> IsPreroll(This)

#define IMediaSample_SetPreroll(This,bIsPreroll)	\
    (This)->lpVtbl -> SetPreroll(This,bIsPreroll)

#define IMediaSample_GetActualDataLength(This)	\
    (This)->lpVtbl -> GetActualDataLength(This)

#define IMediaSample_SetActualDataLength(This,__MIDL_0004)	\
    (This)->lpVtbl -> SetActualDataLength(This,__MIDL_0004)

#define IMediaSample_GetMediaType(This,ppMediaType)	\
    (This)->lpVtbl -> GetMediaType(This,ppMediaType)

#define IMediaSample_SetMediaType(This,pMediaType)	\
    (This)->lpVtbl -> SetMediaType(This,pMediaType)

#define IMediaSample_IsDiscontinuity(This)	\
    (This)->lpVtbl -> IsDiscontinuity(This)

#define IMediaSample_SetDiscontinuity(This,bDiscontinuity)	\
    (This)->lpVtbl -> SetDiscontinuity(This,bDiscontinuity)

#define IMediaSample_GetMediaTime(This,pTimeStart,pTimeEnd)	\
    (This)->lpVtbl -> GetMediaTime(This,pTimeStart,pTimeEnd)

#define IMediaSample_SetMediaTime(This,pTimeStart,pTimeEnd)	\
    (This)->lpVtbl -> SetMediaTime(This,pTimeStart,pTimeEnd)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IMediaSample_GetPointer_Proxy( 
    IMediaSample __RPC_FAR * This,
    /* [out] */ BYTE __RPC_FAR *__RPC_FAR *ppBuffer);


void __RPC_STUB IMediaSample_GetPointer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


long STDMETHODCALLTYPE IMediaSample_GetSize_Proxy( 
    IMediaSample __RPC_FAR * This);


void __RPC_STUB IMediaSample_GetSize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMediaSample_GetTime_Proxy( 
    IMediaSample __RPC_FAR * This,
    /* [out] */ REFERENCE_TIME __RPC_FAR *pTimeStart,
    /* [out] */ REFERENCE_TIME __RPC_FAR *pTimeEnd);


void __RPC_STUB IMediaSample_GetTime_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMediaSample_SetTime_Proxy( 
    IMediaSample __RPC_FAR * This,
    /* [in] */ REFERENCE_TIME __RPC_FAR *pTimeStart,
    /* [in] */ REFERENCE_TIME __RPC_FAR *pTimeEnd);


void __RPC_STUB IMediaSample_SetTime_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMediaSample_IsSyncPoint_Proxy( 
    IMediaSample __RPC_FAR * This);


void __RPC_STUB IMediaSample_IsSyncPoint_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMediaSample_SetSyncPoint_Proxy( 
    IMediaSample __RPC_FAR * This,
    BOOL bIsSyncPoint);


void __RPC_STUB IMediaSample_SetSyncPoint_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMediaSample_IsPreroll_Proxy( 
    IMediaSample __RPC_FAR * This);


void __RPC_STUB IMediaSample_IsPreroll_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMediaSample_SetPreroll_Proxy( 
    IMediaSample __RPC_FAR * This,
    BOOL bIsPreroll);


void __RPC_STUB IMediaSample_SetPreroll_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


long STDMETHODCALLTYPE IMediaSample_GetActualDataLength_Proxy( 
    IMediaSample __RPC_FAR * This);


void __RPC_STUB IMediaSample_GetActualDataLength_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMediaSample_SetActualDataLength_Proxy( 
    IMediaSample __RPC_FAR * This,
    long __MIDL_0004);


void __RPC_STUB IMediaSample_SetActualDataLength_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMediaSample_GetMediaType_Proxy( 
    IMediaSample __RPC_FAR * This,
    AM_MEDIA_TYPE __RPC_FAR *__RPC_FAR *ppMediaType);


void __RPC_STUB IMediaSample_GetMediaType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMediaSample_SetMediaType_Proxy( 
    IMediaSample __RPC_FAR * This,
    AM_MEDIA_TYPE __RPC_FAR *pMediaType);


void __RPC_STUB IMediaSample_SetMediaType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMediaSample_IsDiscontinuity_Proxy( 
    IMediaSample __RPC_FAR * This);


void __RPC_STUB IMediaSample_IsDiscontinuity_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMediaSample_SetDiscontinuity_Proxy( 
    IMediaSample __RPC_FAR * This,
    BOOL bDiscontinuity);


void __RPC_STUB IMediaSample_SetDiscontinuity_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMediaSample_GetMediaTime_Proxy( 
    IMediaSample __RPC_FAR * This,
    /* [out] */ LONGLONG __RPC_FAR *pTimeStart,
    /* [out] */ LONGLONG __RPC_FAR *pTimeEnd);


void __RPC_STUB IMediaSample_GetMediaTime_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMediaSample_SetMediaTime_Proxy( 
    IMediaSample __RPC_FAR * This,
    /* [in] */ LONGLONG __RPC_FAR *pTimeStart,
    /* [in] */ LONGLONG __RPC_FAR *pTimeEnd);


void __RPC_STUB IMediaSample_SetMediaTime_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IMediaSample_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_strmif_0066 */
/* [local] */ 

typedef IMediaSample __RPC_FAR *PMEDIASAMPLE;


enum tagAM_SAMPLE_PROPERTY_FLAGS
    {	AM_SAMPLE_SPLICEPOINT	= 0x1,
	AM_SAMPLE_PREROLL	= 0x2,
	AM_SAMPLE_DATADISCONTINUITY	= 0x4,
	AM_SAMPLE_TYPECHANGED	= 0x8,
	AM_SAMPLE_TIMEVALID	= 0x10,
	AM_SAMPLE_TIMEDISCONTINUITY	= 0x40,
	AM_SAMPLE_FLUSH_ON_PAUSE	= 0x80,
	AM_SAMPLE_STOPVALID	= 0x100,
	AM_SAMPLE_ENDOFSTREAM	= 0x200,
	AM_STREAM_MEDIA	= 0,
	AM_STREAM_CONTROL	= 1
    };
typedef struct tagAM_SAMPLE2_PROPERTIES
    {
    DWORD cbData;
    DWORD dwTypeSpecificFlags;
    DWORD dwSampleFlags;
    LONG lActual;
    REFERENCE_TIME tStart;
    REFERENCE_TIME tStop;
    DWORD dwStreamId;
    AM_MEDIA_TYPE __RPC_FAR *pMediaType;
    BYTE __RPC_FAR *pbBuffer;
    LONG cbBuffer;
    }	AM_SAMPLE2_PROPERTIES;



extern RPC_IF_HANDLE __MIDL_itf_strmif_0066_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_strmif_0066_v0_0_s_ifspec;

#ifndef __IMediaSample2_INTERFACE_DEFINED__
#define __IMediaSample2_INTERFACE_DEFINED__

/* interface IMediaSample2 */
/* [unique][uuid][object][local] */ 


EXTERN_C const IID IID_IMediaSample2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("36b73884-c2c8-11cf-8b46-00805f6cef60")
    IMediaSample2 : public IMediaSample
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetProperties( 
            /* [in] */ DWORD cbProperties,
            /* [size_is][out] */ BYTE __RPC_FAR *pbProperties) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetProperties( 
            /* [in] */ DWORD cbProperties,
            /* [size_is][in] */ const BYTE __RPC_FAR *pbProperties) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMediaSample2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IMediaSample2 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IMediaSample2 __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IMediaSample2 __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetPointer )( 
            IMediaSample2 __RPC_FAR * This,
            /* [out] */ BYTE __RPC_FAR *__RPC_FAR *ppBuffer);
        
        long ( STDMETHODCALLTYPE __RPC_FAR *GetSize )( 
            IMediaSample2 __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTime )( 
            IMediaSample2 __RPC_FAR * This,
            /* [out] */ REFERENCE_TIME __RPC_FAR *pTimeStart,
            /* [out] */ REFERENCE_TIME __RPC_FAR *pTimeEnd);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetTime )( 
            IMediaSample2 __RPC_FAR * This,
            /* [in] */ REFERENCE_TIME __RPC_FAR *pTimeStart,
            /* [in] */ REFERENCE_TIME __RPC_FAR *pTimeEnd);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsSyncPoint )( 
            IMediaSample2 __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetSyncPoint )( 
            IMediaSample2 __RPC_FAR * This,
            BOOL bIsSyncPoint);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsPreroll )( 
            IMediaSample2 __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetPreroll )( 
            IMediaSample2 __RPC_FAR * This,
            BOOL bIsPreroll);
        
        long ( STDMETHODCALLTYPE __RPC_FAR *GetActualDataLength )( 
            IMediaSample2 __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetActualDataLength )( 
            IMediaSample2 __RPC_FAR * This,
            long __MIDL_0004);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMediaType )( 
            IMediaSample2 __RPC_FAR * This,
            AM_MEDIA_TYPE __RPC_FAR *__RPC_FAR *ppMediaType);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetMediaType )( 
            IMediaSample2 __RPC_FAR * This,
            AM_MEDIA_TYPE __RPC_FAR *pMediaType);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsDiscontinuity )( 
            IMediaSample2 __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetDiscontinuity )( 
            IMediaSample2 __RPC_FAR * This,
            BOOL bDiscontinuity);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMediaTime )( 
            IMediaSample2 __RPC_FAR * This,
            /* [out] */ LONGLONG __RPC_FAR *pTimeStart,
            /* [out] */ LONGLONG __RPC_FAR *pTimeEnd);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetMediaTime )( 
            IMediaSample2 __RPC_FAR * This,
            /* [in] */ LONGLONG __RPC_FAR *pTimeStart,
            /* [in] */ LONGLONG __RPC_FAR *pTimeEnd);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetProperties )( 
            IMediaSample2 __RPC_FAR * This,
            /* [in] */ DWORD cbProperties,
            /* [size_is][out] */ BYTE __RPC_FAR *pbProperties);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetProperties )( 
            IMediaSample2 __RPC_FAR * This,
            /* [in] */ DWORD cbProperties,
            /* [size_is][in] */ const BYTE __RPC_FAR *pbProperties);
        
        END_INTERFACE
    } IMediaSample2Vtbl;

    interface IMediaSample2
    {
        CONST_VTBL struct IMediaSample2Vtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMediaSample2_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IMediaSample2_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IMediaSample2_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IMediaSample2_GetPointer(This,ppBuffer)	\
    (This)->lpVtbl -> GetPointer(This,ppBuffer)

#define IMediaSample2_GetSize(This)	\
    (This)->lpVtbl -> GetSize(This)

#define IMediaSample2_GetTime(This,pTimeStart,pTimeEnd)	\
    (This)->lpVtbl -> GetTime(This,pTimeStart,pTimeEnd)

#define IMediaSample2_SetTime(This,pTimeStart,pTimeEnd)	\
    (This)->lpVtbl -> SetTime(This,pTimeStart,pTimeEnd)

#define IMediaSample2_IsSyncPoint(This)	\
    (This)->lpVtbl -> IsSyncPoint(This)

#define IMediaSample2_SetSyncPoint(This,bIsSyncPoint)	\
    (This)->lpVtbl -> SetSyncPoint(This,bIsSyncPoint)

#define IMediaSample2_IsPreroll(This)	\
    (This)->lpVtbl -> IsPreroll(This)

#define IMediaSample2_SetPreroll(This,bIsPreroll)	\
    (This)->lpVtbl -> SetPreroll(This,bIsPreroll)

#define IMediaSample2_GetActualDataLength(This)	\
    (This)->lpVtbl -> GetActualDataLength(This)

#define IMediaSample2_SetActualDataLength(This,__MIDL_0004)	\
    (This)->lpVtbl -> SetActualDataLength(This,__MIDL_0004)

#define IMediaSample2_GetMediaType(This,ppMediaType)	\
    (This)->lpVtbl -> GetMediaType(This,ppMediaType)

#define IMediaSample2_SetMediaType(This,pMediaType)	\
    (This)->lpVtbl -> SetMediaType(This,pMediaType)

#define IMediaSample2_IsDiscontinuity(This)	\
    (This)->lpVtbl -> IsDiscontinuity(This)

#define IMediaSample2_SetDiscontinuity(This,bDiscontinuity)	\
    (This)->lpVtbl -> SetDiscontinuity(This,bDiscontinuity)

#define IMediaSample2_GetMediaTime(This,pTimeStart,pTimeEnd)	\
    (This)->lpVtbl -> GetMediaTime(This,pTimeStart,pTimeEnd)

#define IMediaSample2_SetMediaTime(This,pTimeStart,pTimeEnd)	\
    (This)->lpVtbl -> SetMediaTime(This,pTimeStart,pTimeEnd)


#define IMediaSample2_GetProperties(This,cbProperties,pbProperties)	\
    (This)->lpVtbl -> GetProperties(This,cbProperties,pbProperties)

#define IMediaSample2_SetProperties(This,cbProperties,pbProperties)	\
    (This)->lpVtbl -> SetProperties(This,cbProperties,pbProperties)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IMediaSample2_GetProperties_Proxy( 
    IMediaSample2 __RPC_FAR * This,
    /* [in] */ DWORD cbProperties,
    /* [size_is][out] */ BYTE __RPC_FAR *pbProperties);


void __RPC_STUB IMediaSample2_GetProperties_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMediaSample2_SetProperties_Proxy( 
    IMediaSample2 __RPC_FAR * This,
    /* [in] */ DWORD cbProperties,
    /* [size_is][in] */ const BYTE __RPC_FAR *pbProperties);


void __RPC_STUB IMediaSample2_SetProperties_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IMediaSample2_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_strmif_0067 */
/* [local] */ 

typedef IMediaSample2 __RPC_FAR *PMEDIASAMPLE2;

#define AM_GBF_PREVFRAMESKIPPED 1
#define AM_GBF_NOTASYNCPOINT 2
#define AM_GBF_NOWAIT 4


extern RPC_IF_HANDLE __MIDL_itf_strmif_0067_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_strmif_0067_v0_0_s_ifspec;

#ifndef __IMemAllocator_INTERFACE_DEFINED__
#define __IMemAllocator_INTERFACE_DEFINED__

/* interface IMemAllocator */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IMemAllocator;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("56a8689c-0ad4-11ce-b03a-0020af0ba770")
    IMemAllocator : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SetProperties( 
            /* [in] */ ALLOCATOR_PROPERTIES __RPC_FAR *pRequest,
            /* [out] */ ALLOCATOR_PROPERTIES __RPC_FAR *pActual) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetProperties( 
            /* [out] */ ALLOCATOR_PROPERTIES __RPC_FAR *pProps) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Commit( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Decommit( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetBuffer( 
            /* [out] */ IMediaSample __RPC_FAR *__RPC_FAR *ppBuffer,
            /* [in] */ REFERENCE_TIME __RPC_FAR *pStartTime,
            /* [in] */ REFERENCE_TIME __RPC_FAR *pEndTime,
            /* [in] */ DWORD dwFlags) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ReleaseBuffer( 
            /* [in] */ IMediaSample __RPC_FAR *pBuffer) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMemAllocatorVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IMemAllocator __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IMemAllocator __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IMemAllocator __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetProperties )( 
            IMemAllocator __RPC_FAR * This,
            /* [in] */ ALLOCATOR_PROPERTIES __RPC_FAR *pRequest,
            /* [out] */ ALLOCATOR_PROPERTIES __RPC_FAR *pActual);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetProperties )( 
            IMemAllocator __RPC_FAR * This,
            /* [out] */ ALLOCATOR_PROPERTIES __RPC_FAR *pProps);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Commit )( 
            IMemAllocator __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Decommit )( 
            IMemAllocator __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetBuffer )( 
            IMemAllocator __RPC_FAR * This,
            /* [out] */ IMediaSample __RPC_FAR *__RPC_FAR *ppBuffer,
            /* [in] */ REFERENCE_TIME __RPC_FAR *pStartTime,
            /* [in] */ REFERENCE_TIME __RPC_FAR *pEndTime,
            /* [in] */ DWORD dwFlags);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ReleaseBuffer )( 
            IMemAllocator __RPC_FAR * This,
            /* [in] */ IMediaSample __RPC_FAR *pBuffer);
        
        END_INTERFACE
    } IMemAllocatorVtbl;

    interface IMemAllocator
    {
        CONST_VTBL struct IMemAllocatorVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMemAllocator_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IMemAllocator_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IMemAllocator_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IMemAllocator_SetProperties(This,pRequest,pActual)	\
    (This)->lpVtbl -> SetProperties(This,pRequest,pActual)

#define IMemAllocator_GetProperties(This,pProps)	\
    (This)->lpVtbl -> GetProperties(This,pProps)

#define IMemAllocator_Commit(This)	\
    (This)->lpVtbl -> Commit(This)

#define IMemAllocator_Decommit(This)	\
    (This)->lpVtbl -> Decommit(This)

#define IMemAllocator_GetBuffer(This,ppBuffer,pStartTime,pEndTime,dwFlags)	\
    (This)->lpVtbl -> GetBuffer(This,ppBuffer,pStartTime,pEndTime,dwFlags)

#define IMemAllocator_ReleaseBuffer(This,pBuffer)	\
    (This)->lpVtbl -> ReleaseBuffer(This,pBuffer)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IMemAllocator_SetProperties_Proxy( 
    IMemAllocator __RPC_FAR * This,
    /* [in] */ ALLOCATOR_PROPERTIES __RPC_FAR *pRequest,
    /* [out] */ ALLOCATOR_PROPERTIES __RPC_FAR *pActual);


void __RPC_STUB IMemAllocator_SetProperties_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMemAllocator_GetProperties_Proxy( 
    IMemAllocator __RPC_FAR * This,
    /* [out] */ ALLOCATOR_PROPERTIES __RPC_FAR *pProps);


void __RPC_STUB IMemAllocator_GetProperties_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMemAllocator_Commit_Proxy( 
    IMemAllocator __RPC_FAR * This);


void __RPC_STUB IMemAllocator_Commit_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMemAllocator_Decommit_Proxy( 
    IMemAllocator __RPC_FAR * This);


void __RPC_STUB IMemAllocator_Decommit_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMemAllocator_GetBuffer_Proxy( 
    IMemAllocator __RPC_FAR * This,
    /* [out] */ IMediaSample __RPC_FAR *__RPC_FAR *ppBuffer,
    /* [in] */ REFERENCE_TIME __RPC_FAR *pStartTime,
    /* [in] */ REFERENCE_TIME __RPC_FAR *pEndTime,
    /* [in] */ DWORD dwFlags);


void __RPC_STUB IMemAllocator_GetBuffer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMemAllocator_ReleaseBuffer_Proxy( 
    IMemAllocator __RPC_FAR * This,
    /* [in] */ IMediaSample __RPC_FAR *pBuffer);


void __RPC_STUB IMemAllocator_ReleaseBuffer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IMemAllocator_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_strmif_0068 */
/* [local] */ 

typedef IMemAllocator __RPC_FAR *PMEMALLOCATOR;



extern RPC_IF_HANDLE __MIDL_itf_strmif_0068_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_strmif_0068_v0_0_s_ifspec;

#ifndef __IMemAllocator2_INTERFACE_DEFINED__
#define __IMemAllocator2_INTERFACE_DEFINED__

/* interface IMemAllocator2 */
/* [unique][uuid][object][local] */ 


EXTERN_C const IID IID_IMemAllocator2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("fa04cdc8-cb49-11d1-a4ef-00c04fb6fa12")
    IMemAllocator2 : public IMemAllocator
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetBusyCount( 
            DWORD __RPC_FAR *pdwBusyCount) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetFreeCount( 
            DWORD __RPC_FAR *pdwFreeCount) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMemAllocator2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IMemAllocator2 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IMemAllocator2 __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IMemAllocator2 __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetProperties )( 
            IMemAllocator2 __RPC_FAR * This,
            /* [in] */ ALLOCATOR_PROPERTIES __RPC_FAR *pRequest,
            /* [out] */ ALLOCATOR_PROPERTIES __RPC_FAR *pActual);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetProperties )( 
            IMemAllocator2 __RPC_FAR * This,
            /* [out] */ ALLOCATOR_PROPERTIES __RPC_FAR *pProps);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Commit )( 
            IMemAllocator2 __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Decommit )( 
            IMemAllocator2 __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetBuffer )( 
            IMemAllocator2 __RPC_FAR * This,
            /* [out] */ IMediaSample __RPC_FAR *__RPC_FAR *ppBuffer,
            /* [in] */ REFERENCE_TIME __RPC_FAR *pStartTime,
            /* [in] */ REFERENCE_TIME __RPC_FAR *pEndTime,
            /* [in] */ DWORD dwFlags);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ReleaseBuffer )( 
            IMemAllocator2 __RPC_FAR * This,
            /* [in] */ IMediaSample __RPC_FAR *pBuffer);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetBusyCount )( 
            IMemAllocator2 __RPC_FAR * This,
            DWORD __RPC_FAR *pdwBusyCount);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetFreeCount )( 
            IMemAllocator2 __RPC_FAR * This,
            DWORD __RPC_FAR *pdwFreeCount);
        
        END_INTERFACE
    } IMemAllocator2Vtbl;

    interface IMemAllocator2
    {
        CONST_VTBL struct IMemAllocator2Vtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMemAllocator2_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IMemAllocator2_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IMemAllocator2_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IMemAllocator2_SetProperties(This,pRequest,pActual)	\
    (This)->lpVtbl -> SetProperties(This,pRequest,pActual)

#define IMemAllocator2_GetProperties(This,pProps)	\
    (This)->lpVtbl -> GetProperties(This,pProps)

#define IMemAllocator2_Commit(This)	\
    (This)->lpVtbl -> Commit(This)

#define IMemAllocator2_Decommit(This)	\
    (This)->lpVtbl -> Decommit(This)

#define IMemAllocator2_GetBuffer(This,ppBuffer,pStartTime,pEndTime,dwFlags)	\
    (This)->lpVtbl -> GetBuffer(This,ppBuffer,pStartTime,pEndTime,dwFlags)

#define IMemAllocator2_ReleaseBuffer(This,pBuffer)	\
    (This)->lpVtbl -> ReleaseBuffer(This,pBuffer)


#define IMemAllocator2_GetBusyCount(This,pdwBusyCount)	\
    (This)->lpVtbl -> GetBusyCount(This,pdwBusyCount)

#define IMemAllocator2_GetFreeCount(This,pdwFreeCount)	\
    (This)->lpVtbl -> GetFreeCount(This,pdwFreeCount)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IMemAllocator2_GetBusyCount_Proxy( 
    IMemAllocator2 __RPC_FAR * This,
    DWORD __RPC_FAR *pdwBusyCount);


void __RPC_STUB IMemAllocator2_GetBusyCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMemAllocator2_GetFreeCount_Proxy( 
    IMemAllocator2 __RPC_FAR * This,
    DWORD __RPC_FAR *pdwFreeCount);


void __RPC_STUB IMemAllocator2_GetFreeCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IMemAllocator2_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_strmif_0069 */
/* [local] */ 

typedef IMemAllocator2 __RPC_FAR *PMEMALLOCATOR2;



extern RPC_IF_HANDLE __MIDL_itf_strmif_0069_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_strmif_0069_v0_0_s_ifspec;

#ifndef __IMemInputPin_INTERFACE_DEFINED__
#define __IMemInputPin_INTERFACE_DEFINED__

/* interface IMemInputPin */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IMemInputPin;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("56a8689d-0ad4-11ce-b03a-0020af0ba770")
    IMemInputPin : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetAllocator( 
            /* [out] */ IMemAllocator __RPC_FAR *__RPC_FAR *ppAllocator) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE NotifyAllocator( 
            /* [in] */ IMemAllocator __RPC_FAR *pAllocator,
            /* [in] */ BOOL bReadOnly) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetAllocatorRequirements( 
            /* [out] */ ALLOCATOR_PROPERTIES __RPC_FAR *pProps) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Receive( 
            /* [in] */ IMediaSample __RPC_FAR *pSample) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ReceiveMultiple( 
            /* [size_is][in] */ IMediaSample __RPC_FAR *__RPC_FAR *pSamples,
            /* [in] */ long nSamples,
            /* [out] */ long __RPC_FAR *nSamplesProcessed) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ReceiveCanBlock( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMemInputPinVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IMemInputPin __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IMemInputPin __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IMemInputPin __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetAllocator )( 
            IMemInputPin __RPC_FAR * This,
            /* [out] */ IMemAllocator __RPC_FAR *__RPC_FAR *ppAllocator);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *NotifyAllocator )( 
            IMemInputPin __RPC_FAR * This,
            /* [in] */ IMemAllocator __RPC_FAR *pAllocator,
            /* [in] */ BOOL bReadOnly);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetAllocatorRequirements )( 
            IMemInputPin __RPC_FAR * This,
            /* [out] */ ALLOCATOR_PROPERTIES __RPC_FAR *pProps);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Receive )( 
            IMemInputPin __RPC_FAR * This,
            /* [in] */ IMediaSample __RPC_FAR *pSample);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ReceiveMultiple )( 
            IMemInputPin __RPC_FAR * This,
            /* [size_is][in] */ IMediaSample __RPC_FAR *__RPC_FAR *pSamples,
            /* [in] */ long nSamples,
            /* [out] */ long __RPC_FAR *nSamplesProcessed);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ReceiveCanBlock )( 
            IMemInputPin __RPC_FAR * This);
        
        END_INTERFACE
    } IMemInputPinVtbl;

    interface IMemInputPin
    {
        CONST_VTBL struct IMemInputPinVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMemInputPin_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IMemInputPin_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IMemInputPin_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IMemInputPin_GetAllocator(This,ppAllocator)	\
    (This)->lpVtbl -> GetAllocator(This,ppAllocator)

#define IMemInputPin_NotifyAllocator(This,pAllocator,bReadOnly)	\
    (This)->lpVtbl -> NotifyAllocator(This,pAllocator,bReadOnly)

#define IMemInputPin_GetAllocatorRequirements(This,pProps)	\
    (This)->lpVtbl -> GetAllocatorRequirements(This,pProps)

#define IMemInputPin_Receive(This,pSample)	\
    (This)->lpVtbl -> Receive(This,pSample)

#define IMemInputPin_ReceiveMultiple(This,pSamples,nSamples,nSamplesProcessed)	\
    (This)->lpVtbl -> ReceiveMultiple(This,pSamples,nSamples,nSamplesProcessed)

#define IMemInputPin_ReceiveCanBlock(This)	\
    (This)->lpVtbl -> ReceiveCanBlock(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IMemInputPin_GetAllocator_Proxy( 
    IMemInputPin __RPC_FAR * This,
    /* [out] */ IMemAllocator __RPC_FAR *__RPC_FAR *ppAllocator);


void __RPC_STUB IMemInputPin_GetAllocator_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMemInputPin_NotifyAllocator_Proxy( 
    IMemInputPin __RPC_FAR * This,
    /* [in] */ IMemAllocator __RPC_FAR *pAllocator,
    /* [in] */ BOOL bReadOnly);


void __RPC_STUB IMemInputPin_NotifyAllocator_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMemInputPin_GetAllocatorRequirements_Proxy( 
    IMemInputPin __RPC_FAR * This,
    /* [out] */ ALLOCATOR_PROPERTIES __RPC_FAR *pProps);


void __RPC_STUB IMemInputPin_GetAllocatorRequirements_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMemInputPin_Receive_Proxy( 
    IMemInputPin __RPC_FAR * This,
    /* [in] */ IMediaSample __RPC_FAR *pSample);


void __RPC_STUB IMemInputPin_Receive_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMemInputPin_ReceiveMultiple_Proxy( 
    IMemInputPin __RPC_FAR * This,
    /* [size_is][in] */ IMediaSample __RPC_FAR *__RPC_FAR *pSamples,
    /* [in] */ long nSamples,
    /* [out] */ long __RPC_FAR *nSamplesProcessed);


void __RPC_STUB IMemInputPin_ReceiveMultiple_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMemInputPin_ReceiveCanBlock_Proxy( 
    IMemInputPin __RPC_FAR * This);


void __RPC_STUB IMemInputPin_ReceiveCanBlock_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IMemInputPin_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_strmif_0070 */
/* [local] */ 

typedef IMemInputPin __RPC_FAR *PMEMINPUTPIN;



extern RPC_IF_HANDLE __MIDL_itf_strmif_0070_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_strmif_0070_v0_0_s_ifspec;

#ifndef __IAMovieSetup_INTERFACE_DEFINED__
#define __IAMovieSetup_INTERFACE_DEFINED__

/* interface IAMovieSetup */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IAMovieSetup;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("a3d8cec0-7e5a-11cf-bbc5-00805f6cef20")
    IAMovieSetup : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Register( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Unregister( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAMovieSetupVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IAMovieSetup __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IAMovieSetup __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IAMovieSetup __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Register )( 
            IAMovieSetup __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Unregister )( 
            IAMovieSetup __RPC_FAR * This);
        
        END_INTERFACE
    } IAMovieSetupVtbl;

    interface IAMovieSetup
    {
        CONST_VTBL struct IAMovieSetupVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAMovieSetup_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAMovieSetup_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAMovieSetup_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IAMovieSetup_Register(This)	\
    (This)->lpVtbl -> Register(This)

#define IAMovieSetup_Unregister(This)	\
    (This)->lpVtbl -> Unregister(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IAMovieSetup_Register_Proxy( 
    IAMovieSetup __RPC_FAR * This);


void __RPC_STUB IAMovieSetup_Register_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMovieSetup_Unregister_Proxy( 
    IAMovieSetup __RPC_FAR * This);


void __RPC_STUB IAMovieSetup_Unregister_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAMovieSetup_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_strmif_0071 */
/* [local] */ 

typedef IAMovieSetup __RPC_FAR *PAMOVIESETUP;

typedef 
enum AM_SEEKING_SeekingFlags
    {	AM_SEEKING_NoPositioning	= 0,
	AM_SEEKING_AbsolutePositioning	= 0x1,
	AM_SEEKING_RelativePositioning	= 0x2,
	AM_SEEKING_IncrementalPositioning	= 0x3,
	AM_SEEKING_PositioningBitsMask	= 0x3,
	AM_SEEKING_SeekToKeyFrame	= 0x4,
	AM_SEEKING_ReturnTime	= 0x8,
	AM_SEEKING_Segment	= 0x10,
	AM_SEEKING_NoFlush	= 0x20
    }	AM_SEEKING_SEEKING_FLAGS;

typedef 
enum AM_SEEKING_SeekingCapabilities
    {	AM_SEEKING_CanSeekAbsolute	= 0x1,
	AM_SEEKING_CanSeekForwards	= 0x2,
	AM_SEEKING_CanSeekBackwards	= 0x4,
	AM_SEEKING_CanGetCurrentPos	= 0x8,
	AM_SEEKING_CanGetStopPos	= 0x10,
	AM_SEEKING_CanGetDuration	= 0x20,
	AM_SEEKING_CanPlayBackwards	= 0x40,
	AM_SEEKING_CanDoSegments	= 0x80,
	AM_SEEKING_Source	= 0x100
    }	AM_SEEKING_SEEKING_CAPABILITIES;



extern RPC_IF_HANDLE __MIDL_itf_strmif_0071_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_strmif_0071_v0_0_s_ifspec;

#ifndef __IMediaSeeking_INTERFACE_DEFINED__
#define __IMediaSeeking_INTERFACE_DEFINED__

/* interface IMediaSeeking */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IMediaSeeking;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("36b73880-c2c8-11cf-8b46-00805f6cef60")
    IMediaSeeking : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetCapabilities( 
            /* [out] */ DWORD __RPC_FAR *pCapabilities) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CheckCapabilities( 
            /* [out][in] */ DWORD __RPC_FAR *pCapabilities) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE IsFormatSupported( 
            /* [in] */ const GUID __RPC_FAR *pFormat) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE QueryPreferredFormat( 
            /* [out] */ GUID __RPC_FAR *pFormat) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetTimeFormat( 
            /* [out] */ GUID __RPC_FAR *pFormat) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE IsUsingTimeFormat( 
            /* [in] */ const GUID __RPC_FAR *pFormat) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetTimeFormat( 
            /* [in] */ const GUID __RPC_FAR *pFormat) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetDuration( 
            /* [out] */ LONGLONG __RPC_FAR *pDuration) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetStopPosition( 
            /* [out] */ LONGLONG __RPC_FAR *pStop) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetCurrentPosition( 
            /* [out] */ LONGLONG __RPC_FAR *pCurrent) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ConvertTimeFormat( 
            /* [out] */ LONGLONG __RPC_FAR *pTarget,
            /* [in] */ const GUID __RPC_FAR *pTargetFormat,
            /* [in] */ LONGLONG Source,
            /* [in] */ const GUID __RPC_FAR *pSourceFormat) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetPositions( 
            /* [out][in] */ LONGLONG __RPC_FAR *pCurrent,
            /* [in] */ DWORD dwCurrentFlags,
            /* [out][in] */ LONGLONG __RPC_FAR *pStop,
            /* [in] */ DWORD dwStopFlags) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetPositions( 
            /* [out] */ LONGLONG __RPC_FAR *pCurrent,
            /* [out] */ LONGLONG __RPC_FAR *pStop) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetAvailable( 
            /* [out] */ LONGLONG __RPC_FAR *pEarliest,
            /* [out] */ LONGLONG __RPC_FAR *pLatest) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetRate( 
            /* [in] */ double dRate) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetRate( 
            /* [out] */ double __RPC_FAR *pdRate) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetPreroll( 
            /* [out] */ LONGLONG __RPC_FAR *pllPreroll) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMediaSeekingVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IMediaSeeking __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IMediaSeeking __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IMediaSeeking __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetCapabilities )( 
            IMediaSeeking __RPC_FAR * This,
            /* [out] */ DWORD __RPC_FAR *pCapabilities);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CheckCapabilities )( 
            IMediaSeeking __RPC_FAR * This,
            /* [out][in] */ DWORD __RPC_FAR *pCapabilities);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsFormatSupported )( 
            IMediaSeeking __RPC_FAR * This,
            /* [in] */ const GUID __RPC_FAR *pFormat);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryPreferredFormat )( 
            IMediaSeeking __RPC_FAR * This,
            /* [out] */ GUID __RPC_FAR *pFormat);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTimeFormat )( 
            IMediaSeeking __RPC_FAR * This,
            /* [out] */ GUID __RPC_FAR *pFormat);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsUsingTimeFormat )( 
            IMediaSeeking __RPC_FAR * This,
            /* [in] */ const GUID __RPC_FAR *pFormat);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetTimeFormat )( 
            IMediaSeeking __RPC_FAR * This,
            /* [in] */ const GUID __RPC_FAR *pFormat);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDuration )( 
            IMediaSeeking __RPC_FAR * This,
            /* [out] */ LONGLONG __RPC_FAR *pDuration);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetStopPosition )( 
            IMediaSeeking __RPC_FAR * This,
            /* [out] */ LONGLONG __RPC_FAR *pStop);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetCurrentPosition )( 
            IMediaSeeking __RPC_FAR * This,
            /* [out] */ LONGLONG __RPC_FAR *pCurrent);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ConvertTimeFormat )( 
            IMediaSeeking __RPC_FAR * This,
            /* [out] */ LONGLONG __RPC_FAR *pTarget,
            /* [in] */ const GUID __RPC_FAR *pTargetFormat,
            /* [in] */ LONGLONG Source,
            /* [in] */ const GUID __RPC_FAR *pSourceFormat);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetPositions )( 
            IMediaSeeking __RPC_FAR * This,
            /* [out][in] */ LONGLONG __RPC_FAR *pCurrent,
            /* [in] */ DWORD dwCurrentFlags,
            /* [out][in] */ LONGLONG __RPC_FAR *pStop,
            /* [in] */ DWORD dwStopFlags);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetPositions )( 
            IMediaSeeking __RPC_FAR * This,
            /* [out] */ LONGLONG __RPC_FAR *pCurrent,
            /* [out] */ LONGLONG __RPC_FAR *pStop);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetAvailable )( 
            IMediaSeeking __RPC_FAR * This,
            /* [out] */ LONGLONG __RPC_FAR *pEarliest,
            /* [out] */ LONGLONG __RPC_FAR *pLatest);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetRate )( 
            IMediaSeeking __RPC_FAR * This,
            /* [in] */ double dRate);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetRate )( 
            IMediaSeeking __RPC_FAR * This,
            /* [out] */ double __RPC_FAR *pdRate);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetPreroll )( 
            IMediaSeeking __RPC_FAR * This,
            /* [out] */ LONGLONG __RPC_FAR *pllPreroll);
        
        END_INTERFACE
    } IMediaSeekingVtbl;

    interface IMediaSeeking
    {
        CONST_VTBL struct IMediaSeekingVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMediaSeeking_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IMediaSeeking_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IMediaSeeking_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IMediaSeeking_GetCapabilities(This,pCapabilities)	\
    (This)->lpVtbl -> GetCapabilities(This,pCapabilities)

#define IMediaSeeking_CheckCapabilities(This,pCapabilities)	\
    (This)->lpVtbl -> CheckCapabilities(This,pCapabilities)

#define IMediaSeeking_IsFormatSupported(This,pFormat)	\
    (This)->lpVtbl -> IsFormatSupported(This,pFormat)

#define IMediaSeeking_QueryPreferredFormat(This,pFormat)	\
    (This)->lpVtbl -> QueryPreferredFormat(This,pFormat)

#define IMediaSeeking_GetTimeFormat(This,pFormat)	\
    (This)->lpVtbl -> GetTimeFormat(This,pFormat)

#define IMediaSeeking_IsUsingTimeFormat(This,pFormat)	\
    (This)->lpVtbl -> IsUsingTimeFormat(This,pFormat)

#define IMediaSeeking_SetTimeFormat(This,pFormat)	\
    (This)->lpVtbl -> SetTimeFormat(This,pFormat)

#define IMediaSeeking_GetDuration(This,pDuration)	\
    (This)->lpVtbl -> GetDuration(This,pDuration)

#define IMediaSeeking_GetStopPosition(This,pStop)	\
    (This)->lpVtbl -> GetStopPosition(This,pStop)

#define IMediaSeeking_GetCurrentPosition(This,pCurrent)	\
    (This)->lpVtbl -> GetCurrentPosition(This,pCurrent)

#define IMediaSeeking_ConvertTimeFormat(This,pTarget,pTargetFormat,Source,pSourceFormat)	\
    (This)->lpVtbl -> ConvertTimeFormat(This,pTarget,pTargetFormat,Source,pSourceFormat)

#define IMediaSeeking_SetPositions(This,pCurrent,dwCurrentFlags,pStop,dwStopFlags)	\
    (This)->lpVtbl -> SetPositions(This,pCurrent,dwCurrentFlags,pStop,dwStopFlags)

#define IMediaSeeking_GetPositions(This,pCurrent,pStop)	\
    (This)->lpVtbl -> GetPositions(This,pCurrent,pStop)

#define IMediaSeeking_GetAvailable(This,pEarliest,pLatest)	\
    (This)->lpVtbl -> GetAvailable(This,pEarliest,pLatest)

#define IMediaSeeking_SetRate(This,dRate)	\
    (This)->lpVtbl -> SetRate(This,dRate)

#define IMediaSeeking_GetRate(This,pdRate)	\
    (This)->lpVtbl -> GetRate(This,pdRate)

#define IMediaSeeking_GetPreroll(This,pllPreroll)	\
    (This)->lpVtbl -> GetPreroll(This,pllPreroll)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IMediaSeeking_GetCapabilities_Proxy( 
    IMediaSeeking __RPC_FAR * This,
    /* [out] */ DWORD __RPC_FAR *pCapabilities);


void __RPC_STUB IMediaSeeking_GetCapabilities_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMediaSeeking_CheckCapabilities_Proxy( 
    IMediaSeeking __RPC_FAR * This,
    /* [out][in] */ DWORD __RPC_FAR *pCapabilities);


void __RPC_STUB IMediaSeeking_CheckCapabilities_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMediaSeeking_IsFormatSupported_Proxy( 
    IMediaSeeking __RPC_FAR * This,
    /* [in] */ const GUID __RPC_FAR *pFormat);


void __RPC_STUB IMediaSeeking_IsFormatSupported_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMediaSeeking_QueryPreferredFormat_Proxy( 
    IMediaSeeking __RPC_FAR * This,
    /* [out] */ GUID __RPC_FAR *pFormat);


void __RPC_STUB IMediaSeeking_QueryPreferredFormat_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMediaSeeking_GetTimeFormat_Proxy( 
    IMediaSeeking __RPC_FAR * This,
    /* [out] */ GUID __RPC_FAR *pFormat);


void __RPC_STUB IMediaSeeking_GetTimeFormat_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMediaSeeking_IsUsingTimeFormat_Proxy( 
    IMediaSeeking __RPC_FAR * This,
    /* [in] */ const GUID __RPC_FAR *pFormat);


void __RPC_STUB IMediaSeeking_IsUsingTimeFormat_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMediaSeeking_SetTimeFormat_Proxy( 
    IMediaSeeking __RPC_FAR * This,
    /* [in] */ const GUID __RPC_FAR *pFormat);


void __RPC_STUB IMediaSeeking_SetTimeFormat_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMediaSeeking_GetDuration_Proxy( 
    IMediaSeeking __RPC_FAR * This,
    /* [out] */ LONGLONG __RPC_FAR *pDuration);


void __RPC_STUB IMediaSeeking_GetDuration_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMediaSeeking_GetStopPosition_Proxy( 
    IMediaSeeking __RPC_FAR * This,
    /* [out] */ LONGLONG __RPC_FAR *pStop);


void __RPC_STUB IMediaSeeking_GetStopPosition_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMediaSeeking_GetCurrentPosition_Proxy( 
    IMediaSeeking __RPC_FAR * This,
    /* [out] */ LONGLONG __RPC_FAR *pCurrent);


void __RPC_STUB IMediaSeeking_GetCurrentPosition_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMediaSeeking_ConvertTimeFormat_Proxy( 
    IMediaSeeking __RPC_FAR * This,
    /* [out] */ LONGLONG __RPC_FAR *pTarget,
    /* [in] */ const GUID __RPC_FAR *pTargetFormat,
    /* [in] */ LONGLONG Source,
    /* [in] */ const GUID __RPC_FAR *pSourceFormat);


void __RPC_STUB IMediaSeeking_ConvertTimeFormat_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMediaSeeking_SetPositions_Proxy( 
    IMediaSeeking __RPC_FAR * This,
    /* [out][in] */ LONGLONG __RPC_FAR *pCurrent,
    /* [in] */ DWORD dwCurrentFlags,
    /* [out][in] */ LONGLONG __RPC_FAR *pStop,
    /* [in] */ DWORD dwStopFlags);


void __RPC_STUB IMediaSeeking_SetPositions_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMediaSeeking_GetPositions_Proxy( 
    IMediaSeeking __RPC_FAR * This,
    /* [out] */ LONGLONG __RPC_FAR *pCurrent,
    /* [out] */ LONGLONG __RPC_FAR *pStop);


void __RPC_STUB IMediaSeeking_GetPositions_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMediaSeeking_GetAvailable_Proxy( 
    IMediaSeeking __RPC_FAR * This,
    /* [out] */ LONGLONG __RPC_FAR *pEarliest,
    /* [out] */ LONGLONG __RPC_FAR *pLatest);


void __RPC_STUB IMediaSeeking_GetAvailable_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMediaSeeking_SetRate_Proxy( 
    IMediaSeeking __RPC_FAR * This,
    /* [in] */ double dRate);


void __RPC_STUB IMediaSeeking_SetRate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMediaSeeking_GetRate_Proxy( 
    IMediaSeeking __RPC_FAR * This,
    /* [out] */ double __RPC_FAR *pdRate);


void __RPC_STUB IMediaSeeking_GetRate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMediaSeeking_GetPreroll_Proxy( 
    IMediaSeeking __RPC_FAR * This,
    /* [out] */ LONGLONG __RPC_FAR *pllPreroll);


void __RPC_STUB IMediaSeeking_GetPreroll_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IMediaSeeking_INTERFACE_DEFINED__ */


#ifndef __IAudioRenderer_INTERFACE_DEFINED__
#define __IAudioRenderer_INTERFACE_DEFINED__

/* interface IAudioRenderer */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IAudioRenderer;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("86612576-6b0a-4033-8374-edbdc59ac25c")
    IAudioRenderer : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SetDriftRate( 
            /* [in] */ DOUBLE dwRate) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAudioRendererVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IAudioRenderer __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IAudioRenderer __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IAudioRenderer __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetDriftRate )( 
            IAudioRenderer __RPC_FAR * This,
            /* [in] */ DOUBLE dwRate);
        
        END_INTERFACE
    } IAudioRendererVtbl;

    interface IAudioRenderer
    {
        CONST_VTBL struct IAudioRendererVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAudioRenderer_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAudioRenderer_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAudioRenderer_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IAudioRenderer_SetDriftRate(This,dwRate)	\
    (This)->lpVtbl -> SetDriftRate(This,dwRate)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IAudioRenderer_SetDriftRate_Proxy( 
    IAudioRenderer __RPC_FAR * This,
    /* [in] */ DOUBLE dwRate);


void __RPC_STUB IAudioRenderer_SetDriftRate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAudioRenderer_INTERFACE_DEFINED__ */


#ifndef __IAudioRendererWaveOut_INTERFACE_DEFINED__
#define __IAudioRendererWaveOut_INTERFACE_DEFINED__

/* interface IAudioRendererWaveOut */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IAudioRendererWaveOut;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("8244a79e-9986-4187-abcb-3a69542f7cc7")
    IAudioRendererWaveOut : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SendMessage( 
            /* [in] */ UINT uMsg,
            /* [in] */ DWORD dw1,
            /* [in] */ DWORD dw2) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAudioRendererWaveOutVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IAudioRendererWaveOut __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IAudioRendererWaveOut __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IAudioRendererWaveOut __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SendMessage )( 
            IAudioRendererWaveOut __RPC_FAR * This,
            /* [in] */ UINT uMsg,
            /* [in] */ DWORD dw1,
            /* [in] */ DWORD dw2);
        
        END_INTERFACE
    } IAudioRendererWaveOutVtbl;

    interface IAudioRendererWaveOut
    {
        CONST_VTBL struct IAudioRendererWaveOutVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAudioRendererWaveOut_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAudioRendererWaveOut_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAudioRendererWaveOut_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IAudioRendererWaveOut_SendMessage(This,uMsg,dw1,dw2)	\
    (This)->lpVtbl -> SendMessage(This,uMsg,dw1,dw2)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IAudioRendererWaveOut_SendMessage_Proxy( 
    IAudioRendererWaveOut __RPC_FAR * This,
    /* [in] */ UINT uMsg,
    /* [in] */ DWORD dw1,
    /* [in] */ DWORD dw2);


void __RPC_STUB IAudioRendererWaveOut_SendMessage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAudioRendererWaveOut_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_strmif_0074 */
/* [local] */ 

typedef IMediaSeeking __RPC_FAR *PMEDIASEEKING;

enum tagAM_MEDIAEVENT_FLAGS
{
    AM_MEDIAEVENT_NONOTIFY = 0x01
};
































typedef /* [public][public] */ struct __MIDL___MIDL_itf_strmif_0074_0001
    {
    CLSID Clsid;
    LPWSTR Name;
    }	REGFILTER;



extern RPC_IF_HANDLE __MIDL_itf_strmif_0074_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_strmif_0074_v0_0_s_ifspec;

#ifndef __IEnumRegFilters_INTERFACE_DEFINED__
#define __IEnumRegFilters_INTERFACE_DEFINED__

/* interface IEnumRegFilters */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IEnumRegFilters;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("56a868a4-0ad4-11ce-b03a-0020af0ba770")
    IEnumRegFilters : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Next( 
            /* [in] */ ULONG cFilters,
            /* [out] */ REGFILTER __RPC_FAR *__RPC_FAR *apRegFilter,
            /* [out] */ ULONG __RPC_FAR *pcFetched) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Skip( 
            /* [in] */ ULONG cFilters) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Reset( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Clone( 
            /* [out] */ IEnumRegFilters __RPC_FAR *__RPC_FAR *ppEnum) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IEnumRegFiltersVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IEnumRegFilters __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IEnumRegFilters __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IEnumRegFilters __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Next )( 
            IEnumRegFilters __RPC_FAR * This,
            /* [in] */ ULONG cFilters,
            /* [out] */ REGFILTER __RPC_FAR *__RPC_FAR *apRegFilter,
            /* [out] */ ULONG __RPC_FAR *pcFetched);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Skip )( 
            IEnumRegFilters __RPC_FAR * This,
            /* [in] */ ULONG cFilters);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Reset )( 
            IEnumRegFilters __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Clone )( 
            IEnumRegFilters __RPC_FAR * This,
            /* [out] */ IEnumRegFilters __RPC_FAR *__RPC_FAR *ppEnum);
        
        END_INTERFACE
    } IEnumRegFiltersVtbl;

    interface IEnumRegFilters
    {
        CONST_VTBL struct IEnumRegFiltersVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IEnumRegFilters_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IEnumRegFilters_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IEnumRegFilters_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IEnumRegFilters_Next(This,cFilters,apRegFilter,pcFetched)	\
    (This)->lpVtbl -> Next(This,cFilters,apRegFilter,pcFetched)

#define IEnumRegFilters_Skip(This,cFilters)	\
    (This)->lpVtbl -> Skip(This,cFilters)

#define IEnumRegFilters_Reset(This)	\
    (This)->lpVtbl -> Reset(This)

#define IEnumRegFilters_Clone(This,ppEnum)	\
    (This)->lpVtbl -> Clone(This,ppEnum)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IEnumRegFilters_Next_Proxy( 
    IEnumRegFilters __RPC_FAR * This,
    /* [in] */ ULONG cFilters,
    /* [out] */ REGFILTER __RPC_FAR *__RPC_FAR *apRegFilter,
    /* [out] */ ULONG __RPC_FAR *pcFetched);


void __RPC_STUB IEnumRegFilters_Next_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IEnumRegFilters_Skip_Proxy( 
    IEnumRegFilters __RPC_FAR * This,
    /* [in] */ ULONG cFilters);


void __RPC_STUB IEnumRegFilters_Skip_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IEnumRegFilters_Reset_Proxy( 
    IEnumRegFilters __RPC_FAR * This);


void __RPC_STUB IEnumRegFilters_Reset_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IEnumRegFilters_Clone_Proxy( 
    IEnumRegFilters __RPC_FAR * This,
    /* [out] */ IEnumRegFilters __RPC_FAR *__RPC_FAR *ppEnum);


void __RPC_STUB IEnumRegFilters_Clone_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IEnumRegFilters_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_strmif_0076 */
/* [local] */ 

typedef IEnumRegFilters __RPC_FAR *PENUMREGFILTERS;



extern RPC_IF_HANDLE __MIDL_itf_strmif_0076_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_strmif_0076_v0_0_s_ifspec;

#ifndef __IFilterMapper_INTERFACE_DEFINED__
#define __IFilterMapper_INTERFACE_DEFINED__

/* interface IFilterMapper */
/* [unique][uuid][object] */ 


enum __MIDL_IFilterMapper_0001
    {	MERIT_PREFERRED	= 0x800000,
	MERIT_NORMAL	= 0x600000,
	MERIT_UNLIKELY	= 0x400000,
	MERIT_DO_NOT_USE	= 0x200000,
	MERIT_SW_COMPRESSOR	= 0x100000,
	MERIT_HW_COMPRESSOR	= 0x100050
    };

EXTERN_C const IID IID_IFilterMapper;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("56a868a3-0ad4-11ce-b03a-0020af0ba770")
    IFilterMapper : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE RegisterFilter( 
            /* [in] */ CLSID clsid,
            /* [in] */ LPCWSTR Name,
            /* [in] */ DWORD dwMerit) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RegisterFilterInstance( 
            /* [in] */ CLSID clsid,
            /* [in] */ LPCWSTR Name,
            /* [out] */ CLSID __RPC_FAR *MRId) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RegisterPin( 
            /* [in] */ CLSID Filter,
            /* [in] */ LPCWSTR Name,
            /* [in] */ BOOL bRendered,
            /* [in] */ BOOL bOutput,
            /* [in] */ BOOL bZero,
            /* [in] */ BOOL bMany,
            /* [in] */ CLSID ConnectsToFilter,
            /* [in] */ LPCWSTR ConnectsToPin) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RegisterPinType( 
            /* [in] */ CLSID clsFilter,
            /* [in] */ LPCWSTR strName,
            /* [in] */ CLSID clsMajorType,
            /* [in] */ CLSID clsSubType) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE UnregisterFilter( 
            /* [in] */ CLSID Filter) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE UnregisterFilterInstance( 
            /* [in] */ CLSID MRId) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE UnregisterPin( 
            /* [in] */ CLSID Filter,
            /* [in] */ LPCWSTR Name) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE EnumMatchingFilters( 
            /* [out] */ IEnumRegFilters __RPC_FAR *__RPC_FAR *ppEnum,
            /* [in] */ DWORD dwMerit,
            /* [in] */ BOOL bInputNeeded,
            /* [in] */ CLSID clsInMaj,
            /* [in] */ CLSID clsInSub,
            /* [in] */ BOOL bRender,
            /* [in] */ BOOL bOututNeeded,
            /* [in] */ CLSID clsOutMaj,
            /* [in] */ CLSID clsOutSub) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IFilterMapperVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IFilterMapper __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IFilterMapper __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IFilterMapper __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RegisterFilter )( 
            IFilterMapper __RPC_FAR * This,
            /* [in] */ CLSID clsid,
            /* [in] */ LPCWSTR Name,
            /* [in] */ DWORD dwMerit);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RegisterFilterInstance )( 
            IFilterMapper __RPC_FAR * This,
            /* [in] */ CLSID clsid,
            /* [in] */ LPCWSTR Name,
            /* [out] */ CLSID __RPC_FAR *MRId);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RegisterPin )( 
            IFilterMapper __RPC_FAR * This,
            /* [in] */ CLSID Filter,
            /* [in] */ LPCWSTR Name,
            /* [in] */ BOOL bRendered,
            /* [in] */ BOOL bOutput,
            /* [in] */ BOOL bZero,
            /* [in] */ BOOL bMany,
            /* [in] */ CLSID ConnectsToFilter,
            /* [in] */ LPCWSTR ConnectsToPin);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RegisterPinType )( 
            IFilterMapper __RPC_FAR * This,
            /* [in] */ CLSID clsFilter,
            /* [in] */ LPCWSTR strName,
            /* [in] */ CLSID clsMajorType,
            /* [in] */ CLSID clsSubType);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *UnregisterFilter )( 
            IFilterMapper __RPC_FAR * This,
            /* [in] */ CLSID Filter);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *UnregisterFilterInstance )( 
            IFilterMapper __RPC_FAR * This,
            /* [in] */ CLSID MRId);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *UnregisterPin )( 
            IFilterMapper __RPC_FAR * This,
            /* [in] */ CLSID Filter,
            /* [in] */ LPCWSTR Name);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *EnumMatchingFilters )( 
            IFilterMapper __RPC_FAR * This,
            /* [out] */ IEnumRegFilters __RPC_FAR *__RPC_FAR *ppEnum,
            /* [in] */ DWORD dwMerit,
            /* [in] */ BOOL bInputNeeded,
            /* [in] */ CLSID clsInMaj,
            /* [in] */ CLSID clsInSub,
            /* [in] */ BOOL bRender,
            /* [in] */ BOOL bOututNeeded,
            /* [in] */ CLSID clsOutMaj,
            /* [in] */ CLSID clsOutSub);
        
        END_INTERFACE
    } IFilterMapperVtbl;

    interface IFilterMapper
    {
        CONST_VTBL struct IFilterMapperVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IFilterMapper_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IFilterMapper_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IFilterMapper_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IFilterMapper_RegisterFilter(This,clsid,Name,dwMerit)	\
    (This)->lpVtbl -> RegisterFilter(This,clsid,Name,dwMerit)

#define IFilterMapper_RegisterFilterInstance(This,clsid,Name,MRId)	\
    (This)->lpVtbl -> RegisterFilterInstance(This,clsid,Name,MRId)

#define IFilterMapper_RegisterPin(This,Filter,Name,bRendered,bOutput,bZero,bMany,ConnectsToFilter,ConnectsToPin)	\
    (This)->lpVtbl -> RegisterPin(This,Filter,Name,bRendered,bOutput,bZero,bMany,ConnectsToFilter,ConnectsToPin)

#define IFilterMapper_RegisterPinType(This,clsFilter,strName,clsMajorType,clsSubType)	\
    (This)->lpVtbl -> RegisterPinType(This,clsFilter,strName,clsMajorType,clsSubType)

#define IFilterMapper_UnregisterFilter(This,Filter)	\
    (This)->lpVtbl -> UnregisterFilter(This,Filter)

#define IFilterMapper_UnregisterFilterInstance(This,MRId)	\
    (This)->lpVtbl -> UnregisterFilterInstance(This,MRId)

#define IFilterMapper_UnregisterPin(This,Filter,Name)	\
    (This)->lpVtbl -> UnregisterPin(This,Filter,Name)

#define IFilterMapper_EnumMatchingFilters(This,ppEnum,dwMerit,bInputNeeded,clsInMaj,clsInSub,bRender,bOututNeeded,clsOutMaj,clsOutSub)	\
    (This)->lpVtbl -> EnumMatchingFilters(This,ppEnum,dwMerit,bInputNeeded,clsInMaj,clsInSub,bRender,bOututNeeded,clsOutMaj,clsOutSub)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IFilterMapper_RegisterFilter_Proxy( 
    IFilterMapper __RPC_FAR * This,
    /* [in] */ CLSID clsid,
    /* [in] */ LPCWSTR Name,
    /* [in] */ DWORD dwMerit);


void __RPC_STUB IFilterMapper_RegisterFilter_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IFilterMapper_RegisterFilterInstance_Proxy( 
    IFilterMapper __RPC_FAR * This,
    /* [in] */ CLSID clsid,
    /* [in] */ LPCWSTR Name,
    /* [out] */ CLSID __RPC_FAR *MRId);


void __RPC_STUB IFilterMapper_RegisterFilterInstance_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IFilterMapper_RegisterPin_Proxy( 
    IFilterMapper __RPC_FAR * This,
    /* [in] */ CLSID Filter,
    /* [in] */ LPCWSTR Name,
    /* [in] */ BOOL bRendered,
    /* [in] */ BOOL bOutput,
    /* [in] */ BOOL bZero,
    /* [in] */ BOOL bMany,
    /* [in] */ CLSID ConnectsToFilter,
    /* [in] */ LPCWSTR ConnectsToPin);


void __RPC_STUB IFilterMapper_RegisterPin_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IFilterMapper_RegisterPinType_Proxy( 
    IFilterMapper __RPC_FAR * This,
    /* [in] */ CLSID clsFilter,
    /* [in] */ LPCWSTR strName,
    /* [in] */ CLSID clsMajorType,
    /* [in] */ CLSID clsSubType);


void __RPC_STUB IFilterMapper_RegisterPinType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IFilterMapper_UnregisterFilter_Proxy( 
    IFilterMapper __RPC_FAR * This,
    /* [in] */ CLSID Filter);


void __RPC_STUB IFilterMapper_UnregisterFilter_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IFilterMapper_UnregisterFilterInstance_Proxy( 
    IFilterMapper __RPC_FAR * This,
    /* [in] */ CLSID MRId);


void __RPC_STUB IFilterMapper_UnregisterFilterInstance_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IFilterMapper_UnregisterPin_Proxy( 
    IFilterMapper __RPC_FAR * This,
    /* [in] */ CLSID Filter,
    /* [in] */ LPCWSTR Name);


void __RPC_STUB IFilterMapper_UnregisterPin_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IFilterMapper_EnumMatchingFilters_Proxy( 
    IFilterMapper __RPC_FAR * This,
    /* [out] */ IEnumRegFilters __RPC_FAR *__RPC_FAR *ppEnum,
    /* [in] */ DWORD dwMerit,
    /* [in] */ BOOL bInputNeeded,
    /* [in] */ CLSID clsInMaj,
    /* [in] */ CLSID clsInSub,
    /* [in] */ BOOL bRender,
    /* [in] */ BOOL bOututNeeded,
    /* [in] */ CLSID clsOutMaj,
    /* [in] */ CLSID clsOutSub);


void __RPC_STUB IFilterMapper_EnumMatchingFilters_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IFilterMapper_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_strmif_0078 */
/* [local] */ 

typedef /* [public][public] */ struct __MIDL___MIDL_itf_strmif_0078_0001
    {
    const CLSID __RPC_FAR *clsMajorType;
    const CLSID __RPC_FAR *clsMinorType;
    }	REGPINTYPES;

typedef /* [public] */ struct __MIDL___MIDL_itf_strmif_0078_0002
    {
    LPWSTR strName;
    BOOL bRendered;
    BOOL bOutput;
    BOOL bZero;
    BOOL bMany;
    const CLSID __RPC_FAR *clsConnectsToFilter;
    const WCHAR __RPC_FAR *strConnectsToPin;
    UINT nMediaTypes;
    const REGPINTYPES __RPC_FAR *lpMediaType;
    }	REGFILTERPINS;

typedef 
enum tagQualityMessageType
    {	Famine	= 0,
	Flood	= Famine + 1
    }	QualityMessageType;

typedef struct tagQuality
    {
    QualityMessageType Type;
    long Proportion;
    REFERENCE_TIME Late;
    REFERENCE_TIME TimeStamp;
    }	Quality;

typedef IQualityControl __RPC_FAR *PQUALITYCONTROL;



extern RPC_IF_HANDLE __MIDL_itf_strmif_0078_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_strmif_0078_v0_0_s_ifspec;

#ifndef __IQualityControl_INTERFACE_DEFINED__
#define __IQualityControl_INTERFACE_DEFINED__

/* interface IQualityControl */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IQualityControl;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("56a868a5-0ad4-11ce-b03a-0020af0ba770")
    IQualityControl : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Notify( 
            /* [in] */ IBaseFilter __RPC_FAR *pSelf,
            /* [in] */ Quality q) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetSink( 
            /* [in] */ IQualityControl __RPC_FAR *piqc) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IQualityControlVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IQualityControl __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IQualityControl __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IQualityControl __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Notify )( 
            IQualityControl __RPC_FAR * This,
            /* [in] */ IBaseFilter __RPC_FAR *pSelf,
            /* [in] */ Quality q);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetSink )( 
            IQualityControl __RPC_FAR * This,
            /* [in] */ IQualityControl __RPC_FAR *piqc);
        
        END_INTERFACE
    } IQualityControlVtbl;

    interface IQualityControl
    {
        CONST_VTBL struct IQualityControlVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IQualityControl_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IQualityControl_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IQualityControl_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IQualityControl_Notify(This,pSelf,q)	\
    (This)->lpVtbl -> Notify(This,pSelf,q)

#define IQualityControl_SetSink(This,piqc)	\
    (This)->lpVtbl -> SetSink(This,piqc)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IQualityControl_Notify_Proxy( 
    IQualityControl __RPC_FAR * This,
    /* [in] */ IBaseFilter __RPC_FAR *pSelf,
    /* [in] */ Quality q);


void __RPC_STUB IQualityControl_Notify_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IQualityControl_SetSink_Proxy( 
    IQualityControl __RPC_FAR * This,
    /* [in] */ IQualityControl __RPC_FAR *piqc);


void __RPC_STUB IQualityControl_SetSink_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IQualityControl_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_strmif_0079 */
/* [local] */ 


enum __MIDL___MIDL_itf_strmif_0079_0001
    {	CK_NOCOLORKEY	= 0,
	CK_INDEX	= 0x1,
	CK_RGB	= 0x2
    };
typedef struct tagCOLORKEY
    {
    DWORD KeyType;
    DWORD PaletteIndex;
    COLORREF LowColorValue;
    COLORREF HighColorValue;
    }	COLORKEY;


enum __MIDL___MIDL_itf_strmif_0079_0002
    {	ADVISE_NONE	= 0,
	ADVISE_CLIPPING	= 0x1,
	ADVISE_PALETTE	= 0x2,
	ADVISE_COLORKEY	= 0x4,
	ADVISE_POSITION	= 0x8,
	ADVISE_DISPLAY_CHANGE	= 0x10
    };
#define	ADVISE_ALL	( ADVISE_CLIPPING | ADVISE_PALETTE | ADVISE_COLORKEY | ADVISE_POSITION )

#define	ADVISE_ALL2	( ADVISE_ALL | ADVISE_DISPLAY_CHANGE )

#if !defined(_WINGDI_) && !defined(_WINGDI_H_)
typedef struct _RGNDATAHEADER
    {
    DWORD dwSize;
    DWORD iType;
    DWORD nCount;
    DWORD nRgnSize;
    RECT rcBound;
    }	RGNDATAHEADER;

typedef struct _RGNDATA
    {
    RGNDATAHEADER rdh;
    char Buffer[ 1 ];
    }	RGNDATA;

#endif


extern RPC_IF_HANDLE __MIDL_itf_strmif_0079_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_strmif_0079_v0_0_s_ifspec;

#ifndef __IOverlayNotify_INTERFACE_DEFINED__
#define __IOverlayNotify_INTERFACE_DEFINED__

/* interface IOverlayNotify */
/* [unique][uuid][local][object] */ 


EXTERN_C const IID IID_IOverlayNotify;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("56a868a0-0ad4-11ce-b03a-0020af0ba770")
    IOverlayNotify : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE OnPaletteChange( 
            /* [in] */ DWORD dwColors,
            /* [in] */ const PALETTEENTRY __RPC_FAR *pPalette) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnClipChange( 
            /* [in] */ const RECT __RPC_FAR *pSourceRect,
            /* [in] */ const RECT __RPC_FAR *pDestinationRect,
            /* [in] */ const RGNDATA __RPC_FAR *pRgnData) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnColorKeyChange( 
            /* [in] */ const COLORKEY __RPC_FAR *pColorKey) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnPositionChange( 
            /* [in] */ const RECT __RPC_FAR *pSourceRect,
            /* [in] */ const RECT __RPC_FAR *pDestinationRect) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IOverlayNotifyVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IOverlayNotify __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IOverlayNotify __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IOverlayNotify __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnPaletteChange )( 
            IOverlayNotify __RPC_FAR * This,
            /* [in] */ DWORD dwColors,
            /* [in] */ const PALETTEENTRY __RPC_FAR *pPalette);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnClipChange )( 
            IOverlayNotify __RPC_FAR * This,
            /* [in] */ const RECT __RPC_FAR *pSourceRect,
            /* [in] */ const RECT __RPC_FAR *pDestinationRect,
            /* [in] */ const RGNDATA __RPC_FAR *pRgnData);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnColorKeyChange )( 
            IOverlayNotify __RPC_FAR * This,
            /* [in] */ const COLORKEY __RPC_FAR *pColorKey);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnPositionChange )( 
            IOverlayNotify __RPC_FAR * This,
            /* [in] */ const RECT __RPC_FAR *pSourceRect,
            /* [in] */ const RECT __RPC_FAR *pDestinationRect);
        
        END_INTERFACE
    } IOverlayNotifyVtbl;

    interface IOverlayNotify
    {
        CONST_VTBL struct IOverlayNotifyVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IOverlayNotify_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IOverlayNotify_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IOverlayNotify_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IOverlayNotify_OnPaletteChange(This,dwColors,pPalette)	\
    (This)->lpVtbl -> OnPaletteChange(This,dwColors,pPalette)

#define IOverlayNotify_OnClipChange(This,pSourceRect,pDestinationRect,pRgnData)	\
    (This)->lpVtbl -> OnClipChange(This,pSourceRect,pDestinationRect,pRgnData)

#define IOverlayNotify_OnColorKeyChange(This,pColorKey)	\
    (This)->lpVtbl -> OnColorKeyChange(This,pColorKey)

#define IOverlayNotify_OnPositionChange(This,pSourceRect,pDestinationRect)	\
    (This)->lpVtbl -> OnPositionChange(This,pSourceRect,pDestinationRect)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IOverlayNotify_OnPaletteChange_Proxy( 
    IOverlayNotify __RPC_FAR * This,
    /* [in] */ DWORD dwColors,
    /* [in] */ const PALETTEENTRY __RPC_FAR *pPalette);


void __RPC_STUB IOverlayNotify_OnPaletteChange_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOverlayNotify_OnClipChange_Proxy( 
    IOverlayNotify __RPC_FAR * This,
    /* [in] */ const RECT __RPC_FAR *pSourceRect,
    /* [in] */ const RECT __RPC_FAR *pDestinationRect,
    /* [in] */ const RGNDATA __RPC_FAR *pRgnData);


void __RPC_STUB IOverlayNotify_OnClipChange_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOverlayNotify_OnColorKeyChange_Proxy( 
    IOverlayNotify __RPC_FAR * This,
    /* [in] */ const COLORKEY __RPC_FAR *pColorKey);


void __RPC_STUB IOverlayNotify_OnColorKeyChange_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOverlayNotify_OnPositionChange_Proxy( 
    IOverlayNotify __RPC_FAR * This,
    /* [in] */ const RECT __RPC_FAR *pSourceRect,
    /* [in] */ const RECT __RPC_FAR *pDestinationRect);


void __RPC_STUB IOverlayNotify_OnPositionChange_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IOverlayNotify_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_strmif_0080 */
/* [local] */ 

typedef IOverlayNotify __RPC_FAR *POVERLAYNOTIFY;



extern RPC_IF_HANDLE __MIDL_itf_strmif_0080_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_strmif_0080_v0_0_s_ifspec;

#ifndef __IOverlay_INTERFACE_DEFINED__
#define __IOverlay_INTERFACE_DEFINED__

/* interface IOverlay */
/* [unique][uuid][local][object] */ 


EXTERN_C const IID IID_IOverlay;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("56a868a1-0ad4-11ce-b03a-0020af0ba770")
    IOverlay : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetPalette( 
            /* [out] */ DWORD __RPC_FAR *pdwColors,
            /* [out] */ PALETTEENTRY __RPC_FAR *__RPC_FAR *ppPalette) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetPalette( 
            /* [in] */ DWORD dwColors,
            /* [in] */ PALETTEENTRY __RPC_FAR *pPalette) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetDefaultColorKey( 
            /* [out] */ COLORKEY __RPC_FAR *pColorKey) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetColorKey( 
            /* [out] */ COLORKEY __RPC_FAR *pColorKey) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetColorKey( 
            /* [out][in] */ COLORKEY __RPC_FAR *pColorKey) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetWindowHandle( 
            /* [out] */ HWND __RPC_FAR *pHwnd) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetClipList( 
            /* [out] */ RECT __RPC_FAR *pSourceRect,
            /* [out] */ RECT __RPC_FAR *pDestinationRect,
            /* [out] */ RGNDATA __RPC_FAR *__RPC_FAR *ppRgnData) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetVideoPosition( 
            /* [out] */ RECT __RPC_FAR *pSourceRect,
            /* [out] */ RECT __RPC_FAR *pDestinationRect) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Advise( 
            /* [in] */ IOverlayNotify __RPC_FAR *pOverlayNotify,
            /* [in] */ DWORD dwInterests) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Unadvise( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IOverlayVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IOverlay __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IOverlay __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IOverlay __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetPalette )( 
            IOverlay __RPC_FAR * This,
            /* [out] */ DWORD __RPC_FAR *pdwColors,
            /* [out] */ PALETTEENTRY __RPC_FAR *__RPC_FAR *ppPalette);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetPalette )( 
            IOverlay __RPC_FAR * This,
            /* [in] */ DWORD dwColors,
            /* [in] */ PALETTEENTRY __RPC_FAR *pPalette);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDefaultColorKey )( 
            IOverlay __RPC_FAR * This,
            /* [out] */ COLORKEY __RPC_FAR *pColorKey);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetColorKey )( 
            IOverlay __RPC_FAR * This,
            /* [out] */ COLORKEY __RPC_FAR *pColorKey);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetColorKey )( 
            IOverlay __RPC_FAR * This,
            /* [out][in] */ COLORKEY __RPC_FAR *pColorKey);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetWindowHandle )( 
            IOverlay __RPC_FAR * This,
            /* [out] */ HWND __RPC_FAR *pHwnd);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetClipList )( 
            IOverlay __RPC_FAR * This,
            /* [out] */ RECT __RPC_FAR *pSourceRect,
            /* [out] */ RECT __RPC_FAR *pDestinationRect,
            /* [out] */ RGNDATA __RPC_FAR *__RPC_FAR *ppRgnData);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetVideoPosition )( 
            IOverlay __RPC_FAR * This,
            /* [out] */ RECT __RPC_FAR *pSourceRect,
            /* [out] */ RECT __RPC_FAR *pDestinationRect);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Advise )( 
            IOverlay __RPC_FAR * This,
            /* [in] */ IOverlayNotify __RPC_FAR *pOverlayNotify,
            /* [in] */ DWORD dwInterests);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Unadvise )( 
            IOverlay __RPC_FAR * This);
        
        END_INTERFACE
    } IOverlayVtbl;

    interface IOverlay
    {
        CONST_VTBL struct IOverlayVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IOverlay_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IOverlay_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IOverlay_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IOverlay_GetPalette(This,pdwColors,ppPalette)	\
    (This)->lpVtbl -> GetPalette(This,pdwColors,ppPalette)

#define IOverlay_SetPalette(This,dwColors,pPalette)	\
    (This)->lpVtbl -> SetPalette(This,dwColors,pPalette)

#define IOverlay_GetDefaultColorKey(This,pColorKey)	\
    (This)->lpVtbl -> GetDefaultColorKey(This,pColorKey)

#define IOverlay_GetColorKey(This,pColorKey)	\
    (This)->lpVtbl -> GetColorKey(This,pColorKey)

#define IOverlay_SetColorKey(This,pColorKey)	\
    (This)->lpVtbl -> SetColorKey(This,pColorKey)

#define IOverlay_GetWindowHandle(This,pHwnd)	\
    (This)->lpVtbl -> GetWindowHandle(This,pHwnd)

#define IOverlay_GetClipList(This,pSourceRect,pDestinationRect,ppRgnData)	\
    (This)->lpVtbl -> GetClipList(This,pSourceRect,pDestinationRect,ppRgnData)

#define IOverlay_GetVideoPosition(This,pSourceRect,pDestinationRect)	\
    (This)->lpVtbl -> GetVideoPosition(This,pSourceRect,pDestinationRect)

#define IOverlay_Advise(This,pOverlayNotify,dwInterests)	\
    (This)->lpVtbl -> Advise(This,pOverlayNotify,dwInterests)

#define IOverlay_Unadvise(This)	\
    (This)->lpVtbl -> Unadvise(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IOverlay_GetPalette_Proxy( 
    IOverlay __RPC_FAR * This,
    /* [out] */ DWORD __RPC_FAR *pdwColors,
    /* [out] */ PALETTEENTRY __RPC_FAR *__RPC_FAR *ppPalette);


void __RPC_STUB IOverlay_GetPalette_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOverlay_SetPalette_Proxy( 
    IOverlay __RPC_FAR * This,
    /* [in] */ DWORD dwColors,
    /* [in] */ PALETTEENTRY __RPC_FAR *pPalette);


void __RPC_STUB IOverlay_SetPalette_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOverlay_GetDefaultColorKey_Proxy( 
    IOverlay __RPC_FAR * This,
    /* [out] */ COLORKEY __RPC_FAR *pColorKey);


void __RPC_STUB IOverlay_GetDefaultColorKey_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOverlay_GetColorKey_Proxy( 
    IOverlay __RPC_FAR * This,
    /* [out] */ COLORKEY __RPC_FAR *pColorKey);


void __RPC_STUB IOverlay_GetColorKey_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOverlay_SetColorKey_Proxy( 
    IOverlay __RPC_FAR * This,
    /* [out][in] */ COLORKEY __RPC_FAR *pColorKey);


void __RPC_STUB IOverlay_SetColorKey_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOverlay_GetWindowHandle_Proxy( 
    IOverlay __RPC_FAR * This,
    /* [out] */ HWND __RPC_FAR *pHwnd);


void __RPC_STUB IOverlay_GetWindowHandle_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOverlay_GetClipList_Proxy( 
    IOverlay __RPC_FAR * This,
    /* [out] */ RECT __RPC_FAR *pSourceRect,
    /* [out] */ RECT __RPC_FAR *pDestinationRect,
    /* [out] */ RGNDATA __RPC_FAR *__RPC_FAR *ppRgnData);


void __RPC_STUB IOverlay_GetClipList_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOverlay_GetVideoPosition_Proxy( 
    IOverlay __RPC_FAR * This,
    /* [out] */ RECT __RPC_FAR *pSourceRect,
    /* [out] */ RECT __RPC_FAR *pDestinationRect);


void __RPC_STUB IOverlay_GetVideoPosition_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOverlay_Advise_Proxy( 
    IOverlay __RPC_FAR * This,
    /* [in] */ IOverlayNotify __RPC_FAR *pOverlayNotify,
    /* [in] */ DWORD dwInterests);


void __RPC_STUB IOverlay_Advise_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IOverlay_Unadvise_Proxy( 
    IOverlay __RPC_FAR * This);


void __RPC_STUB IOverlay_Unadvise_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IOverlay_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_strmif_0081 */
/* [local] */ 

typedef IOverlay __RPC_FAR *POVERLAY;



extern RPC_IF_HANDLE __MIDL_itf_strmif_0081_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_strmif_0081_v0_0_s_ifspec;

#ifndef __IMediaEventSink_INTERFACE_DEFINED__
#define __IMediaEventSink_INTERFACE_DEFINED__

/* interface IMediaEventSink */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IMediaEventSink;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("56a868a2-0ad4-11ce-b03a-0020af0ba770")
    IMediaEventSink : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Notify( 
            /* [in] */ long EventCode,
            /* [in] */ LONG_PTR EventParam1,
            /* [in] */ LONG_PTR EventParam2) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMediaEventSinkVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IMediaEventSink __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IMediaEventSink __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IMediaEventSink __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Notify )( 
            IMediaEventSink __RPC_FAR * This,
            /* [in] */ long EventCode,
            /* [in] */ LONG_PTR EventParam1,
            /* [in] */ LONG_PTR EventParam2);
        
        END_INTERFACE
    } IMediaEventSinkVtbl;

    interface IMediaEventSink
    {
        CONST_VTBL struct IMediaEventSinkVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMediaEventSink_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IMediaEventSink_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IMediaEventSink_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IMediaEventSink_Notify(This,EventCode,EventParam1,EventParam2)	\
    (This)->lpVtbl -> Notify(This,EventCode,EventParam1,EventParam2)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IMediaEventSink_Notify_Proxy( 
    IMediaEventSink __RPC_FAR * This,
    /* [in] */ long EventCode,
    /* [in] */ LONG_PTR EventParam1,
    /* [in] */ LONG_PTR EventParam2);


void __RPC_STUB IMediaEventSink_Notify_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IMediaEventSink_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_strmif_0082 */
/* [local] */ 

typedef IMediaEventSink __RPC_FAR *PMEDIAEVENTSINK;



extern RPC_IF_HANDLE __MIDL_itf_strmif_0082_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_strmif_0082_v0_0_s_ifspec;

#ifndef __IFileSourceFilter_INTERFACE_DEFINED__
#define __IFileSourceFilter_INTERFACE_DEFINED__

/* interface IFileSourceFilter */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IFileSourceFilter;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("56a868a6-0ad4-11ce-b03a-0020af0ba770")
    IFileSourceFilter : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Load( 
            /* [in] */ LPCOLESTR pszFileName,
            /* [in] */ const AM_MEDIA_TYPE __RPC_FAR *pmt) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetCurFile( 
            /* [out] */ LPOLESTR __RPC_FAR *ppszFileName,
            /* [out] */ AM_MEDIA_TYPE __RPC_FAR *pmt) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IFileSourceFilterVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IFileSourceFilter __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IFileSourceFilter __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IFileSourceFilter __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Load )( 
            IFileSourceFilter __RPC_FAR * This,
            /* [in] */ LPCOLESTR pszFileName,
            /* [in] */ const AM_MEDIA_TYPE __RPC_FAR *pmt);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetCurFile )( 
            IFileSourceFilter __RPC_FAR * This,
            /* [out] */ LPOLESTR __RPC_FAR *ppszFileName,
            /* [out] */ AM_MEDIA_TYPE __RPC_FAR *pmt);
        
        END_INTERFACE
    } IFileSourceFilterVtbl;

    interface IFileSourceFilter
    {
        CONST_VTBL struct IFileSourceFilterVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IFileSourceFilter_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IFileSourceFilter_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IFileSourceFilter_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IFileSourceFilter_Load(This,pszFileName,pmt)	\
    (This)->lpVtbl -> Load(This,pszFileName,pmt)

#define IFileSourceFilter_GetCurFile(This,ppszFileName,pmt)	\
    (This)->lpVtbl -> GetCurFile(This,ppszFileName,pmt)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IFileSourceFilter_Load_Proxy( 
    IFileSourceFilter __RPC_FAR * This,
    /* [in] */ LPCOLESTR pszFileName,
    /* [in] */ const AM_MEDIA_TYPE __RPC_FAR *pmt);


void __RPC_STUB IFileSourceFilter_Load_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IFileSourceFilter_GetCurFile_Proxy( 
    IFileSourceFilter __RPC_FAR * This,
    /* [out] */ LPOLESTR __RPC_FAR *ppszFileName,
    /* [out] */ AM_MEDIA_TYPE __RPC_FAR *pmt);


void __RPC_STUB IFileSourceFilter_GetCurFile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IFileSourceFilter_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_strmif_0083 */
/* [local] */ 

typedef IFileSourceFilter __RPC_FAR *PFILTERFILESOURCE;



extern RPC_IF_HANDLE __MIDL_itf_strmif_0083_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_strmif_0083_v0_0_s_ifspec;

#ifndef __IFileSinkFilter_INTERFACE_DEFINED__
#define __IFileSinkFilter_INTERFACE_DEFINED__

/* interface IFileSinkFilter */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IFileSinkFilter;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("a2104830-7c70-11cf-8bce-00aa00a3f1a6")
    IFileSinkFilter : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SetFileName( 
            /* [in] */ LPCOLESTR pszFileName,
            /* [in] */ const AM_MEDIA_TYPE __RPC_FAR *pmt) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetCurFile( 
            /* [out] */ LPOLESTR __RPC_FAR *ppszFileName,
            /* [out] */ AM_MEDIA_TYPE __RPC_FAR *pmt) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IFileSinkFilterVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IFileSinkFilter __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IFileSinkFilter __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IFileSinkFilter __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetFileName )( 
            IFileSinkFilter __RPC_FAR * This,
            /* [in] */ LPCOLESTR pszFileName,
            /* [in] */ const AM_MEDIA_TYPE __RPC_FAR *pmt);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetCurFile )( 
            IFileSinkFilter __RPC_FAR * This,
            /* [out] */ LPOLESTR __RPC_FAR *ppszFileName,
            /* [out] */ AM_MEDIA_TYPE __RPC_FAR *pmt);
        
        END_INTERFACE
    } IFileSinkFilterVtbl;

    interface IFileSinkFilter
    {
        CONST_VTBL struct IFileSinkFilterVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IFileSinkFilter_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IFileSinkFilter_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IFileSinkFilter_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IFileSinkFilter_SetFileName(This,pszFileName,pmt)	\
    (This)->lpVtbl -> SetFileName(This,pszFileName,pmt)

#define IFileSinkFilter_GetCurFile(This,ppszFileName,pmt)	\
    (This)->lpVtbl -> GetCurFile(This,ppszFileName,pmt)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IFileSinkFilter_SetFileName_Proxy( 
    IFileSinkFilter __RPC_FAR * This,
    /* [in] */ LPCOLESTR pszFileName,
    /* [in] */ const AM_MEDIA_TYPE __RPC_FAR *pmt);


void __RPC_STUB IFileSinkFilter_SetFileName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IFileSinkFilter_GetCurFile_Proxy( 
    IFileSinkFilter __RPC_FAR * This,
    /* [out] */ LPOLESTR __RPC_FAR *ppszFileName,
    /* [out] */ AM_MEDIA_TYPE __RPC_FAR *pmt);


void __RPC_STUB IFileSinkFilter_GetCurFile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IFileSinkFilter_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_strmif_0084 */
/* [local] */ 

typedef IFileSinkFilter __RPC_FAR *PFILTERFILESINK;



extern RPC_IF_HANDLE __MIDL_itf_strmif_0084_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_strmif_0084_v0_0_s_ifspec;

#ifndef __IFileSinkFilter2_INTERFACE_DEFINED__
#define __IFileSinkFilter2_INTERFACE_DEFINED__

/* interface IFileSinkFilter2 */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IFileSinkFilter2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("00855B90-CE1B-11d0-BD4F-00A0C911CE86")
    IFileSinkFilter2 : public IFileSinkFilter
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SetMode( 
            /* [in] */ DWORD dwFlags) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetMode( 
            /* [out] */ DWORD __RPC_FAR *pdwFlags) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IFileSinkFilter2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IFileSinkFilter2 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IFileSinkFilter2 __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IFileSinkFilter2 __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetFileName )( 
            IFileSinkFilter2 __RPC_FAR * This,
            /* [in] */ LPCOLESTR pszFileName,
            /* [in] */ const AM_MEDIA_TYPE __RPC_FAR *pmt);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetCurFile )( 
            IFileSinkFilter2 __RPC_FAR * This,
            /* [out] */ LPOLESTR __RPC_FAR *ppszFileName,
            /* [out] */ AM_MEDIA_TYPE __RPC_FAR *pmt);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetMode )( 
            IFileSinkFilter2 __RPC_FAR * This,
            /* [in] */ DWORD dwFlags);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMode )( 
            IFileSinkFilter2 __RPC_FAR * This,
            /* [out] */ DWORD __RPC_FAR *pdwFlags);
        
        END_INTERFACE
    } IFileSinkFilter2Vtbl;

    interface IFileSinkFilter2
    {
        CONST_VTBL struct IFileSinkFilter2Vtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IFileSinkFilter2_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IFileSinkFilter2_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IFileSinkFilter2_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IFileSinkFilter2_SetFileName(This,pszFileName,pmt)	\
    (This)->lpVtbl -> SetFileName(This,pszFileName,pmt)

#define IFileSinkFilter2_GetCurFile(This,ppszFileName,pmt)	\
    (This)->lpVtbl -> GetCurFile(This,ppszFileName,pmt)


#define IFileSinkFilter2_SetMode(This,dwFlags)	\
    (This)->lpVtbl -> SetMode(This,dwFlags)

#define IFileSinkFilter2_GetMode(This,pdwFlags)	\
    (This)->lpVtbl -> GetMode(This,pdwFlags)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IFileSinkFilter2_SetMode_Proxy( 
    IFileSinkFilter2 __RPC_FAR * This,
    /* [in] */ DWORD dwFlags);


void __RPC_STUB IFileSinkFilter2_SetMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IFileSinkFilter2_GetMode_Proxy( 
    IFileSinkFilter2 __RPC_FAR * This,
    /* [out] */ DWORD __RPC_FAR *pdwFlags);


void __RPC_STUB IFileSinkFilter2_GetMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IFileSinkFilter2_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_strmif_0085 */
/* [local] */ 

typedef IFileSinkFilter2 __RPC_FAR *PFILESINKFILTER2;

typedef /* [public] */ 
enum __MIDL___MIDL_itf_strmif_0085_0001
    {	AM_FILE_OVERWRITE	= 0x1,
	AM_FILE_TEMPLATE	= 0x2
    }	AM_FILESINK_FLAGS;

typedef struct _AsyncIOReq
    {
    DWORD engine[ 4 ];
    /* [size_is] */ BYTE __RPC_FAR *lpv;
    DWORD cb;
    DWORD dwError;
    DWORD cbDone;
    LARGE_INTEGER liPos;
    DWORD hEvent;
    DWORD_PTR dwUser;
    }	AsyncIOReq;



extern RPC_IF_HANDLE __MIDL_itf_strmif_0085_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_strmif_0085_v0_0_s_ifspec;

#ifndef __IFileAsyncIO_INTERFACE_DEFINED__
#define __IFileAsyncIO_INTERFACE_DEFINED__

/* interface IFileAsyncIO */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IFileAsyncIO;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("56a868a7-0ad4-11ce-b03a-0020af0ba770")
    IFileAsyncIO : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE QueryAlignment( 
            /* [out] */ LPDWORD pdwAlign) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Read( 
            /* [in] */ AsyncIOReq __RPC_FAR *pReq) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Write( 
            /* [in] */ AsyncIOReq __RPC_FAR *pReq) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE WaitForNext( 
            /* [out] */ AsyncIOReq __RPC_FAR *__RPC_FAR *ppReq,
            /* [in] */ DWORD dwTimeout) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE WaitForSpecific( 
            /* [out] */ AsyncIOReq __RPC_FAR *pReq,
            /* [in] */ DWORD dwTimeout) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE DiscardPending( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Flush( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IFileAsyncIOVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IFileAsyncIO __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IFileAsyncIO __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IFileAsyncIO __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryAlignment )( 
            IFileAsyncIO __RPC_FAR * This,
            /* [out] */ LPDWORD pdwAlign);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Read )( 
            IFileAsyncIO __RPC_FAR * This,
            /* [in] */ AsyncIOReq __RPC_FAR *pReq);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Write )( 
            IFileAsyncIO __RPC_FAR * This,
            /* [in] */ AsyncIOReq __RPC_FAR *pReq);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *WaitForNext )( 
            IFileAsyncIO __RPC_FAR * This,
            /* [out] */ AsyncIOReq __RPC_FAR *__RPC_FAR *ppReq,
            /* [in] */ DWORD dwTimeout);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *WaitForSpecific )( 
            IFileAsyncIO __RPC_FAR * This,
            /* [out] */ AsyncIOReq __RPC_FAR *pReq,
            /* [in] */ DWORD dwTimeout);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DiscardPending )( 
            IFileAsyncIO __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Flush )( 
            IFileAsyncIO __RPC_FAR * This);
        
        END_INTERFACE
    } IFileAsyncIOVtbl;

    interface IFileAsyncIO
    {
        CONST_VTBL struct IFileAsyncIOVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IFileAsyncIO_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IFileAsyncIO_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IFileAsyncIO_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IFileAsyncIO_QueryAlignment(This,pdwAlign)	\
    (This)->lpVtbl -> QueryAlignment(This,pdwAlign)

#define IFileAsyncIO_Read(This,pReq)	\
    (This)->lpVtbl -> Read(This,pReq)

#define IFileAsyncIO_Write(This,pReq)	\
    (This)->lpVtbl -> Write(This,pReq)

#define IFileAsyncIO_WaitForNext(This,ppReq,dwTimeout)	\
    (This)->lpVtbl -> WaitForNext(This,ppReq,dwTimeout)

#define IFileAsyncIO_WaitForSpecific(This,pReq,dwTimeout)	\
    (This)->lpVtbl -> WaitForSpecific(This,pReq,dwTimeout)

#define IFileAsyncIO_DiscardPending(This)	\
    (This)->lpVtbl -> DiscardPending(This)

#define IFileAsyncIO_Flush(This)	\
    (This)->lpVtbl -> Flush(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IFileAsyncIO_QueryAlignment_Proxy( 
    IFileAsyncIO __RPC_FAR * This,
    /* [out] */ LPDWORD pdwAlign);


void __RPC_STUB IFileAsyncIO_QueryAlignment_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IFileAsyncIO_Read_Proxy( 
    IFileAsyncIO __RPC_FAR * This,
    /* [in] */ AsyncIOReq __RPC_FAR *pReq);


void __RPC_STUB IFileAsyncIO_Read_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IFileAsyncIO_Write_Proxy( 
    IFileAsyncIO __RPC_FAR * This,
    /* [in] */ AsyncIOReq __RPC_FAR *pReq);


void __RPC_STUB IFileAsyncIO_Write_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IFileAsyncIO_WaitForNext_Proxy( 
    IFileAsyncIO __RPC_FAR * This,
    /* [out] */ AsyncIOReq __RPC_FAR *__RPC_FAR *ppReq,
    /* [in] */ DWORD dwTimeout);


void __RPC_STUB IFileAsyncIO_WaitForNext_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IFileAsyncIO_WaitForSpecific_Proxy( 
    IFileAsyncIO __RPC_FAR * This,
    /* [out] */ AsyncIOReq __RPC_FAR *pReq,
    /* [in] */ DWORD dwTimeout);


void __RPC_STUB IFileAsyncIO_WaitForSpecific_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IFileAsyncIO_DiscardPending_Proxy( 
    IFileAsyncIO __RPC_FAR * This);


void __RPC_STUB IFileAsyncIO_DiscardPending_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IFileAsyncIO_Flush_Proxy( 
    IFileAsyncIO __RPC_FAR * This);


void __RPC_STUB IFileAsyncIO_Flush_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IFileAsyncIO_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_strmif_0086 */
/* [local] */ 

typedef IFileAsyncIO __RPC_FAR *PFILEASYNCIO;



extern RPC_IF_HANDLE __MIDL_itf_strmif_0086_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_strmif_0086_v0_0_s_ifspec;

#ifndef __IGraphBuilder_INTERFACE_DEFINED__
#define __IGraphBuilder_INTERFACE_DEFINED__

/* interface IGraphBuilder */
/* [unique][uuid][local][object] */ 


EXTERN_C const IID IID_IGraphBuilder;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("56a868a9-0ad4-11ce-b03a-0020af0ba770")
    IGraphBuilder : public IFilterGraph
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Connect( 
            /* [in] */ IPin __RPC_FAR *ppinOut,
            /* [in] */ IPin __RPC_FAR *ppinIn) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Render( 
            /* [in] */ IPin __RPC_FAR *ppinOut) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RenderFile( 
            /* [in] */ LPCWSTR lpcwstrFile,
            /* [in] */ LPCWSTR lpcwstrPlayList) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AddSourceFilter( 
            /* [in] */ LPCWSTR lpcwstrFileName,
            /* [in] */ LPCWSTR lpcwstrFilterName,
            /* [out] */ IBaseFilter __RPC_FAR *__RPC_FAR *ppFilter) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetLogFile( 
            /* [in] */ HANDLE hFile) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Abort( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ShouldOperationContinue( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IGraphBuilderVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IGraphBuilder __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IGraphBuilder __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IGraphBuilder __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddFilter )( 
            IGraphBuilder __RPC_FAR * This,
            /* [in] */ IBaseFilter __RPC_FAR *pFilter,
            /* [string][in] */ LPCWSTR pName);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RemoveFilter )( 
            IGraphBuilder __RPC_FAR * This,
            /* [in] */ IBaseFilter __RPC_FAR *pFilter);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *EnumFilters )( 
            IGraphBuilder __RPC_FAR * This,
            /* [out] */ IEnumFilters __RPC_FAR *__RPC_FAR *ppEnum);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *FindFilterByName )( 
            IGraphBuilder __RPC_FAR * This,
            /* [string][in] */ LPCWSTR pName,
            /* [out] */ IBaseFilter __RPC_FAR *__RPC_FAR *ppFilter);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ConnectDirect )( 
            IGraphBuilder __RPC_FAR * This,
            /* [in] */ IPin __RPC_FAR *ppinOut,
            /* [in] */ IPin __RPC_FAR *ppinIn,
            /* [in] */ const AM_MEDIA_TYPE __RPC_FAR *pmt);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Reconnect )( 
            IGraphBuilder __RPC_FAR * This,
            /* [in] */ IPin __RPC_FAR *ppin);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Disconnect )( 
            IGraphBuilder __RPC_FAR * This,
            /* [in] */ IPin __RPC_FAR *ppin);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetDefaultSyncSource )( 
            IGraphBuilder __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Connect )( 
            IGraphBuilder __RPC_FAR * This,
            /* [in] */ IPin __RPC_FAR *ppinOut,
            /* [in] */ IPin __RPC_FAR *ppinIn);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Render )( 
            IGraphBuilder __RPC_FAR * This,
            /* [in] */ IPin __RPC_FAR *ppinOut);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RenderFile )( 
            IGraphBuilder __RPC_FAR * This,
            /* [in] */ LPCWSTR lpcwstrFile,
            /* [in] */ LPCWSTR lpcwstrPlayList);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddSourceFilter )( 
            IGraphBuilder __RPC_FAR * This,
            /* [in] */ LPCWSTR lpcwstrFileName,
            /* [in] */ LPCWSTR lpcwstrFilterName,
            /* [out] */ IBaseFilter __RPC_FAR *__RPC_FAR *ppFilter);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetLogFile )( 
            IGraphBuilder __RPC_FAR * This,
            /* [in] */ HANDLE hFile);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Abort )( 
            IGraphBuilder __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ShouldOperationContinue )( 
            IGraphBuilder __RPC_FAR * This);
        
        END_INTERFACE
    } IGraphBuilderVtbl;

    interface IGraphBuilder
    {
        CONST_VTBL struct IGraphBuilderVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IGraphBuilder_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IGraphBuilder_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IGraphBuilder_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IGraphBuilder_AddFilter(This,pFilter,pName)	\
    (This)->lpVtbl -> AddFilter(This,pFilter,pName)

#define IGraphBuilder_RemoveFilter(This,pFilter)	\
    (This)->lpVtbl -> RemoveFilter(This,pFilter)

#define IGraphBuilder_EnumFilters(This,ppEnum)	\
    (This)->lpVtbl -> EnumFilters(This,ppEnum)

#define IGraphBuilder_FindFilterByName(This,pName,ppFilter)	\
    (This)->lpVtbl -> FindFilterByName(This,pName,ppFilter)

#define IGraphBuilder_ConnectDirect(This,ppinOut,ppinIn,pmt)	\
    (This)->lpVtbl -> ConnectDirect(This,ppinOut,ppinIn,pmt)

#define IGraphBuilder_Reconnect(This,ppin)	\
    (This)->lpVtbl -> Reconnect(This,ppin)

#define IGraphBuilder_Disconnect(This,ppin)	\
    (This)->lpVtbl -> Disconnect(This,ppin)

#define IGraphBuilder_SetDefaultSyncSource(This)	\
    (This)->lpVtbl -> SetDefaultSyncSource(This)


#define IGraphBuilder_Connect(This,ppinOut,ppinIn)	\
    (This)->lpVtbl -> Connect(This,ppinOut,ppinIn)

#define IGraphBuilder_Render(This,ppinOut)	\
    (This)->lpVtbl -> Render(This,ppinOut)

#define IGraphBuilder_RenderFile(This,lpcwstrFile,lpcwstrPlayList)	\
    (This)->lpVtbl -> RenderFile(This,lpcwstrFile,lpcwstrPlayList)

#define IGraphBuilder_AddSourceFilter(This,lpcwstrFileName,lpcwstrFilterName,ppFilter)	\
    (This)->lpVtbl -> AddSourceFilter(This,lpcwstrFileName,lpcwstrFilterName,ppFilter)

#define IGraphBuilder_SetLogFile(This,hFile)	\
    (This)->lpVtbl -> SetLogFile(This,hFile)

#define IGraphBuilder_Abort(This)	\
    (This)->lpVtbl -> Abort(This)

#define IGraphBuilder_ShouldOperationContinue(This)	\
    (This)->lpVtbl -> ShouldOperationContinue(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IGraphBuilder_Connect_Proxy( 
    IGraphBuilder __RPC_FAR * This,
    /* [in] */ IPin __RPC_FAR *ppinOut,
    /* [in] */ IPin __RPC_FAR *ppinIn);


void __RPC_STUB IGraphBuilder_Connect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IGraphBuilder_Render_Proxy( 
    IGraphBuilder __RPC_FAR * This,
    /* [in] */ IPin __RPC_FAR *ppinOut);


void __RPC_STUB IGraphBuilder_Render_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IGraphBuilder_RenderFile_Proxy( 
    IGraphBuilder __RPC_FAR * This,
    /* [in] */ LPCWSTR lpcwstrFile,
    /* [in] */ LPCWSTR lpcwstrPlayList);


void __RPC_STUB IGraphBuilder_RenderFile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IGraphBuilder_AddSourceFilter_Proxy( 
    IGraphBuilder __RPC_FAR * This,
    /* [in] */ LPCWSTR lpcwstrFileName,
    /* [in] */ LPCWSTR lpcwstrFilterName,
    /* [out] */ IBaseFilter __RPC_FAR *__RPC_FAR *ppFilter);


void __RPC_STUB IGraphBuilder_AddSourceFilter_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IGraphBuilder_SetLogFile_Proxy( 
    IGraphBuilder __RPC_FAR * This,
    /* [in] */ HANDLE hFile);


void __RPC_STUB IGraphBuilder_SetLogFile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IGraphBuilder_Abort_Proxy( 
    IGraphBuilder __RPC_FAR * This);


void __RPC_STUB IGraphBuilder_Abort_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IGraphBuilder_ShouldOperationContinue_Proxy( 
    IGraphBuilder __RPC_FAR * This);


void __RPC_STUB IGraphBuilder_ShouldOperationContinue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IGraphBuilder_INTERFACE_DEFINED__ */


#ifndef __ICaptureGraphBuilder_INTERFACE_DEFINED__
#define __ICaptureGraphBuilder_INTERFACE_DEFINED__

/* interface ICaptureGraphBuilder */
/* [unique][uuid][local][object] */ 


EXTERN_C const IID IID_ICaptureGraphBuilder;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("bf87b6e0-8c27-11d0-b3f0-00aa003761c5")
    ICaptureGraphBuilder : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SetFiltergraph( 
            /* [in] */ IGraphBuilder __RPC_FAR *pfg) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetFiltergraph( 
            /* [out] */ IGraphBuilder __RPC_FAR *__RPC_FAR *ppfg) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetOutputFileName( 
            /* [in] */ const GUID __RPC_FAR *pType,
            /* [in] */ LPCOLESTR lpstrFile,
            /* [out] */ IBaseFilter __RPC_FAR *__RPC_FAR *ppf,
            /* [out] */ IFileSinkFilter __RPC_FAR *__RPC_FAR *ppSink) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE FindInterface( 
            /* [in] */ const GUID __RPC_FAR *pCategory,
            /* [in] */ IBaseFilter __RPC_FAR *pf,
            /* [in] */ REFIID riid,
            /* [out] */ void __RPC_FAR *__RPC_FAR *ppint) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RenderStream( 
            /* [in] */ const GUID __RPC_FAR *pCategory,
            /* [in] */ IUnknown __RPC_FAR *pSource,
            /* [in] */ IBaseFilter __RPC_FAR *pfCompressor,
            /* [in] */ IBaseFilter __RPC_FAR *pfRenderer) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ControlStream( 
            /* [in] */ const GUID __RPC_FAR *pCategory,
            /* [in] */ IBaseFilter __RPC_FAR *pFilter,
            /* [in] */ REFERENCE_TIME __RPC_FAR *pstart,
            /* [in] */ REFERENCE_TIME __RPC_FAR *pstop,
            /* [in] */ WORD wStartCookie,
            /* [in] */ WORD wStopCookie) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AllocCapFile( 
            /* [in] */ LPCOLESTR lpstr,
            /* [in] */ DWORDLONG dwlSize) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CopyCaptureFile( 
            /* [in] */ LPOLESTR lpwstrOld,
            /* [in] */ LPOLESTR lpwstrNew,
            /* [in] */ int fAllowEscAbort,
            /* [in] */ IAMCopyCaptureFileProgress __RPC_FAR *pCallback) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ICaptureGraphBuilderVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ICaptureGraphBuilder __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ICaptureGraphBuilder __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ICaptureGraphBuilder __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetFiltergraph )( 
            ICaptureGraphBuilder __RPC_FAR * This,
            /* [in] */ IGraphBuilder __RPC_FAR *pfg);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetFiltergraph )( 
            ICaptureGraphBuilder __RPC_FAR * This,
            /* [out] */ IGraphBuilder __RPC_FAR *__RPC_FAR *ppfg);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetOutputFileName )( 
            ICaptureGraphBuilder __RPC_FAR * This,
            /* [in] */ const GUID __RPC_FAR *pType,
            /* [in] */ LPCOLESTR lpstrFile,
            /* [out] */ IBaseFilter __RPC_FAR *__RPC_FAR *ppf,
            /* [out] */ IFileSinkFilter __RPC_FAR *__RPC_FAR *ppSink);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *FindInterface )( 
            ICaptureGraphBuilder __RPC_FAR * This,
            /* [in] */ const GUID __RPC_FAR *pCategory,
            /* [in] */ IBaseFilter __RPC_FAR *pf,
            /* [in] */ REFIID riid,
            /* [out] */ void __RPC_FAR *__RPC_FAR *ppint);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RenderStream )( 
            ICaptureGraphBuilder __RPC_FAR * This,
            /* [in] */ const GUID __RPC_FAR *pCategory,
            /* [in] */ IUnknown __RPC_FAR *pSource,
            /* [in] */ IBaseFilter __RPC_FAR *pfCompressor,
            /* [in] */ IBaseFilter __RPC_FAR *pfRenderer);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ControlStream )( 
            ICaptureGraphBuilder __RPC_FAR * This,
            /* [in] */ const GUID __RPC_FAR *pCategory,
            /* [in] */ IBaseFilter __RPC_FAR *pFilter,
            /* [in] */ REFERENCE_TIME __RPC_FAR *pstart,
            /* [in] */ REFERENCE_TIME __RPC_FAR *pstop,
            /* [in] */ WORD wStartCookie,
            /* [in] */ WORD wStopCookie);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AllocCapFile )( 
            ICaptureGraphBuilder __RPC_FAR * This,
            /* [in] */ LPCOLESTR lpstr,
            /* [in] */ DWORDLONG dwlSize);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CopyCaptureFile )( 
            ICaptureGraphBuilder __RPC_FAR * This,
            /* [in] */ LPOLESTR lpwstrOld,
            /* [in] */ LPOLESTR lpwstrNew,
            /* [in] */ int fAllowEscAbort,
            /* [in] */ IAMCopyCaptureFileProgress __RPC_FAR *pCallback);
        
        END_INTERFACE
    } ICaptureGraphBuilderVtbl;

    interface ICaptureGraphBuilder
    {
        CONST_VTBL struct ICaptureGraphBuilderVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICaptureGraphBuilder_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ICaptureGraphBuilder_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ICaptureGraphBuilder_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ICaptureGraphBuilder_SetFiltergraph(This,pfg)	\
    (This)->lpVtbl -> SetFiltergraph(This,pfg)

#define ICaptureGraphBuilder_GetFiltergraph(This,ppfg)	\
    (This)->lpVtbl -> GetFiltergraph(This,ppfg)

#define ICaptureGraphBuilder_SetOutputFileName(This,pType,lpstrFile,ppf,ppSink)	\
    (This)->lpVtbl -> SetOutputFileName(This,pType,lpstrFile,ppf,ppSink)

#define ICaptureGraphBuilder_FindInterface(This,pCategory,pf,riid,ppint)	\
    (This)->lpVtbl -> FindInterface(This,pCategory,pf,riid,ppint)

#define ICaptureGraphBuilder_RenderStream(This,pCategory,pSource,pfCompressor,pfRenderer)	\
    (This)->lpVtbl -> RenderStream(This,pCategory,pSource,pfCompressor,pfRenderer)

#define ICaptureGraphBuilder_ControlStream(This,pCategory,pFilter,pstart,pstop,wStartCookie,wStopCookie)	\
    (This)->lpVtbl -> ControlStream(This,pCategory,pFilter,pstart,pstop,wStartCookie,wStopCookie)

#define ICaptureGraphBuilder_AllocCapFile(This,lpstr,dwlSize)	\
    (This)->lpVtbl -> AllocCapFile(This,lpstr,dwlSize)

#define ICaptureGraphBuilder_CopyCaptureFile(This,lpwstrOld,lpwstrNew,fAllowEscAbort,pCallback)	\
    (This)->lpVtbl -> CopyCaptureFile(This,lpwstrOld,lpwstrNew,fAllowEscAbort,pCallback)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE ICaptureGraphBuilder_SetFiltergraph_Proxy( 
    ICaptureGraphBuilder __RPC_FAR * This,
    /* [in] */ IGraphBuilder __RPC_FAR *pfg);


void __RPC_STUB ICaptureGraphBuilder_SetFiltergraph_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICaptureGraphBuilder_GetFiltergraph_Proxy( 
    ICaptureGraphBuilder __RPC_FAR * This,
    /* [out] */ IGraphBuilder __RPC_FAR *__RPC_FAR *ppfg);


void __RPC_STUB ICaptureGraphBuilder_GetFiltergraph_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICaptureGraphBuilder_SetOutputFileName_Proxy( 
    ICaptureGraphBuilder __RPC_FAR * This,
    /* [in] */ const GUID __RPC_FAR *pType,
    /* [in] */ LPCOLESTR lpstrFile,
    /* [out] */ IBaseFilter __RPC_FAR *__RPC_FAR *ppf,
    /* [out] */ IFileSinkFilter __RPC_FAR *__RPC_FAR *ppSink);


void __RPC_STUB ICaptureGraphBuilder_SetOutputFileName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICaptureGraphBuilder_FindInterface_Proxy( 
    ICaptureGraphBuilder __RPC_FAR * This,
    /* [in] */ const GUID __RPC_FAR *pCategory,
    /* [in] */ IBaseFilter __RPC_FAR *pf,
    /* [in] */ REFIID riid,
    /* [out] */ void __RPC_FAR *__RPC_FAR *ppint);


void __RPC_STUB ICaptureGraphBuilder_FindInterface_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICaptureGraphBuilder_RenderStream_Proxy( 
    ICaptureGraphBuilder __RPC_FAR * This,
    /* [in] */ const GUID __RPC_FAR *pCategory,
    /* [in] */ IUnknown __RPC_FAR *pSource,
    /* [in] */ IBaseFilter __RPC_FAR *pfCompressor,
    /* [in] */ IBaseFilter __RPC_FAR *pfRenderer);


void __RPC_STUB ICaptureGraphBuilder_RenderStream_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICaptureGraphBuilder_ControlStream_Proxy( 
    ICaptureGraphBuilder __RPC_FAR * This,
    /* [in] */ const GUID __RPC_FAR *pCategory,
    /* [in] */ IBaseFilter __RPC_FAR *pFilter,
    /* [in] */ REFERENCE_TIME __RPC_FAR *pstart,
    /* [in] */ REFERENCE_TIME __RPC_FAR *pstop,
    /* [in] */ WORD wStartCookie,
    /* [in] */ WORD wStopCookie);


void __RPC_STUB ICaptureGraphBuilder_ControlStream_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICaptureGraphBuilder_AllocCapFile_Proxy( 
    ICaptureGraphBuilder __RPC_FAR * This,
    /* [in] */ LPCOLESTR lpstr,
    /* [in] */ DWORDLONG dwlSize);


void __RPC_STUB ICaptureGraphBuilder_AllocCapFile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICaptureGraphBuilder_CopyCaptureFile_Proxy( 
    ICaptureGraphBuilder __RPC_FAR * This,
    /* [in] */ LPOLESTR lpwstrOld,
    /* [in] */ LPOLESTR lpwstrNew,
    /* [in] */ int fAllowEscAbort,
    /* [in] */ IAMCopyCaptureFileProgress __RPC_FAR *pCallback);


void __RPC_STUB ICaptureGraphBuilder_CopyCaptureFile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ICaptureGraphBuilder_INTERFACE_DEFINED__ */


#ifndef __IAMCopyCaptureFileProgress_INTERFACE_DEFINED__
#define __IAMCopyCaptureFileProgress_INTERFACE_DEFINED__

/* interface IAMCopyCaptureFileProgress */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IAMCopyCaptureFileProgress;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("670d1d20-a068-11d0-b3f0-00aa003761c5")
    IAMCopyCaptureFileProgress : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Progress( 
            /* [in] */ int iProgress) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAMCopyCaptureFileProgressVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IAMCopyCaptureFileProgress __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IAMCopyCaptureFileProgress __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IAMCopyCaptureFileProgress __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Progress )( 
            IAMCopyCaptureFileProgress __RPC_FAR * This,
            /* [in] */ int iProgress);
        
        END_INTERFACE
    } IAMCopyCaptureFileProgressVtbl;

    interface IAMCopyCaptureFileProgress
    {
        CONST_VTBL struct IAMCopyCaptureFileProgressVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAMCopyCaptureFileProgress_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAMCopyCaptureFileProgress_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAMCopyCaptureFileProgress_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IAMCopyCaptureFileProgress_Progress(This,iProgress)	\
    (This)->lpVtbl -> Progress(This,iProgress)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IAMCopyCaptureFileProgress_Progress_Proxy( 
    IAMCopyCaptureFileProgress __RPC_FAR * This,
    /* [in] */ int iProgress);


void __RPC_STUB IAMCopyCaptureFileProgress_Progress_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAMCopyCaptureFileProgress_INTERFACE_DEFINED__ */


#ifndef __ICaptureGraphBuilder2_INTERFACE_DEFINED__
#define __ICaptureGraphBuilder2_INTERFACE_DEFINED__

/* interface ICaptureGraphBuilder2 */
/* [unique][uuid][local][object] */ 


EXTERN_C const IID IID_ICaptureGraphBuilder2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("93E5A4E0-2D50-11d2-ABFA-00A0C9C6E38D")
    ICaptureGraphBuilder2 : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SetFiltergraph( 
            /* [in] */ IGraphBuilder __RPC_FAR *pfg) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetFiltergraph( 
            /* [out] */ IGraphBuilder __RPC_FAR *__RPC_FAR *ppfg) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetOutputFileName( 
            /* [in] */ const GUID __RPC_FAR *pType,
            /* [in] */ LPCOLESTR lpstrFile,
            /* [out] */ IBaseFilter __RPC_FAR *__RPC_FAR *ppf,
            /* [out] */ IFileSinkFilter __RPC_FAR *__RPC_FAR *ppSink) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE FindInterface( 
            /* [in] */ const GUID __RPC_FAR *pCategory,
            /* [in] */ const GUID __RPC_FAR *pType,
            /* [in] */ IBaseFilter __RPC_FAR *pf,
            /* [in] */ REFIID riid,
            /* [out] */ void __RPC_FAR *__RPC_FAR *ppint) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RenderStream( 
            /* [in] */ const GUID __RPC_FAR *pCategory,
            /* [in] */ const GUID __RPC_FAR *pType,
            /* [in] */ IUnknown __RPC_FAR *pSource,
            /* [in] */ IBaseFilter __RPC_FAR *pfCompressor,
            /* [in] */ IBaseFilter __RPC_FAR *pfRenderer) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ControlStream( 
            /* [in] */ const GUID __RPC_FAR *pCategory,
            /* [in] */ const GUID __RPC_FAR *pType,
            /* [in] */ IBaseFilter __RPC_FAR *pFilter,
            /* [in] */ REFERENCE_TIME __RPC_FAR *pstart,
            /* [in] */ REFERENCE_TIME __RPC_FAR *pstop,
            /* [in] */ WORD wStartCookie,
            /* [in] */ WORD wStopCookie) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AllocCapFile( 
            /* [in] */ LPCOLESTR lpstr,
            /* [in] */ DWORDLONG dwlSize) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CopyCaptureFile( 
            /* [in] */ LPOLESTR lpwstrOld,
            /* [in] */ LPOLESTR lpwstrNew,
            /* [in] */ int fAllowEscAbort,
            /* [in] */ IAMCopyCaptureFileProgress __RPC_FAR *pCallback) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE FindPin( 
            /* [in] */ IUnknown __RPC_FAR *pSource,
            /* [in] */ PIN_DIRECTION pindir,
            /* [in] */ const GUID __RPC_FAR *pCategory,
            /* [in] */ const GUID __RPC_FAR *pType,
            /* [in] */ BOOL fUnconnected,
            /* [in] */ int num,
            /* [out] */ IPin __RPC_FAR *__RPC_FAR *ppPin) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ICaptureGraphBuilder2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ICaptureGraphBuilder2 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ICaptureGraphBuilder2 __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ICaptureGraphBuilder2 __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetFiltergraph )( 
            ICaptureGraphBuilder2 __RPC_FAR * This,
            /* [in] */ IGraphBuilder __RPC_FAR *pfg);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetFiltergraph )( 
            ICaptureGraphBuilder2 __RPC_FAR * This,
            /* [out] */ IGraphBuilder __RPC_FAR *__RPC_FAR *ppfg);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetOutputFileName )( 
            ICaptureGraphBuilder2 __RPC_FAR * This,
            /* [in] */ const GUID __RPC_FAR *pType,
            /* [in] */ LPCOLESTR lpstrFile,
            /* [out] */ IBaseFilter __RPC_FAR *__RPC_FAR *ppf,
            /* [out] */ IFileSinkFilter __RPC_FAR *__RPC_FAR *ppSink);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *FindInterface )( 
            ICaptureGraphBuilder2 __RPC_FAR * This,
            /* [in] */ const GUID __RPC_FAR *pCategory,
            /* [in] */ const GUID __RPC_FAR *pType,
            /* [in] */ IBaseFilter __RPC_FAR *pf,
            /* [in] */ REFIID riid,
            /* [out] */ void __RPC_FAR *__RPC_FAR *ppint);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RenderStream )( 
            ICaptureGraphBuilder2 __RPC_FAR * This,
            /* [in] */ const GUID __RPC_FAR *pCategory,
            /* [in] */ const GUID __RPC_FAR *pType,
            /* [in] */ IUnknown __RPC_FAR *pSource,
            /* [in] */ IBaseFilter __RPC_FAR *pfCompressor,
            /* [in] */ IBaseFilter __RPC_FAR *pfRenderer);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ControlStream )( 
            ICaptureGraphBuilder2 __RPC_FAR * This,
            /* [in] */ const GUID __RPC_FAR *pCategory,
            /* [in] */ const GUID __RPC_FAR *pType,
            /* [in] */ IBaseFilter __RPC_FAR *pFilter,
            /* [in] */ REFERENCE_TIME __RPC_FAR *pstart,
            /* [in] */ REFERENCE_TIME __RPC_FAR *pstop,
            /* [in] */ WORD wStartCookie,
            /* [in] */ WORD wStopCookie);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AllocCapFile )( 
            ICaptureGraphBuilder2 __RPC_FAR * This,
            /* [in] */ LPCOLESTR lpstr,
            /* [in] */ DWORDLONG dwlSize);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CopyCaptureFile )( 
            ICaptureGraphBuilder2 __RPC_FAR * This,
            /* [in] */ LPOLESTR lpwstrOld,
            /* [in] */ LPOLESTR lpwstrNew,
            /* [in] */ int fAllowEscAbort,
            /* [in] */ IAMCopyCaptureFileProgress __RPC_FAR *pCallback);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *FindPin )( 
            ICaptureGraphBuilder2 __RPC_FAR * This,
            /* [in] */ IUnknown __RPC_FAR *pSource,
            /* [in] */ PIN_DIRECTION pindir,
            /* [in] */ const GUID __RPC_FAR *pCategory,
            /* [in] */ const GUID __RPC_FAR *pType,
            /* [in] */ BOOL fUnconnected,
            /* [in] */ int num,
            /* [out] */ IPin __RPC_FAR *__RPC_FAR *ppPin);
        
        END_INTERFACE
    } ICaptureGraphBuilder2Vtbl;

    interface ICaptureGraphBuilder2
    {
        CONST_VTBL struct ICaptureGraphBuilder2Vtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICaptureGraphBuilder2_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ICaptureGraphBuilder2_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ICaptureGraphBuilder2_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ICaptureGraphBuilder2_SetFiltergraph(This,pfg)	\
    (This)->lpVtbl -> SetFiltergraph(This,pfg)

#define ICaptureGraphBuilder2_GetFiltergraph(This,ppfg)	\
    (This)->lpVtbl -> GetFiltergraph(This,ppfg)

#define ICaptureGraphBuilder2_SetOutputFileName(This,pType,lpstrFile,ppf,ppSink)	\
    (This)->lpVtbl -> SetOutputFileName(This,pType,lpstrFile,ppf,ppSink)

#define ICaptureGraphBuilder2_FindInterface(This,pCategory,pType,pf,riid,ppint)	\
    (This)->lpVtbl -> FindInterface(This,pCategory,pType,pf,riid,ppint)

#define ICaptureGraphBuilder2_RenderStream(This,pCategory,pType,pSource,pfCompressor,pfRenderer)	\
    (This)->lpVtbl -> RenderStream(This,pCategory,pType,pSource,pfCompressor,pfRenderer)

#define ICaptureGraphBuilder2_ControlStream(This,pCategory,pType,pFilter,pstart,pstop,wStartCookie,wStopCookie)	\
    (This)->lpVtbl -> ControlStream(This,pCategory,pType,pFilter,pstart,pstop,wStartCookie,wStopCookie)

#define ICaptureGraphBuilder2_AllocCapFile(This,lpstr,dwlSize)	\
    (This)->lpVtbl -> AllocCapFile(This,lpstr,dwlSize)

#define ICaptureGraphBuilder2_CopyCaptureFile(This,lpwstrOld,lpwstrNew,fAllowEscAbort,pCallback)	\
    (This)->lpVtbl -> CopyCaptureFile(This,lpwstrOld,lpwstrNew,fAllowEscAbort,pCallback)

#define ICaptureGraphBuilder2_FindPin(This,pSource,pindir,pCategory,pType,fUnconnected,num,ppPin)	\
    (This)->lpVtbl -> FindPin(This,pSource,pindir,pCategory,pType,fUnconnected,num,ppPin)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE ICaptureGraphBuilder2_SetFiltergraph_Proxy( 
    ICaptureGraphBuilder2 __RPC_FAR * This,
    /* [in] */ IGraphBuilder __RPC_FAR *pfg);


void __RPC_STUB ICaptureGraphBuilder2_SetFiltergraph_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICaptureGraphBuilder2_GetFiltergraph_Proxy( 
    ICaptureGraphBuilder2 __RPC_FAR * This,
    /* [out] */ IGraphBuilder __RPC_FAR *__RPC_FAR *ppfg);


void __RPC_STUB ICaptureGraphBuilder2_GetFiltergraph_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICaptureGraphBuilder2_SetOutputFileName_Proxy( 
    ICaptureGraphBuilder2 __RPC_FAR * This,
    /* [in] */ const GUID __RPC_FAR *pType,
    /* [in] */ LPCOLESTR lpstrFile,
    /* [out] */ IBaseFilter __RPC_FAR *__RPC_FAR *ppf,
    /* [out] */ IFileSinkFilter __RPC_FAR *__RPC_FAR *ppSink);


void __RPC_STUB ICaptureGraphBuilder2_SetOutputFileName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICaptureGraphBuilder2_FindInterface_Proxy( 
    ICaptureGraphBuilder2 __RPC_FAR * This,
    /* [in] */ const GUID __RPC_FAR *pCategory,
    /* [in] */ const GUID __RPC_FAR *pType,
    /* [in] */ IBaseFilter __RPC_FAR *pf,
    /* [in] */ REFIID riid,
    /* [out] */ void __RPC_FAR *__RPC_FAR *ppint);


void __RPC_STUB ICaptureGraphBuilder2_FindInterface_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICaptureGraphBuilder2_RenderStream_Proxy( 
    ICaptureGraphBuilder2 __RPC_FAR * This,
    /* [in] */ const GUID __RPC_FAR *pCategory,
    /* [in] */ const GUID __RPC_FAR *pType,
    /* [in] */ IUnknown __RPC_FAR *pSource,
    /* [in] */ IBaseFilter __RPC_FAR *pfCompressor,
    /* [in] */ IBaseFilter __RPC_FAR *pfRenderer);


void __RPC_STUB ICaptureGraphBuilder2_RenderStream_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICaptureGraphBuilder2_ControlStream_Proxy( 
    ICaptureGraphBuilder2 __RPC_FAR * This,
    /* [in] */ const GUID __RPC_FAR *pCategory,
    /* [in] */ const GUID __RPC_FAR *pType,
    /* [in] */ IBaseFilter __RPC_FAR *pFilter,
    /* [in] */ REFERENCE_TIME __RPC_FAR *pstart,
    /* [in] */ REFERENCE_TIME __RPC_FAR *pstop,
    /* [in] */ WORD wStartCookie,
    /* [in] */ WORD wStopCookie);


void __RPC_STUB ICaptureGraphBuilder2_ControlStream_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICaptureGraphBuilder2_AllocCapFile_Proxy( 
    ICaptureGraphBuilder2 __RPC_FAR * This,
    /* [in] */ LPCOLESTR lpstr,
    /* [in] */ DWORDLONG dwlSize);


void __RPC_STUB ICaptureGraphBuilder2_AllocCapFile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICaptureGraphBuilder2_CopyCaptureFile_Proxy( 
    ICaptureGraphBuilder2 __RPC_FAR * This,
    /* [in] */ LPOLESTR lpwstrOld,
    /* [in] */ LPOLESTR lpwstrNew,
    /* [in] */ int fAllowEscAbort,
    /* [in] */ IAMCopyCaptureFileProgress __RPC_FAR *pCallback);


void __RPC_STUB ICaptureGraphBuilder2_CopyCaptureFile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICaptureGraphBuilder2_FindPin_Proxy( 
    ICaptureGraphBuilder2 __RPC_FAR * This,
    /* [in] */ IUnknown __RPC_FAR *pSource,
    /* [in] */ PIN_DIRECTION pindir,
    /* [in] */ const GUID __RPC_FAR *pCategory,
    /* [in] */ const GUID __RPC_FAR *pType,
    /* [in] */ BOOL fUnconnected,
    /* [in] */ int num,
    /* [out] */ IPin __RPC_FAR *__RPC_FAR *ppPin);


void __RPC_STUB ICaptureGraphBuilder2_FindPin_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ICaptureGraphBuilder2_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_strmif_0090 */
/* [local] */ 


enum _AM_RENSDEREXFLAGS
    {	AM_RENDEREX_RENDERTOEXISTINGRENDERERS	= 0x1
    };


extern RPC_IF_HANDLE __MIDL_itf_strmif_0090_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_strmif_0090_v0_0_s_ifspec;

#ifndef __IFilterGraph2_INTERFACE_DEFINED__
#define __IFilterGraph2_INTERFACE_DEFINED__

/* interface IFilterGraph2 */
/* [unique][uuid][local][object] */ 


EXTERN_C const IID IID_IFilterGraph2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("36b73882-c2c8-11cf-8b46-00805f6cef60")
    IFilterGraph2 : public IGraphBuilder
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE AddSourceFilterForMoniker( 
            /* [in] */ IMoniker __RPC_FAR *pMoniker,
            /* [in] */ IBindCtx __RPC_FAR *pCtx,
            /* [in] */ LPCWSTR lpcwstrFilterName,
            /* [out] */ IBaseFilter __RPC_FAR *__RPC_FAR *ppFilter) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ReconnectEx( 
            /* [in] */ IPin __RPC_FAR *ppin,
            /* [in] */ const AM_MEDIA_TYPE __RPC_FAR *pmt) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RenderEx( 
            /* [in] */ IPin __RPC_FAR *pPinOut,
            /* [in] */ DWORD dwFlags,
            /* [out][in] */ LPVOID pvContext) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IFilterGraph2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IFilterGraph2 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IFilterGraph2 __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IFilterGraph2 __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddFilter )( 
            IFilterGraph2 __RPC_FAR * This,
            /* [in] */ IBaseFilter __RPC_FAR *pFilter,
            /* [string][in] */ LPCWSTR pName);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RemoveFilter )( 
            IFilterGraph2 __RPC_FAR * This,
            /* [in] */ IBaseFilter __RPC_FAR *pFilter);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *EnumFilters )( 
            IFilterGraph2 __RPC_FAR * This,
            /* [out] */ IEnumFilters __RPC_FAR *__RPC_FAR *ppEnum);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *FindFilterByName )( 
            IFilterGraph2 __RPC_FAR * This,
            /* [string][in] */ LPCWSTR pName,
            /* [out] */ IBaseFilter __RPC_FAR *__RPC_FAR *ppFilter);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ConnectDirect )( 
            IFilterGraph2 __RPC_FAR * This,
            /* [in] */ IPin __RPC_FAR *ppinOut,
            /* [in] */ IPin __RPC_FAR *ppinIn,
            /* [in] */ const AM_MEDIA_TYPE __RPC_FAR *pmt);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Reconnect )( 
            IFilterGraph2 __RPC_FAR * This,
            /* [in] */ IPin __RPC_FAR *ppin);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Disconnect )( 
            IFilterGraph2 __RPC_FAR * This,
            /* [in] */ IPin __RPC_FAR *ppin);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetDefaultSyncSource )( 
            IFilterGraph2 __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Connect )( 
            IFilterGraph2 __RPC_FAR * This,
            /* [in] */ IPin __RPC_FAR *ppinOut,
            /* [in] */ IPin __RPC_FAR *ppinIn);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Render )( 
            IFilterGraph2 __RPC_FAR * This,
            /* [in] */ IPin __RPC_FAR *ppinOut);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RenderFile )( 
            IFilterGraph2 __RPC_FAR * This,
            /* [in] */ LPCWSTR lpcwstrFile,
            /* [in] */ LPCWSTR lpcwstrPlayList);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddSourceFilter )( 
            IFilterGraph2 __RPC_FAR * This,
            /* [in] */ LPCWSTR lpcwstrFileName,
            /* [in] */ LPCWSTR lpcwstrFilterName,
            /* [out] */ IBaseFilter __RPC_FAR *__RPC_FAR *ppFilter);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetLogFile )( 
            IFilterGraph2 __RPC_FAR * This,
            /* [in] */ HANDLE hFile);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Abort )( 
            IFilterGraph2 __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ShouldOperationContinue )( 
            IFilterGraph2 __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddSourceFilterForMoniker )( 
            IFilterGraph2 __RPC_FAR * This,
            /* [in] */ IMoniker __RPC_FAR *pMoniker,
            /* [in] */ IBindCtx __RPC_FAR *pCtx,
            /* [in] */ LPCWSTR lpcwstrFilterName,
            /* [out] */ IBaseFilter __RPC_FAR *__RPC_FAR *ppFilter);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ReconnectEx )( 
            IFilterGraph2 __RPC_FAR * This,
            /* [in] */ IPin __RPC_FAR *ppin,
            /* [in] */ const AM_MEDIA_TYPE __RPC_FAR *pmt);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RenderEx )( 
            IFilterGraph2 __RPC_FAR * This,
            /* [in] */ IPin __RPC_FAR *pPinOut,
            /* [in] */ DWORD dwFlags,
            /* [out][in] */ LPVOID pvContext);
        
        END_INTERFACE
    } IFilterGraph2Vtbl;

    interface IFilterGraph2
    {
        CONST_VTBL struct IFilterGraph2Vtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IFilterGraph2_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IFilterGraph2_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IFilterGraph2_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IFilterGraph2_AddFilter(This,pFilter,pName)	\
    (This)->lpVtbl -> AddFilter(This,pFilter,pName)

#define IFilterGraph2_RemoveFilter(This,pFilter)	\
    (This)->lpVtbl -> RemoveFilter(This,pFilter)

#define IFilterGraph2_EnumFilters(This,ppEnum)	\
    (This)->lpVtbl -> EnumFilters(This,ppEnum)

#define IFilterGraph2_FindFilterByName(This,pName,ppFilter)	\
    (This)->lpVtbl -> FindFilterByName(This,pName,ppFilter)

#define IFilterGraph2_ConnectDirect(This,ppinOut,ppinIn,pmt)	\
    (This)->lpVtbl -> ConnectDirect(This,ppinOut,ppinIn,pmt)

#define IFilterGraph2_Reconnect(This,ppin)	\
    (This)->lpVtbl -> Reconnect(This,ppin)

#define IFilterGraph2_Disconnect(This,ppin)	\
    (This)->lpVtbl -> Disconnect(This,ppin)

#define IFilterGraph2_SetDefaultSyncSource(This)	\
    (This)->lpVtbl -> SetDefaultSyncSource(This)


#define IFilterGraph2_Connect(This,ppinOut,ppinIn)	\
    (This)->lpVtbl -> Connect(This,ppinOut,ppinIn)

#define IFilterGraph2_Render(This,ppinOut)	\
    (This)->lpVtbl -> Render(This,ppinOut)

#define IFilterGraph2_RenderFile(This,lpcwstrFile,lpcwstrPlayList)	\
    (This)->lpVtbl -> RenderFile(This,lpcwstrFile,lpcwstrPlayList)

#define IFilterGraph2_AddSourceFilter(This,lpcwstrFileName,lpcwstrFilterName,ppFilter)	\
    (This)->lpVtbl -> AddSourceFilter(This,lpcwstrFileName,lpcwstrFilterName,ppFilter)

#define IFilterGraph2_SetLogFile(This,hFile)	\
    (This)->lpVtbl -> SetLogFile(This,hFile)

#define IFilterGraph2_Abort(This)	\
    (This)->lpVtbl -> Abort(This)

#define IFilterGraph2_ShouldOperationContinue(This)	\
    (This)->lpVtbl -> ShouldOperationContinue(This)


#define IFilterGraph2_AddSourceFilterForMoniker(This,pMoniker,pCtx,lpcwstrFilterName,ppFilter)	\
    (This)->lpVtbl -> AddSourceFilterForMoniker(This,pMoniker,pCtx,lpcwstrFilterName,ppFilter)

#define IFilterGraph2_ReconnectEx(This,ppin,pmt)	\
    (This)->lpVtbl -> ReconnectEx(This,ppin,pmt)

#define IFilterGraph2_RenderEx(This,pPinOut,dwFlags,pvContext)	\
    (This)->lpVtbl -> RenderEx(This,pPinOut,dwFlags,pvContext)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IFilterGraph2_AddSourceFilterForMoniker_Proxy( 
    IFilterGraph2 __RPC_FAR * This,
    /* [in] */ IMoniker __RPC_FAR *pMoniker,
    /* [in] */ IBindCtx __RPC_FAR *pCtx,
    /* [in] */ LPCWSTR lpcwstrFilterName,
    /* [out] */ IBaseFilter __RPC_FAR *__RPC_FAR *ppFilter);


void __RPC_STUB IFilterGraph2_AddSourceFilterForMoniker_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IFilterGraph2_ReconnectEx_Proxy( 
    IFilterGraph2 __RPC_FAR * This,
    /* [in] */ IPin __RPC_FAR *ppin,
    /* [in] */ const AM_MEDIA_TYPE __RPC_FAR *pmt);


void __RPC_STUB IFilterGraph2_ReconnectEx_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IFilterGraph2_RenderEx_Proxy( 
    IFilterGraph2 __RPC_FAR * This,
    /* [in] */ IPin __RPC_FAR *pPinOut,
    /* [in] */ DWORD dwFlags,
    /* [out][in] */ LPVOID pvContext);


void __RPC_STUB IFilterGraph2_RenderEx_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IFilterGraph2_INTERFACE_DEFINED__ */


#ifndef __IStreamBuilder_INTERFACE_DEFINED__
#define __IStreamBuilder_INTERFACE_DEFINED__

/* interface IStreamBuilder */
/* [unique][uuid][local][object] */ 


EXTERN_C const IID IID_IStreamBuilder;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("56a868bf-0ad4-11ce-b03a-0020af0ba770")
    IStreamBuilder : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Render( 
            /* [in] */ IPin __RPC_FAR *ppinOut,
            /* [in] */ IGraphBuilder __RPC_FAR *pGraph) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Backout( 
            /* [in] */ IPin __RPC_FAR *ppinOut,
            /* [in] */ IGraphBuilder __RPC_FAR *pGraph) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IStreamBuilderVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IStreamBuilder __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IStreamBuilder __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IStreamBuilder __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Render )( 
            IStreamBuilder __RPC_FAR * This,
            /* [in] */ IPin __RPC_FAR *ppinOut,
            /* [in] */ IGraphBuilder __RPC_FAR *pGraph);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Backout )( 
            IStreamBuilder __RPC_FAR * This,
            /* [in] */ IPin __RPC_FAR *ppinOut,
            /* [in] */ IGraphBuilder __RPC_FAR *pGraph);
        
        END_INTERFACE
    } IStreamBuilderVtbl;

    interface IStreamBuilder
    {
        CONST_VTBL struct IStreamBuilderVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IStreamBuilder_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IStreamBuilder_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IStreamBuilder_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IStreamBuilder_Render(This,ppinOut,pGraph)	\
    (This)->lpVtbl -> Render(This,ppinOut,pGraph)

#define IStreamBuilder_Backout(This,ppinOut,pGraph)	\
    (This)->lpVtbl -> Backout(This,ppinOut,pGraph)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IStreamBuilder_Render_Proxy( 
    IStreamBuilder __RPC_FAR * This,
    /* [in] */ IPin __RPC_FAR *ppinOut,
    /* [in] */ IGraphBuilder __RPC_FAR *pGraph);


void __RPC_STUB IStreamBuilder_Render_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IStreamBuilder_Backout_Proxy( 
    IStreamBuilder __RPC_FAR * This,
    /* [in] */ IPin __RPC_FAR *ppinOut,
    /* [in] */ IGraphBuilder __RPC_FAR *pGraph);


void __RPC_STUB IStreamBuilder_Backout_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IStreamBuilder_INTERFACE_DEFINED__ */


#ifndef __IAsyncReader_INTERFACE_DEFINED__
#define __IAsyncReader_INTERFACE_DEFINED__

/* interface IAsyncReader */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IAsyncReader;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("56a868aa-0ad4-11ce-b03a-0020af0ba770")
    IAsyncReader : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE RequestAllocator( 
            /* [in] */ IMemAllocator __RPC_FAR *pPreferred,
            /* [in] */ ALLOCATOR_PROPERTIES __RPC_FAR *pProps,
            /* [out] */ IMemAllocator __RPC_FAR *__RPC_FAR *ppActual) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Request( 
            /* [in] */ IMediaSample __RPC_FAR *pSample,
            /* [in] */ DWORD_PTR dwUser) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE WaitForNext( 
            /* [in] */ DWORD dwTimeout,
            /* [out] */ IMediaSample __RPC_FAR *__RPC_FAR *ppSample,
            /* [out] */ DWORD_PTR __RPC_FAR *pdwUser) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SyncReadAligned( 
            /* [in] */ IMediaSample __RPC_FAR *pSample) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SyncRead( 
            /* [in] */ LONGLONG llPosition,
            /* [in] */ LONG lLength,
            /* [size_is][out] */ BYTE __RPC_FAR *pBuffer) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Length( 
            /* [out] */ LONGLONG __RPC_FAR *pTotal,
            /* [out] */ LONGLONG __RPC_FAR *pAvailable) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE BeginFlush( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE EndFlush( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAsyncReaderVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IAsyncReader __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IAsyncReader __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IAsyncReader __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RequestAllocator )( 
            IAsyncReader __RPC_FAR * This,
            /* [in] */ IMemAllocator __RPC_FAR *pPreferred,
            /* [in] */ ALLOCATOR_PROPERTIES __RPC_FAR *pProps,
            /* [out] */ IMemAllocator __RPC_FAR *__RPC_FAR *ppActual);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Request )( 
            IAsyncReader __RPC_FAR * This,
            /* [in] */ IMediaSample __RPC_FAR *pSample,
            /* [in] */ DWORD_PTR dwUser);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *WaitForNext )( 
            IAsyncReader __RPC_FAR * This,
            /* [in] */ DWORD dwTimeout,
            /* [out] */ IMediaSample __RPC_FAR *__RPC_FAR *ppSample,
            /* [out] */ DWORD_PTR __RPC_FAR *pdwUser);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SyncReadAligned )( 
            IAsyncReader __RPC_FAR * This,
            /* [in] */ IMediaSample __RPC_FAR *pSample);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SyncRead )( 
            IAsyncReader __RPC_FAR * This,
            /* [in] */ LONGLONG llPosition,
            /* [in] */ LONG lLength,
            /* [size_is][out] */ BYTE __RPC_FAR *pBuffer);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Length )( 
            IAsyncReader __RPC_FAR * This,
            /* [out] */ LONGLONG __RPC_FAR *pTotal,
            /* [out] */ LONGLONG __RPC_FAR *pAvailable);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *BeginFlush )( 
            IAsyncReader __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *EndFlush )( 
            IAsyncReader __RPC_FAR * This);
        
        END_INTERFACE
    } IAsyncReaderVtbl;

    interface IAsyncReader
    {
        CONST_VTBL struct IAsyncReaderVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAsyncReader_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAsyncReader_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAsyncReader_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IAsyncReader_RequestAllocator(This,pPreferred,pProps,ppActual)	\
    (This)->lpVtbl -> RequestAllocator(This,pPreferred,pProps,ppActual)

#define IAsyncReader_Request(This,pSample,dwUser)	\
    (This)->lpVtbl -> Request(This,pSample,dwUser)

#define IAsyncReader_WaitForNext(This,dwTimeout,ppSample,pdwUser)	\
    (This)->lpVtbl -> WaitForNext(This,dwTimeout,ppSample,pdwUser)

#define IAsyncReader_SyncReadAligned(This,pSample)	\
    (This)->lpVtbl -> SyncReadAligned(This,pSample)

#define IAsyncReader_SyncRead(This,llPosition,lLength,pBuffer)	\
    (This)->lpVtbl -> SyncRead(This,llPosition,lLength,pBuffer)

#define IAsyncReader_Length(This,pTotal,pAvailable)	\
    (This)->lpVtbl -> Length(This,pTotal,pAvailable)

#define IAsyncReader_BeginFlush(This)	\
    (This)->lpVtbl -> BeginFlush(This)

#define IAsyncReader_EndFlush(This)	\
    (This)->lpVtbl -> EndFlush(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IAsyncReader_RequestAllocator_Proxy( 
    IAsyncReader __RPC_FAR * This,
    /* [in] */ IMemAllocator __RPC_FAR *pPreferred,
    /* [in] */ ALLOCATOR_PROPERTIES __RPC_FAR *pProps,
    /* [out] */ IMemAllocator __RPC_FAR *__RPC_FAR *ppActual);


void __RPC_STUB IAsyncReader_RequestAllocator_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAsyncReader_Request_Proxy( 
    IAsyncReader __RPC_FAR * This,
    /* [in] */ IMediaSample __RPC_FAR *pSample,
    /* [in] */ DWORD_PTR dwUser);


void __RPC_STUB IAsyncReader_Request_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAsyncReader_WaitForNext_Proxy( 
    IAsyncReader __RPC_FAR * This,
    /* [in] */ DWORD dwTimeout,
    /* [out] */ IMediaSample __RPC_FAR *__RPC_FAR *ppSample,
    /* [out] */ DWORD_PTR __RPC_FAR *pdwUser);


void __RPC_STUB IAsyncReader_WaitForNext_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAsyncReader_SyncReadAligned_Proxy( 
    IAsyncReader __RPC_FAR * This,
    /* [in] */ IMediaSample __RPC_FAR *pSample);


void __RPC_STUB IAsyncReader_SyncReadAligned_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAsyncReader_SyncRead_Proxy( 
    IAsyncReader __RPC_FAR * This,
    /* [in] */ LONGLONG llPosition,
    /* [in] */ LONG lLength,
    /* [size_is][out] */ BYTE __RPC_FAR *pBuffer);


void __RPC_STUB IAsyncReader_SyncRead_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAsyncReader_Length_Proxy( 
    IAsyncReader __RPC_FAR * This,
    /* [out] */ LONGLONG __RPC_FAR *pTotal,
    /* [out] */ LONGLONG __RPC_FAR *pAvailable);


void __RPC_STUB IAsyncReader_Length_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAsyncReader_BeginFlush_Proxy( 
    IAsyncReader __RPC_FAR * This);


void __RPC_STUB IAsyncReader_BeginFlush_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAsyncReader_EndFlush_Proxy( 
    IAsyncReader __RPC_FAR * This);


void __RPC_STUB IAsyncReader_EndFlush_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAsyncReader_INTERFACE_DEFINED__ */


#ifndef __IGraphVersion_INTERFACE_DEFINED__
#define __IGraphVersion_INTERFACE_DEFINED__

/* interface IGraphVersion */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IGraphVersion;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("56a868ab-0ad4-11ce-b03a-0020af0ba770")
    IGraphVersion : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE QueryVersion( 
            LONG __RPC_FAR *pVersion) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IGraphVersionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IGraphVersion __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IGraphVersion __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IGraphVersion __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryVersion )( 
            IGraphVersion __RPC_FAR * This,
            LONG __RPC_FAR *pVersion);
        
        END_INTERFACE
    } IGraphVersionVtbl;

    interface IGraphVersion
    {
        CONST_VTBL struct IGraphVersionVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IGraphVersion_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IGraphVersion_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IGraphVersion_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IGraphVersion_QueryVersion(This,pVersion)	\
    (This)->lpVtbl -> QueryVersion(This,pVersion)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IGraphVersion_QueryVersion_Proxy( 
    IGraphVersion __RPC_FAR * This,
    LONG __RPC_FAR *pVersion);


void __RPC_STUB IGraphVersion_QueryVersion_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IGraphVersion_INTERFACE_DEFINED__ */


#ifndef __IResourceConsumer_INTERFACE_DEFINED__
#define __IResourceConsumer_INTERFACE_DEFINED__

/* interface IResourceConsumer */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IResourceConsumer;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("56a868ad-0ad4-11ce-b03a-0020af0ba770")
    IResourceConsumer : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE AcquireResource( 
            /* [in] */ LONG idResource) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ReleaseResource( 
            /* [in] */ LONG idResource) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IResourceConsumerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IResourceConsumer __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IResourceConsumer __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IResourceConsumer __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AcquireResource )( 
            IResourceConsumer __RPC_FAR * This,
            /* [in] */ LONG idResource);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ReleaseResource )( 
            IResourceConsumer __RPC_FAR * This,
            /* [in] */ LONG idResource);
        
        END_INTERFACE
    } IResourceConsumerVtbl;

    interface IResourceConsumer
    {
        CONST_VTBL struct IResourceConsumerVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IResourceConsumer_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IResourceConsumer_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IResourceConsumer_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IResourceConsumer_AcquireResource(This,idResource)	\
    (This)->lpVtbl -> AcquireResource(This,idResource)

#define IResourceConsumer_ReleaseResource(This,idResource)	\
    (This)->lpVtbl -> ReleaseResource(This,idResource)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IResourceConsumer_AcquireResource_Proxy( 
    IResourceConsumer __RPC_FAR * This,
    /* [in] */ LONG idResource);


void __RPC_STUB IResourceConsumer_AcquireResource_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IResourceConsumer_ReleaseResource_Proxy( 
    IResourceConsumer __RPC_FAR * This,
    /* [in] */ LONG idResource);


void __RPC_STUB IResourceConsumer_ReleaseResource_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IResourceConsumer_INTERFACE_DEFINED__ */


#ifndef __IResourceManager_INTERFACE_DEFINED__
#define __IResourceManager_INTERFACE_DEFINED__

/* interface IResourceManager */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IResourceManager;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("56a868ac-0ad4-11ce-b03a-0020af0ba770")
    IResourceManager : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Register( 
            /* [in] */ LPCWSTR pName,
            /* [in] */ LONG cResource,
            /* [out] */ LONG __RPC_FAR *plToken) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RegisterGroup( 
            /* [in] */ LPCWSTR pName,
            /* [in] */ LONG cResource,
            /* [size_is][in] */ LONG __RPC_FAR *palTokens,
            /* [out] */ LONG __RPC_FAR *plToken) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RequestResource( 
            /* [in] */ LONG idResource,
            /* [in] */ IUnknown __RPC_FAR *pFocusObject,
            /* [in] */ IResourceConsumer __RPC_FAR *pConsumer) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE NotifyAcquire( 
            /* [in] */ LONG idResource,
            /* [in] */ IResourceConsumer __RPC_FAR *pConsumer,
            /* [in] */ HRESULT hr) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE NotifyRelease( 
            /* [in] */ LONG idResource,
            /* [in] */ IResourceConsumer __RPC_FAR *pConsumer,
            /* [in] */ BOOL bStillWant) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CancelRequest( 
            /* [in] */ LONG idResource,
            /* [in] */ IResourceConsumer __RPC_FAR *pConsumer) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetFocus( 
            /* [in] */ IUnknown __RPC_FAR *pFocusObject) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ReleaseFocus( 
            /* [in] */ IUnknown __RPC_FAR *pFocusObject) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IResourceManagerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IResourceManager __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IResourceManager __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IResourceManager __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Register )( 
            IResourceManager __RPC_FAR * This,
            /* [in] */ LPCWSTR pName,
            /* [in] */ LONG cResource,
            /* [out] */ LONG __RPC_FAR *plToken);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RegisterGroup )( 
            IResourceManager __RPC_FAR * This,
            /* [in] */ LPCWSTR pName,
            /* [in] */ LONG cResource,
            /* [size_is][in] */ LONG __RPC_FAR *palTokens,
            /* [out] */ LONG __RPC_FAR *plToken);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RequestResource )( 
            IResourceManager __RPC_FAR * This,
            /* [in] */ LONG idResource,
            /* [in] */ IUnknown __RPC_FAR *pFocusObject,
            /* [in] */ IResourceConsumer __RPC_FAR *pConsumer);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *NotifyAcquire )( 
            IResourceManager __RPC_FAR * This,
            /* [in] */ LONG idResource,
            /* [in] */ IResourceConsumer __RPC_FAR *pConsumer,
            /* [in] */ HRESULT hr);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *NotifyRelease )( 
            IResourceManager __RPC_FAR * This,
            /* [in] */ LONG idResource,
            /* [in] */ IResourceConsumer __RPC_FAR *pConsumer,
            /* [in] */ BOOL bStillWant);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CancelRequest )( 
            IResourceManager __RPC_FAR * This,
            /* [in] */ LONG idResource,
            /* [in] */ IResourceConsumer __RPC_FAR *pConsumer);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetFocus )( 
            IResourceManager __RPC_FAR * This,
            /* [in] */ IUnknown __RPC_FAR *pFocusObject);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ReleaseFocus )( 
            IResourceManager __RPC_FAR * This,
            /* [in] */ IUnknown __RPC_FAR *pFocusObject);
        
        END_INTERFACE
    } IResourceManagerVtbl;

    interface IResourceManager
    {
        CONST_VTBL struct IResourceManagerVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IResourceManager_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IResourceManager_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IResourceManager_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IResourceManager_Register(This,pName,cResource,plToken)	\
    (This)->lpVtbl -> Register(This,pName,cResource,plToken)

#define IResourceManager_RegisterGroup(This,pName,cResource,palTokens,plToken)	\
    (This)->lpVtbl -> RegisterGroup(This,pName,cResource,palTokens,plToken)

#define IResourceManager_RequestResource(This,idResource,pFocusObject,pConsumer)	\
    (This)->lpVtbl -> RequestResource(This,idResource,pFocusObject,pConsumer)

#define IResourceManager_NotifyAcquire(This,idResource,pConsumer,hr)	\
    (This)->lpVtbl -> NotifyAcquire(This,idResource,pConsumer,hr)

#define IResourceManager_NotifyRelease(This,idResource,pConsumer,bStillWant)	\
    (This)->lpVtbl -> NotifyRelease(This,idResource,pConsumer,bStillWant)

#define IResourceManager_CancelRequest(This,idResource,pConsumer)	\
    (This)->lpVtbl -> CancelRequest(This,idResource,pConsumer)

#define IResourceManager_SetFocus(This,pFocusObject)	\
    (This)->lpVtbl -> SetFocus(This,pFocusObject)

#define IResourceManager_ReleaseFocus(This,pFocusObject)	\
    (This)->lpVtbl -> ReleaseFocus(This,pFocusObject)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IResourceManager_Register_Proxy( 
    IResourceManager __RPC_FAR * This,
    /* [in] */ LPCWSTR pName,
    /* [in] */ LONG cResource,
    /* [out] */ LONG __RPC_FAR *plToken);


void __RPC_STUB IResourceManager_Register_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IResourceManager_RegisterGroup_Proxy( 
    IResourceManager __RPC_FAR * This,
    /* [in] */ LPCWSTR pName,
    /* [in] */ LONG cResource,
    /* [size_is][in] */ LONG __RPC_FAR *palTokens,
    /* [out] */ LONG __RPC_FAR *plToken);


void __RPC_STUB IResourceManager_RegisterGroup_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IResourceManager_RequestResource_Proxy( 
    IResourceManager __RPC_FAR * This,
    /* [in] */ LONG idResource,
    /* [in] */ IUnknown __RPC_FAR *pFocusObject,
    /* [in] */ IResourceConsumer __RPC_FAR *pConsumer);


void __RPC_STUB IResourceManager_RequestResource_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IResourceManager_NotifyAcquire_Proxy( 
    IResourceManager __RPC_FAR * This,
    /* [in] */ LONG idResource,
    /* [in] */ IResourceConsumer __RPC_FAR *pConsumer,
    /* [in] */ HRESULT hr);


void __RPC_STUB IResourceManager_NotifyAcquire_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IResourceManager_NotifyRelease_Proxy( 
    IResourceManager __RPC_FAR * This,
    /* [in] */ LONG idResource,
    /* [in] */ IResourceConsumer __RPC_FAR *pConsumer,
    /* [in] */ BOOL bStillWant);


void __RPC_STUB IResourceManager_NotifyRelease_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IResourceManager_CancelRequest_Proxy( 
    IResourceManager __RPC_FAR * This,
    /* [in] */ LONG idResource,
    /* [in] */ IResourceConsumer __RPC_FAR *pConsumer);


void __RPC_STUB IResourceManager_CancelRequest_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IResourceManager_SetFocus_Proxy( 
    IResourceManager __RPC_FAR * This,
    /* [in] */ IUnknown __RPC_FAR *pFocusObject);


void __RPC_STUB IResourceManager_SetFocus_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IResourceManager_ReleaseFocus_Proxy( 
    IResourceManager __RPC_FAR * This,
    /* [in] */ IUnknown __RPC_FAR *pFocusObject);


void __RPC_STUB IResourceManager_ReleaseFocus_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IResourceManager_INTERFACE_DEFINED__ */


#ifndef __IDistributorNotify_INTERFACE_DEFINED__
#define __IDistributorNotify_INTERFACE_DEFINED__

/* interface IDistributorNotify */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IDistributorNotify;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("56a868af-0ad4-11ce-b03a-0020af0ba770")
    IDistributorNotify : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Stop( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Pause( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Run( 
            REFERENCE_TIME tStart) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetSyncSource( 
            /* [in] */ IReferenceClock __RPC_FAR *pClock) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE NotifyGraphChange( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDistributorNotifyVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IDistributorNotify __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IDistributorNotify __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IDistributorNotify __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Stop )( 
            IDistributorNotify __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Pause )( 
            IDistributorNotify __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Run )( 
            IDistributorNotify __RPC_FAR * This,
            REFERENCE_TIME tStart);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetSyncSource )( 
            IDistributorNotify __RPC_FAR * This,
            /* [in] */ IReferenceClock __RPC_FAR *pClock);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *NotifyGraphChange )( 
            IDistributorNotify __RPC_FAR * This);
        
        END_INTERFACE
    } IDistributorNotifyVtbl;

    interface IDistributorNotify
    {
        CONST_VTBL struct IDistributorNotifyVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDistributorNotify_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDistributorNotify_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDistributorNotify_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDistributorNotify_Stop(This)	\
    (This)->lpVtbl -> Stop(This)

#define IDistributorNotify_Pause(This)	\
    (This)->lpVtbl -> Pause(This)

#define IDistributorNotify_Run(This,tStart)	\
    (This)->lpVtbl -> Run(This,tStart)

#define IDistributorNotify_SetSyncSource(This,pClock)	\
    (This)->lpVtbl -> SetSyncSource(This,pClock)

#define IDistributorNotify_NotifyGraphChange(This)	\
    (This)->lpVtbl -> NotifyGraphChange(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IDistributorNotify_Stop_Proxy( 
    IDistributorNotify __RPC_FAR * This);


void __RPC_STUB IDistributorNotify_Stop_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDistributorNotify_Pause_Proxy( 
    IDistributorNotify __RPC_FAR * This);


void __RPC_STUB IDistributorNotify_Pause_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDistributorNotify_Run_Proxy( 
    IDistributorNotify __RPC_FAR * This,
    REFERENCE_TIME tStart);


void __RPC_STUB IDistributorNotify_Run_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDistributorNotify_SetSyncSource_Proxy( 
    IDistributorNotify __RPC_FAR * This,
    /* [in] */ IReferenceClock __RPC_FAR *pClock);


void __RPC_STUB IDistributorNotify_SetSyncSource_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDistributorNotify_NotifyGraphChange_Proxy( 
    IDistributorNotify __RPC_FAR * This);


void __RPC_STUB IDistributorNotify_NotifyGraphChange_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDistributorNotify_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_strmif_0097 */
/* [local] */ 

typedef /* [public] */ 
enum __MIDL___MIDL_itf_strmif_0097_0001
    {	AM_STREAM_INFO_START_DEFINED	= 0x1,
	AM_STREAM_INFO_STOP_DEFINED	= 0x2,
	AM_STREAM_INFO_DISCARDING	= 0x4,
	AM_STREAM_INFO_STOP_SEND_EXTRA	= 0x10
    }	AM_STREAM_INFO_FLAGS;

typedef /* [public][public] */ struct __MIDL___MIDL_itf_strmif_0097_0002
    {
    REFERENCE_TIME tStart;
    REFERENCE_TIME tStop;
    DWORD dwStartCookie;
    DWORD dwStopCookie;
    DWORD dwFlags;
    }	AM_STREAM_INFO;



extern RPC_IF_HANDLE __MIDL_itf_strmif_0097_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_strmif_0097_v0_0_s_ifspec;

#ifndef __IAMStreamControl_INTERFACE_DEFINED__
#define __IAMStreamControl_INTERFACE_DEFINED__

/* interface IAMStreamControl */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IAMStreamControl;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("36b73881-c2c8-11cf-8b46-00805f6cef60")
    IAMStreamControl : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE StartAt( 
            /* [in] */ const REFERENCE_TIME __RPC_FAR *ptStart,
            /* [in] */ DWORD dwCookie) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE StopAt( 
            /* [in] */ const REFERENCE_TIME __RPC_FAR *ptStop,
            /* [in] */ BOOL bSendExtra,
            /* [in] */ DWORD dwCookie) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetInfo( 
            /* [out] */ AM_STREAM_INFO __RPC_FAR *pInfo) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAMStreamControlVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IAMStreamControl __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IAMStreamControl __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IAMStreamControl __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *StartAt )( 
            IAMStreamControl __RPC_FAR * This,
            /* [in] */ const REFERENCE_TIME __RPC_FAR *ptStart,
            /* [in] */ DWORD dwCookie);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *StopAt )( 
            IAMStreamControl __RPC_FAR * This,
            /* [in] */ const REFERENCE_TIME __RPC_FAR *ptStop,
            /* [in] */ BOOL bSendExtra,
            /* [in] */ DWORD dwCookie);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetInfo )( 
            IAMStreamControl __RPC_FAR * This,
            /* [out] */ AM_STREAM_INFO __RPC_FAR *pInfo);
        
        END_INTERFACE
    } IAMStreamControlVtbl;

    interface IAMStreamControl
    {
        CONST_VTBL struct IAMStreamControlVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAMStreamControl_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAMStreamControl_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAMStreamControl_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IAMStreamControl_StartAt(This,ptStart,dwCookie)	\
    (This)->lpVtbl -> StartAt(This,ptStart,dwCookie)

#define IAMStreamControl_StopAt(This,ptStop,bSendExtra,dwCookie)	\
    (This)->lpVtbl -> StopAt(This,ptStop,bSendExtra,dwCookie)

#define IAMStreamControl_GetInfo(This,pInfo)	\
    (This)->lpVtbl -> GetInfo(This,pInfo)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IAMStreamControl_StartAt_Proxy( 
    IAMStreamControl __RPC_FAR * This,
    /* [in] */ const REFERENCE_TIME __RPC_FAR *ptStart,
    /* [in] */ DWORD dwCookie);


void __RPC_STUB IAMStreamControl_StartAt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMStreamControl_StopAt_Proxy( 
    IAMStreamControl __RPC_FAR * This,
    /* [in] */ const REFERENCE_TIME __RPC_FAR *ptStop,
    /* [in] */ BOOL bSendExtra,
    /* [in] */ DWORD dwCookie);


void __RPC_STUB IAMStreamControl_StopAt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMStreamControl_GetInfo_Proxy( 
    IAMStreamControl __RPC_FAR * This,
    /* [out] */ AM_STREAM_INFO __RPC_FAR *pInfo);


void __RPC_STUB IAMStreamControl_GetInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAMStreamControl_INTERFACE_DEFINED__ */


#ifndef __ISeekingPassThru_INTERFACE_DEFINED__
#define __ISeekingPassThru_INTERFACE_DEFINED__

/* interface ISeekingPassThru */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_ISeekingPassThru;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("36b73883-c2c8-11cf-8b46-00805f6cef60")
    ISeekingPassThru : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Init( 
            /* [in] */ BOOL bSupportRendering,
            /* [in] */ IPin __RPC_FAR *pPin) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISeekingPassThruVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ISeekingPassThru __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ISeekingPassThru __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ISeekingPassThru __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Init )( 
            ISeekingPassThru __RPC_FAR * This,
            /* [in] */ BOOL bSupportRendering,
            /* [in] */ IPin __RPC_FAR *pPin);
        
        END_INTERFACE
    } ISeekingPassThruVtbl;

    interface ISeekingPassThru
    {
        CONST_VTBL struct ISeekingPassThruVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISeekingPassThru_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ISeekingPassThru_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ISeekingPassThru_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ISeekingPassThru_Init(This,bSupportRendering,pPin)	\
    (This)->lpVtbl -> Init(This,bSupportRendering,pPin)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE ISeekingPassThru_Init_Proxy( 
    ISeekingPassThru __RPC_FAR * This,
    /* [in] */ BOOL bSupportRendering,
    /* [in] */ IPin __RPC_FAR *pPin);


void __RPC_STUB ISeekingPassThru_Init_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ISeekingPassThru_INTERFACE_DEFINED__ */


#ifndef __IAMStreamConfig_INTERFACE_DEFINED__
#define __IAMStreamConfig_INTERFACE_DEFINED__

/* interface IAMStreamConfig */
/* [unique][uuid][object] */ 

typedef struct _VIDEO_STREAM_CONFIG_CAPS
    {
    GUID guid;
    ULONG VideoStandard;
    SIZE InputSize;
    SIZE MinCroppingSize;
    SIZE MaxCroppingSize;
    int CropGranularityX;
    int CropGranularityY;
    int CropAlignX;
    int CropAlignY;
    SIZE MinOutputSize;
    SIZE MaxOutputSize;
    int OutputGranularityX;
    int OutputGranularityY;
    int StretchTapsX;
    int StretchTapsY;
    int ShrinkTapsX;
    int ShrinkTapsY;
    LONGLONG MinFrameInterval;
    LONGLONG MaxFrameInterval;
    LONG MinBitsPerSecond;
    LONG MaxBitsPerSecond;
    }	VIDEO_STREAM_CONFIG_CAPS;

typedef struct _AUDIO_STREAM_CONFIG_CAPS
    {
    GUID guid;
    ULONG MinimumChannels;
    ULONG MaximumChannels;
    ULONG ChannelsGranularity;
    ULONG MinimumBitsPerSample;
    ULONG MaximumBitsPerSample;
    ULONG BitsPerSampleGranularity;
    ULONG MinimumSampleFrequency;
    ULONG MaximumSampleFrequency;
    ULONG SampleFrequencyGranularity;
    }	AUDIO_STREAM_CONFIG_CAPS;


EXTERN_C const IID IID_IAMStreamConfig;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("C6E13340-30AC-11d0-A18C-00A0C9118956")
    IAMStreamConfig : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SetFormat( 
            /* [in] */ AM_MEDIA_TYPE __RPC_FAR *pmt) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetFormat( 
            /* [out] */ AM_MEDIA_TYPE __RPC_FAR *__RPC_FAR *ppmt) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetNumberOfCapabilities( 
            /* [out] */ int __RPC_FAR *piCount,
            /* [out] */ int __RPC_FAR *piSize) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetStreamCaps( 
            /* [in] */ int iIndex,
            /* [out] */ AM_MEDIA_TYPE __RPC_FAR *__RPC_FAR *ppmt,
            /* [out] */ BYTE __RPC_FAR *pSCC) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAMStreamConfigVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IAMStreamConfig __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IAMStreamConfig __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IAMStreamConfig __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetFormat )( 
            IAMStreamConfig __RPC_FAR * This,
            /* [in] */ AM_MEDIA_TYPE __RPC_FAR *pmt);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetFormat )( 
            IAMStreamConfig __RPC_FAR * This,
            /* [out] */ AM_MEDIA_TYPE __RPC_FAR *__RPC_FAR *ppmt);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNumberOfCapabilities )( 
            IAMStreamConfig __RPC_FAR * This,
            /* [out] */ int __RPC_FAR *piCount,
            /* [out] */ int __RPC_FAR *piSize);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetStreamCaps )( 
            IAMStreamConfig __RPC_FAR * This,
            /* [in] */ int iIndex,
            /* [out] */ AM_MEDIA_TYPE __RPC_FAR *__RPC_FAR *ppmt,
            /* [out] */ BYTE __RPC_FAR *pSCC);
        
        END_INTERFACE
    } IAMStreamConfigVtbl;

    interface IAMStreamConfig
    {
        CONST_VTBL struct IAMStreamConfigVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAMStreamConfig_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAMStreamConfig_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAMStreamConfig_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IAMStreamConfig_SetFormat(This,pmt)	\
    (This)->lpVtbl -> SetFormat(This,pmt)

#define IAMStreamConfig_GetFormat(This,ppmt)	\
    (This)->lpVtbl -> GetFormat(This,ppmt)

#define IAMStreamConfig_GetNumberOfCapabilities(This,piCount,piSize)	\
    (This)->lpVtbl -> GetNumberOfCapabilities(This,piCount,piSize)

#define IAMStreamConfig_GetStreamCaps(This,iIndex,ppmt,pSCC)	\
    (This)->lpVtbl -> GetStreamCaps(This,iIndex,ppmt,pSCC)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IAMStreamConfig_SetFormat_Proxy( 
    IAMStreamConfig __RPC_FAR * This,
    /* [in] */ AM_MEDIA_TYPE __RPC_FAR *pmt);


void __RPC_STUB IAMStreamConfig_SetFormat_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMStreamConfig_GetFormat_Proxy( 
    IAMStreamConfig __RPC_FAR * This,
    /* [out] */ AM_MEDIA_TYPE __RPC_FAR *__RPC_FAR *ppmt);


void __RPC_STUB IAMStreamConfig_GetFormat_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMStreamConfig_GetNumberOfCapabilities_Proxy( 
    IAMStreamConfig __RPC_FAR * This,
    /* [out] */ int __RPC_FAR *piCount,
    /* [out] */ int __RPC_FAR *piSize);


void __RPC_STUB IAMStreamConfig_GetNumberOfCapabilities_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMStreamConfig_GetStreamCaps_Proxy( 
    IAMStreamConfig __RPC_FAR * This,
    /* [in] */ int iIndex,
    /* [out] */ AM_MEDIA_TYPE __RPC_FAR *__RPC_FAR *ppmt,
    /* [out] */ BYTE __RPC_FAR *pSCC);


void __RPC_STUB IAMStreamConfig_GetStreamCaps_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAMStreamConfig_INTERFACE_DEFINED__ */


#ifndef __IConfigInterleaving_INTERFACE_DEFINED__
#define __IConfigInterleaving_INTERFACE_DEFINED__

/* interface IConfigInterleaving */
/* [unique][uuid][object] */ 

typedef /* [public][public][public] */ 
enum __MIDL_IConfigInterleaving_0001
    {	INTERLEAVE_NONE	= 0,
	INTERLEAVE_CAPTURE	= INTERLEAVE_NONE + 1,
	INTERLEAVE_FULL	= INTERLEAVE_CAPTURE + 1
    }	InterleavingMode;


EXTERN_C const IID IID_IConfigInterleaving;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("BEE3D220-157B-11d0-BD23-00A0C911CE86")
    IConfigInterleaving : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE put_Mode( 
            /* [in] */ InterleavingMode mode) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get_Mode( 
            /* [out] */ InterleavingMode __RPC_FAR *pMode) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE put_Interleaving( 
            /* [in] */ const REFERENCE_TIME __RPC_FAR *prtInterleave,
            /* [in] */ const REFERENCE_TIME __RPC_FAR *prtPreroll) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get_Interleaving( 
            /* [out] */ REFERENCE_TIME __RPC_FAR *prtInterleave,
            /* [out] */ REFERENCE_TIME __RPC_FAR *prtPreroll) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IConfigInterleavingVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IConfigInterleaving __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IConfigInterleaving __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IConfigInterleaving __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Mode )( 
            IConfigInterleaving __RPC_FAR * This,
            /* [in] */ InterleavingMode mode);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Mode )( 
            IConfigInterleaving __RPC_FAR * This,
            /* [out] */ InterleavingMode __RPC_FAR *pMode);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Interleaving )( 
            IConfigInterleaving __RPC_FAR * This,
            /* [in] */ const REFERENCE_TIME __RPC_FAR *prtInterleave,
            /* [in] */ const REFERENCE_TIME __RPC_FAR *prtPreroll);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Interleaving )( 
            IConfigInterleaving __RPC_FAR * This,
            /* [out] */ REFERENCE_TIME __RPC_FAR *prtInterleave,
            /* [out] */ REFERENCE_TIME __RPC_FAR *prtPreroll);
        
        END_INTERFACE
    } IConfigInterleavingVtbl;

    interface IConfigInterleaving
    {
        CONST_VTBL struct IConfigInterleavingVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IConfigInterleaving_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IConfigInterleaving_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IConfigInterleaving_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IConfigInterleaving_put_Mode(This,mode)	\
    (This)->lpVtbl -> put_Mode(This,mode)

#define IConfigInterleaving_get_Mode(This,pMode)	\
    (This)->lpVtbl -> get_Mode(This,pMode)

#define IConfigInterleaving_put_Interleaving(This,prtInterleave,prtPreroll)	\
    (This)->lpVtbl -> put_Interleaving(This,prtInterleave,prtPreroll)

#define IConfigInterleaving_get_Interleaving(This,prtInterleave,prtPreroll)	\
    (This)->lpVtbl -> get_Interleaving(This,prtInterleave,prtPreroll)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IConfigInterleaving_put_Mode_Proxy( 
    IConfigInterleaving __RPC_FAR * This,
    /* [in] */ InterleavingMode mode);


void __RPC_STUB IConfigInterleaving_put_Mode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigInterleaving_get_Mode_Proxy( 
    IConfigInterleaving __RPC_FAR * This,
    /* [out] */ InterleavingMode __RPC_FAR *pMode);


void __RPC_STUB IConfigInterleaving_get_Mode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigInterleaving_put_Interleaving_Proxy( 
    IConfigInterleaving __RPC_FAR * This,
    /* [in] */ const REFERENCE_TIME __RPC_FAR *prtInterleave,
    /* [in] */ const REFERENCE_TIME __RPC_FAR *prtPreroll);


void __RPC_STUB IConfigInterleaving_put_Interleaving_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigInterleaving_get_Interleaving_Proxy( 
    IConfigInterleaving __RPC_FAR * This,
    /* [out] */ REFERENCE_TIME __RPC_FAR *prtInterleave,
    /* [out] */ REFERENCE_TIME __RPC_FAR *prtPreroll);


void __RPC_STUB IConfigInterleaving_get_Interleaving_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IConfigInterleaving_INTERFACE_DEFINED__ */


#ifndef __IConfigAviMux_INTERFACE_DEFINED__
#define __IConfigAviMux_INTERFACE_DEFINED__

/* interface IConfigAviMux */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IConfigAviMux;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("5ACD6AA0-F482-11ce-8B67-00AA00A3F1A6")
    IConfigAviMux : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SetMasterStream( 
            /* [in] */ LONG iStream) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetMasterStream( 
            /* [out] */ LONG __RPC_FAR *pStream) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetOutputCompatibilityIndex( 
            /* [in] */ BOOL fOldIndex) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetOutputCompatibilityIndex( 
            /* [out] */ BOOL __RPC_FAR *pfOldIndex) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IConfigAviMuxVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IConfigAviMux __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IConfigAviMux __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IConfigAviMux __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetMasterStream )( 
            IConfigAviMux __RPC_FAR * This,
            /* [in] */ LONG iStream);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMasterStream )( 
            IConfigAviMux __RPC_FAR * This,
            /* [out] */ LONG __RPC_FAR *pStream);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetOutputCompatibilityIndex )( 
            IConfigAviMux __RPC_FAR * This,
            /* [in] */ BOOL fOldIndex);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetOutputCompatibilityIndex )( 
            IConfigAviMux __RPC_FAR * This,
            /* [out] */ BOOL __RPC_FAR *pfOldIndex);
        
        END_INTERFACE
    } IConfigAviMuxVtbl;

    interface IConfigAviMux
    {
        CONST_VTBL struct IConfigAviMuxVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IConfigAviMux_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IConfigAviMux_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IConfigAviMux_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IConfigAviMux_SetMasterStream(This,iStream)	\
    (This)->lpVtbl -> SetMasterStream(This,iStream)

#define IConfigAviMux_GetMasterStream(This,pStream)	\
    (This)->lpVtbl -> GetMasterStream(This,pStream)

#define IConfigAviMux_SetOutputCompatibilityIndex(This,fOldIndex)	\
    (This)->lpVtbl -> SetOutputCompatibilityIndex(This,fOldIndex)

#define IConfigAviMux_GetOutputCompatibilityIndex(This,pfOldIndex)	\
    (This)->lpVtbl -> GetOutputCompatibilityIndex(This,pfOldIndex)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IConfigAviMux_SetMasterStream_Proxy( 
    IConfigAviMux __RPC_FAR * This,
    /* [in] */ LONG iStream);


void __RPC_STUB IConfigAviMux_SetMasterStream_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigAviMux_GetMasterStream_Proxy( 
    IConfigAviMux __RPC_FAR * This,
    /* [out] */ LONG __RPC_FAR *pStream);


void __RPC_STUB IConfigAviMux_GetMasterStream_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigAviMux_SetOutputCompatibilityIndex_Proxy( 
    IConfigAviMux __RPC_FAR * This,
    /* [in] */ BOOL fOldIndex);


void __RPC_STUB IConfigAviMux_SetOutputCompatibilityIndex_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigAviMux_GetOutputCompatibilityIndex_Proxy( 
    IConfigAviMux __RPC_FAR * This,
    /* [out] */ BOOL __RPC_FAR *pfOldIndex);


void __RPC_STUB IConfigAviMux_GetOutputCompatibilityIndex_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IConfigAviMux_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_strmif_0104 */
/* [local] */ 

typedef /* [public] */ 
enum __MIDL___MIDL_itf_strmif_0104_0001
    {	CompressionCaps_CanQuality	= 0x1,
	CompressionCaps_CanCrunch	= 0x2,
	CompressionCaps_CanKeyFrame	= 0x4,
	CompressionCaps_CanBFrame	= 0x8,
	CompressionCaps_CanWindow	= 0x10
    }	CompressionCaps;



extern RPC_IF_HANDLE __MIDL_itf_strmif_0104_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_strmif_0104_v0_0_s_ifspec;

#ifndef __IAMVideoCompression_INTERFACE_DEFINED__
#define __IAMVideoCompression_INTERFACE_DEFINED__

/* interface IAMVideoCompression */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IAMVideoCompression;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("C6E13343-30AC-11d0-A18C-00A0C9118956")
    IAMVideoCompression : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE put_KeyFrameRate( 
            /* [in] */ long KeyFrameRate) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get_KeyFrameRate( 
            /* [out] */ long __RPC_FAR *pKeyFrameRate) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE put_PFramesPerKeyFrame( 
            /* [in] */ long PFramesPerKeyFrame) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get_PFramesPerKeyFrame( 
            /* [out] */ long __RPC_FAR *pPFramesPerKeyFrame) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE put_Quality( 
            /* [in] */ double Quality) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get_Quality( 
            /* [out] */ double __RPC_FAR *pQuality) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE put_WindowSize( 
            /* [in] */ DWORDLONG WindowSize) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get_WindowSize( 
            /* [out] */ DWORDLONG __RPC_FAR *pWindowSize) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetInfo( 
            /* [size_is][out] */ WCHAR __RPC_FAR *pszVersion,
            /* [out][in] */ int __RPC_FAR *pcbVersion,
            /* [size_is][out] */ LPWSTR pszDescription,
            /* [out][in] */ int __RPC_FAR *pcbDescription,
            /* [out] */ long __RPC_FAR *pDefaultKeyFrameRate,
            /* [out] */ long __RPC_FAR *pDefaultPFramesPerKey,
            /* [out] */ double __RPC_FAR *pDefaultQuality,
            /* [out] */ long __RPC_FAR *pCapabilities) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OverrideKeyFrame( 
            /* [in] */ long FrameNumber) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OverrideFrameSize( 
            /* [in] */ long FrameNumber,
            /* [in] */ long Size) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAMVideoCompressionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IAMVideoCompression __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IAMVideoCompression __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IAMVideoCompression __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_KeyFrameRate )( 
            IAMVideoCompression __RPC_FAR * This,
            /* [in] */ long KeyFrameRate);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_KeyFrameRate )( 
            IAMVideoCompression __RPC_FAR * This,
            /* [out] */ long __RPC_FAR *pKeyFrameRate);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_PFramesPerKeyFrame )( 
            IAMVideoCompression __RPC_FAR * This,
            /* [in] */ long PFramesPerKeyFrame);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_PFramesPerKeyFrame )( 
            IAMVideoCompression __RPC_FAR * This,
            /* [out] */ long __RPC_FAR *pPFramesPerKeyFrame);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Quality )( 
            IAMVideoCompression __RPC_FAR * This,
            /* [in] */ double Quality);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Quality )( 
            IAMVideoCompression __RPC_FAR * This,
            /* [out] */ double __RPC_FAR *pQuality);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_WindowSize )( 
            IAMVideoCompression __RPC_FAR * This,
            /* [in] */ DWORDLONG WindowSize);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_WindowSize )( 
            IAMVideoCompression __RPC_FAR * This,
            /* [out] */ DWORDLONG __RPC_FAR *pWindowSize);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetInfo )( 
            IAMVideoCompression __RPC_FAR * This,
            /* [size_is][out] */ WCHAR __RPC_FAR *pszVersion,
            /* [out][in] */ int __RPC_FAR *pcbVersion,
            /* [size_is][out] */ LPWSTR pszDescription,
            /* [out][in] */ int __RPC_FAR *pcbDescription,
            /* [out] */ long __RPC_FAR *pDefaultKeyFrameRate,
            /* [out] */ long __RPC_FAR *pDefaultPFramesPerKey,
            /* [out] */ double __RPC_FAR *pDefaultQuality,
            /* [out] */ long __RPC_FAR *pCapabilities);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OverrideKeyFrame )( 
            IAMVideoCompression __RPC_FAR * This,
            /* [in] */ long FrameNumber);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OverrideFrameSize )( 
            IAMVideoCompression __RPC_FAR * This,
            /* [in] */ long FrameNumber,
            /* [in] */ long Size);
        
        END_INTERFACE
    } IAMVideoCompressionVtbl;

    interface IAMVideoCompression
    {
        CONST_VTBL struct IAMVideoCompressionVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAMVideoCompression_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAMVideoCompression_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAMVideoCompression_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IAMVideoCompression_put_KeyFrameRate(This,KeyFrameRate)	\
    (This)->lpVtbl -> put_KeyFrameRate(This,KeyFrameRate)

#define IAMVideoCompression_get_KeyFrameRate(This,pKeyFrameRate)	\
    (This)->lpVtbl -> get_KeyFrameRate(This,pKeyFrameRate)

#define IAMVideoCompression_put_PFramesPerKeyFrame(This,PFramesPerKeyFrame)	\
    (This)->lpVtbl -> put_PFramesPerKeyFrame(This,PFramesPerKeyFrame)

#define IAMVideoCompression_get_PFramesPerKeyFrame(This,pPFramesPerKeyFrame)	\
    (This)->lpVtbl -> get_PFramesPerKeyFrame(This,pPFramesPerKeyFrame)

#define IAMVideoCompression_put_Quality(This,Quality)	\
    (This)->lpVtbl -> put_Quality(This,Quality)

#define IAMVideoCompression_get_Quality(This,pQuality)	\
    (This)->lpVtbl -> get_Quality(This,pQuality)

#define IAMVideoCompression_put_WindowSize(This,WindowSize)	\
    (This)->lpVtbl -> put_WindowSize(This,WindowSize)

#define IAMVideoCompression_get_WindowSize(This,pWindowSize)	\
    (This)->lpVtbl -> get_WindowSize(This,pWindowSize)

#define IAMVideoCompression_GetInfo(This,pszVersion,pcbVersion,pszDescription,pcbDescription,pDefaultKeyFrameRate,pDefaultPFramesPerKey,pDefaultQuality,pCapabilities)	\
    (This)->lpVtbl -> GetInfo(This,pszVersion,pcbVersion,pszDescription,pcbDescription,pDefaultKeyFrameRate,pDefaultPFramesPerKey,pDefaultQuality,pCapabilities)

#define IAMVideoCompression_OverrideKeyFrame(This,FrameNumber)	\
    (This)->lpVtbl -> OverrideKeyFrame(This,FrameNumber)

#define IAMVideoCompression_OverrideFrameSize(This,FrameNumber,Size)	\
    (This)->lpVtbl -> OverrideFrameSize(This,FrameNumber,Size)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IAMVideoCompression_put_KeyFrameRate_Proxy( 
    IAMVideoCompression __RPC_FAR * This,
    /* [in] */ long KeyFrameRate);


void __RPC_STUB IAMVideoCompression_put_KeyFrameRate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMVideoCompression_get_KeyFrameRate_Proxy( 
    IAMVideoCompression __RPC_FAR * This,
    /* [out] */ long __RPC_FAR *pKeyFrameRate);


void __RPC_STUB IAMVideoCompression_get_KeyFrameRate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMVideoCompression_put_PFramesPerKeyFrame_Proxy( 
    IAMVideoCompression __RPC_FAR * This,
    /* [in] */ long PFramesPerKeyFrame);


void __RPC_STUB IAMVideoCompression_put_PFramesPerKeyFrame_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMVideoCompression_get_PFramesPerKeyFrame_Proxy( 
    IAMVideoCompression __RPC_FAR * This,
    /* [out] */ long __RPC_FAR *pPFramesPerKeyFrame);


void __RPC_STUB IAMVideoCompression_get_PFramesPerKeyFrame_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMVideoCompression_put_Quality_Proxy( 
    IAMVideoCompression __RPC_FAR * This,
    /* [in] */ double Quality);


void __RPC_STUB IAMVideoCompression_put_Quality_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMVideoCompression_get_Quality_Proxy( 
    IAMVideoCompression __RPC_FAR * This,
    /* [out] */ double __RPC_FAR *pQuality);


void __RPC_STUB IAMVideoCompression_get_Quality_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMVideoCompression_put_WindowSize_Proxy( 
    IAMVideoCompression __RPC_FAR * This,
    /* [in] */ DWORDLONG WindowSize);


void __RPC_STUB IAMVideoCompression_put_WindowSize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMVideoCompression_get_WindowSize_Proxy( 
    IAMVideoCompression __RPC_FAR * This,
    /* [out] */ DWORDLONG __RPC_FAR *pWindowSize);


void __RPC_STUB IAMVideoCompression_get_WindowSize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMVideoCompression_GetInfo_Proxy( 
    IAMVideoCompression __RPC_FAR * This,
    /* [size_is][out] */ WCHAR __RPC_FAR *pszVersion,
    /* [out][in] */ int __RPC_FAR *pcbVersion,
    /* [size_is][out] */ LPWSTR pszDescription,
    /* [out][in] */ int __RPC_FAR *pcbDescription,
    /* [out] */ long __RPC_FAR *pDefaultKeyFrameRate,
    /* [out] */ long __RPC_FAR *pDefaultPFramesPerKey,
    /* [out] */ double __RPC_FAR *pDefaultQuality,
    /* [out] */ long __RPC_FAR *pCapabilities);


void __RPC_STUB IAMVideoCompression_GetInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMVideoCompression_OverrideKeyFrame_Proxy( 
    IAMVideoCompression __RPC_FAR * This,
    /* [in] */ long FrameNumber);


void __RPC_STUB IAMVideoCompression_OverrideKeyFrame_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMVideoCompression_OverrideFrameSize_Proxy( 
    IAMVideoCompression __RPC_FAR * This,
    /* [in] */ long FrameNumber,
    /* [in] */ long Size);


void __RPC_STUB IAMVideoCompression_OverrideFrameSize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAMVideoCompression_INTERFACE_DEFINED__ */


#ifndef __IAMDroppedFrames_INTERFACE_DEFINED__
#define __IAMDroppedFrames_INTERFACE_DEFINED__

/* interface IAMDroppedFrames */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IAMDroppedFrames;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("C6E13344-30AC-11d0-A18C-00A0C9118956")
    IAMDroppedFrames : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetNumDropped( 
            /* [out] */ long __RPC_FAR *plDropped) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetNumNotDropped( 
            /* [out] */ long __RPC_FAR *plNotDropped) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetDroppedInfo( 
            /* [in] */ long lSize,
            /* [out] */ long __RPC_FAR *plArray,
            /* [out] */ long __RPC_FAR *plNumCopied) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetAverageFrameSize( 
            /* [out] */ long __RPC_FAR *plAverageSize) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAMDroppedFramesVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IAMDroppedFrames __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IAMDroppedFrames __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IAMDroppedFrames __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNumDropped )( 
            IAMDroppedFrames __RPC_FAR * This,
            /* [out] */ long __RPC_FAR *plDropped);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNumNotDropped )( 
            IAMDroppedFrames __RPC_FAR * This,
            /* [out] */ long __RPC_FAR *plNotDropped);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDroppedInfo )( 
            IAMDroppedFrames __RPC_FAR * This,
            /* [in] */ long lSize,
            /* [out] */ long __RPC_FAR *plArray,
            /* [out] */ long __RPC_FAR *plNumCopied);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetAverageFrameSize )( 
            IAMDroppedFrames __RPC_FAR * This,
            /* [out] */ long __RPC_FAR *plAverageSize);
        
        END_INTERFACE
    } IAMDroppedFramesVtbl;

    interface IAMDroppedFrames
    {
        CONST_VTBL struct IAMDroppedFramesVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAMDroppedFrames_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAMDroppedFrames_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAMDroppedFrames_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IAMDroppedFrames_GetNumDropped(This,plDropped)	\
    (This)->lpVtbl -> GetNumDropped(This,plDropped)

#define IAMDroppedFrames_GetNumNotDropped(This,plNotDropped)	\
    (This)->lpVtbl -> GetNumNotDropped(This,plNotDropped)

#define IAMDroppedFrames_GetDroppedInfo(This,lSize,plArray,plNumCopied)	\
    (This)->lpVtbl -> GetDroppedInfo(This,lSize,plArray,plNumCopied)

#define IAMDroppedFrames_GetAverageFrameSize(This,plAverageSize)	\
    (This)->lpVtbl -> GetAverageFrameSize(This,plAverageSize)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IAMDroppedFrames_GetNumDropped_Proxy( 
    IAMDroppedFrames __RPC_FAR * This,
    /* [out] */ long __RPC_FAR *plDropped);


void __RPC_STUB IAMDroppedFrames_GetNumDropped_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMDroppedFrames_GetNumNotDropped_Proxy( 
    IAMDroppedFrames __RPC_FAR * This,
    /* [out] */ long __RPC_FAR *plNotDropped);


void __RPC_STUB IAMDroppedFrames_GetNumNotDropped_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMDroppedFrames_GetDroppedInfo_Proxy( 
    IAMDroppedFrames __RPC_FAR * This,
    /* [in] */ long lSize,
    /* [out] */ long __RPC_FAR *plArray,
    /* [out] */ long __RPC_FAR *plNumCopied);


void __RPC_STUB IAMDroppedFrames_GetDroppedInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMDroppedFrames_GetAverageFrameSize_Proxy( 
    IAMDroppedFrames __RPC_FAR * This,
    /* [out] */ long __RPC_FAR *plAverageSize);


void __RPC_STUB IAMDroppedFrames_GetAverageFrameSize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAMDroppedFrames_INTERFACE_DEFINED__ */


#ifndef __IAMBufferNegotiation_INTERFACE_DEFINED__
#define __IAMBufferNegotiation_INTERFACE_DEFINED__

/* interface IAMBufferNegotiation */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IAMBufferNegotiation;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("56ED71A0-AF5F-11D0-B3F0-00AA003761C5")
    IAMBufferNegotiation : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SuggestAllocatorProperties( 
            /* [in] */ const ALLOCATOR_PROPERTIES __RPC_FAR *pprop) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetAllocatorProperties( 
            /* [out] */ ALLOCATOR_PROPERTIES __RPC_FAR *pprop) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAMBufferNegotiationVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IAMBufferNegotiation __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IAMBufferNegotiation __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IAMBufferNegotiation __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SuggestAllocatorProperties )( 
            IAMBufferNegotiation __RPC_FAR * This,
            /* [in] */ const ALLOCATOR_PROPERTIES __RPC_FAR *pprop);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetAllocatorProperties )( 
            IAMBufferNegotiation __RPC_FAR * This,
            /* [out] */ ALLOCATOR_PROPERTIES __RPC_FAR *pprop);
        
        END_INTERFACE
    } IAMBufferNegotiationVtbl;

    interface IAMBufferNegotiation
    {
        CONST_VTBL struct IAMBufferNegotiationVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAMBufferNegotiation_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAMBufferNegotiation_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAMBufferNegotiation_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IAMBufferNegotiation_SuggestAllocatorProperties(This,pprop)	\
    (This)->lpVtbl -> SuggestAllocatorProperties(This,pprop)

#define IAMBufferNegotiation_GetAllocatorProperties(This,pprop)	\
    (This)->lpVtbl -> GetAllocatorProperties(This,pprop)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IAMBufferNegotiation_SuggestAllocatorProperties_Proxy( 
    IAMBufferNegotiation __RPC_FAR * This,
    /* [in] */ const ALLOCATOR_PROPERTIES __RPC_FAR *pprop);


void __RPC_STUB IAMBufferNegotiation_SuggestAllocatorProperties_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMBufferNegotiation_GetAllocatorProperties_Proxy( 
    IAMBufferNegotiation __RPC_FAR * This,
    /* [out] */ ALLOCATOR_PROPERTIES __RPC_FAR *pprop);


void __RPC_STUB IAMBufferNegotiation_GetAllocatorProperties_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAMBufferNegotiation_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_strmif_0107 */
/* [local] */ 

typedef 
enum tagAnalogVideoStandard
    {	AnalogVideo_None	= 0,
	AnalogVideo_NTSC_M	= 0x1,
	AnalogVideo_NTSC_M_J	= 0x2,
	AnalogVideo_NTSC_433	= 0x4,
	AnalogVideo_PAL_B	= 0x10,
	AnalogVideo_PAL_D	= 0x20,
	AnalogVideo_PAL_G	= 0x40,
	AnalogVideo_PAL_H	= 0x80,
	AnalogVideo_PAL_I	= 0x100,
	AnalogVideo_PAL_M	= 0x200,
	AnalogVideo_PAL_N	= 0x400,
	AnalogVideo_PAL_60	= 0x800,
	AnalogVideo_SECAM_B	= 0x1000,
	AnalogVideo_SECAM_D	= 0x2000,
	AnalogVideo_SECAM_G	= 0x4000,
	AnalogVideo_SECAM_H	= 0x8000,
	AnalogVideo_SECAM_K	= 0x10000,
	AnalogVideo_SECAM_K1	= 0x20000,
	AnalogVideo_SECAM_L	= 0x40000,
	AnalogVideo_SECAM_L1	= 0x80000
    }	AnalogVideoStandard;

#define AnalogVideo_NTSC_Mask  0x00000007
#define AnalogVideo_PAL_Mask   0x00000FF0
#define AnalogVideo_SECAM_Mask 0x000FF000
typedef 
enum tagTunerInputType
    {	TunerInputCable	= 0,
	TunerInputAntenna	= TunerInputCable + 1
    }	TunerInputType;

typedef /* [public] */ 
enum __MIDL___MIDL_itf_strmif_0107_0001
    {	VideoCopyProtectionMacrovisionBasic	= 0,
	VideoCopyProtectionMacrovisionCBI	= VideoCopyProtectionMacrovisionBasic + 1
    }	VideoCopyProtectionType;

typedef 
enum tagPhysicalConnectorType
    {	PhysConn_Video_Tuner	= 1,
	PhysConn_Video_Composite	= PhysConn_Video_Tuner + 1,
	PhysConn_Video_SVideo	= PhysConn_Video_Composite + 1,
	PhysConn_Video_RGB	= PhysConn_Video_SVideo + 1,
	PhysConn_Video_YRYBY	= PhysConn_Video_RGB + 1,
	PhysConn_Video_SerialDigital	= PhysConn_Video_YRYBY + 1,
	PhysConn_Video_ParallelDigital	= PhysConn_Video_SerialDigital + 1,
	PhysConn_Video_SCSI	= PhysConn_Video_ParallelDigital + 1,
	PhysConn_Video_AUX	= PhysConn_Video_SCSI + 1,
	PhysConn_Video_1394	= PhysConn_Video_AUX + 1,
	PhysConn_Video_USB	= PhysConn_Video_1394 + 1,
	PhysConn_Video_VideoDecoder	= PhysConn_Video_USB + 1,
	PhysConn_Video_VideoEncoder	= PhysConn_Video_VideoDecoder + 1,
	PhysConn_Video_SCART	= PhysConn_Video_VideoEncoder + 1,
	PhysConn_Video_Black	= PhysConn_Video_SCART + 1,
	PhysConn_Audio_Tuner	= 0x1000,
	PhysConn_Audio_Line	= PhysConn_Audio_Tuner + 1,
	PhysConn_Audio_Mic	= PhysConn_Audio_Line + 1,
	PhysConn_Audio_AESDigital	= PhysConn_Audio_Mic + 1,
	PhysConn_Audio_SPDIFDigital	= PhysConn_Audio_AESDigital + 1,
	PhysConn_Audio_SCSI	= PhysConn_Audio_SPDIFDigital + 1,
	PhysConn_Audio_AUX	= PhysConn_Audio_SCSI + 1,
	PhysConn_Audio_1394	= PhysConn_Audio_AUX + 1,
	PhysConn_Audio_USB	= PhysConn_Audio_1394 + 1,
	PhysConn_Audio_AudioDecoder	= PhysConn_Audio_USB + 1
    }	PhysicalConnectorType;

typedef 
enum tagVideoProcAmpProperty
    {	VideoProcAmp_Brightness	= 0,
	VideoProcAmp_Contrast	= VideoProcAmp_Brightness + 1,
	VideoProcAmp_Hue	= VideoProcAmp_Contrast + 1,
	VideoProcAmp_Saturation	= VideoProcAmp_Hue + 1,
	VideoProcAmp_Sharpness	= VideoProcAmp_Saturation + 1,
	VideoProcAmp_Gamma	= VideoProcAmp_Sharpness + 1,
	VideoProcAmp_ColorEnable	= VideoProcAmp_Gamma + 1,
	VideoProcAmp_WhiteBalance	= VideoProcAmp_ColorEnable + 1,
	VideoProcAmp_BacklightCompensation	= VideoProcAmp_WhiteBalance + 1
    }	VideoProcAmpProperty;

typedef 
enum tagVideoProcAmpFlags
    {	VideoProcAmp_Flags_Auto	= 0x1,
	VideoProcAmp_Flags_Manual	= 0x2
    }	VideoProcAmpFlags;



extern RPC_IF_HANDLE __MIDL_itf_strmif_0107_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_strmif_0107_v0_0_s_ifspec;

#ifndef __IAMVideoProcAmp_INTERFACE_DEFINED__
#define __IAMVideoProcAmp_INTERFACE_DEFINED__

/* interface IAMVideoProcAmp */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IAMVideoProcAmp;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("C6E13360-30AC-11d0-A18C-00A0C9118956")
    IAMVideoProcAmp : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetRange( 
            /* [in] */ long Property,
            /* [out] */ long __RPC_FAR *pMin,
            /* [out] */ long __RPC_FAR *pMax,
            /* [out] */ long __RPC_FAR *pSteppingDelta,
            /* [out] */ long __RPC_FAR *pDefault,
            /* [out] */ long __RPC_FAR *pCapsFlags) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Set( 
            /* [in] */ long Property,
            /* [in] */ long lValue,
            /* [in] */ long Flags) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Get( 
            /* [in] */ long Property,
            /* [out] */ long __RPC_FAR *lValue,
            /* [out] */ long __RPC_FAR *Flags) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAMVideoProcAmpVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IAMVideoProcAmp __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IAMVideoProcAmp __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IAMVideoProcAmp __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetRange )( 
            IAMVideoProcAmp __RPC_FAR * This,
            /* [in] */ long Property,
            /* [out] */ long __RPC_FAR *pMin,
            /* [out] */ long __RPC_FAR *pMax,
            /* [out] */ long __RPC_FAR *pSteppingDelta,
            /* [out] */ long __RPC_FAR *pDefault,
            /* [out] */ long __RPC_FAR *pCapsFlags);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Set )( 
            IAMVideoProcAmp __RPC_FAR * This,
            /* [in] */ long Property,
            /* [in] */ long lValue,
            /* [in] */ long Flags);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Get )( 
            IAMVideoProcAmp __RPC_FAR * This,
            /* [in] */ long Property,
            /* [out] */ long __RPC_FAR *lValue,
            /* [out] */ long __RPC_FAR *Flags);
        
        END_INTERFACE
    } IAMVideoProcAmpVtbl;

    interface IAMVideoProcAmp
    {
        CONST_VTBL struct IAMVideoProcAmpVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAMVideoProcAmp_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAMVideoProcAmp_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAMVideoProcAmp_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IAMVideoProcAmp_GetRange(This,Property,pMin,pMax,pSteppingDelta,pDefault,pCapsFlags)	\
    (This)->lpVtbl -> GetRange(This,Property,pMin,pMax,pSteppingDelta,pDefault,pCapsFlags)

#define IAMVideoProcAmp_Set(This,Property,lValue,Flags)	\
    (This)->lpVtbl -> Set(This,Property,lValue,Flags)

#define IAMVideoProcAmp_Get(This,Property,lValue,Flags)	\
    (This)->lpVtbl -> Get(This,Property,lValue,Flags)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IAMVideoProcAmp_GetRange_Proxy( 
    IAMVideoProcAmp __RPC_FAR * This,
    /* [in] */ long Property,
    /* [out] */ long __RPC_FAR *pMin,
    /* [out] */ long __RPC_FAR *pMax,
    /* [out] */ long __RPC_FAR *pSteppingDelta,
    /* [out] */ long __RPC_FAR *pDefault,
    /* [out] */ long __RPC_FAR *pCapsFlags);


void __RPC_STUB IAMVideoProcAmp_GetRange_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMVideoProcAmp_Set_Proxy( 
    IAMVideoProcAmp __RPC_FAR * This,
    /* [in] */ long Property,
    /* [in] */ long lValue,
    /* [in] */ long Flags);


void __RPC_STUB IAMVideoProcAmp_Set_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMVideoProcAmp_Get_Proxy( 
    IAMVideoProcAmp __RPC_FAR * This,
    /* [in] */ long Property,
    /* [out] */ long __RPC_FAR *lValue,
    /* [out] */ long __RPC_FAR *Flags);


void __RPC_STUB IAMVideoProcAmp_Get_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAMVideoProcAmp_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_strmif_0108 */
/* [local] */ 

typedef 
enum tagCameraControlProperty
    {	CameraControl_Pan	= 0,
	CameraControl_Tilt	= CameraControl_Pan + 1,
	CameraControl_Roll	= CameraControl_Tilt + 1,
	CameraControl_Zoom	= CameraControl_Roll + 1,
	CameraControl_Exposure	= CameraControl_Zoom + 1,
	CameraControl_Iris	= CameraControl_Exposure + 1,
	CameraControl_Focus	= CameraControl_Iris + 1,
	CameraControl_Flash	= CameraControl_Focus + 1
    }	CameraControlProperty;

typedef 
enum tagCameraControlFlags
    {	CameraControl_Flags_Auto	= 0x1,
	CameraControl_Flags_Manual	= 0x2
    }	CameraControlFlags;



extern RPC_IF_HANDLE __MIDL_itf_strmif_0108_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_strmif_0108_v0_0_s_ifspec;

#ifndef __IAMCameraControl_INTERFACE_DEFINED__
#define __IAMCameraControl_INTERFACE_DEFINED__

/* interface IAMCameraControl */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IAMCameraControl;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("C6E13370-30AC-11d0-A18C-00A0C9118956")
    IAMCameraControl : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetRange( 
            /* [in] */ long Property,
            /* [out] */ long __RPC_FAR *pMin,
            /* [out] */ long __RPC_FAR *pMax,
            /* [out] */ long __RPC_FAR *pSteppingDelta,
            /* [out] */ long __RPC_FAR *pDefault,
            /* [out] */ long __RPC_FAR *pCapsFlags) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Set( 
            /* [in] */ long Property,
            /* [in] */ long lValue,
            /* [in] */ long Flags) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Get( 
            /* [in] */ long Property,
            /* [out] */ long __RPC_FAR *lValue,
            /* [out] */ long __RPC_FAR *Flags) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAMCameraControlVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IAMCameraControl __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IAMCameraControl __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IAMCameraControl __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetRange )( 
            IAMCameraControl __RPC_FAR * This,
            /* [in] */ long Property,
            /* [out] */ long __RPC_FAR *pMin,
            /* [out] */ long __RPC_FAR *pMax,
            /* [out] */ long __RPC_FAR *pSteppingDelta,
            /* [out] */ long __RPC_FAR *pDefault,
            /* [out] */ long __RPC_FAR *pCapsFlags);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Set )( 
            IAMCameraControl __RPC_FAR * This,
            /* [in] */ long Property,
            /* [in] */ long lValue,
            /* [in] */ long Flags);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Get )( 
            IAMCameraControl __RPC_FAR * This,
            /* [in] */ long Property,
            /* [out] */ long __RPC_FAR *lValue,
            /* [out] */ long __RPC_FAR *Flags);
        
        END_INTERFACE
    } IAMCameraControlVtbl;

    interface IAMCameraControl
    {
        CONST_VTBL struct IAMCameraControlVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAMCameraControl_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAMCameraControl_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAMCameraControl_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IAMCameraControl_GetRange(This,Property,pMin,pMax,pSteppingDelta,pDefault,pCapsFlags)	\
    (This)->lpVtbl -> GetRange(This,Property,pMin,pMax,pSteppingDelta,pDefault,pCapsFlags)

#define IAMCameraControl_Set(This,Property,lValue,Flags)	\
    (This)->lpVtbl -> Set(This,Property,lValue,Flags)

#define IAMCameraControl_Get(This,Property,lValue,Flags)	\
    (This)->lpVtbl -> Get(This,Property,lValue,Flags)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IAMCameraControl_GetRange_Proxy( 
    IAMCameraControl __RPC_FAR * This,
    /* [in] */ long Property,
    /* [out] */ long __RPC_FAR *pMin,
    /* [out] */ long __RPC_FAR *pMax,
    /* [out] */ long __RPC_FAR *pSteppingDelta,
    /* [out] */ long __RPC_FAR *pDefault,
    /* [out] */ long __RPC_FAR *pCapsFlags);


void __RPC_STUB IAMCameraControl_GetRange_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMCameraControl_Set_Proxy( 
    IAMCameraControl __RPC_FAR * This,
    /* [in] */ long Property,
    /* [in] */ long lValue,
    /* [in] */ long Flags);


void __RPC_STUB IAMCameraControl_Set_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMCameraControl_Get_Proxy( 
    IAMCameraControl __RPC_FAR * This,
    /* [in] */ long Property,
    /* [out] */ long __RPC_FAR *lValue,
    /* [out] */ long __RPC_FAR *Flags);


void __RPC_STUB IAMCameraControl_Get_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAMCameraControl_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_strmif_0109 */
/* [local] */ 

typedef 
enum tagVideoControlFlags
    {	VideoControlFlag_FlipHorizontal	= 0x1,
	VideoControlFlag_FlipVertical	= 0x2,
	VideoControlFlag_ExternalTriggerEnable	= 0x4,
	VideoControlFlag_Trigger	= 0x8
    }	VideoControlFlags;



extern RPC_IF_HANDLE __MIDL_itf_strmif_0109_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_strmif_0109_v0_0_s_ifspec;

#ifndef __IAMVideoControl_INTERFACE_DEFINED__
#define __IAMVideoControl_INTERFACE_DEFINED__

/* interface IAMVideoControl */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IAMVideoControl;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6a2e0670-28e4-11d0-a18c-00a0c9118956")
    IAMVideoControl : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetCaps( 
            /* [in] */ IPin __RPC_FAR *pPin,
            /* [out] */ long __RPC_FAR *pCapsFlags) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetMode( 
            /* [in] */ IPin __RPC_FAR *pPin,
            /* [in] */ long Mode) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetMode( 
            /* [in] */ IPin __RPC_FAR *pPin,
            /* [out] */ long __RPC_FAR *Mode) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetCurrentActualFrameRate( 
            /* [in] */ IPin __RPC_FAR *pPin,
            /* [out] */ LONGLONG __RPC_FAR *ActualFrameRate) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetMaxAvailableFrameRate( 
            /* [in] */ IPin __RPC_FAR *pPin,
            /* [in] */ long iIndex,
            /* [in] */ SIZE Dimensions,
            /* [out] */ LONGLONG __RPC_FAR *MaxAvailableFrameRate) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetFrameRateList( 
            /* [in] */ IPin __RPC_FAR *pPin,
            /* [in] */ long iIndex,
            /* [in] */ SIZE Dimensions,
            /* [out] */ long __RPC_FAR *ListSize,
            /* [out] */ LONGLONG __RPC_FAR *__RPC_FAR *FrameRates) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAMVideoControlVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IAMVideoControl __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IAMVideoControl __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IAMVideoControl __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetCaps )( 
            IAMVideoControl __RPC_FAR * This,
            /* [in] */ IPin __RPC_FAR *pPin,
            /* [out] */ long __RPC_FAR *pCapsFlags);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetMode )( 
            IAMVideoControl __RPC_FAR * This,
            /* [in] */ IPin __RPC_FAR *pPin,
            /* [in] */ long Mode);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMode )( 
            IAMVideoControl __RPC_FAR * This,
            /* [in] */ IPin __RPC_FAR *pPin,
            /* [out] */ long __RPC_FAR *Mode);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetCurrentActualFrameRate )( 
            IAMVideoControl __RPC_FAR * This,
            /* [in] */ IPin __RPC_FAR *pPin,
            /* [out] */ LONGLONG __RPC_FAR *ActualFrameRate);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMaxAvailableFrameRate )( 
            IAMVideoControl __RPC_FAR * This,
            /* [in] */ IPin __RPC_FAR *pPin,
            /* [in] */ long iIndex,
            /* [in] */ SIZE Dimensions,
            /* [out] */ LONGLONG __RPC_FAR *MaxAvailableFrameRate);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetFrameRateList )( 
            IAMVideoControl __RPC_FAR * This,
            /* [in] */ IPin __RPC_FAR *pPin,
            /* [in] */ long iIndex,
            /* [in] */ SIZE Dimensions,
            /* [out] */ long __RPC_FAR *ListSize,
            /* [out] */ LONGLONG __RPC_FAR *__RPC_FAR *FrameRates);
        
        END_INTERFACE
    } IAMVideoControlVtbl;

    interface IAMVideoControl
    {
        CONST_VTBL struct IAMVideoControlVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAMVideoControl_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAMVideoControl_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAMVideoControl_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IAMVideoControl_GetCaps(This,pPin,pCapsFlags)	\
    (This)->lpVtbl -> GetCaps(This,pPin,pCapsFlags)

#define IAMVideoControl_SetMode(This,pPin,Mode)	\
    (This)->lpVtbl -> SetMode(This,pPin,Mode)

#define IAMVideoControl_GetMode(This,pPin,Mode)	\
    (This)->lpVtbl -> GetMode(This,pPin,Mode)

#define IAMVideoControl_GetCurrentActualFrameRate(This,pPin,ActualFrameRate)	\
    (This)->lpVtbl -> GetCurrentActualFrameRate(This,pPin,ActualFrameRate)

#define IAMVideoControl_GetMaxAvailableFrameRate(This,pPin,iIndex,Dimensions,MaxAvailableFrameRate)	\
    (This)->lpVtbl -> GetMaxAvailableFrameRate(This,pPin,iIndex,Dimensions,MaxAvailableFrameRate)

#define IAMVideoControl_GetFrameRateList(This,pPin,iIndex,Dimensions,ListSize,FrameRates)	\
    (This)->lpVtbl -> GetFrameRateList(This,pPin,iIndex,Dimensions,ListSize,FrameRates)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IAMVideoControl_GetCaps_Proxy( 
    IAMVideoControl __RPC_FAR * This,
    /* [in] */ IPin __RPC_FAR *pPin,
    /* [out] */ long __RPC_FAR *pCapsFlags);


void __RPC_STUB IAMVideoControl_GetCaps_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMVideoControl_SetMode_Proxy( 
    IAMVideoControl __RPC_FAR * This,
    /* [in] */ IPin __RPC_FAR *pPin,
    /* [in] */ long Mode);


void __RPC_STUB IAMVideoControl_SetMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMVideoControl_GetMode_Proxy( 
    IAMVideoControl __RPC_FAR * This,
    /* [in] */ IPin __RPC_FAR *pPin,
    /* [out] */ long __RPC_FAR *Mode);


void __RPC_STUB IAMVideoControl_GetMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMVideoControl_GetCurrentActualFrameRate_Proxy( 
    IAMVideoControl __RPC_FAR * This,
    /* [in] */ IPin __RPC_FAR *pPin,
    /* [out] */ LONGLONG __RPC_FAR *ActualFrameRate);


void __RPC_STUB IAMVideoControl_GetCurrentActualFrameRate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMVideoControl_GetMaxAvailableFrameRate_Proxy( 
    IAMVideoControl __RPC_FAR * This,
    /* [in] */ IPin __RPC_FAR *pPin,
    /* [in] */ long iIndex,
    /* [in] */ SIZE Dimensions,
    /* [out] */ LONGLONG __RPC_FAR *MaxAvailableFrameRate);


void __RPC_STUB IAMVideoControl_GetMaxAvailableFrameRate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMVideoControl_GetFrameRateList_Proxy( 
    IAMVideoControl __RPC_FAR * This,
    /* [in] */ IPin __RPC_FAR *pPin,
    /* [in] */ long iIndex,
    /* [in] */ SIZE Dimensions,
    /* [out] */ long __RPC_FAR *ListSize,
    /* [out] */ LONGLONG __RPC_FAR *__RPC_FAR *FrameRates);


void __RPC_STUB IAMVideoControl_GetFrameRateList_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAMVideoControl_INTERFACE_DEFINED__ */


#ifndef __IAMCrossbar_INTERFACE_DEFINED__
#define __IAMCrossbar_INTERFACE_DEFINED__

/* interface IAMCrossbar */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IAMCrossbar;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("C6E13380-30AC-11d0-A18C-00A0C9118956")
    IAMCrossbar : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE get_PinCounts( 
            /* [out] */ long __RPC_FAR *OutputPinCount,
            /* [out] */ long __RPC_FAR *InputPinCount) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CanRoute( 
            /* [in] */ long OutputPinIndex,
            /* [in] */ long InputPinIndex) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Route( 
            /* [in] */ long OutputPinIndex,
            /* [in] */ long InputPinIndex) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get_IsRoutedTo( 
            /* [in] */ long OutputPinIndex,
            /* [out] */ long __RPC_FAR *InputPinIndex) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get_CrossbarPinInfo( 
            /* [in] */ BOOL IsInputPin,
            /* [in] */ long PinIndex,
            /* [out] */ long __RPC_FAR *PinIndexRelated,
            /* [out] */ long __RPC_FAR *PhysicalType) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAMCrossbarVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IAMCrossbar __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IAMCrossbar __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IAMCrossbar __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_PinCounts )( 
            IAMCrossbar __RPC_FAR * This,
            /* [out] */ long __RPC_FAR *OutputPinCount,
            /* [out] */ long __RPC_FAR *InputPinCount);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CanRoute )( 
            IAMCrossbar __RPC_FAR * This,
            /* [in] */ long OutputPinIndex,
            /* [in] */ long InputPinIndex);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Route )( 
            IAMCrossbar __RPC_FAR * This,
            /* [in] */ long OutputPinIndex,
            /* [in] */ long InputPinIndex);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IsRoutedTo )( 
            IAMCrossbar __RPC_FAR * This,
            /* [in] */ long OutputPinIndex,
            /* [out] */ long __RPC_FAR *InputPinIndex);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_CrossbarPinInfo )( 
            IAMCrossbar __RPC_FAR * This,
            /* [in] */ BOOL IsInputPin,
            /* [in] */ long PinIndex,
            /* [out] */ long __RPC_FAR *PinIndexRelated,
            /* [out] */ long __RPC_FAR *PhysicalType);
        
        END_INTERFACE
    } IAMCrossbarVtbl;

    interface IAMCrossbar
    {
        CONST_VTBL struct IAMCrossbarVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAMCrossbar_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAMCrossbar_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAMCrossbar_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IAMCrossbar_get_PinCounts(This,OutputPinCount,InputPinCount)	\
    (This)->lpVtbl -> get_PinCounts(This,OutputPinCount,InputPinCount)

#define IAMCrossbar_CanRoute(This,OutputPinIndex,InputPinIndex)	\
    (This)->lpVtbl -> CanRoute(This,OutputPinIndex,InputPinIndex)

#define IAMCrossbar_Route(This,OutputPinIndex,InputPinIndex)	\
    (This)->lpVtbl -> Route(This,OutputPinIndex,InputPinIndex)

#define IAMCrossbar_get_IsRoutedTo(This,OutputPinIndex,InputPinIndex)	\
    (This)->lpVtbl -> get_IsRoutedTo(This,OutputPinIndex,InputPinIndex)

#define IAMCrossbar_get_CrossbarPinInfo(This,IsInputPin,PinIndex,PinIndexRelated,PhysicalType)	\
    (This)->lpVtbl -> get_CrossbarPinInfo(This,IsInputPin,PinIndex,PinIndexRelated,PhysicalType)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IAMCrossbar_get_PinCounts_Proxy( 
    IAMCrossbar __RPC_FAR * This,
    /* [out] */ long __RPC_FAR *OutputPinCount,
    /* [out] */ long __RPC_FAR *InputPinCount);


void __RPC_STUB IAMCrossbar_get_PinCounts_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMCrossbar_CanRoute_Proxy( 
    IAMCrossbar __RPC_FAR * This,
    /* [in] */ long OutputPinIndex,
    /* [in] */ long InputPinIndex);


void __RPC_STUB IAMCrossbar_CanRoute_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMCrossbar_Route_Proxy( 
    IAMCrossbar __RPC_FAR * This,
    /* [in] */ long OutputPinIndex,
    /* [in] */ long InputPinIndex);


void __RPC_STUB IAMCrossbar_Route_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMCrossbar_get_IsRoutedTo_Proxy( 
    IAMCrossbar __RPC_FAR * This,
    /* [in] */ long OutputPinIndex,
    /* [out] */ long __RPC_FAR *InputPinIndex);


void __RPC_STUB IAMCrossbar_get_IsRoutedTo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMCrossbar_get_CrossbarPinInfo_Proxy( 
    IAMCrossbar __RPC_FAR * This,
    /* [in] */ BOOL IsInputPin,
    /* [in] */ long PinIndex,
    /* [out] */ long __RPC_FAR *PinIndexRelated,
    /* [out] */ long __RPC_FAR *PhysicalType);


void __RPC_STUB IAMCrossbar_get_CrossbarPinInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAMCrossbar_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_strmif_0111 */
/* [local] */ 

typedef 
enum tagAMTunerSubChannel
    {	AMTUNER_SUBCHAN_NO_TUNE	= -2,
	AMTUNER_SUBCHAN_DEFAULT	= -1
    }	AMTunerSubChannel;

typedef 
enum tagAMTunerSignalStrength
    {	AMTUNER_HASNOSIGNALSTRENGTH	= -1,
	AMTUNER_NOSIGNAL	= 0,
	AMTUNER_SIGNALPRESENT	= 1
    }	AMTunerSignalStrength;

typedef 
enum tagAMTunerModeType
    {	AMTUNER_MODE_DEFAULT	= 0,
	AMTUNER_MODE_TV	= 0x1,
	AMTUNER_MODE_FM_RADIO	= 0x2,
	AMTUNER_MODE_AM_RADIO	= 0x4,
	AMTUNER_MODE_DSS	= 0x8
    }	AMTunerModeType;

typedef 
enum tagAMTunerEventType
    {	AMTUNER_EVENT_CHANGED	= 0x1
    }	AMTunerEventType;




extern RPC_IF_HANDLE __MIDL_itf_strmif_0111_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_strmif_0111_v0_0_s_ifspec;

#ifndef __IAMTuner_INTERFACE_DEFINED__
#define __IAMTuner_INTERFACE_DEFINED__

/* interface IAMTuner */
/* [unique][uuid][local][object] */ 


EXTERN_C const IID IID_IAMTuner;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("211A8761-03AC-11d1-8D13-00AA00BD8339")
    IAMTuner : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE put_Channel( 
            /* [in] */ long lChannel,
            /* [in] */ long lVideoSubChannel,
            /* [in] */ long lAudioSubChannel) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get_Channel( 
            /* [out] */ long __RPC_FAR *plChannel,
            /* [out] */ long __RPC_FAR *plVideoSubChannel,
            /* [out] */ long __RPC_FAR *plAudioSubChannel) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ChannelMinMax( 
            /* [out] */ long __RPC_FAR *lChannelMin,
            /* [out] */ long __RPC_FAR *lChannelMax) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE put_CountryCode( 
            /* [in] */ long lCountryCode) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get_CountryCode( 
            /* [out] */ long __RPC_FAR *plCountryCode) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE put_TuningSpace( 
            /* [in] */ long lTuningSpace) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get_TuningSpace( 
            /* [out] */ long __RPC_FAR *plTuningSpace) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Logon( 
            /* [in] */ HANDLE hCurrentUser) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Logout( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SignalPresent( 
            /* [out] */ long __RPC_FAR *plSignalStrength) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE put_Mode( 
            /* [in] */ AMTunerModeType lMode) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get_Mode( 
            /* [out] */ AMTunerModeType __RPC_FAR *plMode) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetAvailableModes( 
            /* [out] */ long __RPC_FAR *plModes) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RegisterNotificationCallBack( 
            /* [in] */ IAMTunerNotification __RPC_FAR *pNotify,
            /* [in] */ long lEvents) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE UnRegisterNotificationCallBack( 
            IAMTunerNotification __RPC_FAR *pNotify) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAMTunerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IAMTuner __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IAMTuner __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IAMTuner __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Channel )( 
            IAMTuner __RPC_FAR * This,
            /* [in] */ long lChannel,
            /* [in] */ long lVideoSubChannel,
            /* [in] */ long lAudioSubChannel);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Channel )( 
            IAMTuner __RPC_FAR * This,
            /* [out] */ long __RPC_FAR *plChannel,
            /* [out] */ long __RPC_FAR *plVideoSubChannel,
            /* [out] */ long __RPC_FAR *plAudioSubChannel);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ChannelMinMax )( 
            IAMTuner __RPC_FAR * This,
            /* [out] */ long __RPC_FAR *lChannelMin,
            /* [out] */ long __RPC_FAR *lChannelMax);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_CountryCode )( 
            IAMTuner __RPC_FAR * This,
            /* [in] */ long lCountryCode);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_CountryCode )( 
            IAMTuner __RPC_FAR * This,
            /* [out] */ long __RPC_FAR *plCountryCode);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_TuningSpace )( 
            IAMTuner __RPC_FAR * This,
            /* [in] */ long lTuningSpace);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_TuningSpace )( 
            IAMTuner __RPC_FAR * This,
            /* [out] */ long __RPC_FAR *plTuningSpace);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Logon )( 
            IAMTuner __RPC_FAR * This,
            /* [in] */ HANDLE hCurrentUser);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Logout )( 
            IAMTuner __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SignalPresent )( 
            IAMTuner __RPC_FAR * This,
            /* [out] */ long __RPC_FAR *plSignalStrength);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Mode )( 
            IAMTuner __RPC_FAR * This,
            /* [in] */ AMTunerModeType lMode);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Mode )( 
            IAMTuner __RPC_FAR * This,
            /* [out] */ AMTunerModeType __RPC_FAR *plMode);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetAvailableModes )( 
            IAMTuner __RPC_FAR * This,
            /* [out] */ long __RPC_FAR *plModes);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RegisterNotificationCallBack )( 
            IAMTuner __RPC_FAR * This,
            /* [in] */ IAMTunerNotification __RPC_FAR *pNotify,
            /* [in] */ long lEvents);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *UnRegisterNotificationCallBack )( 
            IAMTuner __RPC_FAR * This,
            IAMTunerNotification __RPC_FAR *pNotify);
        
        END_INTERFACE
    } IAMTunerVtbl;

    interface IAMTuner
    {
        CONST_VTBL struct IAMTunerVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAMTuner_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAMTuner_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAMTuner_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IAMTuner_put_Channel(This,lChannel,lVideoSubChannel,lAudioSubChannel)	\
    (This)->lpVtbl -> put_Channel(This,lChannel,lVideoSubChannel,lAudioSubChannel)

#define IAMTuner_get_Channel(This,plChannel,plVideoSubChannel,plAudioSubChannel)	\
    (This)->lpVtbl -> get_Channel(This,plChannel,plVideoSubChannel,plAudioSubChannel)

#define IAMTuner_ChannelMinMax(This,lChannelMin,lChannelMax)	\
    (This)->lpVtbl -> ChannelMinMax(This,lChannelMin,lChannelMax)

#define IAMTuner_put_CountryCode(This,lCountryCode)	\
    (This)->lpVtbl -> put_CountryCode(This,lCountryCode)

#define IAMTuner_get_CountryCode(This,plCountryCode)	\
    (This)->lpVtbl -> get_CountryCode(This,plCountryCode)

#define IAMTuner_put_TuningSpace(This,lTuningSpace)	\
    (This)->lpVtbl -> put_TuningSpace(This,lTuningSpace)

#define IAMTuner_get_TuningSpace(This,plTuningSpace)	\
    (This)->lpVtbl -> get_TuningSpace(This,plTuningSpace)

#define IAMTuner_Logon(This,hCurrentUser)	\
    (This)->lpVtbl -> Logon(This,hCurrentUser)

#define IAMTuner_Logout(This)	\
    (This)->lpVtbl -> Logout(This)

#define IAMTuner_SignalPresent(This,plSignalStrength)	\
    (This)->lpVtbl -> SignalPresent(This,plSignalStrength)

#define IAMTuner_put_Mode(This,lMode)	\
    (This)->lpVtbl -> put_Mode(This,lMode)

#define IAMTuner_get_Mode(This,plMode)	\
    (This)->lpVtbl -> get_Mode(This,plMode)

#define IAMTuner_GetAvailableModes(This,plModes)	\
    (This)->lpVtbl -> GetAvailableModes(This,plModes)

#define IAMTuner_RegisterNotificationCallBack(This,pNotify,lEvents)	\
    (This)->lpVtbl -> RegisterNotificationCallBack(This,pNotify,lEvents)

#define IAMTuner_UnRegisterNotificationCallBack(This,pNotify)	\
    (This)->lpVtbl -> UnRegisterNotificationCallBack(This,pNotify)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IAMTuner_put_Channel_Proxy( 
    IAMTuner __RPC_FAR * This,
    /* [in] */ long lChannel,
    /* [in] */ long lVideoSubChannel,
    /* [in] */ long lAudioSubChannel);


void __RPC_STUB IAMTuner_put_Channel_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMTuner_get_Channel_Proxy( 
    IAMTuner __RPC_FAR * This,
    /* [out] */ long __RPC_FAR *plChannel,
    /* [out] */ long __RPC_FAR *plVideoSubChannel,
    /* [out] */ long __RPC_FAR *plAudioSubChannel);


void __RPC_STUB IAMTuner_get_Channel_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMTuner_ChannelMinMax_Proxy( 
    IAMTuner __RPC_FAR * This,
    /* [out] */ long __RPC_FAR *lChannelMin,
    /* [out] */ long __RPC_FAR *lChannelMax);


void __RPC_STUB IAMTuner_ChannelMinMax_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMTuner_put_CountryCode_Proxy( 
    IAMTuner __RPC_FAR * This,
    /* [in] */ long lCountryCode);


void __RPC_STUB IAMTuner_put_CountryCode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMTuner_get_CountryCode_Proxy( 
    IAMTuner __RPC_FAR * This,
    /* [out] */ long __RPC_FAR *plCountryCode);


void __RPC_STUB IAMTuner_get_CountryCode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMTuner_put_TuningSpace_Proxy( 
    IAMTuner __RPC_FAR * This,
    /* [in] */ long lTuningSpace);


void __RPC_STUB IAMTuner_put_TuningSpace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMTuner_get_TuningSpace_Proxy( 
    IAMTuner __RPC_FAR * This,
    /* [out] */ long __RPC_FAR *plTuningSpace);


void __RPC_STUB IAMTuner_get_TuningSpace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMTuner_Logon_Proxy( 
    IAMTuner __RPC_FAR * This,
    /* [in] */ HANDLE hCurrentUser);


void __RPC_STUB IAMTuner_Logon_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMTuner_Logout_Proxy( 
    IAMTuner __RPC_FAR * This);


void __RPC_STUB IAMTuner_Logout_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMTuner_SignalPresent_Proxy( 
    IAMTuner __RPC_FAR * This,
    /* [out] */ long __RPC_FAR *plSignalStrength);


void __RPC_STUB IAMTuner_SignalPresent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMTuner_put_Mode_Proxy( 
    IAMTuner __RPC_FAR * This,
    /* [in] */ AMTunerModeType lMode);


void __RPC_STUB IAMTuner_put_Mode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMTuner_get_Mode_Proxy( 
    IAMTuner __RPC_FAR * This,
    /* [out] */ AMTunerModeType __RPC_FAR *plMode);


void __RPC_STUB IAMTuner_get_Mode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMTuner_GetAvailableModes_Proxy( 
    IAMTuner __RPC_FAR * This,
    /* [out] */ long __RPC_FAR *plModes);


void __RPC_STUB IAMTuner_GetAvailableModes_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMTuner_RegisterNotificationCallBack_Proxy( 
    IAMTuner __RPC_FAR * This,
    /* [in] */ IAMTunerNotification __RPC_FAR *pNotify,
    /* [in] */ long lEvents);


void __RPC_STUB IAMTuner_RegisterNotificationCallBack_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMTuner_UnRegisterNotificationCallBack_Proxy( 
    IAMTuner __RPC_FAR * This,
    IAMTunerNotification __RPC_FAR *pNotify);


void __RPC_STUB IAMTuner_UnRegisterNotificationCallBack_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAMTuner_INTERFACE_DEFINED__ */


#ifndef __IAMTunerNotification_INTERFACE_DEFINED__
#define __IAMTunerNotification_INTERFACE_DEFINED__

/* interface IAMTunerNotification */
/* [unique][uuid][local][object] */ 


EXTERN_C const IID IID_IAMTunerNotification;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("211A8760-03AC-11d1-8D13-00AA00BD8339")
    IAMTunerNotification : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE OnEvent( 
            /* [in] */ AMTunerEventType Event) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAMTunerNotificationVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IAMTunerNotification __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IAMTunerNotification __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IAMTunerNotification __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnEvent )( 
            IAMTunerNotification __RPC_FAR * This,
            /* [in] */ AMTunerEventType Event);
        
        END_INTERFACE
    } IAMTunerNotificationVtbl;

    interface IAMTunerNotification
    {
        CONST_VTBL struct IAMTunerNotificationVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAMTunerNotification_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAMTunerNotification_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAMTunerNotification_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IAMTunerNotification_OnEvent(This,Event)	\
    (This)->lpVtbl -> OnEvent(This,Event)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IAMTunerNotification_OnEvent_Proxy( 
    IAMTunerNotification __RPC_FAR * This,
    /* [in] */ AMTunerEventType Event);


void __RPC_STUB IAMTunerNotification_OnEvent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAMTunerNotification_INTERFACE_DEFINED__ */


#ifndef __IAMTVTuner_INTERFACE_DEFINED__
#define __IAMTVTuner_INTERFACE_DEFINED__

/* interface IAMTVTuner */
/* [unique][uuid][local][object] */ 


EXTERN_C const IID IID_IAMTVTuner;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("211A8766-03AC-11d1-8D13-00AA00BD8339")
    IAMTVTuner : public IAMTuner
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE get_AvailableTVFormats( 
            /* [out] */ long __RPC_FAR *lAnalogVideoStandard) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get_TVFormat( 
            /* [out] */ long __RPC_FAR *plAnalogVideoStandard) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AutoTune( 
            /* [in] */ long lChannel,
            /* [out] */ long __RPC_FAR *plFoundSignal) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE StoreAutoTune( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get_NumInputConnections( 
            /* [out] */ long __RPC_FAR *plNumInputConnections) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE put_InputType( 
            /* [in] */ long lIndex,
            /* [in] */ TunerInputType InputType) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get_InputType( 
            /* [in] */ long lIndex,
            /* [out] */ TunerInputType __RPC_FAR *pInputType) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE put_ConnectInput( 
            /* [in] */ long lIndex) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get_ConnectInput( 
            /* [out] */ long __RPC_FAR *plIndex) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get_VideoFrequency( 
            /* [out] */ long __RPC_FAR *lFreq) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get_AudioFrequency( 
            /* [out] */ long __RPC_FAR *lFreq) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAMTVTunerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IAMTVTuner __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IAMTVTuner __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IAMTVTuner __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Channel )( 
            IAMTVTuner __RPC_FAR * This,
            /* [in] */ long lChannel,
            /* [in] */ long lVideoSubChannel,
            /* [in] */ long lAudioSubChannel);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Channel )( 
            IAMTVTuner __RPC_FAR * This,
            /* [out] */ long __RPC_FAR *plChannel,
            /* [out] */ long __RPC_FAR *plVideoSubChannel,
            /* [out] */ long __RPC_FAR *plAudioSubChannel);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ChannelMinMax )( 
            IAMTVTuner __RPC_FAR * This,
            /* [out] */ long __RPC_FAR *lChannelMin,
            /* [out] */ long __RPC_FAR *lChannelMax);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_CountryCode )( 
            IAMTVTuner __RPC_FAR * This,
            /* [in] */ long lCountryCode);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_CountryCode )( 
            IAMTVTuner __RPC_FAR * This,
            /* [out] */ long __RPC_FAR *plCountryCode);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_TuningSpace )( 
            IAMTVTuner __RPC_FAR * This,
            /* [in] */ long lTuningSpace);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_TuningSpace )( 
            IAMTVTuner __RPC_FAR * This,
            /* [out] */ long __RPC_FAR *plTuningSpace);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Logon )( 
            IAMTVTuner __RPC_FAR * This,
            /* [in] */ HANDLE hCurrentUser);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Logout )( 
            IAMTVTuner __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SignalPresent )( 
            IAMTVTuner __RPC_FAR * This,
            /* [out] */ long __RPC_FAR *plSignalStrength);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Mode )( 
            IAMTVTuner __RPC_FAR * This,
            /* [in] */ AMTunerModeType lMode);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Mode )( 
            IAMTVTuner __RPC_FAR * This,
            /* [out] */ AMTunerModeType __RPC_FAR *plMode);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetAvailableModes )( 
            IAMTVTuner __RPC_FAR * This,
            /* [out] */ long __RPC_FAR *plModes);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RegisterNotificationCallBack )( 
            IAMTVTuner __RPC_FAR * This,
            /* [in] */ IAMTunerNotification __RPC_FAR *pNotify,
            /* [in] */ long lEvents);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *UnRegisterNotificationCallBack )( 
            IAMTVTuner __RPC_FAR * This,
            IAMTunerNotification __RPC_FAR *pNotify);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_AvailableTVFormats )( 
            IAMTVTuner __RPC_FAR * This,
            /* [out] */ long __RPC_FAR *lAnalogVideoStandard);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_TVFormat )( 
            IAMTVTuner __RPC_FAR * This,
            /* [out] */ long __RPC_FAR *plAnalogVideoStandard);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AutoTune )( 
            IAMTVTuner __RPC_FAR * This,
            /* [in] */ long lChannel,
            /* [out] */ long __RPC_FAR *plFoundSignal);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *StoreAutoTune )( 
            IAMTVTuner __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_NumInputConnections )( 
            IAMTVTuner __RPC_FAR * This,
            /* [out] */ long __RPC_FAR *plNumInputConnections);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_InputType )( 
            IAMTVTuner __RPC_FAR * This,
            /* [in] */ long lIndex,
            /* [in] */ TunerInputType InputType);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_InputType )( 
            IAMTVTuner __RPC_FAR * This,
            /* [in] */ long lIndex,
            /* [out] */ TunerInputType __RPC_FAR *pInputType);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ConnectInput )( 
            IAMTVTuner __RPC_FAR * This,
            /* [in] */ long lIndex);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ConnectInput )( 
            IAMTVTuner __RPC_FAR * This,
            /* [out] */ long __RPC_FAR *plIndex);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_VideoFrequency )( 
            IAMTVTuner __RPC_FAR * This,
            /* [out] */ long __RPC_FAR *lFreq);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_AudioFrequency )( 
            IAMTVTuner __RPC_FAR * This,
            /* [out] */ long __RPC_FAR *lFreq);
        
        END_INTERFACE
    } IAMTVTunerVtbl;

    interface IAMTVTuner
    {
        CONST_VTBL struct IAMTVTunerVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAMTVTuner_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAMTVTuner_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAMTVTuner_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IAMTVTuner_put_Channel(This,lChannel,lVideoSubChannel,lAudioSubChannel)	\
    (This)->lpVtbl -> put_Channel(This,lChannel,lVideoSubChannel,lAudioSubChannel)

#define IAMTVTuner_get_Channel(This,plChannel,plVideoSubChannel,plAudioSubChannel)	\
    (This)->lpVtbl -> get_Channel(This,plChannel,plVideoSubChannel,plAudioSubChannel)

#define IAMTVTuner_ChannelMinMax(This,lChannelMin,lChannelMax)	\
    (This)->lpVtbl -> ChannelMinMax(This,lChannelMin,lChannelMax)

#define IAMTVTuner_put_CountryCode(This,lCountryCode)	\
    (This)->lpVtbl -> put_CountryCode(This,lCountryCode)

#define IAMTVTuner_get_CountryCode(This,plCountryCode)	\
    (This)->lpVtbl -> get_CountryCode(This,plCountryCode)

#define IAMTVTuner_put_TuningSpace(This,lTuningSpace)	\
    (This)->lpVtbl -> put_TuningSpace(This,lTuningSpace)

#define IAMTVTuner_get_TuningSpace(This,plTuningSpace)	\
    (This)->lpVtbl -> get_TuningSpace(This,plTuningSpace)

#define IAMTVTuner_Logon(This,hCurrentUser)	\
    (This)->lpVtbl -> Logon(This,hCurrentUser)

#define IAMTVTuner_Logout(This)	\
    (This)->lpVtbl -> Logout(This)

#define IAMTVTuner_SignalPresent(This,plSignalStrength)	\
    (This)->lpVtbl -> SignalPresent(This,plSignalStrength)

#define IAMTVTuner_put_Mode(This,lMode)	\
    (This)->lpVtbl -> put_Mode(This,lMode)

#define IAMTVTuner_get_Mode(This,plMode)	\
    (This)->lpVtbl -> get_Mode(This,plMode)

#define IAMTVTuner_GetAvailableModes(This,plModes)	\
    (This)->lpVtbl -> GetAvailableModes(This,plModes)

#define IAMTVTuner_RegisterNotificationCallBack(This,pNotify,lEvents)	\
    (This)->lpVtbl -> RegisterNotificationCallBack(This,pNotify,lEvents)

#define IAMTVTuner_UnRegisterNotificationCallBack(This,pNotify)	\
    (This)->lpVtbl -> UnRegisterNotificationCallBack(This,pNotify)


#define IAMTVTuner_get_AvailableTVFormats(This,lAnalogVideoStandard)	\
    (This)->lpVtbl -> get_AvailableTVFormats(This,lAnalogVideoStandard)

#define IAMTVTuner_get_TVFormat(This,plAnalogVideoStandard)	\
    (This)->lpVtbl -> get_TVFormat(This,plAnalogVideoStandard)

#define IAMTVTuner_AutoTune(This,lChannel,plFoundSignal)	\
    (This)->lpVtbl -> AutoTune(This,lChannel,plFoundSignal)

#define IAMTVTuner_StoreAutoTune(This)	\
    (This)->lpVtbl -> StoreAutoTune(This)

#define IAMTVTuner_get_NumInputConnections(This,plNumInputConnections)	\
    (This)->lpVtbl -> get_NumInputConnections(This,plNumInputConnections)

#define IAMTVTuner_put_InputType(This,lIndex,InputType)	\
    (This)->lpVtbl -> put_InputType(This,lIndex,InputType)

#define IAMTVTuner_get_InputType(This,lIndex,pInputType)	\
    (This)->lpVtbl -> get_InputType(This,lIndex,pInputType)

#define IAMTVTuner_put_ConnectInput(This,lIndex)	\
    (This)->lpVtbl -> put_ConnectInput(This,lIndex)

#define IAMTVTuner_get_ConnectInput(This,plIndex)	\
    (This)->lpVtbl -> get_ConnectInput(This,plIndex)

#define IAMTVTuner_get_VideoFrequency(This,lFreq)	\
    (This)->lpVtbl -> get_VideoFrequency(This,lFreq)

#define IAMTVTuner_get_AudioFrequency(This,lFreq)	\
    (This)->lpVtbl -> get_AudioFrequency(This,lFreq)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IAMTVTuner_get_AvailableTVFormats_Proxy( 
    IAMTVTuner __RPC_FAR * This,
    /* [out] */ long __RPC_FAR *lAnalogVideoStandard);


void __RPC_STUB IAMTVTuner_get_AvailableTVFormats_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMTVTuner_get_TVFormat_Proxy( 
    IAMTVTuner __RPC_FAR * This,
    /* [out] */ long __RPC_FAR *plAnalogVideoStandard);


void __RPC_STUB IAMTVTuner_get_TVFormat_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMTVTuner_AutoTune_Proxy( 
    IAMTVTuner __RPC_FAR * This,
    /* [in] */ long lChannel,
    /* [out] */ long __RPC_FAR *plFoundSignal);


void __RPC_STUB IAMTVTuner_AutoTune_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMTVTuner_StoreAutoTune_Proxy( 
    IAMTVTuner __RPC_FAR * This);


void __RPC_STUB IAMTVTuner_StoreAutoTune_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMTVTuner_get_NumInputConnections_Proxy( 
    IAMTVTuner __RPC_FAR * This,
    /* [out] */ long __RPC_FAR *plNumInputConnections);


void __RPC_STUB IAMTVTuner_get_NumInputConnections_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMTVTuner_put_InputType_Proxy( 
    IAMTVTuner __RPC_FAR * This,
    /* [in] */ long lIndex,
    /* [in] */ TunerInputType InputType);


void __RPC_STUB IAMTVTuner_put_InputType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMTVTuner_get_InputType_Proxy( 
    IAMTVTuner __RPC_FAR * This,
    /* [in] */ long lIndex,
    /* [out] */ TunerInputType __RPC_FAR *pInputType);


void __RPC_STUB IAMTVTuner_get_InputType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMTVTuner_put_ConnectInput_Proxy( 
    IAMTVTuner __RPC_FAR * This,
    /* [in] */ long lIndex);


void __RPC_STUB IAMTVTuner_put_ConnectInput_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMTVTuner_get_ConnectInput_Proxy( 
    IAMTVTuner __RPC_FAR * This,
    /* [out] */ long __RPC_FAR *plIndex);


void __RPC_STUB IAMTVTuner_get_ConnectInput_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMTVTuner_get_VideoFrequency_Proxy( 
    IAMTVTuner __RPC_FAR * This,
    /* [out] */ long __RPC_FAR *lFreq);


void __RPC_STUB IAMTVTuner_get_VideoFrequency_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMTVTuner_get_AudioFrequency_Proxy( 
    IAMTVTuner __RPC_FAR * This,
    /* [out] */ long __RPC_FAR *lFreq);


void __RPC_STUB IAMTVTuner_get_AudioFrequency_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAMTVTuner_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_strmif_0114 */
/* [local] */ 

typedef /* [public] */ 
enum __MIDL___MIDL_itf_strmif_0114_0001
    {	AMPROPERTY_PIN_CATEGORY	= 0,
	AMPROPERTY_PIN_MEDIUM	= AMPROPERTY_PIN_CATEGORY + 1
    }	AMPROPERTY_PIN;

#ifndef _IKsPropertySet_
#define _IKsPropertySet_
#define KSPROPERTY_SUPPORT_GET  1
#define KSPROPERTY_SUPPORT_SET  2


extern RPC_IF_HANDLE __MIDL_itf_strmif_0114_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_strmif_0114_v0_0_s_ifspec;

#ifndef __IKsPropertySet_INTERFACE_DEFINED__
#define __IKsPropertySet_INTERFACE_DEFINED__

/* interface IKsPropertySet */
/* [unique][uuid][local][object] */ 


EXTERN_C const IID IID_IKsPropertySet;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("31EFAC30-515C-11d0-A9AA-00AA0061BE93")
    IKsPropertySet : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Set( 
            /* [in] */ REFGUID guidPropSet,
            /* [in] */ DWORD dwPropID,
            /* [size_is][out] */ LPVOID pInstanceData,
            /* [in] */ DWORD cbInstanceData,
            /* [size_is][out] */ LPVOID pPropData,
            /* [in] */ DWORD cbPropData) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Get( 
            /* [in] */ REFGUID guidPropSet,
            /* [in] */ DWORD dwPropID,
            /* [size_is][out] */ LPVOID pInstanceData,
            /* [in] */ DWORD cbInstanceData,
            /* [size_is][out] */ LPVOID pPropData,
            /* [in] */ DWORD cbPropData,
            /* [out] */ DWORD __RPC_FAR *pcbReturned) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE QuerySupported( 
            /* [in] */ REFGUID guidPropSet,
            /* [in] */ DWORD dwPropID,
            /* [out] */ DWORD __RPC_FAR *pTypeSupport) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IKsPropertySetVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IKsPropertySet __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IKsPropertySet __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IKsPropertySet __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Set )( 
            IKsPropertySet __RPC_FAR * This,
            /* [in] */ REFGUID guidPropSet,
            /* [in] */ DWORD dwPropID,
            /* [size_is][out] */ LPVOID pInstanceData,
            /* [in] */ DWORD cbInstanceData,
            /* [size_is][out] */ LPVOID pPropData,
            /* [in] */ DWORD cbPropData);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Get )( 
            IKsPropertySet __RPC_FAR * This,
            /* [in] */ REFGUID guidPropSet,
            /* [in] */ DWORD dwPropID,
            /* [size_is][out] */ LPVOID pInstanceData,
            /* [in] */ DWORD cbInstanceData,
            /* [size_is][out] */ LPVOID pPropData,
            /* [in] */ DWORD cbPropData,
            /* [out] */ DWORD __RPC_FAR *pcbReturned);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QuerySupported )( 
            IKsPropertySet __RPC_FAR * This,
            /* [in] */ REFGUID guidPropSet,
            /* [in] */ DWORD dwPropID,
            /* [out] */ DWORD __RPC_FAR *pTypeSupport);
        
        END_INTERFACE
    } IKsPropertySetVtbl;

    interface IKsPropertySet
    {
        CONST_VTBL struct IKsPropertySetVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IKsPropertySet_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IKsPropertySet_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IKsPropertySet_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IKsPropertySet_Set(This,guidPropSet,dwPropID,pInstanceData,cbInstanceData,pPropData,cbPropData)	\
    (This)->lpVtbl -> Set(This,guidPropSet,dwPropID,pInstanceData,cbInstanceData,pPropData,cbPropData)

#define IKsPropertySet_Get(This,guidPropSet,dwPropID,pInstanceData,cbInstanceData,pPropData,cbPropData,pcbReturned)	\
    (This)->lpVtbl -> Get(This,guidPropSet,dwPropID,pInstanceData,cbInstanceData,pPropData,cbPropData,pcbReturned)

#define IKsPropertySet_QuerySupported(This,guidPropSet,dwPropID,pTypeSupport)	\
    (This)->lpVtbl -> QuerySupported(This,guidPropSet,dwPropID,pTypeSupport)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IKsPropertySet_Set_Proxy( 
    IKsPropertySet __RPC_FAR * This,
    /* [in] */ REFGUID guidPropSet,
    /* [in] */ DWORD dwPropID,
    /* [size_is][out] */ LPVOID pInstanceData,
    /* [in] */ DWORD cbInstanceData,
    /* [size_is][out] */ LPVOID pPropData,
    /* [in] */ DWORD cbPropData);


void __RPC_STUB IKsPropertySet_Set_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IKsPropertySet_Get_Proxy( 
    IKsPropertySet __RPC_FAR * This,
    /* [in] */ REFGUID guidPropSet,
    /* [in] */ DWORD dwPropID,
    /* [size_is][out] */ LPVOID pInstanceData,
    /* [in] */ DWORD cbInstanceData,
    /* [size_is][out] */ LPVOID pPropData,
    /* [in] */ DWORD cbPropData,
    /* [out] */ DWORD __RPC_FAR *pcbReturned);


void __RPC_STUB IKsPropertySet_Get_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IKsPropertySet_QuerySupported_Proxy( 
    IKsPropertySet __RPC_FAR * This,
    /* [in] */ REFGUID guidPropSet,
    /* [in] */ DWORD dwPropID,
    /* [out] */ DWORD __RPC_FAR *pTypeSupport);


void __RPC_STUB IKsPropertySet_QuerySupported_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IKsPropertySet_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_strmif_0115 */
/* [local] */ 

#endif // _IKsPropertySet_


extern RPC_IF_HANDLE __MIDL_itf_strmif_0115_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_strmif_0115_v0_0_s_ifspec;

#ifndef __IMediaPropertyBag_INTERFACE_DEFINED__
#define __IMediaPropertyBag_INTERFACE_DEFINED__

/* interface IMediaPropertyBag */
/* [unique][uuid][object] */ 

typedef IMediaPropertyBag __RPC_FAR *LPMEDIAPROPERTYBAG;


EXTERN_C const IID IID_IMediaPropertyBag;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6025A880-C0D5-11d0-BD4E-00A0C911CE86")
    IMediaPropertyBag : public IPropertyBag
    {
    public:
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE EnumProperty( 
            /* [in] */ ULONG iProperty,
            /* [out][in] */ VARIANT __RPC_FAR *pvarPropertyName,
            /* [out][in] */ VARIANT __RPC_FAR *pvarPropertyValue) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMediaPropertyBagVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IMediaPropertyBag __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IMediaPropertyBag __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IMediaPropertyBag __RPC_FAR * This);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Read )( 
            IMediaPropertyBag __RPC_FAR * This,
            /* [in] */ LPCOLESTR pszPropName,
            /* [out][in] */ VARIANT __RPC_FAR *pVar,
            /* [in] */ IErrorLog __RPC_FAR *pErrorLog);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Write )( 
            IMediaPropertyBag __RPC_FAR * This,
            /* [in] */ LPCOLESTR pszPropName,
            /* [in] */ VARIANT __RPC_FAR *pVar);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *EnumProperty )( 
            IMediaPropertyBag __RPC_FAR * This,
            /* [in] */ ULONG iProperty,
            /* [out][in] */ VARIANT __RPC_FAR *pvarPropertyName,
            /* [out][in] */ VARIANT __RPC_FAR *pvarPropertyValue);
        
        END_INTERFACE
    } IMediaPropertyBagVtbl;

    interface IMediaPropertyBag
    {
        CONST_VTBL struct IMediaPropertyBagVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMediaPropertyBag_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IMediaPropertyBag_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IMediaPropertyBag_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IMediaPropertyBag_Read(This,pszPropName,pVar,pErrorLog)	\
    (This)->lpVtbl -> Read(This,pszPropName,pVar,pErrorLog)

#define IMediaPropertyBag_Write(This,pszPropName,pVar)	\
    (This)->lpVtbl -> Write(This,pszPropName,pVar)


#define IMediaPropertyBag_EnumProperty(This,iProperty,pvarPropertyName,pvarPropertyValue)	\
    (This)->lpVtbl -> EnumProperty(This,iProperty,pvarPropertyName,pvarPropertyValue)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [local] */ HRESULT STDMETHODCALLTYPE IMediaPropertyBag_EnumProperty_Proxy( 
    IMediaPropertyBag __RPC_FAR * This,
    /* [in] */ ULONG iProperty,
    /* [out][in] */ VARIANT __RPC_FAR *pvarPropertyName,
    /* [out][in] */ VARIANT __RPC_FAR *pvarPropertyValue);


void __RPC_STUB IMediaPropertyBag_EnumProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IMediaPropertyBag_INTERFACE_DEFINED__ */


#ifndef __IPersistMediaPropertyBag_INTERFACE_DEFINED__
#define __IPersistMediaPropertyBag_INTERFACE_DEFINED__

/* interface IPersistMediaPropertyBag */
/* [unique][uuid][object] */ 

typedef IPersistMediaPropertyBag __RPC_FAR *LPPERSISTMEDIAPROPERTYBAG;


EXTERN_C const IID IID_IPersistMediaPropertyBag;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("5738E040-B67F-11d0-BD4D-00A0C911CE86")
    IPersistMediaPropertyBag : public IPersist
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE InitNew( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Load( 
            /* [in] */ IMediaPropertyBag __RPC_FAR *pPropBag,
            /* [in] */ IErrorLog __RPC_FAR *pErrorLog) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Save( 
            /* [in] */ IMediaPropertyBag __RPC_FAR *pPropBag,
            /* [in] */ BOOL fClearDirty,
            /* [in] */ BOOL fSaveAllProperties) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPersistMediaPropertyBagVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IPersistMediaPropertyBag __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IPersistMediaPropertyBag __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IPersistMediaPropertyBag __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetClassID )( 
            IPersistMediaPropertyBag __RPC_FAR * This,
            /* [out] */ CLSID __RPC_FAR *pClassID);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InitNew )( 
            IPersistMediaPropertyBag __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Load )( 
            IPersistMediaPropertyBag __RPC_FAR * This,
            /* [in] */ IMediaPropertyBag __RPC_FAR *pPropBag,
            /* [in] */ IErrorLog __RPC_FAR *pErrorLog);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Save )( 
            IPersistMediaPropertyBag __RPC_FAR * This,
            /* [in] */ IMediaPropertyBag __RPC_FAR *pPropBag,
            /* [in] */ BOOL fClearDirty,
            /* [in] */ BOOL fSaveAllProperties);
        
        END_INTERFACE
    } IPersistMediaPropertyBagVtbl;

    interface IPersistMediaPropertyBag
    {
        CONST_VTBL struct IPersistMediaPropertyBagVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPersistMediaPropertyBag_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPersistMediaPropertyBag_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPersistMediaPropertyBag_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPersistMediaPropertyBag_GetClassID(This,pClassID)	\
    (This)->lpVtbl -> GetClassID(This,pClassID)


#define IPersistMediaPropertyBag_InitNew(This)	\
    (This)->lpVtbl -> InitNew(This)

#define IPersistMediaPropertyBag_Load(This,pPropBag,pErrorLog)	\
    (This)->lpVtbl -> Load(This,pPropBag,pErrorLog)

#define IPersistMediaPropertyBag_Save(This,pPropBag,fClearDirty,fSaveAllProperties)	\
    (This)->lpVtbl -> Save(This,pPropBag,fClearDirty,fSaveAllProperties)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IPersistMediaPropertyBag_InitNew_Proxy( 
    IPersistMediaPropertyBag __RPC_FAR * This);


void __RPC_STUB IPersistMediaPropertyBag_InitNew_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPersistMediaPropertyBag_Load_Proxy( 
    IPersistMediaPropertyBag __RPC_FAR * This,
    /* [in] */ IMediaPropertyBag __RPC_FAR *pPropBag,
    /* [in] */ IErrorLog __RPC_FAR *pErrorLog);


void __RPC_STUB IPersistMediaPropertyBag_Load_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPersistMediaPropertyBag_Save_Proxy( 
    IPersistMediaPropertyBag __RPC_FAR * This,
    /* [in] */ IMediaPropertyBag __RPC_FAR *pPropBag,
    /* [in] */ BOOL fClearDirty,
    /* [in] */ BOOL fSaveAllProperties);


void __RPC_STUB IPersistMediaPropertyBag_Save_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPersistMediaPropertyBag_INTERFACE_DEFINED__ */


#ifndef __IAMPhysicalPinInfo_INTERFACE_DEFINED__
#define __IAMPhysicalPinInfo_INTERFACE_DEFINED__

/* interface IAMPhysicalPinInfo */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IAMPhysicalPinInfo;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("F938C991-3029-11cf-8C44-00AA006B6814")
    IAMPhysicalPinInfo : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetPhysicalType( 
            /* [out] */ long __RPC_FAR *pType,
            /* [out] */ LPOLESTR __RPC_FAR *ppszType) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAMPhysicalPinInfoVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IAMPhysicalPinInfo __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IAMPhysicalPinInfo __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IAMPhysicalPinInfo __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetPhysicalType )( 
            IAMPhysicalPinInfo __RPC_FAR * This,
            /* [out] */ long __RPC_FAR *pType,
            /* [out] */ LPOLESTR __RPC_FAR *ppszType);
        
        END_INTERFACE
    } IAMPhysicalPinInfoVtbl;

    interface IAMPhysicalPinInfo
    {
        CONST_VTBL struct IAMPhysicalPinInfoVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAMPhysicalPinInfo_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAMPhysicalPinInfo_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAMPhysicalPinInfo_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IAMPhysicalPinInfo_GetPhysicalType(This,pType,ppszType)	\
    (This)->lpVtbl -> GetPhysicalType(This,pType,ppszType)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IAMPhysicalPinInfo_GetPhysicalType_Proxy( 
    IAMPhysicalPinInfo __RPC_FAR * This,
    /* [out] */ long __RPC_FAR *pType,
    /* [out] */ LPOLESTR __RPC_FAR *ppszType);


void __RPC_STUB IAMPhysicalPinInfo_GetPhysicalType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAMPhysicalPinInfo_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_strmif_0210 */
/* [local] */ 

typedef IAMPhysicalPinInfo __RPC_FAR *PAMPHYSICALPININFO;



extern RPC_IF_HANDLE __MIDL_itf_strmif_0210_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_strmif_0210_v0_0_s_ifspec;

#ifndef __IAMExtDevice_INTERFACE_DEFINED__
#define __IAMExtDevice_INTERFACE_DEFINED__

/* interface IAMExtDevice */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IAMExtDevice;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("B5730A90-1A2C-11cf-8C23-00AA006B6814")
    IAMExtDevice : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetCapability( 
            /* [in] */ long Capability,
            /* [out] */ long __RPC_FAR *pValue,
            /* [out] */ double __RPC_FAR *pdblValue) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get_ExternalDeviceID( 
            /* [out] */ LPOLESTR __RPC_FAR *ppszData) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get_ExternalDeviceVersion( 
            /* [out] */ LPOLESTR __RPC_FAR *ppszData) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE put_DevicePower( 
            /* [in] */ long PowerMode) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get_DevicePower( 
            /* [out] */ long __RPC_FAR *pPowerMode) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Calibrate( 
            /* [in] */ HEVENT hEvent,
            /* [in] */ long Mode,
            /* [out] */ long __RPC_FAR *pStatus) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE put_DevicePort( 
            /* [in] */ long DevicePort) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get_DevicePort( 
            /* [out] */ long __RPC_FAR *pDevicePort) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAMExtDeviceVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IAMExtDevice __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IAMExtDevice __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IAMExtDevice __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetCapability )( 
            IAMExtDevice __RPC_FAR * This,
            /* [in] */ long Capability,
            /* [out] */ long __RPC_FAR *pValue,
            /* [out] */ double __RPC_FAR *pdblValue);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ExternalDeviceID )( 
            IAMExtDevice __RPC_FAR * This,
            /* [out] */ LPOLESTR __RPC_FAR *ppszData);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ExternalDeviceVersion )( 
            IAMExtDevice __RPC_FAR * This,
            /* [out] */ LPOLESTR __RPC_FAR *ppszData);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_DevicePower )( 
            IAMExtDevice __RPC_FAR * This,
            /* [in] */ long PowerMode);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_DevicePower )( 
            IAMExtDevice __RPC_FAR * This,
            /* [out] */ long __RPC_FAR *pPowerMode);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Calibrate )( 
            IAMExtDevice __RPC_FAR * This,
            /* [in] */ HEVENT hEvent,
            /* [in] */ long Mode,
            /* [out] */ long __RPC_FAR *pStatus);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_DevicePort )( 
            IAMExtDevice __RPC_FAR * This,
            /* [in] */ long DevicePort);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_DevicePort )( 
            IAMExtDevice __RPC_FAR * This,
            /* [out] */ long __RPC_FAR *pDevicePort);
        
        END_INTERFACE
    } IAMExtDeviceVtbl;

    interface IAMExtDevice
    {
        CONST_VTBL struct IAMExtDeviceVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAMExtDevice_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAMExtDevice_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAMExtDevice_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IAMExtDevice_GetCapability(This,Capability,pValue,pdblValue)	\
    (This)->lpVtbl -> GetCapability(This,Capability,pValue,pdblValue)

#define IAMExtDevice_get_ExternalDeviceID(This,ppszData)	\
    (This)->lpVtbl -> get_ExternalDeviceID(This,ppszData)

#define IAMExtDevice_get_ExternalDeviceVersion(This,ppszData)	\
    (This)->lpVtbl -> get_ExternalDeviceVersion(This,ppszData)

#define IAMExtDevice_put_DevicePower(This,PowerMode)	\
    (This)->lpVtbl -> put_DevicePower(This,PowerMode)

#define IAMExtDevice_get_DevicePower(This,pPowerMode)	\
    (This)->lpVtbl -> get_DevicePower(This,pPowerMode)

#define IAMExtDevice_Calibrate(This,hEvent,Mode,pStatus)	\
    (This)->lpVtbl -> Calibrate(This,hEvent,Mode,pStatus)

#define IAMExtDevice_put_DevicePort(This,DevicePort)	\
    (This)->lpVtbl -> put_DevicePort(This,DevicePort)

#define IAMExtDevice_get_DevicePort(This,pDevicePort)	\
    (This)->lpVtbl -> get_DevicePort(This,pDevicePort)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IAMExtDevice_GetCapability_Proxy( 
    IAMExtDevice __RPC_FAR * This,
    /* [in] */ long Capability,
    /* [out] */ long __RPC_FAR *pValue,
    /* [out] */ double __RPC_FAR *pdblValue);


void __RPC_STUB IAMExtDevice_GetCapability_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMExtDevice_get_ExternalDeviceID_Proxy( 
    IAMExtDevice __RPC_FAR * This,
    /* [out] */ LPOLESTR __RPC_FAR *ppszData);


void __RPC_STUB IAMExtDevice_get_ExternalDeviceID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMExtDevice_get_ExternalDeviceVersion_Proxy( 
    IAMExtDevice __RPC_FAR * This,
    /* [out] */ LPOLESTR __RPC_FAR *ppszData);


void __RPC_STUB IAMExtDevice_get_ExternalDeviceVersion_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMExtDevice_put_DevicePower_Proxy( 
    IAMExtDevice __RPC_FAR * This,
    /* [in] */ long PowerMode);


void __RPC_STUB IAMExtDevice_put_DevicePower_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMExtDevice_get_DevicePower_Proxy( 
    IAMExtDevice __RPC_FAR * This,
    /* [out] */ long __RPC_FAR *pPowerMode);


void __RPC_STUB IAMExtDevice_get_DevicePower_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMExtDevice_Calibrate_Proxy( 
    IAMExtDevice __RPC_FAR * This,
    /* [in] */ HEVENT hEvent,
    /* [in] */ long Mode,
    /* [out] */ long __RPC_FAR *pStatus);


void __RPC_STUB IAMExtDevice_Calibrate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMExtDevice_put_DevicePort_Proxy( 
    IAMExtDevice __RPC_FAR * This,
    /* [in] */ long DevicePort);


void __RPC_STUB IAMExtDevice_put_DevicePort_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMExtDevice_get_DevicePort_Proxy( 
    IAMExtDevice __RPC_FAR * This,
    /* [out] */ long __RPC_FAR *pDevicePort);


void __RPC_STUB IAMExtDevice_get_DevicePort_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAMExtDevice_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_strmif_0211 */
/* [local] */ 

typedef IAMExtDevice __RPC_FAR *PEXTDEVICE;



extern RPC_IF_HANDLE __MIDL_itf_strmif_0211_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_strmif_0211_v0_0_s_ifspec;

#ifndef __IAMExtTransport_INTERFACE_DEFINED__
#define __IAMExtTransport_INTERFACE_DEFINED__

/* interface IAMExtTransport */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IAMExtTransport;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("A03CD5F0-3045-11cf-8C44-00AA006B6814")
    IAMExtTransport : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetCapability( 
            /* [in] */ long Capability,
            /* [out] */ long __RPC_FAR *pValue,
            /* [out] */ double __RPC_FAR *pdblValue) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE put_MediaState( 
            /* [in] */ long State) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get_MediaState( 
            /* [out] */ long __RPC_FAR *pState) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE put_LocalControl( 
            /* [in] */ long State) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get_LocalControl( 
            /* [out] */ long __RPC_FAR *pState) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetStatus( 
            /* [in] */ long StatusItem,
            /* [out] */ long __RPC_FAR *pValue) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetTransportBasicParameters( 
            /* [in] */ long Param,
            /* [out] */ long __RPC_FAR *pValue,
            /* [out] */ LPOLESTR __RPC_FAR *ppszData) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetTransportBasicParameters( 
            /* [in] */ long Param,
            /* [in] */ long Value,
            /* [in] */ LPCOLESTR pszData) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetTransportVideoParameters( 
            /* [in] */ long Param,
            /* [out] */ long __RPC_FAR *pValue) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetTransportVideoParameters( 
            /* [in] */ long Param,
            /* [in] */ long Value) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetTransportAudioParameters( 
            /* [in] */ long Param,
            /* [out] */ long __RPC_FAR *pValue) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetTransportAudioParameters( 
            /* [in] */ long Param,
            /* [in] */ long Value) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE put_Mode( 
            /* [in] */ long Mode) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get_Mode( 
            /* [out] */ long __RPC_FAR *pMode) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE put_Rate( 
            /* [in] */ double dblRate) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get_Rate( 
            /* [out] */ double __RPC_FAR *pdblRate) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetChase( 
            /* [out] */ long __RPC_FAR *pEnabled,
            /* [out] */ long __RPC_FAR *pOffset,
            /* [out] */ HEVENT __RPC_FAR *phEvent) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetChase( 
            /* [in] */ long Enable,
            /* [in] */ long Offset,
            /* [in] */ HEVENT hEvent) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetBump( 
            /* [out] */ long __RPC_FAR *pSpeed,
            /* [out] */ long __RPC_FAR *pDuration) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetBump( 
            /* [in] */ long Speed,
            /* [in] */ long Duration) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get_AntiClogControl( 
            /* [out] */ long __RPC_FAR *pEnabled) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE put_AntiClogControl( 
            /* [in] */ long Enable) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetEditPropertySet( 
            /* [in] */ long EditID,
            /* [out] */ long __RPC_FAR *pState) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetEditPropertySet( 
            /* [out][in] */ long __RPC_FAR *pEditID,
            /* [in] */ long State) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetEditProperty( 
            /* [in] */ long EditID,
            /* [in] */ long Param,
            /* [out] */ long __RPC_FAR *pValue) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetEditProperty( 
            /* [in] */ long EditID,
            /* [in] */ long Param,
            /* [in] */ long Value) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get_EditStart( 
            /* [out] */ long __RPC_FAR *pValue) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE put_EditStart( 
            /* [in] */ long Value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAMExtTransportVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IAMExtTransport __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IAMExtTransport __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IAMExtTransport __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetCapability )( 
            IAMExtTransport __RPC_FAR * This,
            /* [in] */ long Capability,
            /* [out] */ long __RPC_FAR *pValue,
            /* [out] */ double __RPC_FAR *pdblValue);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_MediaState )( 
            IAMExtTransport __RPC_FAR * This,
            /* [in] */ long State);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_MediaState )( 
            IAMExtTransport __RPC_FAR * This,
            /* [out] */ long __RPC_FAR *pState);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_LocalControl )( 
            IAMExtTransport __RPC_FAR * This,
            /* [in] */ long State);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_LocalControl )( 
            IAMExtTransport __RPC_FAR * This,
            /* [out] */ long __RPC_FAR *pState);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetStatus )( 
            IAMExtTransport __RPC_FAR * This,
            /* [in] */ long StatusItem,
            /* [out] */ long __RPC_FAR *pValue);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTransportBasicParameters )( 
            IAMExtTransport __RPC_FAR * This,
            /* [in] */ long Param,
            /* [out] */ long __RPC_FAR *pValue,
            /* [out] */ LPOLESTR __RPC_FAR *ppszData);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetTransportBasicParameters )( 
            IAMExtTransport __RPC_FAR * This,
            /* [in] */ long Param,
            /* [in] */ long Value,
            /* [in] */ LPCOLESTR pszData);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTransportVideoParameters )( 
            IAMExtTransport __RPC_FAR * This,
            /* [in] */ long Param,
            /* [out] */ long __RPC_FAR *pValue);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetTransportVideoParameters )( 
            IAMExtTransport __RPC_FAR * This,
            /* [in] */ long Param,
            /* [in] */ long Value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTransportAudioParameters )( 
            IAMExtTransport __RPC_FAR * This,
            /* [in] */ long Param,
            /* [out] */ long __RPC_FAR *pValue);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetTransportAudioParameters )( 
            IAMExtTransport __RPC_FAR * This,
            /* [in] */ long Param,
            /* [in] */ long Value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Mode )( 
            IAMExtTransport __RPC_FAR * This,
            /* [in] */ long Mode);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Mode )( 
            IAMExtTransport __RPC_FAR * This,
            /* [out] */ long __RPC_FAR *pMode);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Rate )( 
            IAMExtTransport __RPC_FAR * This,
            /* [in] */ double dblRate);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Rate )( 
            IAMExtTransport __RPC_FAR * This,
            /* [out] */ double __RPC_FAR *pdblRate);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetChase )( 
            IAMExtTransport __RPC_FAR * This,
            /* [out] */ long __RPC_FAR *pEnabled,
            /* [out] */ long __RPC_FAR *pOffset,
            /* [out] */ HEVENT __RPC_FAR *phEvent);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetChase )( 
            IAMExtTransport __RPC_FAR * This,
            /* [in] */ long Enable,
            /* [in] */ long Offset,
            /* [in] */ HEVENT hEvent);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetBump )( 
            IAMExtTransport __RPC_FAR * This,
            /* [out] */ long __RPC_FAR *pSpeed,
            /* [out] */ long __RPC_FAR *pDuration);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetBump )( 
            IAMExtTransport __RPC_FAR * This,
            /* [in] */ long Speed,
            /* [in] */ long Duration);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_AntiClogControl )( 
            IAMExtTransport __RPC_FAR * This,
            /* [out] */ long __RPC_FAR *pEnabled);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_AntiClogControl )( 
            IAMExtTransport __RPC_FAR * This,
            /* [in] */ long Enable);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetEditPropertySet )( 
            IAMExtTransport __RPC_FAR * This,
            /* [in] */ long EditID,
            /* [out] */ long __RPC_FAR *pState);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetEditPropertySet )( 
            IAMExtTransport __RPC_FAR * This,
            /* [out][in] */ long __RPC_FAR *pEditID,
            /* [in] */ long State);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetEditProperty )( 
            IAMExtTransport __RPC_FAR * This,
            /* [in] */ long EditID,
            /* [in] */ long Param,
            /* [out] */ long __RPC_FAR *pValue);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetEditProperty )( 
            IAMExtTransport __RPC_FAR * This,
            /* [in] */ long EditID,
            /* [in] */ long Param,
            /* [in] */ long Value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_EditStart )( 
            IAMExtTransport __RPC_FAR * This,
            /* [out] */ long __RPC_FAR *pValue);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_EditStart )( 
            IAMExtTransport __RPC_FAR * This,
            /* [in] */ long Value);
        
        END_INTERFACE
    } IAMExtTransportVtbl;

    interface IAMExtTransport
    {
        CONST_VTBL struct IAMExtTransportVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAMExtTransport_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAMExtTransport_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAMExtTransport_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IAMExtTransport_GetCapability(This,Capability,pValue,pdblValue)	\
    (This)->lpVtbl -> GetCapability(This,Capability,pValue,pdblValue)

#define IAMExtTransport_put_MediaState(This,State)	\
    (This)->lpVtbl -> put_MediaState(This,State)

#define IAMExtTransport_get_MediaState(This,pState)	\
    (This)->lpVtbl -> get_MediaState(This,pState)

#define IAMExtTransport_put_LocalControl(This,State)	\
    (This)->lpVtbl -> put_LocalControl(This,State)

#define IAMExtTransport_get_LocalControl(This,pState)	\
    (This)->lpVtbl -> get_LocalControl(This,pState)

#define IAMExtTransport_GetStatus(This,StatusItem,pValue)	\
    (This)->lpVtbl -> GetStatus(This,StatusItem,pValue)

#define IAMExtTransport_GetTransportBasicParameters(This,Param,pValue,ppszData)	\
    (This)->lpVtbl -> GetTransportBasicParameters(This,Param,pValue,ppszData)

#define IAMExtTransport_SetTransportBasicParameters(This,Param,Value,pszData)	\
    (This)->lpVtbl -> SetTransportBasicParameters(This,Param,Value,pszData)

#define IAMExtTransport_GetTransportVideoParameters(This,Param,pValue)	\
    (This)->lpVtbl -> GetTransportVideoParameters(This,Param,pValue)

#define IAMExtTransport_SetTransportVideoParameters(This,Param,Value)	\
    (This)->lpVtbl -> SetTransportVideoParameters(This,Param,Value)

#define IAMExtTransport_GetTransportAudioParameters(This,Param,pValue)	\
    (This)->lpVtbl -> GetTransportAudioParameters(This,Param,pValue)

#define IAMExtTransport_SetTransportAudioParameters(This,Param,Value)	\
    (This)->lpVtbl -> SetTransportAudioParameters(This,Param,Value)

#define IAMExtTransport_put_Mode(This,Mode)	\
    (This)->lpVtbl -> put_Mode(This,Mode)

#define IAMExtTransport_get_Mode(This,pMode)	\
    (This)->lpVtbl -> get_Mode(This,pMode)

#define IAMExtTransport_put_Rate(This,dblRate)	\
    (This)->lpVtbl -> put_Rate(This,dblRate)

#define IAMExtTransport_get_Rate(This,pdblRate)	\
    (This)->lpVtbl -> get_Rate(This,pdblRate)

#define IAMExtTransport_GetChase(This,pEnabled,pOffset,phEvent)	\
    (This)->lpVtbl -> GetChase(This,pEnabled,pOffset,phEvent)

#define IAMExtTransport_SetChase(This,Enable,Offset,hEvent)	\
    (This)->lpVtbl -> SetChase(This,Enable,Offset,hEvent)

#define IAMExtTransport_GetBump(This,pSpeed,pDuration)	\
    (This)->lpVtbl -> GetBump(This,pSpeed,pDuration)

#define IAMExtTransport_SetBump(This,Speed,Duration)	\
    (This)->lpVtbl -> SetBump(This,Speed,Duration)

#define IAMExtTransport_get_AntiClogControl(This,pEnabled)	\
    (This)->lpVtbl -> get_AntiClogControl(This,pEnabled)

#define IAMExtTransport_put_AntiClogControl(This,Enable)	\
    (This)->lpVtbl -> put_AntiClogControl(This,Enable)

#define IAMExtTransport_GetEditPropertySet(This,EditID,pState)	\
    (This)->lpVtbl -> GetEditPropertySet(This,EditID,pState)

#define IAMExtTransport_SetEditPropertySet(This,pEditID,State)	\
    (This)->lpVtbl -> SetEditPropertySet(This,pEditID,State)

#define IAMExtTransport_GetEditProperty(This,EditID,Param,pValue)	\
    (This)->lpVtbl -> GetEditProperty(This,EditID,Param,pValue)

#define IAMExtTransport_SetEditProperty(This,EditID,Param,Value)	\
    (This)->lpVtbl -> SetEditProperty(This,EditID,Param,Value)

#define IAMExtTransport_get_EditStart(This,pValue)	\
    (This)->lpVtbl -> get_EditStart(This,pValue)

#define IAMExtTransport_put_EditStart(This,Value)	\
    (This)->lpVtbl -> put_EditStart(This,Value)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IAMExtTransport_GetCapability_Proxy( 
    IAMExtTransport __RPC_FAR * This,
    /* [in] */ long Capability,
    /* [out] */ long __RPC_FAR *pValue,
    /* [out] */ double __RPC_FAR *pdblValue);


void __RPC_STUB IAMExtTransport_GetCapability_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMExtTransport_put_MediaState_Proxy( 
    IAMExtTransport __RPC_FAR * This,
    /* [in] */ long State);


void __RPC_STUB IAMExtTransport_put_MediaState_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMExtTransport_get_MediaState_Proxy( 
    IAMExtTransport __RPC_FAR * This,
    /* [out] */ long __RPC_FAR *pState);


void __RPC_STUB IAMExtTransport_get_MediaState_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMExtTransport_put_LocalControl_Proxy( 
    IAMExtTransport __RPC_FAR * This,
    /* [in] */ long State);


void __RPC_STUB IAMExtTransport_put_LocalControl_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMExtTransport_get_LocalControl_Proxy( 
    IAMExtTransport __RPC_FAR * This,
    /* [out] */ long __RPC_FAR *pState);


void __RPC_STUB IAMExtTransport_get_LocalControl_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMExtTransport_GetStatus_Proxy( 
    IAMExtTransport __RPC_FAR * This,
    /* [in] */ long StatusItem,
    /* [out] */ long __RPC_FAR *pValue);


void __RPC_STUB IAMExtTransport_GetStatus_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMExtTransport_GetTransportBasicParameters_Proxy( 
    IAMExtTransport __RPC_FAR * This,
    /* [in] */ long Param,
    /* [out] */ long __RPC_FAR *pValue,
    /* [out] */ LPOLESTR __RPC_FAR *ppszData);


void __RPC_STUB IAMExtTransport_GetTransportBasicParameters_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMExtTransport_SetTransportBasicParameters_Proxy( 
    IAMExtTransport __RPC_FAR * This,
    /* [in] */ long Param,
    /* [in] */ long Value,
    /* [in] */ LPCOLESTR pszData);


void __RPC_STUB IAMExtTransport_SetTransportBasicParameters_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMExtTransport_GetTransportVideoParameters_Proxy( 
    IAMExtTransport __RPC_FAR * This,
    /* [in] */ long Param,
    /* [out] */ long __RPC_FAR *pValue);


void __RPC_STUB IAMExtTransport_GetTransportVideoParameters_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMExtTransport_SetTransportVideoParameters_Proxy( 
    IAMExtTransport __RPC_FAR * This,
    /* [in] */ long Param,
    /* [in] */ long Value);


void __RPC_STUB IAMExtTransport_SetTransportVideoParameters_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMExtTransport_GetTransportAudioParameters_Proxy( 
    IAMExtTransport __RPC_FAR * This,
    /* [in] */ long Param,
    /* [out] */ long __RPC_FAR *pValue);


void __RPC_STUB IAMExtTransport_GetTransportAudioParameters_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMExtTransport_SetTransportAudioParameters_Proxy( 
    IAMExtTransport __RPC_FAR * This,
    /* [in] */ long Param,
    /* [in] */ long Value);


void __RPC_STUB IAMExtTransport_SetTransportAudioParameters_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMExtTransport_put_Mode_Proxy( 
    IAMExtTransport __RPC_FAR * This,
    /* [in] */ long Mode);


void __RPC_STUB IAMExtTransport_put_Mode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMExtTransport_get_Mode_Proxy( 
    IAMExtTransport __RPC_FAR * This,
    /* [out] */ long __RPC_FAR *pMode);


void __RPC_STUB IAMExtTransport_get_Mode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMExtTransport_put_Rate_Proxy( 
    IAMExtTransport __RPC_FAR * This,
    /* [in] */ double dblRate);


void __RPC_STUB IAMExtTransport_put_Rate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMExtTransport_get_Rate_Proxy( 
    IAMExtTransport __RPC_FAR * This,
    /* [out] */ double __RPC_FAR *pdblRate);


void __RPC_STUB IAMExtTransport_get_Rate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMExtTransport_GetChase_Proxy( 
    IAMExtTransport __RPC_FAR * This,
    /* [out] */ long __RPC_FAR *pEnabled,
    /* [out] */ long __RPC_FAR *pOffset,
    /* [out] */ HEVENT __RPC_FAR *phEvent);


void __RPC_STUB IAMExtTransport_GetChase_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMExtTransport_SetChase_Proxy( 
    IAMExtTransport __RPC_FAR * This,
    /* [in] */ long Enable,
    /* [in] */ long Offset,
    /* [in] */ HEVENT hEvent);


void __RPC_STUB IAMExtTransport_SetChase_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMExtTransport_GetBump_Proxy( 
    IAMExtTransport __RPC_FAR * This,
    /* [out] */ long __RPC_FAR *pSpeed,
    /* [out] */ long __RPC_FAR *pDuration);


void __RPC_STUB IAMExtTransport_GetBump_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMExtTransport_SetBump_Proxy( 
    IAMExtTransport __RPC_FAR * This,
    /* [in] */ long Speed,
    /* [in] */ long Duration);


void __RPC_STUB IAMExtTransport_SetBump_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMExtTransport_get_AntiClogControl_Proxy( 
    IAMExtTransport __RPC_FAR * This,
    /* [out] */ long __RPC_FAR *pEnabled);


void __RPC_STUB IAMExtTransport_get_AntiClogControl_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMExtTransport_put_AntiClogControl_Proxy( 
    IAMExtTransport __RPC_FAR * This,
    /* [in] */ long Enable);


void __RPC_STUB IAMExtTransport_put_AntiClogControl_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMExtTransport_GetEditPropertySet_Proxy( 
    IAMExtTransport __RPC_FAR * This,
    /* [in] */ long EditID,
    /* [out] */ long __RPC_FAR *pState);


void __RPC_STUB IAMExtTransport_GetEditPropertySet_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMExtTransport_SetEditPropertySet_Proxy( 
    IAMExtTransport __RPC_FAR * This,
    /* [out][in] */ long __RPC_FAR *pEditID,
    /* [in] */ long State);


void __RPC_STUB IAMExtTransport_SetEditPropertySet_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMExtTransport_GetEditProperty_Proxy( 
    IAMExtTransport __RPC_FAR * This,
    /* [in] */ long EditID,
    /* [in] */ long Param,
    /* [out] */ long __RPC_FAR *pValue);


void __RPC_STUB IAMExtTransport_GetEditProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMExtTransport_SetEditProperty_Proxy( 
    IAMExtTransport __RPC_FAR * This,
    /* [in] */ long EditID,
    /* [in] */ long Param,
    /* [in] */ long Value);


void __RPC_STUB IAMExtTransport_SetEditProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMExtTransport_get_EditStart_Proxy( 
    IAMExtTransport __RPC_FAR * This,
    /* [out] */ long __RPC_FAR *pValue);


void __RPC_STUB IAMExtTransport_get_EditStart_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMExtTransport_put_EditStart_Proxy( 
    IAMExtTransport __RPC_FAR * This,
    /* [in] */ long Value);


void __RPC_STUB IAMExtTransport_put_EditStart_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAMExtTransport_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_strmif_0212 */
/* [local] */ 

typedef IAMExtTransport __RPC_FAR *PIAMEXTTRANSPORT;

#if 0
/* the following is what MIDL knows how to remote */
typedef struct tagTIMECODE
    {
    WORD wFrameRate;
    WORD wFrameFract;
    DWORD dwFrames;
    }	TIMECODE;

#else /* 0 */
#ifndef TIMECODE_DEFINED
#define TIMECODE_DEFINED
typedef union _timecode {
   struct {
	 WORD   wFrameRate;
	 WORD   wFrameFract;
	 DWORD  dwFrames;
	 };
   DWORDLONG  qw;
   } TIMECODE;

#endif /* TIMECODE_DEFINED */
#endif /* 0 */
typedef TIMECODE __RPC_FAR *PTIMECODE;

typedef struct tagTIMECODE_SAMPLE
    {
    LONGLONG qwTick;
    TIMECODE timecode;
    DWORD dwUser;
    DWORD dwFlags;
    }	TIMECODE_SAMPLE;

typedef TIMECODE_SAMPLE __RPC_FAR *PTIMECODE_SAMPLE;



extern RPC_IF_HANDLE __MIDL_itf_strmif_0212_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_strmif_0212_v0_0_s_ifspec;

#ifndef __IAMTimecodeReader_INTERFACE_DEFINED__
#define __IAMTimecodeReader_INTERFACE_DEFINED__

/* interface IAMTimecodeReader */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IAMTimecodeReader;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("9B496CE1-811B-11cf-8C77-00AA006B6814")
    IAMTimecodeReader : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetTCRMode( 
            /* [in] */ long Param,
            /* [out] */ long __RPC_FAR *pValue) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetTCRMode( 
            /* [in] */ long Param,
            /* [in] */ long Value) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE put_VITCLine( 
            /* [in] */ long Line) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get_VITCLine( 
            /* [out] */ long __RPC_FAR *pLine) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetTimecode( 
            /* [out] */ PTIMECODE_SAMPLE pTimecodeSample) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAMTimecodeReaderVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IAMTimecodeReader __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IAMTimecodeReader __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IAMTimecodeReader __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTCRMode )( 
            IAMTimecodeReader __RPC_FAR * This,
            /* [in] */ long Param,
            /* [out] */ long __RPC_FAR *pValue);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetTCRMode )( 
            IAMTimecodeReader __RPC_FAR * This,
            /* [in] */ long Param,
            /* [in] */ long Value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_VITCLine )( 
            IAMTimecodeReader __RPC_FAR * This,
            /* [in] */ long Line);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_VITCLine )( 
            IAMTimecodeReader __RPC_FAR * This,
            /* [out] */ long __RPC_FAR *pLine);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTimecode )( 
            IAMTimecodeReader __RPC_FAR * This,
            /* [out] */ PTIMECODE_SAMPLE pTimecodeSample);
        
        END_INTERFACE
    } IAMTimecodeReaderVtbl;

    interface IAMTimecodeReader
    {
        CONST_VTBL struct IAMTimecodeReaderVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAMTimecodeReader_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAMTimecodeReader_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAMTimecodeReader_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IAMTimecodeReader_GetTCRMode(This,Param,pValue)	\
    (This)->lpVtbl -> GetTCRMode(This,Param,pValue)

#define IAMTimecodeReader_SetTCRMode(This,Param,Value)	\
    (This)->lpVtbl -> SetTCRMode(This,Param,Value)

#define IAMTimecodeReader_put_VITCLine(This,Line)	\
    (This)->lpVtbl -> put_VITCLine(This,Line)

#define IAMTimecodeReader_get_VITCLine(This,pLine)	\
    (This)->lpVtbl -> get_VITCLine(This,pLine)

#define IAMTimecodeReader_GetTimecode(This,pTimecodeSample)	\
    (This)->lpVtbl -> GetTimecode(This,pTimecodeSample)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IAMTimecodeReader_GetTCRMode_Proxy( 
    IAMTimecodeReader __RPC_FAR * This,
    /* [in] */ long Param,
    /* [out] */ long __RPC_FAR *pValue);


void __RPC_STUB IAMTimecodeReader_GetTCRMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMTimecodeReader_SetTCRMode_Proxy( 
    IAMTimecodeReader __RPC_FAR * This,
    /* [in] */ long Param,
    /* [in] */ long Value);


void __RPC_STUB IAMTimecodeReader_SetTCRMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMTimecodeReader_put_VITCLine_Proxy( 
    IAMTimecodeReader __RPC_FAR * This,
    /* [in] */ long Line);


void __RPC_STUB IAMTimecodeReader_put_VITCLine_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMTimecodeReader_get_VITCLine_Proxy( 
    IAMTimecodeReader __RPC_FAR * This,
    /* [out] */ long __RPC_FAR *pLine);


void __RPC_STUB IAMTimecodeReader_get_VITCLine_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMTimecodeReader_GetTimecode_Proxy( 
    IAMTimecodeReader __RPC_FAR * This,
    /* [out] */ PTIMECODE_SAMPLE pTimecodeSample);


void __RPC_STUB IAMTimecodeReader_GetTimecode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAMTimecodeReader_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_strmif_0213 */
/* [local] */ 

typedef IAMTimecodeReader __RPC_FAR *PIAMTIMECODEREADER;



extern RPC_IF_HANDLE __MIDL_itf_strmif_0213_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_strmif_0213_v0_0_s_ifspec;

#ifndef __IAMTimecodeGenerator_INTERFACE_DEFINED__
#define __IAMTimecodeGenerator_INTERFACE_DEFINED__

/* interface IAMTimecodeGenerator */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IAMTimecodeGenerator;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("9B496CE0-811B-11cf-8C77-00AA006B6814")
    IAMTimecodeGenerator : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetTCGMode( 
            /* [in] */ long Param,
            /* [out] */ long __RPC_FAR *pValue) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetTCGMode( 
            /* [in] */ long Param,
            /* [in] */ long Value) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE put_VITCLine( 
            /* [in] */ long Line) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get_VITCLine( 
            /* [out] */ long __RPC_FAR *pLine) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetTimecode( 
            /* [in] */ PTIMECODE_SAMPLE pTimecodeSample) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetTimecode( 
            /* [out] */ PTIMECODE_SAMPLE pTimecodeSample) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAMTimecodeGeneratorVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IAMTimecodeGenerator __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IAMTimecodeGenerator __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IAMTimecodeGenerator __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTCGMode )( 
            IAMTimecodeGenerator __RPC_FAR * This,
            /* [in] */ long Param,
            /* [out] */ long __RPC_FAR *pValue);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetTCGMode )( 
            IAMTimecodeGenerator __RPC_FAR * This,
            /* [in] */ long Param,
            /* [in] */ long Value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_VITCLine )( 
            IAMTimecodeGenerator __RPC_FAR * This,
            /* [in] */ long Line);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_VITCLine )( 
            IAMTimecodeGenerator __RPC_FAR * This,
            /* [out] */ long __RPC_FAR *pLine);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetTimecode )( 
            IAMTimecodeGenerator __RPC_FAR * This,
            /* [in] */ PTIMECODE_SAMPLE pTimecodeSample);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTimecode )( 
            IAMTimecodeGenerator __RPC_FAR * This,
            /* [out] */ PTIMECODE_SAMPLE pTimecodeSample);
        
        END_INTERFACE
    } IAMTimecodeGeneratorVtbl;

    interface IAMTimecodeGenerator
    {
        CONST_VTBL struct IAMTimecodeGeneratorVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAMTimecodeGenerator_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAMTimecodeGenerator_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAMTimecodeGenerator_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IAMTimecodeGenerator_GetTCGMode(This,Param,pValue)	\
    (This)->lpVtbl -> GetTCGMode(This,Param,pValue)

#define IAMTimecodeGenerator_SetTCGMode(This,Param,Value)	\
    (This)->lpVtbl -> SetTCGMode(This,Param,Value)

#define IAMTimecodeGenerator_put_VITCLine(This,Line)	\
    (This)->lpVtbl -> put_VITCLine(This,Line)

#define IAMTimecodeGenerator_get_VITCLine(This,pLine)	\
    (This)->lpVtbl -> get_VITCLine(This,pLine)

#define IAMTimecodeGenerator_SetTimecode(This,pTimecodeSample)	\
    (This)->lpVtbl -> SetTimecode(This,pTimecodeSample)

#define IAMTimecodeGenerator_GetTimecode(This,pTimecodeSample)	\
    (This)->lpVtbl -> GetTimecode(This,pTimecodeSample)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IAMTimecodeGenerator_GetTCGMode_Proxy( 
    IAMTimecodeGenerator __RPC_FAR * This,
    /* [in] */ long Param,
    /* [out] */ long __RPC_FAR *pValue);


void __RPC_STUB IAMTimecodeGenerator_GetTCGMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMTimecodeGenerator_SetTCGMode_Proxy( 
    IAMTimecodeGenerator __RPC_FAR * This,
    /* [in] */ long Param,
    /* [in] */ long Value);


void __RPC_STUB IAMTimecodeGenerator_SetTCGMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMTimecodeGenerator_put_VITCLine_Proxy( 
    IAMTimecodeGenerator __RPC_FAR * This,
    /* [in] */ long Line);


void __RPC_STUB IAMTimecodeGenerator_put_VITCLine_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMTimecodeGenerator_get_VITCLine_Proxy( 
    IAMTimecodeGenerator __RPC_FAR * This,
    /* [out] */ long __RPC_FAR *pLine);


void __RPC_STUB IAMTimecodeGenerator_get_VITCLine_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMTimecodeGenerator_SetTimecode_Proxy( 
    IAMTimecodeGenerator __RPC_FAR * This,
    /* [in] */ PTIMECODE_SAMPLE pTimecodeSample);


void __RPC_STUB IAMTimecodeGenerator_SetTimecode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMTimecodeGenerator_GetTimecode_Proxy( 
    IAMTimecodeGenerator __RPC_FAR * This,
    /* [out] */ PTIMECODE_SAMPLE pTimecodeSample);


void __RPC_STUB IAMTimecodeGenerator_GetTimecode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAMTimecodeGenerator_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_strmif_0214 */
/* [local] */ 

typedef IAMTimecodeGenerator __RPC_FAR *PIAMTIMECODEGENERATOR;



extern RPC_IF_HANDLE __MIDL_itf_strmif_0214_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_strmif_0214_v0_0_s_ifspec;

#ifndef __IAMTimecodeDisplay_INTERFACE_DEFINED__
#define __IAMTimecodeDisplay_INTERFACE_DEFINED__

/* interface IAMTimecodeDisplay */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IAMTimecodeDisplay;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("9B496CE2-811B-11cf-8C77-00AA006B6814")
    IAMTimecodeDisplay : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetTCDisplayEnable( 
            /* [out] */ long __RPC_FAR *pState) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetTCDisplayEnable( 
            /* [in] */ long State) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetTCDisplay( 
            /* [in] */ long Param,
            /* [out] */ long __RPC_FAR *pValue) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetTCDisplay( 
            /* [in] */ long Param,
            /* [in] */ long Value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAMTimecodeDisplayVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IAMTimecodeDisplay __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IAMTimecodeDisplay __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IAMTimecodeDisplay __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTCDisplayEnable )( 
            IAMTimecodeDisplay __RPC_FAR * This,
            /* [out] */ long __RPC_FAR *pState);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetTCDisplayEnable )( 
            IAMTimecodeDisplay __RPC_FAR * This,
            /* [in] */ long State);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTCDisplay )( 
            IAMTimecodeDisplay __RPC_FAR * This,
            /* [in] */ long Param,
            /* [out] */ long __RPC_FAR *pValue);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetTCDisplay )( 
            IAMTimecodeDisplay __RPC_FAR * This,
            /* [in] */ long Param,
            /* [in] */ long Value);
        
        END_INTERFACE
    } IAMTimecodeDisplayVtbl;

    interface IAMTimecodeDisplay
    {
        CONST_VTBL struct IAMTimecodeDisplayVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAMTimecodeDisplay_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAMTimecodeDisplay_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAMTimecodeDisplay_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IAMTimecodeDisplay_GetTCDisplayEnable(This,pState)	\
    (This)->lpVtbl -> GetTCDisplayEnable(This,pState)

#define IAMTimecodeDisplay_SetTCDisplayEnable(This,State)	\
    (This)->lpVtbl -> SetTCDisplayEnable(This,State)

#define IAMTimecodeDisplay_GetTCDisplay(This,Param,pValue)	\
    (This)->lpVtbl -> GetTCDisplay(This,Param,pValue)

#define IAMTimecodeDisplay_SetTCDisplay(This,Param,Value)	\
    (This)->lpVtbl -> SetTCDisplay(This,Param,Value)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IAMTimecodeDisplay_GetTCDisplayEnable_Proxy( 
    IAMTimecodeDisplay __RPC_FAR * This,
    /* [out] */ long __RPC_FAR *pState);


void __RPC_STUB IAMTimecodeDisplay_GetTCDisplayEnable_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMTimecodeDisplay_SetTCDisplayEnable_Proxy( 
    IAMTimecodeDisplay __RPC_FAR * This,
    /* [in] */ long State);


void __RPC_STUB IAMTimecodeDisplay_SetTCDisplayEnable_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMTimecodeDisplay_GetTCDisplay_Proxy( 
    IAMTimecodeDisplay __RPC_FAR * This,
    /* [in] */ long Param,
    /* [out] */ long __RPC_FAR *pValue);


void __RPC_STUB IAMTimecodeDisplay_GetTCDisplay_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMTimecodeDisplay_SetTCDisplay_Proxy( 
    IAMTimecodeDisplay __RPC_FAR * This,
    /* [in] */ long Param,
    /* [in] */ long Value);


void __RPC_STUB IAMTimecodeDisplay_SetTCDisplay_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAMTimecodeDisplay_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_strmif_0215 */
/* [local] */ 

typedef IAMTimecodeDisplay __RPC_FAR *PIAMTIMECODEDISPLAY;



extern RPC_IF_HANDLE __MIDL_itf_strmif_0215_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_strmif_0215_v0_0_s_ifspec;

#ifndef __IAMDevMemoryAllocator_INTERFACE_DEFINED__
#define __IAMDevMemoryAllocator_INTERFACE_DEFINED__

/* interface IAMDevMemoryAllocator */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IAMDevMemoryAllocator;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("c6545bf0-e76b-11d0-bd52-00a0c911ce86")
    IAMDevMemoryAllocator : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetInfo( 
            /* [out] */ DWORD __RPC_FAR *pdwcbTotalFree,
            /* [out] */ DWORD __RPC_FAR *pdwcbLargestFree,
            /* [out] */ DWORD __RPC_FAR *pdwcbTotalMemory,
            /* [out] */ DWORD __RPC_FAR *pdwcbMinimumChunk) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CheckMemory( 
            /* [in] */ const BYTE __RPC_FAR *pBuffer) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Alloc( 
            /* [out] */ BYTE __RPC_FAR *__RPC_FAR *ppBuffer,
            /* [out][in] */ DWORD __RPC_FAR *pdwcbBuffer) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Free( 
            /* [in] */ BYTE __RPC_FAR *pBuffer) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetDevMemoryObject( 
            /* [out] */ IUnknown __RPC_FAR *__RPC_FAR *ppUnkInnner,
            /* [in] */ IUnknown __RPC_FAR *pUnkOuter) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAMDevMemoryAllocatorVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IAMDevMemoryAllocator __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IAMDevMemoryAllocator __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IAMDevMemoryAllocator __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetInfo )( 
            IAMDevMemoryAllocator __RPC_FAR * This,
            /* [out] */ DWORD __RPC_FAR *pdwcbTotalFree,
            /* [out] */ DWORD __RPC_FAR *pdwcbLargestFree,
            /* [out] */ DWORD __RPC_FAR *pdwcbTotalMemory,
            /* [out] */ DWORD __RPC_FAR *pdwcbMinimumChunk);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CheckMemory )( 
            IAMDevMemoryAllocator __RPC_FAR * This,
            /* [in] */ const BYTE __RPC_FAR *pBuffer);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Alloc )( 
            IAMDevMemoryAllocator __RPC_FAR * This,
            /* [out] */ BYTE __RPC_FAR *__RPC_FAR *ppBuffer,
            /* [out][in] */ DWORD __RPC_FAR *pdwcbBuffer);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Free )( 
            IAMDevMemoryAllocator __RPC_FAR * This,
            /* [in] */ BYTE __RPC_FAR *pBuffer);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDevMemoryObject )( 
            IAMDevMemoryAllocator __RPC_FAR * This,
            /* [out] */ IUnknown __RPC_FAR *__RPC_FAR *ppUnkInnner,
            /* [in] */ IUnknown __RPC_FAR *pUnkOuter);
        
        END_INTERFACE
    } IAMDevMemoryAllocatorVtbl;

    interface IAMDevMemoryAllocator
    {
        CONST_VTBL struct IAMDevMemoryAllocatorVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAMDevMemoryAllocator_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAMDevMemoryAllocator_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAMDevMemoryAllocator_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IAMDevMemoryAllocator_GetInfo(This,pdwcbTotalFree,pdwcbLargestFree,pdwcbTotalMemory,pdwcbMinimumChunk)	\
    (This)->lpVtbl -> GetInfo(This,pdwcbTotalFree,pdwcbLargestFree,pdwcbTotalMemory,pdwcbMinimumChunk)

#define IAMDevMemoryAllocator_CheckMemory(This,pBuffer)	\
    (This)->lpVtbl -> CheckMemory(This,pBuffer)

#define IAMDevMemoryAllocator_Alloc(This,ppBuffer,pdwcbBuffer)	\
    (This)->lpVtbl -> Alloc(This,ppBuffer,pdwcbBuffer)

#define IAMDevMemoryAllocator_Free(This,pBuffer)	\
    (This)->lpVtbl -> Free(This,pBuffer)

#define IAMDevMemoryAllocator_GetDevMemoryObject(This,ppUnkInnner,pUnkOuter)	\
    (This)->lpVtbl -> GetDevMemoryObject(This,ppUnkInnner,pUnkOuter)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IAMDevMemoryAllocator_GetInfo_Proxy( 
    IAMDevMemoryAllocator __RPC_FAR * This,
    /* [out] */ DWORD __RPC_FAR *pdwcbTotalFree,
    /* [out] */ DWORD __RPC_FAR *pdwcbLargestFree,
    /* [out] */ DWORD __RPC_FAR *pdwcbTotalMemory,
    /* [out] */ DWORD __RPC_FAR *pdwcbMinimumChunk);


void __RPC_STUB IAMDevMemoryAllocator_GetInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMDevMemoryAllocator_CheckMemory_Proxy( 
    IAMDevMemoryAllocator __RPC_FAR * This,
    /* [in] */ const BYTE __RPC_FAR *pBuffer);


void __RPC_STUB IAMDevMemoryAllocator_CheckMemory_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMDevMemoryAllocator_Alloc_Proxy( 
    IAMDevMemoryAllocator __RPC_FAR * This,
    /* [out] */ BYTE __RPC_FAR *__RPC_FAR *ppBuffer,
    /* [out][in] */ DWORD __RPC_FAR *pdwcbBuffer);


void __RPC_STUB IAMDevMemoryAllocator_Alloc_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMDevMemoryAllocator_Free_Proxy( 
    IAMDevMemoryAllocator __RPC_FAR * This,
    /* [in] */ BYTE __RPC_FAR *pBuffer);


void __RPC_STUB IAMDevMemoryAllocator_Free_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMDevMemoryAllocator_GetDevMemoryObject_Proxy( 
    IAMDevMemoryAllocator __RPC_FAR * This,
    /* [out] */ IUnknown __RPC_FAR *__RPC_FAR *ppUnkInnner,
    /* [in] */ IUnknown __RPC_FAR *pUnkOuter);


void __RPC_STUB IAMDevMemoryAllocator_GetDevMemoryObject_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAMDevMemoryAllocator_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_strmif_0216 */
/* [local] */ 

typedef IAMDevMemoryAllocator __RPC_FAR *PAMDEVMEMORYALLOCATOR;



extern RPC_IF_HANDLE __MIDL_itf_strmif_0216_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_strmif_0216_v0_0_s_ifspec;

#ifndef __IAMDevMemoryControl_INTERFACE_DEFINED__
#define __IAMDevMemoryControl_INTERFACE_DEFINED__

/* interface IAMDevMemoryControl */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IAMDevMemoryControl;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("c6545bf1-e76b-11d0-bd52-00a0c911ce86")
    IAMDevMemoryControl : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE QueryWriteSync( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE WriteSync( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetDevId( 
            /* [out] */ DWORD __RPC_FAR *pdwDevId) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAMDevMemoryControlVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IAMDevMemoryControl __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IAMDevMemoryControl __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IAMDevMemoryControl __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryWriteSync )( 
            IAMDevMemoryControl __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *WriteSync )( 
            IAMDevMemoryControl __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDevId )( 
            IAMDevMemoryControl __RPC_FAR * This,
            /* [out] */ DWORD __RPC_FAR *pdwDevId);
        
        END_INTERFACE
    } IAMDevMemoryControlVtbl;

    interface IAMDevMemoryControl
    {
        CONST_VTBL struct IAMDevMemoryControlVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAMDevMemoryControl_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAMDevMemoryControl_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAMDevMemoryControl_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IAMDevMemoryControl_QueryWriteSync(This)	\
    (This)->lpVtbl -> QueryWriteSync(This)

#define IAMDevMemoryControl_WriteSync(This)	\
    (This)->lpVtbl -> WriteSync(This)

#define IAMDevMemoryControl_GetDevId(This,pdwDevId)	\
    (This)->lpVtbl -> GetDevId(This,pdwDevId)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IAMDevMemoryControl_QueryWriteSync_Proxy( 
    IAMDevMemoryControl __RPC_FAR * This);


void __RPC_STUB IAMDevMemoryControl_QueryWriteSync_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMDevMemoryControl_WriteSync_Proxy( 
    IAMDevMemoryControl __RPC_FAR * This);


void __RPC_STUB IAMDevMemoryControl_WriteSync_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMDevMemoryControl_GetDevId_Proxy( 
    IAMDevMemoryControl __RPC_FAR * This,
    /* [out] */ DWORD __RPC_FAR *pdwDevId);


void __RPC_STUB IAMDevMemoryControl_GetDevId_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAMDevMemoryControl_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_strmif_0217 */
/* [local] */ 

typedef IAMDevMemoryControl __RPC_FAR *PAMDEVMEMORYCONTROL;


enum _AMSTREAMSELECTINFOFLAGS
    {	AMSTREAMSELECTINFO_ENABLED	= 0x1,
	AMSTREAMSELECTINFO_EXCLUSIVE	= 0x2
    };

enum _AMSTREAMSELECTENABLEFLAGS
    {	AMSTREAMSELECTENABLE_ENABLE	= 0x1,
	AMSTREAMSELECTENABLE_ENABLEALL	= 0x2
    };


extern RPC_IF_HANDLE __MIDL_itf_strmif_0217_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_strmif_0217_v0_0_s_ifspec;

#ifndef __IAMStreamSelect_INTERFACE_DEFINED__
#define __IAMStreamSelect_INTERFACE_DEFINED__

/* interface IAMStreamSelect */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IAMStreamSelect;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("c1960960-17f5-11d1-abe1-00a0c905f375")
    IAMStreamSelect : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Count( 
            /* [out] */ DWORD __RPC_FAR *pcStreams) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Info( 
            /* [in] */ long lIndex,
            /* [out] */ AM_MEDIA_TYPE __RPC_FAR *__RPC_FAR *ppmt,
            /* [out] */ DWORD __RPC_FAR *pdwFlags,
            /* [out] */ LCID __RPC_FAR *plcid,
            /* [out] */ DWORD __RPC_FAR *pdwGroup,
            /* [out] */ WCHAR __RPC_FAR *__RPC_FAR *ppszName,
            /* [out] */ IUnknown __RPC_FAR *__RPC_FAR *ppObject,
            /* [out] */ IUnknown __RPC_FAR *__RPC_FAR *ppUnk) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Enable( 
            /* [in] */ long lIndex,
            /* [in] */ DWORD dwFlags) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAMStreamSelectVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IAMStreamSelect __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IAMStreamSelect __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IAMStreamSelect __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Count )( 
            IAMStreamSelect __RPC_FAR * This,
            /* [out] */ DWORD __RPC_FAR *pcStreams);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Info )( 
            IAMStreamSelect __RPC_FAR * This,
            /* [in] */ long lIndex,
            /* [out] */ AM_MEDIA_TYPE __RPC_FAR *__RPC_FAR *ppmt,
            /* [out] */ DWORD __RPC_FAR *pdwFlags,
            /* [out] */ LCID __RPC_FAR *plcid,
            /* [out] */ DWORD __RPC_FAR *pdwGroup,
            /* [out] */ WCHAR __RPC_FAR *__RPC_FAR *ppszName,
            /* [out] */ IUnknown __RPC_FAR *__RPC_FAR *ppObject,
            /* [out] */ IUnknown __RPC_FAR *__RPC_FAR *ppUnk);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Enable )( 
            IAMStreamSelect __RPC_FAR * This,
            /* [in] */ long lIndex,
            /* [in] */ DWORD dwFlags);
        
        END_INTERFACE
    } IAMStreamSelectVtbl;

    interface IAMStreamSelect
    {
        CONST_VTBL struct IAMStreamSelectVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAMStreamSelect_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAMStreamSelect_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAMStreamSelect_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IAMStreamSelect_Count(This,pcStreams)	\
    (This)->lpVtbl -> Count(This,pcStreams)

#define IAMStreamSelect_Info(This,lIndex,ppmt,pdwFlags,plcid,pdwGroup,ppszName,ppObject,ppUnk)	\
    (This)->lpVtbl -> Info(This,lIndex,ppmt,pdwFlags,plcid,pdwGroup,ppszName,ppObject,ppUnk)

#define IAMStreamSelect_Enable(This,lIndex,dwFlags)	\
    (This)->lpVtbl -> Enable(This,lIndex,dwFlags)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IAMStreamSelect_Count_Proxy( 
    IAMStreamSelect __RPC_FAR * This,
    /* [out] */ DWORD __RPC_FAR *pcStreams);


void __RPC_STUB IAMStreamSelect_Count_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMStreamSelect_Info_Proxy( 
    IAMStreamSelect __RPC_FAR * This,
    /* [in] */ long lIndex,
    /* [out] */ AM_MEDIA_TYPE __RPC_FAR *__RPC_FAR *ppmt,
    /* [out] */ DWORD __RPC_FAR *pdwFlags,
    /* [out] */ LCID __RPC_FAR *plcid,
    /* [out] */ DWORD __RPC_FAR *pdwGroup,
    /* [out] */ WCHAR __RPC_FAR *__RPC_FAR *ppszName,
    /* [out] */ IUnknown __RPC_FAR *__RPC_FAR *ppObject,
    /* [out] */ IUnknown __RPC_FAR *__RPC_FAR *ppUnk);


void __RPC_STUB IAMStreamSelect_Info_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMStreamSelect_Enable_Proxy( 
    IAMStreamSelect __RPC_FAR * This,
    /* [in] */ long lIndex,
    /* [in] */ DWORD dwFlags);


void __RPC_STUB IAMStreamSelect_Enable_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAMStreamSelect_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_strmif_0218 */
/* [local] */ 

typedef IAMStreamSelect __RPC_FAR *PAMSTREAMSELECT;


enum _AMRESCTL_RESERVEFLAGS
    {	AMRESCTL_RESERVEFLAGS_RESERVE	= 0,
	AMRESCTL_RESERVEFLAGS_UNRESERVE	= 0x1
    };


extern RPC_IF_HANDLE __MIDL_itf_strmif_0218_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_strmif_0218_v0_0_s_ifspec;

#ifndef __IAMResourceControl_INTERFACE_DEFINED__
#define __IAMResourceControl_INTERFACE_DEFINED__

/* interface IAMResourceControl */
/* [local][unique][uuid][object] */ 


EXTERN_C const IID IID_IAMResourceControl;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("8389d2d0-77d7-11d1-abe6-00a0c905f375")
    IAMResourceControl : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Reserve( 
            /* [in] */ DWORD dwFlags,
            /* [in] */ PVOID pvReserved) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAMResourceControlVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IAMResourceControl __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IAMResourceControl __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IAMResourceControl __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Reserve )( 
            IAMResourceControl __RPC_FAR * This,
            /* [in] */ DWORD dwFlags,
            /* [in] */ PVOID pvReserved);
        
        END_INTERFACE
    } IAMResourceControlVtbl;

    interface IAMResourceControl
    {
        CONST_VTBL struct IAMResourceControlVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAMResourceControl_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAMResourceControl_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAMResourceControl_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IAMResourceControl_Reserve(This,dwFlags,pvReserved)	\
    (This)->lpVtbl -> Reserve(This,dwFlags,pvReserved)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IAMResourceControl_Reserve_Proxy( 
    IAMResourceControl __RPC_FAR * This,
    /* [in] */ DWORD dwFlags,
    /* [in] */ PVOID pvReserved);


void __RPC_STUB IAMResourceControl_Reserve_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAMResourceControl_INTERFACE_DEFINED__ */


#ifndef __IAMClockAdjust_INTERFACE_DEFINED__
#define __IAMClockAdjust_INTERFACE_DEFINED__

/* interface IAMClockAdjust */
/* [local][unique][uuid][object] */ 


EXTERN_C const IID IID_IAMClockAdjust;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("4d5466b0-a49c-11d1-abe8-00a0c905f375")
    IAMClockAdjust : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SetClockDelta( 
            /* [in] */ REFERENCE_TIME rtDelta) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAMClockAdjustVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IAMClockAdjust __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IAMClockAdjust __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IAMClockAdjust __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetClockDelta )( 
            IAMClockAdjust __RPC_FAR * This,
            /* [in] */ REFERENCE_TIME rtDelta);
        
        END_INTERFACE
    } IAMClockAdjustVtbl;

    interface IAMClockAdjust
    {
        CONST_VTBL struct IAMClockAdjustVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAMClockAdjust_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAMClockAdjust_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAMClockAdjust_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IAMClockAdjust_SetClockDelta(This,rtDelta)	\
    (This)->lpVtbl -> SetClockDelta(This,rtDelta)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IAMClockAdjust_SetClockDelta_Proxy( 
    IAMClockAdjust __RPC_FAR * This,
    /* [in] */ REFERENCE_TIME rtDelta);


void __RPC_STUB IAMClockAdjust_SetClockDelta_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAMClockAdjust_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_strmif_0220 */
/* [local] */ 


enum _AM_FILTER_MISC_FLAGS
    {	AM_FILTER_MISC_FLAGS_IS_RENDERER	= 0x1,
	AM_FILTER_MISC_FLAGS_IS_SOURCE	= 0x2,
	AM_FILTER_MISC_FLAG_NO_CAPTURE_PAUSE	= 0x4
    };


extern RPC_IF_HANDLE __MIDL_itf_strmif_0220_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_strmif_0220_v0_0_s_ifspec;

#ifndef __IAMFilterMiscFlags_INTERFACE_DEFINED__
#define __IAMFilterMiscFlags_INTERFACE_DEFINED__

/* interface IAMFilterMiscFlags */
/* [local][unique][uuid][object] */ 


EXTERN_C const IID IID_IAMFilterMiscFlags;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("2dd74950-a890-11d1-abe8-00a0c905f375")
    IAMFilterMiscFlags : public IUnknown
    {
    public:
        virtual ULONG STDMETHODCALLTYPE GetMiscFlags( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAMFilterMiscFlagsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IAMFilterMiscFlags __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IAMFilterMiscFlags __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IAMFilterMiscFlags __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *GetMiscFlags )( 
            IAMFilterMiscFlags __RPC_FAR * This);
        
        END_INTERFACE
    } IAMFilterMiscFlagsVtbl;

    interface IAMFilterMiscFlags
    {
        CONST_VTBL struct IAMFilterMiscFlagsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAMFilterMiscFlags_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAMFilterMiscFlags_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAMFilterMiscFlags_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IAMFilterMiscFlags_GetMiscFlags(This)	\
    (This)->lpVtbl -> GetMiscFlags(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



ULONG STDMETHODCALLTYPE IAMFilterMiscFlags_GetMiscFlags_Proxy( 
    IAMFilterMiscFlags __RPC_FAR * This);


void __RPC_STUB IAMFilterMiscFlags_GetMiscFlags_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAMFilterMiscFlags_INTERFACE_DEFINED__ */


#ifndef __IAMAudioRendererStats_INTERFACE_DEFINED__
#define __IAMAudioRendererStats_INTERFACE_DEFINED__

/* interface IAMAudioRendererStats */
/* [local][unique][uuid][object] */ 


enum _AM_AUDIO_RENDERER_STAT_PARAM
    {	AM_AUDREND_STAT_PARAM_BREAK_COUNT	= 1,
	AM_AUDREND_STAT_PARAM_SLAVE_MODE	= AM_AUDREND_STAT_PARAM_BREAK_COUNT + 1,
	AM_AUDREND_STAT_PARAM_SILENCE_DUR	= AM_AUDREND_STAT_PARAM_SLAVE_MODE + 1,
	AM_AUDREND_STAT_PARAM_LAST_BUFFER_DUR	= AM_AUDREND_STAT_PARAM_SILENCE_DUR + 1,
	AM_AUDREND_STAT_PARAM_DISCONTINUITIES	= AM_AUDREND_STAT_PARAM_LAST_BUFFER_DUR + 1,
	AM_AUDREND_STAT_PARAM_SLAVE_RATE	= AM_AUDREND_STAT_PARAM_DISCONTINUITIES + 1,
	AM_AUDREND_STAT_PARAM_SLAVE_DROPWRITE_DUR	= AM_AUDREND_STAT_PARAM_SLAVE_RATE + 1,
	AM_AUDREND_STAT_PARAM_SLAVE_HIGHLOWERROR	= AM_AUDREND_STAT_PARAM_SLAVE_DROPWRITE_DUR + 1,
	AM_AUDREND_STAT_PARAM_SLAVE_LASTHIGHLOWERROR	= AM_AUDREND_STAT_PARAM_SLAVE_HIGHLOWERROR + 1,
	AM_AUDREND_STAT_PARAM_SLAVE_ACCUMERROR	= AM_AUDREND_STAT_PARAM_SLAVE_LASTHIGHLOWERROR + 1,
	AM_AUDREND_STAT_PARAM_BUFFERFULLNESS	= AM_AUDREND_STAT_PARAM_SLAVE_ACCUMERROR + 1,
	AM_AUDREND_STAT_PARAM_JITTER	= AM_AUDREND_STAT_PARAM_BUFFERFULLNESS + 1
    };

EXTERN_C const IID IID_IAMAudioRendererStats;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("f6467d6d-9ce6-4b52-8dfd-63ba1322f90a")
    IAMAudioRendererStats : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetStatParam( 
            DWORD dwParam,
            DWORD __RPC_FAR *pdwParam1,
            DWORD __RPC_FAR *pdwParam2) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAMAudioRendererStatsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IAMAudioRendererStats __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IAMAudioRendererStats __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IAMAudioRendererStats __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetStatParam )( 
            IAMAudioRendererStats __RPC_FAR * This,
            DWORD dwParam,
            DWORD __RPC_FAR *pdwParam1,
            DWORD __RPC_FAR *pdwParam2);
        
        END_INTERFACE
    } IAMAudioRendererStatsVtbl;

    interface IAMAudioRendererStats
    {
        CONST_VTBL struct IAMAudioRendererStatsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAMAudioRendererStats_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAMAudioRendererStats_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAMAudioRendererStats_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IAMAudioRendererStats_GetStatParam(This,dwParam,pdwParam1,pdwParam2)	\
    (This)->lpVtbl -> GetStatParam(This,dwParam,pdwParam1,pdwParam2)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IAMAudioRendererStats_GetStatParam_Proxy( 
    IAMAudioRendererStats __RPC_FAR * This,
    DWORD dwParam,
    DWORD __RPC_FAR *pdwParam1,
    DWORD __RPC_FAR *pdwParam2);


void __RPC_STUB IAMAudioRendererStats_GetStatParam_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAMAudioRendererStats_INTERFACE_DEFINED__ */


#ifndef __IAMVideoRendererMode_INTERFACE_DEFINED__
#define __IAMVideoRendererMode_INTERFACE_DEFINED__

/* interface IAMVideoRendererMode */
/* [local][unique][uuid][object] */ 


EXTERN_C const IID IID_IAMVideoRendererMode;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("E95AD8E0-993D-4817-B629-16B3E05F2520")
    IAMVideoRendererMode : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SetMode( 
            /* [in] */ DWORD dwMode) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetMode( 
            /* [out] */ DWORD __RPC_FAR *pdwMode) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAMVideoRendererModeVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IAMVideoRendererMode __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IAMVideoRendererMode __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IAMVideoRendererMode __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetMode )( 
            IAMVideoRendererMode __RPC_FAR * This,
            /* [in] */ DWORD dwMode);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMode )( 
            IAMVideoRendererMode __RPC_FAR * This,
            /* [out] */ DWORD __RPC_FAR *pdwMode);
        
        END_INTERFACE
    } IAMVideoRendererModeVtbl;

    interface IAMVideoRendererMode
    {
        CONST_VTBL struct IAMVideoRendererModeVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAMVideoRendererMode_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAMVideoRendererMode_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAMVideoRendererMode_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IAMVideoRendererMode_SetMode(This,dwMode)	\
    (This)->lpVtbl -> SetMode(This,dwMode)

#define IAMVideoRendererMode_GetMode(This,pdwMode)	\
    (This)->lpVtbl -> GetMode(This,pdwMode)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IAMVideoRendererMode_SetMode_Proxy( 
    IAMVideoRendererMode __RPC_FAR * This,
    /* [in] */ DWORD dwMode);


void __RPC_STUB IAMVideoRendererMode_SetMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAMVideoRendererMode_GetMode_Proxy( 
    IAMVideoRendererMode __RPC_FAR * This,
    /* [out] */ DWORD __RPC_FAR *pdwMode);


void __RPC_STUB IAMVideoRendererMode_GetMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAMVideoRendererMode_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_strmif_0223 */
/* [local] */ 

typedef /* [public] */ 
enum __MIDL___MIDL_itf_strmif_0223_0001
    {	AM_VIDEO_RENDERER_MODE_DDRAW	= 0,
	AM_VIDEO_RENDERER_MODE_GDI	= AM_VIDEO_RENDERER_MODE_DDRAW + 1
    }	AM_VIDEO_RENDERER_MODE;



extern RPC_IF_HANDLE __MIDL_itf_strmif_0223_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_strmif_0223_v0_0_s_ifspec;

#ifndef __ILanguageSelection_INTERFACE_DEFINED__
#define __ILanguageSelection_INTERFACE_DEFINED__

/* interface ILanguageSelection */
/* [local][unique][uuid][object] */ 


EXTERN_C const IID IID_ILanguageSelection;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("41f6a908-4810-464d-8682-8d5f56440018")
    ILanguageSelection : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetLanguagesCount( 
            /* [out] */ DWORD __RPC_FAR *pdwCount) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetLanguageItem( 
            /* [in] */ DWORD dwLanguageIndex,
            /* [out] */ BSTR __RPC_FAR *pbstrLanguage) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetLanguage( 
            /* [in] */ DWORD dwLanguageIndex) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ILanguageSelectionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ILanguageSelection __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ILanguageSelection __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ILanguageSelection __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetLanguagesCount )( 
            ILanguageSelection __RPC_FAR * This,
            /* [out] */ DWORD __RPC_FAR *pdwCount);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetLanguageItem )( 
            ILanguageSelection __RPC_FAR * This,
            /* [in] */ DWORD dwLanguageIndex,
            /* [out] */ BSTR __RPC_FAR *pbstrLanguage);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetLanguage )( 
            ILanguageSelection __RPC_FAR * This,
            /* [in] */ DWORD dwLanguageIndex);
        
        END_INTERFACE
    } ILanguageSelectionVtbl;

    interface ILanguageSelection
    {
        CONST_VTBL struct ILanguageSelectionVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ILanguageSelection_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ILanguageSelection_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ILanguageSelection_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ILanguageSelection_GetLanguagesCount(This,pdwCount)	\
    (This)->lpVtbl -> GetLanguagesCount(This,pdwCount)

#define ILanguageSelection_GetLanguageItem(This,dwLanguageIndex,pbstrLanguage)	\
    (This)->lpVtbl -> GetLanguageItem(This,dwLanguageIndex,pbstrLanguage)

#define ILanguageSelection_SetLanguage(This,dwLanguageIndex)	\
    (This)->lpVtbl -> SetLanguage(This,dwLanguageIndex)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE ILanguageSelection_GetLanguagesCount_Proxy( 
    ILanguageSelection __RPC_FAR * This,
    /* [out] */ DWORD __RPC_FAR *pdwCount);


void __RPC_STUB ILanguageSelection_GetLanguagesCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ILanguageSelection_GetLanguageItem_Proxy( 
    ILanguageSelection __RPC_FAR * This,
    /* [in] */ DWORD dwLanguageIndex,
    /* [out] */ BSTR __RPC_FAR *pbstrLanguage);


void __RPC_STUB ILanguageSelection_GetLanguageItem_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ILanguageSelection_SetLanguage_Proxy( 
    ILanguageSelection __RPC_FAR * This,
    /* [in] */ DWORD dwLanguageIndex);


void __RPC_STUB ILanguageSelection_SetLanguage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ILanguageSelection_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_strmif_0224 */
/* [local] */ 

typedef /* [public][public] */ struct __MIDL___MIDL_itf_strmif_0224_0001
    {
    GUID Guid;
    ULONG NumberOfValues;
    ULONG Type;
    void __RPC_FAR *Value;
    }	StructEncoderParameter;



extern RPC_IF_HANDLE __MIDL_itf_strmif_0224_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_strmif_0224_v0_0_s_ifspec;

#ifndef __IImageSinkFilter_INTERFACE_DEFINED__
#define __IImageSinkFilter_INTERFACE_DEFINED__

/* interface IImageSinkFilter */
/* [local][unique][uuid][object] */ 


EXTERN_C const IID IID_IImageSinkFilter;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("7f12e45f-b224-449e-907a-a18fca14a579")
    IImageSinkFilter : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SetQuality( 
            /* [in] */ DWORD dwQuality) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetEncoderParameters( 
            /* [in] */ DWORD dwCount,
            /* [in] */ StructEncoderParameter __RPC_FAR *Parameter) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IImageSinkFilterVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IImageSinkFilter __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IImageSinkFilter __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IImageSinkFilter __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetQuality )( 
            IImageSinkFilter __RPC_FAR * This,
            /* [in] */ DWORD dwQuality);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetEncoderParameters )( 
            IImageSinkFilter __RPC_FAR * This,
            /* [in] */ DWORD dwCount,
            /* [in] */ StructEncoderParameter __RPC_FAR *Parameter);
        
        END_INTERFACE
    } IImageSinkFilterVtbl;

    interface IImageSinkFilter
    {
        CONST_VTBL struct IImageSinkFilterVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IImageSinkFilter_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IImageSinkFilter_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IImageSinkFilter_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IImageSinkFilter_SetQuality(This,dwQuality)	\
    (This)->lpVtbl -> SetQuality(This,dwQuality)

#define IImageSinkFilter_SetEncoderParameters(This,dwCount,Parameter)	\
    (This)->lpVtbl -> SetEncoderParameters(This,dwCount,Parameter)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IImageSinkFilter_SetQuality_Proxy( 
    IImageSinkFilter __RPC_FAR * This,
    /* [in] */ DWORD dwQuality);


void __RPC_STUB IImageSinkFilter_SetQuality_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IImageSinkFilter_SetEncoderParameters_Proxy( 
    IImageSinkFilter __RPC_FAR * This,
    /* [in] */ DWORD dwCount,
    /* [in] */ StructEncoderParameter __RPC_FAR *Parameter);


void __RPC_STUB IImageSinkFilter_SetEncoderParameters_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IImageSinkFilter_INTERFACE_DEFINED__ */


#ifndef __ISmartTee_INTERFACE_DEFINED__
#define __ISmartTee_INTERFACE_DEFINED__

/* interface ISmartTee */
/* [local][unique][uuid][object] */ 


EXTERN_C const IID IID_ISmartTee;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("25547a74-22a6-4e1e-9525-004dfa37c31c")
    ISmartTee : public IUnknown
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct ISmartTeeVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ISmartTee __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ISmartTee __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ISmartTee __RPC_FAR * This);
        
        END_INTERFACE
    } ISmartTeeVtbl;

    interface ISmartTee
    {
        CONST_VTBL struct ISmartTeeVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISmartTee_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ISmartTee_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ISmartTee_Release(This)	\
    (This)->lpVtbl -> Release(This)


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ISmartTee_INTERFACE_DEFINED__ */


#ifndef __IBuffering_INTERFACE_DEFINED__
#define __IBuffering_INTERFACE_DEFINED__

/* interface IBuffering */
/* [local][unique][uuid][object] */ 


EXTERN_C const IID IID_IBuffering;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6ddbdd08-d2d4-4cdf-82b3-f61e29c14996")
    IBuffering : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SetBufferingDepth( 
            const REFERENCE_TIME __RPC_FAR *ptStop) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetBufferingDepth( 
            REFERENCE_TIME __RPC_FAR *ptStop) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IBufferingVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IBuffering __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IBuffering __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IBuffering __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetBufferingDepth )( 
            IBuffering __RPC_FAR * This,
            const REFERENCE_TIME __RPC_FAR *ptStop);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetBufferingDepth )( 
            IBuffering __RPC_FAR * This,
            REFERENCE_TIME __RPC_FAR *ptStop);
        
        END_INTERFACE
    } IBufferingVtbl;

    interface IBuffering
    {
        CONST_VTBL struct IBufferingVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IBuffering_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IBuffering_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IBuffering_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IBuffering_SetBufferingDepth(This,ptStop)	\
    (This)->lpVtbl -> SetBufferingDepth(This,ptStop)

#define IBuffering_GetBufferingDepth(This,ptStop)	\
    (This)->lpVtbl -> GetBufferingDepth(This,ptStop)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IBuffering_SetBufferingDepth_Proxy( 
    IBuffering __RPC_FAR * This,
    const REFERENCE_TIME __RPC_FAR *ptStop);


void __RPC_STUB IBuffering_SetBufferingDepth_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IBuffering_GetBufferingDepth_Proxy( 
    IBuffering __RPC_FAR * This,
    REFERENCE_TIME __RPC_FAR *ptStop);


void __RPC_STUB IBuffering_GetBufferingDepth_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IBuffering_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_strmif_0227 */
/* [local] */ 

typedef 
enum tagBufferingFullType
    {	VideoBufferFull	= 1,
	AudioBufferFull	= 2
    }	BufferFullType;

#include <ddraw.h>







typedef 
enum tagDVD_DOMAIN
    {	DVD_DOMAIN_FirstPlay	= 1,
	DVD_DOMAIN_VideoManagerMenu	= DVD_DOMAIN_FirstPlay + 1,
	DVD_DOMAIN_VideoTitleSetMenu	= DVD_DOMAIN_VideoManagerMenu + 1,
	DVD_DOMAIN_Title	= DVD_DOMAIN_VideoTitleSetMenu + 1,
	DVD_DOMAIN_Stop	= DVD_DOMAIN_Title + 1
    }	DVD_DOMAIN;

typedef 
enum tagDVD_MENU_ID
    {	DVD_MENU_Title	= 2,
	DVD_MENU_Root	= 3,
	DVD_MENU_Subpicture	= 4,
	DVD_MENU_Audio	= 5,
	DVD_MENU_Angle	= 6,
	DVD_MENU_Chapter	= 7
    }	DVD_MENU_ID;

typedef 
enum tagDVD_DISC_SIDE
    {	DVD_SIDE_A	= 1,
	DVD_SIDE_B	= 2
    }	DVD_DISC_SIDE;

typedef 
enum tagDVD_PREFERRED_DISPLAY_MODE
    {	DISPLAY_CONTENT_DEFAULT	= 0,
	DISPLAY_16x9	= 1,
	DISPLAY_4x3_PANSCAN_PREFERRED	= 2,
	DISPLAY_4x3_LETTERBOX_PREFERRED	= 3
    }	DVD_PREFERRED_DISPLAY_MODE;

typedef WORD DVD_REGISTER;

typedef DVD_REGISTER __RPC_FAR GPRMARRAY[ 16 ];

typedef DVD_REGISTER __RPC_FAR SPRMARRAY[ 24 ];

typedef struct tagDVD_ATR
    {
    ULONG ulCAT;
    BYTE pbATRI[ 768 ];
    }	DVD_ATR;

typedef BYTE __RPC_FAR DVD_VideoATR[ 2 ];

typedef BYTE __RPC_FAR DVD_AudioATR[ 8 ];

typedef BYTE __RPC_FAR DVD_SubpictureATR[ 6 ];

typedef 
enum tagDVD_FRAMERATE
    {	DVD_FPS_25	= 1,
	DVD_FPS_30NonDrop	= 3
    }	DVD_FRAMERATE;

typedef struct tagDVD_TIMECODE
{
   ULONG Hours1    :4; // Hours
   ULONG Hours10  :4; // Tens of Hours 

   ULONG Minutes1  :4; // Minutes 
   ULONG Minutes10:4; // Tens of Minutes 

   ULONG Seconds1  :4; // Seconds 
   ULONG Seconds10:4; // Tens of Seconds 

   ULONG Frames1   :4; // Frames 
   ULONG Frames10 :2; // Tens of Frames 

   ULONG FrameRateCode: 2; // use DVD_FRAMERATE to indicate frames/sec and drop/non-drop
} DVD_TIMECODE;
typedef 
enum tagDVD_TIMECODE_FLAGS
    {	DVD_TC_FLAG_25fps	= 0x1,
	DVD_TC_FLAG_30fps	= 0x2,
	DVD_TC_FLAG_DropFrame	= 0x4,
	DVD_TC_FLAG_Interpolated	= 0x8
    }	DVD_TIMECODE_FLAGS;

typedef struct tagDVD_HMSF_TIMECODE
    {
    BYTE bHours;
    BYTE bMinutes;
    BYTE bSeconds;
    BYTE bFrames;
    }	DVD_HMSF_TIMECODE;

typedef struct tagDVD_PLAYBACK_LOCATION2
    {
    ULONG TitleNum;
    ULONG ChapterNum;
    DVD_HMSF_TIMECODE TimeCode;
    ULONG TimeCodeFlags;
    }	DVD_PLAYBACK_LOCATION2;

typedef struct tagDVD_PLAYBACK_LOCATION
    {
    ULONG TitleNum;
    ULONG ChapterNum;
    ULONG TimeCode;
    }	DVD_PLAYBACK_LOCATION;

typedef DWORD VALID_UOP_SOMTHING_OR_OTHER;

typedef /* [public] */ 
enum __MIDL___MIDL_itf_strmif_0227_0001
    {	UOP_FLAG_Play_Title_Or_AtTime	= 0x1,
	UOP_FLAG_Play_Chapter	= 0x2,
	UOP_FLAG_Play_Title	= 0x4,
	UOP_FLAG_Stop	= 0x8,
	UOP_FLAG_ReturnFromSubMenu	= 0x10,
	UOP_FLAG_Play_Chapter_Or_AtTime	= 0x20,
	UOP_FLAG_PlayPrev_Or_Replay_Chapter	= 0x40,
	UOP_FLAG_PlayNext_Chapter	= 0x80,
	UOP_FLAG_Play_Forwards	= 0x100,
	UOP_FLAG_Play_Backwards	= 0x200,
	UOP_FLAG_ShowMenu_Title	= 0x400,
	UOP_FLAG_ShowMenu_Root	= 0x800,
	UOP_FLAG_ShowMenu_SubPic	= 0x1000,
	UOP_FLAG_ShowMenu_Audio	= 0x2000,
	UOP_FLAG_ShowMenu_Angle	= 0x4000,
	UOP_FLAG_ShowMenu_Chapter	= 0x8000,
	UOP_FLAG_Resume	= 0x10000,
	UOP_FLAG_Select_Or_Activate_Button	= 0x20000,
	UOP_FLAG_Still_Off	= 0x40000,
	UOP_FLAG_Pause_On	= 0x80000,
	UOP_FLAG_Select_Audio_Stream	= 0x100000,
	UOP_FLAG_Select_SubPic_Stream	= 0x200000,
	UOP_FLAG_Select_Angle	= 0x400000,
	UOP_FLAG_Select_Karaoke_Audio_Presentation_Mode	= 0x800000,
	UOP_FLAG_Select_Video_Mode_Preference	= 0x1000000
    }	VALID_UOP_FLAG;

typedef /* [public] */ 
enum __MIDL___MIDL_itf_strmif_0227_0002
    {	DVD_CMD_FLAG_None	= 0,
	DVD_CMD_FLAG_Flush	= 0x1,
	DVD_CMD_FLAG_SendEvents	= 0x2,
	DVD_CMD_FLAG_Block	= 0x4,
	DVD_CMD_FLAG_StartWhenRendered	= 0x8,
	DVD_CMD_FLAG_EndAfterRendered	= 0x10
    }	DVD_CMD_FLAGS;

typedef /* [public][public] */ 
enum __MIDL___MIDL_itf_strmif_0227_0003
    {	DVD_ResetOnStop	= 1,
	DVD_NotifyParentalLevelChange	= 2,
	DVD_HMSF_TimeCodeEvents	= 3
    }	DVD_OPTION_FLAG;

typedef /* [public][public] */ 
enum __MIDL___MIDL_itf_strmif_0227_0004
    {	DVD_Relative_Upper	= 1,
	DVD_Relative_Lower	= 2,
	DVD_Relative_Left	= 3,
	DVD_Relative_Right	= 4
    }	DVD_RELATIVE_BUTTON;

typedef 
enum tagDVD_PARENTAL_LEVEL
    {	DVD_PARENTAL_LEVEL_8	= 0x8000,
	DVD_PARENTAL_LEVEL_7	= 0x4000,
	DVD_PARENTAL_LEVEL_6	= 0x2000,
	DVD_PARENTAL_LEVEL_5	= 0x1000,
	DVD_PARENTAL_LEVEL_4	= 0x800,
	DVD_PARENTAL_LEVEL_3	= 0x400,
	DVD_PARENTAL_LEVEL_2	= 0x200,
	DVD_PARENTAL_LEVEL_1	= 0x100
    }	DVD_PARENTAL_LEVEL;

typedef 
enum tagDVD_AUDIO_LANG_EXT
    {	DVD_AUD_EXT_NotSpecified	= 0,
	DVD_AUD_EXT_Captions	= 1,
	DVD_AUD_EXT_VisuallyImpaired	= 2,
	DVD_AUD_EXT_DirectorComments1	= 3,
	DVD_AUD_EXT_DirectorComments2	= 4
    }	DVD_AUDIO_LANG_EXT;

typedef 
enum tagDVD_SUBPICTURE_LANG_EXT
    {	DVD_SP_EXT_NotSpecified	= 0,
	DVD_SP_EXT_Caption_Normal	= 1,
	DVD_SP_EXT_Caption_Big	= 2,
	DVD_SP_EXT_Caption_Children	= 3,
	DVD_SP_EXT_CC_Normal	= 5,
	DVD_SP_EXT_CC_Big	= 6,
	DVD_SP_EXT_CC_Children	= 7,
	DVD_SP_EXT_Forced	= 9,
	DVD_SP_EXT_DirectorComments_Normal	= 13,
	DVD_SP_EXT_DirectorComments_Big	= 14,
	DVD_SP_EXT_DirectorComments_Children	= 15
    }	DVD_SUBPICTURE_LANG_EXT;

typedef 
enum tagDVD_AUDIO_APPMODE
    {	DVD_AudioMode_None	= 0,
	DVD_AudioMode_Karaoke	= 1,
	DVD_AudioMode_Surround	= 2,
	DVD_AudioMode_Other	= 3
    }	DVD_AUDIO_APPMODE;

typedef 
enum tagDVD_AUDIO_FORMAT
    {	DVD_AudioFormat_AC3	= 0,
	DVD_AudioFormat_MPEG1	= 1,
	DVD_AudioFormat_MPEG1_DRC	= 2,
	DVD_AudioFormat_MPEG2	= 3,
	DVD_AudioFormat_MPEG2_DRC	= 4,
	DVD_AudioFormat_LPCM	= 5,
	DVD_AudioFormat_DTS	= 6,
	DVD_AudioFormat_SDDS	= 7,
	DVD_AudioFormat_Other	= 8
    }	DVD_AUDIO_FORMAT;

typedef 
enum tagDVD_KARAOKE_DOWNMIX
    {	DVD_Mix_0to0	= 0x1,
	DVD_Mix_1to0	= 0x2,
	DVD_Mix_2to0	= 0x4,
	DVD_Mix_3to0	= 0x8,
	DVD_Mix_4to0	= 0x10,
	DVD_Mix_Lto0	= 0x20,
	DVD_Mix_Rto0	= 0x40,
	DVD_Mix_0to1	= 0x100,
	DVD_Mix_1to1	= 0x200,
	DVD_Mix_2to1	= 0x400,
	DVD_Mix_3to1	= 0x800,
	DVD_Mix_4to1	= 0x1000,
	DVD_Mix_Lto1	= 0x2000,
	DVD_Mix_Rto1	= 0x4000
    }	DVD_KARAOKE_DOWNMIX;

typedef struct tagDVD_AudioAttributes
    {
    DVD_AUDIO_APPMODE AppMode;
    BYTE AppModeData;
    DVD_AUDIO_FORMAT AudioFormat;
    LCID Language;
    DVD_AUDIO_LANG_EXT LanguageExtension;
    BOOL fHasMultichannelInfo;
    DWORD dwFrequency;
    BYTE bQuantization;
    BYTE bNumberOfChannels;
    DWORD dwReserved[ 2 ];
    }	DVD_AudioAttributes;

typedef struct tagDVD_MUA_MixingInfo
    {
    BOOL fMixTo0;
    BOOL fMixTo1;
    BOOL fMix0InPhase;
    BOOL fMix1InPhase;
    DWORD dwSpeakerPosition;
    }	DVD_MUA_MixingInfo;

typedef struct tagDVD_MUA_Coeff
    {
    double log2_alpha;
    double log2_beta;
    }	DVD_MUA_Coeff;

typedef struct tagDVD_MultichannelAudioAttributes
    {
    DVD_MUA_MixingInfo Info[ 8 ];
    DVD_MUA_Coeff Coeff[ 8 ];
    }	DVD_MultichannelAudioAttributes;

typedef 
enum tagDVD_KARAOKE_CONTENTS
    {	DVD_Karaoke_GuideVocal1	= 0x1,
	DVD_Karaoke_GuideVocal2	= 0x2,
	DVD_Karaoke_GuideMelody1	= 0x4,
	DVD_Karaoke_GuideMelody2	= 0x8,
	DVD_Karaoke_GuideMelodyA	= 0x10,
	DVD_Karaoke_GuideMelodyB	= 0x20,
	DVD_Karaoke_SoundEffectA	= 0x40,
	DVD_Karaoke_SoundEffectB	= 0x80
    }	DVD_KARAOKE_CONTENTS;

typedef 
enum tagDVD_KARAOKE_ASSIGNMENT
    {	DVD_Assignment_reserved0	= 0,
	DVD_Assignment_reserved1	= 1,
	DVD_Assignment_LR	= 2,
	DVD_Assignment_LRM	= 3,
	DVD_Assignment_LR1	= 4,
	DVD_Assignment_LRM1	= 5,
	DVD_Assignment_LR12	= 6,
	DVD_Assignment_LRM12	= 7
    }	DVD_KARAOKE_ASSIGNMENT;

typedef struct tagDVD_KaraokeAttributes
    {
    BYTE bVersion;
    BOOL fMasterOfCeremoniesInGuideVocal1;
    BOOL fDuet;
    DVD_KARAOKE_ASSIGNMENT ChannelAssignment;
    WORD wChannelContents[ 8 ];
    }	DVD_KaraokeAttributes;

typedef 
enum tagDVD_VIDEO_COMPRESSION
    {	DVD_VideoCompression_Other	= 0,
	DVD_VideoCompression_MPEG1	= 1,
	DVD_VideoCompression_MPEG2	= 2
    }	DVD_VIDEO_COMPRESSION;

typedef struct tagDVD_VideoAttributes
    {
    BOOL fPanscanPermitted;
    BOOL fLetterboxPermitted;
    ULONG ulAspectX;
    ULONG ulAspectY;
    ULONG ulFrameRate;
    ULONG ulFrameHeight;
    DVD_VIDEO_COMPRESSION Compression;
    BOOL fLine21Field1InGOP;
    BOOL fLine21Field2InGOP;
    ULONG ulSourceResolutionX;
    ULONG ulSourceResolutionY;
    BOOL fIsSourceLetterboxed;
    BOOL fIsFilmMode;
    }	DVD_VideoAttributes;

typedef 
enum tagDVD_SUBPICTURE_TYPE
    {	DVD_SPType_NotSpecified	= 0,
	DVD_SPType_Language	= 1,
	DVD_SPType_Other	= 2
    }	DVD_SUBPICTURE_TYPE;

typedef 
enum tagDVD_SUBPICTURE_CODING
    {	DVD_SPCoding_RunLength	= 0,
	DVD_SPCoding_Extended	= 1,
	DVD_SPCoding_Other	= 2
    }	DVD_SUBPICTURE_CODING;

typedef struct tagDVD_SubpictureAttributes
    {
    DVD_SUBPICTURE_TYPE Type;
    DVD_SUBPICTURE_CODING CodingMode;
    LCID Language;
    DVD_SUBPICTURE_LANG_EXT LanguageExtension;
    }	DVD_SubpictureAttributes;

typedef 
enum tagDVD_TITLE_APPMODE
    {	DVD_AppMode_Not_Specified	= 0,
	DVD_AppMode_Karaoke	= 1,
	DVD_AppMode_Other	= 3
    }	DVD_TITLE_APPMODE;

typedef struct tagDVD_TitleMainAttributes
    {
    DVD_TITLE_APPMODE AppMode;
    DVD_VideoAttributes VideoAttributes;
    ULONG ulNumberOfAudioStreams;
    DVD_AudioAttributes AudioAttributes[ 8 ];
    DVD_MultichannelAudioAttributes MultichannelAudioAttributes[ 8 ];
    ULONG ulNumberOfSubpictureStreams;
    DVD_SubpictureAttributes SubpictureAttributes[ 32 ];
    }	DVD_TitleAttributes;

typedef struct tagDVD_MenuAttributes
    {
    BOOL fCompatibleRegion[ 8 ];
    DVD_VideoAttributes VideoAttributes;
    BOOL fAudioPresent;
    DVD_AudioAttributes AudioAttributes;
    BOOL fSubpicturePresent;
    DVD_SubpictureAttributes SubpictureAttributes;
    }	DVD_MenuAttributes;



extern RPC_IF_HANDLE __MIDL_itf_strmif_0227_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_strmif_0227_v0_0_s_ifspec;

#ifndef __IDvdControl_INTERFACE_DEFINED__
#define __IDvdControl_INTERFACE_DEFINED__

/* interface IDvdControl */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IDvdControl;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("A70EFE61-E2A3-11d0-A9BE-00AA0061BE93")
    IDvdControl : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE TitlePlay( 
            /* [in] */ ULONG uiTitle) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ChapterPlay( 
            /* [in] */ ULONG uiTitle,
            /* [in] */ ULONG uiChapter) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE TimePlay( 
            /* [in] */ ULONG uiTitle,
            /* [in] */ ULONG bcdTime) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE StopForResume( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GoUp( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE TimeSearch( 
            /* [in] */ ULONG bcdTime) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ChapterSearch( 
            /* [in] */ ULONG Chapter) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE PrevPGSearch( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE TopPGSearch( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE NextPGSearch( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ForwardScan( 
            /* [in] */ double dwSpeed) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE BackwardScan( 
            /* [in] */ double dwSpeed) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MenuCall( 
            /* [in] */ DVD_MENU_ID MenuID) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Resume( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE UpperButtonSelect( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE LowerButtonSelect( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE LeftButtonSelect( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RightButtonSelect( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ButtonActivate( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ButtonSelectAndActivate( 
            /* [in] */ ULONG uiButton) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE StillOff( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE PauseOn( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE PauseOff( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MenuLanguageSelect( 
            /* [in] */ LCID Language) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AudioStreamChange( 
            /* [in] */ ULONG nAudio) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SubpictureStreamChange( 
            /* [in] */ ULONG nSubPicture,
            /* [in] */ BOOL bDisplay) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AngleChange( 
            /* [in] */ ULONG ulAngle) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ParentalLevelSelect( 
            /* [in] */ ULONG ulParentalLevel) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ParentalCountrySelect( 
            /* [in] */ WORD wCountry) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE KaraokeAudioPresentationModeChange( 
            /* [in] */ ULONG ulMode) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE VideoModePreferrence( 
            /* [in] */ ULONG ulPreferredDisplayMode) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetRoot( 
            /* [in] */ LPCWSTR pszPath) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MouseActivate( 
            /* [in] */ POINT point) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MouseSelect( 
            /* [in] */ POINT point) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ChapterPlayAutoStop( 
            /* [in] */ ULONG ulTitle,
            /* [in] */ ULONG ulChapter,
            /* [in] */ ULONG ulChaptersToPlay) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDvdControlVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IDvdControl __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IDvdControl __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IDvdControl __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *TitlePlay )( 
            IDvdControl __RPC_FAR * This,
            /* [in] */ ULONG uiTitle);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ChapterPlay )( 
            IDvdControl __RPC_FAR * This,
            /* [in] */ ULONG uiTitle,
            /* [in] */ ULONG uiChapter);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *TimePlay )( 
            IDvdControl __RPC_FAR * This,
            /* [in] */ ULONG uiTitle,
            /* [in] */ ULONG bcdTime);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *StopForResume )( 
            IDvdControl __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GoUp )( 
            IDvdControl __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *TimeSearch )( 
            IDvdControl __RPC_FAR * This,
            /* [in] */ ULONG bcdTime);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ChapterSearch )( 
            IDvdControl __RPC_FAR * This,
            /* [in] */ ULONG Chapter);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PrevPGSearch )( 
            IDvdControl __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *TopPGSearch )( 
            IDvdControl __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *NextPGSearch )( 
            IDvdControl __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ForwardScan )( 
            IDvdControl __RPC_FAR * This,
            /* [in] */ double dwSpeed);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *BackwardScan )( 
            IDvdControl __RPC_FAR * This,
            /* [in] */ double dwSpeed);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MenuCall )( 
            IDvdControl __RPC_FAR * This,
            /* [in] */ DVD_MENU_ID MenuID);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Resume )( 
            IDvdControl __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *UpperButtonSelect )( 
            IDvdControl __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *LowerButtonSelect )( 
            IDvdControl __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *LeftButtonSelect )( 
            IDvdControl __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RightButtonSelect )( 
            IDvdControl __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ButtonActivate )( 
            IDvdControl __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ButtonSelectAndActivate )( 
            IDvdControl __RPC_FAR * This,
            /* [in] */ ULONG uiButton);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *StillOff )( 
            IDvdControl __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PauseOn )( 
            IDvdControl __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PauseOff )( 
            IDvdControl __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MenuLanguageSelect )( 
            IDvdControl __RPC_FAR * This,
            /* [in] */ LCID Language);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AudioStreamChange )( 
            IDvdControl __RPC_FAR * This,
            /* [in] */ ULONG nAudio);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SubpictureStreamChange )( 
            IDvdControl __RPC_FAR * This,
            /* [in] */ ULONG nSubPicture,
            /* [in] */ BOOL bDisplay);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AngleChange )( 
            IDvdControl __RPC_FAR * This,
            /* [in] */ ULONG ulAngle);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ParentalLevelSelect )( 
            IDvdControl __RPC_FAR * This,
            /* [in] */ ULONG ulParentalLevel);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ParentalCountrySelect )( 
            IDvdControl __RPC_FAR * This,
            /* [in] */ WORD wCountry);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *KaraokeAudioPresentationModeChange )( 
            IDvdControl __RPC_FAR * This,
            /* [in] */ ULONG ulMode);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *VideoModePreferrence )( 
            IDvdControl __RPC_FAR * This,
            /* [in] */ ULONG ulPreferredDisplayMode);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetRoot )( 
            IDvdControl __RPC_FAR * This,
            /* [in] */ LPCWSTR pszPath);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MouseActivate )( 
            IDvdControl __RPC_FAR * This,
            /* [in] */ POINT point);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MouseSelect )( 
            IDvdControl __RPC_FAR * This,
            /* [in] */ POINT point);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ChapterPlayAutoStop )( 
            IDvdControl __RPC_FAR * This,
            /* [in] */ ULONG ulTitle,
            /* [in] */ ULONG ulChapter,
            /* [in] */ ULONG ulChaptersToPlay);
        
        END_INTERFACE
    } IDvdControlVtbl;

    interface IDvdControl
    {
        CONST_VTBL struct IDvdControlVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDvdControl_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDvdControl_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDvdControl_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDvdControl_TitlePlay(This,uiTitle)	\
    (This)->lpVtbl -> TitlePlay(This,uiTitle)

#define IDvdControl_ChapterPlay(This,uiTitle,uiChapter)	\
    (This)->lpVtbl -> ChapterPlay(This,uiTitle,uiChapter)

#define IDvdControl_TimePlay(This,uiTitle,bcdTime)	\
    (This)->lpVtbl -> TimePlay(This,uiTitle,bcdTime)

#define IDvdControl_StopForResume(This)	\
    (This)->lpVtbl -> StopForResume(This)

#define IDvdControl_GoUp(This)	\
    (This)->lpVtbl -> GoUp(This)

#define IDvdControl_TimeSearch(This,bcdTime)	\
    (This)->lpVtbl -> TimeSearch(This,bcdTime)

#define IDvdControl_ChapterSearch(This,Chapter)	\
    (This)->lpVtbl -> ChapterSearch(This,Chapter)

#define IDvdControl_PrevPGSearch(This)	\
    (This)->lpVtbl -> PrevPGSearch(This)

#define IDvdControl_TopPGSearch(This)	\
    (This)->lpVtbl -> TopPGSearch(This)

#define IDvdControl_NextPGSearch(This)	\
    (This)->lpVtbl -> NextPGSearch(This)

#define IDvdControl_ForwardScan(This,dwSpeed)	\
    (This)->lpVtbl -> ForwardScan(This,dwSpeed)

#define IDvdControl_BackwardScan(This,dwSpeed)	\
    (This)->lpVtbl -> BackwardScan(This,dwSpeed)

#define IDvdControl_MenuCall(This,MenuID)	\
    (This)->lpVtbl -> MenuCall(This,MenuID)

#define IDvdControl_Resume(This)	\
    (This)->lpVtbl -> Resume(This)

#define IDvdControl_UpperButtonSelect(This)	\
    (This)->lpVtbl -> UpperButtonSelect(This)

#define IDvdControl_LowerButtonSelect(This)	\
    (This)->lpVtbl -> LowerButtonSelect(This)

#define IDvdControl_LeftButtonSelect(This)	\
    (This)->lpVtbl -> LeftButtonSelect(This)

#define IDvdControl_RightButtonSelect(This)	\
    (This)->lpVtbl -> RightButtonSelect(This)

#define IDvdControl_ButtonActivate(This)	\
    (This)->lpVtbl -> ButtonActivate(This)

#define IDvdControl_ButtonSelectAndActivate(This,uiButton)	\
    (This)->lpVtbl -> ButtonSelectAndActivate(This,uiButton)

#define IDvdControl_StillOff(This)	\
    (This)->lpVtbl -> StillOff(This)

#define IDvdControl_PauseOn(This)	\
    (This)->lpVtbl -> PauseOn(This)

#define IDvdControl_PauseOff(This)	\
    (This)->lpVtbl -> PauseOff(This)

#define IDvdControl_MenuLanguageSelect(This,Language)	\
    (This)->lpVtbl -> MenuLanguageSelect(This,Language)

#define IDvdControl_AudioStreamChange(This,nAudio)	\
    (This)->lpVtbl -> AudioStreamChange(This,nAudio)

#define IDvdControl_SubpictureStreamChange(This,nSubPicture,bDisplay)	\
    (This)->lpVtbl -> SubpictureStreamChange(This,nSubPicture,bDisplay)

#define IDvdControl_AngleChange(This,ulAngle)	\
    (This)->lpVtbl -> AngleChange(This,ulAngle)

#define IDvdControl_ParentalLevelSelect(This,ulParentalLevel)	\
    (This)->lpVtbl -> ParentalLevelSelect(This,ulParentalLevel)

#define IDvdControl_ParentalCountrySelect(This,wCountry)	\
    (This)->lpVtbl -> ParentalCountrySelect(This,wCountry)

#define IDvdControl_KaraokeAudioPresentationModeChange(This,ulMode)	\
    (This)->lpVtbl -> KaraokeAudioPresentationModeChange(This,ulMode)

#define IDvdControl_VideoModePreferrence(This,ulPreferredDisplayMode)	\
    (This)->lpVtbl -> VideoModePreferrence(This,ulPreferredDisplayMode)

#define IDvdControl_SetRoot(This,pszPath)	\
    (This)->lpVtbl -> SetRoot(This,pszPath)

#define IDvdControl_MouseActivate(This,point)	\
    (This)->lpVtbl -> MouseActivate(This,point)

#define IDvdControl_MouseSelect(This,point)	\
    (This)->lpVtbl -> MouseSelect(This,point)

#define IDvdControl_ChapterPlayAutoStop(This,ulTitle,ulChapter,ulChaptersToPlay)	\
    (This)->lpVtbl -> ChapterPlayAutoStop(This,ulTitle,ulChapter,ulChaptersToPlay)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IDvdControl_TitlePlay_Proxy( 
    IDvdControl __RPC_FAR * This,
    /* [in] */ ULONG uiTitle);


void __RPC_STUB IDvdControl_TitlePlay_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdControl_ChapterPlay_Proxy( 
    IDvdControl __RPC_FAR * This,
    /* [in] */ ULONG uiTitle,
    /* [in] */ ULONG uiChapter);


void __RPC_STUB IDvdControl_ChapterPlay_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdControl_TimePlay_Proxy( 
    IDvdControl __RPC_FAR * This,
    /* [in] */ ULONG uiTitle,
    /* [in] */ ULONG bcdTime);


void __RPC_STUB IDvdControl_TimePlay_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdControl_StopForResume_Proxy( 
    IDvdControl __RPC_FAR * This);


void __RPC_STUB IDvdControl_StopForResume_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdControl_GoUp_Proxy( 
    IDvdControl __RPC_FAR * This);


void __RPC_STUB IDvdControl_GoUp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdControl_TimeSearch_Proxy( 
    IDvdControl __RPC_FAR * This,
    /* [in] */ ULONG bcdTime);


void __RPC_STUB IDvdControl_TimeSearch_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdControl_ChapterSearch_Proxy( 
    IDvdControl __RPC_FAR * This,
    /* [in] */ ULONG Chapter);


void __RPC_STUB IDvdControl_ChapterSearch_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdControl_PrevPGSearch_Proxy( 
    IDvdControl __RPC_FAR * This);


void __RPC_STUB IDvdControl_PrevPGSearch_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdControl_TopPGSearch_Proxy( 
    IDvdControl __RPC_FAR * This);


void __RPC_STUB IDvdControl_TopPGSearch_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdControl_NextPGSearch_Proxy( 
    IDvdControl __RPC_FAR * This);


void __RPC_STUB IDvdControl_NextPGSearch_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdControl_ForwardScan_Proxy( 
    IDvdControl __RPC_FAR * This,
    /* [in] */ double dwSpeed);


void __RPC_STUB IDvdControl_ForwardScan_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdControl_BackwardScan_Proxy( 
    IDvdControl __RPC_FAR * This,
    /* [in] */ double dwSpeed);


void __RPC_STUB IDvdControl_BackwardScan_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdControl_MenuCall_Proxy( 
    IDvdControl __RPC_FAR * This,
    /* [in] */ DVD_MENU_ID MenuID);


void __RPC_STUB IDvdControl_MenuCall_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdControl_Resume_Proxy( 
    IDvdControl __RPC_FAR * This);


void __RPC_STUB IDvdControl_Resume_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdControl_UpperButtonSelect_Proxy( 
    IDvdControl __RPC_FAR * This);


void __RPC_STUB IDvdControl_UpperButtonSelect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdControl_LowerButtonSelect_Proxy( 
    IDvdControl __RPC_FAR * This);


void __RPC_STUB IDvdControl_LowerButtonSelect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdControl_LeftButtonSelect_Proxy( 
    IDvdControl __RPC_FAR * This);


void __RPC_STUB IDvdControl_LeftButtonSelect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdControl_RightButtonSelect_Proxy( 
    IDvdControl __RPC_FAR * This);


void __RPC_STUB IDvdControl_RightButtonSelect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdControl_ButtonActivate_Proxy( 
    IDvdControl __RPC_FAR * This);


void __RPC_STUB IDvdControl_ButtonActivate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdControl_ButtonSelectAndActivate_Proxy( 
    IDvdControl __RPC_FAR * This,
    /* [in] */ ULONG uiButton);


void __RPC_STUB IDvdControl_ButtonSelectAndActivate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdControl_StillOff_Proxy( 
    IDvdControl __RPC_FAR * This);


void __RPC_STUB IDvdControl_StillOff_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdControl_PauseOn_Proxy( 
    IDvdControl __RPC_FAR * This);


void __RPC_STUB IDvdControl_PauseOn_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdControl_PauseOff_Proxy( 
    IDvdControl __RPC_FAR * This);


void __RPC_STUB IDvdControl_PauseOff_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdControl_MenuLanguageSelect_Proxy( 
    IDvdControl __RPC_FAR * This,
    /* [in] */ LCID Language);


void __RPC_STUB IDvdControl_MenuLanguageSelect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdControl_AudioStreamChange_Proxy( 
    IDvdControl __RPC_FAR * This,
    /* [in] */ ULONG nAudio);


void __RPC_STUB IDvdControl_AudioStreamChange_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdControl_SubpictureStreamChange_Proxy( 
    IDvdControl __RPC_FAR * This,
    /* [in] */ ULONG nSubPicture,
    /* [in] */ BOOL bDisplay);


void __RPC_STUB IDvdControl_SubpictureStreamChange_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdControl_AngleChange_Proxy( 
    IDvdControl __RPC_FAR * This,
    /* [in] */ ULONG ulAngle);


void __RPC_STUB IDvdControl_AngleChange_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdControl_ParentalLevelSelect_Proxy( 
    IDvdControl __RPC_FAR * This,
    /* [in] */ ULONG ulParentalLevel);


void __RPC_STUB IDvdControl_ParentalLevelSelect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdControl_ParentalCountrySelect_Proxy( 
    IDvdControl __RPC_FAR * This,
    /* [in] */ WORD wCountry);


void __RPC_STUB IDvdControl_ParentalCountrySelect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdControl_KaraokeAudioPresentationModeChange_Proxy( 
    IDvdControl __RPC_FAR * This,
    /* [in] */ ULONG ulMode);


void __RPC_STUB IDvdControl_KaraokeAudioPresentationModeChange_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdControl_VideoModePreferrence_Proxy( 
    IDvdControl __RPC_FAR * This,
    /* [in] */ ULONG ulPreferredDisplayMode);


void __RPC_STUB IDvdControl_VideoModePreferrence_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdControl_SetRoot_Proxy( 
    IDvdControl __RPC_FAR * This,
    /* [in] */ LPCWSTR pszPath);


void __RPC_STUB IDvdControl_SetRoot_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdControl_MouseActivate_Proxy( 
    IDvdControl __RPC_FAR * This,
    /* [in] */ POINT point);


void __RPC_STUB IDvdControl_MouseActivate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdControl_MouseSelect_Proxy( 
    IDvdControl __RPC_FAR * This,
    /* [in] */ POINT point);


void __RPC_STUB IDvdControl_MouseSelect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdControl_ChapterPlayAutoStop_Proxy( 
    IDvdControl __RPC_FAR * This,
    /* [in] */ ULONG ulTitle,
    /* [in] */ ULONG ulChapter,
    /* [in] */ ULONG ulChaptersToPlay);


void __RPC_STUB IDvdControl_ChapterPlayAutoStop_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDvdControl_INTERFACE_DEFINED__ */


#ifndef __IDvdInfo_INTERFACE_DEFINED__
#define __IDvdInfo_INTERFACE_DEFINED__

/* interface IDvdInfo */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IDvdInfo;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("A70EFE60-E2A3-11d0-A9BE-00AA0061BE93")
    IDvdInfo : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetCurrentDomain( 
            /* [out] */ DVD_DOMAIN __RPC_FAR *pDomain) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetCurrentLocation( 
            /* [out] */ DVD_PLAYBACK_LOCATION __RPC_FAR *pLocation) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetTotalTitleTime( 
            /* [out] */ ULONG __RPC_FAR *pTotalTime) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetCurrentButton( 
            /* [out] */ ULONG __RPC_FAR *pnButtonsAvailable,
            /* [out] */ ULONG __RPC_FAR *pnCurrentButton) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetCurrentAngle( 
            /* [out] */ ULONG __RPC_FAR *pnAnglesAvailable,
            /* [out] */ ULONG __RPC_FAR *pnCurrentAngle) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetCurrentAudio( 
            /* [out] */ ULONG __RPC_FAR *pnStreamsAvailable,
            /* [out] */ ULONG __RPC_FAR *pnCurrentStream) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetCurrentSubpicture( 
            /* [out] */ ULONG __RPC_FAR *pnStreamsAvailable,
            /* [out] */ ULONG __RPC_FAR *pnCurrentStream,
            /* [out] */ BOOL __RPC_FAR *pIsDisabled) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetCurrentUOPS( 
            /* [out] */ VALID_UOP_SOMTHING_OR_OTHER __RPC_FAR *pUOP) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetAllSPRMs( 
            /* [out] */ SPRMARRAY __RPC_FAR *pRegisterArray) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetAllGPRMs( 
            /* [out] */ GPRMARRAY __RPC_FAR *pRegisterArray) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetAudioLanguage( 
            /* [in] */ ULONG nStream,
            /* [out] */ LCID __RPC_FAR *pLanguage) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetSubpictureLanguage( 
            /* [in] */ ULONG nStream,
            /* [out] */ LCID __RPC_FAR *pLanguage) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetTitleAttributes( 
            /* [in] */ ULONG nTitle,
            /* [out] */ DVD_ATR __RPC_FAR *pATR) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetVMGAttributes( 
            /* [out] */ DVD_ATR __RPC_FAR *pATR) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetCurrentVideoAttributes( 
            /* [out] */ DVD_VideoATR __RPC_FAR *pATR) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetCurrentAudioAttributes( 
            /* [out] */ DVD_AudioATR __RPC_FAR *pATR) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetCurrentSubpictureAttributes( 
            /* [out] */ DVD_SubpictureATR __RPC_FAR *pATR) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetCurrentVolumeInfo( 
            /* [out] */ ULONG __RPC_FAR *pNumOfVol,
            /* [out] */ ULONG __RPC_FAR *pThisVolNum,
            /* [out] */ DVD_DISC_SIDE __RPC_FAR *pSide,
            /* [out] */ ULONG __RPC_FAR *pNumOfTitles) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetDVDTextInfo( 
            /* [size_is][out] */ BYTE __RPC_FAR *pTextManager,
            /* [in] */ ULONG cbBufSize,
            /* [out] */ ULONG __RPC_FAR *pcbActualSize) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetPlayerParentalLevel( 
            /* [out] */ ULONG __RPC_FAR *pParentalLevel,
            /* [out] */ ULONG __RPC_FAR *pCountryCode) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetNumberOfChapters( 
            /* [in] */ ULONG ulTitle,
            /* [out] */ ULONG __RPC_FAR *pNumberOfChapters) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetTitleParentalLevels( 
            /* [in] */ ULONG ulTitle,
            /* [out] */ ULONG __RPC_FAR *pParentalLevels) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetRoot( 
            /* [size_is][out] */ LPSTR pRoot,
            /* [in] */ ULONG cbBufSize,
            /* [out] */ ULONG __RPC_FAR *pcbActualSize) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDvdInfoVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IDvdInfo __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IDvdInfo __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IDvdInfo __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetCurrentDomain )( 
            IDvdInfo __RPC_FAR * This,
            /* [out] */ DVD_DOMAIN __RPC_FAR *pDomain);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetCurrentLocation )( 
            IDvdInfo __RPC_FAR * This,
            /* [out] */ DVD_PLAYBACK_LOCATION __RPC_FAR *pLocation);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTotalTitleTime )( 
            IDvdInfo __RPC_FAR * This,
            /* [out] */ ULONG __RPC_FAR *pTotalTime);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetCurrentButton )( 
            IDvdInfo __RPC_FAR * This,
            /* [out] */ ULONG __RPC_FAR *pnButtonsAvailable,
            /* [out] */ ULONG __RPC_FAR *pnCurrentButton);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetCurrentAngle )( 
            IDvdInfo __RPC_FAR * This,
            /* [out] */ ULONG __RPC_FAR *pnAnglesAvailable,
            /* [out] */ ULONG __RPC_FAR *pnCurrentAngle);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetCurrentAudio )( 
            IDvdInfo __RPC_FAR * This,
            /* [out] */ ULONG __RPC_FAR *pnStreamsAvailable,
            /* [out] */ ULONG __RPC_FAR *pnCurrentStream);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetCurrentSubpicture )( 
            IDvdInfo __RPC_FAR * This,
            /* [out] */ ULONG __RPC_FAR *pnStreamsAvailable,
            /* [out] */ ULONG __RPC_FAR *pnCurrentStream,
            /* [out] */ BOOL __RPC_FAR *pIsDisabled);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetCurrentUOPS )( 
            IDvdInfo __RPC_FAR * This,
            /* [out] */ VALID_UOP_SOMTHING_OR_OTHER __RPC_FAR *pUOP);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetAllSPRMs )( 
            IDvdInfo __RPC_FAR * This,
            /* [out] */ SPRMARRAY __RPC_FAR *pRegisterArray);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetAllGPRMs )( 
            IDvdInfo __RPC_FAR * This,
            /* [out] */ GPRMARRAY __RPC_FAR *pRegisterArray);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetAudioLanguage )( 
            IDvdInfo __RPC_FAR * This,
            /* [in] */ ULONG nStream,
            /* [out] */ LCID __RPC_FAR *pLanguage);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetSubpictureLanguage )( 
            IDvdInfo __RPC_FAR * This,
            /* [in] */ ULONG nStream,
            /* [out] */ LCID __RPC_FAR *pLanguage);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTitleAttributes )( 
            IDvdInfo __RPC_FAR * This,
            /* [in] */ ULONG nTitle,
            /* [out] */ DVD_ATR __RPC_FAR *pATR);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetVMGAttributes )( 
            IDvdInfo __RPC_FAR * This,
            /* [out] */ DVD_ATR __RPC_FAR *pATR);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetCurrentVideoAttributes )( 
            IDvdInfo __RPC_FAR * This,
            /* [out] */ DVD_VideoATR __RPC_FAR *pATR);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetCurrentAudioAttributes )( 
            IDvdInfo __RPC_FAR * This,
            /* [out] */ DVD_AudioATR __RPC_FAR *pATR);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetCurrentSubpictureAttributes )( 
            IDvdInfo __RPC_FAR * This,
            /* [out] */ DVD_SubpictureATR __RPC_FAR *pATR);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetCurrentVolumeInfo )( 
            IDvdInfo __RPC_FAR * This,
            /* [out] */ ULONG __RPC_FAR *pNumOfVol,
            /* [out] */ ULONG __RPC_FAR *pThisVolNum,
            /* [out] */ DVD_DISC_SIDE __RPC_FAR *pSide,
            /* [out] */ ULONG __RPC_FAR *pNumOfTitles);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDVDTextInfo )( 
            IDvdInfo __RPC_FAR * This,
            /* [size_is][out] */ BYTE __RPC_FAR *pTextManager,
            /* [in] */ ULONG cbBufSize,
            /* [out] */ ULONG __RPC_FAR *pcbActualSize);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetPlayerParentalLevel )( 
            IDvdInfo __RPC_FAR * This,
            /* [out] */ ULONG __RPC_FAR *pParentalLevel,
            /* [out] */ ULONG __RPC_FAR *pCountryCode);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNumberOfChapters )( 
            IDvdInfo __RPC_FAR * This,
            /* [in] */ ULONG ulTitle,
            /* [out] */ ULONG __RPC_FAR *pNumberOfChapters);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTitleParentalLevels )( 
            IDvdInfo __RPC_FAR * This,
            /* [in] */ ULONG ulTitle,
            /* [out] */ ULONG __RPC_FAR *pParentalLevels);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetRoot )( 
            IDvdInfo __RPC_FAR * This,
            /* [size_is][out] */ LPSTR pRoot,
            /* [in] */ ULONG cbBufSize,
            /* [out] */ ULONG __RPC_FAR *pcbActualSize);
        
        END_INTERFACE
    } IDvdInfoVtbl;

    interface IDvdInfo
    {
        CONST_VTBL struct IDvdInfoVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDvdInfo_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDvdInfo_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDvdInfo_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDvdInfo_GetCurrentDomain(This,pDomain)	\
    (This)->lpVtbl -> GetCurrentDomain(This,pDomain)

#define IDvdInfo_GetCurrentLocation(This,pLocation)	\
    (This)->lpVtbl -> GetCurrentLocation(This,pLocation)

#define IDvdInfo_GetTotalTitleTime(This,pTotalTime)	\
    (This)->lpVtbl -> GetTotalTitleTime(This,pTotalTime)

#define IDvdInfo_GetCurrentButton(This,pnButtonsAvailable,pnCurrentButton)	\
    (This)->lpVtbl -> GetCurrentButton(This,pnButtonsAvailable,pnCurrentButton)

#define IDvdInfo_GetCurrentAngle(This,pnAnglesAvailable,pnCurrentAngle)	\
    (This)->lpVtbl -> GetCurrentAngle(This,pnAnglesAvailable,pnCurrentAngle)

#define IDvdInfo_GetCurrentAudio(This,pnStreamsAvailable,pnCurrentStream)	\
    (This)->lpVtbl -> GetCurrentAudio(This,pnStreamsAvailable,pnCurrentStream)

#define IDvdInfo_GetCurrentSubpicture(This,pnStreamsAvailable,pnCurrentStream,pIsDisabled)	\
    (This)->lpVtbl -> GetCurrentSubpicture(This,pnStreamsAvailable,pnCurrentStream,pIsDisabled)

#define IDvdInfo_GetCurrentUOPS(This,pUOP)	\
    (This)->lpVtbl -> GetCurrentUOPS(This,pUOP)

#define IDvdInfo_GetAllSPRMs(This,pRegisterArray)	\
    (This)->lpVtbl -> GetAllSPRMs(This,pRegisterArray)

#define IDvdInfo_GetAllGPRMs(This,pRegisterArray)	\
    (This)->lpVtbl -> GetAllGPRMs(This,pRegisterArray)

#define IDvdInfo_GetAudioLanguage(This,nStream,pLanguage)	\
    (This)->lpVtbl -> GetAudioLanguage(This,nStream,pLanguage)

#define IDvdInfo_GetSubpictureLanguage(This,nStream,pLanguage)	\
    (This)->lpVtbl -> GetSubpictureLanguage(This,nStream,pLanguage)

#define IDvdInfo_GetTitleAttributes(This,nTitle,pATR)	\
    (This)->lpVtbl -> GetTitleAttributes(This,nTitle,pATR)

#define IDvdInfo_GetVMGAttributes(This,pATR)	\
    (This)->lpVtbl -> GetVMGAttributes(This,pATR)

#define IDvdInfo_GetCurrentVideoAttributes(This,pATR)	\
    (This)->lpVtbl -> GetCurrentVideoAttributes(This,pATR)

#define IDvdInfo_GetCurrentAudioAttributes(This,pATR)	\
    (This)->lpVtbl -> GetCurrentAudioAttributes(This,pATR)

#define IDvdInfo_GetCurrentSubpictureAttributes(This,pATR)	\
    (This)->lpVtbl -> GetCurrentSubpictureAttributes(This,pATR)

#define IDvdInfo_GetCurrentVolumeInfo(This,pNumOfVol,pThisVolNum,pSide,pNumOfTitles)	\
    (This)->lpVtbl -> GetCurrentVolumeInfo(This,pNumOfVol,pThisVolNum,pSide,pNumOfTitles)

#define IDvdInfo_GetDVDTextInfo(This,pTextManager,cbBufSize,pcbActualSize)	\
    (This)->lpVtbl -> GetDVDTextInfo(This,pTextManager,cbBufSize,pcbActualSize)

#define IDvdInfo_GetPlayerParentalLevel(This,pParentalLevel,pCountryCode)	\
    (This)->lpVtbl -> GetPlayerParentalLevel(This,pParentalLevel,pCountryCode)

#define IDvdInfo_GetNumberOfChapters(This,ulTitle,pNumberOfChapters)	\
    (This)->lpVtbl -> GetNumberOfChapters(This,ulTitle,pNumberOfChapters)

#define IDvdInfo_GetTitleParentalLevels(This,ulTitle,pParentalLevels)	\
    (This)->lpVtbl -> GetTitleParentalLevels(This,ulTitle,pParentalLevels)

#define IDvdInfo_GetRoot(This,pRoot,cbBufSize,pcbActualSize)	\
    (This)->lpVtbl -> GetRoot(This,pRoot,cbBufSize,pcbActualSize)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IDvdInfo_GetCurrentDomain_Proxy( 
    IDvdInfo __RPC_FAR * This,
    /* [out] */ DVD_DOMAIN __RPC_FAR *pDomain);


void __RPC_STUB IDvdInfo_GetCurrentDomain_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdInfo_GetCurrentLocation_Proxy( 
    IDvdInfo __RPC_FAR * This,
    /* [out] */ DVD_PLAYBACK_LOCATION __RPC_FAR *pLocation);


void __RPC_STUB IDvdInfo_GetCurrentLocation_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdInfo_GetTotalTitleTime_Proxy( 
    IDvdInfo __RPC_FAR * This,
    /* [out] */ ULONG __RPC_FAR *pTotalTime);


void __RPC_STUB IDvdInfo_GetTotalTitleTime_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdInfo_GetCurrentButton_Proxy( 
    IDvdInfo __RPC_FAR * This,
    /* [out] */ ULONG __RPC_FAR *pnButtonsAvailable,
    /* [out] */ ULONG __RPC_FAR *pnCurrentButton);


void __RPC_STUB IDvdInfo_GetCurrentButton_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdInfo_GetCurrentAngle_Proxy( 
    IDvdInfo __RPC_FAR * This,
    /* [out] */ ULONG __RPC_FAR *pnAnglesAvailable,
    /* [out] */ ULONG __RPC_FAR *pnCurrentAngle);


void __RPC_STUB IDvdInfo_GetCurrentAngle_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdInfo_GetCurrentAudio_Proxy( 
    IDvdInfo __RPC_FAR * This,
    /* [out] */ ULONG __RPC_FAR *pnStreamsAvailable,
    /* [out] */ ULONG __RPC_FAR *pnCurrentStream);


void __RPC_STUB IDvdInfo_GetCurrentAudio_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdInfo_GetCurrentSubpicture_Proxy( 
    IDvdInfo __RPC_FAR * This,
    /* [out] */ ULONG __RPC_FAR *pnStreamsAvailable,
    /* [out] */ ULONG __RPC_FAR *pnCurrentStream,
    /* [out] */ BOOL __RPC_FAR *pIsDisabled);


void __RPC_STUB IDvdInfo_GetCurrentSubpicture_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdInfo_GetCurrentUOPS_Proxy( 
    IDvdInfo __RPC_FAR * This,
    /* [out] */ VALID_UOP_SOMTHING_OR_OTHER __RPC_FAR *pUOP);


void __RPC_STUB IDvdInfo_GetCurrentUOPS_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdInfo_GetAllSPRMs_Proxy( 
    IDvdInfo __RPC_FAR * This,
    /* [out] */ SPRMARRAY __RPC_FAR *pRegisterArray);


void __RPC_STUB IDvdInfo_GetAllSPRMs_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdInfo_GetAllGPRMs_Proxy( 
    IDvdInfo __RPC_FAR * This,
    /* [out] */ GPRMARRAY __RPC_FAR *pRegisterArray);


void __RPC_STUB IDvdInfo_GetAllGPRMs_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdInfo_GetAudioLanguage_Proxy( 
    IDvdInfo __RPC_FAR * This,
    /* [in] */ ULONG nStream,
    /* [out] */ LCID __RPC_FAR *pLanguage);


void __RPC_STUB IDvdInfo_GetAudioLanguage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdInfo_GetSubpictureLanguage_Proxy( 
    IDvdInfo __RPC_FAR * This,
    /* [in] */ ULONG nStream,
    /* [out] */ LCID __RPC_FAR *pLanguage);


void __RPC_STUB IDvdInfo_GetSubpictureLanguage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdInfo_GetTitleAttributes_Proxy( 
    IDvdInfo __RPC_FAR * This,
    /* [in] */ ULONG nTitle,
    /* [out] */ DVD_ATR __RPC_FAR *pATR);


void __RPC_STUB IDvdInfo_GetTitleAttributes_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdInfo_GetVMGAttributes_Proxy( 
    IDvdInfo __RPC_FAR * This,
    /* [out] */ DVD_ATR __RPC_FAR *pATR);


void __RPC_STUB IDvdInfo_GetVMGAttributes_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdInfo_GetCurrentVideoAttributes_Proxy( 
    IDvdInfo __RPC_FAR * This,
    /* [out] */ DVD_VideoATR __RPC_FAR *pATR);


void __RPC_STUB IDvdInfo_GetCurrentVideoAttributes_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdInfo_GetCurrentAudioAttributes_Proxy( 
    IDvdInfo __RPC_FAR * This,
    /* [out] */ DVD_AudioATR __RPC_FAR *pATR);


void __RPC_STUB IDvdInfo_GetCurrentAudioAttributes_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdInfo_GetCurrentSubpictureAttributes_Proxy( 
    IDvdInfo __RPC_FAR * This,
    /* [out] */ DVD_SubpictureATR __RPC_FAR *pATR);


void __RPC_STUB IDvdInfo_GetCurrentSubpictureAttributes_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdInfo_GetCurrentVolumeInfo_Proxy( 
    IDvdInfo __RPC_FAR * This,
    /* [out] */ ULONG __RPC_FAR *pNumOfVol,
    /* [out] */ ULONG __RPC_FAR *pThisVolNum,
    /* [out] */ DVD_DISC_SIDE __RPC_FAR *pSide,
    /* [out] */ ULONG __RPC_FAR *pNumOfTitles);


void __RPC_STUB IDvdInfo_GetCurrentVolumeInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdInfo_GetDVDTextInfo_Proxy( 
    IDvdInfo __RPC_FAR * This,
    /* [size_is][out] */ BYTE __RPC_FAR *pTextManager,
    /* [in] */ ULONG cbBufSize,
    /* [out] */ ULONG __RPC_FAR *pcbActualSize);


void __RPC_STUB IDvdInfo_GetDVDTextInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdInfo_GetPlayerParentalLevel_Proxy( 
    IDvdInfo __RPC_FAR * This,
    /* [out] */ ULONG __RPC_FAR *pParentalLevel,
    /* [out] */ ULONG __RPC_FAR *pCountryCode);


void __RPC_STUB IDvdInfo_GetPlayerParentalLevel_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdInfo_GetNumberOfChapters_Proxy( 
    IDvdInfo __RPC_FAR * This,
    /* [in] */ ULONG ulTitle,
    /* [out] */ ULONG __RPC_FAR *pNumberOfChapters);


void __RPC_STUB IDvdInfo_GetNumberOfChapters_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdInfo_GetTitleParentalLevels_Proxy( 
    IDvdInfo __RPC_FAR * This,
    /* [in] */ ULONG ulTitle,
    /* [out] */ ULONG __RPC_FAR *pParentalLevels);


void __RPC_STUB IDvdInfo_GetTitleParentalLevels_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdInfo_GetRoot_Proxy( 
    IDvdInfo __RPC_FAR * This,
    /* [size_is][out] */ LPSTR pRoot,
    /* [in] */ ULONG cbBufSize,
    /* [out] */ ULONG __RPC_FAR *pcbActualSize);


void __RPC_STUB IDvdInfo_GetRoot_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDvdInfo_INTERFACE_DEFINED__ */


#ifndef __IDvdCmd_INTERFACE_DEFINED__
#define __IDvdCmd_INTERFACE_DEFINED__

/* interface IDvdCmd */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IDvdCmd;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("5a4a97e4-94ee-4a55-9751-74b5643aa27d")
    IDvdCmd : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE WaitForStart( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE WaitForEnd( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDvdCmdVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IDvdCmd __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IDvdCmd __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IDvdCmd __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *WaitForStart )( 
            IDvdCmd __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *WaitForEnd )( 
            IDvdCmd __RPC_FAR * This);
        
        END_INTERFACE
    } IDvdCmdVtbl;

    interface IDvdCmd
    {
        CONST_VTBL struct IDvdCmdVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDvdCmd_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDvdCmd_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDvdCmd_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDvdCmd_WaitForStart(This)	\
    (This)->lpVtbl -> WaitForStart(This)

#define IDvdCmd_WaitForEnd(This)	\
    (This)->lpVtbl -> WaitForEnd(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IDvdCmd_WaitForStart_Proxy( 
    IDvdCmd __RPC_FAR * This);


void __RPC_STUB IDvdCmd_WaitForStart_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdCmd_WaitForEnd_Proxy( 
    IDvdCmd __RPC_FAR * This);


void __RPC_STUB IDvdCmd_WaitForEnd_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDvdCmd_INTERFACE_DEFINED__ */


#ifndef __IDvdState_INTERFACE_DEFINED__
#define __IDvdState_INTERFACE_DEFINED__

/* interface IDvdState */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IDvdState;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("86303d6d-1c4a-4087-ab42-f711167048ef")
    IDvdState : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetDiscID( 
            /* [out] */ ULONGLONG __RPC_FAR *pullUniqueID) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetParentalLevel( 
            /* [out] */ ULONG __RPC_FAR *pulParentalLevel) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDvdStateVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IDvdState __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IDvdState __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IDvdState __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDiscID )( 
            IDvdState __RPC_FAR * This,
            /* [out] */ ULONGLONG __RPC_FAR *pullUniqueID);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetParentalLevel )( 
            IDvdState __RPC_FAR * This,
            /* [out] */ ULONG __RPC_FAR *pulParentalLevel);
        
        END_INTERFACE
    } IDvdStateVtbl;

    interface IDvdState
    {
        CONST_VTBL struct IDvdStateVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDvdState_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDvdState_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDvdState_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDvdState_GetDiscID(This,pullUniqueID)	\
    (This)->lpVtbl -> GetDiscID(This,pullUniqueID)

#define IDvdState_GetParentalLevel(This,pulParentalLevel)	\
    (This)->lpVtbl -> GetParentalLevel(This,pulParentalLevel)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IDvdState_GetDiscID_Proxy( 
    IDvdState __RPC_FAR * This,
    /* [out] */ ULONGLONG __RPC_FAR *pullUniqueID);


void __RPC_STUB IDvdState_GetDiscID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdState_GetParentalLevel_Proxy( 
    IDvdState __RPC_FAR * This,
    /* [out] */ ULONG __RPC_FAR *pulParentalLevel);


void __RPC_STUB IDvdState_GetParentalLevel_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDvdState_INTERFACE_DEFINED__ */


#ifndef __IDvdControl2_INTERFACE_DEFINED__
#define __IDvdControl2_INTERFACE_DEFINED__

/* interface IDvdControl2 */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IDvdControl2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("33BC7430-EEC0-11D2-8201-00A0C9D74842")
    IDvdControl2 : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE PlayTitle( 
            /* [in] */ ULONG ulTitle,
            /* [in] */ DWORD dwFlags,
            /* [out] */ IDvdCmd __RPC_FAR *__RPC_FAR *ppCmd) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE PlayChapterInTitle( 
            /* [in] */ ULONG ulTitle,
            /* [in] */ ULONG ulChapter,
            /* [in] */ DWORD dwFlags,
            /* [out] */ IDvdCmd __RPC_FAR *__RPC_FAR *ppCmd) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE PlayAtTimeInTitle( 
            /* [in] */ ULONG ulTitle,
            /* [in] */ DVD_HMSF_TIMECODE __RPC_FAR *pStartTime,
            /* [in] */ DWORD dwFlags,
            /* [out] */ IDvdCmd __RPC_FAR *__RPC_FAR *ppCmd) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Stop( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ReturnFromSubmenu( 
            /* [in] */ DWORD dwFlags,
            /* [out] */ IDvdCmd __RPC_FAR *__RPC_FAR *ppCmd) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE PlayAtTime( 
            /* [in] */ DVD_HMSF_TIMECODE __RPC_FAR *pTime,
            /* [in] */ DWORD dwFlags,
            /* [out] */ IDvdCmd __RPC_FAR *__RPC_FAR *ppCmd) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE PlayChapter( 
            /* [in] */ ULONG ulChapter,
            /* [in] */ DWORD dwFlags,
            /* [out] */ IDvdCmd __RPC_FAR *__RPC_FAR *ppCmd) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE PlayPrevChapter( 
            /* [in] */ DWORD dwFlags,
            /* [out] */ IDvdCmd __RPC_FAR *__RPC_FAR *ppCmd) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ReplayChapter( 
            /* [in] */ DWORD dwFlags,
            /* [out] */ IDvdCmd __RPC_FAR *__RPC_FAR *ppCmd) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE PlayNextChapter( 
            /* [in] */ DWORD dwFlags,
            /* [out] */ IDvdCmd __RPC_FAR *__RPC_FAR *ppCmd) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE PlayForwards( 
            /* [in] */ double dSpeed,
            /* [in] */ DWORD dwFlags,
            /* [out] */ IDvdCmd __RPC_FAR *__RPC_FAR *ppCmd) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE PlayBackwards( 
            /* [in] */ double dSpeed,
            /* [in] */ DWORD dwFlags,
            /* [out] */ IDvdCmd __RPC_FAR *__RPC_FAR *ppCmd) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ShowMenu( 
            /* [in] */ DVD_MENU_ID MenuID,
            /* [in] */ DWORD dwFlags,
            /* [out] */ IDvdCmd __RPC_FAR *__RPC_FAR *ppCmd) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Resume( 
            /* [in] */ DWORD dwFlags,
            /* [out] */ IDvdCmd __RPC_FAR *__RPC_FAR *ppCmd) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SelectRelativeButton( 
            DVD_RELATIVE_BUTTON buttonDir) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ActivateButton( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SelectButton( 
            /* [in] */ ULONG ulButton) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SelectAndActivateButton( 
            /* [in] */ ULONG ulButton) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE StillOff( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Pause( 
            /* [in] */ BOOL bState) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SelectAudioStream( 
            /* [in] */ ULONG ulAudio,
            /* [in] */ DWORD dwFlags,
            /* [out] */ IDvdCmd __RPC_FAR *__RPC_FAR *ppCmd) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SelectSubpictureStream( 
            /* [in] */ ULONG ulSubPicture,
            /* [in] */ DWORD dwFlags,
            /* [out] */ IDvdCmd __RPC_FAR *__RPC_FAR *ppCmd) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetSubpictureState( 
            /* [in] */ BOOL bState,
            /* [in] */ DWORD dwFlags,
            /* [out] */ IDvdCmd __RPC_FAR *__RPC_FAR *ppCmd) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SelectAngle( 
            /* [in] */ ULONG ulAngle,
            /* [in] */ DWORD dwFlags,
            /* [out] */ IDvdCmd __RPC_FAR *__RPC_FAR *ppCmd) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SelectParentalLevel( 
            /* [in] */ ULONG ulParentalLevel) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SelectParentalCountry( 
            /* [in] */ BYTE __RPC_FAR bCountry[ 2 ]) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SelectKaraokeAudioPresentationMode( 
            /* [in] */ ULONG ulMode) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SelectVideoModePreference( 
            /* [in] */ ULONG ulPreferredDisplayMode) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetDVDDirectory( 
            /* [in] */ LPCWSTR pszwPath) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ActivateAtPosition( 
            /* [in] */ POINT point) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SelectAtPosition( 
            /* [in] */ POINT point) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE PlayChaptersAutoStop( 
            /* [in] */ ULONG ulTitle,
            /* [in] */ ULONG ulChapter,
            /* [in] */ ULONG ulChaptersToPlay,
            /* [in] */ DWORD dwFlags,
            /* [out] */ IDvdCmd __RPC_FAR *__RPC_FAR *ppCmd) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AcceptParentalLevelChange( 
            /* [in] */ BOOL bAccept) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetOption( 
            /* [in] */ DVD_OPTION_FLAG flag,
            /* [in] */ BOOL fState) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetState( 
            /* [in] */ IDvdState __RPC_FAR *pState,
            /* [in] */ DWORD dwFlags,
            /* [out] */ IDvdCmd __RPC_FAR *__RPC_FAR *ppCmd) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE PlayPeriodInTitleAutoStop( 
            /* [in] */ ULONG ulTitle,
            /* [in] */ DVD_HMSF_TIMECODE __RPC_FAR *pStartTime,
            /* [in] */ DVD_HMSF_TIMECODE __RPC_FAR *pEndTime,
            /* [in] */ DWORD dwFlags,
            /* [out] */ IDvdCmd __RPC_FAR *__RPC_FAR *ppCmd) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetGPRM( 
            /* [in] */ ULONG ulIndex,
            /* [in] */ WORD wValue,
            /* [in] */ DWORD dwFlags,
            /* [out] */ IDvdCmd __RPC_FAR *__RPC_FAR *ppCmd) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SelectDefaultMenuLanguage( 
            /* [in] */ LCID Language) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SelectDefaultAudioLanguage( 
            /* [in] */ LCID Language,
            /* [in] */ DVD_AUDIO_LANG_EXT audioExtension) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SelectDefaultSubpictureLanguage( 
            /* [in] */ LCID Language,
            /* [in] */ DVD_SUBPICTURE_LANG_EXT subpictureExtension) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDvdControl2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IDvdControl2 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IDvdControl2 __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IDvdControl2 __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PlayTitle )( 
            IDvdControl2 __RPC_FAR * This,
            /* [in] */ ULONG ulTitle,
            /* [in] */ DWORD dwFlags,
            /* [out] */ IDvdCmd __RPC_FAR *__RPC_FAR *ppCmd);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PlayChapterInTitle )( 
            IDvdControl2 __RPC_FAR * This,
            /* [in] */ ULONG ulTitle,
            /* [in] */ ULONG ulChapter,
            /* [in] */ DWORD dwFlags,
            /* [out] */ IDvdCmd __RPC_FAR *__RPC_FAR *ppCmd);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PlayAtTimeInTitle )( 
            IDvdControl2 __RPC_FAR * This,
            /* [in] */ ULONG ulTitle,
            /* [in] */ DVD_HMSF_TIMECODE __RPC_FAR *pStartTime,
            /* [in] */ DWORD dwFlags,
            /* [out] */ IDvdCmd __RPC_FAR *__RPC_FAR *ppCmd);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Stop )( 
            IDvdControl2 __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ReturnFromSubmenu )( 
            IDvdControl2 __RPC_FAR * This,
            /* [in] */ DWORD dwFlags,
            /* [out] */ IDvdCmd __RPC_FAR *__RPC_FAR *ppCmd);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PlayAtTime )( 
            IDvdControl2 __RPC_FAR * This,
            /* [in] */ DVD_HMSF_TIMECODE __RPC_FAR *pTime,
            /* [in] */ DWORD dwFlags,
            /* [out] */ IDvdCmd __RPC_FAR *__RPC_FAR *ppCmd);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PlayChapter )( 
            IDvdControl2 __RPC_FAR * This,
            /* [in] */ ULONG ulChapter,
            /* [in] */ DWORD dwFlags,
            /* [out] */ IDvdCmd __RPC_FAR *__RPC_FAR *ppCmd);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PlayPrevChapter )( 
            IDvdControl2 __RPC_FAR * This,
            /* [in] */ DWORD dwFlags,
            /* [out] */ IDvdCmd __RPC_FAR *__RPC_FAR *ppCmd);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ReplayChapter )( 
            IDvdControl2 __RPC_FAR * This,
            /* [in] */ DWORD dwFlags,
            /* [out] */ IDvdCmd __RPC_FAR *__RPC_FAR *ppCmd);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PlayNextChapter )( 
            IDvdControl2 __RPC_FAR * This,
            /* [in] */ DWORD dwFlags,
            /* [out] */ IDvdCmd __RPC_FAR *__RPC_FAR *ppCmd);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PlayForwards )( 
            IDvdControl2 __RPC_FAR * This,
            /* [in] */ double dSpeed,
            /* [in] */ DWORD dwFlags,
            /* [out] */ IDvdCmd __RPC_FAR *__RPC_FAR *ppCmd);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PlayBackwards )( 
            IDvdControl2 __RPC_FAR * This,
            /* [in] */ double dSpeed,
            /* [in] */ DWORD dwFlags,
            /* [out] */ IDvdCmd __RPC_FAR *__RPC_FAR *ppCmd);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ShowMenu )( 
            IDvdControl2 __RPC_FAR * This,
            /* [in] */ DVD_MENU_ID MenuID,
            /* [in] */ DWORD dwFlags,
            /* [out] */ IDvdCmd __RPC_FAR *__RPC_FAR *ppCmd);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Resume )( 
            IDvdControl2 __RPC_FAR * This,
            /* [in] */ DWORD dwFlags,
            /* [out] */ IDvdCmd __RPC_FAR *__RPC_FAR *ppCmd);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SelectRelativeButton )( 
            IDvdControl2 __RPC_FAR * This,
            DVD_RELATIVE_BUTTON buttonDir);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ActivateButton )( 
            IDvdControl2 __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SelectButton )( 
            IDvdControl2 __RPC_FAR * This,
            /* [in] */ ULONG ulButton);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SelectAndActivateButton )( 
            IDvdControl2 __RPC_FAR * This,
            /* [in] */ ULONG ulButton);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *StillOff )( 
            IDvdControl2 __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Pause )( 
            IDvdControl2 __RPC_FAR * This,
            /* [in] */ BOOL bState);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SelectAudioStream )( 
            IDvdControl2 __RPC_FAR * This,
            /* [in] */ ULONG ulAudio,
            /* [in] */ DWORD dwFlags,
            /* [out] */ IDvdCmd __RPC_FAR *__RPC_FAR *ppCmd);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SelectSubpictureStream )( 
            IDvdControl2 __RPC_FAR * This,
            /* [in] */ ULONG ulSubPicture,
            /* [in] */ DWORD dwFlags,
            /* [out] */ IDvdCmd __RPC_FAR *__RPC_FAR *ppCmd);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetSubpictureState )( 
            IDvdControl2 __RPC_FAR * This,
            /* [in] */ BOOL bState,
            /* [in] */ DWORD dwFlags,
            /* [out] */ IDvdCmd __RPC_FAR *__RPC_FAR *ppCmd);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SelectAngle )( 
            IDvdControl2 __RPC_FAR * This,
            /* [in] */ ULONG ulAngle,
            /* [in] */ DWORD dwFlags,
            /* [out] */ IDvdCmd __RPC_FAR *__RPC_FAR *ppCmd);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SelectParentalLevel )( 
            IDvdControl2 __RPC_FAR * This,
            /* [in] */ ULONG ulParentalLevel);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SelectParentalCountry )( 
            IDvdControl2 __RPC_FAR * This,
            /* [in] */ BYTE __RPC_FAR bCountry[ 2 ]);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SelectKaraokeAudioPresentationMode )( 
            IDvdControl2 __RPC_FAR * This,
            /* [in] */ ULONG ulMode);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SelectVideoModePreference )( 
            IDvdControl2 __RPC_FAR * This,
            /* [in] */ ULONG ulPreferredDisplayMode);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetDVDDirectory )( 
            IDvdControl2 __RPC_FAR * This,
            /* [in] */ LPCWSTR pszwPath);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ActivateAtPosition )( 
            IDvdControl2 __RPC_FAR * This,
            /* [in] */ POINT point);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SelectAtPosition )( 
            IDvdControl2 __RPC_FAR * This,
            /* [in] */ POINT point);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PlayChaptersAutoStop )( 
            IDvdControl2 __RPC_FAR * This,
            /* [in] */ ULONG ulTitle,
            /* [in] */ ULONG ulChapter,
            /* [in] */ ULONG ulChaptersToPlay,
            /* [in] */ DWORD dwFlags,
            /* [out] */ IDvdCmd __RPC_FAR *__RPC_FAR *ppCmd);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AcceptParentalLevelChange )( 
            IDvdControl2 __RPC_FAR * This,
            /* [in] */ BOOL bAccept);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetOption )( 
            IDvdControl2 __RPC_FAR * This,
            /* [in] */ DVD_OPTION_FLAG flag,
            /* [in] */ BOOL fState);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetState )( 
            IDvdControl2 __RPC_FAR * This,
            /* [in] */ IDvdState __RPC_FAR *pState,
            /* [in] */ DWORD dwFlags,
            /* [out] */ IDvdCmd __RPC_FAR *__RPC_FAR *ppCmd);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PlayPeriodInTitleAutoStop )( 
            IDvdControl2 __RPC_FAR * This,
            /* [in] */ ULONG ulTitle,
            /* [in] */ DVD_HMSF_TIMECODE __RPC_FAR *pStartTime,
            /* [in] */ DVD_HMSF_TIMECODE __RPC_FAR *pEndTime,
            /* [in] */ DWORD dwFlags,
            /* [out] */ IDvdCmd __RPC_FAR *__RPC_FAR *ppCmd);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetGPRM )( 
            IDvdControl2 __RPC_FAR * This,
            /* [in] */ ULONG ulIndex,
            /* [in] */ WORD wValue,
            /* [in] */ DWORD dwFlags,
            /* [out] */ IDvdCmd __RPC_FAR *__RPC_FAR *ppCmd);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SelectDefaultMenuLanguage )( 
            IDvdControl2 __RPC_FAR * This,
            /* [in] */ LCID Language);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SelectDefaultAudioLanguage )( 
            IDvdControl2 __RPC_FAR * This,
            /* [in] */ LCID Language,
            /* [in] */ DVD_AUDIO_LANG_EXT audioExtension);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SelectDefaultSubpictureLanguage )( 
            IDvdControl2 __RPC_FAR * This,
            /* [in] */ LCID Language,
            /* [in] */ DVD_SUBPICTURE_LANG_EXT subpictureExtension);
        
        END_INTERFACE
    } IDvdControl2Vtbl;

    interface IDvdControl2
    {
        CONST_VTBL struct IDvdControl2Vtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDvdControl2_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDvdControl2_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDvdControl2_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDvdControl2_PlayTitle(This,ulTitle,dwFlags,ppCmd)	\
    (This)->lpVtbl -> PlayTitle(This,ulTitle,dwFlags,ppCmd)

#define IDvdControl2_PlayChapterInTitle(This,ulTitle,ulChapter,dwFlags,ppCmd)	\
    (This)->lpVtbl -> PlayChapterInTitle(This,ulTitle,ulChapter,dwFlags,ppCmd)

#define IDvdControl2_PlayAtTimeInTitle(This,ulTitle,pStartTime,dwFlags,ppCmd)	\
    (This)->lpVtbl -> PlayAtTimeInTitle(This,ulTitle,pStartTime,dwFlags,ppCmd)

#define IDvdControl2_Stop(This)	\
    (This)->lpVtbl -> Stop(This)

#define IDvdControl2_ReturnFromSubmenu(This,dwFlags,ppCmd)	\
    (This)->lpVtbl -> ReturnFromSubmenu(This,dwFlags,ppCmd)

#define IDvdControl2_PlayAtTime(This,pTime,dwFlags,ppCmd)	\
    (This)->lpVtbl -> PlayAtTime(This,pTime,dwFlags,ppCmd)

#define IDvdControl2_PlayChapter(This,ulChapter,dwFlags,ppCmd)	\
    (This)->lpVtbl -> PlayChapter(This,ulChapter,dwFlags,ppCmd)

#define IDvdControl2_PlayPrevChapter(This,dwFlags,ppCmd)	\
    (This)->lpVtbl -> PlayPrevChapter(This,dwFlags,ppCmd)

#define IDvdControl2_ReplayChapter(This,dwFlags,ppCmd)	\
    (This)->lpVtbl -> ReplayChapter(This,dwFlags,ppCmd)

#define IDvdControl2_PlayNextChapter(This,dwFlags,ppCmd)	\
    (This)->lpVtbl -> PlayNextChapter(This,dwFlags,ppCmd)

#define IDvdControl2_PlayForwards(This,dSpeed,dwFlags,ppCmd)	\
    (This)->lpVtbl -> PlayForwards(This,dSpeed,dwFlags,ppCmd)

#define IDvdControl2_PlayBackwards(This,dSpeed,dwFlags,ppCmd)	\
    (This)->lpVtbl -> PlayBackwards(This,dSpeed,dwFlags,ppCmd)

#define IDvdControl2_ShowMenu(This,MenuID,dwFlags,ppCmd)	\
    (This)->lpVtbl -> ShowMenu(This,MenuID,dwFlags,ppCmd)

#define IDvdControl2_Resume(This,dwFlags,ppCmd)	\
    (This)->lpVtbl -> Resume(This,dwFlags,ppCmd)

#define IDvdControl2_SelectRelativeButton(This,buttonDir)	\
    (This)->lpVtbl -> SelectRelativeButton(This,buttonDir)

#define IDvdControl2_ActivateButton(This)	\
    (This)->lpVtbl -> ActivateButton(This)

#define IDvdControl2_SelectButton(This,ulButton)	\
    (This)->lpVtbl -> SelectButton(This,ulButton)

#define IDvdControl2_SelectAndActivateButton(This,ulButton)	\
    (This)->lpVtbl -> SelectAndActivateButton(This,ulButton)

#define IDvdControl2_StillOff(This)	\
    (This)->lpVtbl -> StillOff(This)

#define IDvdControl2_Pause(This,bState)	\
    (This)->lpVtbl -> Pause(This,bState)

#define IDvdControl2_SelectAudioStream(This,ulAudio,dwFlags,ppCmd)	\
    (This)->lpVtbl -> SelectAudioStream(This,ulAudio,dwFlags,ppCmd)

#define IDvdControl2_SelectSubpictureStream(This,ulSubPicture,dwFlags,ppCmd)	\
    (This)->lpVtbl -> SelectSubpictureStream(This,ulSubPicture,dwFlags,ppCmd)

#define IDvdControl2_SetSubpictureState(This,bState,dwFlags,ppCmd)	\
    (This)->lpVtbl -> SetSubpictureState(This,bState,dwFlags,ppCmd)

#define IDvdControl2_SelectAngle(This,ulAngle,dwFlags,ppCmd)	\
    (This)->lpVtbl -> SelectAngle(This,ulAngle,dwFlags,ppCmd)

#define IDvdControl2_SelectParentalLevel(This,ulParentalLevel)	\
    (This)->lpVtbl -> SelectParentalLevel(This,ulParentalLevel)

#define IDvdControl2_SelectParentalCountry(This,bCountry)	\
    (This)->lpVtbl -> SelectParentalCountry(This,bCountry)

#define IDvdControl2_SelectKaraokeAudioPresentationMode(This,ulMode)	\
    (This)->lpVtbl -> SelectKaraokeAudioPresentationMode(This,ulMode)

#define IDvdControl2_SelectVideoModePreference(This,ulPreferredDisplayMode)	\
    (This)->lpVtbl -> SelectVideoModePreference(This,ulPreferredDisplayMode)

#define IDvdControl2_SetDVDDirectory(This,pszwPath)	\
    (This)->lpVtbl -> SetDVDDirectory(This,pszwPath)

#define IDvdControl2_ActivateAtPosition(This,point)	\
    (This)->lpVtbl -> ActivateAtPosition(This,point)

#define IDvdControl2_SelectAtPosition(This,point)	\
    (This)->lpVtbl -> SelectAtPosition(This,point)

#define IDvdControl2_PlayChaptersAutoStop(This,ulTitle,ulChapter,ulChaptersToPlay,dwFlags,ppCmd)	\
    (This)->lpVtbl -> PlayChaptersAutoStop(This,ulTitle,ulChapter,ulChaptersToPlay,dwFlags,ppCmd)

#define IDvdControl2_AcceptParentalLevelChange(This,bAccept)	\
    (This)->lpVtbl -> AcceptParentalLevelChange(This,bAccept)

#define IDvdControl2_SetOption(This,flag,fState)	\
    (This)->lpVtbl -> SetOption(This,flag,fState)

#define IDvdControl2_SetState(This,pState,dwFlags,ppCmd)	\
    (This)->lpVtbl -> SetState(This,pState,dwFlags,ppCmd)

#define IDvdControl2_PlayPeriodInTitleAutoStop(This,ulTitle,pStartTime,pEndTime,dwFlags,ppCmd)	\
    (This)->lpVtbl -> PlayPeriodInTitleAutoStop(This,ulTitle,pStartTime,pEndTime,dwFlags,ppCmd)

#define IDvdControl2_SetGPRM(This,ulIndex,wValue,dwFlags,ppCmd)	\
    (This)->lpVtbl -> SetGPRM(This,ulIndex,wValue,dwFlags,ppCmd)

#define IDvdControl2_SelectDefaultMenuLanguage(This,Language)	\
    (This)->lpVtbl -> SelectDefaultMenuLanguage(This,Language)

#define IDvdControl2_SelectDefaultAudioLanguage(This,Language,audioExtension)	\
    (This)->lpVtbl -> SelectDefaultAudioLanguage(This,Language,audioExtension)

#define IDvdControl2_SelectDefaultSubpictureLanguage(This,Language,subpictureExtension)	\
    (This)->lpVtbl -> SelectDefaultSubpictureLanguage(This,Language,subpictureExtension)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IDvdControl2_PlayTitle_Proxy( 
    IDvdControl2 __RPC_FAR * This,
    /* [in] */ ULONG ulTitle,
    /* [in] */ DWORD dwFlags,
    /* [out] */ IDvdCmd __RPC_FAR *__RPC_FAR *ppCmd);


void __RPC_STUB IDvdControl2_PlayTitle_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdControl2_PlayChapterInTitle_Proxy( 
    IDvdControl2 __RPC_FAR * This,
    /* [in] */ ULONG ulTitle,
    /* [in] */ ULONG ulChapter,
    /* [in] */ DWORD dwFlags,
    /* [out] */ IDvdCmd __RPC_FAR *__RPC_FAR *ppCmd);


void __RPC_STUB IDvdControl2_PlayChapterInTitle_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdControl2_PlayAtTimeInTitle_Proxy( 
    IDvdControl2 __RPC_FAR * This,
    /* [in] */ ULONG ulTitle,
    /* [in] */ DVD_HMSF_TIMECODE __RPC_FAR *pStartTime,
    /* [in] */ DWORD dwFlags,
    /* [out] */ IDvdCmd __RPC_FAR *__RPC_FAR *ppCmd);


void __RPC_STUB IDvdControl2_PlayAtTimeInTitle_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdControl2_Stop_Proxy( 
    IDvdControl2 __RPC_FAR * This);


void __RPC_STUB IDvdControl2_Stop_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdControl2_ReturnFromSubmenu_Proxy( 
    IDvdControl2 __RPC_FAR * This,
    /* [in] */ DWORD dwFlags,
    /* [out] */ IDvdCmd __RPC_FAR *__RPC_FAR *ppCmd);


void __RPC_STUB IDvdControl2_ReturnFromSubmenu_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdControl2_PlayAtTime_Proxy( 
    IDvdControl2 __RPC_FAR * This,
    /* [in] */ DVD_HMSF_TIMECODE __RPC_FAR *pTime,
    /* [in] */ DWORD dwFlags,
    /* [out] */ IDvdCmd __RPC_FAR *__RPC_FAR *ppCmd);


void __RPC_STUB IDvdControl2_PlayAtTime_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdControl2_PlayChapter_Proxy( 
    IDvdControl2 __RPC_FAR * This,
    /* [in] */ ULONG ulChapter,
    /* [in] */ DWORD dwFlags,
    /* [out] */ IDvdCmd __RPC_FAR *__RPC_FAR *ppCmd);


void __RPC_STUB IDvdControl2_PlayChapter_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdControl2_PlayPrevChapter_Proxy( 
    IDvdControl2 __RPC_FAR * This,
    /* [in] */ DWORD dwFlags,
    /* [out] */ IDvdCmd __RPC_FAR *__RPC_FAR *ppCmd);


void __RPC_STUB IDvdControl2_PlayPrevChapter_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdControl2_ReplayChapter_Proxy( 
    IDvdControl2 __RPC_FAR * This,
    /* [in] */ DWORD dwFlags,
    /* [out] */ IDvdCmd __RPC_FAR *__RPC_FAR *ppCmd);


void __RPC_STUB IDvdControl2_ReplayChapter_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdControl2_PlayNextChapter_Proxy( 
    IDvdControl2 __RPC_FAR * This,
    /* [in] */ DWORD dwFlags,
    /* [out] */ IDvdCmd __RPC_FAR *__RPC_FAR *ppCmd);


void __RPC_STUB IDvdControl2_PlayNextChapter_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdControl2_PlayForwards_Proxy( 
    IDvdControl2 __RPC_FAR * This,
    /* [in] */ double dSpeed,
    /* [in] */ DWORD dwFlags,
    /* [out] */ IDvdCmd __RPC_FAR *__RPC_FAR *ppCmd);


void __RPC_STUB IDvdControl2_PlayForwards_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdControl2_PlayBackwards_Proxy( 
    IDvdControl2 __RPC_FAR * This,
    /* [in] */ double dSpeed,
    /* [in] */ DWORD dwFlags,
    /* [out] */ IDvdCmd __RPC_FAR *__RPC_FAR *ppCmd);


void __RPC_STUB IDvdControl2_PlayBackwards_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdControl2_ShowMenu_Proxy( 
    IDvdControl2 __RPC_FAR * This,
    /* [in] */ DVD_MENU_ID MenuID,
    /* [in] */ DWORD dwFlags,
    /* [out] */ IDvdCmd __RPC_FAR *__RPC_FAR *ppCmd);


void __RPC_STUB IDvdControl2_ShowMenu_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdControl2_Resume_Proxy( 
    IDvdControl2 __RPC_FAR * This,
    /* [in] */ DWORD dwFlags,
    /* [out] */ IDvdCmd __RPC_FAR *__RPC_FAR *ppCmd);


void __RPC_STUB IDvdControl2_Resume_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdControl2_SelectRelativeButton_Proxy( 
    IDvdControl2 __RPC_FAR * This,
    DVD_RELATIVE_BUTTON buttonDir);


void __RPC_STUB IDvdControl2_SelectRelativeButton_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdControl2_ActivateButton_Proxy( 
    IDvdControl2 __RPC_FAR * This);


void __RPC_STUB IDvdControl2_ActivateButton_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdControl2_SelectButton_Proxy( 
    IDvdControl2 __RPC_FAR * This,
    /* [in] */ ULONG ulButton);


void __RPC_STUB IDvdControl2_SelectButton_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdControl2_SelectAndActivateButton_Proxy( 
    IDvdControl2 __RPC_FAR * This,
    /* [in] */ ULONG ulButton);


void __RPC_STUB IDvdControl2_SelectAndActivateButton_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdControl2_StillOff_Proxy( 
    IDvdControl2 __RPC_FAR * This);


void __RPC_STUB IDvdControl2_StillOff_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdControl2_Pause_Proxy( 
    IDvdControl2 __RPC_FAR * This,
    /* [in] */ BOOL bState);


void __RPC_STUB IDvdControl2_Pause_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdControl2_SelectAudioStream_Proxy( 
    IDvdControl2 __RPC_FAR * This,
    /* [in] */ ULONG ulAudio,
    /* [in] */ DWORD dwFlags,
    /* [out] */ IDvdCmd __RPC_FAR *__RPC_FAR *ppCmd);


void __RPC_STUB IDvdControl2_SelectAudioStream_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdControl2_SelectSubpictureStream_Proxy( 
    IDvdControl2 __RPC_FAR * This,
    /* [in] */ ULONG ulSubPicture,
    /* [in] */ DWORD dwFlags,
    /* [out] */ IDvdCmd __RPC_FAR *__RPC_FAR *ppCmd);


void __RPC_STUB IDvdControl2_SelectSubpictureStream_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdControl2_SetSubpictureState_Proxy( 
    IDvdControl2 __RPC_FAR * This,
    /* [in] */ BOOL bState,
    /* [in] */ DWORD dwFlags,
    /* [out] */ IDvdCmd __RPC_FAR *__RPC_FAR *ppCmd);


void __RPC_STUB IDvdControl2_SetSubpictureState_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdControl2_SelectAngle_Proxy( 
    IDvdControl2 __RPC_FAR * This,
    /* [in] */ ULONG ulAngle,
    /* [in] */ DWORD dwFlags,
    /* [out] */ IDvdCmd __RPC_FAR *__RPC_FAR *ppCmd);


void __RPC_STUB IDvdControl2_SelectAngle_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdControl2_SelectParentalLevel_Proxy( 
    IDvdControl2 __RPC_FAR * This,
    /* [in] */ ULONG ulParentalLevel);


void __RPC_STUB IDvdControl2_SelectParentalLevel_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdControl2_SelectParentalCountry_Proxy( 
    IDvdControl2 __RPC_FAR * This,
    /* [in] */ BYTE __RPC_FAR bCountry[ 2 ]);


void __RPC_STUB IDvdControl2_SelectParentalCountry_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdControl2_SelectKaraokeAudioPresentationMode_Proxy( 
    IDvdControl2 __RPC_FAR * This,
    /* [in] */ ULONG ulMode);


void __RPC_STUB IDvdControl2_SelectKaraokeAudioPresentationMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdControl2_SelectVideoModePreference_Proxy( 
    IDvdControl2 __RPC_FAR * This,
    /* [in] */ ULONG ulPreferredDisplayMode);


void __RPC_STUB IDvdControl2_SelectVideoModePreference_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdControl2_SetDVDDirectory_Proxy( 
    IDvdControl2 __RPC_FAR * This,
    /* [in] */ LPCWSTR pszwPath);


void __RPC_STUB IDvdControl2_SetDVDDirectory_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdControl2_ActivateAtPosition_Proxy( 
    IDvdControl2 __RPC_FAR * This,
    /* [in] */ POINT point);


void __RPC_STUB IDvdControl2_ActivateAtPosition_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdControl2_SelectAtPosition_Proxy( 
    IDvdControl2 __RPC_FAR * This,
    /* [in] */ POINT point);


void __RPC_STUB IDvdControl2_SelectAtPosition_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdControl2_PlayChaptersAutoStop_Proxy( 
    IDvdControl2 __RPC_FAR * This,
    /* [in] */ ULONG ulTitle,
    /* [in] */ ULONG ulChapter,
    /* [in] */ ULONG ulChaptersToPlay,
    /* [in] */ DWORD dwFlags,
    /* [out] */ IDvdCmd __RPC_FAR *__RPC_FAR *ppCmd);


void __RPC_STUB IDvdControl2_PlayChaptersAutoStop_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdControl2_AcceptParentalLevelChange_Proxy( 
    IDvdControl2 __RPC_FAR * This,
    /* [in] */ BOOL bAccept);


void __RPC_STUB IDvdControl2_AcceptParentalLevelChange_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdControl2_SetOption_Proxy( 
    IDvdControl2 __RPC_FAR * This,
    /* [in] */ DVD_OPTION_FLAG flag,
    /* [in] */ BOOL fState);


void __RPC_STUB IDvdControl2_SetOption_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdControl2_SetState_Proxy( 
    IDvdControl2 __RPC_FAR * This,
    /* [in] */ IDvdState __RPC_FAR *pState,
    /* [in] */ DWORD dwFlags,
    /* [out] */ IDvdCmd __RPC_FAR *__RPC_FAR *ppCmd);


void __RPC_STUB IDvdControl2_SetState_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdControl2_PlayPeriodInTitleAutoStop_Proxy( 
    IDvdControl2 __RPC_FAR * This,
    /* [in] */ ULONG ulTitle,
    /* [in] */ DVD_HMSF_TIMECODE __RPC_FAR *pStartTime,
    /* [in] */ DVD_HMSF_TIMECODE __RPC_FAR *pEndTime,
    /* [in] */ DWORD dwFlags,
    /* [out] */ IDvdCmd __RPC_FAR *__RPC_FAR *ppCmd);


void __RPC_STUB IDvdControl2_PlayPeriodInTitleAutoStop_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdControl2_SetGPRM_Proxy( 
    IDvdControl2 __RPC_FAR * This,
    /* [in] */ ULONG ulIndex,
    /* [in] */ WORD wValue,
    /* [in] */ DWORD dwFlags,
    /* [out] */ IDvdCmd __RPC_FAR *__RPC_FAR *ppCmd);


void __RPC_STUB IDvdControl2_SetGPRM_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdControl2_SelectDefaultMenuLanguage_Proxy( 
    IDvdControl2 __RPC_FAR * This,
    /* [in] */ LCID Language);


void __RPC_STUB IDvdControl2_SelectDefaultMenuLanguage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdControl2_SelectDefaultAudioLanguage_Proxy( 
    IDvdControl2 __RPC_FAR * This,
    /* [in] */ LCID Language,
    /* [in] */ DVD_AUDIO_LANG_EXT audioExtension);


void __RPC_STUB IDvdControl2_SelectDefaultAudioLanguage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdControl2_SelectDefaultSubpictureLanguage_Proxy( 
    IDvdControl2 __RPC_FAR * This,
    /* [in] */ LCID Language,
    /* [in] */ DVD_SUBPICTURE_LANG_EXT subpictureExtension);


void __RPC_STUB IDvdControl2_SelectDefaultSubpictureLanguage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDvdControl2_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_strmif_0238 */
/* [local] */ 


enum DVD_TextStringType
    {	DVD_Struct_Volume	= 0x1,
	DVD_Struct_Title	= 0x2,
	DVD_Struct_ParentalID	= 0x3,
	DVD_Struct_PartOfTitle	= 0x4,
	DVD_Struct_Cell	= 0x5,
	DVD_Stream_Audio	= 0x10,
	DVD_Stream_Subpicture	= 0x11,
	DVD_Stream_Angle	= 0x12,
	DVD_Channel_Audio	= 0x20,
	DVD_General_Name	= 0x30,
	DVD_General_Comments	= 0x31,
	DVD_Title_Series	= 0x38,
	DVD_Title_Movie	= 0x39,
	DVD_Title_Video	= 0x3a,
	DVD_Title_Album	= 0x3b,
	DVD_Title_Song	= 0x3c,
	DVD_Title_Other	= 0x3f,
	DVD_Title_Sub_Series	= 0x40,
	DVD_Title_Sub_Movie	= 0x41,
	DVD_Title_Sub_Video	= 0x42,
	DVD_Title_Sub_Album	= 0x43,
	DVD_Title_Sub_Song	= 0x44,
	DVD_Title_Sub_Other	= 0x47,
	DVD_Title_Orig_Series	= 0x48,
	DVD_Title_Orig_Movie	= 0x49,
	DVD_Title_Orig_Video	= 0x4a,
	DVD_Title_Orig_Album	= 0x4b,
	DVD_Title_Orig_Song	= 0x4c,
	DVD_Title_Orig_Other	= 0x4f,
	DVD_Other_Scene	= 0x50,
	DVD_Other_Cut	= 0x51,
	DVD_Other_Take	= 0x52
    };

enum DVD_TextCharSet
    {	DVD_CharSet_Unicode	= 0,
	DVD_CharSet_ISO646	= 1,
	DVD_CharSet_JIS_Roman_Kanji	= 2,
	DVD_CharSet_ISO8859_1	= 3,
	DVD_CharSet_ShiftJIS_Kanji_Roman_Katakana	= 4
    };
#define DVD_TITLE_MENU				0x000
#define DVD_STREAM_DATA_CURRENT     0x800
#define DVD_STREAM_DATA_VMGM        0x400
#define DVD_STREAM_DATA_VTSM        0x401
#define DVD_DEFAULT_AUDIO_STREAM	0x0f
typedef struct tagDVD_DECODER_CAPS
    {
    DWORD dwSize;
    DWORD dwAudioCaps;
    double dFwdMaxRateVideo;
    double dFwdMaxRateAudio;
    double dFwdMaxRateSP;
    double dBwdMaxRateVideo;
    double dBwdMaxRateAudio;
    double dBwdMaxRateSP;
    DWORD dwRes1;
    DWORD dwRes2;
    DWORD dwRes3;
    DWORD dwRes4;
    }	DVD_DECODER_CAPS;

#define DVD_AUDIO_CAPS_AC3		0x00000001
#define DVD_AUDIO_CAPS_MPEG2	0x00000002
#define DVD_AUDIO_CAPS_LPCM		0x00000004
#define DVD_AUDIO_CAPS_DTS		0x00000008
#define DVD_AUDIO_CAPS_SDDS		0x00000010


extern RPC_IF_HANDLE __MIDL_itf_strmif_0238_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_strmif_0238_v0_0_s_ifspec;

#ifndef __IDvdInfo2_INTERFACE_DEFINED__
#define __IDvdInfo2_INTERFACE_DEFINED__

/* interface IDvdInfo2 */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IDvdInfo2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("34151510-EEC0-11D2-8201-00A0C9D74842")
    IDvdInfo2 : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetCurrentDomain( 
            /* [out] */ DVD_DOMAIN __RPC_FAR *pDomain) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetCurrentLocation( 
            /* [out] */ DVD_PLAYBACK_LOCATION2 __RPC_FAR *pLocation) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetTotalTitleTime( 
            /* [out] */ DVD_HMSF_TIMECODE __RPC_FAR *pTotalTime,
            /* [out] */ ULONG __RPC_FAR *ulTimeCodeFlags) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetCurrentButton( 
            /* [out] */ ULONG __RPC_FAR *pulButtonsAvailable,
            /* [out] */ ULONG __RPC_FAR *pulCurrentButton) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetCurrentAngle( 
            /* [out] */ ULONG __RPC_FAR *pulAnglesAvailable,
            /* [out] */ ULONG __RPC_FAR *pulCurrentAngle) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetCurrentAudio( 
            /* [out] */ ULONG __RPC_FAR *pulStreamsAvailable,
            /* [out] */ ULONG __RPC_FAR *pulCurrentStream) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetCurrentSubpicture( 
            /* [out] */ ULONG __RPC_FAR *pulStreamsAvailable,
            /* [out] */ ULONG __RPC_FAR *pulCurrentStream,
            /* [out] */ BOOL __RPC_FAR *pbIsDisabled) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetCurrentUOPS( 
            /* [out] */ ULONG __RPC_FAR *pulUOPs) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetAllSPRMs( 
            /* [out] */ SPRMARRAY __RPC_FAR *pRegisterArray) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetAllGPRMs( 
            /* [out] */ GPRMARRAY __RPC_FAR *pRegisterArray) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetAudioLanguage( 
            /* [in] */ ULONG ulStream,
            /* [out] */ LCID __RPC_FAR *pLanguage) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetSubpictureLanguage( 
            /* [in] */ ULONG ulStream,
            /* [out] */ LCID __RPC_FAR *pLanguage) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetTitleAttributes( 
            /* [in] */ ULONG ulTitle,
            /* [out] */ DVD_MenuAttributes __RPC_FAR *pMenu,
            /* [out] */ DVD_TitleAttributes __RPC_FAR *pTitle) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetVMGAttributes( 
            /* [out] */ DVD_MenuAttributes __RPC_FAR *pATR) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetCurrentVideoAttributes( 
            /* [out] */ DVD_VideoAttributes __RPC_FAR *pATR) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetAudioAttributes( 
            /* [in] */ ULONG ulStream,
            /* [out] */ DVD_AudioAttributes __RPC_FAR *pATR) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetKaraokeAttributes( 
            /* [in] */ ULONG ulStream,
            /* [out] */ DVD_KaraokeAttributes __RPC_FAR *pAttributes) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetSubpictureAttributes( 
            /* [in] */ ULONG ulStream,
            /* [out] */ DVD_SubpictureAttributes __RPC_FAR *pATR) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetDVDVolumeInfo( 
            /* [out] */ ULONG __RPC_FAR *pulNumOfVolumes,
            /* [out] */ ULONG __RPC_FAR *pulVolume,
            /* [out] */ DVD_DISC_SIDE __RPC_FAR *pSide,
            /* [out] */ ULONG __RPC_FAR *pulNumOfTitles) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetDVDTextNumberOfLanguages( 
            /* [out] */ ULONG __RPC_FAR *pulNumOfLangs) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetDVDTextLanguageInfo( 
            /* [in] */ ULONG ulLangIndex,
            /* [out] */ ULONG __RPC_FAR *pulNumOfStrings,
            /* [out] */ LCID __RPC_FAR *pLangCode,
            /* [out] */ enum DVD_TextCharSet __RPC_FAR *pbCharacterSet) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetDVDTextStringAsNative( 
            /* [in] */ ULONG ulLangIndex,
            /* [in] */ ULONG ulStringIndex,
            /* [out] */ BYTE __RPC_FAR *pbBuffer,
            /* [in] */ ULONG ulMaxBufferSize,
            /* [out] */ ULONG __RPC_FAR *pulActualSize,
            /* [out] */ enum DVD_TextStringType __RPC_FAR *pType) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetDVDTextStringAsUnicode( 
            /* [in] */ ULONG ulLangIndex,
            /* [in] */ ULONG ulStringIndex,
            /* [out] */ WCHAR __RPC_FAR *pchwBuffer,
            /* [in] */ ULONG ulMaxBufferSize,
            /* [out] */ ULONG __RPC_FAR *pulActualSize,
            /* [out] */ enum DVD_TextStringType __RPC_FAR *pType) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetPlayerParentalLevel( 
            /* [out] */ ULONG __RPC_FAR *pulParentalLevel,
            /* [out] */ BYTE __RPC_FAR pbCountryCode[ 2 ]) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetNumberOfChapters( 
            /* [in] */ ULONG ulTitle,
            /* [out] */ ULONG __RPC_FAR *pulNumOfChapters) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetTitleParentalLevels( 
            /* [in] */ ULONG ulTitle,
            /* [out] */ ULONG __RPC_FAR *pulParentalLevels) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetDVDDirectory( 
            /* [size_is][out] */ LPWSTR pszwPath,
            /* [in] */ ULONG ulMaxSize,
            /* [out] */ ULONG __RPC_FAR *pulActualSize) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE IsAudioStreamEnabled( 
            /* [in] */ ULONG ulStreamNum,
            /* [out] */ BOOL __RPC_FAR *pbEnabled) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetDiscID( 
            /* [in] */ LPCWSTR pszwPath,
            /* [out] */ ULONGLONG __RPC_FAR *pullDiscID) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetState( 
            /* [out] */ IDvdState __RPC_FAR *__RPC_FAR *pStateData) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetMenuLanguages( 
            /* [out] */ LCID __RPC_FAR *pLanguages,
            /* [in] */ ULONG ulMaxLanguages,
            /* [out] */ ULONG __RPC_FAR *pulActualLanguages) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetButtonAtPosition( 
            /* [in] */ POINT point,
            /* [out] */ ULONG __RPC_FAR *pulButtonIndex) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetCmdFromEvent( 
            /* [in] */ LONG_PTR lParam1,
            /* [out] */ IDvdCmd __RPC_FAR *__RPC_FAR *pCmdObj) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetDefaultMenuLanguage( 
            /* [out] */ LCID __RPC_FAR *pLanguage) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetDefaultAudioLanguage( 
            /* [out] */ LCID __RPC_FAR *pLanguage,
            /* [out] */ DVD_AUDIO_LANG_EXT __RPC_FAR *pAudioExtension) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetDefaultSubpictureLanguage( 
            /* [out] */ LCID __RPC_FAR *pLanguage,
            /* [out] */ DVD_SUBPICTURE_LANG_EXT __RPC_FAR *pSubpictureExtension) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetDecoderCaps( 
            /* [out] */ DVD_DECODER_CAPS __RPC_FAR *pCaps) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetButtonRect( 
            /* [in] */ ULONG ulButton,
            /* [out] */ RECT __RPC_FAR *pRect) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE IsSubpictureStreamEnabled( 
            /* [in] */ ULONG ulStreamNum,
            /* [out] */ BOOL __RPC_FAR *pbEnabled) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDvdInfo2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IDvdInfo2 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IDvdInfo2 __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IDvdInfo2 __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetCurrentDomain )( 
            IDvdInfo2 __RPC_FAR * This,
            /* [out] */ DVD_DOMAIN __RPC_FAR *pDomain);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetCurrentLocation )( 
            IDvdInfo2 __RPC_FAR * This,
            /* [out] */ DVD_PLAYBACK_LOCATION2 __RPC_FAR *pLocation);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTotalTitleTime )( 
            IDvdInfo2 __RPC_FAR * This,
            /* [out] */ DVD_HMSF_TIMECODE __RPC_FAR *pTotalTime,
            /* [out] */ ULONG __RPC_FAR *ulTimeCodeFlags);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetCurrentButton )( 
            IDvdInfo2 __RPC_FAR * This,
            /* [out] */ ULONG __RPC_FAR *pulButtonsAvailable,
            /* [out] */ ULONG __RPC_FAR *pulCurrentButton);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetCurrentAngle )( 
            IDvdInfo2 __RPC_FAR * This,
            /* [out] */ ULONG __RPC_FAR *pulAnglesAvailable,
            /* [out] */ ULONG __RPC_FAR *pulCurrentAngle);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetCurrentAudio )( 
            IDvdInfo2 __RPC_FAR * This,
            /* [out] */ ULONG __RPC_FAR *pulStreamsAvailable,
            /* [out] */ ULONG __RPC_FAR *pulCurrentStream);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetCurrentSubpicture )( 
            IDvdInfo2 __RPC_FAR * This,
            /* [out] */ ULONG __RPC_FAR *pulStreamsAvailable,
            /* [out] */ ULONG __RPC_FAR *pulCurrentStream,
            /* [out] */ BOOL __RPC_FAR *pbIsDisabled);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetCurrentUOPS )( 
            IDvdInfo2 __RPC_FAR * This,
            /* [out] */ ULONG __RPC_FAR *pulUOPs);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetAllSPRMs )( 
            IDvdInfo2 __RPC_FAR * This,
            /* [out] */ SPRMARRAY __RPC_FAR *pRegisterArray);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetAllGPRMs )( 
            IDvdInfo2 __RPC_FAR * This,
            /* [out] */ GPRMARRAY __RPC_FAR *pRegisterArray);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetAudioLanguage )( 
            IDvdInfo2 __RPC_FAR * This,
            /* [in] */ ULONG ulStream,
            /* [out] */ LCID __RPC_FAR *pLanguage);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetSubpictureLanguage )( 
            IDvdInfo2 __RPC_FAR * This,
            /* [in] */ ULONG ulStream,
            /* [out] */ LCID __RPC_FAR *pLanguage);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTitleAttributes )( 
            IDvdInfo2 __RPC_FAR * This,
            /* [in] */ ULONG ulTitle,
            /* [out] */ DVD_MenuAttributes __RPC_FAR *pMenu,
            /* [out] */ DVD_TitleAttributes __RPC_FAR *pTitle);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetVMGAttributes )( 
            IDvdInfo2 __RPC_FAR * This,
            /* [out] */ DVD_MenuAttributes __RPC_FAR *pATR);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetCurrentVideoAttributes )( 
            IDvdInfo2 __RPC_FAR * This,
            /* [out] */ DVD_VideoAttributes __RPC_FAR *pATR);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetAudioAttributes )( 
            IDvdInfo2 __RPC_FAR * This,
            /* [in] */ ULONG ulStream,
            /* [out] */ DVD_AudioAttributes __RPC_FAR *pATR);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetKaraokeAttributes )( 
            IDvdInfo2 __RPC_FAR * This,
            /* [in] */ ULONG ulStream,
            /* [out] */ DVD_KaraokeAttributes __RPC_FAR *pAttributes);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetSubpictureAttributes )( 
            IDvdInfo2 __RPC_FAR * This,
            /* [in] */ ULONG ulStream,
            /* [out] */ DVD_SubpictureAttributes __RPC_FAR *pATR);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDVDVolumeInfo )( 
            IDvdInfo2 __RPC_FAR * This,
            /* [out] */ ULONG __RPC_FAR *pulNumOfVolumes,
            /* [out] */ ULONG __RPC_FAR *pulVolume,
            /* [out] */ DVD_DISC_SIDE __RPC_FAR *pSide,
            /* [out] */ ULONG __RPC_FAR *pulNumOfTitles);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDVDTextNumberOfLanguages )( 
            IDvdInfo2 __RPC_FAR * This,
            /* [out] */ ULONG __RPC_FAR *pulNumOfLangs);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDVDTextLanguageInfo )( 
            IDvdInfo2 __RPC_FAR * This,
            /* [in] */ ULONG ulLangIndex,
            /* [out] */ ULONG __RPC_FAR *pulNumOfStrings,
            /* [out] */ LCID __RPC_FAR *pLangCode,
            /* [out] */ enum DVD_TextCharSet __RPC_FAR *pbCharacterSet);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDVDTextStringAsNative )( 
            IDvdInfo2 __RPC_FAR * This,
            /* [in] */ ULONG ulLangIndex,
            /* [in] */ ULONG ulStringIndex,
            /* [out] */ BYTE __RPC_FAR *pbBuffer,
            /* [in] */ ULONG ulMaxBufferSize,
            /* [out] */ ULONG __RPC_FAR *pulActualSize,
            /* [out] */ enum DVD_TextStringType __RPC_FAR *pType);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDVDTextStringAsUnicode )( 
            IDvdInfo2 __RPC_FAR * This,
            /* [in] */ ULONG ulLangIndex,
            /* [in] */ ULONG ulStringIndex,
            /* [out] */ WCHAR __RPC_FAR *pchwBuffer,
            /* [in] */ ULONG ulMaxBufferSize,
            /* [out] */ ULONG __RPC_FAR *pulActualSize,
            /* [out] */ enum DVD_TextStringType __RPC_FAR *pType);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetPlayerParentalLevel )( 
            IDvdInfo2 __RPC_FAR * This,
            /* [out] */ ULONG __RPC_FAR *pulParentalLevel,
            /* [out] */ BYTE __RPC_FAR pbCountryCode[ 2 ]);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNumberOfChapters )( 
            IDvdInfo2 __RPC_FAR * This,
            /* [in] */ ULONG ulTitle,
            /* [out] */ ULONG __RPC_FAR *pulNumOfChapters);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTitleParentalLevels )( 
            IDvdInfo2 __RPC_FAR * This,
            /* [in] */ ULONG ulTitle,
            /* [out] */ ULONG __RPC_FAR *pulParentalLevels);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDVDDirectory )( 
            IDvdInfo2 __RPC_FAR * This,
            /* [size_is][out] */ LPWSTR pszwPath,
            /* [in] */ ULONG ulMaxSize,
            /* [out] */ ULONG __RPC_FAR *pulActualSize);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsAudioStreamEnabled )( 
            IDvdInfo2 __RPC_FAR * This,
            /* [in] */ ULONG ulStreamNum,
            /* [out] */ BOOL __RPC_FAR *pbEnabled);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDiscID )( 
            IDvdInfo2 __RPC_FAR * This,
            /* [in] */ LPCWSTR pszwPath,
            /* [out] */ ULONGLONG __RPC_FAR *pullDiscID);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetState )( 
            IDvdInfo2 __RPC_FAR * This,
            /* [out] */ IDvdState __RPC_FAR *__RPC_FAR *pStateData);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMenuLanguages )( 
            IDvdInfo2 __RPC_FAR * This,
            /* [out] */ LCID __RPC_FAR *pLanguages,
            /* [in] */ ULONG ulMaxLanguages,
            /* [out] */ ULONG __RPC_FAR *pulActualLanguages);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetButtonAtPosition )( 
            IDvdInfo2 __RPC_FAR * This,
            /* [in] */ POINT point,
            /* [out] */ ULONG __RPC_FAR *pulButtonIndex);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetCmdFromEvent )( 
            IDvdInfo2 __RPC_FAR * This,
            /* [in] */ LONG_PTR lParam1,
            /* [out] */ IDvdCmd __RPC_FAR *__RPC_FAR *pCmdObj);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDefaultMenuLanguage )( 
            IDvdInfo2 __RPC_FAR * This,
            /* [out] */ LCID __RPC_FAR *pLanguage);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDefaultAudioLanguage )( 
            IDvdInfo2 __RPC_FAR * This,
            /* [out] */ LCID __RPC_FAR *pLanguage,
            /* [out] */ DVD_AUDIO_LANG_EXT __RPC_FAR *pAudioExtension);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDefaultSubpictureLanguage )( 
            IDvdInfo2 __RPC_FAR * This,
            /* [out] */ LCID __RPC_FAR *pLanguage,
            /* [out] */ DVD_SUBPICTURE_LANG_EXT __RPC_FAR *pSubpictureExtension);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDecoderCaps )( 
            IDvdInfo2 __RPC_FAR * This,
            /* [out] */ DVD_DECODER_CAPS __RPC_FAR *pCaps);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetButtonRect )( 
            IDvdInfo2 __RPC_FAR * This,
            /* [in] */ ULONG ulButton,
            /* [out] */ RECT __RPC_FAR *pRect);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsSubpictureStreamEnabled )( 
            IDvdInfo2 __RPC_FAR * This,
            /* [in] */ ULONG ulStreamNum,
            /* [out] */ BOOL __RPC_FAR *pbEnabled);
        
        END_INTERFACE
    } IDvdInfo2Vtbl;

    interface IDvdInfo2
    {
        CONST_VTBL struct IDvdInfo2Vtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDvdInfo2_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDvdInfo2_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDvdInfo2_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDvdInfo2_GetCurrentDomain(This,pDomain)	\
    (This)->lpVtbl -> GetCurrentDomain(This,pDomain)

#define IDvdInfo2_GetCurrentLocation(This,pLocation)	\
    (This)->lpVtbl -> GetCurrentLocation(This,pLocation)

#define IDvdInfo2_GetTotalTitleTime(This,pTotalTime,ulTimeCodeFlags)	\
    (This)->lpVtbl -> GetTotalTitleTime(This,pTotalTime,ulTimeCodeFlags)

#define IDvdInfo2_GetCurrentButton(This,pulButtonsAvailable,pulCurrentButton)	\
    (This)->lpVtbl -> GetCurrentButton(This,pulButtonsAvailable,pulCurrentButton)

#define IDvdInfo2_GetCurrentAngle(This,pulAnglesAvailable,pulCurrentAngle)	\
    (This)->lpVtbl -> GetCurrentAngle(This,pulAnglesAvailable,pulCurrentAngle)

#define IDvdInfo2_GetCurrentAudio(This,pulStreamsAvailable,pulCurrentStream)	\
    (This)->lpVtbl -> GetCurrentAudio(This,pulStreamsAvailable,pulCurrentStream)

#define IDvdInfo2_GetCurrentSubpicture(This,pulStreamsAvailable,pulCurrentStream,pbIsDisabled)	\
    (This)->lpVtbl -> GetCurrentSubpicture(This,pulStreamsAvailable,pulCurrentStream,pbIsDisabled)

#define IDvdInfo2_GetCurrentUOPS(This,pulUOPs)	\
    (This)->lpVtbl -> GetCurrentUOPS(This,pulUOPs)

#define IDvdInfo2_GetAllSPRMs(This,pRegisterArray)	\
    (This)->lpVtbl -> GetAllSPRMs(This,pRegisterArray)

#define IDvdInfo2_GetAllGPRMs(This,pRegisterArray)	\
    (This)->lpVtbl -> GetAllGPRMs(This,pRegisterArray)

#define IDvdInfo2_GetAudioLanguage(This,ulStream,pLanguage)	\
    (This)->lpVtbl -> GetAudioLanguage(This,ulStream,pLanguage)

#define IDvdInfo2_GetSubpictureLanguage(This,ulStream,pLanguage)	\
    (This)->lpVtbl -> GetSubpictureLanguage(This,ulStream,pLanguage)

#define IDvdInfo2_GetTitleAttributes(This,ulTitle,pMenu,pTitle)	\
    (This)->lpVtbl -> GetTitleAttributes(This,ulTitle,pMenu,pTitle)

#define IDvdInfo2_GetVMGAttributes(This,pATR)	\
    (This)->lpVtbl -> GetVMGAttributes(This,pATR)

#define IDvdInfo2_GetCurrentVideoAttributes(This,pATR)	\
    (This)->lpVtbl -> GetCurrentVideoAttributes(This,pATR)

#define IDvdInfo2_GetAudioAttributes(This,ulStream,pATR)	\
    (This)->lpVtbl -> GetAudioAttributes(This,ulStream,pATR)

#define IDvdInfo2_GetKaraokeAttributes(This,ulStream,pAttributes)	\
    (This)->lpVtbl -> GetKaraokeAttributes(This,ulStream,pAttributes)

#define IDvdInfo2_GetSubpictureAttributes(This,ulStream,pATR)	\
    (This)->lpVtbl -> GetSubpictureAttributes(This,ulStream,pATR)

#define IDvdInfo2_GetDVDVolumeInfo(This,pulNumOfVolumes,pulVolume,pSide,pulNumOfTitles)	\
    (This)->lpVtbl -> GetDVDVolumeInfo(This,pulNumOfVolumes,pulVolume,pSide,pulNumOfTitles)

#define IDvdInfo2_GetDVDTextNumberOfLanguages(This,pulNumOfLangs)	\
    (This)->lpVtbl -> GetDVDTextNumberOfLanguages(This,pulNumOfLangs)

#define IDvdInfo2_GetDVDTextLanguageInfo(This,ulLangIndex,pulNumOfStrings,pLangCode,pbCharacterSet)	\
    (This)->lpVtbl -> GetDVDTextLanguageInfo(This,ulLangIndex,pulNumOfStrings,pLangCode,pbCharacterSet)

#define IDvdInfo2_GetDVDTextStringAsNative(This,ulLangIndex,ulStringIndex,pbBuffer,ulMaxBufferSize,pulActualSize,pType)	\
    (This)->lpVtbl -> GetDVDTextStringAsNative(This,ulLangIndex,ulStringIndex,pbBuffer,ulMaxBufferSize,pulActualSize,pType)

#define IDvdInfo2_GetDVDTextStringAsUnicode(This,ulLangIndex,ulStringIndex,pchwBuffer,ulMaxBufferSize,pulActualSize,pType)	\
    (This)->lpVtbl -> GetDVDTextStringAsUnicode(This,ulLangIndex,ulStringIndex,pchwBuffer,ulMaxBufferSize,pulActualSize,pType)

#define IDvdInfo2_GetPlayerParentalLevel(This,pulParentalLevel,pbCountryCode)	\
    (This)->lpVtbl -> GetPlayerParentalLevel(This,pulParentalLevel,pbCountryCode)

#define IDvdInfo2_GetNumberOfChapters(This,ulTitle,pulNumOfChapters)	\
    (This)->lpVtbl -> GetNumberOfChapters(This,ulTitle,pulNumOfChapters)

#define IDvdInfo2_GetTitleParentalLevels(This,ulTitle,pulParentalLevels)	\
    (This)->lpVtbl -> GetTitleParentalLevels(This,ulTitle,pulParentalLevels)

#define IDvdInfo2_GetDVDDirectory(This,pszwPath,ulMaxSize,pulActualSize)	\
    (This)->lpVtbl -> GetDVDDirectory(This,pszwPath,ulMaxSize,pulActualSize)

#define IDvdInfo2_IsAudioStreamEnabled(This,ulStreamNum,pbEnabled)	\
    (This)->lpVtbl -> IsAudioStreamEnabled(This,ulStreamNum,pbEnabled)

#define IDvdInfo2_GetDiscID(This,pszwPath,pullDiscID)	\
    (This)->lpVtbl -> GetDiscID(This,pszwPath,pullDiscID)

#define IDvdInfo2_GetState(This,pStateData)	\
    (This)->lpVtbl -> GetState(This,pStateData)

#define IDvdInfo2_GetMenuLanguages(This,pLanguages,ulMaxLanguages,pulActualLanguages)	\
    (This)->lpVtbl -> GetMenuLanguages(This,pLanguages,ulMaxLanguages,pulActualLanguages)

#define IDvdInfo2_GetButtonAtPosition(This,point,pulButtonIndex)	\
    (This)->lpVtbl -> GetButtonAtPosition(This,point,pulButtonIndex)

#define IDvdInfo2_GetCmdFromEvent(This,lParam1,pCmdObj)	\
    (This)->lpVtbl -> GetCmdFromEvent(This,lParam1,pCmdObj)

#define IDvdInfo2_GetDefaultMenuLanguage(This,pLanguage)	\
    (This)->lpVtbl -> GetDefaultMenuLanguage(This,pLanguage)

#define IDvdInfo2_GetDefaultAudioLanguage(This,pLanguage,pAudioExtension)	\
    (This)->lpVtbl -> GetDefaultAudioLanguage(This,pLanguage,pAudioExtension)

#define IDvdInfo2_GetDefaultSubpictureLanguage(This,pLanguage,pSubpictureExtension)	\
    (This)->lpVtbl -> GetDefaultSubpictureLanguage(This,pLanguage,pSubpictureExtension)

#define IDvdInfo2_GetDecoderCaps(This,pCaps)	\
    (This)->lpVtbl -> GetDecoderCaps(This,pCaps)

#define IDvdInfo2_GetButtonRect(This,ulButton,pRect)	\
    (This)->lpVtbl -> GetButtonRect(This,ulButton,pRect)

#define IDvdInfo2_IsSubpictureStreamEnabled(This,ulStreamNum,pbEnabled)	\
    (This)->lpVtbl -> IsSubpictureStreamEnabled(This,ulStreamNum,pbEnabled)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IDvdInfo2_GetCurrentDomain_Proxy( 
    IDvdInfo2 __RPC_FAR * This,
    /* [out] */ DVD_DOMAIN __RPC_FAR *pDomain);


void __RPC_STUB IDvdInfo2_GetCurrentDomain_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdInfo2_GetCurrentLocation_Proxy( 
    IDvdInfo2 __RPC_FAR * This,
    /* [out] */ DVD_PLAYBACK_LOCATION2 __RPC_FAR *pLocation);


void __RPC_STUB IDvdInfo2_GetCurrentLocation_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdInfo2_GetTotalTitleTime_Proxy( 
    IDvdInfo2 __RPC_FAR * This,
    /* [out] */ DVD_HMSF_TIMECODE __RPC_FAR *pTotalTime,
    /* [out] */ ULONG __RPC_FAR *ulTimeCodeFlags);


void __RPC_STUB IDvdInfo2_GetTotalTitleTime_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdInfo2_GetCurrentButton_Proxy( 
    IDvdInfo2 __RPC_FAR * This,
    /* [out] */ ULONG __RPC_FAR *pulButtonsAvailable,
    /* [out] */ ULONG __RPC_FAR *pulCurrentButton);


void __RPC_STUB IDvdInfo2_GetCurrentButton_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdInfo2_GetCurrentAngle_Proxy( 
    IDvdInfo2 __RPC_FAR * This,
    /* [out] */ ULONG __RPC_FAR *pulAnglesAvailable,
    /* [out] */ ULONG __RPC_FAR *pulCurrentAngle);


void __RPC_STUB IDvdInfo2_GetCurrentAngle_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdInfo2_GetCurrentAudio_Proxy( 
    IDvdInfo2 __RPC_FAR * This,
    /* [out] */ ULONG __RPC_FAR *pulStreamsAvailable,
    /* [out] */ ULONG __RPC_FAR *pulCurrentStream);


void __RPC_STUB IDvdInfo2_GetCurrentAudio_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdInfo2_GetCurrentSubpicture_Proxy( 
    IDvdInfo2 __RPC_FAR * This,
    /* [out] */ ULONG __RPC_FAR *pulStreamsAvailable,
    /* [out] */ ULONG __RPC_FAR *pulCurrentStream,
    /* [out] */ BOOL __RPC_FAR *pbIsDisabled);


void __RPC_STUB IDvdInfo2_GetCurrentSubpicture_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdInfo2_GetCurrentUOPS_Proxy( 
    IDvdInfo2 __RPC_FAR * This,
    /* [out] */ ULONG __RPC_FAR *pulUOPs);


void __RPC_STUB IDvdInfo2_GetCurrentUOPS_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdInfo2_GetAllSPRMs_Proxy( 
    IDvdInfo2 __RPC_FAR * This,
    /* [out] */ SPRMARRAY __RPC_FAR *pRegisterArray);


void __RPC_STUB IDvdInfo2_GetAllSPRMs_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdInfo2_GetAllGPRMs_Proxy( 
    IDvdInfo2 __RPC_FAR * This,
    /* [out] */ GPRMARRAY __RPC_FAR *pRegisterArray);


void __RPC_STUB IDvdInfo2_GetAllGPRMs_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdInfo2_GetAudioLanguage_Proxy( 
    IDvdInfo2 __RPC_FAR * This,
    /* [in] */ ULONG ulStream,
    /* [out] */ LCID __RPC_FAR *pLanguage);


void __RPC_STUB IDvdInfo2_GetAudioLanguage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdInfo2_GetSubpictureLanguage_Proxy( 
    IDvdInfo2 __RPC_FAR * This,
    /* [in] */ ULONG ulStream,
    /* [out] */ LCID __RPC_FAR *pLanguage);


void __RPC_STUB IDvdInfo2_GetSubpictureLanguage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdInfo2_GetTitleAttributes_Proxy( 
    IDvdInfo2 __RPC_FAR * This,
    /* [in] */ ULONG ulTitle,
    /* [out] */ DVD_MenuAttributes __RPC_FAR *pMenu,
    /* [out] */ DVD_TitleAttributes __RPC_FAR *pTitle);


void __RPC_STUB IDvdInfo2_GetTitleAttributes_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdInfo2_GetVMGAttributes_Proxy( 
    IDvdInfo2 __RPC_FAR * This,
    /* [out] */ DVD_MenuAttributes __RPC_FAR *pATR);


void __RPC_STUB IDvdInfo2_GetVMGAttributes_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdInfo2_GetCurrentVideoAttributes_Proxy( 
    IDvdInfo2 __RPC_FAR * This,
    /* [out] */ DVD_VideoAttributes __RPC_FAR *pATR);


void __RPC_STUB IDvdInfo2_GetCurrentVideoAttributes_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdInfo2_GetAudioAttributes_Proxy( 
    IDvdInfo2 __RPC_FAR * This,
    /* [in] */ ULONG ulStream,
    /* [out] */ DVD_AudioAttributes __RPC_FAR *pATR);


void __RPC_STUB IDvdInfo2_GetAudioAttributes_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdInfo2_GetKaraokeAttributes_Proxy( 
    IDvdInfo2 __RPC_FAR * This,
    /* [in] */ ULONG ulStream,
    /* [out] */ DVD_KaraokeAttributes __RPC_FAR *pAttributes);


void __RPC_STUB IDvdInfo2_GetKaraokeAttributes_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdInfo2_GetSubpictureAttributes_Proxy( 
    IDvdInfo2 __RPC_FAR * This,
    /* [in] */ ULONG ulStream,
    /* [out] */ DVD_SubpictureAttributes __RPC_FAR *pATR);


void __RPC_STUB IDvdInfo2_GetSubpictureAttributes_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdInfo2_GetDVDVolumeInfo_Proxy( 
    IDvdInfo2 __RPC_FAR * This,
    /* [out] */ ULONG __RPC_FAR *pulNumOfVolumes,
    /* [out] */ ULONG __RPC_FAR *pulVolume,
    /* [out] */ DVD_DISC_SIDE __RPC_FAR *pSide,
    /* [out] */ ULONG __RPC_FAR *pulNumOfTitles);


void __RPC_STUB IDvdInfo2_GetDVDVolumeInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdInfo2_GetDVDTextNumberOfLanguages_Proxy( 
    IDvdInfo2 __RPC_FAR * This,
    /* [out] */ ULONG __RPC_FAR *pulNumOfLangs);


void __RPC_STUB IDvdInfo2_GetDVDTextNumberOfLanguages_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdInfo2_GetDVDTextLanguageInfo_Proxy( 
    IDvdInfo2 __RPC_FAR * This,
    /* [in] */ ULONG ulLangIndex,
    /* [out] */ ULONG __RPC_FAR *pulNumOfStrings,
    /* [out] */ LCID __RPC_FAR *pLangCode,
    /* [out] */ enum DVD_TextCharSet __RPC_FAR *pbCharacterSet);


void __RPC_STUB IDvdInfo2_GetDVDTextLanguageInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdInfo2_GetDVDTextStringAsNative_Proxy( 
    IDvdInfo2 __RPC_FAR * This,
    /* [in] */ ULONG ulLangIndex,
    /* [in] */ ULONG ulStringIndex,
    /* [out] */ BYTE __RPC_FAR *pbBuffer,
    /* [in] */ ULONG ulMaxBufferSize,
    /* [out] */ ULONG __RPC_FAR *pulActualSize,
    /* [out] */ enum DVD_TextStringType __RPC_FAR *pType);


void __RPC_STUB IDvdInfo2_GetDVDTextStringAsNative_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdInfo2_GetDVDTextStringAsUnicode_Proxy( 
    IDvdInfo2 __RPC_FAR * This,
    /* [in] */ ULONG ulLangIndex,
    /* [in] */ ULONG ulStringIndex,
    /* [out] */ WCHAR __RPC_FAR *pchwBuffer,
    /* [in] */ ULONG ulMaxBufferSize,
    /* [out] */ ULONG __RPC_FAR *pulActualSize,
    /* [out] */ enum DVD_TextStringType __RPC_FAR *pType);


void __RPC_STUB IDvdInfo2_GetDVDTextStringAsUnicode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdInfo2_GetPlayerParentalLevel_Proxy( 
    IDvdInfo2 __RPC_FAR * This,
    /* [out] */ ULONG __RPC_FAR *pulParentalLevel,
    /* [out] */ BYTE __RPC_FAR pbCountryCode[ 2 ]);


void __RPC_STUB IDvdInfo2_GetPlayerParentalLevel_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdInfo2_GetNumberOfChapters_Proxy( 
    IDvdInfo2 __RPC_FAR * This,
    /* [in] */ ULONG ulTitle,
    /* [out] */ ULONG __RPC_FAR *pulNumOfChapters);


void __RPC_STUB IDvdInfo2_GetNumberOfChapters_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdInfo2_GetTitleParentalLevels_Proxy( 
    IDvdInfo2 __RPC_FAR * This,
    /* [in] */ ULONG ulTitle,
    /* [out] */ ULONG __RPC_FAR *pulParentalLevels);


void __RPC_STUB IDvdInfo2_GetTitleParentalLevels_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdInfo2_GetDVDDirectory_Proxy( 
    IDvdInfo2 __RPC_FAR * This,
    /* [size_is][out] */ LPWSTR pszwPath,
    /* [in] */ ULONG ulMaxSize,
    /* [out] */ ULONG __RPC_FAR *pulActualSize);


void __RPC_STUB IDvdInfo2_GetDVDDirectory_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdInfo2_IsAudioStreamEnabled_Proxy( 
    IDvdInfo2 __RPC_FAR * This,
    /* [in] */ ULONG ulStreamNum,
    /* [out] */ BOOL __RPC_FAR *pbEnabled);


void __RPC_STUB IDvdInfo2_IsAudioStreamEnabled_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdInfo2_GetDiscID_Proxy( 
    IDvdInfo2 __RPC_FAR * This,
    /* [in] */ LPCWSTR pszwPath,
    /* [out] */ ULONGLONG __RPC_FAR *pullDiscID);


void __RPC_STUB IDvdInfo2_GetDiscID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdInfo2_GetState_Proxy( 
    IDvdInfo2 __RPC_FAR * This,
    /* [out] */ IDvdState __RPC_FAR *__RPC_FAR *pStateData);


void __RPC_STUB IDvdInfo2_GetState_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdInfo2_GetMenuLanguages_Proxy( 
    IDvdInfo2 __RPC_FAR * This,
    /* [out] */ LCID __RPC_FAR *pLanguages,
    /* [in] */ ULONG ulMaxLanguages,
    /* [out] */ ULONG __RPC_FAR *pulActualLanguages);


void __RPC_STUB IDvdInfo2_GetMenuLanguages_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdInfo2_GetButtonAtPosition_Proxy( 
    IDvdInfo2 __RPC_FAR * This,
    /* [in] */ POINT point,
    /* [out] */ ULONG __RPC_FAR *pulButtonIndex);


void __RPC_STUB IDvdInfo2_GetButtonAtPosition_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdInfo2_GetCmdFromEvent_Proxy( 
    IDvdInfo2 __RPC_FAR * This,
    /* [in] */ LONG_PTR lParam1,
    /* [out] */ IDvdCmd __RPC_FAR *__RPC_FAR *pCmdObj);


void __RPC_STUB IDvdInfo2_GetCmdFromEvent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdInfo2_GetDefaultMenuLanguage_Proxy( 
    IDvdInfo2 __RPC_FAR * This,
    /* [out] */ LCID __RPC_FAR *pLanguage);


void __RPC_STUB IDvdInfo2_GetDefaultMenuLanguage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdInfo2_GetDefaultAudioLanguage_Proxy( 
    IDvdInfo2 __RPC_FAR * This,
    /* [out] */ LCID __RPC_FAR *pLanguage,
    /* [out] */ DVD_AUDIO_LANG_EXT __RPC_FAR *pAudioExtension);


void __RPC_STUB IDvdInfo2_GetDefaultAudioLanguage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdInfo2_GetDefaultSubpictureLanguage_Proxy( 
    IDvdInfo2 __RPC_FAR * This,
    /* [out] */ LCID __RPC_FAR *pLanguage,
    /* [out] */ DVD_SUBPICTURE_LANG_EXT __RPC_FAR *pSubpictureExtension);


void __RPC_STUB IDvdInfo2_GetDefaultSubpictureLanguage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdInfo2_GetDecoderCaps_Proxy( 
    IDvdInfo2 __RPC_FAR * This,
    /* [out] */ DVD_DECODER_CAPS __RPC_FAR *pCaps);


void __RPC_STUB IDvdInfo2_GetDecoderCaps_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdInfo2_GetButtonRect_Proxy( 
    IDvdInfo2 __RPC_FAR * This,
    /* [in] */ ULONG ulButton,
    /* [out] */ RECT __RPC_FAR *pRect);


void __RPC_STUB IDvdInfo2_GetButtonRect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdInfo2_IsSubpictureStreamEnabled_Proxy( 
    IDvdInfo2 __RPC_FAR * This,
    /* [in] */ ULONG ulStreamNum,
    /* [out] */ BOOL __RPC_FAR *pbEnabled);


void __RPC_STUB IDvdInfo2_IsSubpictureStreamEnabled_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDvdInfo2_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_strmif_0240 */
/* [local] */ 

typedef 
enum _AM_DVD_GRAPH_FLAGS
    {	AM_DVD_HWDEC_PREFER	= 0x1,
	AM_DVD_HWDEC_ONLY	= 0x2,
	AM_DVD_SWDEC_PREFER	= 0x4,
	AM_DVD_SWDEC_ONLY	= 0x8,
	AM_DVD_NOVPE	= 0x100
    }	AM_DVD_GRAPH_FLAGS;

typedef 
enum _AM_DVD_STREAM_FLAGS
    {	AM_DVD_STREAM_VIDEO	= 0x1,
	AM_DVD_STREAM_AUDIO	= 0x2,
	AM_DVD_STREAM_SUBPIC	= 0x4
    }	AM_DVD_STREAM_FLAGS;

typedef /* [public][public] */ struct __MIDL___MIDL_itf_strmif_0240_0001
    {
    HRESULT hrVPEStatus;
    BOOL bDvdVolInvalid;
    BOOL bDvdVolUnknown;
    BOOL bNoLine21In;
    BOOL bNoLine21Out;
    int iNumStreams;
    int iNumStreamsFailed;
    DWORD dwFailedStreamsFlag;
    }	AM_DVD_RENDERSTATUS;



extern RPC_IF_HANDLE __MIDL_itf_strmif_0240_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_strmif_0240_v0_0_s_ifspec;

#ifndef __IDvdGraphBuilder_INTERFACE_DEFINED__
#define __IDvdGraphBuilder_INTERFACE_DEFINED__

/* interface IDvdGraphBuilder */
/* [unique][uuid][local][object] */ 


EXTERN_C const IID IID_IDvdGraphBuilder;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("FCC152B6-F372-11d0-8E00-00C04FD7C08B")
    IDvdGraphBuilder : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetFiltergraph( 
            /* [out] */ IGraphBuilder __RPC_FAR *__RPC_FAR *ppGB) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetDvdInterface( 
            /* [in] */ REFIID riid,
            /* [out] */ void __RPC_FAR *__RPC_FAR *ppvIF) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RenderDvdVideoVolume( 
            /* [in] */ LPCWSTR lpcwszPathName,
            /* [in] */ DWORD dwFlags,
            /* [out] */ AM_DVD_RENDERSTATUS __RPC_FAR *pStatus) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDvdGraphBuilderVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IDvdGraphBuilder __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IDvdGraphBuilder __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IDvdGraphBuilder __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetFiltergraph )( 
            IDvdGraphBuilder __RPC_FAR * This,
            /* [out] */ IGraphBuilder __RPC_FAR *__RPC_FAR *ppGB);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDvdInterface )( 
            IDvdGraphBuilder __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [out] */ void __RPC_FAR *__RPC_FAR *ppvIF);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RenderDvdVideoVolume )( 
            IDvdGraphBuilder __RPC_FAR * This,
            /* [in] */ LPCWSTR lpcwszPathName,
            /* [in] */ DWORD dwFlags,
            /* [out] */ AM_DVD_RENDERSTATUS __RPC_FAR *pStatus);
        
        END_INTERFACE
    } IDvdGraphBuilderVtbl;

    interface IDvdGraphBuilder
    {
        CONST_VTBL struct IDvdGraphBuilderVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDvdGraphBuilder_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDvdGraphBuilder_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDvdGraphBuilder_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDvdGraphBuilder_GetFiltergraph(This,ppGB)	\
    (This)->lpVtbl -> GetFiltergraph(This,ppGB)

#define IDvdGraphBuilder_GetDvdInterface(This,riid,ppvIF)	\
    (This)->lpVtbl -> GetDvdInterface(This,riid,ppvIF)

#define IDvdGraphBuilder_RenderDvdVideoVolume(This,lpcwszPathName,dwFlags,pStatus)	\
    (This)->lpVtbl -> RenderDvdVideoVolume(This,lpcwszPathName,dwFlags,pStatus)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IDvdGraphBuilder_GetFiltergraph_Proxy( 
    IDvdGraphBuilder __RPC_FAR * This,
    /* [out] */ IGraphBuilder __RPC_FAR *__RPC_FAR *ppGB);


void __RPC_STUB IDvdGraphBuilder_GetFiltergraph_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdGraphBuilder_GetDvdInterface_Proxy( 
    IDvdGraphBuilder __RPC_FAR * This,
    /* [in] */ REFIID riid,
    /* [out] */ void __RPC_FAR *__RPC_FAR *ppvIF);


void __RPC_STUB IDvdGraphBuilder_GetDvdInterface_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDvdGraphBuilder_RenderDvdVideoVolume_Proxy( 
    IDvdGraphBuilder __RPC_FAR * This,
    /* [in] */ LPCWSTR lpcwszPathName,
    /* [in] */ DWORD dwFlags,
    /* [out] */ AM_DVD_RENDERSTATUS __RPC_FAR *pStatus);


void __RPC_STUB IDvdGraphBuilder_RenderDvdVideoVolume_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDvdGraphBuilder_INTERFACE_DEFINED__ */


#ifndef __IDDrawExclModeVideo_INTERFACE_DEFINED__
#define __IDDrawExclModeVideo_INTERFACE_DEFINED__

/* interface IDDrawExclModeVideo */
/* [unique][uuid][local][object] */ 


EXTERN_C const IID IID_IDDrawExclModeVideo;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("153ACC21-D83B-11d1-82BF-00A0C9696C8F")
    IDDrawExclModeVideo : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SetDDrawObject( 
            /* [in] */ IDirectDraw __RPC_FAR *pDDrawObject) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetDDrawObject( 
            /* [out] */ IDirectDraw __RPC_FAR *__RPC_FAR *ppDDrawObject,
            /* [out] */ BOOL __RPC_FAR *pbUsingExternal) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetDDrawSurface( 
            /* [in] */ IDirectDrawSurface __RPC_FAR *pDDrawSurface) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetDDrawSurface( 
            /* [out] */ IDirectDrawSurface __RPC_FAR *__RPC_FAR *ppDDrawSurface,
            /* [out] */ BOOL __RPC_FAR *pbUsingExternal) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetDrawParameters( 
            /* [in] */ const RECT __RPC_FAR *prcSource,
            /* [in] */ const RECT __RPC_FAR *prcTarget) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetNativeVideoProps( 
            /* [out] */ DWORD __RPC_FAR *pdwVideoWidth,
            /* [out] */ DWORD __RPC_FAR *pdwVideoHeight,
            /* [out] */ DWORD __RPC_FAR *pdwPictAspectRatioX,
            /* [out] */ DWORD __RPC_FAR *pdwPictAspectRatioY) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetCallbackInterface( 
            /* [in] */ IDDrawExclModeVideoCallback __RPC_FAR *pCallback,
            /* [in] */ DWORD dwFlags) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDDrawExclModeVideoVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IDDrawExclModeVideo __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IDDrawExclModeVideo __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IDDrawExclModeVideo __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetDDrawObject )( 
            IDDrawExclModeVideo __RPC_FAR * This,
            /* [in] */ IDirectDraw __RPC_FAR *pDDrawObject);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDDrawObject )( 
            IDDrawExclModeVideo __RPC_FAR * This,
            /* [out] */ IDirectDraw __RPC_FAR *__RPC_FAR *ppDDrawObject,
            /* [out] */ BOOL __RPC_FAR *pbUsingExternal);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetDDrawSurface )( 
            IDDrawExclModeVideo __RPC_FAR * This,
            /* [in] */ IDirectDrawSurface __RPC_FAR *pDDrawSurface);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDDrawSurface )( 
            IDDrawExclModeVideo __RPC_FAR * This,
            /* [out] */ IDirectDrawSurface __RPC_FAR *__RPC_FAR *ppDDrawSurface,
            /* [out] */ BOOL __RPC_FAR *pbUsingExternal);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetDrawParameters )( 
            IDDrawExclModeVideo __RPC_FAR * This,
            /* [in] */ const RECT __RPC_FAR *prcSource,
            /* [in] */ const RECT __RPC_FAR *prcTarget);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNativeVideoProps )( 
            IDDrawExclModeVideo __RPC_FAR * This,
            /* [out] */ DWORD __RPC_FAR *pdwVideoWidth,
            /* [out] */ DWORD __RPC_FAR *pdwVideoHeight,
            /* [out] */ DWORD __RPC_FAR *pdwPictAspectRatioX,
            /* [out] */ DWORD __RPC_FAR *pdwPictAspectRatioY);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetCallbackInterface )( 
            IDDrawExclModeVideo __RPC_FAR * This,
            /* [in] */ IDDrawExclModeVideoCallback __RPC_FAR *pCallback,
            /* [in] */ DWORD dwFlags);
        
        END_INTERFACE
    } IDDrawExclModeVideoVtbl;

    interface IDDrawExclModeVideo
    {
        CONST_VTBL struct IDDrawExclModeVideoVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDDrawExclModeVideo_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDDrawExclModeVideo_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDDrawExclModeVideo_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDDrawExclModeVideo_SetDDrawObject(This,pDDrawObject)	\
    (This)->lpVtbl -> SetDDrawObject(This,pDDrawObject)

#define IDDrawExclModeVideo_GetDDrawObject(This,ppDDrawObject,pbUsingExternal)	\
    (This)->lpVtbl -> GetDDrawObject(This,ppDDrawObject,pbUsingExternal)

#define IDDrawExclModeVideo_SetDDrawSurface(This,pDDrawSurface)	\
    (This)->lpVtbl -> SetDDrawSurface(This,pDDrawSurface)

#define IDDrawExclModeVideo_GetDDrawSurface(This,ppDDrawSurface,pbUsingExternal)	\
    (This)->lpVtbl -> GetDDrawSurface(This,ppDDrawSurface,pbUsingExternal)

#define IDDrawExclModeVideo_SetDrawParameters(This,prcSource,prcTarget)	\
    (This)->lpVtbl -> SetDrawParameters(This,prcSource,prcTarget)

#define IDDrawExclModeVideo_GetNativeVideoProps(This,pdwVideoWidth,pdwVideoHeight,pdwPictAspectRatioX,pdwPictAspectRatioY)	\
    (This)->lpVtbl -> GetNativeVideoProps(This,pdwVideoWidth,pdwVideoHeight,pdwPictAspectRatioX,pdwPictAspectRatioY)

#define IDDrawExclModeVideo_SetCallbackInterface(This,pCallback,dwFlags)	\
    (This)->lpVtbl -> SetCallbackInterface(This,pCallback,dwFlags)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IDDrawExclModeVideo_SetDDrawObject_Proxy( 
    IDDrawExclModeVideo __RPC_FAR * This,
    /* [in] */ IDirectDraw __RPC_FAR *pDDrawObject);


void __RPC_STUB IDDrawExclModeVideo_SetDDrawObject_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDDrawExclModeVideo_GetDDrawObject_Proxy( 
    IDDrawExclModeVideo __RPC_FAR * This,
    /* [out] */ IDirectDraw __RPC_FAR *__RPC_FAR *ppDDrawObject,
    /* [out] */ BOOL __RPC_FAR *pbUsingExternal);


void __RPC_STUB IDDrawExclModeVideo_GetDDrawObject_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDDrawExclModeVideo_SetDDrawSurface_Proxy( 
    IDDrawExclModeVideo __RPC_FAR * This,
    /* [in] */ IDirectDrawSurface __RPC_FAR *pDDrawSurface);


void __RPC_STUB IDDrawExclModeVideo_SetDDrawSurface_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDDrawExclModeVideo_GetDDrawSurface_Proxy( 
    IDDrawExclModeVideo __RPC_FAR * This,
    /* [out] */ IDirectDrawSurface __RPC_FAR *__RPC_FAR *ppDDrawSurface,
    /* [out] */ BOOL __RPC_FAR *pbUsingExternal);


void __RPC_STUB IDDrawExclModeVideo_GetDDrawSurface_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDDrawExclModeVideo_SetDrawParameters_Proxy( 
    IDDrawExclModeVideo __RPC_FAR * This,
    /* [in] */ const RECT __RPC_FAR *prcSource,
    /* [in] */ const RECT __RPC_FAR *prcTarget);


void __RPC_STUB IDDrawExclModeVideo_SetDrawParameters_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDDrawExclModeVideo_GetNativeVideoProps_Proxy( 
    IDDrawExclModeVideo __RPC_FAR * This,
    /* [out] */ DWORD __RPC_FAR *pdwVideoWidth,
    /* [out] */ DWORD __RPC_FAR *pdwVideoHeight,
    /* [out] */ DWORD __RPC_FAR *pdwPictAspectRatioX,
    /* [out] */ DWORD __RPC_FAR *pdwPictAspectRatioY);


void __RPC_STUB IDDrawExclModeVideo_GetNativeVideoProps_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDDrawExclModeVideo_SetCallbackInterface_Proxy( 
    IDDrawExclModeVideo __RPC_FAR * This,
    /* [in] */ IDDrawExclModeVideoCallback __RPC_FAR *pCallback,
    /* [in] */ DWORD dwFlags);


void __RPC_STUB IDDrawExclModeVideo_SetCallbackInterface_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDDrawExclModeVideo_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_strmif_0242 */
/* [local] */ 


enum _AM_OVERLAY_NOTIFY_FLAGS
    {	AM_OVERLAY_NOTIFY_VISIBLE_CHANGE	= 0x1,
	AM_OVERLAY_NOTIFY_SOURCE_CHANGE	= 0x2,
	AM_OVERLAY_NOTIFY_DEST_CHANGE	= 0x4
    };


extern RPC_IF_HANDLE __MIDL_itf_strmif_0242_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_strmif_0242_v0_0_s_ifspec;

#ifndef __IDDrawExclModeVideoCallback_INTERFACE_DEFINED__
#define __IDDrawExclModeVideoCallback_INTERFACE_DEFINED__

/* interface IDDrawExclModeVideoCallback */
/* [unique][uuid][local][object] */ 


EXTERN_C const IID IID_IDDrawExclModeVideoCallback;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("913c24a0-20ab-11d2-9038-00a0c9697298")
    IDDrawExclModeVideoCallback : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE OnUpdateOverlay( 
            /* [in] */ BOOL bBefore,
            /* [in] */ DWORD dwFlags,
            /* [in] */ BOOL bOldVisible,
            /* [in] */ const RECT __RPC_FAR *prcOldSrc,
            /* [in] */ const RECT __RPC_FAR *prcOldDest,
            /* [in] */ BOOL bNewVisible,
            /* [in] */ const RECT __RPC_FAR *prcNewSrc,
            /* [in] */ const RECT __RPC_FAR *prcNewDest) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnUpdateColorKey( 
            /* [in] */ const COLORKEY __RPC_FAR *pKey,
            /* [in] */ DWORD dwColor) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnUpdateSize( 
            /* [in] */ DWORD dwWidth,
            /* [in] */ DWORD dwHeight,
            /* [in] */ DWORD dwARWidth,
            /* [in] */ DWORD dwARHeight) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDDrawExclModeVideoCallbackVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IDDrawExclModeVideoCallback __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IDDrawExclModeVideoCallback __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IDDrawExclModeVideoCallback __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnUpdateOverlay )( 
            IDDrawExclModeVideoCallback __RPC_FAR * This,
            /* [in] */ BOOL bBefore,
            /* [in] */ DWORD dwFlags,
            /* [in] */ BOOL bOldVisible,
            /* [in] */ const RECT __RPC_FAR *prcOldSrc,
            /* [in] */ const RECT __RPC_FAR *prcOldDest,
            /* [in] */ BOOL bNewVisible,
            /* [in] */ const RECT __RPC_FAR *prcNewSrc,
            /* [in] */ const RECT __RPC_FAR *prcNewDest);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnUpdateColorKey )( 
            IDDrawExclModeVideoCallback __RPC_FAR * This,
            /* [in] */ const COLORKEY __RPC_FAR *pKey,
            /* [in] */ DWORD dwColor);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnUpdateSize )( 
            IDDrawExclModeVideoCallback __RPC_FAR * This,
            /* [in] */ DWORD dwWidth,
            /* [in] */ DWORD dwHeight,
            /* [in] */ DWORD dwARWidth,
            /* [in] */ DWORD dwARHeight);
        
        END_INTERFACE
    } IDDrawExclModeVideoCallbackVtbl;

    interface IDDrawExclModeVideoCallback
    {
        CONST_VTBL struct IDDrawExclModeVideoCallbackVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDDrawExclModeVideoCallback_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDDrawExclModeVideoCallback_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDDrawExclModeVideoCallback_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDDrawExclModeVideoCallback_OnUpdateOverlay(This,bBefore,dwFlags,bOldVisible,prcOldSrc,prcOldDest,bNewVisible,prcNewSrc,prcNewDest)	\
    (This)->lpVtbl -> OnUpdateOverlay(This,bBefore,dwFlags,bOldVisible,prcOldSrc,prcOldDest,bNewVisible,prcNewSrc,prcNewDest)

#define IDDrawExclModeVideoCallback_OnUpdateColorKey(This,pKey,dwColor)	\
    (This)->lpVtbl -> OnUpdateColorKey(This,pKey,dwColor)

#define IDDrawExclModeVideoCallback_OnUpdateSize(This,dwWidth,dwHeight,dwARWidth,dwARHeight)	\
    (This)->lpVtbl -> OnUpdateSize(This,dwWidth,dwHeight,dwARWidth,dwARHeight)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IDDrawExclModeVideoCallback_OnUpdateOverlay_Proxy( 
    IDDrawExclModeVideoCallback __RPC_FAR * This,
    /* [in] */ BOOL bBefore,
    /* [in] */ DWORD dwFlags,
    /* [in] */ BOOL bOldVisible,
    /* [in] */ const RECT __RPC_FAR *prcOldSrc,
    /* [in] */ const RECT __RPC_FAR *prcOldDest,
    /* [in] */ BOOL bNewVisible,
    /* [in] */ const RECT __RPC_FAR *prcNewSrc,
    /* [in] */ const RECT __RPC_FAR *prcNewDest);


void __RPC_STUB IDDrawExclModeVideoCallback_OnUpdateOverlay_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDDrawExclModeVideoCallback_OnUpdateColorKey_Proxy( 
    IDDrawExclModeVideoCallback __RPC_FAR * This,
    /* [in] */ const COLORKEY __RPC_FAR *pKey,
    /* [in] */ DWORD dwColor);


void __RPC_STUB IDDrawExclModeVideoCallback_OnUpdateColorKey_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDDrawExclModeVideoCallback_OnUpdateSize_Proxy( 
    IDDrawExclModeVideoCallback __RPC_FAR * This,
    /* [in] */ DWORD dwWidth,
    /* [in] */ DWORD dwHeight,
    /* [in] */ DWORD dwARWidth,
    /* [in] */ DWORD dwARHeight);


void __RPC_STUB IDDrawExclModeVideoCallback_OnUpdateSize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDDrawExclModeVideoCallback_INTERFACE_DEFINED__ */


/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


