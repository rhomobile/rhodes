package org.garret.perst.fulltext;

import java.io.*;
import java.util.*;
import org.garret.perst.*;

/**
 * Helper class for full text search.
 * This class provides functionality for parsing and stemming query
 * and tuning document rank calculation
 */
public class FullTextSearchHelper extends Persistent
{
    protected int maxWordLength = 100;

    /**
     * Perform stemming of the word
     * @param word word to be stemmed
     * @param language language of the word (null if unknown)
     * @return normal forms of the word (some words belongs to more than one part of the speech, so there
     * are can be more than one normal form)
     */
    public String[] getNormalForms(String word, String language) {
        return new String[] { word };
    }
    
    static boolean isLetter(char ch) { 
        return Character.isLowerCase(ch) || Character.isUpperCase(ch);
        //return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch > 0xFF;
    }

    /**
     * Split text of the documents into tokens
     * @param reader stream with document text
     * @return array of occurrences of words in thedocument
     */
    public Occurrence[] parseText(Reader reader) throws IOException {
        int pos = 0;
        ArrayList list = new ArrayList();
        int ch = reader.read();
        
        while (ch > 0) {
            if (isLetter((char)ch) || Character.isDigit((char)ch)) {
                StringBuffer buf = new StringBuffer();
                int wordPos = pos;
                do { 
                    pos += 1;
                    buf.append((char)ch);
                    ch = reader.read();
                } while (ch > 0 && (isLetter((char)ch) || Character.isDigit((char)ch)));
                String word = buf.toString().toLowerCase();
                if (word.length() <= maxWordLength && !isStopWord(word)) { 
                    list.add(new Occurrence(word, wordPos, 0));
                }
            } else {
                pos += 1;
                ch = reader.read();
            }
        }
        return (Occurrence[])list.toArray(new Occurrence[list.size()]);
    }

    protected String AND;
    protected String OR;
    protected String NOT;
    
    protected transient Hashtable stopList;

    protected void fillStopList() {
        stopList = new Hashtable();
        stopList.put("a", this);
        stopList.put("the", this);
        stopList.put("at", this);
        stopList.put("on", this);
        stopList.put("of", this);
        stopList.put("to", this);
        stopList.put("an", this);
    }

    public void onLoad() { 
        fillStopList();
    }

    /**
     * Check if word is stop word and should bw not included in index
     * @param word lowercased word
     * @return true if word is in stop list, false otherwize
     */
    public boolean isStopWord(String word) { 
        return stopList.get(word) != null;
    }

    /*
     * Full text search helper constructor
     */
    public FullTextSearchHelper(Storage storage) 
    { 
        super(storage);
        AND = "AND";
        OR  = "OR";
        NOT = "NOT";
        fillStopList();
    }

    protected FullTextSearchHelper() {}

    protected class QueryScanner 
    {
        String  query;
        int     pos;
        boolean inQuotes;
        boolean unget;
        String  word;
        int     wordPos;
        int     token;
        String  language;
        
        QueryScanner(String query, String language) {
            this.query = query;
            this.language = language;
        }

        static final int TKN_EOQ = 0;
        static final int TKN_WORD = 1;
        static final int TKN_AND = 2;
        static final int TKN_OR = 3;
        static final int TKN_NOT = 4;
        static final int TKN_LPAR = 5;
        static final int TKN_RPAR = 6;

