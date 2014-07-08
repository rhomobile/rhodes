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
/*++
THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.
Copyright (c) Microsoft Corporation. All rights reserved.

-----------------------------------------------------------------------------

@doc EXTERNAL

@module SIMMGR.H - SIM Manager |

SIM Manager is an API set that allows access to information stored on the SIM Card


-----------------------------------------------------------------------------

--*/


#ifndef _SIMMGR_H_
#define _SIMMGR_H_


#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif


// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @constants Error Class | Each SIM error falls into a general error class bucket
//
// @comm In SIM Manager, the low order 16 bits are divided into an 8-bit error class and
//       an 8-bit error value.  Use the SIMERRORCLASS macro to obtain the error
//       class from a SIM HRESULT.
//
// -----------------------------------------------------------------------------
#define SIM_ERRORCLASS_NONE                         0x00  // @constdefine Misc error
#define SIM_ERRORCLASS_PASSWORD                     0x01  // @constdefine Unspecified phone failure
#define SIM_ERRORCLASS_SIM                          0x02  // @constdefine Problem with the SIM
#define SIM_ERRORCLASS_STORAGE                      0x03  // @constdefine Error relating to storage

#define MAKE_SIMERROR(errclass,code) \
    ( (unsigned long)(errclass)<<8) | ((unsigned long)(code) )

#define SIMERRORCLASS(simerror) \
	((unsigned long) (((simerror)>>8) & 0xff))

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @constants Error | Various errors
//
// @comm None
//
// -----------------------------------------------------------------------------
#define FACILITY_SIM                    0x800

