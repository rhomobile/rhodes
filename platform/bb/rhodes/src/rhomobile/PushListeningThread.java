package rhomobile;

import java.io.*;
import java.util.Vector;

import javax.microedition.io.*;

import com.rho.RhoEmptyLogger;
import com.rho.RhoLogger;
import com.rho.RhodesApp;
import com.rho.Tokenizer;
import com.rho.sync.SyncThread;
import com.rho.RhoConf;

import net.rim.device.api.util.*;
import net.rim.device.api.io.http.*;
import com.rho.FilePath;

public class PushListeningThread extends Thread {
	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("PushListeningThread");
	
	// Constants ----------------------------------------------------------------
    private static final String URL = "http://:"; // PORT 100.
    private static final int CHUNK_SIZE = 256;
    
    //
    private boolean _stop = false;
    private StreamConnectionNotifier _notify;

    protected synchronized void stop()
    {
        _stop = true;
        try 
        {
            // Close the connection so the thread will return.
            _notify.close(); 
        } 
        catch (IOException e) 
        {
            System.err.println(e.toString());
        } 
        catch (NullPointerException e) 
        {
            // The notify object likely failed to open, due to an IOException.
        }
    }

    public void run()
    {

        StreamConnection stream = null;
        InputStream input = null;
        MDSPushInputStream pushInputStream=null;
        
        //{
        //	String test = "do_sync=AeropriseSrd,AeropriseRequest,AeropriseWorklog,AeropriseCategory,AeropriseUser\nalert=push message\nvibrate=2\nsound=welcome.mp3";
        //	processPushMessage(test.getBytes(), test.length());
        //}
        
        while (!_stop)
        {
        try 
            {
                // Synchronize here so that we don't end up creating a connection that is never closed.
                synchronized(this)  
                {
                    // Open the connection once (or re-open after an IOException),  so we don't end up 
                    // in a race condition, where a push is lost if it comes in before the connection 
                    // is open again. We open the url with a parameter that indicates that we should 
                    // always use MDS when attempting to connect.
                	int port = RhoConf.getInstance().getInt("push_port");
                	if (port == 0) port = 100;
                    _notify = (StreamConnectionNotifier)Connector.open(URL + port + ";deviceside=false");
                }
                
                while (!_stop)
                {
                    
                    // NOTE: the following will block until data is received.
                	LOG.TRACE("Block push thread until data is recieved");
                    stream = _notify.acceptAndOpen();
                	LOG.TRACE("Recieved push data");
                    
                    try 
                    {
                        input = stream.openInputStream();
                        pushInputStream = new MDSPushInputStream((HttpServerConnection)stream, input);

                        // Extract the data from the input stream.
                     
                        DataBuffer db = new DataBuffer();
                        byte[] data = new byte[CHUNK_SIZE];
                        int chunk = 0;
                        
                        while ( -1 != (chunk = input.read(data)) )
                        {
                            db.write(data, 0, chunk);
                        }
                        
                        try{
                        	processPushMessage(data, db.getLength());
                        }catch(Exception exc)
                        {
                        	LOG.ERROR("processPushMessage failed.Data: " + new String(data), exc);
                        }catch(Throwable th)
                        {
                        	LOG.ERROR("processPushMessage crashed.Data: " + new String(data), th);
                        }
                        
                        // This method is called to accept the push.
                        pushInputStream.accept();

                        input.close();
                        stream.close();

                        data = db.getArray();
                        

                    } 
                    catch (IOException e1) 
                    {
                        // A problem occurred with the input stream , however, the original 
                        // StreamConnectionNotifier is still valid.
                    	LOG.ERROR("A problem occurred with the input stream", e1 );

                        if ( input != null ) 
                        {
                            try 
                            {
                                input.close();
                            } 
                            catch (IOException e2) 
                            {
                            }
                        }
                        
                        if ( stream != null )
                        {
                            try 
                            {
                                stream.close();
                            } 
                            catch (IOException e2) 
                            {
                            }
                        }
                    }
                }
                
                _notify.close();
                _notify = null;   
                
            } 
            catch (IOException ioe)
            {
            	if ( !_stop )
            	{
            		LOG.ERROR("Exception thrown by _notify.acceptAndOpen() - exiting push thread", ioe);
	            	
	            	// Likely the stream was closed. Catches the exception thrown by 
	                // _notify.acceptAndOpen() when this program exits.
	            	
	            	_stop = true;
	                if ( _notify != null ) 
	                {
	                    try 
	                    {
	                        _notify.close();
	                    } 
	                    catch ( IOException e ) 
	                    {
	                    }
	                }
            	}
            	
                _notify = null;
            }
        }
    }
    
