
#import "ISignature.h"
#import "SignatureSingletonBase.h"

@interface SignatureSingleton : SignatureSingletonBase<ISignatureSingleton> {
    
    NSString* mCompressionFormat;
    NSString* mOutputFormat;
    NSString* mFileName;
    BOOL mBorder;
    NSString* mPenColor;
    int mPenWidth;
    NSString* mBgColor;
    int mLeft;
    int mTop;
    int mWidth;
    int mHeight;
    BOOL mFullScreen;
}


@property (nonatomic,retain) NSString* mCompressionFormat;
@property (nonatomic,retain) NSString* mOutputFormat;
@property (nonatomic,retain) NSString* mFileName;
@property (nonatomic,assign) BOOL mBorder;
@property (nonatomic,retain) NSString* mPenColor;
@property (nonatomic,assign) int mPenWidth;
@property (nonatomic,retain) NSString* mBgColor;
@property (nonatomic,assign) int mLeft;
@property (nonatomic,assign) int mTop;
@property (nonatomic,assign) int mWidth;
@property (nonatomic,assign) int mHeight;
@property (nonatomic,assign) BOOL mFullScreen;


-(void) getCompressionFormat:(id<IMethodResult>)methodResult;
-(void) setCompressionFormat:(NSString*)compressionFormat methodResult:(id<IMethodResult>)methodResult;
-(void) getOutputFormat:(id<IMethodResult>)methodResult;
-(void) setOutputFormat:(NSString*)outputFormat methodResult:(id<IMethodResult>)methodResult;
-(void) getFileName:(id<IMethodResult>)methodResult;
-(void) setFileName:(NSString*)fileName methodResult:(id<IMethodResult>)methodResult;
-(void) getBorder:(id<IMethodResult>)methodResult;
-(void) setBorder:(BOOL)border methodResult:(id<IMethodResult>)methodResult;
-(void) getPenWidth:(id<IMethodResult>)methodResult;
-(void) setPenWidth:(int)penWidth methodResult:(id<IMethodResult>)methodResult;
-(void) getLeft:(id<IMethodResult>)methodResult;
-(void) setLeft:(int)left methodResult:(id<IMethodResult>)methodResult;
-(void) getTop:(id<IMethodResult>)methodResult;
-(void) setTop:(int)top methodResult:(id<IMethodResult>)methodResult;
-(void) getWidth:(id<IMethodResult>)methodResult;
-(void) setWidth:(int)width methodResult:(id<IMethodResult>)methodResult;
-(void) getHeight:(id<IMethodResult>)methodResult;
-(void) setHeight:(int)height methodResult:(id<IMethodResult>)methodResult;
-(void) getFullScreen:(id<IMethodResult>)methodResult;
-(void) setFullScreen:(BOOL)fullScreen methodResult:(id<IMethodResult>)methodResult;
-(void) takeFullScreen:(NSDictionary*)propertyMap methodResult:(id<IMethodResult>)methodResult;
-(void) show:(NSDictionary*)propertyMap methodResult:(id<IMethodResult>)methodResult;
-(void) capture:(id<IMethodResult>)methodResult;
-(void) clear:(id<IMethodResult>)methodResult;
-(void) hide:(id<IMethodResult>)methodResult;
-(void) setVectorCallback:(id<IMethodResult>)methodResult;

-(int) convertStringToInt:(NSString*)snum;

@end