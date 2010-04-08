#ifndef __PAYLOAD_TYPES__H__
#define __PAYLOAD_TYPES__H__

#include <common/spirit_comtypes.h>

#define SPIRIT_USER_PAYLOAD_TYPE      96
#define SPIRIT_USER_VOICE_CODEC       0  //  96 +  0 = 96
#define SPIRIT_USER_VIDEO_CODEC       14 //  96 + 14 = 110

typedef struct _sSpiritPayloadType
{
  const char      aFriendlyCodecName[255];
  unsigned char   PayloadType;
}sPayloadType;

/*****************************************************************************

// 96-127  dynamic

Voice codecs
---------------
0    PCMU 
1    reserved
2    reserved
3    GSM
4    G723
5    DVI4
6    DVI4
7    LPC
8    PCMA
9    G722
10   L16
11   L16
12   QCELP
13   CN
14   MPA
15   G728
16   DVI4
17   DVI4
18   G729
19   reserved
20   unassigned
21   unassigned
22   unassigned
23   unassigned
dyn  G726-40
dyn  G726-32
dyn  G726-24
dyn  G726-16
dyn  G729D
dyn  G729E
dyn  GSM-EFR
dyn  L8
dyn  RED
dyn  VDVI

Video codecs
---------------
24      unassigned
25      CelB
26      JPEG
27      unassigned
28      nv
29      unassigned
30      unassigned
31      H261
32      MPV
33      MP2T
34      H263
35-71   unassigned
72-76   reserved
77-95   unassigned
dyn     H263-1998

*****************************************************************************/

// Voice codecs
#define SPIRIT_PAYLOAD_TYPE_PCMU        0
#define SPIRIT_PAYLOAD_TYPE_PCMA        8
#define SPIRIT_PAYLOAD_TYPE_G722        9
#define SPIRIT_PAYLOAD_TYPE_G723        4
#define SPIRIT_PAYLOAD_TYPE_G729        18
#define SPIRIT_PAYLOAD_TYPE_AMRNB       (SPIRIT_USER_PAYLOAD_TYPE + SPIRIT_USER_VOICE_CODEC + 0)  // 96
#define SPIRIT_PAYLOAD_TYPE_AMRWB       (SPIRIT_USER_PAYLOAD_TYPE + SPIRIT_USER_VOICE_CODEC + 1)  // 97
#define SPIRIT_PAYLOAD_TYPE_G722_1      (SPIRIT_USER_PAYLOAD_TYPE + SPIRIT_USER_VOICE_CODEC + 2)  // 98
#define SPIRIT_PAYLOAD_TYPE_ILBC        (SPIRIT_USER_PAYLOAD_TYPE + SPIRIT_USER_VOICE_CODEC + 3)  // 99
#define SPIRIT_PAYLOAD_TYPE_IPMR        120 // same as PC version //(SPIRIT_USER_PAYLOAD_TYPE + SPIRIT_USER_VOICE_CODEC + 2)  // 98

// Video codecs
#define SPIRIT_PAYLOAD_TYPE_CELB        25
#define SPIRIT_PAYLOAD_TYPE_JPEG        26
#define SPIRIT_PAYLOAD_TYPE_NV          28
#define SPIRIT_PAYLOAD_TYPE_H261        31
#define SPIRIT_PAYLOAD_TYPE_MPV         32
#define SPIRIT_PAYLOAD_TYPE_MP2T        33
#define SPIRIT_PAYLOAD_TYPE_H263        34
#define SPIRIT_PAYLOAD_TYPE_INTEL_H263  34

#define SPIRIT_PAYLOAD_TYPE_H263_1998        (SPIRIT_USER_PAYLOAD_TYPE + SPIRIT_USER_VIDEO_CODEC + 0)  // 110
#define SPIRIT_PAYLOAD_TYPE_H264             (SPIRIT_USER_PAYLOAD_TYPE + SPIRIT_USER_VIDEO_CODEC + 1)  // 111
#define SPIRIT_PAYLOAD_TYPE_MPEG4_SP         (SPIRIT_USER_PAYLOAD_TYPE + SPIRIT_USER_VIDEO_CODEC + 2)  // 112
#define SPIRIT_PAYLOAD_TYPE_INTEL_H263_1998  (SPIRIT_USER_PAYLOAD_TYPE + SPIRIT_USER_VIDEO_CODEC + 3)  // 113
#define SPIRIT_PAYLOAD_TYPE_INTEL_H264       (SPIRIT_USER_PAYLOAD_TYPE + SPIRIT_USER_VIDEO_CODEC + 4)  // 114

SPIRIT_EXTERN_C uint8 GetPayloadTypeByName(const char *pFriendlyCodecName);

#endif // __PAYLOAD_TYPES__H__
