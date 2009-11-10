/***********************************************************************************
  test_rope.cpp

 * Copyright (c) 1997
 * Mark of the Unicorn, Inc.
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Mark of the Unicorn makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.

***********************************************************************************/

# ifdef __SUNPRO_CC
#  define _STLP_NO_MEMBER_TEMPLATE_CLASSES 1
# endif

#include "Prefix.h"
#include "Tests.h"
#include "TestClass.h"
#include "LeakCheck.h"
#include "test_construct.h"
#include "test_assign_op.h"
#include "test_push_back.h"
#include "test_insert.h"
#include "test_push_front.h"

#if defined( EH_ROPE_IMPLEMENTED )
#if !( defined(__MWERKS__) && __MWERKS__ < 0x1900 )    // CW1.8 can't compile this!
# define __STD_STUFF 1
# if defined (EH_NEW_HEADERS)
#include <rope>
#else
#include <rope.h>
#endif


typedef STLPORT::rope<char, eh_allocator(char) > TestRope;

# if ( _STLP_STATIC_TEMPLATE_DATA < 1 )

// Instantiate TestRope static data members
const unsigned long TestRope::_S_min_len[46] = { \
/* 0 */1, /* 1 */2, /* 2 */3, /* 3 */5, /* 4 */8, /* 5 */13, /* 6 */21,         \
/* 7 */34, /* 8 */55, /* 9 */89, /* 10 */144, /* 11 */233, /* 12 */377,         \
/* 13 */610, /* 14 */987, /* 15 */1597, /* 16 */2584, /* 17 */4181,             \
/* 18 */6765ul, /* 19 */10946ul, /* 20 */17711ul, /* 21 */28657ul, /* 22 */46368ul,   \
/* 23 */75025ul, /* 24 */121393ul, /* 25 */196418ul, /* 26 */317811ul,                \
/* 27 */514229ul, /* 28 */832040ul, /* 29 */1346269ul, /* 30 */2178309ul,             \
/* 31 */3524578ul, /* 32 */5702887ul, /* 33 */9227465ul, /* 34 */14930352ul,          \
/* 35 */24157817ul, /* 36 */39088169ul, /* 37 */63245986ul, /* 38 */102334155ul,      \
/* 39 */165580141ul, /* 40 */267914296ul, /* 41 */433494437ul,                        \
/* 42 */701408733ul, /* 43 */1134903170ul, /* 44 */1836311903ul,                      \
/* 45 */2971215073ul };

# endif /* ( _STLP_STATIC_TEMPLATE_DATA < 1 ) */

inline sequence_container_tag
container_category(const TestRope&)
{
  return sequence_container_tag();
}

void test_rope()
{
    TestRope testRope, testRope2;
    size_t ropeSize = random_number(random_base);

    while ( testRope.size() < ropeSize )
    {
        TestRope::value_type x = TestRope::value_type(random_number(random_base));  // initialize before use
        testRope.push_back( x );
        testRope2.push_back( TestRope::value_type() );
    }
    WeakCheck( testRope, test_insert_one<TestRope>(testRope) );
    WeakCheck( testRope, test_insert_one<TestRope>(testRope, 0) );
    WeakCheck( testRope, test_insert_one<TestRope>(testRope, (int)testRope.size()) );

    WeakCheck( testRope, test_insert_n<TestRope>(testRope, random_number(random_base) ) );
    WeakCheck( testRope, test_insert_n<TestRope>(testRope, random_number(random_base), 0 ) );
    WeakCheck( testRope, test_insert_n<TestRope>(testRope, random_number(random_base), (int)testRope.size() ) );

    size_t insCnt = random_number(random_base);
    TestRope::value_type *insFirst = new TestRope::value_type[1+insCnt];

    WeakCheck( testRope, insert_range_tester(testRope, insFirst, insFirst+insCnt) );
    WeakCheck( testRope, insert_range_at_begin_tester(testRope, insFirst, insFirst+insCnt) );
    WeakCheck( testRope, insert_range_at_end_tester(testRope, insFirst, insFirst+insCnt) );

    ConstCheck( 0, test_construct_pointer_range<TestRope>(insFirst, insFirst+insCnt) );
    delete[] insFirst;

    WeakCheck( testRope, insert_range_tester(testRope, testRope2.begin(), testRope2.end() ) );

    WeakCheck( testRope, test_push_front<TestRope>(testRope) );
    WeakCheck( testRope, test_push_back<TestRope>(testRope) );

    ConstCheck( 0, test_default_construct<TestRope>() );

// dwa 1/25/00 - not actually valid for rope, because it doesn't
// have the constructor in question! The code will compile, but with the
// wrong result (the constructor that gets used does something different).

//  ConstCheck( 0, test_construct_n<TestRope>( random_number(random_base) ) );

    ConstCheck( 0, test_construct_n_instance<TestRope>( random_number(random_base) ) );
    ConstCheck( 0, test_construct_iter_range<TestRope>( testRope2 ) );
    ConstCheck( testRope, test_copy_construct<TestRope>() );

    WeakCheck( testRope, test_assign_op<TestRope>( testRope2 ) );
}
#endif // __MWERKS__

#endif // EH_ROPE_IMPLEMENTED
