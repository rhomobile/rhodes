//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this sample source code is subject to the terms of the Microsoft
// license agreement under which you licensed this sample source code. If
// you did not accept the terms of the license agreement, you are not
// authorized to use this sample source code. For the terms of the license,
// please see the license agreement between you and Microsoft or, if applicable,
// see the LICENSE.RTF on your install media or the root of your tools installation.
// THE SAMPLE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES OR INDEMNITIES.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//
// DESCRIPTION:
// Configmanager API to process XML configuration files

#ifndef _PROCESSCFG_H_
#define _PROCESSCFG_H_

#ifdef __cplusplus
extern "C" {
#endif

// Possible error codes returned by Configmanager
#define CONFIG_E_OBJECTBUSY                 _HRESULT_TYPEDEF_(0x80042001)
#define CONFIG_E_CANCELTIMEOUT              _HRESULT_TYPEDEF_(0x80042002)
#define CONFIG_E_ENTRYNOTFOUND              _HRESULT_TYPEDEF_(0x80042004)
#define CONFIG_S_PROCESSINGCANCELED         _HRESULT_TYPEDEF_(0x00042005)
#define CONFIG_E_CSPEXCEPTION               _HRESULT_TYPEDEF_(0x80042007)
#define CONFIG_E_TRANSACTIONINGFAILURE      _HRESULT_TYPEDEF_(0x80042008)
#define CONFIG_E_BAD_XML                    _HRESULT_TYPEDEF_(0x80042009)

// Configmanager flags
#define CFGFLAG_PROCESS                     0x0001
#define CFGFLAG_METADATA                    0x0002

HRESULT DMProcessConfigXML(LPCWSTR pszWXMLin, DWORD dwFlags, LPWSTR *ppszwXMLout);

#ifdef __cplusplus
}
#endif

#endif // _PROCESSCFG_H_
