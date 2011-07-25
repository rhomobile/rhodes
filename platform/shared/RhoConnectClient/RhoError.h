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
