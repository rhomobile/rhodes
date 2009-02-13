package org.garret.perst;

import java.util.Date;

/**
 * <p>
 * Time series interface. Time series class is used for efficient
 * handling of time series data. Ussually time series contains a very large number
 * if relatively small elements which are ussually acessed in sucessive order. 
 * To avoid overhead of loading from the disk each particular time series element, 
 * this class group several subsequent time series elements together and store them 
 * as single object (block).
 * </p><p> 
 * As far as Java currently has no templates and
 * Perst need to know format of block class, it is responsibity of prgorammer
 * to create block implementation derived from TimeSeriesBlock class
 * and containing array of time series elements. Size of this array specifies
 * the size of the block.
 * </p>
 */
public interface TimeSeries extends IPersistent, IResource { 
    /**
     * Add new tick to time series
     * @param tick new time series element
     */
    void     add(TimeSeriesTick tick);    

    /**
     * Get forward iterator through all time series elements
     * This iterator doesn't support remove() method. It is not possible to modify
     * time series during iteration.
     * @return forward iterator 
     */
    Iterator iterator();

    /**
     * Get forward iterator for time series elements belonging to the specified range
     * This iterator doesn't support remove() method. It is not possible to modify
     * time series during iteration.
     * @param from inclusive time of the begging of interval, 
     * if null then take all elements from the beginning of time series
     * @param till inclusive time of the ending of interval, 
     * if null then take all elements till the end of time series
     * @return forward iterator within specified range.
     */
    Iterator iterator(Date from, Date till);

    /**
     * Get iterator through all time series elements
     * This iterator doesn't support remove() method. It is not possible to modify
     * time series during iteration.
     * @param ascent direction of iteration
     * @return iterator in specified direction
     */
    Iterator iterator(boolean ascent);

    /**
     * Get forward iterator for time series elements belonging to the specified range.
     * This iterator doesn't support remove() method. It is not possible to modify
     * time series during iteration.
     * @param from inclusive time of the begging of interval, 
     * if null then take all elements from the beginning of time series
     * @param till inclusive time of the ending of interval, 
     * if null then take all elements till the end of time series
     * @param ascent direction of iteration
     * @return iterator within specified range in specified direction
     */
    Iterator iterator(Date from, Date till, boolean ascent);

    /**
     * Get timestamp of first time series element
     * @return time of time series start
     */
    Date     getFirstTime();

    /**
     * Get timestamp of last time series element
     * @return time of time series end
     */
    Date     getLastTime();

    /**
     * Get number of elements in time series
     * @return number of elements in time series
     */
    long     countTicks();

    /**
     * Get number of elements in time series
     * @return number of elements in time series
     */
    int      size();

    /** 
     * Get tick for specified data
     * @param timestamp time series element timestamp
     * return time series element for the specified timestamp or null
     * if no such element was found
     */
    TimeSeriesTick getTick(Date timestamp);
    
    /**
     * Check if data is available in time series for the specified time
     * @param timestamp time series element timestamp
     * @return <code>true</code> if there is element in time series with such timestamp, 
     * <code>false</code> otherwise
     */
    boolean  has(Date timestamp);

    /**
     * Remove timeseries elements belonging to the specified range
     * @param from inclusive time of the begging of interval, 
     * if null then remove all elements from the beginning of time series
     * @param till inclusive time of the ending of interval, 
     * if null then remove all elements till the end of time series
     * @return number of removed elements
     */
    long remove(Date from, Date till);
}

    