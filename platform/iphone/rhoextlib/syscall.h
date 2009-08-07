/*
 *  syscall.h
 *  rhoextlib
 *
 *  Created by Vlad on 6/8/09.
 *
 */
#ifndef __RHO_SYSCALL_H__
#define __RHO_SYSCALL_H__

#if defined(__cplusplus)
extern "C" {
#endif
	
	
typedef struct __params_wrapper__ {
	int _nparams;
	char** _names;
	char** _values;
	const char* _callname;
	int _call_on_ui_thread;
} PARAMS_WRAPPER;
	
PARAMS_WRAPPER* do_syscall(PARAMS_WRAPPER* params);
	
#if defined(__cplusplus)
}
#endif

#endif	/* __RHO_SYSCALL_H__ */
