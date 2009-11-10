#ifndef __MATH_AUX_H
#define __MATH_AUX_H

#include <limits>

/*
 * This function is not only used to compare floating point values with a tolerance,
 * it also leads to ambiguity problems if the called functions do not have the
 * right prototype.
 */
template <class _Tp>
bool are_equals(_Tp val, _Tp ref) {
  if (val < ref) {
    return (ref - val) <= std::numeric_limits<_Tp>::epsilon();
  }
  else {
    return (val - ref) <= std::numeric_limits<_Tp>::epsilon();
  }
}

#endif // __MATH_AUX_H
