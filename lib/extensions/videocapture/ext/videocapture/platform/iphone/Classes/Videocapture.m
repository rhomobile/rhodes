
#import "videocapture.h"

#include "ruby/ext/rho/rhoruby.h"

#import "common/RhodesApp.h"

#import <MobileCoreServices/UTCoreTypes.h>

#import <Rhodes.h>


static RhoVideoCapture* ourRhoVideoCapture = nil;

@implementation RhoVideoCapture

@synthesize callbackURL, destination, recorderFilePath, duration;

+ (RhoVideoCapture*) getInstance {
    if (ourRhoVideoCapture == nil) {
        ourRhoVideoCapture = [[RhoVideoCapture alloc] init];
        ourRhoVideoCapture.duration = 20000;
        ourRhoVideoCapture.moviePickerController = nil;
        
        NSString *folder = [NSString stringWithUTF8String:rho_rhodesapp_getblobsdirpath()];
        
        NSFileManager *fileManager = [NSFileManager defaultManager];
        if (![fileManager fileExistsAtPath:folder]) {
            NSError* error;
            [fileManager createDirectoryAtPath:folder withIntermediateDirectories:YES attributes:nil error:&error];
            
        }
        ourRhoVideoCapture.destination = [folder stringByAppendingPathComponent:@"Videocapture.mp4"];
    }
    return ourRhoVideoCapture;
}

- (VALUE) getProperty:(NSString*)property_name {
    if ([VIDEO_CAPTURE_DESTINATION compare:property_name options:NSCaseInsensitiveSearch] == NSOrderedSame) {
        return rho_ruby_create_string([destination UTF8String]);
    }
    if ([VIDEO_CAPTURE_DURATION compare:property_name options:NSCaseInsensitiveSearch] == NSOrderedSame) {
        return rho_ruby_create_integer(duration);
    }
    if ([VIDEO_CAPTURE_SAVE_EVENT compare:property_name options:NSCaseInsensitiveSearch] == NSOrderedSame) {
        return rho_ruby_create_string([callbackURL UTF8String]);
    }
    return rho_ruby_get_NIL();
    
}

- (void) setProperty:(NSString*)property_name value:(NSString*)value {
    if ([VIDEO_CAPTURE_SAVE_EVENT compare:property_name options:NSCaseInsensitiveSearch] == NSOrderedSame) {
        if ((value != nil) /*&& ([value length] > 0)*/) {
            callbackURL = value;
        }
    }
    if ([VIDEO_CAPTURE_DESTINATION compare:property_name options:NSCaseInsensitiveSearch] == NSOrderedSame) {
        if ((value != nil) && ([value length] > 0)) {
            destination = value;
        }
    }
    if ([VIDEO_CAPTURE_NAME compare:property_name options:NSCaseInsensitiveSearch] == NSOrderedSame) {
        if ((value != nil) && ([value length] > 0)) {
            //TODO: make full path to DB files
            
            NSString *folder = [NSString stringWithUTF8String:rho_rhodesapp_getblobsdirpath()];
            
            NSFileManager *fileManager = [NSFileManager defaultManager];
            if (![fileManager fileExistsAtPath:folder]) {
                NSError* error;
                [fileManager createDirectoryAtPath:folder withIntermediateDirectories:YES attributes:nil error:&error];
                
            }
                
            destination = [folder stringByAppendingPathComponent:value];
        }
    }
    if ([VIDEO_CAPTURE_DURATION compare:property_name options:NSCaseInsensitiveSearch] == NSOrderedSame) {
        if ((value != nil) && ([value length] > 0)) {
            if (value != nil) {
                duration= [value intValue];
            }
        }
    }
    
}

#define DOCUMENTS_FOLDER [NSHomeDirectory() stringByAppendingPathComponent:@"Documents"]


- (BOOL) startCameraControllerFromViewController: (UIViewController*) controller
                                   usingDelegate: (id <UIImagePickerControllerDelegate,
                                                   UINavigationControllerDelegate>) delegate {
    
    if (([UIImagePickerController isSourceTypeAvailable:
          UIImagePickerControllerSourceTypeCamera] == NO)
        || (delegate == nil)
        || (controller == nil))
        return NO;
    
    
    UIImagePickerController *cameraUI = [[UIImagePickerController alloc] init];
    cameraUI.sourceType = UIImagePickerControllerSourceTypeCamera;
    
    // Displays a control that allows the user to choose picture or
    // movie capture, if both are available:
    //cameraUI.mediaTypes =
    //[UIImagePickerController availableMediaTypesForSourceType:
    // UIImagePickerControllerSourceTypeCamera];
    
    cameraUI.mediaTypes = [[NSArray alloc] initWithObjects: (NSString *) kUTTypeMovie, nil];
    
    // Hides the controls for moving & scaling pictures, or for
    // trimming movies. To instead show the controls, use YES.
    cameraUI.allowsEditing = NO;
    
    cameraUI.delegate = delegate;
    
    cameraUI.videoMaximumDuration = (NSTimeInterval) ((double)duration/1000.0);
    
    moviePickerController = cameraUI;
    
    [controller presentModalViewController: cameraUI animated: YES];
    return YES;
}

