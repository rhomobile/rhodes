//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this source code is subject to the terms of the Microsoft end-user
// license agreement (EULA) under which you licensed this SOFTWARE PRODUCT.
// If you did not accept the terms of the EULA, you are not authorized to use
// this source code. For a copy of the EULA, please see the LICENSE.RTF on your
// install media.
//
/*++


Module Name:

    cs.h

Abstract:

    Windows CE Camera Driver Model/Connection and Streaming Architecture core definitions.

--*/

#if !defined(_CS_)
#define _CS_


#if defined(__TCS__)
#define _CS_NO_ANONYMOUS_STRUCTURES_ 1
#endif

#if !defined(_NTRTL_)
    #ifndef DEFINE_GUIDEX
        #define DEFINE_GUIDEX(name) EXTERN_C const CDECL GUID name
    #endif // !defined(DEFINE_GUIDEX)

    #ifndef STATICGUIDOF
        #define STATICGUIDOF(guid) STATIC_##guid
    #endif // !defined(STATICGUIDOF)
#endif // !defined(_NTRTL_)

#ifndef SIZEOF_ARRAY
    #define SIZEOF_ARRAY(ar)        (sizeof(ar)/sizeof((ar)[0]))
#endif // !defined(SIZEOF_ARRAY)

#if defined(__cplusplus) && _MSC_VER >= 1100
#define DEFINE_GUIDSTRUCT(g, n) struct __declspec(uuid(g)) n
#define DEFINE_GUIDNAMED(n) __uuidof(struct n)
#else // !defined(__cplusplus)
#define DEFINE_GUIDSTRUCT(g, n) DEFINE_GUIDEX(n)
#define DEFINE_GUIDNAMED(n) n
#endif // !defined(__cplusplus)

//===========================================================================

#define STATIC_GUID_NULL \
    0x00000000L, 0x0000, 0x0000, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

DEFINE_GUIDSTRUCT("00000000-0000-0000-0000-000000000000", GUID_NULL);
#define GUID_NULL DEFINE_GUIDNAMED(GUID_NULL)

//===========================================================================

#define IOCTL_CS_PROPERTY				CTL_CODE(FILE_DEVICE_KS, 0x000, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_CS_READ_STREAM			CTL_CODE(FILE_DEVICE_KS, 0x005, METHOD_NEITHER, FILE_READ_ACCESS)
#define IOCTL_CS_BUFFERS			CTL_CODE(FILE_DEVICE_KS, 0x007, METHOD_NEITHER, FILE_READ_ACCESS)

#ifndef UNDER_CE
#define IOCTL_CS_ENABLE_EVENT			CTL_CODE(FILE_DEVICE_KS, 0x001, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_CS_DISABLE_EVENT			CTL_CODE(FILE_DEVICE_KS, 0x002, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_CS_METHOD					CTL_CODE(FILE_DEVICE_KS, 0x003, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_CS_WRITE_STREAM			CTL_CODE(FILE_DEVICE_KS, 0x004, METHOD_NEITHER, FILE_WRITE_ACCESS)
#define IOCTL_CS_RESET_STATE			CTL_CODE(FILE_DEVICE_KS, 0x006, METHOD_NEITHER, FILE_ANY_ACCESS)
#endif //#ifndef UNDER_CE





//===========================================================================
#ifndef UNDER_CE
typedef enum {
    CSRESET_BEGIN,
    CSRESET_END
} CSRESET;
#endif //#ifndef UNDER_CE

typedef enum {
    CSSTATE_STOP,
#ifndef UNDER_CE
	CSSTATE_ACQUIRE,
#endif //#ifndef UNDER_CE
    CSSTATE_PAUSE,
    CSSTATE_RUN
} CSSTATE, *PCSSTATE;

#ifndef UNDER_CE
#define CSPRIORITY_LOW        0x00000001
#define CSPRIORITY_NORMAL     0x40000000
#define CSPRIORITY_HIGH       0x80000000
#define CSPRIORITY_EXCLUSIVE  0xFFFFFFFF

typedef struct {
    ULONG   PriorityClass;
    ULONG   PrioritySubClass;
} CSPRIORITY, *PCSPRIORITY;

