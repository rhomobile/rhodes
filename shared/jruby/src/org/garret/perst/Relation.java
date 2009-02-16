package org.garret.perst;

/**
 * Class representing relation between owner and members
 */
public abstract class Relation extends Persistent implements Link {
    /**
     * Get relation owner
     * @return owner of the relation
     */
    public IPersistent getOwner() { 
        return owner;
    }

    /**
     * Set relation owner
     * @param owner new owner of the relation
     */
    public void setOwner(IPersistent owner) { 
        this.owner = owner;
        store();
    }
    
    /**
     * Relation constructor. Creates empty relation with specified owner and no members. 
     * Members can be added to the relation later.
     * @param owner owner of the relation
     */
    public Relation(IPersistent owner) {
        this.owner = owner;
    }
    
    protected Relation() {}

    private IPersistent owner;
}

