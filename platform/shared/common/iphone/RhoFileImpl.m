//
//  RhoFileImpl.m
//  rhorunner
//
//  Created by Dmitry Moskalchuk on 09.10.09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//
#ifdef __OBJC__
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#endif

#define ZIP_STD 1

extern void delete_files_in_folder(const char *szFolderPath);

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

void delete_files_in_folder(const char *szFolderPath)
{
	[RhoFileImpl deleteFilesInFolder:[NSString stringWithUTF8String:szFolderPath]];
}
