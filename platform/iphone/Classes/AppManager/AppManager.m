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

#import <SystemConfiguration/SystemConfiguration.h>
#import <Foundation/Foundation.h>

#include "common/RhoPort.h"
#include "unzip/unzip.h"
#import "AppManager.h"
//#import "HttpContext.h"
//#import "HttpMessage.h"
//#import "Dispatcher.h"
#import "AppLoader.h"
#import "common/RhoConf.h"
#import "common/RhodesApp.h"
#import "logging/RhoLogConf.h"
#include "ruby/ext/rho/rhoruby.h"
#import "logging/RhoLog.h"
#import "../Event/Event.h"

#import "api_generator/iphone/CRubyConverter.h"

#import "Rhodes.h"

#import "common/app_build_configs.h"

#include <sys/xattr.h>
#include <sys/types.h>
#include <sys/sysctl.h>

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "RhodesApp"

extern bool isNewInstallation;
static bool UnzipApplication(const char* appRoot, const void* zipbuf, unsigned int ziplen);
//const char* RhoGetRootPath();



VALUE rho_sys_has_wifi_network();
VALUE rho_sys_has_cell_network();



static BOOL getEnabledStartupLogging() {
    static int res = -1;
    if (res == -1) {
        const char* svalue = get_app_build_config_item("iphone_enable_startup_logging");
        if (svalue != NULL) {
            if ((svalue[0] != '0') && (svalue[0] != 'f') && (svalue[0] != 'F') ) {
                res = 1;
            }
            else {
                res = 0;
            }
        }
    }
    return res == 1;
}

// when Application manager is initialize Rhodes logging system is not ready !!!
#define ENABLE_STARTUP_TRACES getEnabledStartupLogging()



BOOL isPathIsSymLink(NSFileManager *fileManager, NSString* path) {
    NSError *error;

    NSDictionary *attributes = [fileManager attributesOfItemAtPath:path error:&error];

    if (attributes == nil) {
        if (ENABLE_STARTUP_TRACES) NSLog(@"RhoAppManager:      SymLink  NO : %@", path);
        return NO;
    }

    NSString* fileType = [attributes objectForKey:NSFileType];

    if (fileType == nil) {
        if (ENABLE_STARTUP_TRACES) NSLog(@"RhoAppManager:      SymLink  NO : %@", path);
        return NO;
    }

    BOOL res = [NSFileTypeSymbolicLink isEqualToString:fileType];
    if (res) {
        if (ENABLE_STARTUP_TRACES) NSLog(@"RhoAppManager:      SymLink YES : %@", path);
    }
    else {
        if (ENABLE_STARTUP_TRACES) NSLog(@"RhoAppManager:      SymLink  NO : %@", path);
    }

    return res;
}


@interface RhoFileManagerDelegate_RemoveOnly_SymLinks : NSObject {
@public
}

-(BOOL)fileManager:(NSFileManager *)fileManager shouldRemoveItemAtPath:(NSString *)path;
@end

@implementation RhoFileManagerDelegate_RemoveOnly_SymLinks


-(BOOL)fileManager:(NSFileManager *)fileManager shouldRemoveItemAtPath:(NSString *)path {
    return isPathIsSymLink(fileManager, path);
}

@end



@implementation AppManager

+ (AppManager *)instance
{
	static AppManager *gInstance = NULL;
	@synchronized(self)
    {
		if (gInstance == NULL)
			gInstance = [[self alloc] init];
    }
	return(gInstance);
}


+ (void) startupLogging:(NSString*)message {
    if (ENABLE_STARTUP_TRACES) {
        NSLog(@"Rhodes startup : %@", message);
    }
}

+ (BOOL) isEnabledStartupLogging {
    return getEnabledStartupLogging();
}



/*
 * Gets root folder of the site
 * Application folders located undern the root
 */
+ (NSString *) getApplicationsRootPath {
	NSString *documentsDirectory = [NSString stringWithUTF8String:rho_native_rhopath()];
	return [documentsDirectory stringByAppendingPathComponent:@"apps"];
}

+ (NSString *) getDbPath {
	NSString *documentsDirectory = [NSString stringWithUTF8String:rho_native_rhodbpath()];
	return [documentsDirectory stringByAppendingPathComponent:@"db"];
}

+ (NSString *) getApplicationsUserPath {
	return [NSString stringWithUTF8String:rho_native_rhouserpath()];
}


+ (NSString *) getApplicationsRosterUrl {
	return @"https://tau-platform.com/";
}

+ (bool) installApplication:(NSString*)appName data:(NSData*)appData {
	NSFileManager *fileManager = [NSFileManager defaultManager];
	NSString *appPath = [[AppManager getApplicationsRootPath] stringByAppendingPathComponent:appName];
	if ([fileManager fileExistsAtPath:appPath]) {
		NSError *error;
		[fileManager removeItemAtPath:appPath error:&error];
	}
    {
        NSError* error;
        [fileManager createDirectoryAtPath:appPath withIntermediateDirectories:YES attributes:nil error:&error];
    }
	static char appRoot[FILENAME_MAX];
	[appPath getFileSystemRepresentation:appRoot maxLength:sizeof(appRoot)];
	return UnzipApplication( appRoot, [appData bytes], [appData length]);
}

- (void) copyFromMainBundle:(NSFileManager*)fileManager fromPath:(NSString *)source
					 toPath:(NSString*)target remove:(BOOL)remove {
	BOOL dir;
	if(![fileManager fileExistsAtPath:source isDirectory:&dir]) {
		NSAssert1(0, @"Source item '%@' does not exists in bundle", source);
		return;
	}

	if (!remove && dir) {
        NSError *error;
        if (![fileManager fileExistsAtPath:target]) {
			[fileManager createDirectoryAtPath:target withIntermediateDirectories:YES attributes:nil error:&error];
        }
		NSDirectoryEnumerator *enumerator = [fileManager enumeratorAtPath:source];
		NSString *child;
		while (child = [enumerator nextObject]) {
			[self copyFromMainBundle:fileManager fromPath:[source stringByAppendingPathComponent:child]
							  toPath:[target stringByAppendingPathComponent:child] remove:NO];
		}
	}
	else {
		NSError *error;
		if ([fileManager fileExistsAtPath:target] && ![fileManager removeItemAtPath:target error:&error]) {
			NSAssert2(0, @"Failed to remove '%@': %@", target, [error localizedDescription]);
			return;
		}
		if (![fileManager copyItemAtPath:source toPath:target error:&error]) {
			NSAssert3(0, @"Failed to copy '%@' to '%@': %@", source, target, [error localizedDescription]);
			return;
		}
	}
}



- (BOOL)isContentsEqual:(NSFileManager*)fileManager first:(NSString*)filePath1 second:(NSString*)filePath2 {
    if (ENABLE_STARTUP_TRACES) NSLog(@"RhoAppManager: filePath1: %@", filePath1);
    if (ENABLE_STARTUP_TRACES) NSLog(@"RhoAppManager: filePath2: %@", filePath2);
    if (![fileManager fileExistsAtPath:filePath1] || ![fileManager fileExistsAtPath:filePath2])
        return NO;

    NSString *content1 = [[NSString alloc] initWithData:[fileManager contentsAtPath:filePath1]
                                               encoding:NSUTF8StringEncoding];
    NSString *content2 = [[NSString alloc] initWithData:[fileManager contentsAtPath:filePath2]
                                               encoding:NSUTF8StringEncoding];
    BOOL result = [content1 isEqualToString:content2];
    [content1 release];
    [content2 release];
    return result;
}


/*
 * Configures AppManager
 */
- (void) configure {
    [self configure:YES force_update_content:NO only_apps:NO];
}


