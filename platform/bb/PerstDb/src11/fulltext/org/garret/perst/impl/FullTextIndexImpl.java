package org.garret.perst.impl;

import java.io.*;
import java.util.*;
import org.garret.perst.*;
import org.garret.perst.fulltext.*;

public class FullTextIndexImpl extends PersistentResource implements FullTextIndex
{
    protected Index inverseIndex;
    protected Index documents;
    protected FullTextSearchHelper helper;
    
    static class DocumentOccurrences extends Persistent {
        InverseList list;
        int nWordsInDocument;
        int[] occurrences;
    }

    static class Document extends Persistent { 
        IPersistent obj;
        Link occurrences;

        Document() {}

        Document(Storage storage, IPersistent obj) { 
            super(storage);
            this.obj = obj;
            occurrences = storage.createLink();
        }        
    }

    static class InverseList extends Btree
    { 
        int[] oids;
        Link  docs;

        static final int BTREE_THRESHOLD = 500;

        InverseList(Storage db, int oid, DocumentOccurrences doc) { 
            super(Types.Int, true);
            docs = db.createLink(1);
            docs.add(doc);
            oids = new int[1];
            oids[0] = oid;
            assignOid(db, 0, false);
        }

        InverseList() {}

        public int size() { 
            return oids != null ? oids.length : super.size();
        }

        int first() { 
            if (oids != null) { 
                return oids[0];
            }
            Map.Entry entry = (Map.Entry)entryIterator(null, null, Index.ASCENT_ORDER).next();
            return ((Integer)entry.getKey()).intValue();
        }

        int last() { 
            if (oids != null) { 
                return oids[oids.length-1];
            }
            Map.Entry entry = (Map.Entry)entryIterator(null, null, Index.DESCENT_ORDER).next();
            return ((Integer)entry.getKey()).intValue();
        }

        class InverstListIterator extends Iterator 
        {
            int i;

            InverstListIterator(int pos) { 
                i = pos;
            }
            
            public boolean hasNext() { 
                return i < oids.length;
            }

            public int nextOid() {
                return oids[i++];
            }

            public Object next() { 
                final int j = i++;
                return new Map.Entry() { 
                        public Object getKey() { return new Integer(oids[j]); }
                        public Object getValue() { return docs.get(j); }
                        public Object setValue(Object value) { return null; }
                        public Object setKey(Object key) { return null; }
                    };
            }
            
            public void remove() {}
        }

        Iterator iterator(int oid) { 
            int[] os = oids;
            if (os != null) { 
                int l = 0, r = os.length;
                while (l < r) { 
                    int m = (l + r) >>> 1;
                    if (os[m] < oid) { 
                        l = m + 1;
                    } else { 
                        r = m;
                    }
                }
                return new InverstListIterator(r);
            } else { 
                return entryIterator(new Key(oid), null, Index.ASCENT_ORDER);
            }
        }

        void add(int oid, DocumentOccurrences doc) { 
            int[] os = oids;
            if (os == null || os.length >= BTREE_THRESHOLD) { 
                if (os != null) { 
                    for (int i = 0; i < os.length; i++) { 
                        super.put(new Key(os[i]), docs.get(i));
                    }
                    oids = null;
                    docs = null;
                }
                super.put(new Key(oid), doc);
            } else { 
                int l = 0, n = os.length, r = n;
                while (l < r) { 
                    int m = (l + r) >>> 1;
                    if (os[m] < oid) { 
                        l = m + 1;
                    } else { 
                        r = m;
                    }
                }
                os = new int[n+1];
                System.arraycopy(oids, 0, os, 0, r);
                os[r] = oid;
                System.arraycopy(oids, r, os, r+1, n-r);
                docs.insert(r, doc);
                oids = os;
            }
        }

        void remove(int oid) {            
            int[] os = oids;
            if (os != null) { 
                int l = 0, n = os.length, r = n;
                while (l < r) { 
                    int m = (l + r) >>> 1;
                    if (os[m] < oid) { 
                        l = m + 1;
                    } else { 
                        r = m;
                    }
                }
                Assert.that(r < n && os[r] == oid);
                docs.remove(r);
                oids = new int[n-1];
                System.arraycopy(os, 0, oids, 0, r);
                System.arraycopy(os, r+1, oids, r, n-r-1);
            } else { 
                super.remove(new Key(oid));
            }
        }
    }

