/*
 *  config.c
 *  rhorunner
 *
 *  Created by Vlad on 4/21/09.
 *  Copyright 2009 Rhomobile, Inc. All rights reserved.
 *
 */

#include "config.h"
#include "RhoConf.h"

bool config_getBool(const char* szName) {
	return RHOCONF().getBool(szName);
}

void config_setBool(const char* szName, bool value) {
	RHOCONF().setBool(szName,value);
}

char* config_getString(const char* szName) {
	return strdup(RHOCONF().getString(szName).c_str());
}

void config_setString(const char* szName, const char* value){
	RHOCONF().setString(szName,value);
}

void config_save() {
	RHOCONF().saveToFile();
}