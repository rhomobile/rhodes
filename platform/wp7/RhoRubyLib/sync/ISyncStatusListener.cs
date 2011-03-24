using System;

namespace rho.sync
{
    public interface ISyncStatusListener
    {
        void createStatusPopup(String status);
        void reportStatus(String status, int error);
    }
}
