package com.rho.file;

import com.rho.IFileAccess;
import com.rho.db.FileUtilBB;

public class FileAccessBB implements IFileAccess {
	
	private FileUtilBB m_fs = new FileUtilBB();

	public boolean exists(String name) {
		return m_fs.exists(name);
	}
	
	public long size(String name) {
		return m_fs.size(name);
	}
	
	public void delete(String name) {
		m_fs.delete(name);
	}

	public void renameOverwrite(String oldName, String newName) {
		m_fs.renameElement(oldName, newName);
	}

}
