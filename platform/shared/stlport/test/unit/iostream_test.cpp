#include <string>

#if !defined (STLPORT) || !defined (_STLP_USE_NO_IOSTREAMS)
#  include <sstream>

#  include "cppunit/cppunit_proxy.h"

#  if !defined (STLPORT) || defined(_STLP_USE_NAMESPACES)
using namespace std;
#  endif

//
// TestCase class
//
class IOStreamTest : public CPPUNIT_NS::TestCase
{
  CPPUNIT_TEST_SUITE(IOStreamTest);
  CPPUNIT_TEST(manipulators);
  CPPUNIT_TEST_SUITE_END();

private:
  void manipulators();
};

CPPUNIT_TEST_SUITE_REGISTRATION(IOStreamTest);

//
// tests implementation
//
void IOStreamTest::manipulators()
{
  {
    istringstream istr;
    istr.str("bar");

    istr >> ws;
    CPPUNIT_ASSERT( istr.good() );

    string foo;
    istr >> foo;
    CPPUNIT_ASSERT( istr.eof() );
    CPPUNIT_ASSERT( !istr.fail() );
    CPPUNIT_ASSERT( foo == "bar" );

    istr >> ws;
    CPPUNIT_ASSERT( istr.eof() );
    CPPUNIT_ASSERT( !istr.fail() );
    istr.clear();
  }

  {
    istringstream istr;
    istr.str("  bar  ");

    istr >> ws;
    CPPUNIT_ASSERT( istr.good() );

    string foo;
    istr >> foo;
    CPPUNIT_ASSERT( !istr.eof() );
    CPPUNIT_ASSERT( !istr.fail() );
    CPPUNIT_ASSERT( foo == "bar" );

    istr >> ws;
    CPPUNIT_ASSERT( istr.eof() );
    CPPUNIT_ASSERT( !istr.fail() );
    istr.clear();
  }
}

#endif
