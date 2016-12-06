
#import "DeviceSecuritySingleton.h"
#import <UIKit/UIKit.h>

@implementation DeviceSecuritySingleton

-(BOOL) isJailbroken
{
#if !(TARGET_IPHONE_SIMULATOR)
    
    if ([[NSFileManager defaultManager] fileExistsAtPath:@"/Applications/Cydia.app"] ||
        [[NSFileManager defaultManager] fileExistsAtPath:@"/Library/MobileSubstrate/MobileSubstrate.dylib"] ||
        [[NSFileManager defaultManager] fileExistsAtPath:@"/bin/bash"] ||
        [[NSFileManager defaultManager] fileExistsAtPath:@"/usr/sbin/sshd"] ||
        [[NSFileManager defaultManager] fileExistsAtPath:@"/etc/apt"] ||
        [[NSFileManager defaultManager] fileExistsAtPath:@"/private/var/lib/apt/"] ||
        [[UIApplication sharedApplication] canOpenURL:[NSURL URLWithString:@"cydia://package/com.example.package"]])
    {
        return YES;
    }
    
    FILE *f = NULL ;
    if ((f = fopen("/bin/bash", "r")) ||
        (f = fopen("/Applications/Cydia.app", "r")) ||
        (f = fopen("/Library/MobileSubstrate/MobileSubstrate.dylib", "r")) ||
        (f = fopen("/usr/sbin/sshd", "r")) ||
        (f = fopen("/etc/apt", "r")))
    {
        fclose(f);
        return YES;
    }
    fclose(f);
    
    NSError *error;
    NSString *stringToBeWritten = @"This is a test.";
    [stringToBeWritten writeToFile:@"/private/jailbreak.txt" atomically:YES encoding:NSUTF8StringEncoding error:&error];
    [[NSFileManager defaultManager] removeItemAtPath:@"/private/jailbreak.txt" error:nil];
    if(error == nil)
    {
        return YES;
    }
    
#endif
    
    return NO;
}

-(void) isDeviceSecured:(id<IMethodResult>)methodResult {
    [methodResult setResult:[NSNumber numberWithBool:![self isJailbroken]]];
}

-(void) isDeviceJailbroken:(id<IMethodResult>)methodResult {
    [methodResult setResult:[NSNumber numberWithBool:[self isJailbroken]]];
}

-(void) isDeviceRooted:(id<IMethodResult>)methodResult {
    [methodResult setResult:[NSNumber numberWithBool:NO]];
}

-(void) isRunOnSimulator:(id<IMethodResult>)methodResult {
#if !(TARGET_IPHONE_SIMULATOR)
    [methodResult setResult:[NSNumber numberWithBool:NO]];
#else
    [methodResult setResult:[NSNumber numberWithBool:YES]];
#endif
}

-(void) isDebugMode:(id<IMethodResult>)methodResult {
    [methodResult setResult:[NSNumber numberWithBool:NO]];
}




@end
