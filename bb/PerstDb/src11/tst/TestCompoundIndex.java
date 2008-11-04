import org.garret.perst.*;

public class TestCompoundIndex { 
    final static int nRecords = 100000;
    final static int pagePoolSize = 32*1024*1024;

    public static class Record extends Persistent { 
        String strKey;
        int    intKey;
        
        public Record() {}
        
        
        public void writeObject(IOutputStream out) {
            out.writeString(strKey);
            out.writeInt(intKey);
        }
        
        public void readObject(IInputStream in) {
            strKey = in.readString();
            intKey = in.readInt();
        }
    };

    static public void main(String[] args) {    
        Storage db = StorageFactory.getInstance().createStorage();

        for (int i = 0; i < args.length; i++) { 
            if ("altbtree".equals(args[i])) { 
                db.setProperty("perst.alternative.btree", Boolean.TRUE);
            }
        }
        db.open("testcidx.dbs", pagePoolSize);
        Index root = (Index)db.getRoot();
        if (root == null) { 
            root = db.createIndex(new int[]{Types.Int, Types.String}, true);
            db.setRoot(root);
        }
        long start = System.currentTimeMillis();
        long key = 1999;
        int i;
        for (i = 0; i < nRecords; i++) { 
            Record rec = new Record();
            key = (3141592621L*key + 2718281829L) % 1000000007L;
            rec.intKey = (int)(key >>> 32);
            rec.strKey = Integer.toString((int)key);
            root.put(new Key(new Object[]{new Integer(rec.intKey), rec.strKey}), rec);                
        }
        db.commit();
        System.out.println("Elapsed time for inserting " + nRecords + " records: " 
                           + (System.currentTimeMillis() - start) + " milliseconds");
        
        start = System.currentTimeMillis();
        key = 1999;
        int minKey = Integer.MAX_VALUE;
        int maxKey = Integer.MIN_VALUE;
        for (i = 0; i < nRecords; i++) { 
            key = (3141592621L*key + 2718281829L) % 1000000007L;
            int intKey = (int)(key >>> 32);            
            String strKey = Integer.toString((int)key);
            Record rec = (Record)root.get(new Key(new Object[]{new Integer(intKey), strKey}));
            Assert.that(rec != null && rec.intKey == intKey && rec.strKey.equals(strKey));
            if (intKey < minKey) { 
                minKey = intKey;
            }
            if (intKey > maxKey) { 
                maxKey = intKey;
            }
        }
        System.out.println("Elapsed time for performing " + nRecords + " index searches: " 
                           + (System.currentTimeMillis() - start) + " milliseconds");
        
        start = System.currentTimeMillis();
        Iterator iterator = root.iterator(new Key(new Integer(minKey), ""), 
                                          new Key(new Integer(maxKey+1), "???"), 
                                          Index.ASCENT_ORDER);
        int n = 0;
        String prevStr = "";
        int prevInt = minKey;
        while (iterator.hasNext()) { 
            Record rec = (Record)iterator.next();
            Assert.that(rec.intKey > prevInt || rec.intKey == prevInt && rec.strKey.compareTo(prevStr) > 0);
            prevStr = rec.strKey;
            prevInt = rec.intKey;
            n += 1;
        }
        Assert.that(n == nRecords);
        
        iterator = root.iterator(new Key(new Integer(minKey), "", false), 
                                 new Key(new Integer(maxKey+1), "???", false), 
                                 Index.DESCENT_ORDER);
        n = 0;
        prevInt = maxKey+1;
        while (iterator.hasNext()) { 
            Record rec = (Record)iterator.next();
            Assert.that(rec.intKey < prevInt || rec.intKey == prevInt && rec.strKey.compareTo(prevStr) < 0);
            prevStr = rec.strKey;
            prevInt = rec.intKey;
            n += 1;
        }
        Assert.that(n == nRecords);
        System.out.println("Elapsed time for iterating through " + (nRecords*2) + " records: " 
                           + (System.currentTimeMillis() - start) + " milliseconds");
        start = System.currentTimeMillis();
        key = 1999;
        for (i = 0; i < nRecords; i++) { 
            key = (3141592621L*key + 2718281829L) % 1000000007L;
            int intKey = (int)(key >>> 32);            
            String strKey = Integer.toString((int)key);
            Record rec = (Record)root.get(new Key(new Object[]{new Integer(intKey), strKey}));
            Assert.that(rec != null && rec.intKey == intKey && rec.strKey.equals(strKey));
            root.remove(new Key(new Object[]{new Integer(intKey), strKey}), rec);
            rec.deallocate();
        }
        Assert.that(!root.iterator().hasNext());
        Assert.that(!root.iterator(null, null, Index.DESCENT_ORDER).hasNext());
        Assert.that(!root.iterator(null, null, Index.ASCENT_ORDER).hasNext());
        System.out.println("Elapsed time for deleting " + nRecords + " records: " 
                           + (System.currentTimeMillis() - start) + " milliseconds");
        db.close();
    }
}
