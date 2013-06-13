#include <windows.h>
#include "logging/RhoLog.h"
#include "OrientationSettings.h"

/**
 * 
 * 
 * @author GXV738 (6/12/2013)
 * 
 * @return bool true if screen orienation is supported
 */
bool screenorientation::COrientationSettings::IsSupported()
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
	return bSupported;
}

screenorientation::COrientationSettings::COrientationSettings()
{
}

screenorientation::COrientationSettings::~COrientationSettings()
{
}

/**
 * Get the current orientation
 * 
 * @author GXV738 (6/12/2013)
 * 
 * @return screenorientation::ScreenOrientationModes 
 */
screenorientation::ScreenOrientationModes screenorientation::COrientationSettings::GetOrientation()
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
	}
	else
	{
		LOG(WARNING) + __FILE__ + ":" + __LINE__+ "Screen orientation query failed.";
	}
	return orientationMode;
}

/**
 * Set the prientation to one of the orientation modes.
 * 
 * @author GXV738 (6/12/2013)
 * 
 * @param orientationModes 
 * 
 * @return bool 
 */
bool screenorientation::COrientationSettings::SetOrientation(ScreenOrientationModes orientationModes)
{
	DEVMODE dm;
	bool opStatus = true;
	DWORD dwAngle = 0, dwLandscapeMode = 0;

	::ZeroMemory(&dm, sizeof(DEVMODE));
	dm.dmSize = sizeof(DEVMODE);
	dm.dmFields = DM_DISPLAYORIENTATION;

	switch(orientationModes)
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
			LOG(WARNING) + __FILE__ + ":" + __LINE__+ " Screen orientation change failed.";
		}
		else
		{
			PersistSettingsToRegistry(dwAngle, dwLandscapeMode);
		}
	}
	else
	{
		LOG(WARNING) + __FILE__ + ":" + __LINE__+ " Unknown screen orientation mode.";
	}
	return opStatus;
}

void screenorientation::COrientationSettings::PersistSettingsToRegistry(DWORD dwAngle, DWORD dwLandscapeMode)
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