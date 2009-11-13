#include <utility>
#include <vector>
#include <algorithm>

#include "cppunit/cppunit_proxy.h"

#if !defined (STLPORT) || defined(_STLP_USE_NAMESPACES)
using namespace std;
#endif

//
// TestCase class
//
class PairTest : public CPPUNIT_NS::TestCase
{
  CPPUNIT_TEST_SUITE(PairTest);
  CPPUNIT_TEST(pair0);
  CPPUNIT_TEST_SUITE_END();

protected:
  void pair0();
};

CPPUNIT_TEST_SUITE_REGISTRATION(PairTest);

//
// tests implementation
//
void PairTest::pair0()
{
  pair<int, int> p = make_pair(1, 10);

  CPPUNIT_ASSERT(p.first==1);
  CPPUNIT_ASSERT(p.second==10);
}