- (void) configure:(BOOL)make_sym_links force_update_content:(BOOL)force_update_content only_apps:(BOOL)only_apps {

//#define RHO_DONT_COPY_ON_START

    if (ENABLE_STARTUP_TRACES) NSLog(@"RhoAppManager.configure() START");


    NSFileManager *fileManager = [NSFileManager defaultManager];

	NSString *bundleRoot = [[NSBundle mainBundle] resourcePath];

#ifdef RHO_STANDALONE_LIB
    bundleRoot = [bundleRoot stringByAppendingPathComponent:@"RhoBundle"];
#endif

	NSString *rhoRoot = [NSString stringWithUTF8String:rho_native_rhopath()];
	NSString *rhoUserRoot = [NSString stringWithUTF8String:rho_native_rhouserpath()];
    NSString *rhoDBRoot = [NSString stringWithUTF8String:rho_native_rhodbpath()];

	NSString *filePathNew = [bundleRoot stringByAppendingPathComponent:@"name"];
	NSString *filePathOld = [rhoRoot stringByAppendingPathComponent:@"name"];
//#ifndef RHO_DONT_COPY_ON_START
    BOOL hasOldName = [fileManager fileExistsAtPath:filePathOld];
//#endif
    BOOL nameChanged = ![self isContentsEqual:fileManager first:filePathNew second:filePathOld];

    BOOL restoreSymLinks_only = NO;

    BOOL contentChanged = force_update_content;

    isNewInstallation = !(bool)([fileManager fileExistsAtPath:filePathOld]);
    if (isNewInstallation) {
        if (ENABLE_STARTUP_TRACES) NSLog(@"RhoAppManager.configure() isNewInstallation == true");
    }
    else {
        if (ENABLE_STARTUP_TRACES) NSLog(@"RhoAppManager.configure() isNewInstallation == false");
    }


    // additional check for backup restore
    NSString *db_folder = [rhoDBRoot stringByAppendingPathComponent:@"db"];
    if ([fileManager fileExistsAtPath:db_folder]) {
        if (ENABLE_STARTUP_TRACES) NSLog(@"RhoAppManager.configure() DB folder is exist => SET isNewInstallation = false");
        isNewInstallation = false;
    }

    if (nameChanged) {
        if (ENABLE_STARTUP_TRACES) NSLog(@"RhoAppManager.configure() nameChanged == true");
        contentChanged = YES;
    }
	else {
        if (ENABLE_STARTUP_TRACES) NSLog(@"RhoAppManager.configure() nameChanged == false");
		filePathNew = [bundleRoot stringByAppendingPathComponent:@"hash"];
		filePathOld = [rhoRoot stringByAppendingPathComponent:@"hash"];

        contentChanged = ![self isContentsEqual:fileManager first:filePathNew second:filePathOld];
        // check for lost sym-links (upgrade OS or reinstall application without change version)
        if (!contentChanged) {
            if (ENABLE_STARTUP_TRACES) NSLog(@"RhoAppManager.configure() old hash == new hash");

            // check exist of sym-link
            NSString* testName = [rhoRoot stringByAppendingPathComponent:@"lib"];
            if (![fileManager fileExistsAtPath:testName]) {
                if (ENABLE_STARTUP_TRACES) NSLog(@"RhoAppManager.configure()  Can not found main Sym-Link - we should restore all sym-links !");
                contentChanged = YES;
                restoreSymLinks_only = YES;
            }
            else {
                if (ENABLE_STARTUP_TRACES) NSLog(@"RhoAppManager.configure()  Main Sym-Link founded - disable restoring !");
            }
        }
        else {
            if (ENABLE_STARTUP_TRACES) NSLog(@"RhoAppManager.configure() old hash != new hash");
        }

	}

    NSString* testName = [rhoRoot stringByAppendingPathComponent:@"lib"];
    BOOL libExist = [fileManager fileExistsAtPath:testName];
    if (libExist) {
        if (ENABLE_STARTUP_TRACES) NSLog(@"RhoAppManager.configure()  Lib File is Exist: %@", testName);
    }
    else {
        if (ENABLE_STARTUP_TRACES) NSLog(@"RhoAppManager.configure()  Lib File is NOT Exist: %@", testName);
    }


    if (contentChanged) {
        if (ENABLE_STARTUP_TRACES) NSLog(@"RhoAppManager.configure()  contentChanged == true");
        if (make_sym_links) {
            if (ENABLE_STARTUP_TRACES) NSLog(@"RhoAppManager.configure()  make_sym_links == true");
//#ifdef RHO_DONT_COPY_ON_START
            // we have next situations when we should remove old content:
            // 1. we upgrade old version (where we copy all files)
            //    we should remove all files
            // 2. we upgrade version with symlinks
            //    we should remove only symlinks
            // 3. we should only restore sym-lins after that was cleared - OS upgrade/reinstall app with the same version/restore from bakup etc.
            // we check old "lib" file - if it is SymLink then we have new version of Rhodes (with SymLinks instead of files)

            BOOL isNewVersion = isPathIsSymLink(fileManager, testName);

            RhoFileManagerDelegate_RemoveOnly_SymLinks* myDelegate = nil;
            if (isNewVersion) {
                myDelegate = [[RhoFileManagerDelegate_RemoveOnly_SymLinks alloc] init];
                [fileManager setDelegate:myDelegate];
            }

            NSError *error;

            NSString *appsDocDir = [rhoUserRoot stringByAppendingPathComponent:@"apps"];
            [fileManager createDirectoryAtPath:rhoRoot withIntermediateDirectories:YES attributes:nil error:&error];

            [fileManager createDirectoryAtPath:appsDocDir withIntermediateDirectories:YES attributes:nil error:&error];

            // Create symlink to "lib"
            NSString *src = [bundleRoot stringByAppendingPathComponent:@"lib"];
            if (ENABLE_STARTUP_TRACES) NSLog(@"RhoAppManager: src: %@", src);
            NSString *dst = [rhoRoot stringByAppendingPathComponent:@"lib"];
            if (ENABLE_STARTUP_TRACES) NSLog(@"RhoAppManager: dst: %@", dst);
            [fileManager removeItemAtPath:dst error:&error];

            [fileManager createSymbolicLinkAtPath:dst withDestinationPath:src error:&error];
            //[self copyFromMainBundle:fileManager fromPath:src toPath:dst remove:YES];

            NSString *dirs[] = {@"apps", @"db"};
            for (int i = 0, lim = sizeof(dirs)/sizeof(dirs[0]); i < lim; ++i) {
                // Create directory
                src = [bundleRoot stringByAppendingPathComponent:dirs[i]];
                if (ENABLE_STARTUP_TRACES) NSLog(@"RhoAppManager: src: %@", src);
                dst = [rhoRoot stringByAppendingPathComponent:dirs[i]];
                if (ENABLE_STARTUP_TRACES) NSLog(@"RhoAppManager: dst: %@", dst);
                if (![fileManager fileExistsAtPath:dst])
                    [fileManager createDirectoryAtPath:dst withIntermediateDirectories:YES attributes:nil error:&error];

                // And make symlinks from its content

                NSArray *subelements = [fileManager contentsOfDirectoryAtPath:src error:&error];
                for (int i = 0, lim = [subelements count]; i < lim; ++i) {
                    NSString *child = [subelements objectAtIndex:i];
                    NSString *fchild = [src stringByAppendingPathComponent:child];
                    if (ENABLE_STARTUP_TRACES) NSLog(@"RhoAppManager:  .. src: %@", fchild);
                    NSString *target = [dst stringByAppendingPathComponent:child];
                    if (ENABLE_STARTUP_TRACES) NSLog(@"RhoAppManager:  .. dst: %@", target);
                    [fileManager removeItemAtPath:target error:&error];
                    if ([child isEqualToString:@"rhoconfig.txt"]) {
                        [fileManager setDelegate:nil];
                        [fileManager removeItemAtPath:target error:&error];
                        [fileManager setDelegate:myDelegate];
                        [fileManager copyItemAtPath:fchild toPath:target error:&error];
                    }
                    else {
                        [fileManager createSymbolicLinkAtPath:target withDestinationPath:fchild error:&error];
                    }
                    //[self addSkipBackupAttributeToItemAtURL:target];
                }
            }

            // make symlinks for db files

            [fileManager setDelegate:nil];
            if (myDelegate != nil) {
                [myDelegate release];
            }
            // copy "db"


            NSString* exclude_db[] = {@"syncdb.schema", @"syncdb.triggers", @"syncdb_java.triggers"};

            if (!restoreSymLinks_only && !only_apps) {
                NSString *copy_dirs[] = {@"db"};
                for (int i = 0, lim = sizeof(copy_dirs)/sizeof(copy_dirs[0]); i < lim; ++i) {
                    BOOL remove = nameChanged;
                    if ([copy_dirs[i] isEqualToString:@"db"] && !hasOldName)
                        remove = NO;
                    NSString *src = [bundleRoot stringByAppendingPathComponent:copy_dirs[i]];
                    if (ENABLE_STARTUP_TRACES) NSLog(@"RhoAppManager: copy src: %@", src);
                    NSString *dst = [rhoDBRoot stringByAppendingPathComponent:copy_dirs[i]];
                    if (ENABLE_STARTUP_TRACES) NSLog(@"RhoAppManager: copy dst: %@", dst);

                    //[self copyFromMainBundle:fileManager fromPath:src toPath:dst remove:remove];

                    NSArray *subelements = [fileManager contentsOfDirectoryAtPath:src error:&error];
                    for (int i = 0, lim = [subelements count]; i < lim; ++i) {
                        NSString *child = [subelements objectAtIndex:i];
                        NSString *fchild = [src stringByAppendingPathComponent:child];
                        if (ENABLE_STARTUP_TRACES) NSLog(@"RhoAppManager:  .. copy src: %@", fchild);
                        NSString *target = [dst stringByAppendingPathComponent:child];
                        if (ENABLE_STARTUP_TRACES) NSLog(@"RhoAppManager:  .. copy dst: %@", target);

                        BOOL copyit = YES;

                        int j, jlim;
                        for (j = 0, jlim = sizeof(exclude_db)/sizeof(exclude_db[0]); j < jlim; j++) {
                            if ([child isEqualToString:exclude_db[j]]) {
                                copyit = NO;
                            }
                        }

                        if (copyit) {
                            [fileManager removeItemAtPath:target error:&error];
                            [fileManager copyItemAtPath:fchild toPath:target error:&error];
                        }
                    }

                }
                // Finally, copy "hash" and "name" files
                NSString *items[] = {@"hash", @"name"};
                for (int i = 0, lim = sizeof(items)/sizeof(items[0]); i < lim; ++i) {
                    NSString *src = [bundleRoot stringByAppendingPathComponent:items[i]];
                    if (ENABLE_STARTUP_TRACES) NSLog(@"RhoAppManager: copy src: %@", src);
                    NSString *dst = [rhoRoot stringByAppendingPathComponent:items[i]];
                    if (ENABLE_STARTUP_TRACES) NSLog(@"RhoAppManager: copy dst: %@", dst);
                    [fileManager removeItemAtPath:dst error:&error];
                    [fileManager copyItemAtPath:src toPath:dst error:&error];

                    //[self addSkipBackupAttributeToItemAtURL:dst];
                }
            }

        }
        else {
            NSString *dirs[] = {@"apps", @"lib", @"db", @"hash", @"name"};
            for (int i = 0, lim = sizeof(dirs)/sizeof(dirs[0]); i < lim; ++i) {
                if (!only_apps || [dirs[i] isEqualToString:@"apps"]) {
                    BOOL remove = nameChanged;
                    if ([dirs[i] isEqualToString:@"db"] && !hasOldName)
                        remove = NO;
                    NSString *src = [bundleRoot stringByAppendingPathComponent:dirs[i]];
                    if (ENABLE_STARTUP_TRACES) NSLog(@"RhoAppManager: src: %@", src);
                    NSString *dst = [rhoRoot stringByAppendingPathComponent:dirs[i]];
                    if (ENABLE_STARTUP_TRACES) NSLog(@"RhoAppManager: dst: %@", dst);
                    [self copyFromMainBundle:fileManager fromPath:src toPath:dst remove:remove];
                }
            }
        }
	}
    //if (ENABLE_STARTUP_TRACES) NSLog(@"RhoAppManager: Create rhodes logging and rhoconfig.txt loading");
	//rho_logconf_Init_with_separate_user_path(rho_native_rhopath(), rho_native_rhopath(), "", rho_native_rhouserpath());
    //if (ENABLE_STARTUP_TRACES) NSLog(@"RhoAppManager: Create rhodes app");
	//rho_rhodesapp_create_with_separate_user_path(rho_native_rhopath(), rho_native_rhouserpath());
	//RAWLOG_INFO("Rhodes started");
}


