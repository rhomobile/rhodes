/*------------------------------------------------------------------------
* (The MIT License)
*
* Copyright (c) 2008-2011 Rhomobile, Inc.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*
* http://rhomobile.com
*------------------------------------------------------------------------*/

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

	public boolean isLocked()
	{
		synchronized (m_mutex) {
			return m_lockThread != null && !m_lockThread.equals(Thread.currentThread());
		}
	}

	public boolean isHeldByCurrentThread()
	{
		synchronized (m_mutex) {
			return m_lockThread != null && m_lockThread.equals(Thread.currentThread());
		}
	}

	public boolean tryLock()
	{
		synchronized (m_mutex) {

			if ( isLocked() )
				return false;

			Lock();
			return true;
		}
	}

	public int Sleep(int nSec)
	{
		Unlock();
		try{
			Thread.sleep(nSec*1000);
		}catch(InterruptedException exc){}
		Lock();
		return nSec;
	}

};
