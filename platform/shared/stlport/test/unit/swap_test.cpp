#include <vector>
#include <algorithm>

#include "cppunit/cppunit_proxy.h"

#if !defined (STLPORT) || defined(_STLP_USE_NAMESPACES)
using namespace std;
#endif

//
// TestCase class
//
class SwapTest : public CPPUNIT_NS::TestCase
{
  CPPUNIT_TEST_SUITE(SwapTest);
  CPPUNIT_TEST(swap1);
  CPPUNIT_TEST(swprnge1);
  CPPUNIT_TEST_SUITE_END();

protected:
  void swap1();
  void swprnge1();
};

CPPUNIT_TEST_SUITE_REGISTRATION(SwapTest);

//
// tests implementation
//
void SwapTest::swap1()
{
  int a = 42;
  int b = 19;
  swap(a, b);

  CPPUNIT_ASSERT(a==19);
  CPPUNIT_ASSERT(b==42);
}
void SwapTest::swprnge1()
{
  char word1[] = "World";
  char word2[] = "Hello";
  swap_ranges((char*)word1, (char*)word1 + ::strlen(word1), (char*)word2);
  CPPUNIT_ASSERT(!strcmp(word1, "Hello"));
  CPPUNIT_ASSERT(!strcmp(word2, "World"));
}
