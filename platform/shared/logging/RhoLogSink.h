#ifndef _RHOLOGSINK_H_
#define _RHOLOGSINK_H_

#include "RhoLogConf.h"

namespace rho {
namespace common {
    class CRhoFile;
}

class CLogFileSink : public ILogSink{
    common::CRhoFile* m_pFile;
    common::CRhoFile* m_pPosFile;
    const LogSettings& m_oLogConf;
    int                m_nCirclePos;

    unsigned int       m_nFileLogSize;

    const LogSettings& getLogConf()const{ return m_oLogConf; }

public:
    CLogFileSink(const LogSettings& oSettings);

    void writeLogMessage( String& strMsg );
    int getCurPos();
    void clear();
private:
    void loadLogPosition();
    void saveLogPosition();
};

class CLogOutputSink : public ILogSink{
    const LogSettings& m_oLogConf;

public:
    CLogOutputSink(const LogSettings& oSettings) : m_oLogConf(oSettings){}

    void writeLogMessage( String& strMsg );
    int getCurPos(){ return -1; }
    void clear(){}
};

}

#endif //_RHOLOGSINK_H_
