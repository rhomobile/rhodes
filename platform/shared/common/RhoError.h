#ifndef _RHOERROR_H_
#define _RHOERROR_H_

#define RHO_ERR_NONE  0
#define RHO_ERR_NETWORK  1
#define RHO_ERR_REMOTESERVER  2
#define RHO_ERR_RUNTIME  3
#define RHO_ERR_UNEXPECTEDSERVERRESPONSE  4
#define RHO_ERR_DIFFDOMAINSINSYNCSRC  5
#define RHO_ERR_NOSERVERRESPONSE  6
#define RHO_ERR_CLIENTISNOTLOGGEDIN  7
#define RHO_ERR_CUSTOMSYNCSERVER  8
#define RHO_ERR_UNATHORIZED  9
#define RHO_ERR_CANCELBYUSER  10
#define RHO_ERR_SYNCVERSION  11
#define RHO_ERR_GEOLOCATION  12

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

char* rho_error_getErrorText(int nError);
void rho_error_freeString(char* sz);

#ifdef __cplusplus
};
#endif //__cplusplus

#endif //_RHOERROR_H_
