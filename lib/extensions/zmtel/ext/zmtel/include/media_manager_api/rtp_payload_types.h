/******************************************************************************
  FILE: rtp_payload_types.h
  DESCRIPTION: N/A yet

                          This file is a part of Spirit Embedded Media Manager.

  Copyright SPIRIT DSP 2008.                                 www.spiritdsp.com
******************************************************************************/
#ifndef _RTP_PALOAD_TYPES_H_
#define _RTP_PALOAD_TYPES_H_

/*****************************************************************************
             Table 4: Payload types (PT) for audio encodings

             PT   encoding    media type  clock rate   channels
                  name                    (Hz)
             ___________________________________________________
             0    PCMU        A            8,000       1
             1    reserved    A
             2    reserved    A
             3    GSM         A            8,000       1
             4    G723        A            8,000       1
             5    DVI4        A            8,000       1
             6    DVI4        A           16,000       1
             7    LPC         A            8,000       1
             8    PCMA        A            8,000       1
             9    G722        A            8,000       1
             10   L16         A           44,100       2
             11   L16         A           44,100       1
             12   QCELP       A            8,000       1
             13   CN          A            8,000       1
             14   MPA         A           90,000       (see text)
             15   G728        A            8,000       1
             16   DVI4        A           11,025       1
             17   DVI4        A           22,050       1
             18   G729        A            8,000       1
             19   reserved    A
             20   unassigned  A
             21   unassigned  A
             22   unassigned  A
             23   unassigned  A
             dyn  G726-40     A            8,000       1
             dyn  G726-32     A            8,000       1
             dyn  G726-24     A            8,000       1
             dyn  G726-16     A            8,000       1
             dyn  G729D       A            8,000       1
             dyn  G729E       A            8,000       1
             dyn  GSM-EFR     A            8,000       1
             dyn  L8          A            var.        var.
             dyn  RED         A                        (see text)
             dyn  VDVI        A            var.        1
******************************************************************************/
#define RTP_PAYLOAD_TYPE_DYN       255


#define RTP_PAYLOAD_TYPE_PCMU      0
#define RTP_PAYLOAD_TYPE_GSM       3
#define RTP_PAYLOAD_TYPE_G723      4
#define RTP_PAYLOAD_TYPE_DVI4_8    5
#define RTP_PAYLOAD_TYPE_DVI4_16   6
#define RTP_PAYLOAD_TYPE_LPC       7
#define RTP_PAYLOAD_TYPE_PCMA      8
#define RTP_PAYLOAD_TYPE_G722      9
#define RTP_PAYLOAD_TYPE_L16_2CH   10
#define RTP_PAYLOAD_TYPE_L16_1CH   11
#define RTP_PAYLOAD_TYPE_QCELP     12
#define RTP_PAYLOAD_TYPE_CN        13
#define RTP_PAYLOAD_TYPE_MPA       14
#define RTP_PAYLOAD_TYPE_G728      15
#define RTP_PAYLOAD_TYPE_DVI4_11   16
#define RTP_PAYLOAD_TYPE_DVI4_22   17
#define RTP_PAYLOAD_TYPE_G729      18

#define RTP_PAYLOAD_TYPE_G726_40   RTP_PAYLOAD_TYPE_DYN
#define RTP_PAYLOAD_TYPE_G726_32   RTP_PAYLOAD_TYPE_DYN
#define RTP_PAYLOAD_TYPE_G726_24   RTP_PAYLOAD_TYPE_DYN
#define RTP_PAYLOAD_TYPE_G726_16   RTP_PAYLOAD_TYPE_DYN
#define RTP_PAYLOAD_TYPE_G729D     RTP_PAYLOAD_TYPE_DYN
#define RTP_PAYLOAD_TYPE_G729E     RTP_PAYLOAD_TYPE_DYN
#define RTP_PAYLOAD_TYPE_GSM_EFR   RTP_PAYLOAD_TYPE_DYN
#define RTP_PAYLOAD_TYPE_L8        RTP_PAYLOAD_TYPE_DYN
#define RTP_PAYLOAD_TYPE_RED       RTP_PAYLOAD_TYPE_DYN
#define RTP_PAYLOAD_TYPE_VDVI      RTP_PAYLOAD_TYPE_DYN



/*****************************************************************************

            Table 5: Payload types (PT) for video and combined    encodings

             PT      encoding    media type  clock rate
                     name                    (Hz)
             _____________________________________________
             24      unassigned  V
             25      CelB        V           90,000
             26      JPEG        V           90,000
             27      unassigned  V
             28      nv          V           90,000
             29      unassigned  V
             30      unassigned  V
             31      H261        V           90,000
             32      MPV         V           90,000
             33      MP2T        AV          90,000
             34      H263        V           90,000
             35-71   unassigned  ?
             72-76   reserved    N/A         N/A
             77-95   unassigned  ?
             96-127  dynamic     ?
             dyn     H263-1998   V           90,000

******************************************************************************/

#define RTP_PAYLOAD_TYPE_CELB       25
#define RTP_PAYLOAD_TYPE_JPEG       26
#define RTP_PAYLOAD_TYPE_NV         28
#define RTP_PAYLOAD_TYPE_H261       31
#define RTP_PAYLOAD_TYPE_MPV        32
#define RTP_PAYLOAD_TYPE_MP2T       33
#define RTP_PAYLOAD_TYPE_H263       34

#endif // _RTP_PALOAD_TYPES_H_