- (UIViewController *) documentInteractionControllerViewControllerForPreview: (UIDocumentInteractionController *) controller {

    return [[[Rhodes sharedInstance] mainView] getMainViewController];

}

- (void)openDocInteractCommand:(NSString*)url {
    if (NSClassFromString(@"UIDocumentInteractionController")) {
        NSURL *fileURL = [NSURL fileURLWithPath:url];

        UIDocumentInteractionController* docController = [UIDocumentInteractionController interactionControllerWithURL:fileURL];

        [docController retain];
        docController.delegate = self;//[AppManager instance];

        BOOL result = [docController presentPreviewAnimated:YES];
        [docController autorelease];

        if (!result) {
            //[docController retain];
            CGPoint centerPoint = [Rhodes sharedInstance].window.center;
            CGRect centerRec = CGRectMake(centerPoint.x, centerPoint.y, 0, 0);
            BOOL isValid = [docController presentOpenInMenuFromRect:centerRec inView:[Rhodes sharedInstance].window animated:YES];
            //[docController autorelease];
        }
    }
}

- (void)documentInteractionControllerDidEndPreview:(UIDocumentInteractionController *)docController
{
	//[docController autorelease];
    //[docController release];
    //docController = nil;
}

- (void)openDocInteract:(NSString*)url {
	[self performSelectorOnMainThread:@selector(openDocInteractCommand:) withObject:[url retain] waitUntilDone:NO];
}


- (void) openURLComand:(NSString*)url {

    RAWLOG_INFO1("rho_sys_open_url: %s", [url UTF8String]);

	NSString* strUrl = url;//[NSString stringWithUTF8String:url];
	BOOL res = FALSE;

    BOOL fileExists = [[NSFileManager defaultManager] fileExistsAtPath:strUrl];
    if (!fileExists) {
        NSString *fixed_path = [NSString stringWithUTF8String:rho_rhodesapp_getapprootpath()];
        fixed_path = [fixed_path stringByAppendingString:strUrl];
        fileExists = [[NSFileManager defaultManager] fileExistsAtPath:fixed_path];
        if (fileExists) {
            strUrl = fixed_path;
        }
    }
    if (fileExists) {
        res = TRUE;
        [[AppManager instance] openDocInteract:strUrl];
    }
    else {
#ifdef __IPHONE_8_0
        if (SYSTEM_VERSION_GREATER_THAN_OR_EQUAL_TO(@"9.0")) {
            if ([strUrl rangeOfString:@":"].location == NSNotFound) {
                //Append : for backward compatibility
                strUrl = [strUrl stringByAppendingString:@":"];
            } else {
                //Do nothing
            }
        }
#endif

#ifdef __IPHONE_10_0
    
        NSURL *url = [NSURL URLWithString:strUrl];
        UIApplication *application = [UIApplication sharedApplication];
        if ([application canOpenURL:url]) {
            [application openURL:url options:@{} completionHandler:^(BOOL success) {
            if (success) {
                const char* full_url = [strUrl UTF8String];
                RAWLOG_INFO1("URL is opened [%s]", full_url);
            }
            else {
                const char* full_url = [strUrl UTF8String];
                RAWLOG_ERROR1("URL is NOT opened [%s]", full_url);
            }
            }];
        }

#else
        if ([[UIApplication sharedApplication] canOpenURL:[NSURL URLWithString:strUrl]]) {
            res = [[UIApplication sharedApplication] openURL:[NSURL URLWithString:strUrl]];
        }
#endif
    }
	if ( res)
		RAWLOG_INFO("rho_sys_open_url suceeded.");
	else
		RAWLOG_INFO("rho_sys_open_url failed.");
}


