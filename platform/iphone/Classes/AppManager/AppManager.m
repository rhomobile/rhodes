//
//  AppManager.m
//  rhorunner
//
//  Created by vlad on 9/24/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import <SystemConfiguration/SystemConfiguration.h>
#import <Foundation/Foundation.h>

#include "common/RhoPort.h"
#import "unzip.h"
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
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "RhodesApp"

static bool UnzipApplication(const char* appRoot, const void* zipbuf, unsigned int ziplen);
//const char* RhoGetRootPath();

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

/*
 * Gets root folder of the site
 * Application folders located undern the root
 */
+ (NSString *) getApplicationsRootPath {
	NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
	NSString *documentsDirectory = [paths objectAtIndex:0];
	return [documentsDirectory stringByAppendingPathComponent:@"apps"];
}

+ (NSString *) getDbPath {
	NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
	NSString *documentsDirectory = [paths objectAtIndex:0];
	return [documentsDirectory stringByAppendingPathComponent:@"db"];
}

+ (NSString *) getApplicationsRosterUrl {
	return @"http://dev.rhomobile.com/vlad/";
}

+ (bool) installApplication:(NSString*)appName data:(NSData*)appData {
	NSFileManager *fileManager = [NSFileManager defaultManager];
	NSString *appPath = [[AppManager getApplicationsRootPath] stringByAppendingPathComponent:appName]; 
	if ([fileManager fileExistsAtPath:appPath]) {
		NSError *error;
		[fileManager removeItemAtPath:appPath error:&error];
	}
    [fileManager createDirectoryAtPath:appPath attributes:NULL];
	
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
		if (![fileManager fileExistsAtPath:target])
			[fileManager createDirectoryAtPath:target attributes:nil];
		
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
    if (![fileManager fileExistsAtPath:filePath1] || ![fileManager fileExistsAtPath:filePath2])
        return NO;
    
    NSString *content1 = [[NSString alloc] initWithData:[fileManager contentsAtPath:filePath1]
                                               encoding:NSUTF8StringEncoding];
    NSString *content2 = [[NSString alloc] initWithData:[fileManager contentsAtPath:filePath2]
                                               encoding:NSUTF8StringEncoding];
    return [content1 isEqualToString:content2];
}

/*
 * Configures AppManager
 */
- (void) configure {
	
    NSFileManager *fileManager = [NSFileManager defaultManager];
    
	NSString *bundleRoot = [[NSBundle mainBundle] resourcePath];
	NSString *rhoRoot = [NSString stringWithUTF8String:rho_native_rhopath()];

	NSString *filePathNew = [bundleRoot stringByAppendingPathComponent:@"name"];
	NSString *filePathOld = [rhoRoot stringByAppendingPathComponent:@"name"];
    BOOL hasOldName = [fileManager fileExistsAtPath:filePathOld];
    BOOL nameChanged = ![self isContentsEqual:fileManager first:filePathNew second:filePathOld];

    BOOL contentChanged;
    if (nameChanged)
        contentChanged = YES;
	else {
		filePathNew = [bundleRoot stringByAppendingPathComponent:@"hash"];
		filePathOld = [rhoRoot stringByAppendingPathComponent:@"hash"];

        contentChanged = ![self isContentsEqual:fileManager first:filePathNew second:filePathOld];
	}

	if (contentChanged) {
        NSString *dirs[] = {@"apps", @"lib", @"db", @"hash", @"name"};
        for (int i = 0, lim = sizeof(dirs)/sizeof(dirs[0]); i < lim; ++i) {
            BOOL remove = nameChanged;
            if ([dirs[i] isEqualToString:@"db"] && !hasOldName)
                remove = NO;
            [self copyFromMainBundle:fileManager
                            fromPath:[bundleRoot stringByAppendingPathComponent:dirs[i]]
                              toPath:[rhoRoot stringByAppendingPathComponent:dirs[i]]
                              remove:remove];
        }
	}

	rho_logconf_Init(rho_native_rhopath());
	rho_rhodesapp_create(rho_native_rhopath());
	RAWLOG_INFO("Rhodes started");
}

@end

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

VALUE rho_sys_get_locale() 
{
	NSString *preferredLang = [[NSLocale preferredLanguages] objectAtIndex:0];
	
	return rho_ruby_create_string( [preferredLang UTF8String] );
}

int rho_sys_get_screen_width()
{
    CGRect rect = [[UIScreen mainScreen] bounds];
    return rect.size.width;
}

int rho_sys_get_screen_height()
{
    CGRect rect = [[UIScreen mainScreen] bounds];
    return rect.size.height;
}

VALUE rho_sysimpl_get_property(char* szPropName)
{
    if (strcasecmp("has_camera",szPropName) == 0) {
        int has_camera = [UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypeCamera];
        return rho_ruby_create_boolean(has_camera);
    }

    return rho_ruby_get_NIL();
}

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