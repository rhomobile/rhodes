/*
 * ports_mngt.h
 *
 *  Created on: Apr 28, 2009
 *      Author: Anton
 */

#ifndef PORTS_MNGT_H_
#define PORTS_MNGT_H_

#ifdef __cplusplus
extern "C" {
#endif

const char* get_free_listening_port();

const char* get_home_url();

const wchar_t* get_home_url_w();

#ifdef __cplusplus
}
#endif

#endif /* PORTS_MNGT_H_ */
