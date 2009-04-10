#ifndef _RHOLOGSINK_H_
#define _RHOLOGSINK_H_

#include "RhoLogConf.h"

namespace rho {
namespace general {
    class CRhoFile;
}

class CLogFileSink : public ILogSink{
    general::CRhoFile* m_pFile;
    general::CRhoFile* m_pPosFile;
    int                m_nCirclePos;

    unsigned int       m_nFileLogSize;
    const LogSettings& m_oLogConf;

    const LogSettings& getLogConf()const{ return m_oLogConf; }

public:
    CLogFileSink(const LogSettings& oSettings);

    void writeLogMessage( String& strMsg );
    int getCurPos();

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
};

}

#endif //_RHOLOGSINK_H_
