

#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>






#ifdef __cplusplus
extern "C" {
#endif //__cplusplus
	
void Barcode_executeRhoCallBackWithParams(NSString* callbackURL,
	NSString* intName, int intValue,
    NSString* str1Name, NSString* str1Value,
    NSString* str2Name, NSString* str2Value,
    NSString* str3Name, NSString* str3Value,
    NSString* dataName, NSData* dataValue
);

#ifdef __cplusplus
}
#endif //__cplusplus
