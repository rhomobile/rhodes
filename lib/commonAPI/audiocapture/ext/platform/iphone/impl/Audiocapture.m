
#import "AudioCapture.h"


extern const char* rho_rhodesapp_getblobsdirpath();

@implementation AudioCapture

@synthesize callback, destination, recorderFilePath;

#define DOCUMENTS_FOLDER [NSHomeDirectory() stringByAppendingPathComponent:@"Documents"]


- (id) init {
    self = [super init];
    
    maxDuration = 20000;
    destination = [self getPreparedFileNameWithName:@""];
    
    [self setProperty:PROPERTY_SOURCE propertyValue:@"mic" methodResult:nil];
    [self setProperty:PROPERTY_MAX_DURATION propertyValue:@"20000" methodResult:nil];
    [self setProperty:PROPERTY_FILE_NAME propertyValue:@"AudioCapture" methodResult:nil];
    
    return self;
}


-(void) setProperty:(NSString*)propertyName propertyValue:(NSString*)propertyValue methodResult:(id<IMethodResult>)methodResult {
    [super setProperty:propertyName propertyValue:propertyValue methodResult:methodResult];
    if ([PROPERTY_MAX_DURATION compare:propertyName] == NSOrderedSame) {
        int dur = [self getMaxDurationWithHash:nil];
        [super setProperty:PROPERTY_MAX_DURATION propertyValue:[NSString stringWithFormat:@"%@", [NSNumber numberWithInt:dur]] methodResult:nil];
    }
}


- (void) startCommand {
    
    NSLog(@"$$$ AudioCapture.start");
    
    AVAudioSession *audioSession = [AVAudioSession sharedInstance];
    NSError *err = nil;
    [audioSession setCategory :AVAudioSessionCategoryPlayAndRecord error:&err];
    if(err){
        NSLog(@"audioSession: %@ %d %@", [err domain], [err code], [[err userInfo] description]);
        //fire ERROR
        [self fireCallback:CALLBACK_STATUS_ERROR param_name:nil param_value:nil];
        return;
    }
    [audioSession setActive:YES error:&err];
    err = nil;
    if(err){
        NSLog(@"audioSession: %@ %d %@", [err domain], [err code], [[err userInfo] description]);
        //fire ERROR
        [self fireCallback:CALLBACK_STATUS_ERROR param_name:nil param_value:nil];
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
        [self fireCallback:CALLBACK_STATUS_ERROR param_name:nil param_value:nil];
        return;
    }
    
    //prepare to record
    [recorder setDelegate:self];
    [recorder prepareToRecord];
    recorder.meteringEnabled = YES;
    
    BOOL audioHWAvailable = audioSession.inputIsAvailable;
    if (! audioHWAvailable) {
        //fire ERROR
        [self fireCallback:CALLBACK_STATUS_ERROR param_name:@"message" param_value:@"AudioCapture hardware is not available !"];
        return;
    }
    
    // start recording
    [recorder recordForDuration:(NSTimeInterval) (((double)maxDuration)/1000.0)];
    
    isSaveFile = YES;
    
    
}

- (void) stopCommand {
    if (recorder != nil) {
        isSaveFile = YES;
        isStopping = YES;
        [recorder stop];
    }
    else {
        if (!isStopping) {
          [self fireCallback:CALLBACK_STATUS_ERROR param_name:@"message" param_value:@"not any record process"];
        }
    }
    recorder = nil;

    //NSURL *url = [NSURL fileURLWithPath: destination];
    //NSError *err = nil;
    //NSData *audioData = [NSData dataWithContentsOfFile:[url path] options: 0 error:&err];
    //if(!audioData)
    //    NSLog(@"audio data: %@ %d %@", [err domain], [err code], [[err userInfo] description]);
    
}

- (void) cancelCommand {
    if (recorder != nil) {
        isSaveFile = NO;
        isStopping = YES;
        [recorder stop];
    }
    else {
      if (!isStopping) {
        [self fireCallback:CALLBACK_STATUS_ERROR param_name:@"message" param_value:@"not any record process"];
      }
    }
    recorder = nil;
}


-(void) convertCommand {
    if (![self exportAssetAsWaveFormat:recorderFilePath outPath:destination] ) {
        //fire ERROR
        [self fireCallback:CALLBACK_STATUS_ERROR param_name:@"message" param_value:@"can not save WAV after capturing"];
        if (isStopping) {
          isStopping = NO;
          self.callback = nil;
        }
    }
}

- (void)audioRecorderDidFinishRecording:(AVAudioRecorder *) aRecorder successfully:(BOOL)flag
{
    recorder = nil;
    isStarted = NO;
    //NSLog (@"audioRecorderDidFinishRecording:successfully:");
    // your actions here
    if (flag) {
        if (isSaveFile) {
            [self performSelectorOnMainThread:@selector(convertCommand) withObject:nil waitUntilDone:NO];
        }
        else {
            //fire CANCEL
            [self fireCallback:CALLBACK_STATUS_CANCEL param_name:nil param_value:nil];
            self.callback = nil;
            isStopping = NO;
        }
    }
    else {
        //fire ERROR
        [self fireCallback:CALLBACK_STATUS_ERROR param_name:nil param_value:nil];
        self.callback = nil;
        isStopping = NO;
    }
}


