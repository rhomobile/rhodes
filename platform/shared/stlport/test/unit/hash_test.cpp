//Has to be first for StackAllocator swap overload to be taken
//into account (at least using GCC 4.0.1)
#include "stack_allocator.h"

#include <vector>
#include <algorithm>

#if defined (STLPORT) && !defined (_STLP_NO_EXTENSIONS)
#  include <hash_map>
#  include <hash_set>
#    include <rope>
#  endif

#include <string>

#include "cppunit/cppunit_proxy.h"

#if defined (__MVS__)
const char star = 92;
#else
const char star = 42;
#endif

#if !defined (STLPORT) || defined (_STLP_USE_NAMESPACES)
using namespace std;
#endif

//
// TestCase class
//
class HashTest : public CPPUNIT_NS::TestCase
{
  CPPUNIT_TEST_SUITE(HashTest);
#if !defined (STLPORT) || defined (_STLP_NO_EXTENSIONS)
  CPPUNIT_IGNORE;
#endif
  CPPUNIT_TEST(hmap1);
  CPPUNIT_TEST(hmmap1);
  CPPUNIT_TEST(hmset1);
  CPPUNIT_TEST(hset2);
  CPPUNIT_TEST(insert_erase);
  CPPUNIT_TEST(allocator_with_state);
  //CPPUNIT_TEST(equality);
  CPPUNIT_TEST_SUITE_END();

#if defined (STLPORT) && !defined (_STLP_NO_EXTENSIONS)
  typedef hash_multiset<char, hash<char>, equal_to<char> > hmset;
#endif

protected:
  void hmap1();
  void hmmap1();
  void hmset1();
  void hset2();
  void insert_erase();
  //void equality();
  void allocator_with_state();
};

CPPUNIT_TEST_SUITE_REGISTRATION(HashTest);

//
// tests implementation
//
void HashTest::hmap1()
{
#if defined (STLPORT) && !defined (_STLP_NO_EXTENSIONS)
  typedef hash_map<char, crope, hash<char>, equal_to<char> > maptype;
  maptype m;
  // Store mappings between roman numerals and decimals.
  m['l'] = "50";
  m['x'] = "20"; // Deliberate mistake.
  m['v'] = "5";
  m['i'] = "1";
  CPPUNIT_ASSERT( !strcmp(m['x'].c_str(),"20") );
  m['x'] = "10"; // Correct mistake.
  CPPUNIT_ASSERT( !strcmp(m['x'].c_str(),"10") );

  CPPUNIT_ASSERT( !strcmp(m['z'].c_str(),"") );

  CPPUNIT_ASSERT( m.count('z')==1 );
  pair<maptype::iterator, bool> p = m.insert(pair<const char, crope>('c', crope("100")));

  CPPUNIT_ASSERT(p.second);

  p = m.insert(pair<const char, crope>('c', crope("100")));
  CPPUNIT_ASSERT(!p.second);

  //Some iterators compare check, really compile time checks
  maptype::iterator ite(m.begin());
  maptype::const_iterator cite(m.begin());
  cite = m.begin();
  maptype const& cm = m;
  cite = cm.begin();
  CPPUNIT_ASSERT( ite == cite );
  CPPUNIT_ASSERT( !(ite != cite) );
  CPPUNIT_ASSERT( cite == ite );
  CPPUNIT_ASSERT( !(cite != ite) );
#endif
}

void HashTest::hmmap1()
{
#if defined (STLPORT) && !defined (_STLP_NO_EXTENSIONS)
  typedef hash_multimap<char, int, hash<char>,equal_to<char> > mmap;
  mmap m;
  CPPUNIT_ASSERT(m.count('X')==0);
  m.insert(pair<const char,int>('X', 10)); // Standard way.
  CPPUNIT_ASSERT(m.count('X')==1);
//  m.insert('X', 20); // Non-standard, but very convenient!
  m.insert(pair<const char,int>('X', 20));  // jbuck: standard way
  CPPUNIT_ASSERT(m.count('X')==2);
//  m.insert('Y', 32);
  m.insert(pair<const char,int>('Y', 32));  // jbuck: standard way
  mmap::iterator i = m.find('X'); // Find first match.

  CPPUNIT_ASSERT((*i).first=='X');
  CPPUNIT_ASSERT((*i).second==10);
  i++;
  CPPUNIT_ASSERT((*i).first=='X');
  CPPUNIT_ASSERT((*i).second==20);
  i++;
  CPPUNIT_ASSERT((*i).first=='Y');
  CPPUNIT_ASSERT((*i).second==32);
  i++;
  CPPUNIT_ASSERT(i==m.end());

  size_t count = m.erase('X');
  CPPUNIT_ASSERT(count==2);

  //Some iterators compare check, really compile time checks
  mmap::iterator ite(m.begin());
  mmap::const_iterator cite(m.begin());
  CPPUNIT_ASSERT( ite == cite );
  CPPUNIT_ASSERT( !(ite != cite) );
  CPPUNIT_ASSERT( cite == ite );
  CPPUNIT_ASSERT( !(cite != ite) );

  typedef hash_multimap<size_t, size_t> HMapType;
  HMapType hmap;

  //We fill the map to implicitely start a rehash.
  for (size_t counter = 0; counter < 3077; ++counter)
    hmap.insert(HMapType::value_type(1, counter));

  hmap.insert(HMapType::value_type(12325, 1));
  hmap.insert(HMapType::value_type(12325, 2));

  CPPUNIT_ASSERT( hmap.count(12325) == 2 );

  //At this point 23 goes to the same bucket as 12325, it used to reveal a bug.
  hmap.insert(HMapType::value_type(23, 0));

  CPPUNIT_ASSERT( hmap.count(12325) == 2 );
#endif
}

