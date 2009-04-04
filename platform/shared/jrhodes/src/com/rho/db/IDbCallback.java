package com.rho.db;

import com.xruby.runtime.builtin.RubyArray;

public interface IDbCallback {

	public abstract void OnDeleteAllFromTable(String tableName);
	public abstract void OnDeleteFromTable(String tableName, RubyArray rows2Delete);
}
