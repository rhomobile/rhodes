#ifndef DISPLAYSETTINGS_H_34612E1C_97AA_43C1_B1E1_61EB28C378DE
#define DISPLAYSETTINGS_H_34612E1C_97AA_43C1_B1E1_61EB28C378DE

#pragma once

namespace screenorientation
{
	enum ScreenOrientationModes
	{
		SOM_BAD_ORIENTATION = -1,
		SOM_NORMAL = 0,
		SOM_RIGHT_HANDED,
		SOM_UPSIDE_DOWN,
		SOM_LEFT_HANDED

	}; // ScreenOrientationModes

	class COrientationSettings
	{
	public:

		COrientationSettings();
		~COrientationSettings();

		static bool IsSupported();
		static ScreenOrientationModes GetOrientation();
		static bool SetOrientation(ScreenOrientationModes orientationModes);		
	protected:
		static void PersistSettingsToRegistry(DWORD dwAngle, DWORD dwLandscapeMode);
		COrientationSettings(const COrientationSettings&);
		COrientationSettings& operator=(const COrientationSettings&);
	private:
		
	}; // CDisplaySettings
} // screenorientation

#endif	// DISPLAYSETTINGS_H_34612E1C_97AA_43C1_B1E1_61EB28C378DE
