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

#ifndef RHO_SHARED_NET_5686566c7ccb458784a377272387d53a
#define RHO_SHARED_NET_5686566c7ccb458784a377272387d53a

#ifdef _WIN32_WCE

/*
*	Because we need a per-thread errno, we define a function
*	pointer that we can call to return a pointer to the errno
*	for the current thread.  Then we define a macro for errno
*	that dereferences this function's result.
*
*	This makes it syntactically just like the "real" errno.
*
*	Using a function pointer allows us to use a very fast
*	function when there are no threads running and a slower
*	function when there are multiple threads running.
*/
void __WinCE_Errno_New_Thread(int *Errno_Pointer);
void __WinCE_Errno_Thread_Exit(void);
extern int *(*__WinCE_Errno_Pointer_Function)(void);

#define	errno (*(*__WinCE_Errno_Pointer_Function)())
/*
struct stat {
	unsigned long st_size;
	unsigned long st_ino;
	int st_mode;
	unsigned long st_atime;
	unsigned long st_mtime;
	unsigned long st_ctime;
	unsigned short st_dev;
	unsigned short st_nlink;
	unsigned short st_uid;
	unsigned short st_gid;
};

int stat(const char *path, struct stat *st);   */

#define _S_IFMT   0170000
#define _S_IFDIR  0040000
#define _S_IFREG  0100000
#define _S_IEXEC  0000100
#define _S_IWRITE 0000200 
#define _S_IREAD  0000400

#define S_IFMT _S_IFMT
#define S_IFDIR _S_IFDIR
#define S_IFREG _S_IFREG
#define S_IEXEC _S_IEXEC
#define S_IWRITE _S_IWRITE
#define S_IREAD _S_IREAD

#endif // _WIN32_WCE

#endif // RHO_SHARED_NET_5686566c7ccb458784a377272387d53a
