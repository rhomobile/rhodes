package com.rho;

public interface IFileAccess {
	public void delete(String name);
	public boolean exists(String name);
	public void renameOverwrite(String oldName, String newName);

}
