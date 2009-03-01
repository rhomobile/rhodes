package org.garret.perst.fulltext;

import org.garret.perst.Storage;
import org.garret.perst.impl.FullTextIndexImpl;

/**
 * Full text search index factory.
 * This classs creates particular implementation of full text search index
 */
public class FullTextIndexFactory
{
    /**
     * Create full text search index
     * @param db storage in which index should be created
     * @param helper helper class which provides method for scanning, stemming and tuning query
     * @return full text search index
     */
    public static FullTextIndex createFullTextIndex(Storage db, FullTextSearchHelper helper) {
        return new FullTextIndexImpl(db, helper);
    }

    /**
     * Create full text search index with default helper
     * @param db storage in which index should be created
     * @return full text search index
     */
    public static FullTextIndex createFullTextIndex(Storage db) {
        return createFullTextIndex(db, new FullTextSearchHelper(db));
    }
}