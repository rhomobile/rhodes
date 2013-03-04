
#import <Foundation/Foundation.h>
#import "IMethodResult.h"



typedef struct RHO_API_PARAM RHO_API_PARAM;

typedef enum {
    RHO_API_STRING,
    RHO_API_INTEGER,
    RHO_API_FLOAT,
    RHO_API_BOOLEAN,
    RHO_API_ARRAY,
    RHO_API_HASH,
    RHO_API_OBJECT
    
}RHO_API_PARAM_TYPE;

struct RHO_API_PARAM{
    RHO_API_PARAM_TYPE type;
    const char* class_name;
    const char* name;
    int subtypes_count;
    RHO_API_PARAM* subtypes;
};

@interface CJSConverter : NSObject {
}

+ (NSString*) convertToJS:(NSObject*)objectiveC_value level:(int)level;

+ (NSObject*) convertFromJS:(void*)json_entry rho_api_param:(RHO_API_PARAM*)rho_api_param;

+ (NSObject*) getObjectiveCNULL;

@end
