/*
 * $Id: debug.h,v 1.5 2006/01/30 23:07:57 mclark Exp $
 *
 * Copyright (c) 2004, 2005 Metaparadigm Pte. Ltd.
 * Michael Clark <michael@metaparadigm.com>
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See COPYING for details.
 *
 */
/*
#ifndef _DEBUG_H_
#define _DEBUG_H_

extern void mc_set_debug(int debug);
extern int mc_get_debug(void);

extern void mc_set_syslog(int syslog);
extern void mc_abort(const char *msg, ...);
extern void mc_debug(const char *msg, ...);
extern void mc_error(const char *msg, ...);
extern void mc_info(const char *msg, ...);

#ifdef MC_MAINTAINER_MODE
#define MC_SET_DEBUG(x) mc_set_debug(x)
#define MC_GET_DEBUG() mc_get_debug()
#define MC_SET_SYSLOG(x) mc_set_syslog(x)

#define MC_ABORT0(x) mc_abort(x)
#define MC_ABORT(x, arg1)  mc_abort(x, arg1)
#define MC_ABORT2(x, arg1, arg2)  mc_abort(x, arg1, arg2)
#define MC_ABORT3(x, arg1, arg2, arg3)  mc_abort(x, arg1, arg2, arg3)

#define MC_DEBUG0(x)  mc_debug(x)
#define MC_DEBUG(x, arg1)  mc_debug(x, arg1)
#define MC_DEBUG2(x, arg1, arg2)  mc_debug(x, arg1, arg2)
#define MC_DEBUG3(x, arg1, arg2, arg3)  mc_debug(x, arg1, arg2, arg3)

#define MC_ERROR0(x)  mc_error(x)
#define MC_ERROR(x, arg1)  mc_error(x, arg1)
#define MC_ERROR2(x, arg1, arg2)  mc_error(x, arg1, arg2)
#define MC_ERROR3(x, arg1, arg2, arg3)  mc_error(x, arg1, arg2, arg3)

#define MC_INFO0(x)  mc_info(x)
#define MC_INFO(x, arg1)  mc_info(x, arg1)
#define MC_INFO2(x, arg1, arg2)  mc_info(x, arg1, arg2)
#define MC_INFO3(x, arg1, arg2, arg3)  mc_info(x, arg1, arg2, arg3)

#else
#define MC_SET_DEBUG(x) if (0) mc_set_debug(x)
#define MC_GET_DEBUG() (0)
#define MC_SET_SYSLOG(x) if (0) mc_set_syslog(x)
#define MC_ABORT0(x) if (0) mc_abort(x)
#define MC_ABORT(x, arg1) if (0) mc_abort(x, arg1)
#define MC_ABORT2(x, arg1, arg2) if (0) mc_abort(x, arg1, arg2)
#define MC_ABORT3(x, arg1, arg2, arg3) if (0) mc_abort(x, arg1, arg2, arg3)

#define MC_DEBUG0(x) if (0) mc_debug(x)
#define MC_DEBUG(x, arg1) if (0) mc_debug(x, arg1)
#define MC_DEBUG2(x, arg1, arg2) if (0) mc_debug(x, arg1, arg2)
#define MC_DEBUG3(x, arg1, arg2, arg3) if (0) mc_debug(x, arg1, arg2, arg3)

#define MC_ERROR0(x) if (0) mc_error(x)
#define MC_ERROR(x, arg1) if (0) mc_error(x, arg1)
#define MC_ERROR2(x, arg1, arg2) if (0) mc_error(x, arg1, arg2)
#define MC_ERROR3(x, arg1, arg2, arg3) if (0) mc_error(x, arg1, arg2, arg3)

#define MC_INFO0(x) if (0) mc_info(x)
#define MC_INFO(x, arg1) if (0) mc_info(x, arg1)
#define MC_INFO2(x, arg1, arg2) if (0) mc_info(x, arg1, arg2)
#define MC_INFO3(x, arg1, arg2, arg3) if (0) mc_info(x, arg1, arg2, arg3)
	
#endif

#endif
*/
