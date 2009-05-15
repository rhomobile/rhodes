/*
 *  config.h
 *  rhorunner
 *
 *  Created by Vlad on 4/21/09.
 *  Copyright 2009 Rhomobile, Inc. All rights reserved.
 *
 */

#ifndef __IPHONE_CONFIG__
#define __IPHONE_CONFIG__

#if defined(__cplusplus)
extern "C" {
#endif
	
	bool  config_getBool(const char* szName);
	void  config_setBool(const char* szName, bool value);
	char* config_getString(const char* szName);
	void  config_freeString(char* str);
	void  config_setString(const char* szName, const char* value);
	void  config_save();
	
#if defined(__cplusplus)
}
#endif

#endif //__IPHONE_CONFIG__
		
