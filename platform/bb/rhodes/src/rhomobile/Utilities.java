package rhomobile;

import java.io.IOException;
import java.io.OutputStream;

//import javax.microedition.io.Connector;
import javax.microedition.io.HttpConnection;

import net.rim.device.api.io.http.HttpHeaders;
import net.rim.device.api.io.http.HttpProtocolConstants;
import net.rim.device.api.util.StringUtilities;

import com.rho.RhoClassFactory;
import com.rho.RhodesApp;
import com.rho.net.URI;
import com.rho.net.IHttpConnection;
import com.rho.net.bb.NativeBBHttpConnection;
import com.rho.net.RhoConnection;
/**
 * 
 */
public class Utilities {

	public static HttpConnection makeConnection(String url,
			HttpHeaders requestHeaders, byte[] postData, HttpConnection oldConn)throws IOException {

		HttpConnection conn = oldConn;
		OutputStream out = null;

		try {
			
			if ( conn == null )
			{
				if ( RhodesApp.getInstance().isRhodesAppUrl(url) )
				{
					URI uri = new URI(url);
					conn = new NativeBBHttpConnection( new RhoConnection(uri) );
				} else {
					// conn = (HttpConnection) Connector.open(url);
					IHttpConnection httpconn = RhoClassFactory.getNetworkAccess().connect(url,true);
					conn = (HttpConnection)(httpconn!=null?httpconn.getNativeConnection() : null);
					// conn = NetworkAccess.connect(url);
				}
			}
				
			if (requestHeaders != null) {
				// From
				// http://www.w3.org/Protocols/rfc2616/rfc2616-sec15.html#sec15.1.3
				//
				// Clients SHOULD NOT include a Referer header
				// field in a (non-secure) HTTP request if the
				// referring page was transferred with a secure
				// protocol.
				String referer = requestHeaders.getPropertyValue("referer");
				boolean sendReferrer = false;
				if (referer != null
						&& StringUtilities.startsWithIgnoreCase(referer,
								"https:")
						&& !StringUtilities.startsWithIgnoreCase(url, "https:")) {
					sendReferrer = false;
				}
				
				// Set cookie
				String cookie = RhodesApplication.getInstance().getCookie(url);
				if (cookie != null) {
					conn.setRequestProperty("Cookie", cookie);
				}

				int size = requestHeaders.size();
				for (int i = 0; i < size;) {
					String header = requestHeaders.getPropertyKey(i);

					// remove referer header if needed
					if (!sendReferrer && header.equals("referer")) {
						requestHeaders.removeProperty(i);
						--size;
						continue;
					}

					String value = requestHeaders.getPropertyValue(i++);
					if (value != null) {
						conn.setRequestProperty(header, value);
					}
				}
			}

			if (postData == null) {
				conn.setRequestMethod(IHttpConnection.GET);
			} else {
				conn.setRequestMethod(IHttpConnection.POST);

				conn.setRequestProperty(
						HttpProtocolConstants.HEADER_CONTENT_LENGTH, String
								.valueOf(postData.length));

				out = conn.openOutputStream();
				out.write(postData);

			}

		} catch (IOException e) {
			if ( conn != null )
				try{ conn.close(); }catch(Exception e1){}
			throw e;
		} finally {
			if (out != null) {
				try {
					out.close();
				} catch (IOException e2) {
				}
			}
		}

		return conn;
	}
	
	public static String replaceAll(String content, String that, String withthat) { 
		int from = 0; 
		StringBuffer sb = new StringBuffer(); 
		int index = -1; 
		
		while(true) { 
			index = content.indexOf(that,from); 
			if(index!=-1) { 
				sb = new StringBuffer(); 
				String upto = content.substring(0,index); 
				sb.append(upto+withthat); 
				String lastbit = content.substring(index+that.length(),content.length()); 
				sb.append(lastbit); 
				content = sb.toString(); 
				from = index+that.length(); 
			} else { 
				break; 
			} 
		} 
		return content; 
	} 	
}
