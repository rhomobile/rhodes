
#import "Videocapture.h"
#import "common/RhodesApp.h"

#import <MobileCoreServices/UTCoreTypes.h>

#import <Rhodes.h>

@implementation Videocapture

-(id) init {
    self = [super init];

    NSString *folder = [NSString stringWithUTF8String:rho_rhodesapp_getblobsdirpath()];

    NSFileManager *fileManager = [NSFileManager defaultManager];
    if (![fileManager fileExistsAtPath:folder]) {
        NSError* error;
        [fileManager createDirectoryAtPath:folder withIntermediateDirectories:YES attributes:nil error:&error];
    }
        [fileManager createDirectoryAtPath:folder attributes:nil];
    NSString* destination = [folder stringByAppendingPathComponent:@"Videocapture.mov"];
    [self setFileName:destination methodResult:nil];

    return self;
}


-(int) getDurationInt {
    NSString* durStr = (NSString*)[mProperties objectForKey:@"duration"];
    return [durStr intValue];
}

-(BOOL) isHighResolution {
    NSString* resStr = (NSString*)[mProperties objectForKey:@"resolution"];
    return ([resStr isEqualToString:@"high"] || [resStr isEqualToString:@"HIGH"]);
}
-(BOOL) isMediumResolution {
    NSString* resStr = (NSString*)[mProperties objectForKey:@"resolution"];
    return ([resStr isEqualToString:@"medium"] || [resStr isEqualToString:@"MEDIUM"]);
}
-(BOOL) isLowResolution {
    NSString* resStr = (NSString*)[mProperties objectForKey:@"resolution"];
    return ([resStr isEqualToString:@"low"] || [resStr isEqualToString:@"LOW"]);
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
    
    if ([self isHighResolution]) {
        cameraUI.videoQuality = UIImagePickerControllerQualityTypeHigh;
    }
    if ([self isMediumResolution]) {
        cameraUI.videoQuality = UIImagePickerControllerQualityTypeMedium;
    }
    if ([self isLowResolution]) {
        cameraUI.videoQuality = UIImagePickerControllerQualityTypeLow;
    }
    cameraUI.videoMaximumDuration = (NSTimeInterval) ((double)[self getDurationInt]/1000.0);

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
    [self fireCallback:@"CANCEL"fileName:nil fileSize:nil];
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
            NSString* destination = [mProperties objectForKey:@"fileName"];

            if (destination == nil) {
                destination = @"Videocapture.mov";
            }

            NSString *appDirectory = [NSString stringWithUTF8String:rho_native_rhopath()];
            NSString *dbDirectory = [NSString stringWithUTF8String:rho_native_rhodbpath()];



            if ((![destination hasPrefix:appDirectory])   && (![destination hasPrefix:dbDirectory])) {
                NSString* userfolder = [NSString stringWithUTF8String:rho_native_rhouserpath()];

                destination = [userfolder stringByAppendingPathComponent:destination];
            }

            if ([fileManager fileExistsAtPath:destination]) {
                [fileManager removeItemAtPath:destination error:&err];
            }

            //move recorded to destination
            [fileManager moveItemAtPath:moviePath toPath:destination error:&err];

            NSNumber* size = nil;
            if ([fileManager isReadableFileAtPath:destination]) {
                NSDictionary *attributes = [fileManager attributesOfItemAtPath:destination error:&err];
                if (!err) {
                    NSNumber *fsize = [attributes objectForKey:NSFileSize];
                    if (fsize != nil) {
                        size = [NSNumber numberWithInt:([fsize intValue]/1024)];
                    }
                }
            }

            [self fireCallback:@"OK" fileName:destination fileSize:size];

        }

    }
    else {
        // error
        [self fireCallback:@"ERROR"fileName:@"message" fileSize:@"not found recorded video"];
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
        [self fireCallback:@"ERROR"fileName:@"message" fileSize:@"can not start record process"];
    }

}

-(void) start:(id<IMethodResult>)methodResult {
    callback = [methodResult retain];
    [self performSelectorOnMainThread:@selector(startCommand) withObject:nil waitUntilDone:NO];
}

-(void) stop:(id<IMethodResult>)methodResult {

}

-(void) cancel:(id<IMethodResult>)methodResult {

}

- (void) fireCallback:(NSString*)status fileName:(NSString*)fileName fileSize:(NSNumber*)fileSize {
    if (callback == nil) {
        return;
    }
    NSMutableDictionary* dict = [NSMutableDictionary dictionaryWithCapacity:5];
    if (![status isEqualToString:@"ERROR"]) {
        [dict setObject:status forKey:@"status"];
        [dict setObject:status forKey:@"transferResult"];
        if (fileName != nil) {
            [dict setObject:fileName forKey:@"fileName"];
        }
        if (fileSize != nil) {
            [dict setObject:fileSize forKey:@"fileSize"];
        }
    }
    else {
        [dict setObject:status forKey:@"status"];
        [dict setObject:status forKey:@"transferResult"];
        [dict setObject:fileSize forKey:fileName];
    }



    [callback setResult:dict];
    [callback release];
    callback = nil;
}



@end
