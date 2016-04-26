
#import "SignatureSingleton.h"
#import "api_generator/iphone/CMethodResult.h"
#import "api_generator/iphone/CRubyConverter.h"
#include "Classes/Signature/SignatureParam.h"

extern void init_signature_param( struct SignatureParam* sigparam );
extern void rho_signature_take_ex( id<IMethodResult> callback, struct SignatureParam* sig_params);
extern void rho_signature_visible_ex(bool b, struct SignatureParam* p);
extern void rho_signature_capture(const char* callback_url);
extern void rho_signature_clear();
extern void rho_signature_capture_ex(id<IMethodResult> callback);


@implementation SignatureSingleton

@synthesize mCompressionFormat;
@synthesize mOutputFormat;
@synthesize mFileName;
@synthesize mBorder;
@synthesize mPenColor;
@synthesize mPenWidth;
@synthesize mBgColor;
@synthesize mLeft;
@synthesize mTop;
@synthesize mWidth;
@synthesize mHeight;
@synthesize mFullScreen;

-(id)init {
    self = [super init];
    
    self.mCompressionFormat = @"png";
    self.mOutputFormat = @"image";
    self.mBorder = NO;
    self.mPenColor = @"#FF000000";
    self.mPenWidth = 3;
    self.mBgColor = @"#FFFFFFFF";
    self.mLeft = 15;
    self.mTop = 60;
    self.mWidth = 200;
    self.mHeight = 150;
    self.mFullScreen = NO;
    self.mFileName = @"signature";
    
    return self;
}





-(void) dictToSigParam:(NSDictionary*)dict param:(struct SignatureParam*)sig
{
    // set to default
    sig->compressionFormat = CF_PNG;
    if ( [self.mCompressionFormat isEqualToString:@"jpg"])
    {
        sig->compressionFormat = CF_JPEG;
    } else if ([self.mCompressionFormat isEqualToString:@"png"])
    {
        sig->compressionFormat = CF_PNG;
    } else if ([self.mCompressionFormat isEqualToString:@"bmp"])
    {
        sig->compressionFormat = CF_PNG;
    }
    sig->penColor = [self convertStringToInt:self.mPenColor];
    sig->penWidth = self.mPenWidth;
    sig->bgColor = [self convertStringToInt:self.mBgColor];
    sig->left = self.mLeft;
    sig->top = self.mTop;
    sig->width = self.mWidth;
    sig->height = self.mHeight;
    sig->fileName = [self.mFileName retain];
    
    if (![dict isKindOfClass:[NSDictionary class]]) {
        return;
    }
    
    // dictionary
    NSEnumerator* enumerator = [dict keyEnumerator];
    NSObject* obj = nil;

    
    // copy sig params to object properties (now hash from sjow() and takeFullScreen() should modify object parameters)
    
    while ((obj = [enumerator nextObject]) != nil) {
        NSString* objKey = (NSString*)obj;
        NSString* objVal = (NSString*)[dict objectForKey:objKey];
        
        if ([objKey isEqualToString:@"imageFormat"] || [objKey isEqualToString:@"compressionFormat"]) {
            if ( [objVal isEqualToString:@"jpg"])
            {
                sig->compressionFormat = CF_JPEG;
                self.mCompressionFormat = @"jpg";
            } else if ([objVal isEqualToString:@"png"])
            {
                sig->compressionFormat = CF_PNG;
                self.mCompressionFormat = @"png";
            } else if ([objVal isEqualToString:@"bmp"])
            {
                sig->compressionFormat = CF_PNG;
                self.mCompressionFormat = @"png";
            }
        } else if ([objKey isEqualToString:@"penColor"]) {
            sig->penColor = [self convertStringToInt:objVal];
            self.mPenColor = objVal;
        } else if ([objKey isEqualToString:@"penWidth"]) {
            sig->penWidth = (float)[objVal intValue];
            self.mPenWidth = (int)sig->penWidth;
        } else if ([objKey isEqualToString:@"bgColor"]) {
            sig->bgColor = [self convertStringToInt:objVal];
            self.mBgColor = objVal;
        } else if ([objKey isEqualToString:@"left"]) {
            sig->left = (int)[objVal intValue];
            self.mLeft = sig->left;
        } else if ([objKey isEqualToString:@"top"]) {
            sig->top = (int)[objVal intValue];
            self.mTop = sig->top;
        } else if ([objKey isEqualToString:@"width"]) {
            sig->width = (int)[objVal intValue];
            self.mWidth = sig->width;
        } else if ([objKey isEqualToString:@"height"]) {
            sig->height = (int)[objVal intValue];
            self.mHeight = sig->height;
        } else if ([objKey isEqualToString:@"fileName"] || [objKey isEqualToString:@"name"]) {
            sig->fileName = [objVal retain];
            self.mFileName = objVal;
        }
    }
    
}

-(void) takeFullScreen:(NSDictionary*)propertyMap methodResult:(id<IMethodResult>)methodResult {
    struct SignatureParam sigparams;
    init_signature_param(&sigparams);
    
    [self dictToSigParam:propertyMap param:&sigparams];
    
    rho_signature_take_ex((NSObject<IMethodResult>*)methodResult, &sigparams);
}

