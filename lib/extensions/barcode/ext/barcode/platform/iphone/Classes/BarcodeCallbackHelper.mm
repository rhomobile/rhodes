


// rhodes/platform/shared/rubyext/WebView.h
#import "rubyext/WebView.h"
//void rho_webview_navigate(const char* url, int index);


// rhodes/platform/shared/common/RhodesApp.h
#import "common/RhodesApp.h"
//void rho_net_request(const char *url);
//char* rho_http_normalizeurl(const char* szUrl);

//#include "net/INetRequest.h"

#include "ruby/ext/rho/rhoruby.h"

#include <string.h>

#import "BarcodeCallbackHelper.h"


/*
 VALUE rho_ruby_create_string(const char* szVal);
 VALUE rho_ruby_create_string_withlen(int len);
 VALUE rho_ruby_create_boolean(unsigned char b);
 VALUE rho_ruby_create_integer(__int64 i);
 VALUE rho_ruby_create_double(double d);
 VALUE rho_ruby_create_time(long t);
 void rho_ruby_add_to_array(VALUE ar, VALUE val);
 
 time_t rho_ruby_get_time(VALUE rTime);
 
 VALUE rho_ruby_create_byte_array(unsigned char* buf, int length);	
 int rho_ruby_unpack_byte_array(VALUE array_value, unsigned char* buf, int max_length);

*/



class BarcodeRhoCallbackIntContainer : public rho::ICallbackObject
{
public:
	BarcodeRhoCallbackIntContainer(int number)
	{
		mNumber = number;
	}
	virtual ~BarcodeRhoCallbackIntContainer() {
	}
	
	// return Ruby object
	virtual unsigned long getObjectValue() {
		unsigned long rrr = rho_ruby_create_integer(mNumber);
		return rrr;
	}
	
private:
	int mNumber;
	
};

class BarcodeRhoCallbackStringContainer : public rho::ICallbackObject
{
public:
	BarcodeRhoCallbackStringContainer(NSString* str)
	{
		const char* utf8str = [str UTF8String];
		mString = new char[strlen(utf8str) + 1];
		strcpy(mString, utf8str);
	}
	virtual ~BarcodeRhoCallbackStringContainer() {
		delete mString;
	}
	
	// return Ruby object
	virtual unsigned long getObjectValue() {
		unsigned long rrr = rho_ruby_create_string((const char*)mString);
		return rrr;
	}
	
private:
	char* mString;
	
};

class BarcodeRhoCallbackDataContainer : public rho::ICallbackObject
{
public:
	BarcodeRhoCallbackDataContainer(NSData* data)
	{
		mDataSize = [data length];
		mData = new unsigned char[mDataSize+1];
		[data getBytes:mData length:mDataSize];
	}
	virtual ~BarcodeRhoCallbackDataContainer() {
		delete mData;
	}
	
	// return Ruby object
	virtual unsigned long getObjectValue() {
		return rho_ruby_create_byte_array(mData, mDataSize);
	}
	
private:
	unsigned char*	mData;
	int				mDataSize;
	
};



#ifdef __cplusplus
extern "C" {
#endif //__cplusplus


void Barcode_executeRhoCallBackWithParams(NSString* callbackURL,
							   NSString* intName, int intValue,
							   NSString* str1Name, NSString* str1Value,
							   NSString* str2Name, NSString* str2Value,
							   NSString* str3Name, NSString* str3Value,
							   NSString* dataName, NSData* dataValue
							   ) 
{
	NSString* strBody = @"&rho_callback=1";
	
	if (intName != nil) {
		strBody = [strBody stringByAppendingString:@"&"];
		
		//NSNumber *number = [NSNumber numberWithInt:intValue];
		//NSString* intValueStr = [NSString stringWithFormat:@"%@",number];
		//NSString* strParam = @"";						 
		//strParam = [strParam stringByAppendingString:intName];
		//strParam = [strParam stringByAppendingString:@"="];
		//strParam = [strParam stringByAppendingString:intValueStr];
		NSString* strParam = [NSString stringWithUTF8String:(RHODESAPP().addCallbackObject( new BarcodeRhoCallbackIntContainer(intValue), [intName UTF8String]).c_str())]; 
		
		strBody = [strBody stringByAppendingString:strParam];
	}
	if ((str1Name != nil) && (str1Value != null)) {
		strBody = [strBody stringByAppendingString:@"&"];
		
		//NSString* strParam = @"";						 
		//strParam = [strParam stringByAppendingString:str1Name];
		//strParam = [strParam stringByAppendingString:@"="];
		//strParam = [strParam stringByAppendingString:str1Value];
		NSString* strParam = [NSString stringWithUTF8String:(RHODESAPP().addCallbackObject( new BarcodeRhoCallbackStringContainer(str1Value), [str1Name UTF8String]).c_str())]; 

		strBody = [strBody stringByAppendingString:strParam];
	}
	if ((str2Name != nil) && (str2Value != null)) {
		strBody = [strBody stringByAppendingString:@"&"];

		//NSString* strParam = @"";						 
		//strParam = [strParam stringByAppendingString:str2Name];
		//strParam = [strParam stringByAppendingString:@"="];
		//strParam = [strParam stringByAppendingString:str2Value];
		NSString* strParam = [NSString stringWithUTF8String:(RHODESAPP().addCallbackObject( new BarcodeRhoCallbackStringContainer(str2Value), [str2Name UTF8String]).c_str())]; 

		strBody = [strBody stringByAppendingString:strParam];
	}
	if ((str3Name != nil) && (str3Value != null)) {
		strBody = [strBody stringByAppendingString:@"&"];
		
		//NSString* strParam = @"";						 
		//strParam = [strParam stringByAppendingString:str3Name];
		//strParam = [strParam stringByAppendingString:@"="];
		//strParam = [strParam stringByAppendingString:str3Value];
		NSString* strParam = [NSString stringWithUTF8String:(RHODESAPP().addCallbackObject( new BarcodeRhoCallbackStringContainer(str3Value), [str3Name UTF8String]).c_str())]; 
		
		strBody = [strBody stringByAppendingString:strParam];
	}
	if (dataName != nil) {
		strBody = [strBody stringByAppendingString:@"&"];
		NSString* strParam = [NSString stringWithUTF8String:(RHODESAPP().addCallbackObject( new BarcodeRhoCallbackDataContainer(dataValue), [dataName UTF8String]).c_str())]; 
		strBody = [strBody stringByAppendingString:strParam];
	}
	
	const char* cb = [callbackURL UTF8String];
	const char* b = [strBody UTF8String];
    char* norm_url = rho_http_normalizeurl(cb);

	rho_net_request_with_data(rho_http_normalizeurl(cb), b);
    
    rho_http_free(norm_url);
    
    
}

	
#ifdef __cplusplus
	}
#endif //__cplusplus
	

