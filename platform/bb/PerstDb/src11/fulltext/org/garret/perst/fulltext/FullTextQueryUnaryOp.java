package org.garret.perst.fulltext;

/**
 * Unary node of full text query
 */
public class FullTextQueryUnaryOp extends FullTextQuery
{
    public FullTextQuery opd;
    
    /**
     * Query node visitor.
     */
    public void visit(FullTextQueryVisitor visitor) { 
        visitor.visit(this);
        opd.visit(visitor);
    }

    /**
     * This method checks that query can be executed by interection of keyword occurrences lists
     * @return true if quuery can be executed by FullTextIndex, false otherwise
     */
    public boolean isConstrained() { 
        return op == NOT ? false : opd.isConstrained();
    }

    public String toString() { 
        return operatorName[op] + '(' + opd.toString() + ')';
    }

    /**
     * Unary node constructor
     * @param op operation code
     * @param opd operand
     */
    public FullTextQueryUnaryOp(int op, FullTextQuery opd) { 
        super(op);
        this.opd = opd;
    }
}    
