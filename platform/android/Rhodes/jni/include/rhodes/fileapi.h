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

#ifndef FILEAPI_H_
#define FILEAPI_H_

#include "common/RhoDefs.h"
#include "common/RhoStd.h"

enum RhoFsSetupMode {RHO_FS_SETUP_NONE = 0, RHO_FS_SETUP_FORCE_DIRECTORIES, RHO_FS_SETUP_FORCE_FILES};
enum RhoFsMode {RHO_FS_DISK_ONLY = 0, RHO_FS_TRANSPARRENT};

RHO_GLOBAL void rho_file_set_fs_mode(int mode);
RHO_GLOBAL void rho_file_android_process_stat_table(int setupMode);

#endif /* FILEAPI_H_ */