- (void) openURL:(NSString*)url {
    [self performSelectorOnMainThread:@selector(openURLComand:) withObject:[url retain] waitUntilDone:NO];
}



@end



const char* getUserPath() {
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

const char* rho_native_rhopath()
{
	static bool loaded = FALSE;
	static char root[FILENAME_MAX];
	if (!loaded){

        NSArray *paths = NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES);
        NSString *rootDirectory = [ [paths objectAtIndex:0] stringByAppendingString:@"/Private Documents/"];

        const char* svalue = get_app_build_config_item("iphone_set_approot");
        if (svalue != NULL) {
            NSString* value = [NSString stringWithUTF8String:svalue];

            if ([value compare:@"Documents" options:NSCaseInsensitiveSearch] == NSOrderedSame) {
                paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
                rootDirectory = [ [paths objectAtIndex:0] stringByAppendingString:@"/"];
            }
            else if ([value compare:@"Library_Caches" options:NSCaseInsensitiveSearch] == NSOrderedSame) {
                paths = NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES);
                rootDirectory = [ [paths objectAtIndex:0] stringByAppendingString:@"/Private Documents/"];
            }
            else if ([value compare:@"Library_Private_Documents" options:NSCaseInsensitiveSearch] == NSOrderedSame) {
                paths = NSSearchPathForDirectoriesInDomains(NSLibraryDirectory, NSUserDomainMask, YES);
                rootDirectory = [ [paths objectAtIndex:0] stringByAppendingString:@"/Private Documents/"];
            }
        }

        [rootDirectory getFileSystemRepresentation:root maxLength:sizeof(root)];
		loaded = TRUE;
	}

	return root;
}

const char* rho_native_reruntimepath()
{
    return rho_native_rhopath();
}

const char* rho_native_rhouserpath()
{
    BOOL user_path_in_root = NO;
    const char* svalue = get_app_build_config_item("iphone_userpath_in_approot");
    if (svalue != NULL) {
        user_path_in_root = svalue[0] != '0';
    }

    if (user_path_in_root) {
        return rho_native_rhopath();
    }

    return getUserPath();
}

const char* rho_native_rhodbpath()
{
    BOOL db_path_in_root = NO;
    const char* svalue = get_app_build_config_item("iphone_db_in_approot");
    if (svalue != NULL) {
        db_path_in_root = svalue[0] != '0';
    }

    if (db_path_in_root) {
        return rho_native_rhopath();
    }
    return rho_native_rhouserpath();
}




NSString* rho_sys_get_locale_iphone()
{
    NSArray* ar = [NSLocale preferredLanguages];
    NSString *preferredLang = [ar objectAtIndex:0];

    NSDictionary *languageDic = [NSLocale componentsFromLocaleIdentifier:preferredLang];
    //NSString *countryCode = [languageDic objectForKey:@"kCFLocaleCountryCodeKey"];
    NSString *languageCode = [languageDic objectForKey:@"kCFLocaleLanguageCodeKey"];
    if (languageCode == nil) {
        languageCode = preferredLang;
    }

	return languageCode;
}



VALUE rho_sys_get_locale()
{
	NSString *preferredLang = rho_sys_get_locale_iphone();

	return rho_ruby_create_string( [preferredLang UTF8String] );
}



int rho_sys_get_screen_width()
{
    CGRect rect = [[UIScreen mainScreen] bounds];
    UIInterfaceOrientation current_orientation = [[UIApplication sharedApplication] statusBarOrientation];
	if ((current_orientation == UIInterfaceOrientationLandscapeLeft) || (current_orientation == UIInterfaceOrientationLandscapeRight)) {
        return rect.size.height;
	}
    return rect.size.width;
}

int rho_sys_get_screen_height()
{
    CGRect rect = [[UIScreen mainScreen] bounds];
    UIInterfaceOrientation current_orientation = [[UIApplication sharedApplication] statusBarOrientation];
	if ((current_orientation == UIInterfaceOrientationLandscapeLeft) || (current_orientation == UIInterfaceOrientationLandscapeRight)) {
        return rect.size.width;
	}
    return rect.size.height;
}

int rho_sys_set_sleeping(int sleeping)
{
	int ret = [[UIApplication sharedApplication] isIdleTimerDisabled] ? 0 : 1;
	[[UIApplication sharedApplication] setIdleTimerDisabled: (!sleeping ? YES : NO)];
    return ret;
}

void rho_sys_app_exit() {
    if (([Rhodes sharedInstance] != nil) && (![Rhodes sharedInstance].mBlockExit)) {
        exit(EXIT_SUCCESS);
    }
}

int rho_sys_is_app_installed(const char *appname) {
	NSString* app_name = [NSString stringWithUTF8String:appname];
	app_name = [app_name stringByAppendingString:@":check_for_exist"];
	NSURL* nsurl = [NSURL URLWithString:app_name];
	if ([[UIApplication sharedApplication] canOpenURL:nsurl]) {
		return 1;
	}
	return 0;
}

void rho_sys_app_uninstall(const char *appname) {
	RAWLOG_ERROR("ERROR: Uninstall of applications is unsupported on iOS platfrom !!!");
}


void rho_sys_open_url(const char* url)
{
    [[AppManager instance] openURL:[NSString stringWithUTF8String:url]];
}

BOOL rho_sys_app_install(const char *url) {
    //rho_sys_open_url(url);
    const char* full_url = url;
    RAWLOG_INFO1("rho_sys_app_install: %s", full_url);

	BOOL res = FALSE;

    if ([[UIApplication sharedApplication] canOpenURL:[NSURL URLWithString:[NSString stringWithUTF8String:full_url]]]) {
        res = [[UIApplication sharedApplication] openURL:[NSURL URLWithString:[NSString stringWithUTF8String:full_url]]];
    }

	if ( res)
		RAWLOG_INFO("rho_sys_app_install suceeded.");
	else
		RAWLOG_INFO("rho_sys_app_install failed.");

    return res;
}



BOOL rho_sys_run_app_iphone(const char* appname, char* params) {
	NSString* app_name = [NSString stringWithUTF8String:appname];
	app_name = [app_name stringByAppendingString:@":"];

	if (params != NULL) {
        NSString* param = [NSString stringWithUTF8String:params];
        app_name = [app_name stringByAppendingString:param];
	}
    const char* full_url = [app_name UTF8String];
    RAWLOG_INFO1("rho_sys_run_app: %s", full_url);


	BOOL res = FALSE;

    if ([[UIApplication sharedApplication] canOpenURL:[NSURL URLWithString:app_name]]) {
        res = [[UIApplication sharedApplication] openURL:[NSURL URLWithString:app_name]];
    }

	if ( res)
		RAWLOG_INFO("rho_sys_run_app suceeded.");
	else
		RAWLOG_INFO("rho_sys_run_app failed.");

    return res;
}

