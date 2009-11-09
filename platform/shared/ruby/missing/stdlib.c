#ifdef OS_ANDROID

#include <sys/types.h>

int mblen(const char *mbstr, size_t count)
{
  return 1;
}

#endif // OS_ANDROID

