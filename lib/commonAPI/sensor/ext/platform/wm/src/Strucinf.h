
//--------------------------------------------------------------------
// FILENAME:			StrucInf.h
//
// Copyright(c) 1999,2000 Symbol Technologies Inc. All rights reserved.
//
// DESCRIPTION:			Constants and types used for structure information
//
// NOTES:				Public
//
// 
//--------------------------------------------------------------------


#ifndef STRUCINF_H_
#define STRUCINF_H_

#pragma message ("Using Motorola STRUCINF ...")

#ifdef __cplusplus
extern "C"
{
#endif


#ifdef _X86_
#ifndef _x86_
#define _x86_
#endif
#endif


// Structure information structure

typedef struct tagSTRUCT_INFO
{
	DWORD	dwAllocated;				// Size of allocated structure in bytes
	DWORD	dwUsed;						// Amount of structure actually used

} STRUCT_INFO;

typedef STRUCT_INFO * LPSTRUCT_INFO;


// Struct Info Macros

#define SI_FIELD_OFFSET(ptr,fld) \
	( (UINT) ((((LPBYTE)(&((ptr)->fld))) - (LPBYTE)ptr)) )

#define SI_FIELD_SIZE(ptr,fld) \
	( sizeof((ptr)->fld) )

#define SI_FIELD_EXTENT(ptr,fld) \
	( SI_FIELD_OFFSET(ptr,fld) + SI_FIELD_SIZE(ptr,fld) )

#define SI_FIELD_VALID(ptr,fld) \
  ( (ptr)->StructInfo.dwUsed >= SI_FIELD_EXTENT(ptr,fld) )

#define SI_FIELD_EXISTS(ptr,fld) \
  ( (ptr)->StructInfo.dwAllocated >= SI_FIELD_EXTENT(ptr,fld) )

#define SI_ALLOC_ALL(ptr) \
	{ (ptr)->StructInfo.dwAllocated = sizeof(*(ptr)); }

#define SI_USED_NONE(ptr) \
	{ (ptr)->StructInfo.dwUsed = sizeof(STRUCT_INFO); }

#define SI_INIT(ptr) \
	{ SI_ALLOC_ALL(ptr); SI_USED_NONE(ptr); }

#define SI_INVALID(ptr) \
	( \
		((ptr)->StructInfo.dwAllocated < sizeof(STRUCT_INFO)) \
		|| ((ptr)->StructInfo.dwAllocated < (ptr)->StructInfo.dwUsed) \
	) \

#define SI_VALID(ptr) \
	( ! SI_INVALID(ptr) )

#define SI_GET_POINTER(ptr,fld,type)\
	(SI_FIELD_VALID(ptr,fld) && (0 != ptr->fld)) ? (type)((LPBYTE)ptr + ptr->fld) : NULL

#define SI_SET_USED(ptr,fld) \
	{ if ( ! SI_FIELD_VALID(ptr,fld) ) (ptr)->StructInfo.dwUsed = SI_FIELD_EXTENT(ptr,fld); }

#define SI_SET_FIELD(ptr,fld,src) \
	{ if ( SI_FIELD_EXISTS(ptr,fld) ) { (ptr)->fld = src; SI_SET_USED(ptr,fld); } }

#define SI_GET_FIELD(ptr,fld,dst) \
	{ if ( SI_FIELD_VALID(ptr,fld) ) dst = (ptr)->fld; }

#define SI_SET_IF_CHANGED(ptr,fld,src) \
	{ if ( src != PARAMETER_NO_CHANGE ) SI_SET_FIELD((ptr),fld,src); }
 
#define SI_SET_STRING_W(ptr,fld,src) \
	{ if ( SI_FIELD_EXISTS(ptr,fld) ) \
		{ \
			int size = sizeof((ptr)->fld)/sizeof(WCHAR); \
			wcsncpy((ptr)->fld,src,size); \
			(ptr)->fld[size-1] = L'\0'; \
			SI_SET_USED(ptr,fld); \
		} \
	}

#define SI_GET_STRING_W(ptr,fld,dst) \
	{ \
		if ( SI_FIELD_VALID(ptr,fld) ) \
			wcscpy(dst,(ptr)->fld); \
	}

#define SI_SET_STRING_A(ptr,fld,src) \
	{ if ( SI_FIELD_EXISTS(ptr,fld) ) \
		{ \
			int size = sizeof((ptr)->fld); \
			strncpy((ptr)->fld,src,size); \
			(ptr)->fld[size-1] = ('\0'); \
			SI_SET_USED(ptr,fld); \
		} \
	}

#define SI_GET_STRING_A(ptr,fld,dst) \
	{ \
		if ( SI_FIELD_VALID(ptr,fld) ) \
			strcpy(dst,(ptr)->fld); \
	}

#ifdef UNICODE

#define SI_SET_STRING SI_SET_STRING_W

#define SI_GET_STRING SI_GET_STRING_W

#else

#define SI_SET_STRING SI_SET_STRING_A

#define SI_GET_STRING SI_GET_STRING_A

#endif

// Macro to copy wide character string to ANSI structure member
#define SI_SET_STRING_WA(ptr,fld,src) \
	{ if ( SI_FIELD_EXISTS(ptr,fld) ) \
		{ \
			int srclen = sizeof(src)/sizeof(src[0]); \
			int dstlen = sizeof((ptr)->fld); \
			WideCharToMultiByte(CP_OEMCP, 0, src, srclen, (ptr)->fld, dstlen, NULL, NULL); \
			(ptr)->fld[dstlen-1] = '\0'; \
			SI_SET_USED(ptr,fld); \
		}\
	}

// Macro to copy ANSI string to wide character structure member
#define SI_SET_STRING_AW(ptr,fld,src) \
	{ if ( SI_FIELD_EXISTS(ptr,fld) ) \
		{ \
			int srclen = sizeof(src); \
			int dstlen = sizeof((ptr)->fld)/sizeof((ptr)->fld[0]); \
			MultiByteToWideChar(CP_OEMCP, MB_PRECOMPOSED, (LPSTR) src, srclen, (LPWSTR)(ptr)->fld, dstlen); \
			(ptr)->fld[dstlen-1] = L'\0'; \
			SI_SET_USED(ptr,fld); \
		}\
	}


//-----------------------------------------------------------------------------
// Helper function prototypes
//-----------------------------------------------------------------------------

BOOL CopyStructInfo(LPSTRUCT_INFO lpDest, LPSTRUCT_INFO lpSource);


#ifdef __cplusplus
}
#endif

#endif	// #ifndef STRUCINF_H_
