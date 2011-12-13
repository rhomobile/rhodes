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

#ifdef __OBJC__
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#endif

#define ZIP_STD 1

extern void rho_file_impl_delete_files_in_folder(const char *szFolderPath);

@interface RhoFileImpl : NSObject {
}

+ (void)deleteFilesInFolder: (NSString*)folderPath;

@end

@implementation RhoFileImpl

+ (void)deleteFilesInFolder: (NSString*)folderPath {
	NSFileManager *fileManager = [NSFileManager defaultManager];
	
	NSDirectoryEnumerator *enumerator = [fileManager enumeratorAtPath:folderPath];
	if (enumerator == nil)
		return;
	
	NSError *err;
	id path;
	while ((path = [enumerator nextObject])) {
		NSString *file = [NSString stringWithFormat:@"%@/%@", folderPath, [NSString stringWithFormat:@"%@", path]];
		[fileManager removeItemAtPath:file error:&err];
	}
}

@end

void rho_file_impl_delete_files_in_folder(const char *szFolderPath)
{
	[RhoFileImpl deleteFilesInFolder:[NSString stringWithUTF8String:szFolderPath]];
}

void rho_file_impl_delete_folder(const char* szFolderPath) {
	NSFileManager *fileManager = [NSFileManager defaultManager];
	NSError *err;
    [fileManager removeItemAtPath:[NSString stringWithUTF8String:szFolderPath] error:&err];
}

void rho_file_impl_copy_folders_content_to_another_folder(const char* szSrcFolderPath, const char* szDstFolderPath) {

	NSFileManager *fileManager = [NSFileManager defaultManager];
    
    NSString* src_folder = [NSString stringWithUTF8String:szSrcFolderPath];
    NSString* dst_folder = [NSString stringWithUTF8String:szDstFolderPath];
	
	NSDirectoryEnumerator *enumerator = [fileManager enumeratorAtPath:src_folder];
	if (enumerator == nil)
		return;
	
	NSError *err;
	id path;
	while ((path = [enumerator nextObject])) {
		NSString *src_item = [NSString stringWithFormat:@"%@/%@", src_folder, [NSString stringWithFormat:@"%@", path]];
		NSString *dst_item = [NSString stringWithFormat:@"%@/%@", dst_folder, [NSString stringWithFormat:@"%@", path]];
		[fileManager copyItemAtPath:src_item toPath:dst_item error:&err];
	}
    
}

