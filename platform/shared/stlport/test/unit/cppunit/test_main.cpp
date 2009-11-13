/*
 * Copyright (c) 2003, 2004
 * Zdenek Nemec
 *
 * This material is provided "as is", with absolutely no warranty expressed
 * or implied. Any use is at your own risk.
 *
 * Permission to use or copy this software for any purpose is hereby granted
 * without fee, provided the above notices are retained on all copies.
 * Permission to modify the code and to distribute modified code is granted,
 * provided the above notices are retained, and a notice that the code was
 * modified is included with the above copyright notice.
 *
 */

#include "cppunit_proxy.h"
#include "file_reporter.h"
#include "cppunit_timer.h"

namespace CPPUNIT_NS
{
  int CPPUNIT_NS::TestCase::m_numErrors = 0;
  int CPPUNIT_NS::TestCase::m_numTests = 0;

  CPPUNIT_NS::TestCase *CPPUNIT_NS::TestCase::m_root = 0;
  CPPUNIT_NS::Reporter *CPPUNIT_NS::TestCase::m_reporter = 0;

  void CPPUNIT_NS::TestCase::registerTestCase(TestCase *in_testCase) {
    in_testCase->m_next = m_root;
    m_root = in_testCase;
  }

  int CPPUNIT_NS::TestCase::run(Reporter *in_reporter, const char *in_testName, bool invert) {
    TestCase::m_reporter = in_reporter;

    m_numErrors = 0;
    m_numTests = 0;

    TestCase *tmp = m_root;
    while (tmp != 0) {
      tmp->myRun(in_testName, invert);
      tmp = tmp->m_next;
    }
    return m_numErrors;
  }
}

int main(int argc, char** argv) {

  // CppUnit(mini) test launcher
  // command line option syntax:
  // test [OPTIONS]
  // where OPTIONS are
  //  -t=CLASS[::TEST]    run the test class CLASS or member test CLASS::TEST
  //  -x=CLASS[::TEST]    run all except the test class CLASS or member test CLASS::TEST
  //  -f=FILE             save output in file FILE instead of stdout
  //  -m                  monitor test(s) execution
  char *fileName = 0;
  char *testName = "";
  char *xtestName = "";
  bool doMonitoring = false;

  for (int i = 1; i < argc; ++i) {
    if (argv[i][0] != '-')
      break;
    if (!strncmp(argv[i], "-t=", 3)) {
      testName = argv[i]+3;
    }
    else if (!strncmp(argv[i], "-f=", 3)) {
      fileName = argv[i]+3;
    }
    else if (!strncmp(argv[i], "-x=", 3)) {
      xtestName = argv[i]+3;
    }
    else if (Timer::supported() && !strncmp(argv[i], "-m", 2)) {
      doMonitoring = true;
    }
  }

  CPPUNIT_NS::Reporter* reporter;
  if (fileName != 0)
    reporter = new FileReporter(fileName, doMonitoring);
  else
    reporter = new FileReporter(stdout, doMonitoring);

  int num_errors;
  if (xtestName[0] != 0) {
    num_errors = CPPUNIT_NS::TestCase::run(reporter, xtestName, true);
  } else {
    num_errors = CPPUNIT_NS::TestCase::run(reporter, testName);
  }

  reporter->printSummary();
  delete reporter;

  return num_errors;
}

// See doc/README.intel for explanation about this code
#if defined (STLPORT) && defined (__ICL) && (__ICL >= 900) && \
            (_STLP_MSVC_LIB < 1300) && defined (_STLP_USE_DYNAMIC_LIB)
#  include <exception>

#  undef std
namespace std
{
  void _STLP_CALL unexpected() {
    unexpected_handler hdl;
    set_unexpected(hdl = set_unexpected((unexpected_handler)0));
    hdl();
  }
}
#endif
