using System;
using System.Threading;

namespace rho.common
{
    public class Mutex
    {
        private Object m_mutex = new Object();

        public void Lock()
	    {
            Monitor.Enter(m_mutex);
	    }

        public void Unlock()
	    {
            Monitor.Exit(m_mutex);
	    }

        public int Sleep(int nSec)
        {
            Unlock();
            Thread.Sleep(nSec * 1000);
            Lock();
            return nSec;
        }


    }
}
