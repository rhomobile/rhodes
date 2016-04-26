using System;
using System.Linq;
using System.Collections.Generic;
using System.Text;
using System.Threading;
using System.Reflection;

using PrintingService;

namespace rho
{
    public abstract class PrintingThread
    {
        protected Thread   m_thread   = null;
        protected MainForm m_applicationForm = null;
        private   bool     m_stopThread = false;

        public PrintingThread(MainForm applicationForm)
        {
            m_thread          = new Thread(runThread);
            m_applicationForm = applicationForm;
        }

        public void startThread()
        {
            m_thread.Start();
        }

        public void runThread()
        {
            while (!m_stopThread)
            {
                checkForMessage();
            }
        }

        public void stopThread()
        {
            finalizeThread();

            m_stopThread = true;
        }

        public void joinThread()
        {
            m_thread.Join();
        }

        protected abstract void finalizeThread();

        protected abstract void checkForMessage();
    }
}