    public void add(FullTextSearchable obj) {
        add(obj, obj.getText(), obj.getLanguage());
    }
    
    public void add(IPersistent obj, Reader text, String language) { 
        Occurrence[] occurrences;
        try { 
            occurrences = helper.parseText(text);
        } catch (IOException x) { 
            throw new StorageError(StorageError.FULL_TEXT_INDEX_ERROR, x);
        }
        delete(obj);
        if (occurrences.length > 0) { 
            Document doc = new Document(getStorage(), obj);
            documents.put(new Key(obj), doc);
            Arrays.sort(occurrences);
            String word = occurrences[0].word;
            int i = 0;
            for (int j = 1; j < occurrences.length; j++) { 
                Occurrence occ = occurrences[j];
                if (!occ.word.equals(word)) {                     
                    addReference(doc, word, occurrences, i, j, language);
                    word = occ.word;
                    i = j;
                }
            }
            addReference(doc, word, occurrences, i, occurrences.length, language);
        }
    }
     
    static final int OCC_KIND_OFFSET = 24;
    static final int OCC_POSITION_MASK = (1 << OCC_KIND_OFFSET) - 1;

    private final void addReference(Document doc, String word, Occurrence[] occurrences, int from, int till)
    {
        DocumentOccurrences d = new DocumentOccurrences();
        d.occurrences = new int[till - from];
        d.nWordsInDocument = occurrences.length;
        for (int i = from; i < till; i++) { 
            d.occurrences[i - from] = occurrences[i].position | (occurrences[i].kind << OCC_KIND_OFFSET);
        }
        int oid = doc.obj.getOid();
        InverseList list = (InverseList)inverseIndex.get(word);
        if (list == null) { 
            list = new InverseList(getStorage(), oid, d);
            inverseIndex.put(word, list);
        } else { 
            list.add(oid, d);
        }
        d.list = list;
        doc.occurrences.add(d);
    }
            
    private final void addReference(Document doc, String word, Occurrence[] occurrences, int from, int till, String language)
    {
        String[] normalForms = helper.getNormalForms(word, language);
        boolean isNormalForm = false;
        for (int i = 0; i < normalForms.length; i++) { 
            if (word.equals(normalForms[i])) {
                isNormalForm = true;
            }
            addReference(doc, normalForms[i], occurrences, from, till);
        }
        if (!isNormalForm) { 
            addReference(doc, word, occurrences, from, till);
        }
    }

    public void delete(IPersistent obj) { 
        Key key = new Key(obj);
        Document doc = (Document)documents.get(key);
        if (doc != null) { 
            for (int i = 0, n = doc.occurrences.size(); i < n; i++) { 
                DocumentOccurrences d = (DocumentOccurrences)doc.occurrences.get(i);
                d.list.remove(obj.getOid());
                d.deallocate();
            }
            documents.remove(key);
            doc.deallocate();
        }
    }
    
    public int getNumberOfWords() { 
        return inverseIndex.size();
    }

    public int getNumberOfDocuments() { 
        return documents.size();
    }

    public FullTextSearchResult search(String query, String language, int maxResults, int timeLimit) {
        return search(helper.parseQuery(query, language), maxResults, timeLimit);
    }

    protected static class KeywordList {
        InverseList list;
        int[]       occ;
        String      word;
        int         sameAs;
        int         kwdLen;
        int         kwdOffset;
        int         occPos;
        int         currDoc;
        Map.Entry   currEntry;
        Iterator    iterator;

        KeywordList(String word) {
            this.word = word;
            kwdLen = word.length();
            sameAs = -1;
        }
    }

    static class ExpressionWeight implements Comparable {
        int weight;
        FullTextQuery expr;
        
        public int compareTo(Object o) { 
            return weight - ((ExpressionWeight)o).weight;
        }
    }

    protected class FullTextSearchEngine extends FullTextQueryVisitor
    {
        KeywordList[] kwds;
        ArrayList     kwdList;
        int[]         occurrences;
        int           nOccurrences;
        float[]       occurrenceKindWeight;

        public void visit(FullTextQueryMatchOp q) { 
            q.wno = kwdList.size(); 
            KeywordList kwd = new KeywordList(q.word);
            kwd.list = (InverseList)inverseIndex.get(q.word);
            kwdList.add(kwd);
        }
        
