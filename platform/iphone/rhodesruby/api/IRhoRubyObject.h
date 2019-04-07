
#import <Foundation/Foundation.h>


typedef enum RHO_RUBY_BASIC_TYPES : NSUInteger {
    kRhoRubyNone,
    // types decorate real ruby objects
    kRhoRubyNil,
    kRhoRubyClass,
    kRhoRubyObject,
    kRhoRubyBoolean,
    kRhoRubyInteger,
    kRhoRubyFloat,
    kRhoRubyString,
    kRhoRubyArray,
    kRhoRubyHash,
    // types for prepare parameters for send to ruby - can be changed instead of read-only base types (used for receive ruby objects from ruby code)
    kRhoRubyMutableBoolean,
    kRhoRubyMutableInteger,
    kRhoRubyMutableFloat,
    kRhoRubyMutableString,
    kRhoRubyMutableArray,
    kRhoRubyMutableHash
} RHO_RUBY_BASIC_TYPES;



@protocol IRhoRubyObject <NSObject>

- (NSString*) getClassName;

- (RHO_RUBY_BASIC_TYPES) getBasicType;


@end