- (void) fireCallback:(NSString*)status param_name:(NSString*)param_name param_value:(NSString*)param_value {
    
    if (param_name != nil) {
        NSLog(@"$$$ AudioCapture. CALLBACK %@, %@, %@", status, param_name, param_value);
    }
    
    
    if (callback == nil) {
        return;
    }
	/*
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
    */
    NSMutableDictionary* dict = [NSMutableDictionary dictionaryWithCapacity:2];
    [dict setObject:status forKey:HK_STATUS];
    if ((param_name != nil) && (param_value != nil)) {
        [dict setObject:param_value forKey:param_name];
    }
    [callback setResult:dict];
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
        [self fireCallback:CALLBACK_STATUS_OK param_name:HK_FILE_NAME param_value:[@"file://" stringByAppendingString:destination]];//[NSString stringWithFormat:@"%d", durInMilis]];
      
        if (isStopping) {
          isStopping = NO;
          self.callback = nil;
        }
    }];
    
    dispatch_release(queue);
    return YES;
}






-(void) start:(NSDictionary*)props methodResult:(id<IMethodResult>)methodResult {
    if (isStarted) {
        [self fireCallback:CALLBACK_STATUS_ERROR param_name:HK_MESSAGE param_value:@"Audiocapture process already started. Please cancel or stop previous process before start new one."];
        return;
    }
    isStarted = YES;
    isStopping = NO;
  
    CMethodResult_SimpleHolder* sh = [CMethodResult_SimpleHolder makeEmptyHolder];
    [super setProperties:props methodResult:sh];
  
    self.callback = methodResult;
    
    destination = [[self getPreparedFileNameWithHash:props] retain];
    if (destination == nil) {
        isStarted = NO;
        [self fireCallback:CALLBACK_STATUS_ERROR param_name:@"message" param_value:@"File name must be set atleast once."];
        return;
    }
    maxDuration = [self getMaxDurationWithHash:props];
    [self performSelectorOnMainThread:@selector(startCommand) withObject:nil waitUntilDone:NO];
}

-(void) stop:(id<IMethodResult>)methodResult {
    isStarted = NO;
    [self performSelectorOnMainThread:@selector(stopCommand) withObject:nil waitUntilDone:NO];
}

-(void) cancel:(id<IMethodResult>)methodResult {\
    isStarted = NO;
    [self performSelectorOnMainThread:@selector(cancelCommand) withObject:nil waitUntilDone:NO];
}


-(int) getMaxDurationWithHash:(NSDictionary*)options {
    
    CMethodResult_SimpleHolder* sh = [CMethodResult_SimpleHolder makeEmptyHolder];
    
    [self getMaxDuration:sh];
    NSNumber* num = [sh getResult];
    
    int dur = [num intValue];
    
    if (options != nil) {
        NSObject* val = [options objectForKey:PROPERTY_MAX_DURATION];
        if (val != nil) {
            if ([val isKindOfClass:[NSNumber class]]) {
                dur = [((NSNumber*)val) intValue];
            }
            if ([val isKindOfClass:[NSString class]]) {
                dur = [((NSString*)val) intValue];
            }
        }
    }
    
    if (dur < 1000) {
        dur = 20000;
    }
    
    return dur;
}

-(NSString*) getPreparedFileNameWithHash:(NSDictionary*)options {
    CMethodResult_SimpleHolder* sh = [CMethodResult_SimpleHolder makeEmptyHolder];
    
    [self getFileName:sh];
    NSString* name = [sh getResult];
    
    if (options != nil) {
        NSObject* val = [options objectForKey:PROPERTY_FILE_NAME];
        if (val != nil) {
            if ([val isKindOfClass:[NSString class]]) {
                NSString* names = (NSString*)val;
                if ([names length] > 0) {
                    name = names;
                }
            }
        }
    }

    if (name == nil) {
        return nil;
    }
    if ([name length] <= 0) {
        return nil;
    }
    
    return [self getPreparedFileNameWithName:name];
}

-(NSString*) getPreparedFileNameWithName:(NSString*)name {
    
    NSString* res = @"";
    
    if (name == nil) {
        name = @"audiocapture";
    }
    
    if ([name length] <= 0) {
        return nil;
    }
    
    NSRange range = [name rangeOfString:@"/"];
    if (range.location != NSNotFound ) {
        //full path
        NSRange area;
        if ([name length] >= 4) {
            area.location = [name length] - 4;
        }
        else {
            area.location = 0;
        }
        
        area.length = [name length] - area.location;
        range = [name rangeOfString:@".wav" options:NSCaseInsensitiveSearch range:area];
        if (range.location != NSNotFound ) {
            // already has .wav extension
            return name;
        }
        return [name stringByAppendingString:@".wav"];
    
    }
    else {
        // only name

        NSString *folder = [NSString stringWithUTF8String:rho_rhodesapp_getblobsdirpath()];
        
        NSFileManager *fileManager = [NSFileManager defaultManager];
        if (![fileManager fileExistsAtPath:folder]) {
            NSError* error;
            [fileManager createDirectoryAtPath:folder withIntermediateDirectories:YES attributes:nil error:&error];
            
        }
        NSRange area;
        if ([name length] >= 4) {
            area.location = [name length] - 4;
        }
        else {
            area.location = 0;
        }
        
        area.length = [name length] - area.location;
        range = [name rangeOfString:@".wav" options:NSCaseInsensitiveSearch range:area];
        if (range.location == NSNotFound ) {
            name = [name stringByAppendingString:@".wav"];
        }
        
        res = [folder stringByAppendingPathComponent:name];
    }
    
    return res;
}


@end