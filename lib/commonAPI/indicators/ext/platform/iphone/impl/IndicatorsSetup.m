#import <Foundation/Foundation.h>

extern void Init_SignalIndicators_extension();
extern void Init_Keystate_extension();
extern void Init_Battery_extension();

void Init_Indicators_extension()
{
    Init_SignalIndicators_extension();
    Init_Battery_extension();  
}