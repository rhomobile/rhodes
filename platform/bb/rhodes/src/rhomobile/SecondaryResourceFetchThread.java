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

package rhomobile;

import java.util.Vector;

import javax.microedition.io.HttpConnection;

import com.rho.RhoEmptyLogger;
import com.rho.RhoLogger;

import net.rim.device.api.browser.field.BrowserContent;
import net.rim.device.api.browser.field.RequestedResource;


/**
 * 
 */
public class SecondaryResourceFetchThread extends Thread {

	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("RhodesApplication");
	
    /**
     * Callback browser field
     */
    private BrowserContent    _browserField;
    
    /**
     * Images to retrieve
     */
    private Vector            _imageQueue;
    
    /**
     * True is all images have been enqueued
     */
    private boolean           _done;
    
    /**
     * Sync object
     */
    private static Object     _syncObject = new Object();
    
    /**
     * Secondary thread
     */
    private static SecondaryResourceFetchThread _currentThread;
    
    /**
     * Enqueues secondary resource for a browser field.
     * 
     * @param resource - resource to retrieve
     * @param referrer - call back browsr field
     */
    public static void enqueue(RequestedResource resource, BrowserContent referrer) {
        
        if (resource == null) {
            return;
        }
        
        synchronized( _syncObject ) {
            
            // create new thread
            if (_currentThread == null) {
                _currentThread = new SecondaryResourceFetchThread();
                _currentThread.start();
            } else {
                // if thread alread is running, check that we are adding images for the same browser field
                if (referrer != _currentThread._browserField) {  
                    synchronized( _currentThread._imageQueue) {
                        // if the request is for a different browser field,
                        // clear old elements
                        _currentThread._imageQueue.removeAllElements();
                    }
                }
            }   
            
            synchronized( _currentThread._imageQueue) {
                _currentThread._imageQueue.addElement(resource);
            }
            
            _currentThread._browserField = referrer;
        }
    }
    
    /**
     * Constructor
     *
     */
    private SecondaryResourceFetchThread() {
        _imageQueue = new Vector();        
    }
    
    /**
     * Indicate that all images have been enqueued for this browser field
     */
    public static void doneAddingImages() {
        synchronized( _syncObject ) {
            if (_currentThread != null) {
                _currentThread._done = true;
            }
        }
    }
    
    public void run() {
        
        while (true) {
            
            if (_done) {
                
                // check if we are done requesting images
                synchronized( _syncObject ) {
                    synchronized( _imageQueue ) {
                        if (_imageQueue.size() == 0) {
                            _currentThread = null;   
                            break;
                        }
                    }
                        
                }
            }
            
            RequestedResource resource = null;
                              
            // request next image
            synchronized( _imageQueue ) {
                if (_imageQueue.size() > 0) {
                    resource = (RequestedResource)_imageQueue.elementAt(0);
                    _imageQueue.removeElementAt(0);
                }
            }
            
            if (resource != null) {
                
            	try{
	                HttpConnection connection = Utilities.makeConnection(resource.getUrl(), resource.getRequestHeaders(), null, null);
	                resource.setHttpConnection(connection);
	                
	                // signal to the browser field that resource is ready
	                if (_browserField != null) {
	                	//synchronized (RhodesApplication.getEventLock())
	                	{
	                		_browserField.resourceReady(resource);
	                	}
	                	
	                	if (_imageQueue.size() == 0)
	                	{
		                	synchronized (RhodesApplication.getEventLock())
		                	{
		                		RhodesApplication.getInstance().invalidateMainScreen();
		                	}
	                		
	                	}
	                }
            	}catch(Exception exc)
            	{
            		LOG.ERROR("SecondaryResourceFetchThread failed.", exc);
            	}
            }
            
        }       
    }   
    
}