#endif //#ifndef UNDER_CE
typedef struct {
    union {
#if defined( _CS_NO_ANONYMOUS_STRUCTURES_ )
        struct _IDENTIFIER {
#else        
        struct {
#endif        
            GUID    Set;
            ULONG   Id;
            ULONG   Flags;
        };
        LONGLONG    Alignment;
    };
} CSIDENTIFIER, *PCSIDENTIFIER;

typedef CSIDENTIFIER CSPROPERTY, *PCSPROPERTY, CSMETHOD, *PCSMETHOD, CSEVENT, *PCSEVENT;

#ifndef UNDER_CE
#define CSMETHOD_TYPE_NONE                  0x00000000
#define CSMETHOD_TYPE_READ                  0x00000001
#define CSMETHOD_TYPE_WRITE                 0x00000002
#define CSMETHOD_TYPE_MODIFY                0x00000003
#define CSMETHOD_TYPE_SOURCE                0x00000004

#define CSMETHOD_TYPE_SEND                  0x00000001
#define CSMETHOD_TYPE_SETSUPPORT            0x00000100
#define CSMETHOD_TYPE_BASICSUPPORT          0x00000200

#define CSMETHOD_TYPE_TOPOLOGY 0x10000000
#endif //#ifndef UNDER_CE

#define CSPROPERTY_TYPE_GET                 0x00000001
#define CSPROPERTY_TYPE_SET                 0x00000002
#define CSPROPERTY_TYPE_SETSUPPORT          0x00000100
#define CSPROPERTY_TYPE_BASICSUPPORT        0x00000200

#ifndef UNDER_CE
#define CSPROPERTY_TYPE_RELATIONS           0x00000400
#define CSPROPERTY_TYPE_SERIALIZESET        0x00000800
#define CSPROPERTY_TYPE_UNSERIALIZESET      0x00001000
#define CSPROPERTY_TYPE_SERIALIZERAW        0x00002000
#define CSPROPERTY_TYPE_UNSERIALIZERAW      0x00004000
#define CSPROPERTY_TYPE_SERIALIZESIZE       0x00008000
#endif //#ifndef UNDER_CE

#define CSPROPERTY_TYPE_DEFAULTVALUES       0x00010000

#ifndef UNDER_CE
#define CSPROPERTY_TYPE_TOPOLOGY 0x10000000

typedef struct {
    CSPROPERTY      Property;
    ULONG           NodeId;
    ULONG           Reserved;
} CSP_NODE, *PCSP_NODE;


typedef struct {
    CSMETHOD        Method;
    ULONG           NodeId;
    ULONG           Reserved;
} CSM_NODE, *PCSM_NODE;

typedef struct {
    CSEVENT         Event;
    ULONG           NodeId;
    ULONG           Reserved;
} CSE_NODE, *PCSE_NODE;

#endif //#ifndef UNDER_CE

#define STATIC_CSPROPTYPESETID_General \
    0x97E99BA0L, 0xBDEA, 0x11CF, 0xA5, 0xD6, 0x28, 0xDB, 0x04, 0xC1, 0x00, 0x00
DEFINE_GUIDSTRUCT("97E99BA0-BDEA-11CF-A5D6-28DB04C10000", CSPROPTYPESETID_General);
#define CSPROPTYPESETID_General DEFINE_GUIDNAMED(CSPROPTYPESETID_General)

#if defined(_NTDDK_) && !defined(__wtypes_h__)
enum VARENUM {
    VT_EMPTY = 0,
    VT_NULL = 1,
    VT_I2 = 2,
    VT_I4 = 3,
    VT_R4 = 4,
    VT_R8 = 5,
    VT_CY = 6,
    VT_DATE = 7,
    VT_BSTR = 8,
    VT_DISPATCH = 9,
    VT_ERROR = 10,
    VT_BOOL = 11,
    VT_VARIANT = 12,
    VT_UNKNOWN = 13,
    VT_DECIMAL = 14,
    VT_I1 = 16,
    VT_UI1 = 17,
    VT_UI2 = 18,
    VT_UI4 = 19,
    VT_I8 = 20,
    VT_UI8 = 21,
    VT_INT = 22,
    VT_UINT = 23,
    VT_VOID = 24,
    VT_HRESULT  = 25,
    VT_PTR = 26,
    VT_SAFEARRAY = 27,
    VT_CARRAY = 28,
    VT_USERDEFINED = 29,
    VT_LPSTR = 30,
    VT_LPWSTR = 31,
    VT_FILETIME = 64,
    VT_BLOB = 65,
    VT_STREAM = 66,
    VT_STORAGE = 67,
    VT_STREAMED_OBJECT = 68,
    VT_STORED_OBJECT = 69,
    VT_BLOB_OBJECT = 70,
    VT_CF = 71,
    VT_CLSID = 72,
    VT_VECTOR = 0x1000,
    VT_ARRAY = 0x2000,
    VT_BYREF = 0x4000,
    VT_RESERVED = 0x8000,
    VT_ILLEGAL = 0xffff,
    VT_ILLEGALMASKED = 0xfff,
    VT_TYPEMASK = 0xfff
};
#endif // _NTDDK_ && !__wtypes_h__

typedef struct {
    ULONG    Size;
    ULONG    Count;
} CSMULTIPLE_ITEM, *PCSMULTIPLE_ITEM;

typedef struct {
    ULONG           AccessFlags;
    ULONG           DescriptionSize;
    CSIDENTIFIER    PropTypeSet;
    ULONG           MembersListCount;
    ULONG           Reserved;
} CSPROPERTY_DESCRIPTION, *PCSPROPERTY_DESCRIPTION;

#define CSPROPERTY_MEMBER_RANGES            0x00000001
#define CSPROPERTY_MEMBER_STEPPEDRANGES     0x00000002
#define CSPROPERTY_MEMBER_VALUES            0x00000003

#define CSPROPERTY_MEMBER_FLAG_DEFAULT                      0x00000001
#define CSPROPERTY_MEMBER_FLAG_BASICSUPPORT_MULTICHANNEL    0x00000002
#define CSPROPERTY_MEMBER_FLAG_BASICSUPPORT_UNIFORM         0x00000004

typedef struct {
    ULONG   MembersFlags;
    ULONG   MembersSize;
    ULONG   MembersCount;
    ULONG   Flags;
} CSPROPERTY_MEMBERSHEADER, *PCSPROPERTY_MEMBERSHEADER;

typedef union {
#if defined( _CS_NO_ANONYMOUS_STRUCTURES_ )
    struct _SIGNED {
#else
    struct {
#endif    
        LONG    SignedMinimum;
        LONG    SignedMaximum;
    };
#if defined( _CS_NO_ANONYMOUS_STRUCTURES_ )
    struct _UNSIGNED {
#else
    struct {
#endif    
        ULONG   UnsignedMinimum;
        ULONG   UnsignedMaximum;
    };
} CSPROPERTY_BOUNDS_LONG, *PCSPROPERTY_BOUNDS_LONG;

typedef union {
#if defined( _CS_NO_ANONYMOUS_STRUCTURES_ )
    struct _SIGNED64 {
#else
    struct {
#endif    
        LONGLONG    SignedMinimum;
        LONGLONG    SignedMaximum;
    };
#if defined( _CS_NO_ANONYMOUS_STRUCTURES_ )
    struct _UNSIGNED64 {
#else
    struct {
#endif    
#if defined(_NTDDK_)
        ULONGLONG   UnsignedMinimum;
        ULONGLONG   UnsignedMaximum;
#else // !_NTDDK_
        DWORDLONG   UnsignedMinimum;
        DWORDLONG   UnsignedMaximum;
#endif // !_NTDDK_
    };
} CSPROPERTY_BOUNDS_LONGLONG, *PCSPROPERTY_BOUNDS_LONGLONG;

typedef struct {
    ULONG                       SteppingDelta;
    ULONG                       Reserved;
    CSPROPERTY_BOUNDS_LONG      Bounds;
} CSPROPERTY_STEPPING_LONG, *PCSPROPERTY_STEPPING_LONG;

typedef struct {
#if defined(_NTDDK_)
    ULONGLONG                   SteppingDelta;
#else // !_NTDDK_
    DWORDLONG                   SteppingDelta;
#endif // !_NTDDK_
    CSPROPERTY_BOUNDS_LONGLONG  Bounds;
} CSPROPERTY_STEPPING_LONGLONG, *PCSPROPERTY_STEPPING_LONGLONG;

//===========================================================================
#ifndef UNDER_CE
#if defined(_NTDDK_)
//
// Structure forward declarations.
//
typedef struct _CSDEVICE_DESCRIPTOR
CSDEVICE_DESCRIPTOR, *PCSDEVICE_DESCRIPTOR;
typedef struct _CSDEVICE_DISPATCH
CSDEVICE_DISPATCH, *PCSDEVICE_DISPATCH;
typedef struct _CSDEVICE 
CSDEVICE, *PCSDEVICE;
typedef struct _CSFILTERFACTORY 
CSFILTERFACTORY, *PCSFILTERFACTORY;
typedef struct _CSFILTER_DESCRIPTOR
CSFILTER_DESCRIPTOR, *PCSFILTER_DESCRIPTOR;
typedef struct _CSFILTER_DISPATCH
CSFILTER_DISPATCH, *PCSFILTER_DISPATCH;
typedef struct _CSFILTER 
CSFILTER, *PCSFILTER;
typedef struct _CSPIN_DESCRIPTOR_EX
CSPIN_DESCRIPTOR_EX, *PCSPIN_DESCRIPTOR_EX;
typedef struct _CSPIN_DISPATCH
CSPIN_DISPATCH, *PCSPIN_DISPATCH;
typedef struct _CSCLOCK_DISPATCH
CSCLOCK_DISPATCH, *PCSCLOCK_DISPATCH;
typedef struct _CSALLOCATOR_DISPATCH
CSALLOCATOR_DISPATCH, *PCSALLOCATOR_DISPATCH;
typedef struct _CSPIN 
CSPIN, *PCSPIN;
typedef struct _CSNODE_DESCRIPTOR
CSNODE_DESCRIPTOR, *PCSNODE_DESCRIPTOR;
typedef struct _CSSTREAM_POINTER_OFFSET
CSSTREAM_POINTER_OFFSET, *PCSSTREAM_POINTER_OFFSET;
typedef struct _CSSTREAM_POINTER
CSSTREAM_POINTER, *PCSSTREAM_POINTER;
typedef struct _CSMAPPING
CSMAPPING, *PCSMAPPING;
typedef struct _CSPROCESSPIN
CSPROCESSPIN, *PCSPROCESSPIN;
typedef struct _CSPROCESSPIN_INDEXENTRY
CSPROCESSPIN_INDEXENTRY, *PCSPROCESSPIN_INDEXENTRY;
#endif // _NTDDK_

typedef PVOID PCSWORKER;

typedef struct {
    ULONG       NotificationType;
    union {
        struct {
            HANDLE              Event;
            ULONG_PTR           Reserved[2];
        } EventHandle;
        struct {
            HANDLE              Semaphore;
            ULONG               Reserved;
            LONG                Adjustment;
        } SemaphoreHandle;
#if defined(_NTDDK_)
        struct {
            PVOID               Event;
            KPRIORITY           Increment;
            ULONG_PTR           Reserved;
        } EventObject;
        struct {
            PVOID               Semaphore;
            KPRIORITY           Increment;
            LONG                Adjustment;
        } SemaphoreObject;
        struct {
            PKDPC               Dpc;
            ULONG               ReferenceCount;
            ULONG_PTR           Reserved;
        } Dpc;
        struct {
            PWORK_QUEUE_ITEM    WorkQueueItem;
            WORK_QUEUE_TYPE     WorkQueueType;
            ULONG_PTR           Reserved;
        } WorkItem;
        struct {
            PWORK_QUEUE_ITEM    WorkQueueItem;
            PCSWORKER           CsWorkerObject;
            ULONG_PTR           Reserved;
        } CsWorkItem;
// @@BEGIN_DDCSPLIT
        struct {
            PCSFILTER           Filter;
            ULONG_PTR           Reserved[2];
        } CsFilterProcessing;
        struct {
            PCSPIN              Pin;
            ULONG_PTR           Reserved[2];
        } CsPinProcessing;
// @@END_DDCSPLIT
#endif // defined(_NTDDK_)
        struct {
            PVOID               Unused;
            LONG_PTR            Alignment[2];
        } Alignment;
    };
} CSEVENTDATA, *PCSEVENTDATA;


#define CSEVENTF_EVENT_HANDLE       0x00000001
#define CSEVENTF_SEMAPHORE_HANDLE   0x00000002
#if defined(_NTDDK_)
#define CSEVENTF_EVENT_OBJECT       0x00000004
#define CSEVENTF_SEMAPHORE_OBJECT   0x00000008
#define CSEVENTF_DPC                0x00000010
#define CSEVENTF_WORKITEM           0x00000020
#define CSEVENTF_CSWORKITEM         0x00000080
// @@BEGIN_DDCSPLIT
#define CSEVENTF_CSFILTERPROCESSING 0x00000100
#define CSEVENTF_CSPINPROCESSING    0x00000200
// @@END_DDCSPLIT
#endif // defined(_NTDDK_)

#define CSEVENT_TYPE_ENABLE         0x00000001
#define CSEVENT_TYPE_ONESHOT        0x00000002
#define CSEVENT_TYPE_ENABLEBUFFERED 0x00000004
#define CSEVENT_TYPE_SETSUPPORT     0x00000100
#define CSEVENT_TYPE_BASICSUPPORT   0x00000200
#define CSEVENT_TYPE_QUERYBUFFER    0x00000400

#define CSEVENT_TYPE_TOPOLOGY 0x10000000

typedef struct {
    CSEVENT         Event;
    PCSEVENTDATA    EventData;
    PVOID           Reserved;
} CSQUERYBUFFER, *PCSQUERYBUFFER;

typedef struct {
    ULONG Size;
    ULONG Flags;
    union {
        HANDLE ObjectHandle;
        PVOID ObjectPointer;
    };
    PVOID Reserved;
    CSEVENT Event;
    CSEVENTDATA EventData;
} CSRELATIVEEVENT;

#define CSRELATIVEEVENT_FLAG_HANDLE 0x00000001
#define CSRELATIVEEVENT_FLAG_POINTER 0x00000002

//===========================================================================

typedef struct {
    CSEVENTDATA     EventData;
    LONGLONG        MarkTime;
} CSEVENT_TIME_MARK, *PCSEVENT_TIME_MARK;

typedef struct {
    CSEVENTDATA     EventData;
    LONGLONG        TimeBase;
    LONGLONG        Interval;
} CSEVENT_TIME_INTERVAL, *PCSEVENT_TIME_INTERVAL;

typedef struct {
    LONGLONG        TimeBase;
    LONGLONG        Interval;
} CSINTERVAL, *PCSINTERVAL;

#endif //#ifndef UNDER_CE
//===========================================================================
				
#define STATIC_CSPROPSETID_General\
    0x1464EDA5L, 0x6A8F, 0x11D1, 0x9A, 0xA7, 0x00, 0xA0, 0xC9, 0x22, 0x31, 0x96
DEFINE_GUIDSTRUCT("1464EDA5-6A8F-11D1-9AA7-00A0C9223196", CSPROPSETID_General);
#define CSPROPSETID_General DEFINE_GUIDNAMED(CSPROPSETID_General)

typedef enum {
    CSPROPERTY_GENERAL_COMPONENTID
} CSPROPERTY_GENERAL;

#ifndef UNDER_CE
typedef struct {
    GUID    Manufacturer;
    GUID    Product;
    GUID    Component;
    GUID    Name;
    ULONG   Version;
    ULONG   Revision;
} CSCOMPONENTID, *PCSCOMPONENTID;

#define DEFINE_CSPROPERTY_ITEM_GENERAL_COMPONENTID(Handler)\
    DEFINE_CSPROPERTY_ITEM(\
        CSPROPERTY_GENERAL_COMPONENTID,\
        (Handler),\
        sizeof(CSPROPERTY),\
        sizeof(CSCOMPONENTID),\
        NULL, NULL, 0, NULL, NULL, 0)

#define STATIC_CSMETHODSETID_StreamIo\
    0x65D003CAL, 0x1523, 0x11D2, 0xB2, 0x7A, 0x00, 0xA0, 0xC9, 0x22, 0x31, 0x96
DEFINE_GUIDSTRUCT("65D003CA-1523-11D2-B27A-00A0C9223196", CSMETHODSETID_StreamIo);
#define CSMETHODSETID_StreamIo DEFINE_GUIDNAMED(CSMETHODSETID_StreamIo)

typedef enum {
    CSMETHOD_STREAMIO_READ,
    CSMETHOD_STREAMIO_WRITE
} CSMETHOD_STREAMIO;

#define DEFINE_CSMETHOD_ITEM_STREAMIO_READ(Handler)\
    DEFINE_CSMETHOD_ITEM(\
        CSMETHOD_STREAMIO_READ,\
        CSMETHOD_TYPE_WRITE,\
        (Handler),\
        sizeof(CSMETHOD),\
        0,\
        NULL)

#define DEFINE_CSMETHOD_ITEM_STREAMIO_WRITE(Handler)\
    DEFINE_CSMETHOD_ITEM(\
        CSMETHOD_STREAMIO_WRITE,\
        CSMETHOD_TYPE_READ,\
        (Handler),\
        sizeof(CSMETHOD),\
        0,\
        NULL)

#define STATIC_CSPROPSETID_MediaSeeking\
    0xEE904F0CL, 0xD09B, 0x11D0, 0xAB, 0xE9, 0x00, 0xA0, 0xC9, 0x22, 0x31, 0x96
DEFINE_GUIDSTRUCT("EE904F0C-D09B-11D0-ABE9-00A0C9223196", CSPROPSETID_MediaSeeking);
#define CSPROPSETID_MediaSeeking DEFINE_GUIDNAMED(CSPROPSETID_MediaSeeking)

typedef enum {
    CSPROPERTY_MEDIASEEKING_CAPABILITIES,
    CSPROPERTY_MEDIASEEKING_FORMATS,
    CSPROPERTY_MEDIASEEKING_TIMEFORMAT,
    CSPROPERTY_MEDIASEEKING_POSITION,
    CSPROPERTY_MEDIASEEKING_STOPPOSITION,
    CSPROPERTY_MEDIASEEKING_POSITIONS,
    CSPROPERTY_MEDIASEEKING_DURATION,
    CSPROPERTY_MEDIASEEKING_AVAILABLE,
    CSPROPERTY_MEDIASEEKING_PREROLL,
    CSPROPERTY_MEDIASEEKING_CONVERTTIMEFORMAT
} CSPROPERTY_MEDIASEEKING;

typedef enum {
    CS_SEEKING_NoPositioning,
    CS_SEEKING_AbsolutePositioning,
    CS_SEEKING_RelativePositioning,
    CS_SEEKING_IncrementalPositioning,
    CS_SEEKING_PositioningBitsMask = 0x3,
    CS_SEEKING_SeekToKeyFrame,
    CS_SEEKING_ReturnTime = 0x8
} CS_SEEKING_FLAGS;

typedef enum {
    CS_SEEKING_CanSeekAbsolute = 0x1,
    CS_SEEKING_CanSeekForwards = 0x2,
    CS_SEEKING_CanSeekBackwards = 0x4,
    CS_SEEKING_CanGetCurrentPos = 0x8,
    CS_SEEKING_CanGetStopPos = 0x10,
    CS_SEEKING_CanGetDuration = 0x20,
    CS_SEEKING_CanPlayBackwards = 0x40
} CS_SEEKING_CAPABILITIES;

typedef struct {
    LONGLONG            Current;
    LONGLONG            Stop;
    CS_SEEKING_FLAGS    CurrentFlags;
    CS_SEEKING_FLAGS    StopFlags;
} CSPROPERTY_POSITIONS, *PCSPROPERTY_POSITIONS;

typedef struct {
    LONGLONG    Earliest;
    LONGLONG    Latest;
} CSPROPERTY_MEDIAAVAILABLE, *PCSPROPERTY_MEDIAAVAILABLE;

typedef struct {
    CSPROPERTY  Property;
    GUID        SourceFormat;
    GUID        TargetFormat;
    LONGLONG    Time;
} CSP_TIMEFORMAT, *PCSP_TIMEFORMAT;

#define DEFINE_CSPROPERTY_ITEM_MEDIASEEKING_CAPABILITIES(Handler)\
    DEFINE_CSPROPERTY_ITEM(\
        CSPROPERTY_MEDIASEEKING_CAPABILITIES,\
        (Handler),\
        sizeof(CSPROPERTY),\
        sizeof(CS_SEEKING_CAPABILITIES),\
        NULL, NULL, 0, NULL, NULL, 0)

#define DEFINE_CSPROPERTY_ITEM_MEDIASEEKING_FORMATS(Handler)\
    DEFINE_CSPROPERTY_ITEM(\
        CSPROPERTY_MEDIASEEKING_FORMATS,\
        (Handler),\
        sizeof(CSPROPERTY),\
        0,\
        NULL, NULL, 0, NULL, NULL, 0)

#define DEFINE_CSPROPERTY_ITEM_MEDIASEEKING_TIMEFORMAT(GetHandler, SetHandler)\
    DEFINE_CSPROPERTY_ITEM(\
        CSPROPERTY_MEDIASEEKING_TIMEFORMAT,\
        (GetHandler),\
        sizeof(CSPROPERTY),\
        sizeof(GUID),\
        (SetHandler),\
        NULL, 0, NULL, NULL, 0)

#define DEFINE_CSPROPERTY_ITEM_MEDIASEEKING_POSITION(Handler)\
    DEFINE_CSPROPERTY_ITEM(\
        CSPROPERTY_MEDIASEEKING_POSITION,\
        (Handler),\
        sizeof(CSPROPERTY),\
        sizeof(LONGLONG),\
        NULL, NULL, 0, NULL, NULL, 0)

#define DEFINE_CSPROPERTY_ITEM_MEDIASEEKING_STOPPOSITION(Handler)\
    DEFINE_CSPROPERTY_ITEM(\
        CSPROPERTY_MEDIASEEKING_STOPPOSITION,\
        (Handler),\
        sizeof(CSPROPERTY),\
        sizeof(LONGLONG),\
        NULL, NULL, 0, NULL, NULL, 0)

#define DEFINE_CSPROPERTY_ITEM_MEDIASEEKING_POSITIONS(Handler)\
    DEFINE_CSPROPERTY_ITEM(\
        CSPROPERTY_MEDIASEEKING_POSITIONS,\
        NULL,\
        sizeof(CSPROPERTY),\
        sizeof(CSPROPERTY_POSITIONS),\
        (Handler),\
        NULL, 0, NULL, NULL, 0)

#define DEFINE_CSPROPERTY_ITEM_MEDIASEEKING_DURATION(Handler)\
    DEFINE_CSPROPERTY_ITEM(\
        CSPROPERTY_MEDIASEEKING_DURATION,\
        (Handler),\
        sizeof(CSPROPERTY),\
        sizeof(LONGLONG),\
        NULL, NULL, 0, NULL, NULL, 0)

#define DEFINE_CSPROPERTY_ITEM_MEDIASEEKING_AVAILABLE(Handler)\
    DEFINE_CSPROPERTY_ITEM(\
        CSPROPERTY_MEDIASEEKING_AVAILABLE,\
        (Handler),\
        sizeof(CSPROPERTY),\
        sizeof(CSPROPERTY_MEDIAAVAILABLE),\
        NULL, NULL, 0, NULL, NULL, 0)

#define DEFINE_CSPROPERTY_ITEM_MEDIASEEKING_PREROLL(Handler)\
    DEFINE_CSPROPERTY_ITEM(\
        CSPROPERTY_MEDIASEEKING_PREROLL,\
        (Handler),\
        sizeof(CSPROPERTY),\
        sizeof(LONGLONG),\
        NULL, NULL, 0, NULL, NULL, 0)

#define DEFINE_CSPROPERTY_ITEM_MEDIASEEKING_CONVERTTIMEFORMAT(Handler)\
    DEFINE_CSPROPERTY_ITEM(\
        CSPROPERTY_MEDIASEEKING_CONVERTTIMEFORMAT,\
        (Handler),\
        sizeof(CSP_TIMEFORMAT),\
        sizeof(LONGLONG),\
        NULL, NULL, 0, NULL, NULL, 0)

//===========================================================================

#define STATIC_CSPROPSETID_Topology\
    0x720D4AC0L, 0x7533, 0x11D0, 0xA5, 0xD6, 0x28, 0xDB, 0x04, 0xC1, 0x00, 0x00
DEFINE_GUIDSTRUCT("720D4AC0-7533-11D0-A5D6-28DB04C10000", CSPROPSETID_Topology);
#define CSPROPSETID_Topology DEFINE_GUIDNAMED(CSPROPSETID_Topology)

typedef enum {
    CSPROPERTY_TOPOLOGY_CATEGORIES,
    CSPROPERTY_TOPOLOGY_NODES,
    CSPROPERTY_TOPOLOGY_CONNECTIONS,
    CSPROPERTY_TOPOLOGY_NAME
} CSPROPERTY_TOPOLOGY;

#define DEFINE_CSPROPERTY_ITEM_TOPOLOGY_CATEGORIES(Handler)\
    DEFINE_CSPROPERTY_ITEM(\
        CSPROPERTY_TOPOLOGY_CATEGORIES,\
        (Handler),\
        sizeof(CSPROPERTY),\
        0,\
        NULL, NULL, 0, NULL, NULL, 0)

#define DEFINE_CSPROPERTY_ITEM_TOPOLOGY_NODES(Handler)\
    DEFINE_CSPROPERTY_ITEM(\
        CSPROPERTY_TOPOLOGY_NODES,\
        (Handler),\
        sizeof(CSPROPERTY),\
        0,\
        NULL, NULL, 0, NULL, NULL, 0)

#define DEFINE_CSPROPERTY_ITEM_TOPOLOGY_CONNECTIONS(Handler)\
    DEFINE_CSPROPERTY_ITEM(\
        CSPROPERTY_TOPOLOGY_CONNECTIONS,\
        (Handler),\
        sizeof(CSPROPERTY),\
        0,\
        NULL, NULL, 0, NULL, NULL, 0)

#define DEFINE_CSPROPERTY_ITEM_TOPOLOGY_NAME(Handler)\
    DEFINE_CSPROPERTY_ITEM(\
        CSPROPERTY_TOPOLOGY_NAME,\
        (Handler),\
        sizeof(CSP_NODE),\
        0,\
        NULL, NULL, 0, NULL, NULL, 0)

#define DEFINE_CSPROPERTY_TOPOLOGYSET(TopologySet, Handler)\
DEFINE_CSPROPERTY_TABLE(TopologySet) {\
    DEFINE_CSPROPERTY_ITEM_TOPOLOGY_CATEGORIES(Handler),\
    DEFINE_CSPROPERTY_ITEM_TOPOLOGY_NODES(Handler),\
    DEFINE_CSPROPERTY_ITEM_TOPOLOGY_CONNECTIONS(Handler),\
    DEFINE_CSPROPERTY_ITEM_TOPOLOGY_NAME(Handler)\
}

//=============================================================================

//
// properties used by graph manager to talk to particular filters
//
#if defined(_NTDDK_)

#define STATIC_CSPROPSETID_GM \
    0xAF627536L, 0xE719, 0x11D2, 0x8A, 0x1D, 0x00, 0x60, 0x97, 0xD2, 0xDF, 0x5D    
DEFINE_GUIDSTRUCT("AF627536-E719-11D2-8A1D-006097D2DF5D", CSPROPSETID_GM);
#define CSPROPSETID_GM DEFINE_GUIDNAMED(CSPROPSETID_GM)

typedef VOID (*PFNCSGRAPHMANAGER_NOTIFY)(IN PFILE_OBJECT GraphManager,
                                         IN ULONG EventId,
                                         IN PVOID Filter,
                                         IN PVOID Pin,
                                         IN PVOID Frame,
                                         IN ULONG Duration);

typedef struct CSGRAPHMANAGER_FUNCTIONTABLE {
    PFNCSGRAPHMANAGER_NOTIFY NotifyEvent;
} CSGRAPHMANAGER_FUNCTIONTABLE, PCSGRAPHMANAGER_FUNCTIONTABLE;

typedef struct _CSPROPERTY_GRAPHMANAGER_INTERFACE {
    PFILE_OBJECT                 GraphManager;
    CSGRAPHMANAGER_FUNCTIONTABLE FunctionTable;
} CSPROPERTY_GRAPHMANAGER_INTERFACE, *PCSPROPERTY_GRAPHMANAGER_INTERFACE;


//
// Commands
//
typedef enum {
    CSPROPERTY_GM_GRAPHMANAGER,    
    CSPROPERTY_GM_TIMESTAMP_CLOCK, 
    CSPROPERTY_GM_RATEMATCH,       
    CSPROPERTY_GM_RENDER_CLOCK,    
} CSPROPERTY_GM;

#endif

//===========================================================================


#define STATIC_CSCATEGORY_BRIDGE \
    0x085AFF00L, 0x62CE, 0x11CF, 0xA5, 0xD6, 0x28, 0xDB, 0x04, 0xC1, 0x00, 0x00
DEFINE_GUIDSTRUCT("085AFF00-62CE-11CF-A5D6-28DB04C10000", CSCATEGORY_BRIDGE);
#define CSCATEGORY_BRIDGE DEFINE_GUIDNAMED(CSCATEGORY_BRIDGE)

#define STATIC_CSCATEGORY_CAPTURE \
    0x65E8773DL, 0x8F56, 0x11D0, 0xA3, 0xB9, 0x00, 0xA0, 0xC9, 0x22, 0x31, 0x96
DEFINE_GUIDSTRUCT("65E8773D-8F56-11D0-A3B9-00A0C9223196", CSCATEGORY_CAPTURE);
#define CSCATEGORY_CAPTURE DEFINE_GUIDNAMED(CSCATEGORY_CAPTURE)

#define STATIC_CSCATEGORY_RENDER \
    0x65E8773EL, 0x8F56, 0x11D0, 0xA3, 0xB9, 0x00, 0xA0, 0xC9, 0x22, 0x31, 0x96
DEFINE_GUIDSTRUCT("65E8773E-8F56-11D0-A3B9-00A0C9223196", CSCATEGORY_RENDER);
#define CSCATEGORY_RENDER DEFINE_GUIDNAMED(CSCATEGORY_RENDER)

#define STATIC_CSCATEGORY_MIXER \
    0xAD809C00L, 0x7B88, 0x11D0, 0xA5, 0xD6, 0x28, 0xDB, 0x04, 0xC1, 0x00, 0x00
DEFINE_GUIDSTRUCT("AD809C00-7B88-11D0-A5D6-28DB04C10000", CSCATEGORY_MIXER);
#define CSCATEGORY_MIXER DEFINE_GUIDNAMED(CSCATEGORY_MIXER)

#define STATIC_CSCATEGORY_SPLITTER \
    0x0A4252A0L, 0x7E70, 0x11D0, 0xA5, 0xD6, 0x28, 0xDB, 0x04, 0xC1, 0x00, 0x00
DEFINE_GUIDSTRUCT("0A4252A0-7E70-11D0-A5D6-28DB04C10000", CSCATEGORY_SPLITTER);
#define CSCATEGORY_SPLITTER DEFINE_GUIDNAMED(CSCATEGORY_SPLITTER)

#define STATIC_CSCATEGORY_DATACOMPRESSOR \
    0x1E84C900L, 0x7E70, 0x11D0, 0xA5, 0xD6, 0x28, 0xDB, 0x04, 0xC1, 0x00, 0x00
DEFINE_GUIDSTRUCT("1E84C900-7E70-11D0-A5D6-28DB04C10000", CSCATEGORY_DATACOMPRESSOR);
#define CSCATEGORY_DATACOMPRESSOR DEFINE_GUIDNAMED(CSCATEGORY_DATACOMPRESSOR)

#define STATIC_CSCATEGORY_DATADECOMPRESSOR \
    0x2721AE20L, 0x7E70, 0x11D0, 0xA5, 0xD6, 0x28, 0xDB, 0x04, 0xC1, 0x00, 0x00
DEFINE_GUIDSTRUCT("2721AE20-7E70-11D0-A5D6-28DB04C10000", CSCATEGORY_DATADECOMPRESSOR);
#define CSCATEGORY_DATADECOMPRESSOR DEFINE_GUIDNAMED(CSCATEGORY_DATADECOMPRESSOR)

#define STATIC_CSCATEGORY_DATATRANSFORM \
    0x2EB07EA0L, 0x7E70, 0x11D0, 0xA5, 0xD6, 0x28, 0xDB, 0x04, 0xC1, 0x00, 0x00
DEFINE_GUIDSTRUCT("2EB07EA0-7E70-11D0-A5D6-28DB04C10000", CSCATEGORY_DATATRANSFORM);
#define CSCATEGORY_DATATRANSFORM DEFINE_GUIDNAMED(CSCATEGORY_DATATRANSFORM)

#define STATIC_CSCATEGORY_COMMUNICATIONSTRANSFORM \
    0xCF1DDA2CL, 0x9743, 0x11D0, 0xA3, 0xEE, 0x00, 0xA0, 0xC9, 0x22, 0x31, 0x96
DEFINE_GUIDSTRUCT("CF1DDA2C-9743-11D0-A3EE-00A0C9223196", CSCATEGORY_COMMUNICATIONSTRANSFORM);
#define CSCATEGORY_COMMUNICATIONSTRANSFORM DEFINE_GUIDNAMED(CSCATEGORY_COMMUNICATIONSTRANSFORM)

#define STATIC_CSCATEGORY_INTERFACETRANSFORM \
    0xCF1DDA2DL, 0x9743, 0x11D0, 0xA3, 0xEE, 0x00, 0xA0, 0xC9, 0x22, 0x31, 0x96
DEFINE_GUIDSTRUCT("CF1DDA2D-9743-11D0-A3EE-00A0C9223196", CSCATEGORY_INTERFACETRANSFORM);
#define CSCATEGORY_INTERFACETRANSFORM DEFINE_GUIDNAMED(CSCATEGORY_INTERFACETRANSFORM)

#define STATIC_CSCATEGORY_MEDIUMTRANSFORM \
    0xCF1DDA2EL, 0x9743, 0x11D0, 0xA3, 0xEE, 0x00, 0xA0, 0xC9, 0x22, 0x31, 0x96
DEFINE_GUIDSTRUCT("CF1DDA2E-9743-11D0-A3EE-00A0C9223196", CSCATEGORY_MEDIUMTRANSFORM);
#define CSCATEGORY_MEDIUMTRANSFORM DEFINE_GUIDNAMED(CSCATEGORY_MEDIUMTRANSFORM)

#define STATIC_CSCATEGORY_FILESYSTEM \
    0x760FED5EL, 0x9357, 0x11D0, 0xA3, 0xCC, 0x00, 0xA0, 0xC9, 0x22, 0x31, 0x96
DEFINE_GUIDSTRUCT("760FED5E-9357-11D0-A3CC-00A0C9223196", CSCATEGORY_FILESYSTEM);
#define CSCATEGORY_FILESYSTEM DEFINE_GUIDNAMED(CSCATEGORY_FILESYSTEM)

// CSNAME_Clock
#define STATIC_CSCATEGORY_CLOCK \
    0x53172480L, 0x4791, 0x11D0, 0xA5, 0xD6, 0x28, 0xDB, 0x04, 0xC1, 0x00, 0x00
DEFINE_GUIDSTRUCT("53172480-4791-11D0-A5D6-28DB04C10000", CSCATEGORY_CLOCK);
#define CSCATEGORY_CLOCK DEFINE_GUIDNAMED(CSCATEGORY_CLOCK)

#define STATIC_CSCATEGORY_PROXY \
    0x97EBAACAL, 0x95BD, 0x11D0, 0xA3, 0xEA, 0x00, 0xA0, 0xC9, 0x22, 0x31, 0x96
DEFINE_GUIDSTRUCT("97EBAACA-95BD-11D0-A3EA-00A0C9223196", CSCATEGORY_PROXY);
#define CSCATEGORY_PROXY DEFINE_GUIDNAMED(CSCATEGORY_PROXY)

#define STATIC_CSCATEGORY_QUALITY \
    0x97EBAACBL, 0x95BD, 0x11D0, 0xA3, 0xEA, 0x00, 0xA0, 0xC9, 0x22, 0x31, 0x96
DEFINE_GUIDSTRUCT("97EBAACB-95BD-11D0-A3EA-00A0C9223196", CSCATEGORY_QUALITY);
#define CSCATEGORY_QUALITY DEFINE_GUIDNAMED(CSCATEGORY_QUALITY)

typedef struct {
    ULONG   FromNode;
    ULONG   FromNodePin;
    ULONG   ToNode;
    ULONG   ToNodePin;
} CSTOPOLOGY_CONNECTION, *PCSTOPOLOGY_CONNECTION;

typedef struct {
    ULONG                           CategoriesCount;
    const GUID*                     Categories;
    ULONG                           TopologyNodesCount;
    const GUID*                     TopologyNodes;
    ULONG                           TopologyConnectionsCount;
    const CSTOPOLOGY_CONNECTION*    TopologyConnections;
    const GUID*                     TopologyNodesNames;
    ULONG                           Reserved;
} CSTOPOLOGY, *PCSTOPOLOGY;

#define CSFILTER_NODE   ((ULONG)-1)
#define CSALL_NODES     ((ULONG)-1)

typedef struct {
    ULONG       CreateFlags;
    ULONG       Node;
} CSNODE_CREATE, *PCSNODE_CREATE;

//===========================================================================

// TIME_FORMAT_NONE
#define STATIC_CSTIME_FORMAT_NONE       STATIC_GUID_NULL
#define CSTIME_FORMAT_NONE              GUID_NULL

// TIME_FORMAT_FRAME
#define STATIC_CSTIME_FORMAT_FRAME\
    0x7b785570L, 0x8c82, 0x11cf, 0xbc, 0x0c, 0x00, 0xaa, 0x00, 0xac, 0x74, 0xf6
DEFINE_GUIDSTRUCT("7b785570-8c82-11cf-bc0c-00aa00ac74f6", CSTIME_FORMAT_FRAME);
#define CSTIME_FORMAT_FRAME DEFINE_GUIDNAMED(CSTIME_FORMAT_FRAME)

// TIME_FORMAT_BYTE             
#define STATIC_CSTIME_FORMAT_BYTE\
    0x7b785571L, 0x8c82, 0x11cf, 0xbc, 0x0c, 0x00, 0xaa, 0x00, 0xac, 0x74, 0xf6
DEFINE_GUIDSTRUCT("7b785571-8c82-11cf-bc0c-00aa00ac74f6", CSTIME_FORMAT_BYTE);
#define CSTIME_FORMAT_BYTE DEFINE_GUIDNAMED(CSTIME_FORMAT_BYTE)

// TIME_FORMAT_SAMPLE
#define STATIC_CSTIME_FORMAT_SAMPLE\
    0x7b785572L, 0x8c82, 0x11cf, 0xbc, 0x0c, 0x00, 0xaa, 0x00, 0xac, 0x74, 0xf6
DEFINE_GUIDSTRUCT("7b785572-8c82-11cf-bc0c-00aa00ac74f6", CSTIME_FORMAT_SAMPLE);
#define CSTIME_FORMAT_SAMPLE DEFINE_GUIDNAMED(CSTIME_FORMAT_SAMPLE)

// TIME_FORMAT_FIELD
#define STATIC_CSTIME_FORMAT_FIELD\
    0x7b785573L, 0x8c82, 0x11cf, 0xbc, 0x0c, 0x00, 0xaa, 0x00, 0xac, 0x74, 0xf6
DEFINE_GUIDSTRUCT("7b785573-8c82-11cf-bc0c-00aa00ac74f6", CSTIME_FORMAT_FIELD);
#define CSTIME_FORMAT_FIELD DEFINE_GUIDNAMED(CSTIME_FORMAT_FIELD)

// TIME_FORMAT_MEDIA_TIME
#define STATIC_CSTIME_FORMAT_MEDIA_TIME\
    0x7b785574L, 0x8c82, 0x11cf, 0xbc, 0x0c, 0x00, 0xaa, 0x00, 0xac, 0x74, 0xf6
DEFINE_GUIDSTRUCT("7b785574-8c82-11cf-bc0c-00aa00ac74f6", CSTIME_FORMAT_MEDIA_TIME);
#define CSTIME_FORMAT_MEDIA_TIME DEFINE_GUIDNAMED(CSTIME_FORMAT_MEDIA_TIME)


//===========================================================================

typedef CSIDENTIFIER CSPIN_INTERFACE, *PCSPIN_INTERFACE;

#define STATIC_CSINTERFACESETID_Standard \
    0x1A8766A0L, 0x62CE, 0x11CF, 0xA5, 0xD6, 0x28, 0xDB, 0x04, 0xC1, 0x00, 0x00
DEFINE_GUIDSTRUCT("1A8766A0-62CE-11CF-A5D6-28DB04C10000", CSINTERFACESETID_Standard);
#define CSINTERFACESETID_Standard DEFINE_GUIDNAMED(CSINTERFACESETID_Standard)

typedef enum {
    CSINTERFACE_STANDARD_STREAMING,
    CSINTERFACE_STANDARD_LOOPED_STREAMING,
    CSINTERFACE_STANDARD_CONTROL
} CSINTERFACE_STANDARD;

#define STATIC_CSINTERFACESETID_FileIo \
    0x8C6F932CL, 0xE771, 0x11D0, 0xB8, 0xFF, 0x00, 0xA0, 0xC9, 0x22, 0x31, 0x96
DEFINE_GUIDSTRUCT("8C6F932C-E771-11D0-B8FF-00A0C9223196", CSINTERFACESETID_FileIo);
#define CSINTERFACESETID_FileIo DEFINE_GUIDNAMED(CSINTERFACESETID_FileIo)

typedef enum {
    CSINTERFACE_FILEIO_STREAMING
} CSINTERFACE_FILEIO;

//===========================================================================

#define CSMEDIUM_TYPE_ANYINSTANCE       0

#define STATIC_CSMEDIUMSETID_Standard \
    0x4747B320L, 0x62CE, 0x11CF, 0xA5, 0xD6, 0x28, 0xDB, 0x04, 0xC1, 0x00, 0x00
DEFINE_GUIDSTRUCT("4747B320-62CE-11CF-A5D6-28DB04C10000", CSMEDIUMSETID_Standard);
#define CSMEDIUMSETID_Standard DEFINE_GUIDNAMED(CSMEDIUMSETID_Standard)

//For compatibility only
#define CSMEDIUM_STANDARD_DEVIO     CSMEDIUM_TYPE_ANYINSTANCE
#endif  //#ifndef UNDER_CE
//===========================================================================


#define STATIC_CSPROPSETID_VERSION\
    0x84FF1B03L, 0x2D79, 0x4BFC, 0x8A, 0xEA, 0xBE, 0x85, 0xF4, 0xB9, 0xF4, 0x64
DEFINE_GUIDSTRUCT("84FF1B03-2D79-4BFC-8AEA-BE85F4B9F464", CSPROPSETID_VERSION);
#define CSPROPSETID_VERSION DEFINE_GUIDNAMED(CSPROPSETID_VERSION)


#define STATIC_CSPROPSETID_Pin\
    0x8C134960L, 0x51AD, 0x11CF, 0x87, 0x8A, 0x94, 0xF8, 0x01, 0xC1, 0x00, 0x00
DEFINE_GUIDSTRUCT("8C134960-51AD-11CF-878A-94F801C10000", CSPROPSETID_Pin);
#define CSPROPSETID_Pin DEFINE_GUIDNAMED(CSPROPSETID_Pin)

typedef enum {
    CSPROPERTY_PIN_CINSTANCES=0,
    CSPROPERTY_PIN_CTYPES,
#ifndef UNDER_CE
	CSPROPERTY_PIN_DATAFLOW,
#endif //#ifndef UNDER_CE
	CSPROPERTY_PIN_DATARANGES,
    CSPROPERTY_PIN_DATAINTERSECTION,
#ifndef UNDER_CE
	CSPROPERTY_PIN_INTERFACES,
    CSPROPERTY_PIN_MEDIUMS,
    CSPROPERTY_PIN_COMMUNICATION,
    CSPROPERTY_PIN_GLOBALCINSTANCES,
    CSPROPERTY_PIN_NECESSARYINSTANCES,
    CSPROPERTY_PIN_PHYSICALCONNECTION,
#endif //#ifndef UNDER_CE
	CSPROPERTY_PIN_CATEGORY,
    CSPROPERTY_PIN_NAME,
#ifdef UNDER_CE
    CSPROPERTY_PIN_DEVICENAME
#endif // UNDER_CE
#ifndef UNDER_CE
	,
    CSPROPERTY_PIN_CONSTRAINEDDATARANGES,
    CSPROPERTY_PIN_PROPOSEDATAFORMAT
#endif //#ifndef UNDER_CE
} CSPROPERTY_PIN;

typedef struct {
    CSPROPERTY      Property;
    ULONG           PinId;
    ULONG           Reserved;
} CSP_PIN, *PCSP_PIN;

#define CSINSTANCE_INDETERMINATE    ((ULONG)-1)

typedef struct {
    ULONG  PossibleCount;
    ULONG  CurrentCount;
} CSPIN_CINSTANCES, *PCSPIN_CINSTANCES;

#ifndef UNDER_CE
typedef enum {
    CSPIN_DATAFLOW_IN = 1,
    CSPIN_DATAFLOW_OUT
} CSPIN_DATAFLOW, *PCSPIN_DATAFLOW;
#endif //#ifndef UNDER_CE

#define CSDATAFORMAT_BIT_TEMPORAL_COMPRESSION   0
#define CSDATAFORMAT_TEMPORAL_COMPRESSION       (1 << CSDATAFORMAT_BIT_TEMPORAL_COMPRESSION)
#define CSDATAFORMAT_BIT_ATTRIBUTES 1
#define CSDATAFORMAT_ATTRIBUTES (1 << CSDATAFORMAT_BIT_ATTRIBUTES)

#define CSDATARANGE_BIT_ATTRIBUTES 1
#define CSDATARANGE_ATTRIBUTES (1 << CSDATARANGE_BIT_ATTRIBUTES)
#define CSDATARANGE_BIT_REQUIRED_ATTRIBUTES 2
#define CSDATARANGE_REQUIRED_ATTRIBUTES (1 << CSDATARANGE_BIT_REQUIRED_ATTRIBUTES)

#if !defined( _MSC_VER ) 
typedef struct {
    ULONG   FormatSize;
    ULONG   Flags;
    ULONG   SampleSize;
    ULONG   Reserved;
    GUID    MajorFormat;
    GUID    SubFormat;
    GUID    Specifier;
} CSDATAFORMAT, *PCSDATAFORMAT, CSDATARANGE, *PCSDATARANGE;
#else
typedef union {
    struct {
        ULONG   FormatSize;
        ULONG   Flags;
        ULONG   SampleSize;
        ULONG   Reserved;
        GUID    MajorFormat;
        GUID    SubFormat;
        GUID    Specifier;
    };
    LONGLONG    Alignment;
} CSDATAFORMAT, *PCSDATAFORMAT, CSDATARANGE, *PCSDATARANGE;
#endif

#define CSATTRIBUTE_REQUIRED 0x00000001

#ifndef UNDER_CE
typedef struct {
    ULONG Size;
    ULONG Flags;
    GUID Attribute;
} CSATTRIBUTE, *PCSATTRIBUTE;

typedef enum {
    CSPIN_COMMUNICATION_NONE,
    CSPIN_COMMUNICATION_SINK,
    CSPIN_COMMUNICATION_SOURCE,
    CSPIN_COMMUNICATION_BOTH,
    CSPIN_COMMUNICATION_BRIDGE
} CSPIN_COMMUNICATION, *PCSPIN_COMMUNICATION;

typedef CSIDENTIFIER CSPIN_MEDIUM, *PCSPIN_MEDIUM;

typedef struct {
    CSPIN_INTERFACE Interface;
    CSPIN_MEDIUM    Medium;
    ULONG           PinId;
    HANDLE          PinToHandle;
    CSPRIORITY      Priority;
} CSPIN_CONNECT, *PCSPIN_CONNECT;

typedef struct {
    ULONG   Size;
    ULONG   Pin;
    WCHAR   SymbolicLinkName[1];
} CSPIN_PHYSICALCONNECTION, *PCSPIN_PHYSICALCONNECTION;

#if defined(_NTDDK_)
typedef
NTSTATUS
(*PFNCSINTERSECTHANDLER)(
    IN PIRP Irp,
    IN PCSP_PIN Pin,
    IN PCSDATARANGE DataRange,
    OUT PVOID Data OPTIONAL
    );
typedef
NTSTATUS
(*PFNCSINTERSECTHANDLEREX)(
    IN PVOID Context,
    IN PIRP Irp,
    IN PCSP_PIN Pin,
    IN PCSDATARANGE DataRange,
    IN PCSDATARANGE MatchingDataRange,
    IN ULONG DataBufferSize,
    OUT PVOID Data OPTIONAL,
    OUT PULONG DataSize
    );
#endif // _NTDDK_

#define DEFINE_CSPIN_INTERFACE_TABLE(tablename)\
    const CSPIN_INTERFACE tablename[] =

#define DEFINE_CSPIN_INTERFACE_ITEM(guid, interface)\
    {\
        STATICGUIDOF(guid),\
        (interface),\
        0\
    }

#endif //#ifndef UNDER_CE

#define DEFINE_CSPROPERTY_ITEM_PIN_CINSTANCES(Handler)\
    DEFINE_CSPROPERTY_ITEM(\
        CSPROPERTY_PIN_CINSTANCES,\
        (Handler),\
        sizeof(CSP_PIN),\
        sizeof(CSPIN_CINSTANCES),\
        NULL, NULL, 0, NULL, NULL, 0)

#define DEFINE_CSPROPERTY_ITEM_PIN_CTYPES(Handler)\
    DEFINE_CSPROPERTY_ITEM(\
        CSPROPERTY_PIN_CTYPES,\
        (Handler),\
        sizeof(CSPROPERTY),\
        sizeof(ULONG),\
        NULL, NULL, 0, NULL, NULL, 0)

#ifndef UNDER_CE
#define DEFINE_CSPROPERTY_ITEM_PIN_DATAFLOW(Handler)\
    DEFINE_CSPROPERTY_ITEM(\
        CSPROPERTY_PIN_DATAFLOW,\
        (Handler),\
        sizeof(CSP_PIN),\
        sizeof(CSPIN_DATAFLOW),\
        NULL, NULL, 0, NULL, NULL, 0)
#endif //#ifndef UNDER_CE

#define DEFINE_CSPROPERTY_ITEM_PIN_DATARANGES(Handler)\
    DEFINE_CSPROPERTY_ITEM(\
        CSPROPERTY_PIN_DATARANGES,\
        (Handler),\
        sizeof(CSP_PIN),\
        0,\
        NULL, NULL, 0, NULL, NULL, 0)

#define DEFINE_CSPROPERTY_ITEM_PIN_DATAINTERSECTION(Handler)\
    DEFINE_CSPROPERTY_ITEM(\
        CSPROPERTY_PIN_DATAINTERSECTION,\
        (Handler),\
        sizeof(CSP_PIN) + sizeof(CSMULTIPLE_ITEM),\
        0,\
        NULL, NULL, 0, NULL, NULL, 0)

#ifndef UNDER_CE
#define DEFINE_CSPROPERTY_ITEM_PIN_INTERFACES(Handler)\
    DEFINE_CSPROPERTY_ITEM(\
        CSPROPERTY_PIN_INTERFACES,\
        (Handler),\
        sizeof(CSP_PIN),\
        0,\
        NULL, NULL, 0, NULL, NULL, 0)

#define DEFINE_CSPROPERTY_ITEM_PIN_MEDIUMS(Handler)\
    DEFINE_CSPROPERTY_ITEM(\
        CSPROPERTY_PIN_MEDIUMS,\
        (Handler),\
        sizeof(CSP_PIN),\
        0,\
        NULL, NULL, 0, NULL, NULL, 0)

#define DEFINE_CSPROPERTY_ITEM_PIN_COMMUNICATION(Handler)\
    DEFINE_CSPROPERTY_ITEM(\
        CSPROPERTY_PIN_COMMUNICATION,\
        (Handler),\
        sizeof(CSP_PIN),\
        sizeof(CSPIN_COMMUNICATION),\
        NULL, NULL, 0, NULL, NULL, 0)

#define DEFINE_CSPROPERTY_ITEM_PIN_GLOBALCINSTANCES(Handler)\
    DEFINE_CSPROPERTY_ITEM(\
        CSPROPERTY_PIN_GLOBALCINSTANCES,\
        (Handler),\
        sizeof(CSP_PIN),\
        sizeof(CSPIN_CINSTANCES),\
        NULL, NULL, 0, NULL, NULL, 0)

#define DEFINE_CSPROPERTY_ITEM_PIN_NECESSARYINSTANCES(Handler)\
    DEFINE_CSPROPERTY_ITEM(\
        CSPROPERTY_PIN_NECESSARYINSTANCES,\
        (Handler),\
        sizeof(CSP_PIN),\
        sizeof(ULONG),\
        NULL, NULL, 0, NULL, NULL, 0)

#define DEFINE_CSPROPERTY_ITEM_PIN_PHYSICALCONNECTION(Handler)\
    DEFINE_CSPROPERTY_ITEM(\
        CSPROPERTY_PIN_PHYSICALCONNECTION,\
        (Handler),\
        sizeof(CSP_PIN),\
        0,\
        NULL, NULL, 0, NULL, NULL, 0)

#endif //#ifndef UNDER_CE

#define DEFINE_CSPROPERTY_ITEM_PIN_CATEGORY(Handler)\
    DEFINE_CSPROPERTY_ITEM(\
        CSPROPERTY_PIN_CATEGORY,\
        (Handler),\
        sizeof(CSP_PIN),\
        sizeof(GUID),\
        NULL, NULL, 0, NULL, NULL, 0)

#define DEFINE_CSPROPERTY_ITEM_PIN_NAME(Handler)\
    DEFINE_CSPROPERTY_ITEM(\
        CSPROPERTY_PIN_NAME,\
        (Handler),\
        sizeof(CSP_PIN),\
        0,\
        NULL, NULL, 0, NULL, NULL, 0)

#ifndef UNDER_CE
#define DEFINE_CSPROPERTY_ITEM_PIN_CONSTRAINEDDATARANGES(Handler)\
    DEFINE_CSPROPERTY_ITEM(\
        CSPROPERTY_PIN_CONSTRAINEDDATARANGES,\
        (Handler),\
        sizeof(CSP_PIN),\
        0,\
        NULL, NULL, 0, NULL, NULL, 0)

#define DEFINE_CSPROPERTY_ITEM_PIN_PROPOSEDATAFORMAT(Handler)\
    DEFINE_CSPROPERTY_ITEM(\
        CSPROPERTY_PIN_PROPOSEDATAFORMAT,\
        NULL,\
        sizeof(CSP_PIN),\
        sizeof(CSDATAFORMAT),\
        (Handler), NULL, 0, NULL, NULL, 0)

#endif //#ifndef UNDER_CE

#define DEFINE_CSPROPERTY_PINSET(PinSet,\
    PropGeneral, PropInstances, PropIntersection) \
DEFINE_CSPROPERTY_TABLE(PinSet) {\
    DEFINE_CSPROPERTY_ITEM_PIN_CINSTANCES(PropInstances),\
    DEFINE_CSPROPERTY_ITEM_PIN_CTYPES(PropGeneral),\
    DEFINE_CSPROPERTY_ITEM_PIN_DATARANGES(PropGeneral),\
    DEFINE_CSPROPERTY_ITEM_PIN_DATAINTERSECTION(PropIntersection),\
	DEFINE_CSPROPERTY_ITEM_PIN_CATEGORY(PropGeneral),\
    DEFINE_CSPROPERTY_ITEM_PIN_NAME(PropGeneral)\
}

#ifndef UNDER_CE
#define DEFINE_CSPROPERTY_PINSETCONSTRAINED(PinSet,\
    PropGeneral, PropInstances, PropIntersection)\
DEFINE_CSPROPERTY_TABLE(PinSet) {\
    DEFINE_CSPROPERTY_ITEM_PIN_CINSTANCES(PropInstances),\
    DEFINE_CSPROPERTY_ITEM_PIN_CTYPES(PropGeneral),\
    DEFINE_CSPROPERTY_ITEM_PIN_DATAFLOW(PropGeneral),\
    DEFINE_CSPROPERTY_ITEM_PIN_DATARANGES(PropGeneral),\
    DEFINE_CSPROPERTY_ITEM_PIN_DATAINTERSECTION(PropIntersection),\
    DEFINE_CSPROPERTY_ITEM_PIN_INTERFACES(PropGeneral),\
    DEFINE_CSPROPERTY_ITEM_PIN_MEDIUMS(PropGeneral),\
    DEFINE_CSPROPERTY_ITEM_PIN_COMMUNICATION(PropGeneral),\
    DEFINE_CSPROPERTY_ITEM_PIN_CATEGORY(PropGeneral),\
    DEFINE_CSPROPERTY_ITEM_PIN_NAME(PropGeneral),\
    DEFINE_CSPROPERTY_ITEM_PIN_CONSTRAINEDDATARANGES(PropGeneral)\
}

#define STATIC_CSNAME_Filter\
    0x9b365890L, 0x165f, 0x11d0, 0xa1, 0x95, 0x00, 0x20, 0xaf, 0xd1, 0x56, 0xe4
DEFINE_GUIDSTRUCT("9b365890-165f-11d0-a195-0020afd156e4", CSNAME_Filter);
#define CSNAME_Filter DEFINE_GUIDNAMED(CSNAME_Filter)

#define CSSTRING_Filter L"{9B365890-165F-11D0-A195-0020AFD156E4}"

#define STATIC_CSNAME_Pin\
    0x146F1A80L, 0x4791, 0x11D0, 0xA5, 0xD6, 0x28, 0xDB, 0x04, 0xC1, 0x00, 0x00
DEFINE_GUIDSTRUCT("146F1A80-4791-11D0-A5D6-28DB04C10000", CSNAME_Pin);
#define CSNAME_Pin DEFINE_GUIDNAMED(CSNAME_Pin)

#define CSSTRING_Pin L"{146F1A80-4791-11D0-A5D6-28DB04C10000}"

#define STATIC_CSNAME_Clock\
    0x53172480L, 0x4791, 0x11D0, 0xA5, 0xD6, 0x28, 0xDB, 0x04, 0xC1, 0x00, 0x00
DEFINE_GUIDSTRUCT("53172480-4791-11D0-A5D6-28DB04C10000", CSNAME_Clock);
#define CSNAME_Clock DEFINE_GUIDNAMED(CSNAME_Clock)

#define CSSTRING_Clock L"{53172480-4791-11D0-A5D6-28DB04C10000}"

#define STATIC_CSNAME_Allocator\
    0x642F5D00L, 0x4791, 0x11D0, 0xA5, 0xD6, 0x28, 0xDB, 0x04, 0xC1, 0x00, 0x00
DEFINE_GUIDSTRUCT("642F5D00-4791-11D0-A5D6-28DB04C10000", CSNAME_Allocator);
#define CSNAME_Allocator DEFINE_GUIDNAMED(CSNAME_Allocator)

#define CSSTRING_Allocator L"{642F5D00-4791-11D0-A5D6-28DB04C10000}"

#define CSSTRING_AllocatorEx L"{091BB63B-603F-11D1-B067-00A0C9062802}"

#define STATIC_CSNAME_TopologyNode\
    0x0621061AL, 0xEE75, 0x11D0, 0xB9, 0x15, 0x00, 0xA0, 0xC9, 0x22, 0x31, 0x96
DEFINE_GUIDSTRUCT("0621061A-EE75-11D0-B915-00A0C9223196", CSNAME_TopologyNode);
#define CSNAME_TopologyNode DEFINE_GUIDNAMED(CSNAME_TopologyNode)

#define CSSTRING_TopologyNode L"{0621061A-EE75-11D0-B915-00A0C9223196}"

#if defined(_NTDDK_)

typedef struct {
    ULONG                   InterfacesCount;
    const CSPIN_INTERFACE*  Interfaces;
    ULONG                   MediumsCount;
    const CSPIN_MEDIUM*     Mediums;
    ULONG                   DataRangesCount;
    const PCSDATARANGE*     DataRanges;
    CSPIN_DATAFLOW          DataFlow;
    CSPIN_COMMUNICATION     Communication;
    const GUID*             Category;
    const GUID*             Name;
    union {
        LONGLONG            Reserved;
        struct {
            ULONG           ConstrainedDataRangesCount;
            PCSDATARANGE*   ConstrainedDataRanges;
        };
    };
} CSPIN_DESCRIPTOR, *PCSPIN_DESCRIPTOR;
typedef const CSPIN_DESCRIPTOR *PCCSPIN_DESCRIPTOR;

#define DEFINE_CSPIN_DESCRIPTOR_TABLE(tablename)\
    const CSPIN_DESCRIPTOR tablename[] =

#define DEFINE_CSPIN_DESCRIPTOR_ITEM(\
    InterfacesCount, Interfaces,\
    MediumsCount, Mediums,\
    DataRangesCount, DataRanges,\
    DataFlow, Communication)\
{\
    InterfacesCount, Interfaces, MediumsCount, Mediums,\
    DataRangesCount, DataRanges, DataFlow, Communication,\
    NULL, NULL, 0\
}
#define DEFINE_CSPIN_DESCRIPTOR_ITEMEX(\
    InterfacesCount, Interfaces,\
    MediumsCount, Mediums,\
    DataRangesCount, DataRanges,\
    DataFlow, Communication,\
    Category, Name)\
{\
    InterfacesCount, Interfaces, MediumsCount, Mediums,\
    DataRangesCount, DataRanges, DataFlow, Communication,\
    Category, Name, 0\
}

#endif // defined(_NTDDK_)
#endif //#ifndef UNDER_CE
//===========================================================================

// MEDIATYPE_NULL
#define STATIC_CSDATAFORMAT_TYPE_WILDCARD       STATIC_GUID_NULL
#define CSDATAFORMAT_TYPE_WILDCARD              GUID_NULL

// MEDIASUBTYPE_NULL
#define STATIC_CSDATAFORMAT_SUBTYPE_WILDCARD    STATIC_GUID_NULL
#define CSDATAFORMAT_SUBTYPE_WILDCARD           GUID_NULL

// MEDIATYPE_Stream
#define STATIC_CSDATAFORMAT_TYPE_STREAM\
    0xE436EB83L, 0x524F, 0x11CE, 0x9F, 0x53, 0x00, 0x20, 0xAF, 0x0B, 0xA7, 0x70
DEFINE_GUIDSTRUCT("E436EB83-524F-11CE-9F53-0020AF0BA770", CSDATAFORMAT_TYPE_STREAM);
#define CSDATAFORMAT_TYPE_STREAM DEFINE_GUIDNAMED(CSDATAFORMAT_TYPE_STREAM)

// MEDIASUBTYPE_None
#define STATIC_CSDATAFORMAT_SUBTYPE_NONE\
    0xE436EB8EL, 0x524F, 0x11CE, 0x9F, 0x53, 0x00, 0x20, 0xAF, 0x0B, 0xA7, 0x70
DEFINE_GUIDSTRUCT("E436EB8E-524F-11CE-9F53-0020AF0BA770", CSDATAFORMAT_SUBTYPE_NONE);
#define CSDATAFORMAT_SUBTYPE_NONE DEFINE_GUIDNAMED(CSDATAFORMAT_SUBTYPE_NONE)

#define STATIC_CSDATAFORMAT_SPECIFIER_WILDCARD  STATIC_GUID_NULL
#define CSDATAFORMAT_SPECIFIER_WILDCARD         GUID_NULL

#define STATIC_CSDATAFORMAT_SPECIFIER_FILENAME\
    0xAA797B40L, 0xE974, 0x11CF, 0xA5, 0xD6, 0x28, 0xDB, 0x04, 0xC1, 0x00, 0x00
DEFINE_GUIDSTRUCT("AA797B40-E974-11CF-A5D6-28DB04C10000", CSDATAFORMAT_SPECIFIER_FILENAME);
#define CSDATAFORMAT_SPECIFIER_FILENAME DEFINE_GUIDNAMED(CSDATAFORMAT_SPECIFIER_FILENAME)

#define STATIC_CSDATAFORMAT_SPECIFIER_FILEHANDLE\
    0x65E8773CL, 0x8F56, 0x11D0, 0xA3, 0xB9, 0x00, 0xA0, 0xC9, 0x22, 0x31, 0x96
DEFINE_GUIDSTRUCT("65E8773C-8F56-11D0-A3B9-00A0C9223196", CSDATAFORMAT_SPECIFIER_FILEHANDLE);
#define CSDATAFORMAT_SPECIFIER_FILEHANDLE DEFINE_GUIDNAMED(CSDATAFORMAT_SPECIFIER_FILEHANDLE)

// FORMAT_None
#define STATIC_CSDATAFORMAT_SPECIFIER_NONE\
    0x0F6417D6L, 0xC318, 0x11D0, 0xA4, 0x3F, 0x00, 0xA0, 0xC9, 0x22, 0x31, 0x96
DEFINE_GUIDSTRUCT("0F6417D6-C318-11D0-A43F-00A0C9223196", CSDATAFORMAT_SPECIFIER_NONE);
#define CSDATAFORMAT_SPECIFIER_NONE DEFINE_GUIDNAMED(CSDATAFORMAT_SPECIFIER_NONE)

#ifndef UNDER_CE
//===========================================================================

#define STATIC_CSPROPSETID_Quality \
    0xD16AD380L, 0xAC1A, 0x11CF, 0xA5, 0xD6, 0x28, 0xDB, 0x04, 0xC1, 0x00, 0x00
DEFINE_GUIDSTRUCT("D16AD380-AC1A-11CF-A5D6-28DB04C10000", CSPROPSETID_Quality);
#define CSPROPSETID_Quality DEFINE_GUIDNAMED(CSPROPSETID_Quality)

typedef enum {
    CSPROPERTY_QUALITY_REPORT,
    CSPROPERTY_QUALITY_ERROR
} CSPROPERTY_QUALITY;

#define DEFINE_CSPROPERTY_ITEM_QUALITY_REPORT(GetHandler, SetHandler)\
    DEFINE_CSPROPERTY_ITEM(\
        CSPROPERTY_QUALITY_REPORT,\
        (GetHandler),\
        sizeof(CSPROPERTY),\
        sizeof(CSQUALITY),\
        (SetHandler),\
        NULL, 0, NULL, NULL, 0)

#define DEFINE_CSPROPERTY_ITEM_QUALITY_ERROR(GetHandler, SetHandler)\
    DEFINE_CSPROPERTY_ITEM(\
        CSPROPERTY_QUALITY_ERROR,\
        (GetHandler),\
        sizeof(CSPROPERTY),\
        sizeof(CSERROR),\
        (SetHandler),\
        NULL, 0, NULL, NULL, 0)

#endif //#ifndef UNDER_CE
//===========================================================================

#define STATIC_CSPROPSETID_Connection \
    0x1D58C920L, 0xAC9B, 0x11CF, 0xA5, 0xD6, 0x28, 0xDB, 0x04, 0xC1, 0x00, 0x00
DEFINE_GUIDSTRUCT("1D58C920-AC9B-11CF-A5D6-28DB04C10000", CSPROPSETID_Connection);
#define CSPROPSETID_Connection DEFINE_GUIDNAMED(CSPROPSETID_Connection)

typedef enum {
    CSPROPERTY_CONNECTION_STATE,
#ifndef UNDER_CE
	CSPROPERTY_CONNECTION_PRIORITY,
#endif //#ifndef UNDER_CE
    CSPROPERTY_CONNECTION_DATAFORMAT,
    CSPROPERTY_CONNECTION_ALLOCATORFRAMING,
    CSPROPERTY_CONNECTION_PROPOSEDATAFORMAT,
#ifndef UNDER_CE
	CSPROPERTY_CONNECTION_ACQUIREORDERING,
#endif //#ifndef UNDER_CE    
	CSPROPERTY_CONNECTION_ALLOCATORFRAMING_EX
#ifndef UNDER_CE
	,
	CSPROPERTY_CONNECTION_STARTAT
#endif //#ifndef UNDER_CE
} CSPROPERTY_CONNECTION;

#define DEFINE_CSPROPERTY_ITEM_CONNECTION_STATE(GetHandler, SetHandler)\
    DEFINE_CSPROPERTY_ITEM(\
        CSPROPERTY_CONNECTION_STATE,\
        (GetHandler),\
        sizeof(CSPROPERTY),\
        sizeof(CSSTATE),\
        (SetHandler),\
        NULL, 0, NULL, NULL, 0)

#ifndef UNDER_CE
#define DEFINE_CSPROPERTY_ITEM_CONNECTION_PRIORITY(GetHandler, SetHandler)\
    DEFINE_CSPROPERTY_ITEM(\
        CSPROPERTY_CONNECTION_PRIORITY,\
        (GetHandler),\
        sizeof(CSPROPERTY),\
        sizeof(CSPRIORITY),\
        (SetHandler),\
        NULL, 0, NULL, NULL, 0)
#endif //#ifndef UNDER_CE

#define DEFINE_CSPROPERTY_ITEM_CONNECTION_DATAFORMAT(GetHandler, SetHandler)\
    DEFINE_CSPROPERTY_ITEM(\
        CSPROPERTY_CONNECTION_DATAFORMAT,\
        (GetHandler),\
        sizeof(CSPROPERTY),\
        0,\
        (SetHandler),\
        NULL, 0, NULL, NULL, 0)

#define DEFINE_CSPROPERTY_ITEM_CONNECTION_ALLOCATORFRAMING(Handler)\
    DEFINE_CSPROPERTY_ITEM(\
        CSPROPERTY_CONNECTION_ALLOCATORFRAMING,\
        (Handler),\
        sizeof(CSPROPERTY),\
        sizeof(CSALLOCATOR_FRAMING),\
        NULL, NULL, 0, NULL, NULL, 0)
        
#define DEFINE_CSPROPERTY_ITEM_CONNECTION_ALLOCATORFRAMING_EX(Handler)\
    DEFINE_CSPROPERTY_ITEM(\
        CSPROPERTY_CONNECTION_ALLOCATORFRAMING_EX,\
        (Handler),\
        sizeof(CSPROPERTY),\
        0,\
        NULL, NULL, 0, NULL, NULL, 0)

#ifndef UNDER_CE
#define DEFINE_CSPROPERTY_ITEM_CONNECTION_PROPOSEDATAFORMAT(Handler)\
    DEFINE_CSPROPERTY_ITEM(\
        CSPROPERTY_CONNECTION_PROPOSEDATAFORMAT,\
        NULL,\
        sizeof(CSPROPERTY),\
        sizeof(CSDATAFORMAT),\
        (Handler),\
        NULL, 0, NULL, NULL, 0)

#define DEFINE_CSPROPERTY_ITEM_CONNECTION_ACQUIREORDERING(Handler)\
    DEFINE_CSPROPERTY_ITEM(\
        CSPROPERTY_CONNECTION_ACQUIREORDERING,\
        (Handler),\
        sizeof(CSPROPERTY),\
        sizeof(int),\
        NULL, NULL, 0, NULL, NULL, 0)

#define DEFINE_CSPROPERTY_ITEM_CONNECTION_STARTAT(Handler)\
    DEFINE_CSPROPERTY_ITEM(\
        CSPROPERTY_CONNECTION_STARTAT,\
        NULL,\
        sizeof(CSPROPERTY),\
        sizeof(CSRELATIVEEVENT),\
        (Handler),\
        NULL, 0, NULL, NULL, 0)
#endif //#ifndef UNDER_CE

//===========================================================================
//
// pins flags
//
#define CSALLOCATOR_REQUIREMENTF_INPLACE_MODIFIER   0x00000001
#define CSALLOCATOR_REQUIREMENTF_SYSTEM_MEMORY      0x00000002
#define CSALLOCATOR_REQUIREMENTF_FRAME_INTEGRITY    0x00000004
#define CSALLOCATOR_REQUIREMENTF_MUST_ALLOCATE      0x00000008
#define CSALLOCATOR_REQUIREMENTF_PREFERENCES_ONLY   0x80000000

#define CSALLOCATOR_OPTIONF_COMPATIBLE              0x00000001
#define CSALLOCATOR_OPTIONF_SYSTEM_MEMORY           0x00000002
#define CSALLOCATOR_OPTIONF_VALID                   0x00000003
// 
// pins extended framing flags
//
#define CSALLOCATOR_FLAG_PARTIAL_READ_SUPPORT       0x00000010
#define CSALLOCATOR_FLAG_DEVICE_SPECIFIC            0x00000020
#define CSALLOCATOR_FLAG_CAN_ALLOCATE               0x00000040
#define CSALLOCATOR_FLAG_INSIST_ON_FRAMESIZE_RATIO  0x00000080
//
// allocator pipes flags
//
// there is at least one data modification in a pipe
#define CSALLOCATOR_FLAG_NO_FRAME_INTEGRITY         0x00000100
#define CSALLOCATOR_FLAG_MULTIPLE_OUTPUT            0x00000200
#define CSALLOCATOR_FLAG_CYCLE                      0x00000400
#define CSALLOCATOR_FLAG_ALLOCATOR_EXISTS           0x00000800
// there is no framing dependency between neighbouring pipes.
#define CSALLOCATOR_FLAG_INDEPENDENT_RANGES         0x00001000
#define CSALLOCATOR_FLAG_ATTENTION_STEPPING         0x00002000


//
// old Framing structure
//
typedef struct {
    union {
        ULONG       OptionsFlags;       // allocator options (create)
        ULONG       RequirementsFlags;  // allocation requirements (query)
    };
#if defined(_NTDDK_)
    POOL_TYPE   PoolType;
#else // !_NTDDK_
    ULONG       PoolType;
#endif // !_NTDDK_
    ULONG       Frames;     // total number of allowable outstanding frames
    ULONG       FrameSize;  // total size of frame
    ULONG       FileAlignment;
    ULONG       Reserved;
} CSALLOCATOR_FRAMING, *PCSALLOCATOR_FRAMING;

#ifndef UNDER_CE
#if defined(_NTDDK_)
typedef
PVOID
(*PFNCSDEFAULTALLOCATE)(
    IN PVOID Context
    );

typedef
VOID
(*PFNCSDEFAULTFREE)(
    IN PVOID Context,
    IN PVOID Buffer
    );

typedef
NTSTATUS
(*PFNCSINITIALIZEALLOCATOR)(
    IN PVOID InitialContext,
    IN PCSALLOCATOR_FRAMING AllocatorFraming,
    OUT PVOID* Context
    );

typedef
VOID
(*PFNCSDELETEALLOCATOR)(
    IN PVOID Context
    );
#endif // !_NTDDK_

#endif //#ifndef UNDER_CE

//
// new Framing structure, eventually will replace CSALLOCATOR_FRAMING.
// 
typedef struct {
    ULONG   MinFrameSize;
    ULONG   MaxFrameSize;
    ULONG   Stepping;
} CS_FRAMING_RANGE, *PCS_FRAMING_RANGE;


typedef struct {
    CS_FRAMING_RANGE  Range;
    ULONG             InPlaceWeight;
    ULONG             NotInPlaceWeight;
} CS_FRAMING_RANGE_WEIGHTED, *PCS_FRAMING_RANGE_WEIGHTED;


typedef struct {
    ULONG   RatioNumerator;      // compression/expansion ratio
    ULONG   RatioDenominator; 
    ULONG   RatioConstantMargin;
} CS_COMPRESSION, *PCS_COMPRESSION;


//
// Memory Types and Buses are repeated in each entry.
// Easiest to use but takes a little more memory than the varsize layout Pin\Memories\Buses\Ranges.
//
typedef struct {
    GUID                        MemoryType;
    GUID                        BusType;
    ULONG                       MemoryFlags;
    ULONG                       BusFlags;   
    ULONG                       Flags;   
    ULONG                       Frames;              // total number of allowable outstanding frames
    ULONG                       FileAlignment;
    ULONG                       MemoryTypeWeight;    // this memory type Weight pin-wide
    CS_FRAMING_RANGE            PhysicalRange;
    CS_FRAMING_RANGE_WEIGHTED   FramingRange; 
} CS_FRAMING_ITEM, *PCS_FRAMING_ITEM;


typedef struct {
    ULONG               CountItems;         // count of FramingItem-s below.
    ULONG               PinFlags;
    CS_COMPRESSION      OutputCompression;
    ULONG               PinWeight;          // this pin framing's Weight graph-wide
    CS_FRAMING_ITEM     FramingItem[1]; 
} CSALLOCATOR_FRAMING_EX, *PCSALLOCATOR_FRAMING_EX;


#ifndef UNDER_CE
//
// define memory type GUIDs
//
#define CSMEMORY_TYPE_WILDCARD          GUID_NULL
#define STATIC_CSMEMORY_TYPE_WILDCARD   STATIC_GUID_NULL

#define CSMEMORY_TYPE_DONT_CARE         GUID_NULL
#define STATIC_CSMEMORY_TYPE_DONT_CARE  STATIC_GUID_NULL

#define CS_TYPE_DONT_CARE           GUID_NULL
#define STATIC_CS_TYPE_DONT_CARE    STATIC_GUID_NULL
     
#define STATIC_CSMEMORY_TYPE_SYSTEM \
    0x091bb638L, 0x603f, 0x11d1, 0xb0, 0x67, 0x00, 0xa0, 0xc9, 0x06, 0x28, 0x02
DEFINE_GUIDSTRUCT("091bb638-603f-11d1-b067-00a0c9062802", CSMEMORY_TYPE_SYSTEM);
#define CSMEMORY_TYPE_SYSTEM  DEFINE_GUIDNAMED(CSMEMORY_TYPE_SYSTEM)

#define STATIC_CSMEMORY_TYPE_USER \
    0x8cb0fc28L, 0x7893, 0x11d1, 0xb0, 0x69, 0x00, 0xa0, 0xc9, 0x06, 0x28, 0x02
DEFINE_GUIDSTRUCT("8cb0fc28-7893-11d1-b069-00a0c9062802", CSMEMORY_TYPE_USER);
#define CSMEMORY_TYPE_USER  DEFINE_GUIDNAMED(CSMEMORY_TYPE_USER)

#define STATIC_CSMEMORY_TYPE_KERNEL_PAGED \
    0xd833f8f8L, 0x7894, 0x11d1, 0xb0, 0x69, 0x00, 0xa0, 0xc9, 0x06, 0x28, 0x02
DEFINE_GUIDSTRUCT("d833f8f8-7894-11d1-b069-00a0c9062802", CSMEMORY_TYPE_KERNEL_PAGED);
#define CSMEMORY_TYPE_KERNEL_PAGED  DEFINE_GUIDNAMED(CSMEMORY_TYPE_KERNEL_PAGED)

#define STATIC_CSMEMORY_TYPE_KERNEL_NONPAGED \
    0x4a6d5fc4L, 0x7895, 0x11d1, 0xb0, 0x69, 0x00, 0xa0, 0xc9, 0x06, 0x28, 0x02
DEFINE_GUIDSTRUCT("4a6d5fc4-7895-11d1-b069-00a0c9062802", CSMEMORY_TYPE_KERNEL_NONPAGED);
#define CSMEMORY_TYPE_KERNEL_NONPAGED  DEFINE_GUIDNAMED(CSMEMORY_TYPE_KERNEL_NONPAGED)

// old CS clients did not specify the device memory type
#define STATIC_CSMEMORY_TYPE_DEVICE_UNKNOWN \
    0x091bb639L, 0x603f, 0x11d1, 0xb0, 0x67, 0x00, 0xa0, 0xc9, 0x06, 0x28, 0x02
DEFINE_GUIDSTRUCT("091bb639-603f-11d1-b067-00a0c9062802", CSMEMORY_TYPE_DEVICE_UNKNOWN);
#define CSMEMORY_TYPE_DEVICE_UNKNOWN DEFINE_GUIDNAMED(CSMEMORY_TYPE_DEVICE_UNKNOWN)

#endif //#ifndef UNDER_CE

//
// Helper framing macros.
//
#define DECLARE_SIMPLE_FRAMING_EX(FramingExName, MemoryType, Flags, Frames, Alignment, MinFrameSize, MaxFrameSize) \
    const CSALLOCATOR_FRAMING_EX FramingExName = \
    {\
        1, \
        0, \
        {\
            1, \
            1, \
            0 \
        }, \
        0, \
        {\
            {\
                MemoryType, \
                STATIC_CS_TYPE_DONT_CARE, \
                0, \
                0, \
                Flags, \
                Frames, \
                Alignment, \
                0, \
                {\
                    0, \
                    (ULONG)-1, \
                    1 \
                }, \
                {\
                    {\
                        MinFrameSize, \
                        MaxFrameSize, \
                        1 \
                    }, \
                    0, \
                    0  \
                }\
            }\
        }\
    }

#define SetDefaultCsCompression(CsCompressionPointer) \
{\
    CsCompressionPointer->RatioNumerator = 1;\
    CsCompressionPointer->RatioDenominator = 1;\
    CsCompressionPointer->RatioConstantMargin = 0;\
}

#define SetDontCareCsFramingRange(CsFramingRangePointer) \
{\
    CsFramingRangePointer->MinFrameSize = 0;\
    CsFramingRangePointer->MaxFrameSize = (ULONG) -1;\
    CsFramingRangePointer->Stepping = 1;\
}

#define SetCsFramingRange(CsFramingRangePointer, P_MinFrameSize, P_MaxFrameSize) \
{\
    CsFramingRangePointer->MinFrameSize = P_MinFrameSize;\
    CsFramingRangePointer->MaxFrameSize = P_MaxFrameSize;\
    CsFramingRangePointer->Stepping = 1;\
}

#define SetCsFramingRangeWeighted(CsFramingRangeWeightedPointer, P_MinFrameSize, P_MaxFrameSize) \
{\
    CS_FRAMING_RANGE *CsFramingRange = &CsFramingRangeWeightedPointer->Range;\
    SetCsFramingRange(CsFramingRange, P_MinFrameSize, P_MaxFrameSize);\
    CsFramingRangeWeightedPointer->InPlaceWeight = 0;\
    CsFramingRangeWeightedPointer->NotInPlaceWeight = 0;\
}

#define INITIALIZE_SIMPLE_FRAMING_EX(FramingExPointer, P_MemoryType, P_Flags, P_Frames, P_Alignment, P_MinFrameSize, P_MaxFrameSize) \
{\
    CS_COMPRESSION *CsCompression = &FramingExPointer->OutputCompression;\
    CS_FRAMING_RANGE *CsFramingRange = &FramingExPointer->FramingItem[0].PhysicalRange;\
    CS_FRAMING_RANGE_WEIGHTED *CsFramingRangeWeighted = &FramingExPointer->FramingItem[0].FramingRange;\
    FramingExPointer->CountItems = 1;\
    FramingExPointer->PinFlags = 0;\
    SetDefaultCsCompression(CsCompression);\
    FramingExPointer->PinWeight = 0;\
    FramingExPointer->FramingItem[0].MemoryType = P_MemoryType;\
    FramingExPointer->FramingItem[0].BusType = CS_TYPE_DONT_CARE;\
    FramingExPointer->FramingItem[0].MemoryFlags = 0;\
    FramingExPointer->FramingItem[0].BusFlags = 0;\
    FramingExPointer->FramingItem[0].Flags = P_Flags;\
    FramingExPointer->FramingItem[0].Frames = P_Frames;\
    FramingExPointer->FramingItem[0].FileAlignment = P_Alignment;\
    FramingExPointer->FramingItem[0].MemoryTypeWeight = 0;\
    SetDontCareCsFramingRange(CsFramingRange);\
    SetCsFramingRangeWeighted(CsFramingRangeWeighted, P_MinFrameSize, P_MaxFrameSize);\
}


#ifndef UNDER_CE
// CSEVENTSETID_StreamAllocator: {75D95571-073C-11d0-A161-0020AFD156E4}

#define STATIC_CSEVENTSETID_StreamAllocator\
    0x75d95571L, 0x073c, 0x11d0, 0xa1, 0x61, 0x00, 0x20, 0xaf, 0xd1, 0x56, 0xe4
DEFINE_GUIDSTRUCT("75d95571-073c-11d0-a161-0020afd156e4", CSEVENTSETID_StreamAllocator);
#define CSEVENTSETID_StreamAllocator DEFINE_GUIDNAMED(CSEVENTSETID_StreamAllocator)

typedef enum {
    CSEVENT_STREAMALLOCATOR_INTERNAL_FREEFRAME,
    CSEVENT_STREAMALLOCATOR_FREEFRAME
} CSEVENT_STREAMALLOCATOR;

#define STATIC_CSMETHODSETID_StreamAllocator\
    0xcf6e4341L, 0xec87, 0x11cf, 0xa1, 0x30, 0x00, 0x20, 0xaf, 0xd1, 0x56, 0xe4
DEFINE_GUIDSTRUCT("cf6e4341-ec87-11cf-a130-0020afd156e4", CSMETHODSETID_StreamAllocator);
#define CSMETHODSETID_StreamAllocator DEFINE_GUIDNAMED(CSMETHODSETID_StreamAllocator)

typedef enum {
    CSMETHOD_STREAMALLOCATOR_ALLOC,
    CSMETHOD_STREAMALLOCATOR_FREE
} CSMETHOD_STREAMALLOCATOR;

#define DEFINE_CSMETHOD_ITEM_STREAMALLOCATOR_ALLOC(Handler)\
    DEFINE_CSMETHOD_ITEM(\
        CSMETHOD_STREAMALLOCATOR_ALLOC,\
        CSMETHOD_TYPE_WRITE,\
        (Handler),\
        sizeof(CSMETHOD),\
        sizeof(PVOID),\
        NULL)

#define DEFINE_CSMETHOD_ITEM_STREAMALLOCATOR_FREE(Handler)\
    DEFINE_CSMETHOD_ITEM(\
        CSMETHOD_STREAMALLOCATOR_FREE,\
        CSMETHOD_TYPE_READ,\
        (Handler),\
        sizeof(CSMETHOD),\
        sizeof(PVOID),\
        NULL)

#define DEFINE_CSMETHOD_ALLOCATORSET(AllocatorSet, MethodAlloc, MethodFree)\
DEFINE_CSMETHOD_TABLE(AllocatorSet) {\
    DEFINE_CSMETHOD_ITEM_STREAMALLOCATOR_ALLOC(MethodAlloc),\
    DEFINE_CSMETHOD_ITEM_STREAMALLOCATOR_FREE(MethodFree)\
}

#define STATIC_CSPROPSETID_StreamAllocator\
    0xcf6e4342L, 0xec87, 0x11cf, 0xa1, 0x30, 0x00, 0x20, 0xaf, 0xd1, 0x56, 0xe4
DEFINE_GUIDSTRUCT("cf6e4342-ec87-11cf-a130-0020afd156e4", CSPROPSETID_StreamAllocator);
#define CSPROPSETID_StreamAllocator DEFINE_GUIDNAMED(CSPROPSETID_StreamAllocator)

#if defined(_NTDDK_)
typedef enum {
    CSPROPERTY_STREAMALLOCATOR_FUNCTIONTABLE,
    CSPROPERTY_STREAMALLOCATOR_STATUS
} CSPROPERTY_STREAMALLOCATOR;

#define DEFINE_CSPROPERTY_ITEM_STREAMALLOCATOR_FUNCTIONTABLE(Handler)\
    DEFINE_CSPROPERTY_ITEM(\
        CSPROPERTY_STREAMALLOCATOR_FUNCTIONTABLE,\
        (Handler),\
        sizeof(CSPROPERTY),\
        sizeof(CSSTREAMALLOCATOR_FUNCTIONTABLE),\
        NULL, NULL, 0, NULL, NULL, 0)
        
#define DEFINE_CSPROPERTY_ITEM_STREAMALLOCATOR_STATUS(Handler)\
    DEFINE_CSPROPERTY_ITEM(\
        CSPROPERTY_STREAMALLOCATOR_STATUS,\
        (Handler),\
        sizeof(CSPROPERTY),\
        sizeof(CSSTREAMALLOCATOR_STATUS),\
        NULL, NULL, 0, NULL, NULL, 0)

#define DEFINE_CSPROPERTY_ALLOCATORSET(AllocatorSet, PropFunctionTable, PropStatus)\
DEFINE_CSPROPERTY_TABLE(AllocatorSet) {\
    DEFINE_CSPROPERTY_ITEM_STREAMALLOCATOR_STATUS(PropStatus),\
    DEFINE_CSPROPERTY_ITEM_STREAMALLOCATOR_FUNCTIONTABLE(PropFunctionTable)\
}

typedef
NTSTATUS
(*PFNALLOCATOR_ALLOCATEFRAME)(
    IN PFILE_OBJECT FileObject,
    PVOID *Frame
    );

typedef
VOID
(*PFNALLOCATOR_FREEFRAME)(
    IN PFILE_OBJECT FileObject,
    IN PVOID Frame
    );

typedef struct {
    PFNALLOCATOR_ALLOCATEFRAME  AllocateFrame;
    PFNALLOCATOR_FREEFRAME      FreeFrame;
} CSSTREAMALLOCATOR_FUNCTIONTABLE, *PCSSTREAMALLOCATOR_FUNCTIONTABLE;
#endif // defined(_NTDDK_)

typedef struct {
    CSALLOCATOR_FRAMING Framing;
    ULONG               AllocatedFrames;
    ULONG               Reserved;
} CSSTREAMALLOCATOR_STATUS, *PCSSTREAMALLOCATOR_STATUS;

typedef struct {
    CSALLOCATOR_FRAMING_EX Framing;
    ULONG                  AllocatedFrames;
    ULONG                  Reserved;
} CSSTREAMALLOCATOR_STATUS_EX, *PCSSTREAMALLOCATOR_STATUS_EX;

#endif //#ifndef UNDER_CE

#define CSSTREAM_HEADER_OPTIONSF_SPLICEPOINT        0x00000001
#define CSSTREAM_HEADER_OPTIONSF_PREROLL            0x00000002
#define CSSTREAM_HEADER_OPTIONSF_DATADISCONTINUITY  0x00000004
#define CSSTREAM_HEADER_OPTIONSF_TYPECHANGED        0x00000008
#define CSSTREAM_HEADER_OPTIONSF_TIMEVALID          0x00000010
#define CSSTREAM_HEADER_OPTIONSF_TIMEDISCONTINUITY  0x00000040
#define CSSTREAM_HEADER_OPTIONSF_FLUSHONPAUSE       0x00000080
#define CSSTREAM_HEADER_OPTIONSF_DURATIONVALID      0x00000100
#define CSSTREAM_HEADER_OPTIONSF_ENDOFSTREAM        0x00000200
#define CSSTREAM_HEADER_OPTIONSF_LOOPEDDATA         0x80000000

typedef struct {
    LONGLONG    Time;
    ULONG       Numerator;
    ULONG       Denominator;
} CSTIME, *PCSTIME;


typedef struct {
    ULONG       Size;
    ULONG       TypeSpecificFlags;
    CSTIME      PresentationTime;
    LONGLONG    Duration;
    ULONG       FrameExtent;
    ULONG       DataUsed;
    PVOID       Data;
    ULONG       OptionsFlags;
    DWORD       Handle;
#if _WIN64
    ULONG       Reserved;
#endif
} CSSTREAM_HEADER, *PCSSTREAM_HEADER;

#ifndef UNDER_CE
#define STATIC_CSPROPSETID_StreamInterface\
    0x1fdd8ee1L, 0x9cd3, 0x11d0, 0x82, 0xaa, 0x00, 0x00, 0xf8, 0x22, 0xfe, 0x8a
DEFINE_GUIDSTRUCT("1fdd8ee1-9cd3-11d0-82aa-0000f822fe8a", CSPROPSETID_StreamInterface);
#define CSPROPSETID_StreamInterface DEFINE_GUIDNAMED(CSPROPSETID_StreamInterface)

typedef enum {
    CSPROPERTY_STREAMINTERFACE_HEADERSIZE
} CSPROPERTY_STREAMINTERFACE;

#define DEFINE_CSPROPERTY_ITEM_STREAMINTERFACE_HEADERSIZE( GetHandler )\
    DEFINE_CSPROPERTY_ITEM(\
        CSPROPERTY_STREAMINTERFACE_HEADERSIZE,\
        (GetHandler),\
        sizeof(CSPROPERTY),\
        sizeof(ULONG),\
        NULL, NULL, 0, NULL, NULL, 0)
        
#define DEFINE_CSPROPERTY_STREAMINTERFACESET(StreamInterfaceSet,\
    HeaderSizeHandler)\
