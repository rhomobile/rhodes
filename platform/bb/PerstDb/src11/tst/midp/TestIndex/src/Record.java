import org.garret.perst.*;

public class Record extends Persistent { 
    public void writeObject(IOutputStream out) { 
        out.writeLong(key);
    }

    public void readObject(IInputStream in) { 
        key = in.readLong();
    }
    
    public Record(long key) { 
        this.key = key;
    }

    public Record() {}

    long key;
}

