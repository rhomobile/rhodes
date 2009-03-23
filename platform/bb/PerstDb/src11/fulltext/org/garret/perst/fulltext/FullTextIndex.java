package org.garret.perst.fulltext;

import org.garret.perst.*;
import java.io.Reader;


/**
 * Full text search index.
 * This index split document text in words, perform stemming of the words and build inverse index.
 * Full text index is able to execute search queries with logical operators (AND/OR/NOT) and 
 * strict match. Returned results are ordered by rank, which includes inverse document frequency (IDF),
 * frequency of word in the document, occurrence kind and nearness of query keywords in the document text.
 */
public interface FullTextIndex extends IPersistent, IResource
{
    /**
     * Add document to the index
     * @param obj document to be added
     */
    void add(FullTextSearchable obj);

    /**
     * Add document to the index
     * @param obj document to be added
     * @param text document text to be indexed
     * @param language language of the text
     */
    void add(IPersistent obj, Reader text, String language);

    /**
     * Delete document from the index
     * @param obj document to be deleted
     */
    void delete(IPersistent obj);

    /**
     * Parse and execute full text search query
     * @param query text of the query
     * @param language language if the query
     * @param maxResults maximal amount of selected documents
     * @param timeLimit limit for query execution time
     * @return result of query execution ordered by rank or null in case of empty or incorrect query
     */
    FullTextSearchResult search(String query, String language, int maxResults, int timeLimit);

    /**
     * Execute full text search query
     * @param query prepared query
     * @param maxResults maximal amount of selected documents
     * @param timeLimit limit for query execution time
     * @return result of query execution ordered by rank or null in case of empty or incorrect query
     */    
    FullTextSearchResult search(FullTextQuery query, int maxResults, int timeLimit);

    /**
     * Get total number of different words in all documents
     */
    int getNumberOfWords();

    /**
     * Get total number of indexed documents
     */
    int getNumberOfDocuments();
    
    /**
     * Get full text search helper
     */
    FullTextSearchHelper getHelper();
}