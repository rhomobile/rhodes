
#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
#import <AVFoundation/AVAudioSession.h>

#import "IAudioCapture.h"
#import "AudioCaptureBase.h"



#define CALLBACK_STATUS_ERROR  @"error"
#define CALLBACK_STATUS_OK  @"ok"
#define CALLBACK_STATUS_CANCEL  @"cancel"

@interface AudioCapture : AudioCaptureBase<IAudioCapture,AVAudioRecorderDelegate> {
    
    id<IMethodResult> callback;
    NSString* destination;
    BOOL isSaveFile;
    int maxDuration;
    BOOL isStarted;
    
    AVAudioRecorder* recorder;
    NSString* recorderFilePath;
    NSMutableDictionary* recordSetting;
}

@property (readwrite, retain) id<IMethodResult> callback;
@property (readwrite, retain) NSString *destination;
@property (readwrite, retain) NSString *recorderFilePath;


-(int) getMaxDurationWithHash:(NSDictionary*)options;
-(NSString*) getPreparedFileNameWithHash:(NSDictionary*)options;
-(NSString*) getPreparedFileNameWithName:(NSString*)name;

-(void) start:(NSDictionary*)props methodResult:(id<IMethodResult>)methodResult;
-(void) stop:(id<IMethodResult>)methodResult;
-(void) cancel:(id<IMethodResult>)methodResult;

- (void)audioRecorderDidFinishRecording:(AVAudioRecorder *) aRecorder successfully:(BOOL)flag;

-(BOOL)exportAssetAsWaveFormat:(NSString*)filePath outPath:(NSString*)outPath;

- (void) fireCallback:(NSString*)status param_name:(NSString*)param_name param_value:(NSString*)param_value;


@end