/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/

#include "logging/RhoLog.h"
#include "syscall.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "RhoDoSysCall"

PARAMS_WRAPPER* do_syscall(PARAMS_WRAPPER* params) {
	int i;

	//Look through input parameters
	if (params) {
		RAWLOG_INFO1("Calling \"%s\" with parameters:\n", params->_callname);
		for (i=0;i<params->_nparams;i++) {
			RAWLOG_INFO2("%s => %s\n",params->_names[i],params->_values[i]);
		}	
	}
	
	//If synchronous call on ruby thread, generate return parameters 
	if (params && !params->_call_on_ui_thread) {
		PARAMS_WRAPPER* pw = malloc(sizeof(PARAMS_WRAPPER));
		pw->_nparams = 2;
		pw->_names = malloc(pw->_nparams*sizeof(char*));	
		pw->_values = malloc(pw->_nparams*sizeof(char*));
		for(i=0;i<pw->_nparams;i++){
			pw->_names[i] = malloc(128*sizeof(char));	
			sprintf(pw->_names[i], "key %d", i);
			pw->_values[i] = malloc(128*sizeof(char));		
			sprintf(pw->_values[i], "value %d", i);
		}
		return pw;
	} else {
		return NULL;
	}
}

