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

#pragma once

#include "common/RhoStd.h"

using namespace rho;

#ifdef ENABLE_DYNAMIC_RHOBUNDLE
class CAppManager
{
	enum {
		RRB_NONE_ERR,
		RRB_LOADING_ERR,
		RRB_UNZIP_ERR,
		RRB_REMOVEOLD_ERR,
		RRB_UPDATE_ERR,
		RRB_UNKNOWN_ERR
	};

public:
	CAppManager  (void);
	~CAppManager (void);

	/**
	 * @param szUrl - url of the zip file with rhobundle
	 * @param szZipPassword - zip password if required
	 */
	static void ReloadRhoBundle(HWND hwnd, const char* szUrl, const char* szZipPassword);
	static bool RemoveFolder(String pathname);
	static bool MoveFolder(const String& pathFrom, const String &pathTo);
	static bool RestartClient(HWND hwnd);

private:

	static int unzipRhoBundle (const char *zipData, unsigned int zipDataSize, const char *zipPassword, const String &targetPath);
	static int removeOldRhoBundle (void);
	static int updateRhoBundle(const String &root_path, const String &from_path);
	static void showMessage (HWND hWnd, int rrbErrCode);
};

#endif