-(void) show:(NSDictionary*)propertyMap methodResult:(id<IMethodResult>)methodResult{
    struct SignatureParam sigparams;
    init_signature_param(&sigparams);
    
    [self dictToSigParam:propertyMap param:&sigparams];
    
    rho_signature_visible_ex(true, &sigparams);
}

-(void) capture:(id<IMethodResult>)methodResult{
    rho_signature_capture_ex(methodResult);
}

-(void) clear:(id<IMethodResult>)methodResult{
    rho_signature_clear();
}

-(void) hide:(id<IMethodResult>)methodResult{
    rho_signature_visible_ex(false, nil);
}

-(void) setVectorCallback:(id<IMethodResult>)methodResult {
    // not yet implemented
}






-(void) getCompressionFormat:(id<IMethodResult>)methodResult {
    [methodResult setResult:self.mCompressionFormat];
}

-(void) setCompressionFormat:(NSString*)compressionFormat methodResult:(id<IMethodResult>)methodResult {
    self.mCompressionFormat = compressionFormat;
}

-(void) getOutputFormat:(id<IMethodResult>)methodResult {
    [methodResult setResult:self.mOutputFormat];
}

-(void) setOutputFormat:(NSString*)outputFormat methodResult:(id<IMethodResult>)methodResult {
    self.mOutputFormat = outputFormat;
}

-(void) getFileName:(id<IMethodResult>)methodResult {
    [methodResult setResult:self.mFileName];
}

-(void) setFileName:(NSString*)fileName methodResult:(id<IMethodResult>)methodResult {
    self.mFileName = fileName;
}

-(void) getBorder:(id<IMethodResult>)methodResult {
    [methodResult setResult:[NSNumber numberWithBool:self.mBorder]];
}

-(void) setBorder:(BOOL)border methodResult:(id<IMethodResult>)methodResult {
    self.mBorder = border;
}

-(void) getPenWidth:(id<IMethodResult>)methodResult {
    [methodResult setResult:[NSNumber numberWithInt:self.mPenWidth]];
    
}

-(void) setPenWidth:(int)penWidth methodResult:(id<IMethodResult>)methodResult {
    self.mPenWidth = penWidth;
}

-(void) getLeft:(id<IMethodResult>)methodResult {
    [methodResult setResult:[NSNumber numberWithInt:self.mLeft]];
}

-(void) setLeft:(int)left methodResult:(id<IMethodResult>)methodResult {
    self.mLeft = left;
}

-(void) getTop:(id<IMethodResult>)methodResult {
    [methodResult setResult:[NSNumber numberWithInt:self.mTop]];
}

-(void) setTop:(int)top methodResult:(id<IMethodResult>)methodResult {
    self.mTop = top;
}

-(void) getWidth:(id<IMethodResult>)methodResult {
    [methodResult setResult:[NSNumber numberWithInt:self.mWidth]];
}

-(void) setWidth:(int)width methodResult:(id<IMethodResult>)methodResult {
    self.mWidth = width;
}

-(void) getHeight:(id<IMethodResult>)methodResult {
    [methodResult setResult:[NSNumber numberWithInt:self.mHeight]];
}

-(void) setHeight:(int)height methodResult:(id<IMethodResult>)methodResult {
    self.mHeight = height;
}

-(void) getFullScreen:(id<IMethodResult>)methodResult {
    [methodResult setResult:[NSNumber numberWithBool:self.mFullScreen]];
}

-(void) setFullScreen:(BOOL)fullScreen methodResult:(id<IMethodResult>)methodResult {
    self.mFullScreen = fullScreen;
}


/* getter for "penColor" property */
-(void) getPenColor:(id<IMethodResult>)methodResult {
    [methodResult setResult:self.mPenColor];
}

/* setter for "penColor" property */
-(void) setPenColor:(NSString*)penColor methodResult:(id<IMethodResult>)methodResult {
    self.mPenColor = penColor;
}

/* getter for "bgColor" property */
-(void) getBgColor:(id<IMethodResult>)methodResult {
    [methodResult setResult:self.mBgColor];
}

/* setter for "bgColor" property */
-(void) setBgColor:(NSString*)bgColor methodResult:(id<IMethodResult>)methodResult {
    self.mBgColor = bgColor;
}

-(int) convertStringToInt:(NSString*)snum {
    if ([snum isKindOfClass:[NSNumber class]]) {
        return [snum intValue];
    }
    if (![snum isKindOfClass:[NSString class]]) {
        return 0xFF000000;
    }
    if ([snum hasPrefix:@"#"]) {
        int result = 0xFF000000;
        NSScanner *scanner = [NSScanner scannerWithString:snum];
        [scanner setScanLocation:1]; // bypass '#' character
        [scanner scanHexInt:&result];
        if ([snum length] <= 7) {
            result = result | 0xFF000000;
        }
        return result;
    }
    else if ([snum hasPrefix:@"0x"]) {
        int result = 0xFF000000;
        NSScanner *scanner = [NSScanner scannerWithString:snum];
        [scanner setScanLocation:2]; // bypass '#' character
        [scanner scanHexInt:&result];
        return result;
    }
    return [snum intValue];
}




@end