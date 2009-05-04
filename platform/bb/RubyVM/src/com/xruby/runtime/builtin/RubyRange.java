/**
 * Copyright 2005-2007 Xue Yong Zhi, Jie Li, Ye Zheng
 * Distributed under the BSD License
 */

package com.xruby.runtime.builtin;

import com.xruby.runtime.lang.*;

//@RubyLevelClass(name="Range")
public class RubyRange extends RubyBasic {
    private RubyValue begin_;
    private RubyValue end_;
    private boolean exclude_end_;

    RubyRange() {
        super(RubyRuntime.RangeClass);
    }

    protected void doClone(RubyValue orig){
    	RubyRange cl = (RubyRange)orig;
    	begin_ = cl.begin_;
    	end_ = cl.end_;
    	exclude_end_ = cl.exclude_end_;
    	
    	super.doClone(orig);
    }
    
    public void setValue(RubyValue left, RubyValue right, boolean isExclusive) {
        if(!(left instanceof RubyFixnum && right instanceof RubyFixnum)){
            try {
                RubyValue result = RubyAPI.callOneArgMethod(left, right, null, RubyID.unequalID);
                if (result == RubyConstant.QNIL) {
                    throw new RubyException(RubyRuntime.ArgumentErrorClass, "bad value for range");
                }
            } catch (RubyException e) {
                RubyValue value = RubyAPI.convertRubyException2RubyValue(e);
                if (value.getRubyClass() == RubyRuntime.ArgumentErrorClass) {
                    throw new RubyException(RubyRuntime.ArgumentErrorClass, "bad value for range");
                }
                throw e;
            }
        }
        begin_ = left;
        end_ = right;
        exclude_end_ = isExclusive;
    }
    
    //@RubyAllocMethod
	public static RubyRange alloc(RubyValue receiver) {
		return ObjectFactory.createRange();
	}
    
    //@RubyLevelMethod(name="initialize")
    public RubyValue initialize(RubyValue arg0, RubyValue arg1) {
        this.setValue(arg0, arg1, false);
        return this;
    }
    
    //@RubyLevelMethod(name="initialize")
    public RubyValue initialize(RubyArray args) {
    	RubyValue left = args.get(0);
        RubyValue right = args.get(1);
        boolean isExclusive = false;
        if (args.size() == 3) {
            RubyValue exclusive = args.get(2);
            if (exclusive != RubyConstant.QNIL && exclusive != RubyConstant.QFALSE) {
                isExclusive = true;
            }
        }
        
        this.setValue(left, right, isExclusive);
        return this;
    }

    //@RubyLevelMethod(name="begin")
    public RubyValue getLeft() {
        return begin_;
    }

    //@RubyLevelMethod(name="end")
    public RubyValue getRight() {
        return end_;
    }
    
    //@RubyLevelMethod(name="exclude_end?")
    public RubyValue excludeEndP() {
    	return ObjectFactory.createBoolean(this.exclude_end_);
    }

    //RHO_COMMENT
    //@RubyLevelMethod(name="eql?")
    public RubyValue eql(RubyValue v) {
    	if ( !(v instanceof RubyRange))
    		return RubyConstant.QFALSE;
    	
    	RubyRange range = (RubyRange)v;
    	if ( range.begin_.equals(this.begin_) &&
    		 range.end_.equals(this.end_) &&
    		 range.exclude_end_ == this.exclude_end_ )
    		return RubyConstant.QTRUE;
    	
    	return RubyConstant.QFALSE;
    }
    
    public boolean isExcludeEnd() {
        return exclude_end_;
    }

    //@RubyLevelMethod(name="to_a")
    public RubyArray to_a() {
    	if (this.begin_ instanceof RubyFixnum && this.end_ instanceof RubyFixnum) {
    		return fixnumRangeToA();
    	} else {
    		return defaultToA();
    	}
    }

	private RubyArray defaultToA() {
		RubyArray a = new RubyArray();
		
		RubyValue iter = this.begin_;
		while (this.compare(iter, this.end_)) {
			a.add(iter);
			iter = RubyAPI.callPublicNoArgMethod(iter, null, RubyID.succID);
		}
		
		if (!this.exclude_end_) {
			a.add(iter);
		}
		
		return a;
	}

	private RubyArray fixnumRangeToA() {
		int left = begin_.toInt();
		int right = end_.toInt();
		if (!exclude_end_) {
			++right;
		}

		RubyArray a = new RubyArray();
		for (int i = left; i < right; ++i) {
			a.add(ObjectFactory.createFixnum(i));
		}
		return a;
	}
    
    //@RubyLevelMethod(name="hash")
    public RubyFixnum hash() {
        int baseHash = this.exclude_end_ ? 1 : 0;
        int beginHash = RubyAPI.callPublicNoArgMethod(this.begin_, null, RubyID.hashID).toInt();
        int endHash = RubyAPI.callPublicNoArgMethod(this.end_, null, RubyID.hashID).toInt();
        
        int hash = baseHash;
        hash = hash ^ (beginHash << 1);
        hash = hash ^ (endHash << 9);
        hash = hash ^ (baseHash << 24);
        return ObjectFactory.createFixnum(hash);
    }
    
    //@RubyLevelMethod(name="each")
    public RubyValue each(RubyBlock block) {
        if (this.begin_ instanceof RubyFixnum && this.end_ instanceof RubyFixnum) {
        	return eachForFixnum(block);
        }
        // FIXME: for string
        
        return rangeEach(block);
    }

	private RubyValue eachForFixnum(RubyBlock block) {
		int begin = this.begin_.toInt();
		int limit = this.end_.toInt(); 
		if (!this.exclude_end_) {
			limit++;
		}
		
		for (int i = begin; i < limit; i++) {
			RubyValue v = block.invoke(this, ObjectFactory.createFixnum(i));
			if (block.breakedOrReturned()) {
				return v;
			} else if (block.shouldRetry()) {
				i = begin - 1;
				continue;
			}
		}
		
		return this;
	}
	
    private boolean compare(RubyValue value1, RubyValue value2) {
        RubyValue r = RubyAPI.callPublicOneArgMethod(value1, value2, null, RubyID.unequalID);
        return !RubyAPI.testEqual(r, ObjectFactory.FIXNUM0);
    }
	
	private RubyValue rangeEach(RubyBlock block) {
		RubyValue ite = this.begin_;

        while (true) {
            while (compare(ite, this.end_)) {
                RubyValue v = block.invoke(this, ite);
                if (block.breakedOrReturned()) {
                    return v;
                } else if (block.shouldRetry()) {
                    ite = this.begin_;
                    continue;
                } else {
                    ite = RubyAPI.callPublicNoArgMethod(ite, null, RubyID.succID);
                }
            }

            if (!this.exclude_end_) {
                RubyValue v = block.invoke(this, ite);
                if (block.breakedOrReturned()) {
                    return v;
                } else if (block.shouldRetry()) {
                    ite = this.begin_;
                    continue;
                } else {
                    break;
                }
            }

            break;
        }

        return this;
	}
	
}
