
#import "audioCapture.h"

#include "ruby/ext/rho/rhoruby.h"

#import "common/RhodesApp.h"


static RhoAudioCapture* ourRhoAudioCapture = nil;

@implementation RhoAudioCapture

@synthesize callbackURL, destination, recorderFilePath, duration;

+ (RhoAudioCapture*) getInstance {
    if (ourRhoAudioCapture == nil) {
        ourRhoAudioCapture = [[RhoAudioCapture alloc] init];
        ourRhoAudioCapture.duration = 20000;
        
        NSString *folder = [NSString stringWithUTF8String:rho_rhodesapp_getblobsdirpath()];
        
        NSFileManager *fileManager = [NSFileManager defaultManager];
        if (![fileManager fileExistsAtPath:folder])
            [fileManager createDirectoryAtPath:folder attributes:nil];
        
        ourRhoAudioCapture.destination = [folder stringByAppendingPathComponent:@"audiocapture.wav"];
    }
    return ourRhoAudioCapture;
}

- (VALUE) getProperty:(NSString*)property_name {
    if ([AUDIO_CAPTURE_DESTINATION compare:property_name options:NSCaseInsensitiveSearch] == NSOrderedSame) {
        return rho_ruby_create_string([destination UTF8String]);
    }
    if ([AUDIO_CAPTURE_DURATION compare:property_name options:NSCaseInsensitiveSearch] == NSOrderedSame) {
        return rho_ruby_create_integer(duration);
    }
    if ([AUDIO_CAPTURE_SAVE_EVENT compare:property_name options:NSCaseInsensitiveSearch] == NSOrderedSame) {
        return rho_ruby_create_string([callbackURL UTF8String]);
    }
    return rho_ruby_get_NIL();
    
}

- (void) setProperty:(NSString*)property_name value:(NSString*)value {
    if ([AUDIO_CAPTURE_SAVE_EVENT compare:property_name options:NSCaseInsensitiveSearch] == NSOrderedSame) {
        if ((value != nil) /*&& ([value length] > 0)*/) {
            callbackURL = value;
        }
    }
    if ([AUDIO_CAPTURE_DESTINATION compare:property_name options:NSCaseInsensitiveSearch] == NSOrderedSame) {
        if ((value != nil) && ([value length] > 0)) {
            destination = value;
        }
    }
    if ([AUDIO_CAPTURE_NAME compare:property_name options:NSCaseInsensitiveSearch] == NSOrderedSame) {
        if ((value != nil) && ([value length] > 0)) {
            //TODO: make full path to DB files
            
            NSString *folder = [NSString stringWithUTF8String:rho_rhodesapp_getblobsdirpath()];
            
            NSFileManager *fileManager = [NSFileManager defaultManager];
            if (![fileManager fileExistsAtPath:folder])
                [fileManager createDirectoryAtPath:folder attributes:nil];
            
            destination = [folder stringByAppendingPathComponent:value];
        }
    }
    if ([AUDIO_CAPTURE_DURATION compare:property_name options:NSCaseInsensitiveSearch] == NSOrderedSame) {
        if ((value != nil) && ([value length] > 0)) {
            if (value != nil) {
                duration= [value intValue];
            }
        }
    }
    
}

#define DOCUMENTS_FOLDER [NSHomeDirectory() stringByAppendingPathComponent:@"Documents"]


- (void) startCommand {

    AVAudioSession *audioSession = [AVAudioSession sharedInstance];
    NSError *err = nil;
    [audioSession setCategory :AVAudioSessionCategoryPlayAndRecord error:&err];
    if(err){
        NSLog(@"audioSession: %@ %d %@", [err domain], [err code], [[err userInfo] description]);
        //fire ERROR
        [self fireCallback:@"ERROR" param_name:nil param_value:nil];
        return;
    }
    [audioSession setActive:YES error:&err];
    err = nil;
    if(err){
        NSLog(@"audioSession: %@ %d %@", [err domain], [err code], [[err userInfo] description]);
        //fire ERROR
        [self fireCallback:@"ERROR" param_name:nil param_value:nil];
        return;
    }
    
    recordSetting = [[NSMutableDictionary alloc] init];
    
    [recordSetting setValue :[NSNumber numberWithInt:kAudioFormatLinearPCM] forKey:AVFormatIDKey];
    [recordSetting setValue:[NSNumber numberWithFloat:22050.0] forKey:AVSampleRateKey];
    [recordSetting setValue:[NSNumber numberWithInt: 1] forKey:AVNumberOfChannelsKey];
    
    [recordSetting setValue :[NSNumber numberWithInt:16] forKey:AVLinearPCMBitDepthKey];
    [recordSetting setValue :[NSNumber numberWithBool:NO] forKey:AVLinearPCMIsBigEndianKey];
    [recordSetting setValue :[NSNumber numberWithBool:NO] forKey:AVLinearPCMIsFloatKey];
    
    
    
    // Create a new dated file
    //NSURL *url = [NSURL fileURLWithPath:destination];
    
    //NSDate *now = [NSDate dateWithTimeIntervalSinceNow:0];
    //NSString *caldate = [now description];
    
    
    NSArray *paths = paths = NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES);
    NSString* rootDirectory = [ [paths objectAtIndex:0] stringByAppendingString:@""] ;//]@"/TempFiles/"];
    
    self.recorderFilePath = [NSString stringWithString:rootDirectory];
    self.recorderFilePath = [recorderFilePath stringByAppendingString:@"/temporary.caf"];
    
    NSURL *url = [NSURL fileURLWithPath:self.recorderFilePath];

    
    
    err = nil;
    recorder = [[ AVAudioRecorder alloc] initWithURL:url settings:recordSetting error:&err];
    if(!recorder){
        //fire ERROR
        [self fireCallback:@"ERROR" param_name:nil param_value:nil];
        return;
    }
    
    //prepare to record
    [recorder setDelegate:self];
    [recorder prepareToRecord];
    recorder.meteringEnabled = YES;
    
    BOOL audioHWAvailable = audioSession.inputIsAvailable;
    if (! audioHWAvailable) {
        //fire ERROR
        [self fireCallback:@"ERROR" param_name:@"message" param_value:@"AudioCapture hardware is not available !"];
        return;
    }
    
    // start recording
    [recorder recordForDuration:(NSTimeInterval) ((double)duration/1000.0)];
    
    isSaveFile = YES;
    
    
}

