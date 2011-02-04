#include <math.h>
#include "common/RhoMath.h"

double rho_math_sqrt(double n)
{
    return sqrt(n);
}

uint64 rho_math_pow2(int n)
{
    uint64 ret;
    for (ret = 1; n > 0; --n)
        ret *= 2;
    return ret;
}

double rho_math_log2(double n)
{
    return log(n)/M_LN2;
}

double rho_math_sin(double n)
{
    return sin(n);
}

double rho_math_asin(double n)
{
    return asin(n);
}

double rho_math_tanh(double n)
{
    double epx = exp(n);
    double emx = exp(-n);
    return (epx - emx)/(epx + emx);
}

double rho_math_atanh(double n)
{
    return 0.5 * log((1 + n)/(1 - n));
}

