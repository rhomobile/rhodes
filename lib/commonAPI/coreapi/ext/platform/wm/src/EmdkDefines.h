#pragma once
//  Defines here are duplicates to those found in EMDK, declared separately to allow code compilation under Rhodes

//  From EMDK Keyboard API
typedef struct tagSTRUCT_INFO
{
	DWORD	dwAllocated;
	DWORD	dwUsed;	
} STRUCT_INFO;
typedef struct tagUNITID_EX
{
	STRUCT_INFO StructInfo;
	BYTE byUUID[24];
} UNITID_EX;
typedef UNITID_EX FAR * LPUNITID_EX;
#define	ERROR_BIT				0x80000000	// Use bit 31 to indicate errror
#define	USER_BIT				0x20000000	// Bit 29 means not Win32 error	
#define	RCM_ERROR(code)			(ERROR_BIT | USER_BIT | (WORD) code)
#define	E_RCM_SUCCESS				0
#define	E_RCM_NOTSUPPORTED			RCM_ERROR(0x0015)
//  These defines originally came from EMDK and are applicable only to Symbol devices, adding them
//  here to enable building under rhodes
typedef BOOL (WINAPI* LPFNSETKEYSTATE)(DWORD dwState, DWORD dwActiveModifer, BOOL bUpdateRegistry);	
#define SETKEYSTATE		      TEXT("SetKeyState")
#define UN_SHIFTED			0x01
#define KEYBOARD_DLL TEXT("keybddr.dll")
//  End defines from EMDK kbd API.