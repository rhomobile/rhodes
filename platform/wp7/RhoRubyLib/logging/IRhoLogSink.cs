using System;

namespace rho.logging
{
    public interface IRhoLogSink
    {
        void writeLogMessage(String strMsg);
        int getCurPos();
        void close();
        void clear();
    }
}
