/** 
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.compiler.codedom;

import java.util.ArrayList;

public abstract class Statement implements Visitable {

	void getNewlyAssignedVariables(ISymbolTable symboltable, ArrayList<String> result) {
		//Default behavior is do nothing
	}

    void pullBlock(ArrayList<Block> result) {
        //Default behavior is do nothing
    }
}
