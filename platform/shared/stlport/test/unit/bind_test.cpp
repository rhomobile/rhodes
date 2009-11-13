#include <algorithm>
#include <functional>

#include "cppunit/cppunit_proxy.h"

#if !defined (STLPORT) || defined(_STLP_USE_NAMESPACES)
using namespace std;
#endif

//
// TestCase class
//
class BindTest : public CPPUNIT_NS::TestCase
{
  CPPUNIT_TEST_SUITE(BindTest);
  CPPUNIT_TEST(bind1st1);
  CPPUNIT_TEST(bind2nd1);
  CPPUNIT_TEST(bind2nd2);
#if !defined (STLPORT) || \
    defined (_STLP_NO_EXTENSIONS) || !defined (_STLP_CLASS_PARTIAL_SPECIALIZATION)
  CPPUNIT_IGNORE;
#endif
  CPPUNIT_TEST(bind2nd3);
  CPPUNIT_TEST(bind_memfn);
  CPPUNIT_TEST_SUITE_END();

protected:
  void bind1st1();
  void bind2nd1();
  void bind2nd2();
  void bind2nd3();
  void bind_memfn();
};

CPPUNIT_TEST_SUITE_REGISTRATION(BindTest);

//
// tests implementation
//
void BindTest::bind1st1()
{
  int array [3] = { 1, 2, 3 };
  int* p = remove_if((int*)array, (int*)array + 3, bind1st(less<int>(), 2));

  CPPUNIT_ASSERT(p==&array[2]);
  CPPUNIT_ASSERT(array[0]==1);
  CPPUNIT_ASSERT(array[1]==2);
}
void BindTest::bind2nd1()
{
  int array [3] = { 1, 2, 3 };
  replace_if(array, array + 3, binder2nd<greater<int> >(greater<int>(), 2), 4);

  CPPUNIT_ASSERT(array[0]==1);
  CPPUNIT_ASSERT(array[1]==2);
  CPPUNIT_ASSERT(array[2]==4);
}
void BindTest::bind2nd2()
{
  int array [3] = { 1, 2, 3 };
  replace_if(array, array + 3, bind2nd(greater<int>(), 2), 4);
  CPPUNIT_ASSERT(array[0]==1);
  CPPUNIT_ASSERT(array[1]==2);
  CPPUNIT_ASSERT(array[2]==4);
}

int test_func1 (const int &param1, const int &param2) {
  return param1 + param2;
}

int test_func2 (int &param1, int param2) {
  param1 += param2;
  return param1 + param2;
}

void BindTest::bind2nd3()
{
#if defined (STLPORT) && \
    !defined (_STLP_NO_EXTENSIONS) && defined (_STLP_CLASS_PARTIAL_SPECIALIZATION)
  int array[3] = { 1, 2, 3 };
  transform(array, array + 3, array, bind2nd(ptr_fun(test_func1), 1));
  transform(array, array + 3, array, bind1st(ptr_fun(test_func1), -1));
  CPPUNIT_ASSERT(array[0] == 1);
  CPPUNIT_ASSERT(array[1] == 2);
  CPPUNIT_ASSERT(array[2] == 3);

  transform(array, array + 3, array, bind2nd(ptr_fun(test_func2), 10));
  CPPUNIT_ASSERT(array[0] == 21);
  CPPUNIT_ASSERT(array[1] == 22);
  CPPUNIT_ASSERT(array[2] == 23);
#endif
}

class A
{
  public:
    A() :
        m_n( 0 )
      {}

    void f( int n ) const
      { m_n = n; }

    int v() const
      { return m_n; }

  private:
    mutable int m_n;
};

void BindTest::bind_memfn()
{
#if defined (STLPORT) && \
    !defined (_STLP_NO_EXTENSIONS) && defined (_STLP_CLASS_PARTIAL_SPECIALIZATION)
  A array[3];

  for_each( array, array + 3, bind2nd( mem_fun_ref(&A::f), 12 ) );

  CPPUNIT_CHECK( array[0].v() == 12 );
#endif
}
