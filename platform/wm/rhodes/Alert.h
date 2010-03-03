#ifndef __ALERT__H__
#define __ALERT__H__

#include <common/RhoStd.h>
#include <logging/RhoLog.h>

using namespace rho;
using namespace rho::common;

class CAlert {
    DEFINE_LOGCLASS;
  public:
    static void showPopup(String message);
    static void vibrate();
    static void playFile(String fileName);
};

#endif // __ALERT__H__
