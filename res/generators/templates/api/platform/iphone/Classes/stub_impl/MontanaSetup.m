#import <Foundation/Foundation.h>

extern void Init_<%= $cur_module.name %>_API();

void Init_<%= $cur_module.name %>()
{
    Init_<%= $cur_module.name %>_API();
}