void rho_sys_run_app(const char* appname, VALUE params)
{
    char* str_params = NULL;
    if (params != 0) {
       char* parameter = getStringFromValue(params);
        if (parameter != NULL) {
            str_params = parameter;
        }
    }
    rho_sys_run_app_iphone(appname, str_params);
}





void rho_sys_bring_to_front()
{
    RAWLOG_INFO("rho_sys_bring_to_front has no implementation on iPhone.");
}

void rho_sys_report_app_started()
{
    RAWLOG_INFO("rho_sys_report_app_started has no implementation on iPhone.");
}


extern VALUE rho_sys_has_network();

// http://www.apple.com/iphone/specs.html
static const double RHO_IPHONE_PPI = 163.0;
static const double RHO_IPHONE4_PPI = 326.0;
// http://www.apple.com/ipad/specs/
static const double RHO_IPAD_PPI = 132.0;
static const double RHO_NEW_IPAD_PPI = 264.0;
static const double RHO_IPAD_MINI_PPI = 163.0;
static const double RHO_IPAD_MINI_RETINA_PPI = 326.0;

static float get_scale() {
    float scales = 1;//[[UIScreen mainScreen] scale];
#ifdef __IPHONE_4_0
    if ( [[UIScreen mainScreen] respondsToSelector:@selector(scale)] ) {
        scales = [[UIScreen mainScreen] scale];
    }
#endif
    return scales;
}



/*
 "iPod5,1"                                  = "iPod Touch 5"
 "iPod7,1"                                  = "iPod Touch 6"
 "iPhone3,1", "iPhone3,2", "iPhone3,3"      = "iPhone 4"
 "iPhone4,1"                                = "iPhone 4s"
 "iPhone5,1", "iPhone5,2"                   = "iPhone 5"
 "iPhone5,3", "iPhone5,4"                   = "iPhone 5c"
 "iPhone6,1", "iPhone6,2"                   = "iPhone 5s"
 "iPhone7,2"                                = "iPhone 6"
 "iPhone7,1"                                = "iPhone 6 Plus"
 "iPhone8,1"                                = "iPhone 6s"
 "iPhone8,2"                                = "iPhone 6s Plus"
 "iPhone9,1", "iPhone9,3"                   = "iPhone 7"
 "iPhone9,2", "iPhone9,4"                   = "iPhone 7 Plus"
 "iPhone8,4"                                = "iPhone SE"
 "iPhone10,1", "iPhone10,4"                 = "iPhone 8"
 "iPhone10,2", "iPhone10,5"                 = "iPhone 8 Plus"
 "iPhone10,3", "iPhone10,6"                 = "iPhone X"
 "iPhone11,2"                               = "iPhone XS"
 "iPhone11,4", "iPhone11,6"                 = "iPhone XS MAX"
 "iPhone11,8"                               = "iPhone XR"
 "iPad2,1", "iPad2,2", "iPad2,3", "iPad2,4" = "iPad 2"
 "iPad3,1", "iPad3,2", "iPad3,3"            = "iPad 3"
 "iPad3,4", "iPad3,5", "iPad3,6"            = "iPad 4"
 "iPad4,1", "iPad4,2", "iPad4,3"            = "iPad Air"
 "iPad5,3", "iPad5,4"                       = "iPad Air 2"
 "iPad6,11", "iPad6,12"                     = "iPad 5"
 "iPad7,5", "iPad7,6"                       = "iPad 6"
 "iPad2,5", "iPad2,6", "iPad2,7"            = "iPad Mini"
 "iPad4,4", "iPad4,5", "iPad4,6"            = "iPad Mini 2"
 "iPad4,7", "iPad4,8", "iPad4,9"            = "iPad Mini 3"
 "iPad5,1", "iPad5,2"                       = "iPad Mini 4"
 "iPad6,3", "iPad6,4"                       = "iPad Pro 9.7 Inch"
 "iPad6,7", "iPad6,8"                       = "iPad Pro 12.9 Inch"
 "iPad7,1", "iPad7,2"                       = "iPad Pro 12.9 Inch 2. Generation"
 "iPad7,3", "iPad7,4"                       = "iPad Pro 10.5 Inch"
 "AppleTV5,3"                               = "Apple TV"
 "AppleTV6,2"                               = "Apple TV 4K"
 "AudioAccessory1,1"                        = "HomePod"
 "AppleTV5,3"  return "Apple TV 4"
 "AppleTV6,2"  = "Apple TV 4K"
 */

NSString* getDeviceIdentifier() {
#if TARGET_IPHONE_SIMULATOR
    NSString* simmodel = [[[NSProcessInfo processInfo] environment] objectForKey:@"SIMULATOR_MODEL_IDENTIFIER"];
    if (simmodel != nil) {
        return simmodel;
    }
#endif
    size_t size;
    sysctlbyname("hw.machine", NULL, &size, NULL, 0);
    char *machine = malloc(size);
    sysctlbyname("hw.machine", machine, &size, NULL, 0);
    NSString *platform = [NSString stringWithUTF8String:machine];
    free(machine);
    return platform;
}





float getPPI() {
    NSString* model = getDeviceIdentifier();


    if (
        [model isEqualToString:@"iPad2,1"] ||
        [model isEqualToString:@"iPad2,2"] ||
        [model isEqualToString:@"iPad2,3"] ||
        [model isEqualToString:@"iPad2,4"]
        ) {
        return 132.0;
    }
    if (
        [model isEqualToString:@"iPad2,5"] ||
        [model isEqualToString:@"iPad2,6"] ||
        [model isEqualToString:@"iPad2,7"]
        ) {
        return 163.0;
    }
    if (
        [model isEqualToString:@"iPad3,1"] ||
        [model isEqualToString:@"iPad3,2"] ||
        [model isEqualToString:@"iPad3,3"] ||
        [model isEqualToString:@"iPad3,4"] ||
        [model isEqualToString:@"iPad3,5"] ||
        [model isEqualToString:@"iPad3,6"] ||
        [model isEqualToString:@"iPad4,1"] ||
        [model isEqualToString:@"iPad4,2"] ||
        [model isEqualToString:@"iPad4,3"] ||
        [model isEqualToString:@"iPad5,3"] ||
        [model isEqualToString:@"iPad5,4"] ||
        [model isEqualToString:@"iPad6,3"] ||
        [model isEqualToString:@"iPad6,4"] ||
        [model isEqualToString:@"iPad6,7"] ||
        [model isEqualToString:@"iPad6,8"] ||
        [model isEqualToString:@"iPad6,11"] ||
        [model isEqualToString:@"iPad6,12"] ||
        [model isEqualToString:@"iPad7,1"] ||
        [model isEqualToString:@"iPad7,2"] ||
        [model isEqualToString:@"iPad7,3"] ||
        [model isEqualToString:@"iPad7,4"] ||
        [model isEqualToString:@"iPad7,5"] ||
        [model isEqualToString:@"iPad7,6"]
        ) {
        return 264.0;
    }
    if (
        [model isEqualToString:@"iPhone4,1"] ||
        [model isEqualToString:@"iPhone5,1"] ||
        [model isEqualToString:@"iPhone5,2"] ||
        [model isEqualToString:@"iPhone5,3"] ||
        [model isEqualToString:@"iPhone5,4"] ||
        [model isEqualToString:@"iPhone6,1"] ||
        [model isEqualToString:@"iPhone6,2"] ||
        [model isEqualToString:@"iPhone7,2"] ||
        [model isEqualToString:@"iPhone8,1"] ||
        [model isEqualToString:@"iPhone8,4"] ||
        [model isEqualToString:@"iPhone9,1"] ||
        [model isEqualToString:@"iPhone9,3"] ||
        [model isEqualToString:@"iPhone10,1"] ||
        [model isEqualToString:@"iPhone10,4"] ||
        [model isEqualToString:@"iPhone11,8"] ||
        [model isEqualToString:@"iPad4,4"] ||
        [model isEqualToString:@"iPad4,5"] ||
        [model isEqualToString:@"iPad4,6"] ||
        [model isEqualToString:@"iPad4,7"] ||
        [model isEqualToString:@"iPad4,8"] ||
        [model isEqualToString:@"iPad4,9"] ||
        [model isEqualToString:@"iPad5,1"] ||
        [model isEqualToString:@"iPad5,2"] ||
        [model isEqualToString:@"iPod5,1"] ||
        [model isEqualToString:@"iPod7,1"]
        ) {
        return 326.0;
    }
    if (
        [model isEqualToString:@"iPhone7,1"] ||
        [model isEqualToString:@"iPhone8,2"] ||
        [model isEqualToString:@"iPhone9,2"] ||
        [model isEqualToString:@"iPhone9,4"] ||
        [model isEqualToString:@"iPhone10,2"] ||
        [model isEqualToString:@"iPhone10,5"]
        ) {
        return 401.0;
    }
    if (
        [model isEqualToString:@"iPhone10,3"] ||
        [model isEqualToString:@"iPhone10,6"] ||
        [model isEqualToString:@"iPhone11,2"] ||
        [model isEqualToString:@"iPhone11,4"] ||
        [model isEqualToString:@"iPhone11,6"]
        ) {
        return 458.0;
    }
    // unknown device !
    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
        if (get_scale() > 1.2) {
            return 264.0;
        }
        else {
            return 132.0;
        }
    }
    if (get_scale() > 2.2) {
        return 458.0;
    }
    return 326.0;
}






