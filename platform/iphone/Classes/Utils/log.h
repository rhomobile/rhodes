/*
 *  log.h
 *  rhorunner
 *
 *  Created by Vlad on 4/15/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __IPHONE_LOG__
#define __IPHONE_LOG__

#if defined(__cplusplus)
extern "C" {
#endif
	
	char* getLogText();
	int grtLogTextPos();

	char* getLogEnabledCategories();	
	char* getLogDisabledCategories();
	int getLogSeverity();

	void setLogEnabledCategories(const char* categories);
	void setLogDisabledCategories(const char* categories);
	void setLogSeveity(int nLevel);
		
	void saveLogSettings();
	
#if defined(__cplusplus)
}
#endif

#endif //__IPHONE_LOG__