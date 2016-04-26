#pragma once

#include "..\stdafx.h"
#include "../../../Engine/common/pbengine_defs.h"

//This interface should be implemented by the clients of GpsManager
class IGpsWrapperObserver
{
public:
	//Position change notifier
	virtual void OnGpsPositionReceived(PGPS_POSITION pPosition) = 0;
	//Position change notified adapted to 
	virtual void OnGpsPositionReceived(EngineLocation* pPosition) = 0;
	//GPS state change notifier
	virtual void OnGpsStateReceived(PGPS_DEVICE pDevice) = 0;
	//Geolocation error notifier
	virtual void OnGpsLocationError(EngineLocationError errorCode, LPCTSTR errorMessage) = 0;
	//  Log Method
	virtual BOOL EngineLog(LogTypeInterface logSeverity,LPCTSTR pLogComment, 
						LPCTSTR pFunctionName, DWORD dwLineNumber,LPCTSTR pCallingModule) = 0;
};
