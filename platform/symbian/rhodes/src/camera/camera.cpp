/*
 * camera.c
 *
 *  Created on: Mar 12, 2009
 *      Author: Anton
 */

#include "rhodes.hrh"
#include <eikenv.h>

extern void SendWindowEvent(TInt aEvent, const char* data);

extern "C" {

void take_picture(char* callback_url) {
	SendWindowEvent(ECmdTakePicture, callback_url);
}

void choose_picture(char* callback_url){
	SendWindowEvent(ECmdChoosePicture, callback_url);
}

}
