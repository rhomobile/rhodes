
#import "<%= name.camel_case %>.h"

#include "ruby/ext/rho/rhoruby.h"



VALUE <%= name.downcase %>_native_process_string(const char* str) {
	
    NSString* os_string = @"<iOS>";
    
    NSString* result_string = [os_string stringByAppendingString:[NSString stringWithUTF8String:str]];
    result_string = [result_string stringByAppendingString:os_string];
    
	VALUE result = rho_ruby_create_string([result_string UTF8String]);
    
    return result;
}


int <%= name.downcase %>_calc_summ(int x, int y) {
	return (x+y);
}