// For responding to the user tapping Cancel.
- (void) imagePickerControllerDidCancel: (UIImagePickerController *) picker {
    moviePickerController = nil;
    //[[picker parentViewController] dismissModalViewControllerAnimated: YES];
    [[[[Rhodes sharedInstance] mainView] getMainViewController] dismissModalViewControllerAnimated:YES];
    [picker release];
    [self fireCallback:@"CANCEL" param_name:nil param_value:nil];
}

// For responding to the user accepting a newly-captured picture or movie
- (void) imagePickerController: (UIImagePickerController *) picker
 didFinishPickingMediaWithInfo: (NSDictionary *) info {
    
    moviePickerController = nil;
    
    NSString *mediaType = [info objectForKey: UIImagePickerControllerMediaType];
    UIImage *originalImage, *editedImage, *imageToSave;
    
    // Handle a movie capture
    if (CFStringCompare ((CFStringRef) mediaType, kUTTypeMovie, 0)
        == kCFCompareEqualTo) {
        
        
        if (isSaveFile) {
            NSString *moviePath = [[info objectForKey:
                                    UIImagePickerControllerMediaURL] path];
            
            NSURL *movieUrl = [[[info objectForKey:UIImagePickerControllerMediaURL] copy] autorelease];
            
            NSFileManager *fileManager = [NSFileManager defaultManager];
            NSError *err = nil;
            
            if ([fileManager fileExistsAtPath:destination]) {
                [fileManager removeItemAtPath:destination error:&err];
            }
            
            //move recorded to destination
            [fileManager moveItemAtPath:moviePath toPath:destination error:&err];
            
            [self fireCallback:@"OK" param_name:@"filename" param_value:destination];
            
        }
        
    }
    else {
        // error
        [self fireCallback:@"ERROR" param_name:@"message" param_value:@"not found recorded video"];
    }
    
    //[[picker parentViewController] dismissModalViewControllerAnimated: YES];
    [[[[Rhodes sharedInstance] mainView] getMainViewController] dismissModalViewControllerAnimated:YES];
    [picker release];
}


- (void) startCommand {
    isSaveFile = YES;
    if ([self startCameraControllerFromViewController:[[[Rhodes sharedInstance] mainView] getMainViewController] usingDelegate:self] ) {
        // OK
    }
    else {
        // error
        [self fireCallback:@"ERROR" param_name:@"message" param_value:@"can not start record process"];
    }
    
}

- (void) start {
    [self performSelectorOnMainThread:@selector(startCommand) withObject:nil waitUntilDone:NO];
}



- (void) stopCommand {
    
    if (moviePickerController != nil) {
        isSaveFile = YES;
        [moviePickerController stopVideoCapture];
        moviePickerController = nil;
    }
    else {
        [self fireCallback:@"ERROR" param_name:@"message" param_value:@"not any record process"];
    }
    
}

- (void) stop {
    [self performSelectorOnMainThread:@selector(stopCommand) withObject:nil waitUntilDone:NO];
}


- (void) cancelCommand {

    
    if (moviePickerController != nil) {
        isSaveFile = NO;
        [moviePickerController stopVideoCapture];
        moviePickerController = nil;
    }
    else {
        [self fireCallback:@"ERROR" param_name:@"message" param_value:@"not any record process"];
    }
}

- (void) cancel {
    [self performSelectorOnMainThread:@selector(cancelCommand) withObject:nil waitUntilDone:NO];
}




- (void) fireCallback:(NSString*)status param_name:(NSString*)param_name param_value:(NSString*)param_value {
    if (callbackURL == nil) {
        return;
    }
    if ([callbackURL length] <= 0) {
        return;
    }
	NSString* strBody = @"&rho_callback=1&status=";
    strBody = [strBody stringByAppendingString:status];
    
    if ((param_name != nil) && (param_value != nil)) {
        strBody = [strBody stringByAppendingString:@"&"];
        strBody = [strBody stringByAppendingString:param_name];
        strBody = [strBody stringByAppendingString:@"="];
        strBody = [strBody stringByAppendingString:param_value];
    }
    
	const char* cb = [callbackURL UTF8String];
	const char* b = [strBody UTF8String];
    char* norm_url = rho_http_normalizeurl(cb);
    
	rho_net_request_with_data(rho_http_normalizeurl(cb), b);
    
    rho_http_free(norm_url);
}

@end



VALUE videocapture_get_property(const char* property_name) {
    VALUE result = [[RhoVideoCapture getInstance] getProperty:[NSString stringWithUTF8String:property_name]];
    return result;
}

void videocapture_set_property(const char* property_name, const char* value) {
    [[RhoVideoCapture getInstance] setProperty:[NSString stringWithUTF8String:property_name] value:[NSString stringWithUTF8String:value]];
}

void videocapture_start() {
    [[RhoVideoCapture getInstance] start];
}

void videocapture_stop() {
    [[RhoVideoCapture getInstance] stop];
}

void videocapture_cancel() {
    [[RhoVideoCapture getInstance] cancel];
}