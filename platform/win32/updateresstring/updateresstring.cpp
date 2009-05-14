// updateresstring.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "strblock.h"

int _tmain(int argc, _TCHAR* argv[])
{
	if ( argc < 4 )
	{
		printf("Invalid number of arguments!\n");
		printf("Usage:\n\tupdateresstring <path to executable> <new string value> <IDS_ to replace>\n\n"
			   "For example:\n\treplaceicon C:\rhodes.exe \"Rhodes.MainWindow\" 10005");
		return -1;
	}

	int IDS_STRING = _ttoi((const _TCHAR*)argv[3]);
	UINT nBlockId = (IDS_STRING / NO_OF_STRINGS_PER_BLOCK) + 1;
	UINT nIndex = NO_OF_STRINGS_PER_BLOCK - ( nBlockId * NO_OF_STRINGS_PER_BLOCK - IDS_STRING );

	HSTRBLOCK hStrBlock = GetStringBlockA( argv[1], nBlockId, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US) );
	if ( !SetStringA(hStrBlock, nIndex, argv[2])  )
	{
		printf( "Unable to update resource string" );
	}
	else
	{
		if ( !UpdateStringBlockA( argv[1], hStrBlock, nBlockId, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US) ) )
			printf( "Unable to update string block" );
	}
	return 0;
}

