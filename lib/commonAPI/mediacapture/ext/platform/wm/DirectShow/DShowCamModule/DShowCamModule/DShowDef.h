//------------------------------------------------------------------
// FILENAME: DShowDef.h
//
// Copyright(c) 2003 Symbol Technologies Inc. All rights reserved.
//
// DESCRIPTION:	This is a C++ header containing user Defintions that needed to be used in the DirectShow Wrapper class implementation
//
// %IF Symbol_Internal
// AUTHOR: Wajra Attale
// CREATION DATE: 01/20/2007
// DERIVED FROM: New File
//
// EDIT HISTORY: No PVCS history for this file
// $Log:   T:/MPA2.0_sandbox/archives/SymbolValueAdd_mpa2/CCL/Camera/DShowCam/test/DShowTestApp/DShowDef.h-arc  $
//
//   Rev 1.0   Apr 18 2008 10:17:30   cheung
//Initial revision.
//
//   Rev 1.0   Dec 14 2007 07:20:58   sepalas
//Initial revision.
//
//%End
//------------------------------------------------------------------
#ifndef HEADERFILE_DSD_
#define HEADERFILE_DSD_

//#ifndef DEBUG_MODE
//#define DEBUG_MODE
//#endif 
//#define CUSTOM_PROP

const MEDIA_SUBTYPE_LIST media_list[] = {
      {TEXT("RGB1"),&MEDIASUBTYPE_RGB1},
      {TEXT("RGB4"),&MEDIASUBTYPE_RGB4},
      {TEXT("RGB8"),&MEDIASUBTYPE_RGB8},
      {TEXT("RGB565"),&MEDIASUBTYPE_RGB565},
      {TEXT("RGB555"),&MEDIASUBTYPE_RGB555},
      {TEXT("RGB24"),&MEDIASUBTYPE_RGB24},
      {TEXT("RGB32"),&MEDIASUBTYPE_RGB32},
	  {TEXT("YVU9"),&MEDIASUBTYPE_YVU9},
      {TEXT("Y411"),&MEDIASUBTYPE_Y411},
      {TEXT("Y41P"),&MEDIASUBTYPE_Y41P},
      {TEXT("YUY2"),&MEDIASUBTYPE_YUY2},
      {TEXT("YVYU"),&MEDIASUBTYPE_YVYU},
      {TEXT("UYVY"),&MEDIASUBTYPE_UYVY},
      {TEXT("Y211"),&MEDIASUBTYPE_Y211},
      {TEXT("YV12"),&MEDIASUBTYPE_YV12},
      {TEXT("Overlay"),&MEDIASUBTYPE_Overlay},
      {TEXT("INVALIDE"),NULL},
};


#define DFT_VDO_FN L"\\My Documents\\My Pictures\\Default.wmv"
#define EXT_VDO_FN L"\\My Documents\\My Pictures\\DefaultExt"
#define TXT_LENGTH 256
#define PROP_LENGHT 128
#define DAY_LENGTH 10
#define TIME_LENGTH 10
#define START_TIME 0
#define STOP_TIME MAXLONGLONG
#define START_COOKIE 1
#define STOP_COOKIE 2
#define VSTART_COOKIE 3
#define VSTOP_COOKIE 4
#define ASTART_COOKIE 5
#define ASTOP_COOKIE 6

#define Index0 0
#define Index1 1
#define Index2 2
#define Index3 3
#define Index4 4
#define Index5 5
#define Index6 6
#define Index7 7
#define Index8 8
#define Index9 9

#endif //HEADERFILE_DSD_