package com.rhomobile.rhodes;

import java.io.File;

import com.rho.IFileAccess;

public class FileAccess implements IFileAccess {

	public void delete(String name) {
		File file = new File(name);
		file.delete();
	}

	public boolean exists(String name) {
		File file = new File(name);
		return file.exists();
	}

	public void renameOverwrite(String oldName, String newName) {
		File file = new File(oldName);
		if (!file.renameTo(new File(newName)))
			throw new RuntimeException("Can not rename file");
	}

	public long size(String name) {
		File file = new File(name);
		return file.length();
	}

}