DEFINE_CSPROPERTY_TABLE(StreamInterfaceSet) {\
    DEFINE_CSPROPERTY_ITEM_STREAMINTERFACE_HEADERSIZE( HeaderSizeHandler )\
}
#endif //#ifndef UNDER_CE

#define STATIC_CSPROPSETID_Stream\
    0x65aaba60L, 0x98ae, 0x11cf, 0xa1, 0x0d, 0x00, 0x20, 0xaf, 0xd1, 0x56, 0xe4
DEFINE_GUIDSTRUCT("65aaba60-98ae-11cf-a10d-0020afd156e4", CSPROPSETID_Stream);
#define CSPROPSETID_Stream DEFINE_GUIDNAMED(CSPROPSETID_Stream)

typedef enum {
    CSPROPERTY_STREAM_ALLOCATOR
#ifndef UNDER_CE
	,
	CSPROPERTY_STREAM_QUALITY,
    CSPROPERTY_STREAM_DEGRADATION,
    CSPROPERTY_STREAM_MASTERCLOCK,
    CSPROPERTY_STREAM_TIMEFORMAT,
    CSPROPERTY_STREAM_PRESENTATIONTIME,
    CSPROPERTY_STREAM_PRESENTATIONEXTENT,
    CSPROPERTY_STREAM_FRAMETIME,
    CSPROPERTY_STREAM_RATECAPABILITY,
    CSPROPERTY_STREAM_RATE,
    CSPROPERTY_STREAM_PIPE_ID
#endif //#ifndef UNDER_CE
} CSPROPERTY_STREAM;


