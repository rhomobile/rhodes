package org.garret.perst.fulltext;

import org.garret.perst.*;

/**
 * Class representing full text search result hit (document + rank)
 */
public class FullTextSearchHit implements Comparable { 
    /**
     * Get document matching full text query 
     */
    public IPersistent getDocument() { 
        return storage.getObjectByOID(oid);
    }

    /**
     * Rank of the document for this query
     */
    public final float rank;

    /**
     * Object identifier of document
     */
    public final int oid;

    public final Storage storage;

    public int compareTo(Object o) {
        float oRank = ((FullTextSearchHit)o).rank;
        return rank > oRank ? -1 : rank < oRank ? 1 : 0;
    }

    /**
     * Constructor of the full text search result hit
     */
    public FullTextSearchHit(Storage storage, int oid, float rank) { 
        this.storage = storage;
        this.oid = oid;
        this.rank = rank;
    }    
}