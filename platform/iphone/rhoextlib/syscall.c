#include "ruby.h"
#include "logging/RhoLog.h"
#include "syscall.h"
#include "ruby/ext/rho/rhoruby.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "RhoSysCall"

extern void _rho_ext_syscall(PARAMS_WRAPPER* params);

VALUE rho_sys_makephonecall(const char* callname, int nparams, char** param_names, char** param_values) {
	PARAMS_WRAPPER params = {nparams,param_names,param_values,callname,0};
	PARAMS_WRAPPER* ret;
	VALUE ret_value = Qnil;
	int i;
	
	//Check if we need to do syscall on UI thread
	for (i=0;i<nparams;i++) {
		if (strcmp("thread",param_names[i]) == 0) {
			if (strcmp("ui",param_values[i]) == 0) {
				RAWLOG_INFO("SysCall on UI theread...\n");
				params._call_on_ui_thread = 1;
			}
			break;
		}
	}	

	if (params._call_on_ui_thread) {
		// Switching to UI thread
		// There is no return parameters because syscall on UI thread will be done asynchronously
		_rho_ext_syscall(&params);
	} else {
		// Doing syscall on "ruby" thread; this call is done syncronously
		ret = do_syscall(&params);
		if (ret!=NULL) {
			VALUE hash = rb_hash_new();
            rho_ruby_holdValue(hash);
			RAWLOG_INFO("Add to return hash:\n");
			for (i=0;i<ret->_nparams;i++) {
				RAWLOG_INFO2("%s => %s\n",ret->_names[i],ret->_values[i]);
				if (ret->_names[i] && ret->_values[i])
					rb_hash_aset(hash, rb_str_new2(ret->_names[i]), rb_str_new2(ret->_values[i]));
				if (ret->_names[i]) free(ret->_names[i]);
				if (ret->_values[i]) free(ret->_values[i]);
			}	
			if (ret->_names) free(ret->_names);
			if (ret->_values) free(ret->_values);
			free(ret);			
			ret_value = hash;
            rho_ruby_releaseValue(hash);
		}
	}
	
	return ret_value;
}