#define DEFINE_CSPROPERTY_ITEM_STREAM_ALLOCATOR(GetHandler, SetHandler)\
    DEFINE_CSPROPERTY_ITEM(\
        CSPROPERTY_STREAM_ALLOCATOR,\
        (GetHandler),\
        sizeof(CSPROPERTY),\
        sizeof(HANDLE),\
        (SetHandler),\
        NULL, 0, NULL, NULL, 0)

#ifndef UNDER_CE
#define DEFINE_CSPROPERTY_ITEM_STREAM_QUALITY(Handler)\
    DEFINE_CSPROPERTY_ITEM(\
        CSPROPERTY_STREAM_QUALITY,\
        (Handler),\
        sizeof(CSPROPERTY),\
        sizeof(CSQUALITY_MANAGER),\
        NULL, NULL, 0, NULL, NULL, 0)

#define DEFINE_CSPROPERTY_ITEM_STREAM_DEGRADATION(GetHandler, SetHandler)\
    DEFINE_CSPROPERTY_ITEM(\
        CSPROPERTY_STREAM_DEGRADATION,\
        (GetHandler),\
        sizeof(CSPROPERTY),\
        0,\
        (SetHandler),\
        NULL, 0, NULL, NULL, 0)

#define DEFINE_CSPROPERTY_ITEM_STREAM_MASTERCLOCK(GetHandler, SetHandler)\
    DEFINE_CSPROPERTY_ITEM(\
        CSPROPERTY_STREAM_MASTERCLOCK,\
        (GetHandler),\
        sizeof(CSPROPERTY),\
        sizeof(HANDLE),\
        (SetHandler),\
        NULL, 0, NULL, NULL, 0)

