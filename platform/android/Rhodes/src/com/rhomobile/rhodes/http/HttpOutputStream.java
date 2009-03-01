package com.rhomobile.rhodes.http;

import java.io.BufferedOutputStream;
import java.io.ByteArrayOutputStream;
import java.io.FilterOutputStream;
import java.io.IOException;
import java.io.OutputStream;

import android.util.Log;

public class HttpOutputStream extends OutputStream {

	final static byte[] CRLF = { (byte) '\015', (byte) '\12' };
	final static byte[] EOF = { (byte) '0', (byte) ';', (byte) '\015',
			(byte) '\012', (byte) '\015', (byte) '\012' };

	class SwitchOutputStream extends FilterOutputStream {
		SwitchOutputStream(OutputStream out) {
			super(out);
		}

		void switchStream(OutputStream out) {
			this.out = out;
		}

		OutputStream getOutputStream() {
			return this.out;
		}

		public void write(byte[] b) throws IOException {
			this.out.write(b, 0, b.length);
		}

		public void write(byte buf[], int off, int len) throws IOException {
			this.out.write(buf, off, len);
		}
	}

	OutputStream out;
	OutputStream realOut;
	ByteArrayOutputStream chunk = new ByteArrayOutputStream(4096);
	SwitchOutputStream switchOut = null;

	private HttpResponse response;
	private boolean writtenHeaders = false;

	public HttpOutputStream(OutputStream out, HttpResponse response) {
		this.realOut = new BufferedOutputStream(out);
		this.switchOut = new SwitchOutputStream(realOut);
		this.out = switchOut;
		this.response = response;
		writtenHeaders = (response == null);
	}

	public OutputStream getOutputStream() {
		return out;
	}

	public OutputStream replaceOutputStream(OutputStream newOut) {
		OutputStream oldOut = out;
		out = newOut;
		return oldOut;
	}

	/**
	 * Switch chunking on an off
	 * 
	 * @param on
	 */
	public void setChunking(boolean on) throws IOException {
		flush();
		switchOut.switchStream(on ? chunk : realOut);
	}

	public void write(int b) throws IOException {
		if (!writtenHeaders) {
			writtenHeaders = true;
			response.writeHeaders();
		}
		out.write(b);
		if (chunk.size() > 4000)
			flush();
	}

	public void write(byte b[]) throws IOException {
		if (!writtenHeaders) {
			writtenHeaders = true;
			response.writeHeaders();
		}
		out.write(b);
		if (chunk.size() > 4000)
			flush();
	}

	public void write(byte b[], int off, int len) throws IOException {
		if (!writtenHeaders) {
			writtenHeaders = true;
			response.writeHeaders();
		}
		out.write(b, off, len);
		if (chunk.size() > 4000)
			flush();
	}

	public void flush() throws IOException {
		if (!writtenHeaders) {
			writtenHeaders = true;
			response.writeHeaders();
		}
		out.flush();
		if (chunk.size() > 0) {
			String size = Integer.toString(chunk.size(), 16);
			byte[] b = size.getBytes();
			realOut.write(b);
			realOut.write(';');
			realOut.write(CRLF);
			chunk.writeTo(realOut);
			chunk.reset();
			realOut.write(CRLF);
		}
		realOut.flush();
	}

	public void close() throws IOException {
		try {
			flush();

			// close filters
			out.close();

			// If chunking
			if (switchOut.getOutputStream() == chunk) {
				// send last chunk and revert to normal output
				realOut.write(EOF);
				realOut.flush();
				switchOut.switchStream(realOut);
			}
		} catch (IOException e) {
			Log.e(this.getClass().getSimpleName(), e.getMessage());
		}
	}

	public void print(String string) throws IOException {
		write(string.getBytes());
	}

	public void print(int i) throws IOException {
		write(Integer.toString(i).getBytes());
	}

	public void print(long i) throws IOException {
		write(Long.toString(i).getBytes());
	}

	public void println(String string) throws IOException {
		write(string.getBytes());
		write(CRLF);
	}

	public void println(int i) throws IOException {
		write(Integer.toString(i).getBytes());
		write(CRLF);
	}

	public void println(long i) throws IOException {
		write(Long.toString(i).getBytes());
		write(CRLF);
	}

	public void println() throws IOException {
		write(CRLF);
	}
}
