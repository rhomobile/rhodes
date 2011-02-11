#ifndef _RHOMATH_H_
#define _RHOMATH_H_

#include "common/RhoPort.h"

#ifdef __cplusplus
extern "C" {
#endif

double rho_math_sqrt(double n);
uint64 rho_math_pow2(int n);

double rho_math_log2(double n);

double rho_math_sin(double n);
double rho_math_asin(double n);

/* Hyperbolic tangent */
double rho_math_tanh(double n);
/* Hyperbolic arctangent */
double rho_math_atanh(double n);

#ifdef __cplusplus
} // extern "C"
#endif

#endif