void HashTest::hmset1()
{
#if defined (STLPORT) && !defined (_STLP_NO_EXTENSIONS)
  hmset s;
  CPPUNIT_ASSERT( s.count(star) == 0 );
  s.insert(star);
  CPPUNIT_ASSERT( s.count(star) == 1 );
  s.insert(star);
  CPPUNIT_ASSERT( s.count(star) == 2 );
  hmset::iterator i = s.find(char(40));
  CPPUNIT_ASSERT( i == s.end() );

  i = s.find(star);
  CPPUNIT_ASSERT( i != s.end() )
  CPPUNIT_ASSERT( *i == '*' );
  CPPUNIT_ASSERT( s.erase(star) == 2 );
#endif
}
void HashTest::hset2()
{
#if defined (STLPORT) && !defined (_STLP_NO_EXTENSIONS)
  hash_set<int, hash<int>, equal_to<int> > s;
  pair<hash_set<int, hash<int>, equal_to<int> >::iterator, bool> p = s.insert(42);
  CPPUNIT_ASSERT( p.second );
  CPPUNIT_ASSERT( *(p.first) == 42 );

  p = s.insert(42);
  CPPUNIT_ASSERT( !p.second );
#endif
}

void HashTest::insert_erase()
{
#if defined (STLPORT) && !defined (_STLP_NO_EXTENSIONS)
  typedef hash_map<string, size_t, hash<string>, equal_to<string> > hmap;
  typedef hmap::value_type val_type;
  {
    hmap values;
    CPPUNIT_ASSERT( values.insert(val_type("foo", 0)).second );
    CPPUNIT_ASSERT( values.insert(val_type("bar", 0)).second );
    CPPUNIT_ASSERT( values.insert(val_type("abc", 0)).second );

    CPPUNIT_ASSERT( values.erase("foo") == 1 );
    CPPUNIT_ASSERT( values.erase("bar") == 1 );
    CPPUNIT_ASSERT( values.erase("abc") == 1 );
  }

  {
    hmap values;
    CPPUNIT_ASSERT( values.insert(val_type("foo", 0)).second );
    CPPUNIT_ASSERT( values.insert(val_type("bar", 0)).second );
    CPPUNIT_ASSERT( values.insert(val_type("abc", 0)).second );

    CPPUNIT_ASSERT( values.erase("abc") == 1 );
    CPPUNIT_ASSERT( values.erase("bar") == 1 );
    CPPUNIT_ASSERT( values.erase("foo") == 1 );
  }
#endif
}

/*
 * Here is the test showing why equality operator on hash containers
 * has no meaning:

struct equality_hash_func {
  size_t operator () (size_t val) const {
    return val % 10;
  }
};

void HashTest::equality()
{
  hash_set<size_t, equality_hash_func, equal_to<size_t> > s1, s2;

  s1.insert(10);
  s1.insert(20);

  s2.insert(20);
  s2.insert(10);

  //s1 and s2 contains both 10 and 20:
  CPPUNIT_ASSERT( s1 == s2 );
}
*/

void HashTest::allocator_with_state()
{
#if defined (STLPORT) && !defined (_STLP_NO_EXTENSIONS)
  char buf1[2048];
  StackAllocator<int> stack1(buf1, buf1 + sizeof(buf1));

  char buf2[2048];
  StackAllocator<int> stack2(buf2, buf2 + sizeof(buf2));

  {
    typedef hash_set<int, hash<int>, equal_to<int>, StackAllocator<int> > HashSetInt;
    HashSetInt hint1(10, hash<int>(), equal_to<int>(), stack1);

    int i;
    for (i = 0; i < 5; ++i)
      hint1.insert(i);
    HashSetInt hint1Cpy(hint1);

    HashSetInt hint2(10, hash<int>(), equal_to<int>(), stack2);
    for (; i < 10; ++i)
      hint2.insert(i);
    HashSetInt hint2Cpy(hint2);

    hint1.swap(hint2);

    CPPUNIT_ASSERT( hint1.get_allocator().swaped() );
    CPPUNIT_ASSERT( hint2.get_allocator().swaped() );

    CPPUNIT_ASSERT( hint1.get_allocator() == stack2 );
    CPPUNIT_ASSERT( hint2.get_allocator() == stack1 );
  }
  CPPUNIT_ASSERT( stack1.ok() );
  CPPUNIT_ASSERT( stack2.ok() );
#endif
}
