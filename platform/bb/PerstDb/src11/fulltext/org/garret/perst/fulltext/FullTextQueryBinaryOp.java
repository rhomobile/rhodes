package org.garret.perst.fulltext;


/**
 * Binary node of full text query
 */
public class FullTextQueryBinaryOp extends FullTextQuery
{
    public FullTextQuery left;
    public FullTextQuery right;
    
    /**
     * Query node visitor.
     */
    public void visit(FullTextQueryVisitor visitor) { 
        visitor.visit(this);
        left.visit(visitor);
        right.visit(visitor);
    }

    /**
     * This method checks that query can be executed by interection of keyword occurrences lists
     * @return true if quuery can be executed by FullTextIndex, false otherwise
     */
    public boolean isConstrained() { 
        return op == OR 
            ? left.isConstrained() && right.isConstrained()
            : left.isConstrained() || right.isConstrained();
    }

    public String toString() {
        return op == OR
            ? '(' + left.toString() + ") OR (" + right.toString() + ')'
            : left.toString() + ' ' + operatorName[op] + ' ' + right.toString();
    }

    /**
     * Binary node constructor
     * @param op operation code
     * @param left left operand
     * @param right right operand
     */
    public FullTextQueryBinaryOp(int op, FullTextQuery left, FullTextQuery right) { 
        super(op);
        this.left = left;
        this.right = right;
    }
}    
    
