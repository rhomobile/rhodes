package org.garret.perst.fulltext;

/**
 * Match node of full text query
 */
public class FullTextQueryMatchOp extends FullTextQuery
{
    /**
     * Matched word (shown be lowercvases and in normal form, unless used in quotes)
     */
    public String word;

    /**
     * Position of word in the query (zero based)
     */
    public int    pos;

    /**
     * Index of the word in query (set and used internally, should not be accessed by application)
     */
    public int    wno;


    /**
     * Query node visitor.
     */
    public void visit(FullTextQueryVisitor visitor) { 
        visitor.visit(this);
    }

    /**
     * Match node provides query constraint
     */
    public boolean isConstrained() { 
        return true;
    }

    public String toString() { 
        return op == MATCH ? word : '"' + word + '"';
    }


    /**
     * Match node constructor
     * @param op operation code (should ne MATCH or STICT_MATCH)
     * @param word searched word
     * @param pos position of word in the query
     */
    public FullTextQueryMatchOp(int op, String word, int pos) { 
        super(op);
        this.word = word;
        this.pos = pos;
    }
}    
