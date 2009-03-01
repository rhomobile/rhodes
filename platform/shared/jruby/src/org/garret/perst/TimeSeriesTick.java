package org.garret.perst;

/**
 * Interface for timeseries element.
 * You should derive your time series element from this class
 * and implement getTime method.
 */
public interface TimeSeriesTick { 
    /**
     * Get time series element timestamp
     * @return timestamp in milliseconds
     */
    long getTime();
}
    
