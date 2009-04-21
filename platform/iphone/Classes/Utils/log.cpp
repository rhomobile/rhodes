/*
 *  log.cpp
 *  rhorunner
 *
 *  Created by Vlad on 4/15/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "log.h"
#include "RhoLog.h"

char* getLogText() {
    rho::String strText;
    LOGCONF().getLogText(strText);
	return strdup(strText.c_str());
}

int grtLogTextPos() {
	return LOGCONF().getLogTextPos();
}

char* getLogEnabledCategories() {
	return strdup(LOGCONF().getEnabledCategories().c_str());
}

char* getLogDisabledCategories() {
	return strdup(LOGCONF().getDisabledCategories().c_str());
}

int getLogSeverity() {
	return LOGCONF().getMinSeverity();
}

void setLogEnabledCategories(const char* categories) {
	LOGCONF().setEnabledCategories(categories);
}

void setLogDisabledCategories(const char* categories) {
	LOGCONF().setDisabledCategories(categories);
}

void setLogSeveity(int nLevel) {
	LOGCONF().setMinSeverity(nLevel);
}

void saveLogSettings() {
	 LOGCONF().saveToFile();
}