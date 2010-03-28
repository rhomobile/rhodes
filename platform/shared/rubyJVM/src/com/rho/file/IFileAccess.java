package com.rho.file;

public interface IFileAccess {
	public boolean exists(String name);
	public long size(String name);
	public void delete(String name);
	public void renameOverwrite(String oldName, String newName);

}