        static final int STRICT_MATCH_BONUS = 8;

        int calculateWeight(FullTextQuery query) 
        { 
            switch (query.op) {
            case FullTextQuery.AND:
                {
                    return calculateWeight(((FullTextQueryBinaryOp)query).left);
                }
            case FullTextQuery.NEAR:
                {
                    int shift = STRICT_MATCH_BONUS;
                    for (FullTextQuery q = ((FullTextQueryBinaryOp)query).right; q.op == FullTextQuery.NEAR; q = ((FullTextQueryBinaryOp)q).right) { 
                        shift += STRICT_MATCH_BONUS;
                    }
                    return shift >= 32 ? 0 : (calculateWeight(((FullTextQueryBinaryOp)query).left) >> shift);
                }
            case FullTextQuery.OR:
                {
                    int leftWeight = calculateWeight(((FullTextQueryBinaryOp)query).left);
                    int rightWeight = calculateWeight(((FullTextQueryBinaryOp)query).right);
                    return leftWeight > rightWeight ? leftWeight : rightWeight;
                }
            case FullTextQuery.MATCH:
            case FullTextQuery.STRICT_MATCH:
                {
                    InverseList list = kwds[((FullTextQueryMatchOp)query).wno].list;
                    return list == null ? 0 : list.size();
                }
            default:
                return Integer.MAX_VALUE;
            }
        }

        FullTextQuery optimize(FullTextQuery query) 
        { 
            switch (query.op) {
            case FullTextQuery.AND:
            case FullTextQuery.NEAR:
                {
                    int op = query.op;
                    int nConjuncts = 1;
                    FullTextQuery q = query; 
                    while ((q = ((FullTextQueryBinaryOp)q).right).op == op) { 
                        nConjuncts += 1;
                    }
                    ExpressionWeight[] conjuncts = new ExpressionWeight[nConjuncts+1];
                    q = query; 
                    for (int i = 0; i < nConjuncts; i++) { 
                        FullTextQueryBinaryOp and = (FullTextQueryBinaryOp)q;
                        conjuncts[i] = new ExpressionWeight();
                        conjuncts[i].expr = optimize(and.left);
                        conjuncts[i].weight = calculateWeight(conjuncts[i].expr);
                        q = and.right;
                    }
                    conjuncts[nConjuncts] = new ExpressionWeight();
                    conjuncts[nConjuncts].expr = optimize(q);
                    conjuncts[nConjuncts].weight = calculateWeight(conjuncts[nConjuncts].expr);
                    Arrays.sort(conjuncts);
                    if (op == FullTextQuery.AND) { // eliminate duplicates
                        int n = 0, j = -1;
                        InverseList list = null;
                        for (int i = 0; i <= nConjuncts; i++) {
                            q = conjuncts[i].expr;
                            if (q instanceof FullTextQueryMatchOp) {
                                FullTextQueryMatchOp match = (FullTextQueryMatchOp)q;
                                if (n == 0 || kwds[match.wno].list != list) { 
                                    j = match.wno;
                                    list = kwds[j].list;
                                    conjuncts[n++] = conjuncts[i];                            
                                } else { 
                                    kwds[match.wno].sameAs = j;
                                }
                            } else { 
                                conjuncts[n++] = conjuncts[i];
                            }
                        }
                        nConjuncts = n - 1;
                    } else { // calculate distance between keywords
                        int kwdPos = 0;
                        for (int i = 0; i <= nConjuncts; i++) {
                            q = conjuncts[i].expr;
                            if (q instanceof FullTextQueryMatchOp) {
                                FullTextQueryMatchOp match = (FullTextQueryMatchOp)q;
                                kwds[match.wno].kwdOffset = match.pos - kwdPos;
                                kwdPos = match.pos;
                            }
                        }
                    }
                    if (nConjuncts == 0) { 
                        return conjuncts[0].expr; 
                    } else { 
                        q = query; 
                        int i = 0;
                        while (true) { 
                            FullTextQueryBinaryOp and = (FullTextQueryBinaryOp)q;
                            and.left = conjuncts[i].expr; 
                            if (++i < nConjuncts) { 
                                q = and.right;
                            } else { 
                                and.right = conjuncts[i].expr; 
                                break;
                            }
                        }
                    }
                    break;
                }
            case FullTextQuery.OR:
                {
                    FullTextQueryBinaryOp or = (FullTextQueryBinaryOp)query;
                    or.left = optimize(or.left);
                    or.right = optimize(or.right);
                    break;
                }
            case FullTextQuery.NOT:
                {
                    FullTextQueryUnaryOp not = (FullTextQueryUnaryOp)query;
                    not.opd = optimize(not.opd);
                }
                break;
            default:;
            }
            return query;
        }
        