#define DEFINE_CSPROPERTY_ITEM_STREAM_TIMEFORMAT(Handler)\
    DEFINE_CSPROPERTY_ITEM(\
        CSPROPERTY_STREAM_TIMEFORMAT,\
        (Handler),\
        sizeof(CSPROPERTY),\
        sizeof(GUID),\
        NULL, NULL, 0, NULL, NULL, 0)

#define DEFINE_CSPROPERTY_ITEM_STREAM_PRESENTATIONTIME(GetHandler, SetHandler)\
    DEFINE_CSPROPERTY_ITEM(\
        CSPROPERTY_STREAM_PRESENTATIONTIME,\
        (GetHandler),\
        sizeof(CSPROPERTY),\
        sizeof(CSTIME),\
        (SetHandler),\
        NULL, 0, NULL, NULL, 0)

#define DEFINE_CSPROPERTY_ITEM_STREAM_PRESENTATIONEXTENT(Handler)\
    DEFINE_CSPROPERTY_ITEM(\
        CSPROPERTY_STREAM_PRESENTATIONEXTENT,\
        (Handler),\
        sizeof(CSPROPERTY),\
        sizeof(LONGLONG),\
        NULL, NULL, 0, NULL, NULL, 0)

#define DEFINE_CSPROPERTY_ITEM_STREAM_FRAMETIME(Handler)\
    DEFINE_CSPROPERTY_ITEM(\
        CSPROPERTY_STREAM_FRAMETIME,\
        (Handler),\
        sizeof(CSPROPERTY),\
        sizeof(CSFRAMETIME),\
        NULL, NULL, 0, NULL, NULL, 0)

