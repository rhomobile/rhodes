package com.rho;

public class Mutex
{
	private Object m_mutex = new Object();
	private int m_givenLocks = 0;
	private Thread m_lockThread = null;
	
	public void Lock()
	{
		synchronized (m_mutex) {
			if ( m_lockThread == null || !m_lockThread.equals(Thread.currentThread()))
			{
				try {
					while (m_givenLocks > 0) {
						m_mutex.wait();
					}
				} catch (java.lang.InterruptedException e) {
					System.out.println(e);
				}
				
				m_lockThread = Thread.currentThread();
			}
			m_givenLocks ++;
		}
	}
	
	public void Unlock()
	{
		synchronized (m_mutex) {

			if (m_givenLocks <= 0)
				return;

			m_givenLocks--;
			if ( m_givenLocks == 0 )
				m_lockThread = null;
			
			m_mutex.notifyAll();
		}
	}
};