        int intersect(int doc, FullTextQuery query) {
            int left, right;

            switch (query.op) { 
            case FullTextQuery.AND:
            case FullTextQuery.NEAR:
                do { 
                    left = intersect(doc, ((FullTextQueryBinaryOp)query).left);
                    if (left == Integer.MAX_VALUE) {
                        return left;
                    }
                    doc = intersect(left, ((FullTextQueryBinaryOp)query).right);
                } while (left != doc && doc != Integer.MAX_VALUE);
                return doc;
            case FullTextQuery.OR:
                left = intersect(doc, ((FullTextQueryBinaryOp)query).left);
                right = intersect(doc, ((FullTextQueryBinaryOp)query).right);
                return left < right ? left : right;
            case FullTextQuery.MATCH:
            case FullTextQuery.STRICT_MATCH:
                {
                    KeywordList kwd = kwds[((FullTextQueryMatchOp)query).wno]; 
                    if (kwd.currDoc >= doc) { 
                        return kwd.currDoc;
                    }
                    Iterator iterator = kwd.iterator;                    
                    if (iterator != null) { 
                        if (iterator.hasNext()) { 
                            Map.Entry entry = (Map.Entry)iterator.next();
                            int nextDoc = ((Integer)entry.getKey()).intValue();
                            if (nextDoc >= doc) { 
                                kwd.currEntry = entry;
                                kwd.currDoc = nextDoc;
                                return nextDoc;
                            } 
                        } else { 
                            kwd.currEntry = null;
                            kwd.currDoc = 0;
                            return Integer.MAX_VALUE;
                        }
                    } 
                    if (kwd.list != null) { 
                        kwd.iterator = iterator = kwd.list.iterator(doc);
                        if (iterator.hasNext()) { 
                            Map.Entry entry = (Map.Entry)iterator.next();
                            doc = ((Integer)entry.getKey()).intValue();
                            kwd.currEntry = entry;
                            kwd.currDoc = doc;
                            return doc;
                        }
                    }
                    kwd.currEntry = null;
                    kwd.currDoc = 0;
                    return Integer.MAX_VALUE;
                }
            case FullTextQuery.NOT:
                {
                    int nextDoc = intersect(doc, ((FullTextQueryUnaryOp)query).opd);
                    if (nextDoc == doc) { 
                        doc += 1;
                    }
                    return doc;
                }
            default:
                return doc;
            }
        }

        int calculateEstimation(FullTextQuery query, int nResults)
        {
            switch (query.op) { 
            case FullTextQuery.AND:
            case FullTextQuery.NEAR:
                { 
                    int left = calculateEstimation(((FullTextQueryBinaryOp)query).left, nResults);
                    int right = calculateEstimation(((FullTextQueryBinaryOp)query).right, nResults);
                    return left < right ? left : right;
                }
            case FullTextQuery.OR:
                { 
                    int left = calculateEstimation(((FullTextQueryBinaryOp)query).left, nResults);
                    int right = calculateEstimation(((FullTextQueryBinaryOp)query).right, nResults);
                    return left > right ? left : right;
                }
            case FullTextQuery.MATCH:
            case FullTextQuery.STRICT_MATCH:
                {
                    KeywordList kwd = kwds[((FullTextQueryMatchOp)query).wno];
                    if (kwd.currDoc == 0) { 
                        return 0;
                    } else { 
                        int curr = kwd.currDoc;
                        int first = kwd.list.first();
                        int last = kwd.list.last();
                        int estimation = nResults * (last - first + 1) / (curr - first + 1);
                        if (estimation > kwd.list.size()) { 
                            estimation = kwd.list.size();
                        }
                        return estimation;
                    }
                }
            case FullTextQuery.NOT:
                return documents.size();
                
            }
            return 0;
        }
        