#define DEFINE_CSPROPERTY_ITEM_STREAM_RATECAPABILITY(Handler)\
    DEFINE_CSPROPERTY_ITEM(\
        CSPROPERTY_STREAM_RATECAPABILITY,\
        (Handler),\
        sizeof(CSRATE_CAPABILITY),\
        sizeof(CSRATE),\
        NULL, NULL, 0, NULL, NULL, 0)

#define DEFINE_CSPROPERTY_ITEM_STREAM_RATE(GetHandler, SetHandler)\
    DEFINE_CSPROPERTY_ITEM(\
        CSPROPERTY_STREAM_RATE,\
        (GetHandler),\
        sizeof(CSPROPERTY),\
        sizeof(CSRATE),\
        (SetHandler),\
        NULL, 0, NULL, NULL, 0)

#define DEFINE_CSPROPERTY_ITEM_STREAM_PIPE_ID(GetHandler, SetHandler)\
    DEFINE_CSPROPERTY_ITEM(\
        CSPROPERTY_STREAM_PIPE_ID,\
        (GetHandler),\
        sizeof(CSPROPERTY),\
        sizeof(HANDLE),\
        (SetHandler),\
        NULL, 0, NULL, NULL, 0)

typedef struct {
    HANDLE      QualityManager;
    PVOID       Context;
} CSQUALITY_MANAGER, *PCSQUALITY_MANAGER;

