package com.rho.db;

import com.xruby.runtime.builtin.RubyArray;
import com.xruby.runtime.lang.RubyValue;

public interface IDBAdapter {

	public abstract IDBAdapter initialize(RubyValue v);

	// @RubyLevelMethod(name="insertIntoTable")
	public abstract RubyValue insertIntoTable(RubyValue tableName,
			RubyValue mapValues);

	// @RubyLevelMethod(name="selectFromTable")
	public abstract RubyValue selectFromTable(RubyArray args);

	// @RubyLevelMethod(name="updateIntoTable")
	public abstract RubyValue updateIntoTable(RubyArray args);

	public abstract RubyValue updateIntoTable(RubyValue tableName,
			RubyValue mapValues, RubyValue where);

	// @RubyLevelMethod(name="deleteAllFromTable")
	public abstract RubyValue deleteAllFromTable(RubyValue tableName);

	// @RubyLevelMethod(name="deleteFromTable")
	public abstract RubyValue deleteFromTable(RubyValue tableName,
			RubyValue where);

	// @RubyLevelMethod(name="close")
	public abstract RubyValue close();
	
	public abstract void setDbCallback(IDbCallback callback);

}