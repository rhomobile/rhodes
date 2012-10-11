
#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
#import <AVFoundation/AVAudioSession.h>

#include "ruby/ext/rho/rhoruby.h"


#define AUDIO_CAPTURE_DURATION @"duration"
#define AUDIO_CAPTURE_DESTINATION @"destination"
#define AUDIO_CAPTURE_NAME @"name"
#define AUDIO_CAPTURE_SAVE_EVENT @"audioSaveEvent"

@interface RhoAudioCapture: NSObject <AVAudioRecorderDelegate> {

    NSString* callbackURL;
    int duration;
    NSString* destination;
    BOOL isSaveFile;
    
    
    AVAudioRecorder* recorder;
    NSMutableDictionary* recordSetting;
    NSString* recorderFilePath;
}

@property (readwrite, copy) NSString *callbackURL;
@property (readwrite, copy) NSString *destination;
@property (readwrite, copy) NSString *recorderFilePath;
@property (readwrite, assign) int duration;

+ (RhoAudioCapture*) getInstance;

- (VALUE) getProperty:(NSString*)property_name;

- (void) setProperty:(NSString*)property_name value:(NSString*)value;

- (void) start;

- (void) stop;

- (void) cancel;

- (void)audioRecorderDidFinishRecording:(AVAudioRecorder *) aRecorder successfully:(BOOL)flag;

-(BOOL)exportAssetAsWaveFormat:(NSString*)filePath outPath:(NSString*)outPath;

- (void) fireCallback:(NSString*)status param_name:(NSString*)param_name param_value:(NSString*)param_value;

@end
