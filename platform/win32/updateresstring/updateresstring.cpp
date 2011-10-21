/*------------------------------------------------------------------------
* (The MIT License)
*
* Copyright (c) 2008-2011 Rhomobile, Inc.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*
* http://rhomobile.com
*------------------------------------------------------------------------*/

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

