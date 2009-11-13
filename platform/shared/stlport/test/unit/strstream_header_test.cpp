/* This test purpose is simply to check Standard header independancy that
 * is to say that the header can be included alone without any previous
 * include.
 * Additionnaly, for C Standard headers that STLport expose, it can also be
 * used to check that files included by those headers are compatible with
 * pure C compilers.
 */

//Including this fle makes the test STLport specific but this is the only way to check
//compiler config:
#include <stl/config/features.h>

#if !defined (_STLP_USE_NO_IOSTREAMS)
#  include <strstream>
#endif