typedef struct {
    LONGLONG    Duration;
    ULONG       FrameFlags;
    ULONG       Reserved;
} CSFRAMETIME, *PCSFRAMETIME;

#define CSFRAMETIME_VARIABLESIZE    0x00000001

typedef struct {
    LONGLONG        PresentationStart;
    LONGLONG        Duration;
    CSPIN_INTERFACE Interface;
    LONG            Rate;
    ULONG           Flags;
} CSRATE, *PCSRATE;

#define CSRATE_NOPRESENTATIONSTART      0x00000001
#define CSRATE_NOPRESENTATIONDURATION   0x00000002

typedef struct {
    CSPROPERTY      Property;
    CSRATE          Rate;
} CSRATE_CAPABILITY, *PCSRATE_CAPABILITY;

#define STATIC_CSPROPSETID_Clock \
    0xDF12A4C0L, 0xAC17, 0x11CF, 0xA5, 0xD6, 0x28, 0xDB, 0x04, 0xC1, 0x00, 0x00
DEFINE_GUIDSTRUCT("DF12A4C0-AC17-11CF-A5D6-28DB04C10000", CSPROPSETID_Clock);
#define CSPROPSETID_Clock DEFINE_GUIDNAMED(CSPROPSETID_Clock)

//
// Performs a x*y/z operation on 64 bit quantities by splitting the operation. The equation
// is simplified with respect to adding in the remainder for the upper 32 bits.
//
// (xh * 10000000 / Frequency) * 2^32 + ((((xh * 10000000) % Frequency) * 2^32 + (xl * 10000000)) / Frequency)
//
#define NANOSECONDS 10000000
#define CSCONVERT_PERFORMANCE_TIME(Frequency, PerformanceTime) \
    ((((ULONGLONG)(ULONG)(PerformanceTime).HighPart * NANOSECONDS / (Frequency)) << 32) + \
    ((((((ULONGLONG)(ULONG)(PerformanceTime).HighPart * NANOSECONDS) % (Frequency)) << 32) + \
    ((ULONGLONG)(PerformanceTime).LowPart * NANOSECONDS)) / (Frequency)))