extern BOOL rho_sys_has_wifi_network_iphone();
extern BOOL rho_sys_has_cell_network_iphone();
extern BOOL rho_sys_has_network_iphone();


int rho_sysimpl_get_property_iphone(char* szPropName, NSObject** resValue)
{
    if (strcasecmp("platform", szPropName) == 0)
    {*resValue = [NSString stringWithUTF8String:"APPLE"]; return 1;}
    else if (strcasecmp("phone_id", szPropName) == 0){
        NSString* uniqueIdentifier = nil;
        if( [UIDevice instancesRespondToSelector:@selector(identifierForVendor)] ) {
            // iOS 6+
            uniqueIdentifier = [[[UIDevice currentDevice] identifierForVendor] UUIDString];
        } else {
            // before iOS 6, so just generate an identifier and store it
            uniqueIdentifier = [[NSUserDefaults standardUserDefaults] objectForKey:@"identiferForVendor"];
            if( !uniqueIdentifier ) {
                CFUUIDRef uuid = CFUUIDCreate(NULL);
                uniqueIdentifier = (NSString*)CFUUIDCreateString(NULL, uuid);
                CFRelease(uuid);
                [[NSUserDefaults standardUserDefaults] setObject:uniqueIdentifier forKey:@"identifierForVendor"];
            }
        }
        *resValue = uniqueIdentifier;
        return 1;
    }
    else if (strcasecmp("locale", szPropName) == 0)
    {*resValue = rho_sys_get_locale_iphone(); return 1; }
    else if (strcasecmp("country", szPropName) == 0) {
        NSLocale *locale = [NSLocale currentLocale];
        NSString *cl = [locale objectForKey:NSLocaleCountryCode];
        *resValue = cl;
        return 1;
    }
    else if (strcasecmp("screen_width", szPropName) == 0)
    {*resValue = [NSNumber numberWithInt:rho_sys_get_screen_width()]; return 1; }
    else if (strcasecmp("screen_height", szPropName) == 0)
    {*resValue = [NSNumber numberWithInt:rho_sys_get_screen_height()]; return 1; }
    else if (strcasecmp("real_screen_height", szPropName) == 0)
    {

        *resValue = [NSNumber numberWithInt:((int)(rho_sys_get_screen_height()*get_scale()))];
        return 1;
    }
    else if (strcasecmp("real_screen_width", szPropName) == 0)
    {
        *resValue = [NSNumber numberWithInt:((int)(rho_sys_get_screen_width()*get_scale()))];
        return 1;
    }
    else if (strcasecmp("screen_orientation", szPropName) == 0) {
        UIInterfaceOrientation current_orientation = [[UIApplication sharedApplication] statusBarOrientation];
        if ((current_orientation == UIInterfaceOrientationLandscapeLeft) || (current_orientation == UIInterfaceOrientationLandscapeRight)) {
            *resValue = @"landscape";
        }
        else {
            *resValue = @"portrait";
        }
        return 1;
    }
    else if (strcasecmp("has_network", szPropName) == 0)
    {*resValue = [NSNumber numberWithBool:rho_sys_has_network_iphone()]; return 1; }
    else if (strcasecmp("has_wifi_network", szPropName) == 0)
    {*resValue = [NSNumber numberWithBool:rho_sys_has_wifi_network_iphone()]; return 1; }
    else if (strcasecmp("has_cell_network", szPropName) == 0)
    {*resValue = [NSNumber numberWithBool:rho_sys_has_cell_network_iphone()]; return 1; }
    else if (strcasecmp("has_camera", szPropName) == 0) {
        int has_camera = [UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypeCamera];
        *resValue = [NSNumber numberWithBool:(has_camera != 0)];
        return 1;
    }
    else if (strcasecmp("ppi_x", szPropName) == 0 ||
             strcasecmp("ppi_y", szPropName) == 0) {
        *resValue = [NSNumber numberWithDouble:getPPI()];
        return 1;
    }
    else if (strcasecmp("device_name", szPropName) == 0) {
        NSString *model = [[UIDevice currentDevice] model];
        *resValue = model;
        return 1;
    }
    else if (strcasecmp("os_version", szPropName) == 0) {
        NSString *version = [[UIDevice currentDevice] systemVersion];
        *resValue = version;
        return 1;
    }
    else if (strcasecmp("webview_framework", szPropName) == 0) {
        NSString *version = [[UIDevice currentDevice] systemVersion];
        NSString *wvf = @"WEBKIT/";
        wvf = [wvf stringByAppendingString:version];
        *resValue = wvf;
        return 1;
    }
    else if (strcasecmp("is_emulator", szPropName) == 0) {
        int bSim = 0;
#if TARGET_IPHONE_SIMULATOR
		bSim = 1;
#endif
		*resValue = [NSNumber numberWithBool:(bSim != 0)];
        return 1;
    }else if (strcasecmp("has_calendar", szPropName) == 0) {
		int bCal = 0;
		if (is_rho_calendar_supported())
			bCal = 1;
		*resValue = [NSNumber numberWithBool:(bCal != 0)];
		return 1;
	}
    return 0;
}



int rho_sysimpl_get_property(char* szPropName, VALUE* resValue)
{
    NSObject* result = nil;
    if (rho_sysimpl_get_property_iphone(szPropName, &result)) {
        *resValue = [CRubyConverter convertToRuby:result];
        return 1;
    }
    return 0;
}



