#pragma once

#include "common/RhoStd.h"

typedef struct hostTrackerConfigInfo{
	bool isTrackConnectionSet; //To check whether tracking the connection is required or not.	
	int iDialogTimeout; //Timeout value for closing the dialog.
	int iPort; //Port number to which connection will be estabilished.
	int iPollInterval; //Thread Interval
	int iPingTimeout; //Socket Connection Timeout
	rho::String szHostName; //Host URL or IP Address
	//TCHAR szHostMessage[30]; ; //Host Message during tracking.
	TCHAR szBadLinkURL[MAX_PATH + 1];//Badlink URL
	
}stHostTrackerConfigInfo;