using System;

namespace rho.logging
{
    interface IRhoLogSink
    {
        void writeLogMessage(String strMsg);
        int getCurPos();
        void close();
        void clear();
    }
}
