package com.xruby.runtime.builtin;

import java.io.File;

import com.xruby.runtime.lang.RubyBlock;
import com.xruby.runtime.lang.RubyValue;
import com.xruby.runtime.lang.annotation.RubyLevelMethod;
import com.xruby.runtime.lang.annotation.RubyLevelModule;

@RubyLevelModule(name="FileTest")
public class RubyFileTestModule {
	@RubyLevelMethod(name="exist?", singleton=true)
	public static RubyValue exist_p(RubyValue receiver, RubyValue arg, RubyBlock block) {
		RubyString s = (RubyString)arg;
		File f = new File(s.toString());
		return ObjectFactory.createBoolean(f.exists());
	}
}
