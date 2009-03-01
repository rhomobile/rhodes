package org.garret.perst;

/**
 * Abstract base class for time series block.
 * Progammer has to define its own block class derived from this class
 * containign array of time series elements and providing getTicks()
 * method to access this array. It is better no to initialize this array in constructor
 * (because it will be also used when block will be loaded from the disk), 
 * but check in getTicks() method that array is null, and if so - create new array.
 */
public abstract class TimeSeriesBlock extends Persistent { 
    public long timestamp;
    public int  used;
    
    public void writeObject(IOutputStream out) {
        out.writeLong(timestamp);
        out.writeInt(used);
    }

    public void readObject(IInputStream in) {
        timestamp = in.readLong();
        used = in.readInt();
    }


    /**
     * Get time series elements stored in this block.
     * @return preallocated array of time series element. Only <code>used</code>
     * items of this array actually contains time series elements.
     * But all array items should be not null and conain referen to Tick object.
     */
    public abstract TimeSeriesTick[] getTicks();
}

