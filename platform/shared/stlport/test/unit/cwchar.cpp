// #include <cwchar>
// #include <climits>
#include <cstdio>
#include <cwchar>
#include <climits>

#if defined(WCHAR_MIN) && (WCHAR_MIN == 0)
// do nothing, this is test for macro expansion
#endif

#if defined(WCHAR_MAX) && (WCHAR_MAX <= USHRT_MAX)
// do nothing, this is test for macro expansion
#endif
