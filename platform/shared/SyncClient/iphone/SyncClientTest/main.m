//
//  main.m
//  SyncClientTest
//
//  Created by evgeny vovchenko on 8/19/10.
//  Copyright RhoMobile 2010. All rights reserved.
//

#import <UIKit/UIKit.h>

const char* rho_native_rhopath() ;
void copyFromMainBundle( NSFileManager* fileManager,  NSString * source, NSString * target, BOOL remove )
{
	BOOL dir;
	if(![fileManager fileExistsAtPath:source isDirectory:&dir]) {
		//NSAssert1(0, @"Source item '%@' does not exists in bundle", source);
		return;
	}
	
	if (!remove && dir) {
		if (![fileManager fileExistsAtPath:target])
			[fileManager createDirectoryAtPath:target attributes:nil];
		
		NSDirectoryEnumerator *enumerator = [fileManager enumeratorAtPath:source];
		NSString *child;
		while (child = [enumerator nextObject]) {
			copyFromMainBundle( fileManager, [source stringByAppendingPathComponent:child],
							  [target stringByAppendingPathComponent:child], NO );
		}
	}
	else {
		NSError *error;
		if ([fileManager fileExistsAtPath:target] && ![fileManager removeItemAtPath:target error:&error]) {
			//NSAssert2(0, @"Failed to remove '%@': %@", target, [error localizedDescription]);
			return;
		}
		if (![fileManager copyItemAtPath:source toPath:target error:&error]) {
			//NSAssert3(0, @"Failed to copy '%@' to '%@': %@", source, target, [error localizedDescription]);
			return;
		}
	}
}

int runSyncClientTests();
int main(int argc, char *argv[]) {
    
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
    //int retVal = UIApplicationMain(argc, argv, nil, nil);
	
    NSFileManager *fileManager = [NSFileManager defaultManager];
    
	NSString *bundleRoot = [[NSBundle mainBundle] resourcePath];
	NSString *rhoRoot = [NSString stringWithUTF8String:rho_native_rhopath()];
	
    NSString *dirs[] = {@"db"};	
	copyFromMainBundle( fileManager,
					[bundleRoot stringByAppendingPathComponent:dirs[0]],
					  [rhoRoot stringByAppendingPathComponent:dirs[0]],
					  NO);
	
    int retVal = runSyncClientTests();
    [pool release];
    return retVal;
}

const char* rho_native_rhopath() 
{
	static bool loaded = FALSE;
	static char root[FILENAME_MAX];
	if (!loaded){
		NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
		NSString *documentsDirectory = //[paths objectAtIndex:0];
		[ [paths objectAtIndex:0] stringByAppendingString:@"/"];
		[documentsDirectory getFileSystemRepresentation:root maxLength:sizeof(root)];
		loaded = TRUE;
	}
	
	return root;
}