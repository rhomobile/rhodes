using System;

namespace rho.sync
{
    interface ISyncStatusListener
    {
        void createStatusPopup(String status);
        void reportStatus(String status, int error);
    }
}
