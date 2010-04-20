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