#define SIM_E_SIMFAILURE                (MAKE_HRESULT(SEVERITY_ERROR, FACILITY_SIM, MAKE_SIMERROR(SIM_ERRORCLASS_SIM,0x01)))  // @constdefine SIM failure was detected
#define SIM_E_SIMBUSY                   (MAKE_HRESULT(SEVERITY_ERROR, FACILITY_SIM, MAKE_SIMERROR(SIM_ERRORCLASS_SIM,0x02)))  // @constdefine SIM is busy
#define SIM_E_SIMWRONG                  (MAKE_HRESULT(SEVERITY_ERROR, FACILITY_SIM, MAKE_SIMERROR(SIM_ERRORCLASS_SIM,0x03)))  // @constdefine Inorrect SIM was inserted
#define SIM_E_NOSIMMSGSTORAGE           (MAKE_HRESULT(SEVERITY_ERROR, FACILITY_SIM, MAKE_SIMERROR(SIM_ERRORCLASS_SIM,0x04)))  // @constdefine SIM isn't capable of storing messages
#define SIM_E_SIMTOOLKITBUSY            (MAKE_HRESULT(SEVERITY_ERROR, FACILITY_SIM, MAKE_SIMERROR(SIM_ERRORCLASS_SIM,0x05)))  // @constdefine SIM Application Toolkit is busy
#define SIM_E_SIMDOWNLOADERROR          (MAKE_HRESULT(SEVERITY_ERROR, FACILITY_SIM, MAKE_SIMERROR(SIM_ERRORCLASS_SIM,0x06)))  // @constdefine SIM data download error
#define SIM_E_SIMNOTINSERTED            (MAKE_HRESULT(SEVERITY_ERROR, FACILITY_SIM, MAKE_SIMERROR(SIM_ERRORCLASS_SIM,0x07)))  // @constdefine SIM isn't inserted into the phone
#define SIM_E_PHSIMPINREQUIRED          (MAKE_HRESULT(SEVERITY_ERROR, FACILITY_SIM, MAKE_SIMERROR(SIM_ERRORCLASS_PASSWORD,0x20)))  // @constdefine PH-SIM PIN is required to perform this operation
#define SIM_E_PHFSIMPINREQUIRED         (MAKE_HRESULT(SEVERITY_ERROR, FACILITY_SIM, MAKE_SIMERROR(SIM_ERRORCLASS_PASSWORD,0x21)))  // @constdefine PH-FSIM PIN is required to perform this operation
#define SIM_E_PHFSIMPUKREQUIRED         (MAKE_HRESULT(SEVERITY_ERROR, FACILITY_SIM, MAKE_SIMERROR(SIM_ERRORCLASS_PASSWORD,0x22)))  // @constdefine PH-FSIM PUK is required to perform this operation
#define SIM_E_SIMPINREQUIRED            (MAKE_HRESULT(SEVERITY_ERROR, FACILITY_SIM, MAKE_SIMERROR(SIM_ERRORCLASS_PASSWORD,0x23)))  // @constdefine SIM PIN is required to perform this operation
#define SIM_E_SIMPUKREQUIRED            (MAKE_HRESULT(SEVERITY_ERROR, FACILITY_SIM, MAKE_SIMERROR(SIM_ERRORCLASS_PASSWORD,0x24)))  // @constdefine SIM PUK is required to perform this operation
#define SIM_E_INCORRECTPASSWORD         (MAKE_HRESULT(SEVERITY_ERROR, FACILITY_SIM, MAKE_SIMERROR(SIM_ERRORCLASS_PASSWORD,0x25)))  // @constdefine Incorrect password was supplied
#define SIM_E_SIMPIN2REQUIRED           (MAKE_HRESULT(SEVERITY_ERROR, FACILITY_SIM, MAKE_SIMERROR(SIM_ERRORCLASS_PASSWORD,0x26)))  // @constdefine SIM PIN2 is required to perform this operation
#define SIM_E_SIMPUK2REQUIRED           (MAKE_HRESULT(SEVERITY_ERROR, FACILITY_SIM, MAKE_SIMERROR(SIM_ERRORCLASS_PASSWORD,0x27)))  // @constdefine SIM PUK2 is required to perform this operation
#define SIM_E_NETWKPINREQUIRED          (MAKE_HRESULT(SEVERITY_ERROR, FACILITY_SIM, MAKE_SIMERROR(SIM_ERRORCLASS_PASSWORD,0x28)))  // @constdefine Network Personalization PIN is required to perform this operation
#define SIM_E_NETWKPUKREQUIRED          (MAKE_HRESULT(SEVERITY_ERROR, FACILITY_SIM, MAKE_SIMERROR(SIM_ERRORCLASS_PASSWORD,0x29)))  // @constdefine Network Personalization PUK is required to perform this operation
#define SIM_E_SUBSETPINREQUIRED         (MAKE_HRESULT(SEVERITY_ERROR, FACILITY_SIM, MAKE_SIMERROR(SIM_ERRORCLASS_PASSWORD,0x2A)))  // @constdefine Network Subset Personalization PIN is required to perform this operation
#define SIM_E_SUBSETPUKREQUIRED         (MAKE_HRESULT(SEVERITY_ERROR, FACILITY_SIM, MAKE_SIMERROR(SIM_ERRORCLASS_PASSWORD,0x2B)))  // @constdefine Network Subset Personalization PUK is required to perform this operation
#define SIM_E_SVCPINREQUIRED            (MAKE_HRESULT(SEVERITY_ERROR, FACILITY_SIM, MAKE_SIMERROR(SIM_ERRORCLASS_PASSWORD,0x2C)))  // @constdefine Service Provider Personalization PIN is required to perform this operation
#define SIM_E_SVCPUKREQUIRED            (MAKE_HRESULT(SEVERITY_ERROR, FACILITY_SIM, MAKE_SIMERROR(SIM_ERRORCLASS_PASSWORD,0x2D)))  // @constdefine Service Provider Personalization PUK is required to perform this operation
#define SIM_E_CORPPINREQUIRED           (MAKE_HRESULT(SEVERITY_ERROR, FACILITY_SIM, MAKE_SIMERROR(SIM_ERRORCLASS_PASSWORD,0x2E)))  // @constdefine Corporate Personalization PIN is required to perform this operation
#define SIM_E_CORPPUKREQUIRED           (MAKE_HRESULT(SEVERITY_ERROR, FACILITY_SIM, MAKE_SIMERROR(SIM_ERRORCLASS_PASSWORD,0x2F)))  // @constdefine Corporate Personalization PUK is required to perform this operation
#define SIM_E_MEMORYFULL                (MAKE_HRESULT(SEVERITY_ERROR, FACILITY_SIM, MAKE_SIMERROR(SIM_ERRORCLASS_STORAGE,0x40)))  // @constdefine Storage memory is full
#define SIM_E_INVALIDINDEX              (MAKE_HRESULT(SEVERITY_ERROR, FACILITY_SIM, MAKE_SIMERROR(SIM_ERRORCLASS_STORAGE,0x41)))  // @constdefine Invalid storage index was supplied
#define SIM_E_NOTFOUND                  (MAKE_HRESULT(SEVERITY_ERROR, FACILITY_SIM, MAKE_SIMERROR(SIM_ERRORCLASS_STORAGE,0x42)))  // @constdefine A requested storage entry was not found
#define SIM_E_MEMORYFAILURE             (MAKE_HRESULT(SEVERITY_ERROR, FACILITY_SIM, MAKE_SIMERROR(SIM_ERRORCLASS_STORAGE,0x43)))  // @constdefine Storage memory failure
#define SIM_E_SIMMSGSTORAGEFULL         (MAKE_HRESULT(SEVERITY_ERROR, FACILITY_SIM, MAKE_SIMERROR(SIM_ERRORCLASS_STORAGE,0x44)))  // @constdefine Message storage on the SIM is full
#define SIM_E_EMPTYINDEX                (MAKE_HRESULT(SEVERITY_ERROR, FACILITY_SIM, MAKE_SIMERROR(SIM_ERRORCLASS_STORAGE,0x45)))  // @constdefine Storage location is empty
#define SIM_E_TEXTSTRINGTOOLONG         (MAKE_HRESULT(SEVERITY_ERROR, FACILITY_SIM, MAKE_SIMERROR(SIM_ERRORCLASS_STORAGE,0x46)))  // @constdefine Supplied text string is too long
#define SIM_E_DIALSTRINGTOOLONG         (MAKE_HRESULT(SEVERITY_ERROR, FACILITY_SIM, MAKE_SIMERROR(SIM_ERRORCLASS_STORAGE,0x47)))  // @constdefine Supplied dial string contains invalid characters
#define SIM_E_NOTREADY                  (MAKE_HRESULT(SEVERITY_ERROR, FACILITY_SIM, MAKE_SIMERROR(SIM_ERRORCLASS_NONE,0x60)))  // @constdefine SIM isn't yet ready to perform the requested operation
#define SIM_E_SECURITYFAILURE           (MAKE_HRESULT(SEVERITY_ERROR, FACILITY_SIM, MAKE_SIMERROR(SIM_ERRORCLASS_NONE,0x61)))  // @constdefine SIM isn't yet ready to perform the requested operation
#define SIM_E_BUFFERTOOSMALL            (MAKE_HRESULT(SEVERITY_ERROR, FACILITY_SIM, MAKE_SIMERROR(SIM_ERRORCLASS_NONE,0x62)))  // @constdefine Buffer too small
#define SIM_E_NOTTEXTMESSAGE            (MAKE_HRESULT(SEVERITY_ERROR, FACILITY_SIM, MAKE_SIMERROR(SIM_ERRORCLASS_NONE,0x63)))  // @constdefine Requested SMS message is not a text message
#define SIM_E_NOSIM                     (MAKE_HRESULT(SEVERITY_ERROR, FACILITY_SIM, MAKE_SIMERROR(SIM_ERRORCLASS_NONE,0x64)))  // @constdefine Device doesn't have a SIM
#define SIM_E_NETWORKERROR              (MAKE_HRESULT(SEVERITY_ERROR, FACILITY_SIM, MAKE_SIMERROR(SIM_ERRORCLASS_NONE,0x65)))  // @constdefine There was a network error
#define SIM_E_MOBILEERROR               (MAKE_HRESULT(SEVERITY_ERROR, FACILITY_SIM, MAKE_SIMERROR(SIM_ERRORCLASS_NONE,0x66)))  // @constdefine Mobile error
#define SIM_E_UNSUPPORTED               (MAKE_HRESULT(SEVERITY_ERROR, FACILITY_SIM, MAKE_SIMERROR(SIM_ERRORCLASS_NONE,0x67)))  // @constdefine The command is unsupported
#define SIM_E_BADPARAM                  (MAKE_HRESULT(SEVERITY_ERROR, FACILITY_SIM, MAKE_SIMERROR(SIM_ERRORCLASS_NONE,0x68)))  // @constdefine Bad parameter
#define SIM_E_UNDETERMINED              (MAKE_HRESULT(SEVERITY_ERROR, FACILITY_SIM, MAKE_SIMERROR(SIM_ERRORCLASS_NONE,0x69)))  // @constdefine Undetermined error
#define SIM_E_RADIONOTPRESENT           (MAKE_HRESULT(SEVERITY_ERROR, FACILITY_SIM, MAKE_SIMERROR(SIM_ERRORCLASS_NONE,0x6A)))  // @constdefine The Radio is not present
#define SIM_E_RADIOOFF                  (MAKE_HRESULT(SEVERITY_ERROR, FACILITY_SIM, MAKE_SIMERROR(SIM_ERRORCLASS_NONE,0x6B)))  // @constdefine The Radio is off

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @constants Callback | Various notifications that are passed to the callback
//
// @comm None
//
// -----------------------------------------------------------------------------
#define SIM_NOTIFY_CARD_REMOVED         (0x100)          // @constdefine SIM card was removed; lpData is NULL
#define SIM_NOTIFY_FILE_REFRESH         (0x101)          // @constdefine Files on the SIM were refreshed; lpData points to a SIMFILEREFRESH structure
#define SIM_NOTIFY_MSG_STORED           (0x102)          // @constdefine A message was stored to the SIM; lpData points to a SIMMESSAGECHANGE structure
#define SIM_NOTIFY_MSG_DELETED          (0x103)          // @constdefine A message was removed from the SIM; lpData points to a SIMMESSAGECHANGE structure
#define SIM_NOTIFY_PBE_STORED           (0x104)          // @constdefine A phone book entry was stored to the SIM; lpData points to a SIMPBECHANGE structure
#define SIM_NOTIFY_PBE_DELETED          (0x105)          // @constdefine A phone book entry was removed from the SIM; lpData points to a SIMPBECHANGE structure
#define SIM_NOTIFY_MSG_RECEIVED         (0x106)          // @constdefine Class 2 SMS was sent directly to the SIM; lpData points to a SIMMESSAGECHANGE structure
#define SIM_NOTIFY_RADIOOFF             (0x107)          // @constdefine The Radio has been turned off but AT interpreter is still on; lpData is NULL
#define SIM_NOTIFY_RADIOON              (0x108)          // @constdefine The Radio is present and is now on; lpData is NULL
#define SIM_NOTIFY_RADIOPRESENT         (0x109)          // @constdefine A Radio Module/Driver has been installed; lpData is points to a DWORD which is 0 if the radio is OFF and 1 if the radio is ON
#define SIM_NOTIFY_RADIOREMOVED         (0x10A)          // @constdefine A Radio Module/Driver has been removed; lpData is NULL
#define SIM_NOTIFY_SMSSTORAGEFULL       (0x10B)          // @constdefine SIM SMS Store is full; lpData points to a SIMSMSSTORAGESTATUS structure. Only supports SIM_SMSSTORAGE_SIM

