#pragma once
// Defines here are duplicates to those found in EMDK, declared separately to allow code compilation under Rhodes

// Structure information structure

typedef struct tagSTRUCT_INFO
{
	DWORD	dwAllocated;				// Size of allocated structure in bytes
	DWORD	dwUsed;						// Amount of structure actually used

} STRUCT_INFO;

typedef STRUCT_INFO * LPSTRUCT_INFO;

#define MAX_DEVICE_NAME					6

// FindInfo structure
typedef struct tagIMAGE_FINDINFO
{
	STRUCT_INFO		StructInfo;

	TCHAR			tszDeviceName[MAX_DEVICE_NAME];
	TCHAR			tszFriendlyName[MAX_PATH];
	TCHAR			tszRegistryBasePath[MAX_PATH];

} IMAGE_FINDINFO, FAR *LPIMAGE_FINDINFO;

// The function completed successfully.
#define	E_IMG_SUCCESS				0
#define	IMG_ACQCAP_AIMING				30007
#define	IMG_VFCAP_WINWIDTH				50010
#define	IMG_VFCAP_WINHEIGHT				50011
#define	IMG_IMGCAP_MATCHVFCAPCROP		40007
#define	IMG_VFCAP_WINHANDLE				50007
#define TWFF_JPEG						4
#define	IMG_IMGCAP_FILEFORMAT			40000
#define	IMG_ACQCAP_LAMPSTATE			30006


#define TRIGGER_ALL_MASK		( TRIGGER_STAGE1_MASK | TRIGGER_STAGE2_MASK )
#define TRIGGER_STAGE1_MASK		0x000000ff
#define TRIGGER_STAGE2_MASK		0x0000ff00
#define	E_RCM_SUCCESS				0


#define SI_USED_NONE(ptr) \
	{ (ptr)->StructInfo.dwUsed = sizeof(STRUCT_INFO); }

#define SI_ALLOC_ALL(ptr) \
	{ (ptr)->StructInfo.dwAllocated = sizeof(*(ptr)); }

#define SI_INIT(ptr) \
	{ SI_ALLOC_ALL(ptr); SI_USED_NONE(ptr); }


#define SI_FIELD_OFFSET(ptr,fld) \
	( (UINT) ((((LPBYTE)(&((ptr)->fld))) - (LPBYTE)ptr)) )

#define SI_FIELD_SIZE(ptr,fld) \
	( sizeof((ptr)->fld) )

#define SI_FIELD_EXTENT(ptr,fld) \
	( SI_FIELD_OFFSET(ptr,fld) + SI_FIELD_SIZE(ptr,fld) )

#define SI_FIELD_VALID(ptr,fld) \
  ( (ptr)->StructInfo.dwUsed >= SI_FIELD_EXTENT(ptr,fld) )

#define SI_SET_USED(ptr,fld) \
	{ if ( ! SI_FIELD_VALID(ptr,fld) ) (ptr)->StructInfo.dwUsed = SI_FIELD_EXTENT(ptr,fld); }