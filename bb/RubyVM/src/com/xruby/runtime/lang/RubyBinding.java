/** 
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.runtime.lang;

import com.xruby.runtime.builtin.RubyArray;
//import com.xruby.runtime.builtin.RubyRegexp;

import j2me.util.*;

public class RubyBinding extends RubyBasic {
	private RubyModule scope_;
	private RubyValue self_;
	private RubyBlock block_;
	private RubyArray variables_ = new RubyArray();
	private List/*<String>*/ names_ = new ArrayList/*<String>*/();
	
	public RubyBinding(RubyClass c) {
		super(c);
	}
	
	public RubyBinding() {
		super(RubyRuntime.BindingClass);
	}

    public RubyValue clone(){
    	RubyBinding cl = new RubyBinding(this.class_);
    	cl.doClone(this);
    	return cl;
    }
    protected void doClone(RubyValue orig){
    	RubyBinding cl = (RubyBinding)orig; 
    	scope_ = cl.scope_;
    	self_ = cl.self_;
    	block_ = cl.block_;
    	variables_ = cl.variables_;
    	names_ = cl.names_;
    	
    	super.doClone(orig);
    }
	
	public boolean hasName(String name) {
		return names_.indexOf(name) >= 0;
	}

	public RubyBinding setScope(RubyModule scope) {
		scope_ = scope;
		return this;
	}

	public RubyBinding setSelf(RubyValue self) {
		self_ = self;
		return this;
	}

	public RubyValue getSelf() {
		return self_;
	}

	public RubyBinding setBlock(RubyBlock block) {
		block_ = block;
		return this;
	}

	public RubyBlock getBlock() {
		return block_;
	}

	public RubyModule getScope() {
		return scope_;
	}

	public void addVariableName(String name) {
		if (names_.indexOf(name) < 0) {
			names_.add(name);
		}
	}

	public RubyBinding addVariable(String name, RubyValue value) {
		int i = names_.indexOf(name);
		if (i < 0) {
			names_.add(name);
			variables_.add(value);
		} else {
			variables_.set(i, value);
		}
		return this;
	}

	public RubyArray getVariables() {
		return variables_;
	}

	public List/*<String>*/ getVariableNames() {
		return names_;
	}
	
	protected RubyValue getVariable(String name) {
		int i = names_.indexOf(name);
		if (i < 0) {
			return null;
		} else {
			return variables_.get(i);
		}
		
	}
}
