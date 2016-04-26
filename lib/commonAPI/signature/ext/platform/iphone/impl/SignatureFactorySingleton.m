
#import "ISignature.h"
#import "SignatureFactoryBase.h"

static SignatureFactoryBase* ourSignatureFactory = nil;

@implementation SignatureFactorySingleton

+(id<ISignatureFactory>) getSignatureFactoryInstance {
    if (ourSignatureFactory == nil) {
        ourSignatureFactory = [[SignatureFactoryBase alloc] init];
    }
    return ourSignatureFactory;
}

@end