// Parameter flags for SimInitialize
#define SIM_INIT_NONE                   (0x00000000)     // @constdefine Do not send any notifications
#define SIM_INIT_SIMCARD_NOTIFICATIONS  (0x00000001)     // @constdefine Send SIM card related notifications

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @params SIMPHONEBOOKENTRY
//
// @comm None
//
// -----------------------------------------------------------------------------
#define SIM_PARAM_PBE_ADDRESS           (0x00000001)     // @paramdefine lpszAddress field is valid
#define SIM_PARAM_PBE_ADDRESS_TYPE      (0x00000002)     // @paramdefine dwAddressType field is valid
#define SIM_PARAM_PBE_NUMPLAN           (0x00000004)     // @paramdefine dwNumPlan field is valid
#define SIM_PARAM_PBE_TEXT              (0x00000008)     // @paramdefine lpszText field is valid
#define SIM_PARAM_PBE_ALL               (0x0000000f)     // @paramdefine All fields are valid

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @params SIMPHONEBOOKENTRYEX
//
// @comm None
//
// -----------------------------------------------------------------------------
#define SIM_PARAM_PBEX_ADDRESS              (0x00000001) // @paramdefine lpszAddress field is valid
#define SIM_PARAM_PBEX_ADDRESS_TYPE         (0x00000002) // @paramdefine dwAddressType field is valid
#define SIM_PARAM_PBEX_NUMPLAN              (0x00000004) // @paramdefine dwNumPlan field is valid
#define SIM_PARAM_PBEX_TEXT                 (0x00000008) // @paramdefine lpszText field is valid
#define SIM_PARAM_PBEX_SECONDNAME           (0x00000010) // @paramdefine lpszSecondName field is valid
#define SIM_PARAM_PBEX_HIDDEN               (0x00000020) // @paramdefine fHidden field is valid
#define SIM_PARAM_PBEX_GROUPIDCOUNT         (0x00000040) // @paramdefine dwGroupIdCount field is valid
#define SIM_PARAM_PBEX_GROUPID              (0x00000080) // @paramdefine rgdwGroupId field is valid
#define SIM_PARAM_PBEX_UID                  (0x00000100) // @paramdefine dwUid field is valid
#define SIM_PARAM_PBEX_ADDITIONALNUMCOUNT   (0x00000200) // @paramdefine dwAdditionalNumCount field is valid
#define SIM_PARAM_PBEX_ADDITIONALNUMBERS    (0x00000400) // @paramdefine lpAdditionalNumbers field is valid
#define SIM_PARAM_PBEX_EMAILCOUNT           (0x00000800) // @paramdefine dwEmailCount field is valid
#define SIM_PARAM_PBEX_EMAILADDRESSES       (0x00001000) // @paramdefine lpEmailAddresses field is valid
#define SIM_PARAM_PBEX_INDEX                (0x00002000) // @paramdefine dwIndex field is valid
#define SIM_PARAM_PBEX_ALL                  (0x00003FFF) // @paramdefine All fields are valid

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @params SIMMESSAGE
//
// @comm None
//
// -----------------------------------------------------------------------------
#define SIM_PARAM_MSG_ADDRESS           (0x00000001)     // @paramdefine lpszAddress field is valid
#define SIM_PARAM_MSG_ADDRESS_TYPE      (0x00000002)     // @paramdefine dwAddressType field is valid
#define SIM_PARAM_MSG_NUMPLAN           (0x00000004)     // @paramdefine dwNumPlan field is valid
#define SIM_PARAM_MSG_RECEIVE_TIME      (0x00000008)     // @paramdefine stReceiveTime field is valid
#define SIM_PARAM_MSG_HEADER            (0x00000010)     // @paramdefine rgbHeader field is valid
#define SIM_PARAM_MSG_HEADER_LENGTH     (0x00000020)     // @paramdefine cbHdrLength field is valid
#define SIM_PARAM_MSG_MESSAGE           (0x00000040)     // @paramdefine lpszMessage field is valid
#define SIM_PARAM_MSG_ALL               (0x0000007f)     // @paramdefine All fields are valid

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @params SIMCAPS
//
// @comm None
//
// -----------------------------------------------------------------------------
#define SIM_PARAM_CAPS_PBSTORAGES           (0x00000001)     // @paramdefine dwPBStorages field is valid
#define SIM_PARAM_CAPS_PBEMAXADDRESSLENGTH  (0x00000002)     // @paramdefine dwPBEMaxAddressLength field is valid
#define SIM_PARAM_CAPS_PBEMAXTEXTLENGTH     (0x00000004)     // @paramdefine dwPBEMaxTextLength field is valid
#define SIM_PARAM_CAPS_PBEMININDEX          (0x00000008)     // @paramdefine dwMinPBIndex field is valid
#define SIM_PARAM_CAPS_PBEMAXINDEX          (0x00000010)     // @paramdefine dwMaxPBIndex field is valid
#define SIM_PARAM_CAPS_LOCKFACILITIES       (0x00000020)     // @paramdefine dwLockFacilities field is valid
#define SIM_PARAM_CAPS_LOCKINGPWDLENGTH     (0x00000040)     // @paramdefine dwNumLockingPwdLengths and rgLockingPwdLengths fields are valid
#define SIM_PARAM_CAPS_READMSGSTORAGES      (0x00000080)     // @paramdefine dwReadMsgStorages field is valid
#define SIM_PARAM_CAPS_WRITEMSGSTORAGES     (0x00000100)     // @paramdefine dwWriteMsgStorages field is valid
#define SIM_PARAM_CAPS_ALL                  (0x000001ff)     // @paramdefine All fields are valid

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @params SIMPHONEBOOKCAPS
//
// @comm None
//
// -----------------------------------------------------------------------------
#define SIM_PARAM_PBCAPS_STORAGES                     (0x00000001)    // @paramdefine dwStorages field is valid
#define SIM_PARAM_PBCAPS_MININDEX                     (0x00000002)    // @paramdefine dwMinIndex field is valid
#define SIM_PARAM_PBCAPS_MAXINDEX                     (0x00000004)    // @paramdefine dwMaxIndex field is valid
#define SIM_PARAM_PBCAPS_MAXADDRESSLENGTH             (0x00000008)    // @paramdefine dwMaxAddressLength field is valid
#define SIM_PARAM_PBCAPS_MAXTEXTLENGTH                (0x00000010)    // @paramdefine dwMaxTextLength field is valid
#define SIM_PARAM_PBCAPS_MAXSECONDNAMELENGTH          (0x00000020)    // @paramdefine dwMaxSecondNameLength field is valid
#define SIM_PARAM_PBCAPS_MAXADDITIONALNUMBERLENGTH    (0x00000040)    // @paramdefine dwMaxAdditionalNumberLength field is valid
#define SIM_PARAM_PBCAPS_MAXEMAILADDRESSLENGTH        (0x00000080)    // @paramdefine dwMaxEmailAddressLength field is valid
#define SIM_PARAM_PBCAPS_MAXGROUPTAGLENGTH            (0x00000100)    // @paramdefine dwGroupTagLength field is valid
#define SIM_PARAM_PBCAPS_MAXADDITIONALNUMBERTAGLENGTH (0x00000200)    // @paramdefine dwAdditionalNumberTagLength field is valid
#define SIM_PARAM_PBCAPS_ADDITIONALNUMBERCOUNT        (0x00000400)    // @paramdefine dwAdditionalNumberCount field is valid
#define SIM_PARAM_PBCAPS_EMAILADDRESSCOUNT            (0x00000800)    // @paramdefine dwEmailAddressCount field is valid
#define SIM_PARAM_PBCAPS_MAXGROUPTAGS                 (0x00001000)    // @paramdefine dwMaxGroupTags field is valid
#define SIM_PARAM_PBCAPS_MAXADDITIONALNUMBERTAGS      (0x00002000)    // @paramdefine dwMaxAddtionalNumberTags field is valid
#define SIM_PARAM_PBCAPS_HIDDEN                       (0x00004000)    // @paramdefine fHidden field is valid
#define SIM_PARAM_PBCAPS_UID                          (0x00008000)    // @paramdefine fUid field is valid
#define SIM_PARAM_PBCAPS_MAXGROUPIDCOUNT              (0x00010000)    // @paramdefine dwMaxGroupIdCount is valid
#define SIM_PARAM_PBCAPS_ALL                          (0x0001FFFF)    // @paramdefine All fields are valid

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @params SIMRECORDINFO
//
// @comm None
//
// -----------------------------------------------------------------------------
#define SIM_PARAM_RECORDINFO_RECORDTYPE (0x00000001)     // @paramdefine dwRecordType field is valid
#define SIM_PARAM_RECORDINFO_ITEMCOUNT  (0x00000002)     // @paramdefine dwItemCount field is valid
#define SIM_PARAM_RECORDINFO_SIZE       (0x00000004)     // @paramdefine dwSize field is valid
#define SIM_PARAM_RECORDINFO_ALL        (0x00000007)     // @paramdefine All fields are valid

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @params SIMFILEREFRESH
//
// @comm None
//
// -----------------------------------------------------------------------------
#define SIM_PARAM_FILEREFRESH_FLAGS      (0x00000001)     // @paramdefine dwFlags field is valid
#define SIM_PARAM_FILEREFRESH_FILECOUNT  (0x00000002)     // @paramdefine dwFileCount field is valid
#define SIM_PARAM_FILEREFRESH_FILEARRAY  (0x00000004)     // @paramdefine rgdwAddress field is valid
#define SIM_PARAM_FILEREFRESH_ALL        (0x00000007)     // @paramdefine All fields are valid


// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @params SIMPHONEBOOKADDITIONALNUMBER
//
// @comm None
//
// -----------------------------------------------------------------------------
#define SIM_PARAM_ADDITIONALNUM_ADDRESS      (0x00000001)  // @paramdefine lpszAddress field is valid
#define SIM_PARAM_ADDITIONALNUM_ADDRESS_TYPE (0x00000002)  // @paramdefine dwAddressType field is valid
#define SIM_PARAM_ADDITIONALNUM_NUMPLAN      (0x00000004)  // @paramdefine dwNumPlan field is valid
#define SIM_PARAM_ADDITIONALNUM_NUMID        (0x00000008)  // @paramdefine dwNumId field is valid
#define SIM_PARAM_ADDITIONALNUM_ALL          (0x0000000F)  // @paramdefine ALL fields are valid

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @params SIMPHONEBOOKEMAILADDRESS
//
// @comm None
//
// -----------------------------------------------------------------------------
#define SIM_PARAM_EMAIL_ADDRESS             (0x00000001) // @paramdefine lpszAddress field is valid
#define SIM_PARAM_EMAIL_ALL                 (0x00000001) // @paramdefine ALL fields are valid


// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @constants Phonebook Storage | Phone book storage locations
//
// @comm None
//
// -----------------------------------------------------------------------------
#define SIM_PBSTORAGE_EMERGENCY         (0x00000001)      // @constdefine Emergency dial list
#define SIM_PBSTORAGE_FIXEDDIALING      (0x00000002)      // @constdefine SIM fixed dialing list
#define SIM_PBSTORAGE_LASTDIALING       (0x00000004)      // @constdefine SIM last dialing list
#define SIM_PBSTORAGE_OWNNUMBERS        (0x00000008)      // @constdefine SIM ownnumbers lists
#define SIM_PBSTORAGE_SIM               (0x00000010)      // @constdefine General SIM Storage
#define SIM_PBSTORAGE_SERVICEDIALING 	(0x00000020)	  // @constdefine SDN SIM Storage
#define SIM_NUMPBSTORAGES               6                 // @constdefine Number of phonebook storages

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @constants Phonebook Storage | Phone book storage locations
//
// @comm None
//
// -----------------------------------------------------------------------------
#define SIM_PBTAG_GROUP                 (1)              // @constdefine Group name tag
#define SIM_PBTAG_NUMBER                (2)              // @constdefine Additional Number tag


// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @constants SIM DevCaps | Device Capabilities
//
// @comm None
//
// -----------------------------------------------------------------------------
#define SIM_CAPSTYPE_PBENTRYLENGTH      (0x00000001)      // @constdefine Phonebook entry lengths
#define SIM_CAPSTYPE_PBSTORELOCATIONS   (0x00000002)      // @constdefine Phonebook storage locations
#define SIM_CAPSTYPE_LOCKFACILITIES     (0x00000004)      // @constdefine Lock facilities
#define SIM_CAPSTYPE_PBINDEXRANGE       (0x00000008)      // @constdefine Valid phonebook entry indexes
#define SIM_CAPSTYPE_LOCKINGPWDLENGTHS  (0x00000010)      // @constdefine Locking password lengths
#define SIM_CAPSTYPE_MSGMEMORYLOCATIONS (0x00000020)      // @constdefine Message memory locations
#define SIM_CAPSTYPE_ALL                (0x0000003F)      // @constdefine All of the above

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @constants SMS Storage | SMS storage locations
//
// @comm None
//
// -----------------------------------------------------------------------------
#define SIM_SMSSTORAGE_BROADCAST        (0x00000001)      // @constdefine Broadcast message storage location
#define SIM_SMSSTORAGE_SIM              (0x00000002)      // @constdefine SIM storage location
#define SIM_NUMSMSSTORAGES              2                 // @constdefine Number of message storage locations

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @constants Address Type | Defines different address types
//
// @comm None
//
// -----------------------------------------------------------------------------
#define SIM_ADDRTYPE_UNKNOWN            (0x00000000)      // @constdefine Unknown
#define SIM_ADDRTYPE_INTERNATIONAL      (0x00000001)      // @constdefine International number
#define SIM_ADDRTYPE_NATIONAL           (0x00000002)      // @constdefine National number
#define SIM_ADDRTYPE_NETWKSPECIFIC      (0x00000003)      // @constdefine Network specific number
#define SIM_ADDRTYPE_SUBSCRIBER         (0x00000004)      // @constdefine Subscriber number (protocol-specific)
#define SIM_ADDRTYPE_ALPHANUM           (0x00000005)      // @constdefine Alphanumeric address
#define SIM_ADDRTYPE_ABBREV             (0x00000006)      // @constdefine Abbreviated number

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @constants Numbering Plan | Defines different numbering plans for SIM_ADDRTYPE_UNKNOWN,
//            SIM_ADDRTYPE_INTERNATIONAL, and SIM_ADDRTYPE_NATIONAL
//
// @comm None
//
// -----------------------------------------------------------------------------
#define SIM_NUMPLAN_UNKNOWN             (0x00000000)      // @constdefine Unknown
#define SIM_NUMPLAN_TELEPHONE           (0x00000001)      // @constdefine ISDN/telephone numbering plan (E.164/E.163)
#define SIM_NUMPLAN_DATA                (0x00000002)      // @constdefine Data numbering plan (X.121)
#define SIM_NUMPLAN_TELEX               (0x00000003)      // @constdefine Telex numbering plan
#define SIM_NUMPLAN_NATIONAL            (0x00000004)      // @constdefine National numbering plan
#define SIM_NUMPLAN_PRIVATE             (0x00000005)      // @constdefine Private numbering plan
#define SIM_NUMPLAN_ERMES               (0x00000006)      // @constdefine ERMES numbering plan (ETSI DE/PS 3 01-3)

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @constants Phone Locked | Indicates if the phone is currently locked (i.e.
//            awaiting password) and what password to enter
//
// @comm None
//
// -----------------------------------------------------------------------------
#define SIM_LOCKEDSTATE_UNKNOWN         (0x00000000)      // @constdefine Locking state is unknown
#define SIM_LOCKEDSTATE_READY           (0x00000001)      // @constdefine Not awaiting a password (unlocked)
#define SIM_LOCKEDSTATE_SIM_PIN         (0x00000002)      // @constdefine Awaiting the SIM PIN
#define SIM_LOCKEDSTATE_SIM_PUK         (0x00000003)      // @constdefine Awaiting the SIM PUK
#define SIM_LOCKEDSTATE_PH_SIM_PIN      (0x00000004)      // @constdefine Awaiting the Phone to SIM Personalization PIN
#define SIM_LOCKEDSTATE_PH_FSIM_PIN     (0x00000005)      // @constdefine Awaiting the Phone to first SIM Personalization PIN
#define SIM_LOCKEDSTATE_PH_FSIM_PUK     (0x00000006)      // @constdefine Awaiting the Phone to first SIM Personalization PUK
#define SIM_LOCKEDSTATE_SIM_PIN2        (0x00000007)      // @constdefine Awaiting the SIM PIN2
#define SIM_LOCKEDSTATE_SIM_PUK2        (0x00000008)      // @constdefine Awaiting the SIM PUK2
#define SIM_LOCKEDSTATE_PH_NET_PIN      (0x00000009)      // @constdefine Awaiting the Network Personalization PIN
#define SIM_LOCKEDSTATE_PH_NET_PUK      (0x0000000a)      // @constdefine Awaiting the Network Personalization PUK
#define SIM_LOCKEDSTATE_PH_NETSUB_PIN   (0x0000000b)      // @constdefine Awaiting the Network Subset Personalization PIN
#define SIM_LOCKEDSTATE_PH_NETSUB_PUK   (0x0000000c)      // @constdefine Awaiting the Network Subset Personalization PUK
#define SIM_LOCKEDSTATE_PH_SP_PIN       (0x0000000d)      // @constdefine Awaiting the Service Provider Personalization PIN
#define SIM_LOCKEDSTATE_PH_SP_PUK       (0x0000000e)      // @constdefine Awaiting the Service Provider Personalization PUK
#define SIM_LOCKEDSTATE_PH_CORP_PIN     (0x0000000f)      // @constdefine Awaiting the Corporate Personalization PIN
#define SIM_LOCKEDSTATE_PH_CORP_PUK     (0x00000010)      // @constdefine Awaiting the Corporate Personalization PUK

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @constants Phonebook Misc | Special phonebook constants
//
// @comm None
//
// -----------------------------------------------------------------------------
#define SIM_PBINDEX_FIRSTAVAILABLE      (0xffffffff)      // @constdefine Use first phonebook storage entry available

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @constants Phone Locking | Indicates the phone's locking behavior
//
// @comm None
//
// -----------------------------------------------------------------------------
#define SIM_LOCKFACILITY_CNTRL              (0x00000001)      // @constdefine Lock control curface
#define SIM_LOCKFACILITY_PH_SIM             (0x00000002)      // @constdefine Lock phone to SIM card
#define SIM_LOCKFACILITY_PH_FSIM            (0x00000004)      // @constdefine Lock phone to first SIM card
#define SIM_LOCKFACILITY_SIM                (0x00000008)      // @constdefine Lock SIM card
#define SIM_LOCKFACILITY_SIM_PIN2           (0x00000010)      // @constdefine Lock SIM card
#define SIM_LOCKFACILITY_SIM_FIXEDIALING    (0x00000020)      // @constdefine SIM fixed dialing memory
#define SIM_LOCKFACILITY_NETWORKPERS        (0x00000040)      // @constdefine Network personalization
#define SIM_LOCKFACILITY_NETWORKSUBPERS     (0x00000080)      // @constdefine Network subset personalization
#define SIM_LOCKFACILITY_SERVICEPROVPERS    (0x00000100)      // @constdefine Service provider personalization
#define SIM_LOCKFACILITY_CORPPERS           (0x00000200)      // @constdefine Corporate personalization
#define SIM_NUMLOCKFACILITIES               10                // @constdefine Number of locking facilities

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @constants SIM Record | Different SIM file types
//
// @comm None
//
// -----------------------------------------------------------------------------
#define SIM_RECORDTYPE_UNKNOWN          (0x00000000)         // @constdefine An unknown file type
#define SIM_RECORDTYPE_TRANSPARENT      (0x00000001)         // @constdefine A single veriable lengthed record
#define SIM_RECORDTYPE_CYCLIC           (0x00000002)         // @constdefine A cyclic set of records, each of the same length
#define SIM_RECORDTYPE_LINEAR           (0x00000003)         // @constdefine A linear set of records, each of the same length
#define SIM_RECORDTYPE_MASTER           (0x00000004)         // @constdefine Every SIM has a single master record, effectively the head node
#define SIM_RECORDTYPE_DEDICATED        (0x00000005)         // @constdefine Effectively a "directory" file which is a parent of other records

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @constants SIM Record Refresh | Different ways of being notified that SIM
//            have been updated
//
// @comm None
//
// -----------------------------------------------------------------------------
#define SIMFILE_FULLFILECHANGE          (0x00000001)         // @constdefine All files have been changed
#define SIMFILE_FILECHANGE              (0x00000002)         // @constdefine Only a few files have been changed
#define SIMFILE_SIMINIT                 (0x00000004)         // @constdefine SIM Initiailization
#define SIMFILE_SIMRESET                (0x00000008)         // @constdefine Reset the SIM


// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @constants Max_Length | Maximum length constants
//
// @comm None
//
// -----------------------------------------------------------------------------
#define MAX_LENGTH_ADDRESS              (256)                // @constdefine Maximum length of an address
#define MAX_LENGTH_PHONEBOOKENTRYTEXT   (256)                // @constdefine Maximum length of text in a phonebook entry
#define MAX_LENGTH_HEADER               (256)                // @constdefine Maximum length of a SMS header
#define MAX_LENGTH_MESSAGE              (256)                // @constdefine Maximum length of a SMS message
#define MAX_FILES                       (32)                 // @constdefine Maximum number of files in a file change list
#define MAX_NUM_GROUPS                  (10)                 // @constdefine Maximum number of group ID's in a phonebook entry

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @struct HSIM | Handle to a SIM
//
// @comm None
//
// -----------------------------------------------------------------------------
typedef HANDLE HSIM, *LPHSIM;

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @struct SIMPHONEBOOKENTRY | A SIM phonebook entry
//
// @comm None
//
// -----------------------------------------------------------------------------
typedef struct simphonebookentry_tag {
    DWORD cbSize;                                          // @field Size of the structure in bytes
    DWORD dwParams;                                        // @field Indicates valid parameter values
    TCHAR lpszAddress[MAX_LENGTH_ADDRESS];                 // @field The actual phone number
    DWORD dwAddressType;                                   // @field A SIM_ADDRTYPE_* constant
    DWORD dwNumPlan;                                       // @field A SIM_NUMPLAN_* constant
    TCHAR lpszText[MAX_LENGTH_PHONEBOOKENTRYTEXT];         // @field Text associated with the entry
} SIMPHONEBOOKENTRY, *LPSIMPHONEBOOKENTRY;

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @struct LPSIMPHONEBOOKADDITIONALNUMBER | An additional phone number item
//         associated with a SIMPHONEBOOKENTRYEX structure.
//
// @comm None
//
// -----------------------------------------------------------------------------
typedef struct simphonebookadditionalnumber_tag {
    DWORD cbSize;                                          // @field Size of buffer in bytes.
    DWORD dwParams;                                        // @field @field Indicates valid parameter values in structure.
    TCHAR lpszAddress[MAX_LENGTH_ADDRESS];                 // @field The actual phone number
    DWORD dwAddressType;                                   // @field A SIM_ADDRTYPE_* constant
    DWORD dwNumPlan;                                       // @field A SIM_NUMPLAN_* constant
    DWORD dwNumId;                                         // @field The additional number type tag
} SIMPHONEBOOKADDITIONALNUMBER, *LPSIMPHONEBOOKADDITIONALNUMBER;

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @struct SIMPHONEBOOKEMAILADDRESS | An email address item associated with a
//         SIMPHONEBOOKENTRYEX structure.
//
//
// @comm None
//
// -----------------------------------------------------------------------------
typedef struct simphonebookemailaddress_tag {
    DWORD cbSize;                                          // @field Size of buffer in bytes.
    DWORD dwParams;                                        // @field Indicates valid parameter values in structure.
    TCHAR lpszAddress[MAX_LENGTH_PHONEBOOKENTRYTEXT];      // @field A string containing the email address. For MAX_LENGTH_PHONEBOOKTEXT see SIMPHONEBOOKENTRYEX lpszText.
} SIMPHONEBOOKEMAILADDRESS, *LPSIMPHONEBOOKEMAILADDRESS;

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @struct SIMPHONEBOOKENTRYEX | An extended SIM phonebook entry
//
// @comm None
//
// -----------------------------------------------------------------------------
typedef struct simphonebookentryex_tag {
    DWORD cbSize;                                          // @field Size of the structure in bytes
    DWORD dwParams;                                        // @field Indicates valid parameter values
    TCHAR lpszAddress[MAX_LENGTH_ADDRESS];                 // @field The actual phone number
    DWORD dwAddressType;                                   // @field A SIM_ADDRTYPE_* constant
    DWORD dwNumPlan;                                       // @field A SIM_NUMPLAN_* constant
    TCHAR lpszText[MAX_LENGTH_PHONEBOOKENTRYTEXT];         // @field Text associated with the entry
    TCHAR lpszSecondName[MAX_LENGTH_PHONEBOOKENTRYTEXT];   // @field Second text field associated with the entry
    DWORD dwIndex;                                         // @field Index of the entry
    BOOL  fHidden;                                         // @field Indicates a hidden entry
    DWORD dwGroupIdCount;                                  // @field Count of valid group ID's
    DWORD rgdwGroupId[MAX_NUM_GROUPS];                     // @field Array of group ID's
    DWORD dwUid;                                           // @field Unique identifier of entry
    DWORD dwAdditionalNumberCount;                         // @field Number of additional numbers in the array.
    LPSIMPHONEBOOKADDITIONALNUMBER lpAdditionalNumbers;    // @field Pointer to an array of SIMPHONEBOOKADDITIONALNUMBER structures
    DWORD dwEmailCount;                                    // @field Number of email addresses in the array.
    LPSIMPHONEBOOKEMAILADDRESS lpEmailAddresses;           // @field Pointer to an array of SIMPHONEBOOKEMAILADDRESS structures
} SIMPHONEBOOKENTRYEX, *LPSIMPHONEBOOKENTRYEX;

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @struct SIMMESSAGE | A SIM message entry
//
// @comm None
//
// -----------------------------------------------------------------------------
typedef struct simmessage_tag {
    DWORD cbSize;                           // @field Size of the structure in bytes
    DWORD dwParams;                         // @field Indicates valid parameter values
    TCHAR lpszAddress[MAX_LENGTH_ADDRESS];  // @field The actual phone number
    DWORD dwAddressType;                    // @field A SIM_ADDRTYPE_* constant
    DWORD dwNumPlan;                        // @field A SIM_NUMPLAN_* constant
    SYSTEMTIME stReceiveTime;               // @field Timestamp for the incoming message
    DWORD cbHdrLength;                      // @field Header length in bytes
    BYTE rgbHeader[MAX_LENGTH_HEADER];      // @field The actual header data
    TCHAR lpszMessage[MAX_LENGTH_MESSAGE];  // @field The actual message data
} SIMMESSAGE, FAR *LPSIMMESSAGE;

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @struct SIMLOCKINGPWDLENGTH | Minimum password length
//
// @comm None
//
// -----------------------------------------------------------------------------
typedef struct simlockingpwdlength
{
    DWORD dwFacility;                      // @field The locking facility
    DWORD dwPasswordLength;                // @field The minimum password length
} SIMLOCKINGPWDLENGTH, FAR *LPSIMLOCKINGPWDLENGTH;

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @struct SIMCAPS | Capabilities of the SIM
//
// @comm None
//
// -----------------------------------------------------------------------------
typedef struct simcaps_tag {
    DWORD cbSize;                           // @field Size of the structure in bytes
    DWORD dwParams;                         // @field Indicates valid parameter values
    DWORD dwPBStorages;                     // @field Supported phonebook storages
    DWORD dwMinPBIndex;                     // @field Minimum phonebook storages
    DWORD dwMaxPBIndex;                     // @field Maximum phonebook storages
    DWORD dwMaxPBEAddressLength;            // @field Maximum address length of phonebook entries
    DWORD dwMaxPBETextLength;               // @field Maximum text length of phonebook entries
    DWORD dwLockFacilities;                 // @field Supported locking facilities
    DWORD dwReadMsgStorages;                // @field Supported read message stores
    DWORD dwWriteMsgStorages;               // @field Supported write message stores
    DWORD dwNumLockingPwdLengths;           // @field Number of entries in rgLockingPwdLengths
    SIMLOCKINGPWDLENGTH rgLockingPwdLengths[SIM_NUMLOCKFACILITIES]; // @field Password lengths for each facility
} SIMCAPS, FAR *LPSIMCAPS;

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @struct SIMPHONEBOOKCAPS | Capabilities of the SIM Phonebook
//
// @comm None
//
// -----------------------------------------------------------------------------
typedef struct simphonebookcaps_tag {
    DWORD cbSize;                           // @field Size of the structure in bytes
    DWORD dwParams;                         // @field Indicates valid parameter values
    DWORD dwStorages;                       // @field Supported phonebook storages
    DWORD dwMinIndex;                       // @field Minimum phonebook storages
    DWORD dwMaxIndex;                       // @field Maximum phonebook storages
    DWORD dwMaxAddressLength;               // @field Maximum address length of phonebook entries
    DWORD dwMaxTextLength;                  // @field Maximum text length of phonebook entries
    DWORD dwMaxSecondNameLength;            // @field Maximum text length for the second name
    DWORD dwMaxAdditionalNumberLength;      // @field Maximum text length for additional numbers
    DWORD dwMaxEmailAddressLength;          // @field Maximum text length for email addresses
    DWORD dwMaxGroupTagLength;              // @field Maximum text length for group tag text
    DWORD dwMaxAdditionalNumberTagLength;   // @field Maximum text length for additional number tag text
    DWORD dwAdditionalNumberCount;          // @field Count of additional numbers supported per entry
    DWORD dwEmailAddressCount;              // @field Number of emails addresses supported per entry
    DWORD dwMaxGroupTags;                   // @field Number of Group tags available
    DWORD dwMaxAdditionalNumberTags;        // @field Number of additional number tags available
    BOOL  fHidden;                          // @field Hidden flag is available
    BOOL  fUid;                             // @field Unique identifier is available
    DWORD dwMaxGroupIdCount;                // @field Number of groups supported per entry
} SIMPHONEBOOKCAPS, FAR *LPSIMPHONEBOOKCAPS;

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @struct SIMRECORDINFO | Information about a particular SIM file
//
// @comm None
//
// -----------------------------------------------------------------------------
typedef struct simrecordinfo_tag {
    DWORD cbSize;                           // @field Size of the structure in bytes
    DWORD dwParams;                         // @field Indicates valid parameter values
    DWORD dwRecordType;                     // @field SIM_RECORDTYPE_* Constant
    DWORD dwItemCount;                      // @field Number of items in the record
    DWORD dwSize;                           // @field Size in bytes of each item
} SIMRECORDINFO, FAR *LPSIMRECORDINFO;

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @struct SIMFILEREFRESH | Information about which file(s) have been updated
//
// @comm None
//
// -----------------------------------------------------------------------------
typedef struct simfilerefresh_tag {
    DWORD cbSize;                           // @field Size of the structure in bytes
    DWORD dwParams;                         // @field Indicates valid parameter values
    DWORD dwFlags;                          // @field Combination of SIMFILE_* constants
    DWORD dwFileCount;                      // @field Number of files in the update list
    DWORD rgdwAddress[MAX_FILES];           // @field Array of files
} SIMFILEREFRESH, FAR *LPSIMFILEREFRESH;

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @struct SIMPBECHANGE | Information about which SIM Phonebook entries have changed
//
// @comm None
//
// -----------------------------------------------------------------------------
typedef struct simpbechange_tag {
    DWORD dwEntry;                          // @field The index of the entry that has changed
    DWORD dwStorage;                        // @field SIM_PBSTORAGE_* constant detailing which phonebook this entry is in
} SIMPBECHANGE, FAR *LPSIMPBECHANGE;

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @struct SIMMESSAGECHANGE | Information about which SMS messages on the SIM have changed
//
// @comm None
//
// -----------------------------------------------------------------------------
typedef struct simmessagechange_tag {
    DWORD dwEntry;                          // @field The index of the entry that has changed
    DWORD dwStorage;                        // @field SIM_SMSSTORAGE_* constant details which storage location this entry is in
} SIMMESSAGECHANGE, FAR *LPSIMMESSAGECHANGE;

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @struct SIMSMSSTORAGESTATUS | Status information about when a SIM SMS
//         storage location has become full or has space available.
//
// @comm None
//
// -----------------------------------------------------------------------------
typedef struct simsmsstoragestatus_tag {
    DWORD dwLocation;                       // contains the SIM_SMSSTORAGE_* value
    BOOL fFull;                             // True, the store has become full. False, at least one index has been freed.
} SIMSMSSTORAGESTATUS;


// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @func This is the callback function prototype used by SIM Manager when
//       sending notifications.
//
// @comm None
//
// -----------------------------------------------------------------------------
typedef void (*SIMCALLBACK)(
    DWORD dwNotifyCode,                     // @parm Indicates type of notification received
    const void* pData,                      // @parm Points to data structure specific to the notification
    DWORD dwDataSize,                       // @parm Size of data structure in bytes
    DWORD dwParam);                         // @parm Parameter passed to simInititialize


// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @func This function must be called by applications in order to use any of the
//       functions in this API.
//
// @comm Passing in a function callback is required only for applications that
//       wish to obtain notifications.
//
// -----------------------------------------------------------------------------
HRESULT SimInitialize(
    DWORD dwFlags,                          // @parm Indicates which notifications to receive
    SIMCALLBACK lpfnCallBack,               // @parm Function callback for notifications, may be NULL if notifications are not desired
    DWORD dwParam,                          // @parm Parameter to pass on each notification function call, may be NULL
    LPHSIM lphSim                           // @parm Points to a HSIM handle to use on subsequent function calls
);


// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @func This function deinitializes an HSIM handle.
//
// @comm None
//
// -----------------------------------------------------------------------------
HRESULT SimDeinitialize(
    HSIM hSim                               // @parm A valid HSIM handle to deinitialize
);

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @func Gets the device capabilities of the SIM.
//
// @comm None
//
// -----------------------------------------------------------------------------
HRESULT SimGetDevCaps(
    HSIM hSim,                              // @parm Points to a valid HSIM handle
    DWORD dwCapsType,                       // @parm Which device capabilities are we interested in?
    LPSIMCAPS lpSimCaps                     // @parm Capabilities structure
);

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @func Reads a phonebook entry off the SIM card.
//
// @comm None
//
// -----------------------------------------------------------------------------
HRESULT SimReadPhonebookEntry(
    HSIM hSim,                              // @parm Points to a valid HSIM handle
    DWORD dwLocation,                       // @parm A SIMPBSTORAGE_* Constant
    DWORD dwIndex,                          // @parm Index of the entry to retrieve
    LPSIMPHONEBOOKENTRY lpPhonebookEntry    // @parm Points to a phonebook entry structure
);

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @func Reads extended phonebook entries from the specified range of indices
//       off the SIM card.
//
// @comm None
//
// -----------------------------------------------------------------------------
HRESULT SimReadPhonebookEntries(
    HSIM hSim,                              // @parm Points to a valid HSIM handle
    DWORD dwLocation,                       // @parm A SIMPBSTORAGE_* Constant
    DWORD dwStartIndex,                     // @parm Starting index in the range of entries to retrieve
    LPDWORD lpdwCount,                      // @parm On input, the total number of entries to read starting from dwStartIndex. On output (if the function succeeds), the actual number of SIMPHONEBOOKENTRYEX structures returned in lpEntries
    LPSIMPHONEBOOKENTRYEX lpEntries,        // @parm Buffer to hold an array of SIMPHONEBOOKENTRYEX structures and associated variable data
    LPDWORD lpdwBufferSize                  // @parm On input, the number of bytes contained in the buffer pointed to by lpEntries. On output, if the function fails it contains the minimum number of bytes to pass for the lpEntries parameter to retrieve the entries.
);

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @func Gets the status of a phonebook location.
//
// @comm None
//
// -----------------------------------------------------------------------------
HRESULT SimGetPhonebookStatus(
    HSIM hSim,                              // @parm Points to a valid HSIM handle
    DWORD dwLocation,                       // @parm A SIMPBSTORAGE_* Constant
    LPDWORD lpdwUsed,                       // @parm Number of used locations
    LPDWORD lpdwTotal                       // @parm Total number of locations
);

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @func Writes a phonebook entry to the SIM card.
//
// @comm None
//
// -----------------------------------------------------------------------------
HRESULT SimWritePhonebookEntry(
    HSIM hSim,                              // @parm Points to a valid HSIM handle
    DWORD dwLocation,                       // @parm A SIMPBSTORAGE_* Constant
    DWORD dwIndex,                          // @parm Index of the entry to retrieve (may be SIM_PBINDEX_FIRSTAVAILABLE)
    LPSIMPHONEBOOKENTRY lpPhonebookEntry    // @parm Points to a phonebook entry structure
);

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @func Writes a phonebook entry to the SIM card.
//
// @comm None
//
// -----------------------------------------------------------------------------
HRESULT SimWritePhonebookEntryEx(
    HSIM hSim,                              // @parm Points to a valid HSIM handle
    DWORD dwLocation,                       // @parm A SIMPBSTORAGE_* Constant
    DWORD dwIndex,                          // @parm Index of the entry to retrieve (may be SIM_PBINDEX_FIRSTAVAILABLE)
    LPSIMPHONEBOOKENTRYEX lpPhonebookEntry  // @parm Points to a extended phonebook entry structure
);

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @func Deletes a phonebook entry from the SIM card.
//
// @comm None
//
// -----------------------------------------------------------------------------
HRESULT SimDeletePhonebookEntry(
    HSIM hSim,                              // @parm Points to a valid HSIM handle
    DWORD dwLocation,                       // @parm A SIMPBSTORAGE_* Constant
    DWORD dwIndex                           // @parm Index of the entry to retrieve
);

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @func Checks if the SIM is currently awaiting a password.
//
// @comm This is called when powering on the phone.
//
// -----------------------------------------------------------------------------
HRESULT SimGetPhoneLockedState(
    HSIM hSim,                              // @parm Points to a valid HSIM handle
    LPDWORD lpdwLockedState                 // @parm Points to a SIM_LOCKEDSTATE_* constant
);

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @func Sends a password to unlock the phone.
//
// @comm None
//
// -----------------------------------------------------------------------------
HRESULT SimUnlockPhone(
    HSIM hSim,                              // @parm Points to a valid HSIM handle
    LPTSTR lpszPassword,                    // @parm Points to password string
    LPTSTR lpszNewPin                       // @parm Some locked states require a second password (e.g. PUK requires a new PIN to replace the old, presumably forgotten PIN)
);

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @func Gets the locking status of the phone.
//
// @comm None
//
// -----------------------------------------------------------------------------
HRESULT SimGetLockingStatus(
    HSIM hSim,                              // @parm Points to a valid HSIM handle
    DWORD dwLockingFacility,                // @parm A SIMLOCKFACILITY_* constant
    LPTSTR lpszPassword,                    // @parm Some facilities require a password
    BOOL *pfEnabled                         // @parm Enabled or diabled
);

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @func Sets the locking status of the phone.
//
// @comm None
//
// -----------------------------------------------------------------------------
HRESULT SimSetLockingStatus(
    HSIM hSim,                              // @parm Points to a valid HSIM handle
    DWORD dwLockingFacility,                // @parm A SIMLOCKFACILITY_* constant
    LPTSTR lpszPassword,                    // @parm Some facilities require a password
    BOOL fEnabled                           // @parm Enable or diable
);

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @func Changes a locking password.
//
// @comm None
//
// -----------------------------------------------------------------------------
HRESULT SimChangeLockingPassword(
    HSIM hSim,                              // @parm Points to a valid HSIM handle
    DWORD dwLockingFacility,                // @parm A SIMLOCKFACILITY_* constant
    LPTSTR lpszOldPassword,                 // @parm The old password
    LPTSTR lpszNewPassword                  // @parm The new password
);

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @func Gets the status of a SMS storage location.
//
// @comm None
//
// -----------------------------------------------------------------------------
HRESULT SimGetSmsStorageStatus(
    HSIM hSim,                              // @parm Points to a valid HSIM handle
    DWORD dwStorage,                        // @parm A SIM_SMSSTORAGE_* constant
    LPDWORD lpdwUsed,                       // @parm Number of used locations
    LPDWORD lpdwTotal                       // @parm Total number of locations
);

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @func Reads an SMS from a particular storage location.
//
// @comm None
//
// -----------------------------------------------------------------------------
HRESULT SimReadMessage(
    HSIM hSim,                              // @parm Points to a valid HSIM handle
    DWORD dwStorage,                        // @parm A SIM_SMSSTORAGE_* constant
    DWORD dwIndex,                          // @parm Index of the entry to retrieve
    LPSIMMESSAGE lpSimMessage               // @parm Points to an SMS message structure
);

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @func Writes an SMS to a particular storage location.
//
// @comm Note that SMS messages are always written to the first available storage location
// @comm lpdwIndex will be set to the index to which the message was written
//
// -----------------------------------------------------------------------------
HRESULT SimWriteMessage(
    HSIM hSim,                              // @parm Points to a valid HSIM handle
    DWORD dwStorage,                        // @parm A SIM_SMSSTORAGE_* constant
    LPDWORD lpdwIndex,                      // @parm Set to the index where the message was written
    LPSIMMESSAGE lpSimMessage               // @parm Points to an SMS message structure
);

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @func Deletes an SMS from a particular storage location.
//
// @comm None
//
// -----------------------------------------------------------------------------
HRESULT SimDeleteMessage(
    HSIM hSim,                              // @parm Points to a valid HSIM handle
    DWORD dwStorage,                        // @parm A SIM_SMSSTORAGE_* constant
    DWORD dwIndex                           // @parm Index of the entry to retrieve
);

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @func Reads a file form the SIM.
//
// @comm None
//
// -----------------------------------------------------------------------------
HRESULT SimReadRecord(
    HSIM hSim,                                  // @parm Points to a valid HSIM handle
    DWORD dwAddress,                            // @parm SIM address
    DWORD dwRecordType,                         // @parm A SIM_RECORDTYPE_* constant
    DWORD dwIndex,                              // @parm Applies only to SIM_RECORDTYPE_CYCLIC and SIM_RECORDTYPE_LINEAR, otherwise ignored
    __out_bcount(*lpdwBytesRead) LPBYTE lpData, // @parm Data buffer
    DWORD dwBufferSize,                         // @parm Size of data buffer
    LPDWORD lpdwBytesRead                       // @parm Number of bytes read
);

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @func Writes a file to the SIM.
//
// @comm None
//
// -----------------------------------------------------------------------------
HRESULT SimWriteRecord(
    HSIM hSim,                              // @parm Points to a valid HSIM handle
    DWORD dwAddress,                        // @parm SIM address
    DWORD dwRecordType,                     // @parm A SIM_RECORDTYPE_* constant
    DWORD dwIndex,                          // @parm Applies only to SIM_RECORDTYPE_CYCLIC and SIM_RECORDTYPE_LINEAR, otherwise ignored
    __in_bcount(dwByteCount) LPBYTE lpData, // @parm Data to write
    DWORD dwByteCount                       // @parm Number of bytes to write
);

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @func Gets information about a particular record.
//
// @comm None
//
// -----------------------------------------------------------------------------
HRESULT SimGetRecordInfo(
    HSIM hSim,                              // @parm Points to a valid HSIM handle
    DWORD dwAddress,                        // @parm SIM address
    LPSIMRECORDINFO lpSimRecordInfo         // @parm Points to a SIM record information structure
);

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @func Gets the phonebook capabilities of the SIM.
//
// @comm None
//
// -----------------------------------------------------------------------------
HRESULT SimGetPhonebookCapabilities(
    HSIM hSim,                              // @parm Points to a valid HSIM handle
    LPSIMPHONEBOOKCAPS lpCapabilities       // @parm Points to a SIMPHONEBOOKCAPS structure
);


// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @func Reads the name value for a given index and tag.
//
// @comm None
//
// -----------------------------------------------------------------------------
HRESULT SimReadPhonebookTag(
    HSIM hSim,                              // @parm Points to a valid HSIM handle
    DWORD dwTag,                            // @parm A SIM_PBTAG_* constant
    DWORD dwIndex,                          // @parm Index of tag to read
    __out_ecount(cchNameSize) LPTSTR szName,// @parm Returned name of tag
    DWORD cchNameSize                       // @parm Size of name in characters
);

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @func Writes the name value to a given index and tag.
//
// @comm None
//
// -----------------------------------------------------------------------------
HRESULT SimWritePhonebookTag(
    HSIM hSim,                              // @parm Points to a valid HSIM handle
    DWORD dwTag,                            // @parm A SIM_PBTAG_* constant
    DWORD dwIndex,                          // @parm Index to write
    LPTSTR szName                           // @parm Name for index
);


#ifdef __cplusplus
}
#endif

#endif // _SIMMGR_H_