- (void) start {
    [self performSelectorOnMainThread:@selector(startCommand) withObject:nil waitUntilDone:NO];
}



- (void) stopCommand {
    if (recorder != nil) {
        isSaveFile = YES;
        [recorder stop];
    }
    else {
        [self fireCallback:@"ERROR" param_name:@"message" param_value:@"not any record process"];
    }
    recorder = nil;
    
    //NSURL *url = [NSURL fileURLWithPath: destination];
    //NSError *err = nil;
    //NSData *audioData = [NSData dataWithContentsOfFile:[url path] options: 0 error:&err];
    //if(!audioData)
    //    NSLog(@"audio data: %@ %d %@", [err domain], [err code], [[err userInfo] description]);
   
}

- (void) stop {
    [self performSelectorOnMainThread:@selector(stopCommand) withObject:nil waitUntilDone:NO];
}


- (void) cancelCommand {
    if (recorder != nil) {
        isSaveFile = NO;
        [recorder stop];
    }
    else {
        [self fireCallback:@"ERROR" param_name:@"message" param_value:@"not any record process"];
    }
    recorder = nil;
}

- (void) cancel {
    [self performSelectorOnMainThread:@selector(cancelCommand) withObject:nil waitUntilDone:NO];
}



-(void) convertCommand {
    if (![self exportAssetAsWaveFormat:recorderFilePath outPath:destination] ) {
        //fire ERROR
        [self fireCallback:@"ERROR" param_name:@"message" param_value:@"can not save WAV after capturing"];
    }
}