typedef struct {
    ULONG       CreateFlags;
} CSCLOCK_CREATE, *PCSCLOCK_CREATE;

typedef struct {
    LONGLONG    Time;
    LONGLONG    SystemTime;
} CSCORRELATED_TIME, *PCSCORRELATED_TIME;

typedef struct {
    LONGLONG    Granularity;
    LONGLONG    Error;
} CSRESOLUTION, *PCSRESOLUTION;

typedef enum {
    CSPROPERTY_CLOCK_TIME,
    CSPROPERTY_CLOCK_PHYSICALTIME,
    CSPROPERTY_CLOCK_CORRELATEDTIME,
    CSPROPERTY_CLOCK_CORRELATEDPHYSICALTIME,
    CSPROPERTY_CLOCK_RESOLUTION,
    CSPROPERTY_CLOCK_STATE,
#if defined(_NTDDK_)
    CSPROPERTY_CLOCK_FUNCTIONTABLE
#endif // defined(_NTDDK_)
} CSPROPERTY_CLOCK;

#if defined(_NTDDK_)

typedef
LONGLONG
(FASTCALL *PFNCSCLOCK_GETTIME)(
    IN PFILE_OBJECT FileObject
    );
typedef
LONGLONG
(FASTCALL *PFNCSCLOCK_CORRELATEDTIME)(
    IN PFILE_OBJECT FileObject,
    OUT PLONGLONG SystemTime);

typedef struct {
    PFNCSCLOCK_GETTIME GetTime;
    PFNCSCLOCK_GETTIME GetPhysicalTime;
    PFNCSCLOCK_CORRELATEDTIME GetCorrelatedTime;
    PFNCSCLOCK_CORRELATEDTIME GetCorrelatedPhysicalTime;
} CSCLOCK_FUNCTIONTABLE, *PCSCLOCK_FUNCTIONTABLE;

typedef
BOOLEAN
(*PFNCSSETTIMER)(
    IN PVOID Context,
    IN PKTIMER Timer,
    IN LARGE_INTEGER DueTime,
    IN PKDPC Dpc
    );

typedef
BOOLEAN
(*PFNCSCANCELTIMER)(
    IN PVOID Context,
    IN PKTIMER Timer
    );

typedef
LONGLONG
(FASTCALL *PFNCSCORRELATEDTIME)(
    IN PVOID Context,
    OUT PLONGLONG SystemTime);

typedef PVOID   PCSDEFAULTCLOCK;

#define DEFINE_CSPROPERTY_ITEM_CLOCK_TIME(Handler)\
    DEFINE_CSPROPERTY_ITEM(\
        CSPROPERTY_CLOCK_TIME,\
        (Handler),\
        sizeof(CSPROPERTY),\
        sizeof(LONGLONG),\
        NULL, NULL, 0, NULL, NULL, 0)

#define DEFINE_CSPROPERTY_ITEM_CLOCK_PHYSICALTIME(Handler)\
    DEFINE_CSPROPERTY_ITEM(\
        CSPROPERTY_CLOCK_PHYSICALTIME,\
        (Handler),\
        sizeof(CSPROPERTY),\
        sizeof(LONGLONG),\
        NULL, NULL, 0, NULL, NULL, 0)

#define DEFINE_CSPROPERTY_ITEM_CLOCK_CORRELATEDTIME(Handler)\
    DEFINE_CSPROPERTY_ITEM(\
        CSPROPERTY_CLOCK_CORRELATEDTIME,\
        (Handler),\
        sizeof(CSPROPERTY),\
        sizeof(CSCORRELATED_TIME),\
        NULL, NULL, 0, NULL, NULL, 0)

#define DEFINE_CSPROPERTY_ITEM_CLOCK_CORRELATEDPHYSICALTIME(Handler)\
    DEFINE_CSPROPERTY_ITEM(\
        CSPROPERTY_CLOCK_CORRELATEDPHYSICALTIME,\
        (Handler),\
        sizeof(CSPROPERTY),\
        sizeof(CSCORRELATED_TIME),\
        NULL, NULL, 0, NULL, NULL, 0)

#define DEFINE_CSPROPERTY_ITEM_CLOCK_RESOLUTION(Handler)\
    DEFINE_CSPROPERTY_ITEM(\
        CSPROPERTY_CLOCK_RESOLUTION,\
        (Handler),\
        sizeof(CSPROPERTY),\
        sizeof(CSRESOLUTION),\
        NULL, NULL, 0, NULL, NULL, 0)

#define DEFINE_CSPROPERTY_ITEM_CLOCK_STATE(Handler)\
    DEFINE_CSPROPERTY_ITEM(\
        CSPROPERTY_CLOCK_STATE,\
        (Handler),\
        sizeof(CSPROPERTY),\
        sizeof(CSSTATE),\
        NULL, NULL, 0, NULL, NULL, 0)

#define DEFINE_CSPROPERTY_ITEM_CLOCK_FUNCTIONTABLE(Handler)\
    DEFINE_CSPROPERTY_ITEM(\
        CSPROPERTY_CLOCK_FUNCTIONTABLE,\
        (Handler),\
        sizeof(CSPROPERTY),\
        sizeof(CSCLOCK_FUNCTIONTABLE),\
        NULL, NULL, 0, NULL, NULL, 0)

#define DEFINE_CSPROPERTY_CLOCCSET(ClockSet,\
    PropTime, PropPhysicalTime,\
    PropCorrelatedTime, PropCorrelatedPhysicalTime,\
    PropResolution, PropState, PropFunctionTable)\
DEFINE_CSPROPERTY_TABLE(ClockSet) {\
    DEFINE_CSPROPERTY_ITEM_CLOCK_TIME(PropTime),\
    DEFINE_CSPROPERTY_ITEM_CLOCK_PHYSICALTIME(PropPhysicalTime),\
    DEFINE_CSPROPERTY_ITEM_CLOCK_CORRELATEDTIME(PropCorrelatedTime),\
    DEFINE_CSPROPERTY_ITEM_CLOCK_CORRELATEDPHYSICALTIME(PropCorrelatedPhysicalTime),\
    DEFINE_CSPROPERTY_ITEM_CLOCK_RESOLUTION(PropResolution),\
    DEFINE_CSPROPERTY_ITEM_CLOCK_STATE(PropState),\
    DEFINE_CSPROPERTY_ITEM_CLOCK_FUNCTIONTABLE(PropFunctionTable),\
}

#endif // defined(_NTDDK_)

#define STATIC_CSEVENTSETID_Clock \
    0x364D8E20L, 0x62C7, 0x11CF, 0xA5, 0xD6, 0x28, 0xDB, 0x04, 0xC1, 0x00, 0x00
DEFINE_GUIDSTRUCT("364D8E20-62C7-11CF-A5D6-28DB04C10000", CSEVENTSETID_Clock);
#define CSEVENTSETID_Clock DEFINE_GUIDNAMED(CSEVENTSETID_Clock)

typedef enum {
    CSEVENT_CLOCK_INTERVAL_MARK,
    CSEVENT_CLOCK_POSITION_MARK
} CSEVENT_CLOCK_POSITION;

#define STATIC_CSEVENTSETID_Connection\
    0x7f4bcbe0L, 0x9ea5, 0x11cf, 0xa5, 0xd6, 0x28, 0xdb, 0x04, 0xc1, 0x00, 0x00
DEFINE_GUIDSTRUCT("7f4bcbe0-9ea5-11cf-a5d6-28db04c10000", CSEVENTSETID_Connection);
#define CSEVENTSETID_Connection DEFINE_GUIDNAMED(CSEVENTSETID_Connection)


#if defined(_NTDDK_)


#define CSSTREAM_READ           CSPROBE_STREAMREAD
#define CSSTREAM_WRITE          CSPROBE_STREAMWRITE
#define CSSTREAM_PAGED_DATA     0x00000000
#define CSSTREAM_NONPAGED_DATA  0x00000100
#define CSSTREAM_SYNCHRONOUS    0x00001000
#define CSSTREAM_FAILUREEXCEPTION 0x00002000

typedef
NTSTATUS
(*PFNCSHANDLER)(
//    IN PIRP Irp,
    IN PCSIDENTIFIER Request,
    IN OUT PVOID Data
    );

typedef
BOOLEAN
(*PFNCSFASTHANDLER)(
//    IN PFILE_OBJECT FileObject,
    IN PCSIDENTIFIER Request,
    IN ULONG RequestLength,
    IN OUT PVOID Data,
    IN ULONG DataLength //, 
//    OUT PIO_STATUS_BLOCK IoStatus
    );

typedef
NTSTATUS
(*PFNCSALLOCATOR)(
//    IN PIRP Irp,
    IN ULONG BufferSize,
    IN BOOLEAN InputOperation
    );
#endif 
#endif //#ifndef UNDER_CE

typedef struct {
    CSPROPERTY_MEMBERSHEADER    MembersHeader;
    const VOID*                 Members;
} CSPROPERTY_MEMBERSLIST, *PCSPROPERTY_MEMBERSLIST;

typedef struct {
    CSIDENTIFIER                    PropTypeSet;
    ULONG                           MembersListCount;
    const CSPROPERTY_MEMBERSLIST*   MembersList;
} CSPROPERTY_VALUES, *PCSPROPERTY_VALUES;

#define DEFINE_CSPROPERTY_TABLE(tablename)\
    const CSPROPERTY_ITEM tablename[] =

#ifndef UNDER_CE
#define DEFINE_CSPROPERTY_ITEM(PropertyId, GetHandler,\
                               MinProperty,\
                               MinData,\
                               SetHandler,\
                               Values, RelationsCount, Relations, SupportHandler,\
                               SerializedSize)\
{\
    PropertyId, (PFNCSHANDLER)GetHandler, MinProperty, MinData,\
    (PFNCSHANDLER)SetHandler,\
    (PCSPROPERTY_VALUES)Values, RelationsCount, (PCSPROPERTY)Relations,\
    (PFNCSHANDLER)SupportHandler, (ULONG)SerializedSize\
}
#else
#define DEFINE_CSPROPERTY_ITEM(PropertyId, GetHandler,\
                               MinProperty,\
                               MinData,\
                               SetHandler,\
                               Values, RelationsCount, Relations, SupportHandler,\
                               SerializedSize)\
{\
    PropertyId, (BOOL)GetHandler, MinProperty, MinData,\
    (BOOL)SetHandler,\
    (PCSPROPERTY_VALUES)Values, RelationsCount, (PCSPROPERTY)Relations,\
	NULL,0\
}
#endif //#ifndef UNDER_CE

typedef struct {
    ULONG                   PropertyId;
#ifndef UNDER_CE
    union {
        PFNCSHANDLER            GetPropertyHandler;
#endif //#ifndef UNDER_CE
        BOOLEAN                 GetSupported;
#ifndef UNDER_CE
    };
#endif //#ifndef UNDER_CE
    ULONG                   MinProperty;
    ULONG                   MinData;
#ifndef UNDER_CE
	union {
        PFNCSHANDLER            SetPropertyHandler;
#endif //#ifndef UNDER_CE
		BOOLEAN                 SetSupported;
#ifndef UNDER_CE
	   };
#endif //#ifndef UNDER_CE
    const CSPROPERTY_VALUES*Values;
    ULONG                   RelationsCount;
    const CSPROPERTY*       Relations;
#ifndef UNDER_CE
    PFNCSHANDLER            SupportHandler;
	ULONG                   SerializedSize;
#else
	LPVOID					Reserved1;		//Must be NULL
	ULONG                   Reserved2;		//Must be 0
#endif //#ifndef UNDER_CE
    
} CSPROPERTY_ITEM, *PCSPROPERTY_ITEM;

#ifndef UNDER_CE

typedef struct {
    ULONG                       PropertyId;
    union {
        PFNCSFASTHANDLER            GetPropertyHandler;
        BOOLEAN                     GetSupported;
    };
    union {
        PFNCSFASTHANDLER            SetPropertyHandler;
        BOOLEAN                     SetSupported;
    };
    ULONG                       Reserved;
} CSFASTPROPERTY_ITEM, *PCSFASTPROPERTY_ITEM;
#endif //#ifndef UNDER_CE

#define DEFINE_CSPROPERTY_SET(Set,\
                              PropertiesCount,\
                              PropertyItem,\
                              FastIoCount,\
                              FastIoTable)\
{\
    Set,\
    PropertiesCount,\
    PropertyItem,\
    FastIoCount,\
    FastIoTable\
}

#define DEFINE_CSPROPERTY_SET_TABLE(tablename)\
    const CSPROPERTY_SET tablename[] =

typedef struct {
    const GUID*                 Set;
    ULONG                       PropertiesCount;
    const CSPROPERTY_ITEM*      PropertyItem;
    ULONG                       FastIoCount;
#ifndef UNDER_CE
    const CSFASTPROPERTY_ITEM*  FastIoTable;
#else
	LPVOID						Reserved ;
#endif //#ifndef UNDER_CE
} CSPROPERTY_SET, *PCSPROPERTY_SET;

#ifndef UNDER_CE
#define DEFINE_CSMETHOD_TABLE(tablename)\
    const CSMETHOD_ITEM tablename[] =

#define DEFINE_CSMETHOD_ITEM(MethodId, Flags,\
                             MethodHandler,\
                             MinMethod, MinData, SupportHandler)\
{\
    MethodId, (PFNCSHANDLER)MethodHandler, MinMethod, MinData,\
    SupportHandler, Flags\
}

typedef struct {
    ULONG                   MethodId;
    union {
        PFNCSHANDLER            MethodHandler;
        BOOLEAN                 MethodSupported;
    };
    ULONG                   MinMethod;
    ULONG                   MinData;
    PFNCSHANDLER            SupportHandler;
    ULONG                   Flags;
} CSMETHOD_ITEM, *PCSMETHOD_ITEM;

#define DEFINE_CSFASTMETHOD_ITEM(MethodId, MethodHandler)\
{\
    MethodId, (PFNCSFASTHANDLER)MethodHandler\
}

typedef struct {
    ULONG                   MethodId;
    union {
        PFNCSFASTHANDLER        MethodHandler;
        BOOLEAN                 MethodSupported;
    };
} CSFASTMETHOD_ITEM, *PCSFASTMETHOD_ITEM;

#define DEFINE_CSMETHOD_SET(Set,\
                            MethodsCount,\
                            MethodItem,\
                            FastIoCount,\
                            FastIoTable)\
{\
    Set,\
    MethodsCount,\
    MethodItem,\
    FastIoCount,\
    FastIoTable\
}

#define DEFINE_CSMETHOD_SET_TABLE(tablename)\
    const CSMETHOD_SET tablename[] =

typedef struct {
    const GUID*             Set;
    ULONG                   MethodsCount;
    const CSMETHOD_ITEM*    MethodItem;
    ULONG                   FastIoCount;
    const CSFASTMETHOD_ITEM*FastIoTable;
} CSMETHOD_SET, *PCSMETHOD_SET;

typedef struct _CSEVENT_ENTRY
CSEVENT_ENTRY, *PCSEVENT_ENTRY;
#endif //#ifndef UNDER_CE


typedef enum {
    CS_ALLOCATE,
    CS_ENQUEUE,
    CS_DEALLOCATE
} BUFFER_COMMANDS;

#endif // !_CS_
