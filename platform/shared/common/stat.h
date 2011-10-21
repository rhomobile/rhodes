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

#if defined(__APPLE__) && !defined(__arm__) && !defined(__IPHONE_4_0)
/* Simulator 3.2 or less */
#define RHO_IPHONE_SIMULATOR_3
#endif

#ifdef RHO_IPHONE_SIMULATOR_3
#ifdef stat
#undef stat
#endif
#ifdef lstat
#undef lstat
#endif
#ifdef fstat
#undef fstat
#endif
#endif

#include <sys/stat.h>

#ifdef RHO_IPHONE_SIMULATOR_3
/*
 * map stat functions and structure to theirs 64-bit analogues to be binary
 * compatible with iPhone 4 x86/x86_64 application - in iPhone 4 SDK stat
 * become 64-bit only so enabling such mapping we could run applications built
 * with 3.x SDK on iPhone 4 simulator
 * This is not required for iPhone devices - there stat was always 64-bit.
 */
#define stat stat64
#define lstat lstat64
#define fstat fstat64
#endif