/*
int rho_sysimpl_get_property(char* szPropName, VALUE* resValue)
{
    if (strcasecmp("platform", szPropName) == 0)
        {*resValue = rho_ruby_create_string("APPLE"); return 1;}
    else if (strcasecmp("locale", szPropName) == 0)
        {*resValue = rho_sys_get_locale(); return 1; }
    else if (strcasecmp("country", szPropName) == 0) {
        NSLocale *locale = [NSLocale currentLocale];
        NSString *cl = [locale objectForKey:NSLocaleCountryCode];
        *resValue = rho_ruby_create_string([cl UTF8String]);
        return 1;
    }
    else if (strcasecmp("screen_width", szPropName) == 0)
        {*resValue = rho_ruby_create_integer(rho_sys_get_screen_width()); return 1; }
    else if (strcasecmp("screen_height", szPropName) == 0)
        {*resValue = rho_ruby_create_integer(rho_sys_get_screen_height()); return 1; }
    else if (strcasecmp("real_screen_height", szPropName) == 0)
    {

        *resValue = rho_ruby_create_integer((int)(rho_sys_get_screen_height()*get_scale()));
        return 1;
    }
    else if (strcasecmp("real_screen_width", szPropName) == 0)
    {
        *resValue = rho_ruby_create_integer((int)(rho_sys_get_screen_width()*get_scale()));
        return 1;
    }
    else if (strcasecmp("screen_orientation", szPropName) == 0) {
        UIInterfaceOrientation current_orientation = [[UIApplication sharedApplication] statusBarOrientation];
        if ((current_orientation == UIInterfaceOrientationLandscapeLeft) || (current_orientation == UIInterfaceOrientationLandscapeRight)) {
            *resValue = rho_ruby_create_string("landscape");
        }
        else {
            *resValue = rho_ruby_create_string("portrait");
        }
        return 1;
    }
    else if (strcasecmp("has_network", szPropName) == 0)
        {*resValue = rho_sys_has_network(); return 1; }
    else if (strcasecmp("has_wifi_network", szPropName) == 0)
    {*resValue = rho_sys_has_wifi_network(); return 1; }
    else if (strcasecmp("has_cell_network", szPropName) == 0)
    {*resValue = rho_sys_has_cell_network(); return 1; }
    else if (strcasecmp("has_camera", szPropName) == 0) {
        int has_camera = [UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypeCamera];
        *resValue = rho_ruby_create_boolean(has_camera);
        return 1;
    }
    else if (strcasecmp("ppi_x", szPropName) == 0 ||
             strcasecmp("ppi_y", szPropName) == 0) {
#ifdef __IPHONE_3_2
        if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
            if (get_scale() > 1.2) {
                *resValue = rho_ruby_create_double(RHO_NEW_IPAD_PPI);
            }
            else {
                *resValue = rho_ruby_create_double(RHO_IPAD_PPI);
            }
            return 1;
        }
#endif
        if (get_scale() > 1.2) {
            *resValue = rho_ruby_create_double(RHO_IPHONE4_PPI);
        }
        else {
            *resValue = rho_ruby_create_double(RHO_IPHONE_PPI);
        }
        return 1;
    }
    else if (strcasecmp("device_name", szPropName) == 0) {
        NSString *model = [[UIDevice currentDevice] model];
        *resValue = rho_ruby_create_string([model UTF8String]);
        return 1;
    }
    else if (strcasecmp("os_version", szPropName) == 0) {
        NSString *version = [[UIDevice currentDevice] systemVersion];
        *resValue = rho_ruby_create_string([version UTF8String]);
        return 1;
    }
    else if (strcasecmp("webview_framework", szPropName) == 0) {
        NSString *version = [[UIDevice currentDevice] systemVersion];
        NSString *wvf = @"WEBKIT/";
        wvf = [wvf stringByAppendingString:version];
        *resValue = rho_ruby_create_string([wvf UTF8String]);
        return 1;
    }
    else if (strcasecmp("is_emulator", szPropName) == 0) {
        int bSim = 0;
#if TARGET_IPHONE_SIMULATOR
		bSim = 1;
#endif
		*resValue = rho_ruby_create_boolean(bSim);
        return 1;
    }else if (strcasecmp("has_calendar", szPropName) == 0) {
		int bCal = 0;
		if (is_rho_calendar_supported())
			bCal = 1;
		*resValue = rho_ruby_create_boolean(bCal);
		return 1;
	}

	//// "device_id" property used only for PUSH technology !
	// else if (strcasecmp("device_id", szPropName) == 0) {
	//	NSString* uuid = [[UIDevice currentDevice] uniqueIdentifier];
    //    *resValue = rho_ruby_create_string([uuid UTF8String]);
    //    return 1;
	//}




	// [[UIDevice currentDevice] uniqueIdentifier]
    //// Removed because it's possibly dangerous: Apple could reject application
    //// used such approach from its AppStore
    //else if (strcasecmp("phone_number", szPropName) == 0) {
    //    NSString *num = [[NSUserDefaults standardUserDefaults] stringForKey:@"SBFormattedPhoneNumber"];
    //    if (!num)
    //        return 0;
    //    *resValue =  rho_ruby_create_string([num UTF8String]);
    //    return 1;
    //}


    return 0;
}

*/










const char* GetApplicationsRootPath() {
	static bool loaded = FALSE;
	static char root[FILENAME_MAX];
	if (!loaded) {
		NSString *rootDirectory = [AppManager getApplicationsRootPath];
		[rootDirectory getFileSystemRepresentation:root maxLength:sizeof(root)];
		loaded = TRUE;
	}
	return root;
}

// TODO - do error checking
bool UnzipApplication(const char* appRoot, const void* zipbuf, unsigned int ziplen) {

	ZIPENTRY ze;
    // Open zip file
	HZIP hz = OpenZip((void*)zipbuf, ziplen, 0);

	// Set base for unziping
	SetUnzipBaseDir(hz, appRoot);

	// Get info about the zip
	// -1 gives overall information about the zipfile
	GetZipItem(hz,-1,&ze);
	int numitems = ze.index;

	// Iterate through items and unzip them
	for (int zi = 0; zi<numitems; zi++)
	{
		//ZIPENTRY ze;
		// fetch individual details, e.g. the item's name.
		GetZipItem(hz,zi,&ze);
		// unzip item
		UnzipItem(hz, zi, ze.name);
	}

	CloseZip(hz);

	return true;
}
/*
int _LoadApp(HttpContextRef context) {

	if ( (context->_request->_query!=NULL) &&
		(strlen(context->_request->_query)>0) ) {

		AppLoader* appLoader = [[AppLoader alloc] init];
		bool ret = [appLoader loadApplication:[NSString stringWithCString:context->_request->_query]];
		[appLoader release];
		if (!ret) {
			HttpSendErrorToTheServer(context, 500, "Error loading application");
			return -1;
		}

		char location[strlen(context->_request->_query)+2];
		HttpSnprintf(location, sizeof(location), "/%s", context->_request->_query);
		return HTTPRedirect(context, location);
	}

	HttpSendErrorToTheServer(context, 400, "Application name to load and install is not specifyed");
	return -1;
}*/

void rho_appmanager_load( void* httpContext, const char* szQuery)
{
	if ( !szQuery || !*szQuery )
	{
		rho_http_senderror(httpContext, 400, "Application name to load and install is not specifyed");
		return;
	}

	AppLoader* appLoader = [[AppLoader alloc] init];
	bool ret = [appLoader loadApplication:[NSString stringWithUTF8String:szQuery]];
	[appLoader release];
	if (!ret) {
		rho_http_senderror(httpContext, 500, "Error loading application");
		return;
	}

	char location[strlen(szQuery)+2];
	rho_http_snprintf(location, sizeof(location), "/%s", szQuery);
    rho_http_redirect(httpContext, location);

	return;
}


#define SYSTEM_VERSION_LESS_THAN(v) ([[[UIDevice currentDevice] systemVersion] compare:v options:NSNumericSearch] == NSOrderedAscending)

#ifdef __IPHONE_8_0

BOOL checkNotificationType(UIUserNotificationType type)
{
    UIUserNotificationSettings *currentSettings = [[UIApplication sharedApplication] currentUserNotificationSettings];

    return (currentSettings.types & type);
}

#endif