- (void)audioRecorderDidFinishRecording:(AVAudioRecorder *) aRecorder successfully:(BOOL)flag
{
    recorder = nil;
    //NSLog (@"audioRecorderDidFinishRecording:successfully:");
    // your actions here
    if (flag) {
        if (isSaveFile) {
            [self performSelectorOnMainThread:@selector(convertCommand) withObject:nil waitUntilDone:NO];
        }
        else {
            //fire CANCEL
            [self fireCallback:@"CANCEL" param_name:nil param_value:nil];
        }
    }
    else {
        //fire ERROR
        [self fireCallback:@"ERROR" param_name:nil param_value:nil];
    }
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



-(BOOL)exportAssetAsWaveFormat:(NSString*)filePath  outPath:(NSString*)outPath
{
    NSError *error = nil ;
    
    //const char* inStr = [filePath UTF8String];
    //const char* outStr = [outPath UTF8String];
    
    ///*
     NSDictionary *audioSetting = [NSDictionary dictionaryWithObjectsAndKeys:
                                  [ NSNumber numberWithFloat:22050.0], AVSampleRateKey,
                                  [ NSNumber numberWithInt:1], AVNumberOfChannelsKey,
                                  [ NSNumber numberWithInt:16], AVLinearPCMBitDepthKey,
                                  [ NSNumber numberWithInt:kAudioFormatLinearPCM], AVFormatIDKey,
                                  [ NSNumber numberWithBool:NO], AVLinearPCMIsFloatKey,
                                  [ NSNumber numberWithBool:0], AVLinearPCMIsBigEndianKey,
                                  [ NSNumber numberWithBool:NO], AVLinearPCMIsNonInterleaved,
                                  [ NSData data], AVChannelLayoutKey, nil ];
    //*/
    /*
    recordSetting = [[NSMutableDictionary alloc] init];
    
    [recordSetting setValue :[NSNumber numberWithInt:kAudioFormatLinearPCM] forKey:AVFormatIDKey];
    [recordSetting setValue:[NSNumber numberWithFloat:22050.0] forKey:AVSampleRateKey];
    [recordSetting setValue:[NSNumber numberWithInt: 1] forKey:AVNumberOfChannelsKey];
    
    [recordSetting setValue :[NSNumber numberWithInt:16] forKey:AVLinearPCMBitDepthKey];
    [recordSetting setValue :[NSNumber numberWithBool:NO] forKey:AVLinearPCMIsBigEndianKey];
    [recordSetting setValue :[NSNumber numberWithBool:NO] forKey:AVLinearPCMIsFloatKey];
     */
    
    
    NSString *audioFilePath = filePath;
    NSURL* inUrl = [NSURL fileURLWithPath:audioFilePath];
    AVURLAsset * URLAsset = [[AVURLAsset alloc]  initWithURL:inUrl options:nil];
    
    if (!URLAsset) return NO ;
    
    AVAssetReader *assetReader = [AVAssetReader assetReaderWithAsset:URLAsset error:&error];
    if (error) return NO;
    
    NSArray *tracks = [URLAsset tracksWithMediaType:AVMediaTypeAudio];
    if (![tracks count]) return NO;
    
    AVAssetReaderAudioMixOutput *audioMixOutput = [AVAssetReaderAudioMixOutput
                                                   assetReaderAudioMixOutputWithAudioTracks:tracks
                                                   audioSettings :audioSetting];
    
    if (![assetReader canAddOutput:audioMixOutput]) return NO ;
    
    [assetReader addOutput :audioMixOutput];
    
    if (![assetReader startReading]) return NO;
    
    
    
    //NSString *title = @"WavConverted";
    //NSArray *docDirs = NSSearchPathForDirectoriesInDomains (NSDocumentDirectory, NSUserDomainMask, YES);
    //NSString *docDir = [docDirs objectAtIndex: 0];
    //NSString *outPath = [[docDir stringByAppendingPathComponent :title]
    //                     stringByAppendingPathExtension:@"wav" ];
    
    NSFileManager *fileManager = [NSFileManager defaultManager];
    if ([fileManager fileExistsAtPath:outPath]) {
        NSError *err = nil;
        [fileManager removeItemAtPath:outPath error:&err];
    }
    
    
    NSURL *outURL = [NSURL fileURLWithPath:outPath];
    AVAssetWriter *assetWriter = [AVAssetWriter assetWriterWithURL:outURL
                                                          fileType:AVFileTypeWAVE
                                                             error:&error];
    if (error) return NO;
    
    AVAssetWriterInput *assetWriterInput = [ AVAssetWriterInput assetWriterInputWithMediaType :AVMediaTypeAudio
                                                                                outputSettings:audioSetting];
    assetWriterInput. expectsMediaDataInRealTime = NO;
    
    if (![assetWriter canAddInput:assetWriterInput]) return NO ;
    
    [assetWriter addInput :assetWriterInput];
    
    if (![assetWriter startWriting]) return NO;
    
    
    [assetReader retain];
    [assetWriter retain];
    
    [assetWriter startSessionAtSourceTime:kCMTimeZero ];
    
    dispatch_queue_t queue = dispatch_queue_create( "assetWriterQueue", NULL );
    
    [assetWriterInput requestMediaDataWhenReadyOnQueue:queue usingBlock:^{
        
        //NSLog(@"start");
        
        double actualDuration = 0;
        
        while (1)
        {
            if ([assetWriterInput isReadyForMoreMediaData]) {
                
                CMSampleBufferRef sampleBuffer = [audioMixOutput copyNextSampleBuffer];
                
                if (sampleBuffer) {
                    [assetWriterInput appendSampleBuffer :sampleBuffer];
                    
                    CMTime t = CMSampleBufferGetOutputDuration(sampleBuffer);
                    
                    actualDuration += (((double)t.value * 1000)/ (double)t.timescale);
                    CFRelease(sampleBuffer);
                } else {
                    [assetWriterInput markAsFinished];
                    break;
                }
            }
        }
        
        [assetWriter finishWriting];
        [assetReader release ];
        [assetWriter release ];
        
        //NSLog(@"finish");
        
        //int durInMilis = (int)actualDuration;
        
        //fire OK
        [self fireCallback:@"OK" param_name:@"filename" param_value:destination];//[NSString stringWithFormat:@"%d", durInMilis]];
    }];
    
    dispatch_release(queue);
}


@end



VALUE audiocapture_get_property(const char* property_name) {
    VALUE result = [[RhoAudioCapture getInstance] getProperty:[NSString stringWithUTF8String:property_name]];
    return result;
}

void audiocapture_set_property(const char* property_name, const char* value) {
    [[RhoAudioCapture getInstance] setProperty:[NSString stringWithUTF8String:property_name] value:[NSString stringWithUTF8String:value]];    
}

void audiocapture_start() {
    [[RhoAudioCapture getInstance] start];
}

void audiocapture_stop() {
    [[RhoAudioCapture getInstance] stop];
}

void audiocapture_cancel() {
    [[RhoAudioCapture getInstance] cancel];
}