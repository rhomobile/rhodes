package org.garret.perst.fulltext;

/**
 * Base class for full text query visitor
 */
public class FullTextQueryVisitor 
{    
    public void visit(FullTextQuery q) { 
    }

    public void visit(FullTextQueryBinaryOp q) { 
        visit((FullTextQuery)q);
    }

    public void visit(FullTextQueryUnaryOp q) { 
        visit((FullTextQuery)q);
    }

    public void visit(FullTextQueryMatchOp q) { 
        visit((FullTextQuery)q);
    }
}

    