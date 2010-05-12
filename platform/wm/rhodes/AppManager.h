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