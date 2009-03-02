package org.garret.perst.fulltext;

/**
 * Full text search result
 */
public class FullTextSearchResult { 
    /**
     * Estimation of total number of documents in the index matching this query.
     * Full text search query result is usually limited by number of returned documents
     * and query execution time. So there are can be more documents in the index matching this query than 
     * actually returned. This field provides estimation for total number of documents matching the query.
     */
    public int estimation;

    /**
     * Full text search result hits
     */
    public FullTextSearchHit[] hits;
    
    public FullTextSearchResult(FullTextSearchHit[] hits, int estimation) { 
        this.hits = hits;
        this.estimation = estimation;
    }
}