        static final double DENSITY_MAGIC = 2;

        double evaluate(int doc, FullTextQuery query) { 
            double left, right;
            switch (query.op) { 
            case FullTextQuery.NEAR:
            case FullTextQuery.AND:
                left = evaluate(doc, ((FullTextQueryBinaryOp)query).left);
                right = evaluate(doc, ((FullTextQueryBinaryOp)query).right);
                nOccurrences = 0;
                return left < 0 || right < 0 ? -1 : left + right;
            case FullTextQuery.OR:
                left = evaluate(doc, ((FullTextQueryBinaryOp)query).left);
                right = evaluate(doc, ((FullTextQueryBinaryOp)query).right);
                return left > right ? left : right;
            case FullTextQuery.MATCH:
            case FullTextQuery.STRICT_MATCH:
                {
                    KeywordList kwd = kwds[((FullTextQueryMatchOp)query).wno]; 
                    if (kwd.currDoc != doc) {
                        return -1;
                    }                    
                    DocumentOccurrences d = (DocumentOccurrences)kwd.currEntry.getValue();
                    int[] occ = d.occurrences;
                    kwd.occ = occ;
                    int frequency = occ.length;
                    if (query.op == FullTextQuery.STRICT_MATCH) { 
                        if (nOccurrences == 0) { 
                            nOccurrences = frequency;
                            if (occurrences == null || occurrences.length < frequency) {
                                occurrences = new int[frequency];
                            }
                            for (int i = 0; i < frequency; i++) { 
                                occurrences[i] = occ[i] & OCC_POSITION_MASK;
                            }
                        } else { 
                            int nPairs = 0;
                            int[] dst = occurrences;
                            int occ1 = dst[0];
                            int occ2 = occ[0] & OCC_POSITION_MASK;
                            int i = 0, j = 0; 
                            int offs = kwd.kwdOffset;
                            while (true) { 
                                if (occ1 + offs <= occ2) { 
                                    if (occ1 + offs + 1 >= occ2) { 
                                        dst[nPairs++] = occ2;
                                    } 
                                    if (++j == nOccurrences) { 
                                        break;
                                    }
                                    occ1 = dst[j];
                                } else { 
                                    if (++i == frequency) { 
                                        break;
                                    }
                                    occ2 = occ[i] & OCC_POSITION_MASK;
                                }
                            }
                            nOccurrences = nPairs;
                            if (nPairs == 0) { 
                                return -1;
                            }
                        }
                    }
                    int totalNumberOfDocuments = documents.size();
                    int nRelevantDocuments = kwd.list.size();
                    int totalNumberOfWords = inverseIndex.size();
                    double idf = MathCLDC11.log((double)totalNumberOfDocuments / nRelevantDocuments);
                    double averageWords = (double)totalNumberOfWords / totalNumberOfDocuments;
                    double density = frequency * MathCLDC11.log(1 + (DENSITY_MAGIC*averageWords / d.nWordsInDocument)); 
                    double wordWeight = (density * idf);
                    double wordScore = 1;
                    for (int i = 0; i < frequency; i++) { 
                        wordScore += wordWeight * occurrenceKindWeight[occ[i] >>> OCC_KIND_OFFSET];
                    }
                    return MathCLDC11.log(wordScore);
                }
            case FullTextQuery.NOT:
                {
                    double rank = evaluate(doc, ((FullTextQueryUnaryOp)query).opd);
                    return (rank >= 0) ? -1 : 0;
                }
            default:
                return -1;
            }
        }
            
        void buildOccurrenceKindWeightTable() 
        {
            occurrenceKindWeight = new float[256];
            float[] weights = helper.getOccurrenceKindWeights();
            occurrenceKindWeight[0] = 1.0f;
            for (int i = 1; i < 256; i++) { 
                float weight = 0;
                for (int j = 0; j < weights.length; j++) { 
                    if ((i & (1 << j)) != 0) { 
                        weight += weights[j];
                    }
                    occurrenceKindWeight[i] = weight;
                }
            }
        }
               