    private String[] split(String original, String separator) {
		Vector nodes = new Vector();
		// Parse nodes into vector
		int index = original.indexOf(separator);
		while (index >= 0) {
			nodes.addElement(original.substring(0, index));
			original = original.substring(index + separator.length());
			index = original.indexOf(separator);
		}
		// Get the last node
		nodes.addElement(original);

		// Create splitted string array
		String[] result = new String[nodes.size()];
		if (nodes.size() > 0) {
			for (int loop = 0; loop < nodes.size(); loop++) {
				result[loop] = ((String)nodes.elementAt(loop)).trim();
			}
		}

		return result;
	}
   
    private String[] splitOnce(String original, String separator) {
		String[] result;
    	int index = original.indexOf(separator);
    	if (index>=0) {
    		result = new String[2];
    		result[0] = original.substring(0, index).trim();
    		result[1] = original.substring(index + separator.length()).trim();
    	} else {
    		result = new String[1];
    		result[0] = original.trim();
    	}
    	return result;
    }
    
    private void showPopup(String message) {
    	com.rho.rubyext.Alert.showPopup(message);
	}
    
    private void vibrate(String duration) {
    	com.rho.rubyext.Alert.vibrate(duration);
    }

    private void play_file(String file_name, String media_type) {
    	com.rho.rubyext.Alert.play_file(file_name, media_type);
    }
   
    private void processPushMessage(final byte[] data, int nLen)
    {
       /* Application.getApplication().invokeLater(new Runnable() 
        {
            public void run() 
            {*/
            	String msg = new String(data, 0, nLen);
            	LOG.INFO("Triger sync on PUSH message [" + msg + " ]");

            	String[] op;
            	String[] ops = split(msg,"\n");
            	
            	if ( RhodesApp.getInstance() != null )
            	{
            		try
            		{
            			String strMsg = "";
                    	for (int loop = 0; loop < ops.length; loop++) 
                    	{
                    		if ( ops[loop] == null || ops[loop].length() == 0 )
                    			continue;
                    		
                    		if ( strMsg .length() > 0 )
                    			strMsg += "&";
                    		strMsg += ops[loop];
                    	}
                    	
	            		if ( RhodesApp.getInstance().callPushCallback(strMsg) )
	            			return;
            		}catch(Exception exc)
            		{
            			LOG.ERROR("callPushCallback failed", exc);
            			return;
            		}
            	}
            	
            	for (int loop = 0; loop < ops.length; loop++) {
            		if (ops[loop].startsWith("do_sync")) {
            			op = splitOnce(ops[loop],"=");
            			if(op.length <= 1 || "all".equalsIgnoreCase(op[1])) {
            				SyncThread.doSyncAllSources(false);
            			} else if ((op[1] != null) && (op[1].length()>0))
            			{
            				Tokenizer stringtokenizer = new Tokenizer(op[1].trim(), ",");
            				while (stringtokenizer.hasMoreTokens()) 
            				{
            					String tok = stringtokenizer.nextToken();
            					String name = tok.trim();
            					if (name.length() == 0) 
            						continue;
            					
                				SyncThread.doSyncSourceByName(name,false);
            				}
            				
            			}
            		} else if (ops[loop].startsWith("alert")) {
            			op = splitOnce(ops[loop],"=");
            			if (op.length>1) {
            				showPopup(op[1]);
            			}
            		} else if (ops[loop].startsWith("vibrate")) {
            			op = splitOnce(ops[loop],"=");
            			if (op.length>1) {
            				vibrate(op[1]);
            			} else {
            				vibrate("2500");
            			}
            		} else if (ops[loop].startsWith("sound")) {
            			op = splitOnce(ops[loop],"=");
            			if (op.length>1) {
            				op = splitOnce(op[1],",");
            				String fileName = FilePath.join("/public/alerts/", op[0]);
            				if (op.length>1) {
            					play_file(fileName,op[1]);
            				} else {
            					play_file(fileName,null);
            				}
            			}            			
            		}
            	}            	
            /*}
        });*/
    }

}
