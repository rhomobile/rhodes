package rhomobile.mapview;

import java.io.IOException;
import java.io.InputStream;
import java.util.Vector;

import org.json.me.RhoJSONArray;
import org.json.me.RhoJSONException;
import org.json.me.RhoJSONObject;

import com.rho.RhoClassFactory;
import com.rho.RhoEmptyLogger;
import com.rho.RhoLogger;
import com.rho.net.IHttpConnection;
import com.rho.net.URI;

public class GoogleGeoCoding extends Thread implements GeoCoding {
	
	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("GoogleGeoCoding");
	
	private static final int BLOCK_SIZE = 4096;
	
	private static final String MAPKEY = "ABQIAAAA-X8Mm7F-7Nmz820lFEBHYxT2yXp_ZAY8_ufC3CFXhHIE1NvwkxSfNPZbryNEPHF-5PQKi9c7Fbdf-A";
	
	private static class Command {
		public String address;
		public OnGeocodingDone callback;
		
		public Command(String addr, OnGeocodingDone cb) {
			if (addr == null || addr.length() == 0 || cb == null)
				throw new IllegalArgumentException();
			address = addr;
			callback = cb;
		}
	};
	
	private boolean active = true;
	
	private Vector commands = new Vector();
		
	public GoogleGeoCoding() {
		start();
	}
	
	public void stop() {
		active = false;
		interrupt();
	}

	public void resolve(String address, OnGeocodingDone callback) {
		synchronized (commands) {
			commands.addElement(new Command(address, callback));
			commands.notify();
		}
	}
	
	public void run() {
		while (active) {
			Command cmd = null;
			synchronized (commands) {
				if (commands.isEmpty()) {
					try {
						commands.wait();
					} catch (InterruptedException e) {}
					continue;
				}
				cmd = (Command)commands.elementAt(0);
				commands.removeElementAt(0);
			}
			
			if (cmd == null)
				continue;
			
			try {
				processCommand(cmd);
			}
			catch (Exception e) {
				LOG.ERROR("Processing of geo coding command failed", e);
			}finally{
				LOG.INFO("Thread exit.");
			}
		}
	}
	
	private byte[] fetchData(String url) throws IOException {
		IHttpConnection conn = RhoClassFactory.getNetworkAccess().connect(url,false);
		
		conn.setRequestMethod("GET");
		
		InputStream is = conn.openInputStream();
		
		int code = conn.getResponseCode();
		if (code/100 != 2)
			throw new IOException("Google map respond with " + code + " " + conn.getResponseMessage());
		
		int size = conn.getHeaderFieldInt("Content-Length", 0);
		byte[] data = new byte[size];
		if (size == 0)
			size = 1073741824; // 1Gb :)
		
		byte[] buf = new byte[BLOCK_SIZE];
		for (int offset = 0; offset < size;) {
			int n = is.read(buf, 0, BLOCK_SIZE);
			if (n <= 0)
				break;
			if (offset + n > data.length) {
				byte[] newData = new byte[offset + n];
				System.arraycopy(data, 0, newData, 0, data.length);
				data = newData;
			}
			System.arraycopy(buf, 0, data, offset, n);
			offset += n;
		}
		
		return data;
	}
	
	private void processCommand(Command cmd) throws IOException, RhoJSONException {
		StringBuffer url = new StringBuffer();
		url.append("http://maps.google.com/maps/geo?");
		url.append("q=" + URI.urlEncode(cmd.address));
		url.append("&output=json&mobile=true&sensor=false");
		url.append("&key=" + MAPKEY);
		
		String finalUrl = url.toString();
		
		byte[] data = fetchData(finalUrl);
		String response = new String(data);
		
		RhoJSONObject resp = new RhoJSONObject(response);
		RhoJSONObject status = resp.getJSONObject("Status");
		int statusCode = status.getInt("code");
		if (statusCode/100 != 2) {
			cmd.callback.onError("geocoding service return error " + statusCode);
			return;
		}
		
		RhoJSONArray placemarks = resp.getJSONArray("Placemark");
		if (placemarks.length() == 0) {
			cmd.callback.onError("geocoding service return empty responce");
			return;
		}
		RhoJSONObject placemark = placemarks.getJSONObject(0);
		RhoJSONObject point = placemark.getJSONObject("Point");
		RhoJSONArray coordinates = point.getJSONArray("coordinates");
		if (coordinates.length() < 2) {
			cmd.callback.onError("geocoding service return less than 2 coordinates");
			return;
		}
		double longitude = coordinates.getDouble(0);
		double latitude = coordinates.getDouble(1);
		
		cmd.callback.onSuccess(latitude, longitude);
	}

}
