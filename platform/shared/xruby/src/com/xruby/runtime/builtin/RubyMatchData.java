/**
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.runtime.builtin;

import org.apache.oro.text.regex.MatchResult;
import com.xruby.runtime.lang.*;
import com.xruby.runtime.lang.annotation.RubyLevelClass;
import com.xruby.runtime.lang.annotation.RubyLevelMethod;

@RubyLevelClass(name="MatchData")
public class RubyMatchData extends RubyBasic {
    private MatchResult result_;

    RubyMatchData(MatchResult m) {
        super(RubyRuntime.MatchDataClass);
        result_ = m;
    }

    @RubyLevelMethod(name="to_s")
    public RubyString to_s() {
        return ObjectFactory.createString(result_.toString());
    }

    public String toString() {
        return result_.group(0);
    }

    @RubyLevelMethod(name="[]")
    public RubyValue aref(RubyValue arg) {
        int index = arg.toInt();
        return ObjectFactory.createString(result_.group(index));
    }
}