        int scan() {    
            if (unget) { 
                unget = false;
                return token;
            }
            int len = query.length();
            int p = pos;
            String q = query;
            while (p < len) { 
                char ch = q.charAt(p);
                if (ch == '"') { 
                    inQuotes = !inQuotes;
                    p += 1;
                } else if (ch == '(') { 
                    pos = p + 1;
                    return token = TKN_LPAR;
                } else if (ch == ')') { 
                    pos = p + 1;
                    return token = TKN_RPAR;
                } else if (isLetter(ch) || Character.isDigit(ch)) { 
                    wordPos = p;
                    while (++p < len && (isLetter(q.charAt(p)) || Character.isDigit(q.charAt(p))));
                    String word = q.substring(wordPos, p);
                    pos = p;
                    if (word.equals(AND)) { 
                        return token = TKN_AND;
                    } else if (word.equals(OR)) { 
                        return token = TKN_OR;
                    } else if (word.equals(NOT)) { 
                        return token = TKN_NOT;
                    } else { 
                        word = word.toLowerCase();
                        if (!isStopWord(word)) { 
                            if (!inQuotes) { 
                                // just get the first normal form and ignore all other alternatives 
                                word = getNormalForms(word, language)[0]; 
                            }   
                            this.word = word;
                            return token = TKN_WORD;
                        }
                    }
                } else { 
                    p += 1;
                }
            }
            pos = p;
            return token = TKN_EOQ;
        }
    }    

    protected FullTextQuery disjunction(QueryScanner scanner) { 
        FullTextQuery left = conjunction(scanner);
        if (scanner.token == QueryScanner.TKN_OR) {
            FullTextQuery right = disjunction(scanner);
            if (left != null && right != null) { 
                return new FullTextQueryBinaryOp(FullTextQuery.OR, left, right);
            } else if (right != null) { 
                return right;
            }
        }
        return left;
    }

    protected FullTextQuery conjunction(QueryScanner scanner) { 
        FullTextQuery left = term(scanner);
        if (scanner.token == QueryScanner.TKN_WORD || scanner.token == QueryScanner.TKN_AND) { 
            if (scanner.token == QueryScanner.TKN_WORD) { 
                scanner.unget = true;
            }
            int cop = scanner.inQuotes ? FullTextQuery.NEAR : FullTextQuery.AND;
            FullTextQuery right = disjunction(scanner);
            if (left != null && right != null) { 
                return new FullTextQueryBinaryOp(cop, left, right);
            } else if (right != null) { 
                return right;
            }
        }
        return left;
    }

    protected FullTextQuery term(QueryScanner scanner) {
        FullTextQuery q = null;
        switch (scanner.scan()) { 
           case QueryScanner.TKN_NOT:
               q = term(scanner);
               return (q != null) ? new FullTextQueryUnaryOp(FullTextQuery.NOT, q) : null;
           case QueryScanner.TKN_LPAR:
               q = disjunction(scanner);
               break;
           case QueryScanner.TKN_WORD:
               q = new FullTextQueryMatchOp(scanner.inQuotes ? FullTextQuery.STRICT_MATCH : FullTextQuery.MATCH,
                                            scanner.word, scanner.wordPos);
               break;
           case QueryScanner.TKN_EOQ:
               return null;
        }
        scanner.scan();
        return q;
    }

    public FullTextQuery parseQuery(String query, String language) {
        return disjunction(new QueryScanner(query, language));
    }

    static final float[] OCCURRENCE_KIND_WEIGHTS = new float[0];

    /**
     * Get occurrence kind weight. Occurrence kinds can be: in-title, in-header, emphased,...
     * It is up to the document scanner implementation how to enumerate occurence kinds.
     * These is only one limitation - number of difference kinds should not exceed 8.
     * @return array with weights of each occurrence kind
     */
    public float[] getOccurrenceKindWeights() {
        return OCCURRENCE_KIND_WEIGHTS;
    }
    
    /**
     * Get weight of nearness criteria in document rank.
     * Document rank is calculated as (keywordRank*(1 + nearness*nearnessWeight))
     * @return weight of nearness criteria
     */
    public float getNearnessWeight() { 
        return 10.0f;
    }
    
    /** 
     * Get penalty of inverse word order in the text.
     * Assume that document text contains phrase "ah oh ugh".
     * And query "ugh ah" is executed. The distance between "ugh" and "ah"
     * in the document text is 6. But as far as them are in difference order than in query, 
     * this distance will be multiplied on "swap penalty", so if swap penalty is 10, then distance
     * between these two word is considered to be 60.
     * @return swap penalty
     */
    public int getWordSwapPenalty() { 
        return 10;
    }
}
    
