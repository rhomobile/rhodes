// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA

#pragma comment(linker, "/nodefaultlib:libc.lib")
#pragma comment(linker, "/nodefaultlib:libcd.lib")

#ifndef STRICT
#define STRICT
#endif

#include <ceconfig.h>
#if defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP)
#define SHELL_AYGSHELL
#endif

// NOTE - this is value is not strongly correlated to the Windows CE OS version being targeted
#define WINVER _WIN32_WCE

#ifdef _CE_DCOM
#define _ATL_APARTMENT_THREADED
#endif
#define _ATL_NO_AUTOMATIC_NAMESPACE

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit


#include "resourceppc.h"

#include <atlbase.h>
#include <atlcom.h>
#include <atlctl.h>


#if defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP)
#ifndef _DEVICE_RESOLUTION_AWARE
#define _DEVICE_RESOLUTION_AWARE
#endif
#endif

#ifdef _DEVICE_RESOLUTION_AWARE
#include "DeviceResolutionAware.h"
#endif

using namespace ATL;

#include <aygshell.h>
#pragma comment(lib, "aygshell.lib") 

#if _WIN32_WCE < 0x500 && ( defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP) )
	#pragma comment(lib, "ccrtrtti.lib")
	#ifdef _X86_	
		#if defined(_DEBUG)
			#pragma comment(lib, "libcmtx86d.lib")
		#else
			#pragma comment(lib, "libcmtx86.lib")
		#endif
	#endif
#endif

#include <altcecrt.h>

/******************************************************************************/
// ODAX definitions, etc. for use by all source files
#include <windows.h>
#include <stdio.h>
#include <stdarg.h>

// Expat includes and library
#define XML_STATIC
//#define XML_UNICODE_WCHAR_T

#include "..\\..\\..\\Expat\\include\\expat.h"
#pragma comment (lib, "..\\..\\..\\Expat\\bin\\expat.lib")

//#define throw
//#define Exception

//#define _MAX_PATH   260 /* max. length of full pathname */
#define _MAX_DRIVE  3   /* max. length of drive component */
#define _MAX_DIR    256 /* max. length of path component */
#define _MAX_FNAME  256 /* max. length of file name component */
#define _MAX_EXT    256 /* max. length of extension component */
#define _MAX_BUFF_LEN 2048 /* maximum length of buffer for CSV File Record*/

#include "splitpath.h"

// Name of result set file.
#define RESULT_SET_FILENAME			"rset.dat"
#define TEMP_RESULT_SET_FILENAME	"temprset.dat"

// Name of temporary working files..
#define TEMP_XML_FILENAME	"tempxml.xml"
#define TEMP_CSV_FILENAME	"tempcsv.csv"
// (these files get created in the same dir as the db file)

#define EXCEPTION_MSG_BUF_SIZE 512

#include "Tokens.h"
#include "Exception.h"
#include "Errors.h"
#include "CString.h"
#include "CList.h"
#include "Unicode.h"

#define odd(x)		(x % 2 != 0)
#define even(x)	(!odd(x))
#define dim(x) (sizeof (x) / sizeof (x[0]))

enum enumInputType
	{
	XML_INPUT,
	CSV_INPUT
	};

#define DETERMINE_FORMAT_FROM_FILE	-1
#define XML_INPUT_FORMAT	0
#define CSV_INPUT_FORMAT	1

enum enumOutputType
	{
	XML_OUTPUT,
	CSV_OUTPUT,
	RESULT_SET_OUTPUT
	};

enum enumSqlStatementType
	{
	SQL_INSERT,
	SQL_DELETE,
	SQL_UPDATE,
	SQL_SELECT
	};

typedef CList<CString> CStringList;

#include "CEvalResult.h"

#include "CFile.h"
#include "CRecord.h"
#include "CExpr.h"

// Turn this into real class
struct CStatementData
	{
	CString SourceDbName;
	enumSqlStatementType StatementType;
	enumOutputType OutputType;
	CExpr *pCondition;
	CRecord *pCurrentResult;
	CRecord NewValues;			// <-- New. Added for UPDATE, may be able to use this for insert
										// also, instead of separate field/value lists.

	CFile TempDb;					// <-- New. TempDB in case of update or delete

	char Delimiter;				// Used by csv only..
	int FirstRow;					// Used by csv only..

	int ErrorNo;
	};

#include "Xml.h"
#include "Csv.h"
//#include "..\Expat\xmlparse.h"
#include "CExpat.h"
#include "CResultSet.h"
#include "Output.h"
#include "SQLParse.h"
