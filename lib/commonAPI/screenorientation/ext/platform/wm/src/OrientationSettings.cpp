#include <windows.h>
#include "common/RhoStd.h"
#include "common/StringConverter.h"
#include "logging/RhoLog.h"
#include "OrientationSettings.h"

/**
 * 
 * 
 * @author GXV738 (6/12/2013)
 * 
 * @return bool true if screen orientation is supported
 */
bool ScreenOrientationExt::COrientationSettings::IsSupported()
{
	DEVMODE dm;
	bool bSupported = false;

	::ZeroMemory(&dm, sizeof(DEVMODE));
	dm.dmSize = sizeof(DEVMODE);
	dm.dmFields = DM_DISPLAYQUERYORIENTATION;

	LONG lCanRotate = ::ChangeDisplaySettingsEx(NULL, &dm, NULL, CDS_TEST, NULL);
	if ((DISP_CHANGE_SUCCESSFUL == lCanRotate) && (DMDO_0 != dm.dmFields))
	{
		bSupported = true;
	}
#if DEBUG
	LOG(TRACE) + "Orientation support query: " + rho::common::convertToStringA<long>(lCanRotate);
#endif
	return bSupported;
}

ScreenOrientationExt::COrientationSettings::COrientationSettings()
{
}

ScreenOrientationExt::COrientationSettings::~COrientationSettings()
{
}

/**
 * Get the current orientation
 * 
 * @author GXV738 (6/12/2013)
 * 
 * @return ScreenOrientationExt::ScreenOrientationModes 
 */
ScreenOrientationExt::ScreenOrientationModes ScreenOrientationExt::COrientationSettings::GetOrientation()
{
	DEVMODE dm;
	ScreenOrientationModes orientationMode = SOM_BAD_ORIENTATION;

	::ZeroMemory(&dm, sizeof(DEVMODE));
	dm.dmSize = sizeof(DEVMODE);
	dm.dmFields = DM_DISPLAYORIENTATION;

	LONG lCanRotate = ::ChangeDisplaySettingsEx(NULL, &dm, NULL, CDS_TEST, NULL);
	if (DISP_CHANGE_SUCCESSFUL == lCanRotate)
	{
		if ((dm.dmDisplayOrientation & DMDO_90) == DMDO_90)
		{
			orientationMode = SOM_RIGHT_HANDED;
		}
		else if ((dm.dmDisplayOrientation & DMDO_180) == DMDO_180)
		{
			orientationMode = SOM_UPSIDE_DOWN;
		}
		else if ((dm.dmDisplayOrientation & DMDO_270) == DMDO_270)
		{
			orientationMode = SOM_LEFT_HANDED;
		}
		else if ((dm.dmDisplayOrientation & DMDO_0) == DMDO_0)
		{
			orientationMode = SOM_NORMAL;
		}
#if DEBUG
		LOG(TRACE) + "Screen orientation query returned : " + rho::common::convertToStringA<unsigned long>(dm.dmDisplayOrientation);
#endif
	}
	else
	{
		LOG(TRACE) + "Screen orientation query failed : " + rho::common::convertToStringA<long>(lCanRotate);
	}
#if DEBUG
	LOG(TRACE) + "Screen orientation mode : " + rho::common::convertToStringA<unsigned int>(static_cast<unsigned int>(orientationMode));
#endif
	return orientationMode;
}

/**
 * Set the orientation to one of the orientation modes.
 * 
 * @author GXV738 (6/12/2013)
 * 
 * @param orientationModes 
 * 
 * @return bool 
 */
bool ScreenOrientationExt::COrientationSettings::SetOrientation(ScreenOrientationModes orientationMode)
{
	DEVMODE dm;
	bool opStatus = true;
	DWORD dwAngle = 0, dwLandscapeMode = 0;
#if DEBUG
	LOG(TRACE) + "Set screen orientation  to mode : " + rho::common::convertToStringA<unsigned int>(static_cast<unsigned int>(orientationMode));
#endif

	::ZeroMemory(&dm, sizeof(DEVMODE));
	dm.dmSize = sizeof(DEVMODE);
	dm.dmFields = DM_DISPLAYORIENTATION;

	switch(orientationMode)
	{
	case SOM_NORMAL:
		dm.dmDisplayOrientation |= DMDO_0;
		break;
	case SOM_RIGHT_HANDED:
		dm.dmDisplayOrientation |= DMDO_90;
		dwAngle = 90;
		dwLandscapeMode = 1;
		break;
	case SOM_UPSIDE_DOWN:
		dm.dmDisplayOrientation |= DMDO_180;
		dwAngle = 180;
		break;
	case SOM_LEFT_HANDED:
		dm.dmDisplayOrientation |= DMDO_270;
		dwAngle = 270;
		dwLandscapeMode = 1;
		break;
	default:
		opStatus = false;
		break;
	}

	if (opStatus)
	{
		LONG lCanRotate = ::ChangeDisplaySettingsEx(NULL, &dm, NULL, CDS_RESET, NULL);
		if (DISP_CHANGE_SUCCESSFUL != lCanRotate)
		{
			opStatus = false;
			LOG(WARNING) + "Setting screen orientation failed : " + rho::common::convertToStringA<long>(lCanRotate);
		}
		else
		{
			PersistSettingsToRegistry(dwAngle, dwLandscapeMode);
		}
	}
	else
	{
		LOG(WARNING) + "Unknown screen orientation mode.";
	}
	return opStatus;
}

void ScreenOrientationExt::COrientationSettings::PersistSettingsToRegistry(DWORD dwAngle, DWORD dwLandscapeMode)
{
	HKEY hRegKey = NULL;
	DWORD dwRetVal = 0;
	dwRetVal = RegCreateKeyEx(HKEY_LOCAL_MACHINE, L"System\\GDI\\Rotation", 0, NULL, 0, 0, 0, &hRegKey, 0);
	if (ERROR_SUCCESS == dwRetVal)
	{
		RegSetValueEx(hRegKey, L"Angle", 0, REG_DWORD, (BYTE*)&dwAngle, sizeof(DWORD));		
		RegCloseKey(hRegKey);
	}
	dwRetVal = RegCreateKeyEx(HKEY_LOCAL_MACHINE, L"System\\GDI\\Rotation", 0, NULL, 0, 0, 0, &hRegKey, 0);
	if (ERROR_SUCCESS == dwRetVal)
	{
		RegSetValueEx(hRegKey, L"LandscapeMode", 0, REG_DWORD, (BYTE*)&dwLandscapeMode, sizeof(DWORD));
		RegCloseKey(hRegKey);
	}
}