using System;

namespace rho.db
{
    interface IDBCallback
    {
        void onBeforeDelete(String tableName, IDBResult rows2Delete);
        void onBeforeUpdate(String tableName, IDBResult rows2Insert, int[] cols);
    }
}
