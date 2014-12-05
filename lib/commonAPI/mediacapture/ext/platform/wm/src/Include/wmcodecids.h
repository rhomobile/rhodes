//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this source code is subject to the terms of your Microsoft Windows CE
// Source Alliance Program license form.  If you did not accept the terms of
// such a license, you are not authorized to use this source code.
//
//+-------------------------------------------------------------------------
//
//  Microsoft Windows Media
//
//
//  File:       wmcodecids.h
//
//--------------------------------------------------------------------------

#ifndef __WMCODECIDS_H_
#define __WMCODECIDS_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//
// DMO CLSIDs for Windows Media Codecs
//

EXTERN_GUID(CLSID_CWMADecMediaObject,  0x2eeb4adf, 0x4578, 0x4d10, 0xbc, 0xa7, 0xbb, 0x95, 0x5f, 0x56, 0x32, 0x0a);
EXTERN_GUID(CLSID_CWMSDecMediaObject,  0x874131cb, 0x4ecc, 0x443b, 0x89, 0x48, 0x74, 0x6b, 0x89, 0x59, 0x5d, 0x20);
EXTERN_GUID(CLSID_CWMV9EncMediaObject, 0xd23b90d0, 0x144f, 0x46bd, 0x84, 0x1d, 0x59, 0xe4, 0xeb, 0x19, 0xdc, 0x59);
EXTERN_GUID(CLSID_CWMVDecMediaObject,  0x82d353df, 0x90bd, 0x4382, 0x8b, 0xc2, 0x3f, 0x61, 0x92, 0xb7, 0x6e, 0x34);

#endif  // !defined(__WMCODECIDS_H_)
