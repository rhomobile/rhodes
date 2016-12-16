
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

-(BOOL) isSimulator {
#if !(TARGET_IPHONE_SIMULATOR)
    return NO;
#else
    return YES;
#endif
}

-(void) isDeviceSecured:(id<IMethodResult>)methodResult {
    [methodResult setResult:[NSNumber numberWithBool:!([self isJailbroken] || [self isSimulator])]];
}

-(void) isDeviceJailbroken:(id<IMethodResult>)methodResult {
    [methodResult setResult:[NSNumber numberWithBool:[self isJailbroken]]];
}

-(void) isDeviceRooted:(id<IMethodResult>)methodResult {
    [methodResult setResult:[NSNumber numberWithBool:NO]];
}

-(void) isRunOnSimulator:(id<IMethodResult>)methodResult {
    [methodResult setResult:[NSNumber numberWithBool:[self isSimulator]]];
}

-(void) isDebugMode:(id<IMethodResult>)methodResult {
    [methodResult setResult:[NSNumber numberWithBool:NO]];
}

-(void) getAppCertificateSignatures:(id<IMethodResult>)methodResult {
    NSMutableArray *arr = [[NSMutableArray alloc] initWithCapacity:1];
    [methodResult setResult:arr];
}

-(void) getAllowedCertificateSignatures:(id<IMethodResult>)methodResult {
    NSMutableArray *arr = [[NSMutableArray alloc] initWithCapacity:1];
    [methodResult setResult:arr];
}

-(void) getInstallerPackageName:(id<IMethodResult>)methodResult {
    NSMutableArray *arr = [[NSMutableArray alloc] initWithCapacity:1];
    [methodResult setResult:@""];
}

-(void) getAllowedInstallerPackages:(id<IMethodResult>)methodResult {
    NSMutableArray *arr = [[NSMutableArray alloc] initWithCapacity:1];
    [methodResult setResult:arr];
}

-(void) haveAnySignatureIssues:(id<IMethodResult>)methodResult {
    [methodResult setResult:[NSNumber numberWithBool:NO]];
}


-(void) haveAnyInstallerIssues:(id<IMethodResult>)methodResult {
    [methodResult setResult:[NSNumber numberWithBool:NO]];
}


@end