void setApplicationBadgeNumber(NSInteger badgeNumber)
{
    [[Rhodes sharedInstance] registerForRemoteNotification];

    UIApplication *application = [UIApplication sharedApplication];

#ifdef __IPHONE_8_0
    // compile with Xcode 6 or higher (iOS SDK >= 8.0)

    if(SYSTEM_VERSION_LESS_THAN(@"8.0"))
    {
        [UIApplication sharedApplication].applicationIconBadgeNumber = badgeNumber;
    }
    else
    {
        if (checkNotificationType(UIUserNotificationTypeBadge))
        {
            RAWLOG_INFO1("badge number changed to %d", (int)badgeNumber);
            [UIApplication sharedApplication].applicationIconBadgeNumber = badgeNumber;
        }
        else
            RAWLOG_INFO("access denied for UIUserNotificationTypeBadge");
    }
#else
    // compile with Xcode 5 (iOS SDK < 8.0)
    [UIApplication sharedApplication].applicationIconBadgeNumber = badgeNumber;

#endif
}

void rho_sys_set_application_icon_badge(int badge_number) {
    int badge_number_value = badge_number;
    dispatch_async(dispatch_get_main_queue(), ^{
      setApplicationBadgeNumber(badge_number_value);
    });
}

void rho_platform_restart_application() {
//    [Rhodes restart_app];
}

//#ifndef __IPHONE_5_1
//#define NSURLIsExcludedFromBackupKey @"NSURLIsExcludedFromBackupKey"
//#endif

int rho_sys_set_do_not_bakup_attribute(const char* path, int value) {
    const char* attrName = "com.apple.MobileBackup";
    NSString* pathString = [NSString stringWithUTF8String:path];
    u_int8_t attrValue = value;
    int result = -1;

    // HACK !!!
    // We can not use NSURLIsExcludedFromBackupKey external const from CoreFoundation because app crash on load stage (can not resolve links)
    // So if we want to launch on 5.0 iOS we must hardcore this const value to @"NSURLIsExcludedFromBackupKey"
    if (SYSTEM_VERSION_LESS_THAN_OR_EQUAL_TO(@"5.1")  /*|| &NSURLIsExcludedFromBackupKey == nil*/ ) {
        // iOS 5.0.1 and lower
        result = setxattr(path, attrName, &attrValue, sizeof(attrValue), 0, 0);

        if (result != 0) {
            RAWLOG_WARNING1("WARNING: Can not change [do_not_bakup] attribute for path: %s", path);
        }
    }
    else {

        // Remove old style attribute if it exists
        int result = getxattr(path, attrName, NULL, sizeof(u_int8_t), 0, 0);
        if (result != -1) {
            int removeResult = removexattr(path, attrName, 0);
            if (removeResult == 0) {
                RAWLOG_WARNING1("Removed extended attribute on file %s", [pathString UTF8String]);
            }
        }

        NSError *error = nil;
        NSURL* url = [NSURL fileURLWithPath:pathString];
        BOOL success = [url setResourceValue: [NSNumber numberWithBool: (value == 1)]
                                      forKey: @"NSURLIsExcludedFromBackupKey"
                                       error: &error];

        result = success ? 0 : -1;

        if (result != 0) {
            RAWLOG_WARNING2("Can not change [do_not_bakup] attribute for path: %s, error: %d", [pathString UTF8String], (int)[error localizedDescription]);
        }
    }


    return (int)(result == 0);
}


int rho_prepare_folder_for_upgrade(const char* szPath) {
    // replace all folders/files to real folder/files in this path
    NSFileManager *fileManager = [NSFileManager defaultManager];

    NSString* main_path = [NSString stringWithUTF8String:szPath];

    NSDirectoryEnumerator *enumerator = [fileManager enumeratorAtPath:main_path];
    NSString *child;
    while (child = [enumerator nextObject]) {
        // check for sym_link
        NSString* child_path = [main_path stringByAppendingPathComponent:child];
        if (isPathIsSymLink(fileManager, child_path) ) {

            NSError *error;

            NSString* tmp_path = [main_path stringByAppendingPathComponent:@"temporary_path_for_copying_sym_link"];

            [[AppManager instance] copyFromMainBundle:fileManager fromPath:child_path toPath:tmp_path remove:NO];
            if ([fileManager removeItemAtPath:child_path error:&error] != YES) {
                return 0;
            }
            if ([fileManager moveItemAtPath:tmp_path toPath:child_path error:&error] != YES) {
                return 0;
            }
        }
    }
    return 1;
}

void rho_title_change(const int tabIndex, const char* strTitle) {
    // not implemented on iOS
}


// this method called from logging system - loggins system add logmessage to log and also copy it to iOS console by NSLog()
void rho_ios_log_console_output(const char* message) {
    NSString* str = [[NSString  alloc] initWithUTF8String:message];
    NSLog(@"%@", str);
    [str release];
}

void rho_startup_logging(const char* message) {
    [AppManager startupLogging:[NSString stringWithUTF8String:message]];
}


// return 0 if OK
// return -1 if error
// for Citrix support
int rhodes_ios_delete_file_via_platform_api(const char* filePath) {
    if (filePath == NULL) {
        return -1;
    }
    NSFileManager *fileManager = [NSFileManager defaultManager];
    NSString* sPath = [NSString stringWithUTF8String:filePath];
    if ([sPath length] <= 0) {
        return -1;
    }
    NSError *error = nil;
    if (![fileManager fileExistsAtPath:sPath]) {
        NSString *msg = [NSString stringWithFormat:@"rhodes_ios_delete_file_via_platform_api() error during delete file, ERROR = object not exists \"%@\"", sPath];
        NSLog(msg);
        RAWLOG_ERROR([msg UTF8String]);
        return -1;
    }
    if (![fileManager isDeletableFileAtPath:sPath]) {
        NSString *msg = [NSString stringWithFormat:@"rhodes_ios_delete_file_via_platform_api() error during delete file, ERROR = object is not deletable \"%@\"", sPath];
        NSLog(msg);
        RAWLOG_ERROR([msg UTF8String]);
        return -1;
    }
    if (![fileManager removeItemAtPath:sPath error:&error]) {
        NSString *msg = [NSString stringWithFormat:@"$$$ rhodes_ios_delete_file_via_platform_api() error during delete file, ERROR = %@", [error localizedDescription]];
        NSLog(msg);
        RAWLOG_ERROR([msg UTF8String]);
        return -1;
    }
    return 0;
}



/*
#define MAX_ACTIONS 4
const static struct {
	char*  _name;
	struct _action {
		char *_name;
		int (*_process)(HttpContextRef context);
	} _actions[MAX_ACTIONS];
} controllers[] = {
	{"loader" , { {"load", _LoadApp}, {NULL, NULL} } },
	{NULL}
};

int ExecuteAppManager(HttpContextRef context, RouteRef route) {
	char err[512];

	if (!route->_model) {
		HttpSendErrorToTheServer(context, 404, "Controller is not specifyed for the App Manager");
		return -1;
	}

	for (int i = 0; controllers[i]._name != NULL; i++ ) {
		if (!strcmp(route->_model, controllers[i]._name)) {

			if (!route->_action) {
				sprintf(err,"No action specifyed for the controller [%s]", route->_model);
				HttpSendErrorToTheServer(context, 404, err);
				return -1;
			}

			for (int n = 0; controllers[i]._actions[n]._name; n++) {
				if (!strcmp(controllers[i]._actions[n]._name, route->_action)) {
					return (controllers[i]._actions[n]._process)(context);
				}
			}

			sprintf(err,"No action [%s] found for the App Manager controller [%s]",
					route->_action, route->_model);
			HttpSendErrorToTheServer(context, 404, err);
			return -1;

		}
	}

	sprintf(err,"No [%s] controller found for App Manager", route->_model);
	HttpSendErrorToTheServer(context, 404, err);
	return -1;
}*/
