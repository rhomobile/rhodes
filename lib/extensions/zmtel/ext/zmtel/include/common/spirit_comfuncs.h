#ifndef __SPIRIT_COMFUNCS_H
#define __SPIRIT_COMFUNCS_H

#define ABS(X)   ( ((X)>0)?(X):(-(X)) )
#define Abs(X)   ( ((X)>0)?(X):(-(X)) )

#define MAX(X,Y) ( ((X)>=(Y))?(X):(Y) )
#define Max(X,Y) ( ((X)>=(Y))?(X):(Y) )

#define MIN(Value1,Value2) ( ( (Value1)<=(Value2) ) ? (Value1) : (Value2) )
#define Min(Value1,Value2) ( ( (Value1)<=(Value2) ) ? (Value1) : (Value2) )

#define HIGH(LValue) ( (LValue)>>16 )
#define High(LValue) ( (LValue)>>16 )

#define COUNT(x) (sizeof(x)/sizeof((x)[0]))

#endif // __SPIRIT_COMFUNCS_H
