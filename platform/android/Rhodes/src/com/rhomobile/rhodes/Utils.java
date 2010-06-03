package com.rhomobile.rhodes;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import android.content.res.AssetManager;

public class Utils {
	
	public static class FileSource {
		
		public FileSource() {}
		
		String[] list(String dir) throws IOException {
			return new File(dir).list();
		}
		
		InputStream open(String file) throws FileNotFoundException, IOException {
			return new FileInputStream(file);
		}
	};
	
	public static class AssetsSource extends FileSource {

		private AssetManager amgr;
		
		public AssetsSource(AssetManager a) {
			amgr = a;
		}
		
		String[] list(String dir) throws IOException {
			return amgr.list(dir);
		};
		
		InputStream open(String file) throws IOException {
			return amgr.open(file);
		}
		
	}; 
	
	public static String getContent(InputStream in) throws IOException {
		String retval = "";
		byte[] buf = new byte[512];
		while(true) {
			int n = in.read(buf);
			if (n <= 0)
				break;
			retval += new String(buf);
		}
		return retval;
	}
	
	public static boolean isContentsEquals(FileSource source1, String file1, FileSource source2, String file2) throws IOException {
		InputStream stream1 = null;
		InputStream stream2 = null;
		try {
			stream1 = source1.open(file1);
			stream2 = source2.open(file2);
			
			String newName = Utils.getContent(stream1);
			String oldName = Utils.getContent(stream2);
			return newName.equals(oldName);
		} catch (Exception e) {
			return false;
		}
		finally {
			if (stream1 != null) stream1.close();
			if (stream2 != null) stream2.close();
		}
	}
	
	public static boolean deleteRecursively(File target) {
		if (target.isDirectory()) {
			String[] children = target.list();
			for(int i = 0; i != children.length; ++i)
				if (!deleteRecursively(new File(target, children[i])))
					return false;
		}
		return target.delete();
	}
	
	public static void copyRecursively(FileSource fs, String source, File target, boolean remove) throws IOException
	{
		if (remove && target.exists() && !deleteRecursively(target))
			throw new IOException("Can not delete " + target.getAbsolutePath());
		
		String[] children = fs.list(source);
		if (children != null && children.length > 0) {
			if (!target.exists())
				target.mkdirs();
			
			for(int i = 0; i != children.length; ++i)
				copyRecursively(fs, source + "/" + children[i], new File(target, children[i]), false);
		}
		else {
			InputStream in = null;
			OutputStream out = null;
			try {
				in = fs.open(source);
				target.getParentFile().mkdirs();
				out = new FileOutputStream(target);
				
				byte[] buf = new byte[1024];
				int len;
				while((len = in.read(buf)) > 0)
					out.write(buf, 0, len);
				
			}
			catch (FileNotFoundException e) {
				if (in != null)
					throw e;
				
				target.createNewFile();
			}
			finally {
				if (in != null)
					in.close();
				if (out != null)
					out.close();
			}
		}
	}
	
	public static void copy(String src, String dst) throws IOException {
		InputStream is = null;
		OutputStream os = null;
		try {
			is = new FileInputStream(src);
			os = new FileOutputStream(dst);
			
			byte[] buf = new byte[1024];
			for(;;) {
				int n = is.read(buf);
				if (n <= 0)
					break;
				os.write(buf, 0, n);
			}
			os.flush();
		}
		finally {
			if (is != null)
				is.close();
			if (os != null)
				os.close();
		}
	}
	
	public static String getDirName(String filePath) {
		if (filePath == null)
			return null;
		return new File(filePath).getParent();
	}
	
	public static String getBaseName(String filePath) {
		if (filePath == null)
			return null;
		return new File(filePath).getName();
	}

}
