package j2me.util.concurrent.locks;

import com.rho.Mutex;

public class ReentrantLock  
{
    Mutex m_mutex = new Mutex();
    
    public ReentrantLock(){}
    
    public void lock() 
    {
    	m_mutex.Lock();
    }
    public void unlock() 
    {
    	m_mutex.Unlock();
    }
    public boolean isHeldByCurrentThread() 
    {
    	return m_mutex.isHeldByCurrentThread();
    }
};