        double calculateNearness() {
            KeywordList[] kwds = this.kwds;
            int nKwds = kwds.length;
            if (nKwds < 2) { 
                return 0;
            }
            for (int i = 0; i < nKwds; i++) {                 
                if (kwds[i].occ == null) { 
                    int j = kwds[i].sameAs;
                    if (j >= 0 && kwds[j].occ != null) { 
                        kwds[i].occ = kwds[j].occ;
                    } else { 
                        return 0;
                    }
                }
                kwds[i].occPos = 0;
            }
            double maxNearness = 0;
            int swapPenalty = helper.getWordSwapPenalty();
            while (true) { 
                int minPos = Integer.MAX_VALUE;
                double nearness = 0;
                KeywordList first = null;
                KeywordList prev = null;
                for (int i = 0; i < nKwds; i++) {                 
                    KeywordList curr = kwds[i];
                    if (curr.occPos < curr.occ.length) { 
                        if (prev != null) { 
                            int offset = curr.occ[curr.occPos] - prev.occ[prev.occPos];
                            if (offset < 0) { 
                                offset = (-offset - curr.kwdLen) * swapPenalty;
                            } else { 
                                offset -= prev.kwdLen;
                            }
                            if (offset <= 2) {
                                offset = 1;
                            }                            
                            nearness += 1/Math.sqrt(offset);
                        }
                        if (curr.occ[curr.occPos] < minPos) { 
                            minPos = curr.occ[curr.occPos];
                            first = curr;
                        }
                        prev = curr;
                    }
                }
                if (first == null) { 
                    break;
                }
                first.occPos += 1;
                    
                if (nearness > maxNearness) { 
                    maxNearness = nearness;
                }
            }
            return maxNearness;
        }
         
        void reset() {
            nOccurrences = 0;
            for (int i = 0; i < kwds.length; i++) {                 
                kwds[i].occ = null;
            }
        }
     
        FullTextSearchResult search(FullTextQuery query, int maxResults, int timeLimit) { 
            if (query == null || !query.isConstrained()) {
                return null;
            }
            long start = System.currentTimeMillis();
            buildOccurrenceKindWeightTable();
            kwdList = new ArrayList();
            query.visit(this);
            kwds = (KeywordList[])kwdList.toArray(new KeywordList[kwdList.size()]);
            query = optimize(query);
            //System.out.println(query.toString());
            FullTextSearchHit[] hits = new FullTextSearchHit[maxResults];
            int currDoc = 1;
            int nResults = 0;            
            float nearnessWeight = helper.getNearnessWeight();
            boolean noMoreMatches = false;
            while (nResults < maxResults && System.currentTimeMillis() < start + timeLimit) { 
                currDoc = intersect(currDoc, query);
                if (currDoc == Integer.MAX_VALUE) { 
                    noMoreMatches = true;
                    break;
                }
                reset();
                double kwdRank = evaluate(currDoc, query);
                if (kwdRank >= 0) { 
                    double nearness = calculateNearness();
                    float rank = (float)(kwdRank*(1 + nearness*nearnessWeight));
                    //System.out.println("kwdRank=" + kwdRank + ", nearness=" + nearness + ", total rank=" + rank);
                    hits[nResults++] = new FullTextSearchHit(getStorage(), currDoc, rank);
                }
                currDoc += 1;
            }
            int estimation;
            if (nResults < maxResults) { 
                FullTextSearchHit[] realHits = new FullTextSearchHit[nResults];
                System.arraycopy(hits, 0, realHits, 0, nResults);
                hits = realHits;
            }
            if (noMoreMatches) {
                estimation = nResults;
            } else if (query instanceof FullTextQueryMatchOp) { 
                estimation = kwds[0].list.size();
            } else { 
                estimation = calculateEstimation(query, nResults);
            }
            Arrays.sort(hits);
            return new FullTextSearchResult(hits, estimation);
        }
    }

    public FullTextSearchResult search(FullTextQuery query, int maxResults, int timeLimit) { 
        FullTextSearchEngine engine = new FullTextSearchEngine();
        return engine.search(query, maxResults, timeLimit);
    }

    public FullTextSearchHelper getHelper() {
        return helper;
    }

    public FullTextIndexImpl(Storage storage, FullTextSearchHelper helper) { 
        super(storage);
        this.helper = helper;
        inverseIndex = storage.createIndex(Types.String, true);
        documents = storage.createIndex(Types.Object, true);
    }
    
    private  FullTextIndexImpl() {}
}
