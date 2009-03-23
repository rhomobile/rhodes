package org.garret.perst.fulltext;

/**
 * Base class for full test search query nodes.
 * Query can be parsed by FullTextSearchHelper class or explicitly created by user.
 */
public class FullTextQuery 
{
    public static final int MATCH = 0;     
    public static final int STRICT_MATCH = 1;     
    public static final int AND   = 2; 
    public static final int NEAR  = 3; 
    public static final int OR    = 4; 
    public static final int NOT   = 5; 

    public static final String[] operatorName = { "MATCH", "STRICT_MATCH", "AND", "NEAR", "OR", "NOT" };

    public int op;

    /**
     * Query node visitor.
     * It provides convenient way of iterating through query nodes.
     */
    public void visit(FullTextQueryVisitor visitor) { 
        visitor.visit(this);
    }

    /**
     * This method checks that query can be executed by interection of keyword occurrences lists
     * @return true if quuery can be executed by FullTextIndex, false otherwise
     */
    public boolean isConstrained() { 
        return false;
    }

    /**
     * Query node constructor
     * @param op operation code
     */
    public FullTextQuery(int op) { 
        this.op = op;
    }       
}


