package javolution.util;

import j2me.util.Collection;
import j2me.util.Map;
import j2me.util.Set;
import javolution.context.LocalContext;

/**
 * <p> This class represents a map which can be temporarily modified 
 *     without impacting other threads ({@link LocalContext locally}
 *     scoped changes).</p>
 *     
 * <p> Operation on instances of this class are completely thread-safe. 
 *     For example:
 *     public class XMLFormat {
 *         static LocalMap<Class, XMLFormat> CLASS_TO_FORMAT = new LocalMap<Class, XMLFormat>();
 *         public static void setFormat(Class forClass, XMLFormat that) {
 *             CLASS_TO_FORMAT.put(forClass, that); // No synchronization required.
 *         }
 *         public static XMLFormat getInstance(Class forClass) {
 *             return CLASS_TO_FORMAT.get(forClass); // No synchronization required.
 *         }
 *     }
 *     public void main(String[] args) {
 *         // Sets default (global settings).
 *         XMLFormat.setFormat(Foo.class, xFormat);
 *         XMLFormat.setFormat(Bar.class, yFormat);
 *     }
 *     ... // Another thread.
 *     LocalContext.enter();
 *     try { // Use of local context to avoid impacting other threads.
 *         XMLFormat.setFormat(Foo.class, zFormat);
 *         XMLFormat.getInstance(Foo.class); // Returns zFormat
 *         XMLFormat.getInstance(Bar.class); // Returns yFormat (inherited)
 *     } finally {
 *         LocalContext.exit();
 *     }
 *     getInstance(Foo.class); // Returns xFormat
 *     [/code]</p>
 * 
 * <p> <b>Note:</b> Because key-value mappings are inherited, the semantic of  
 *     {@link #remove} and {@link #clear} is slightly modified (associate
 *     <code>null</code> values instead of removing the entries).</p>
 *     
 * @author <a href="mailto:jean-marie@dautelle.com">Jean-Marie Dautelle</a>
 * @version 3.7, January 27, 2005
 */
public final class LocalMap/*<K,V>*/implements Map/*<K,V>*/{

    /**
     * Holds the fast map reference (shared map).
     */
    private final LocalContext.Reference _mapRef 
        = new LocalContext.Reference(new FastMap().setShared(true));

    /**
     * Default constructor.
     */
    public LocalMap() {
    }

    /**
     * Sets the key comparator for this local map.
     * 
     * @param keyComparator the key comparator.
     * @return <code>this</code>
     */
    public LocalMap /*<K,V>*/setKeyComparator(FastComparator/*<? super K>*/ keyComparator) {
        localMap().setKeyComparator(keyComparator);
        return this;
    }

    /**
     * Sets the value comparator for this local map.
     * 
     * @param valueComparator the value comparator.
     * @return <code>this</code>
     */
    public LocalMap/*<K,V>*/setValueComparator(FastComparator/*<? super V>*/ valueComparator) {
        localMap().setValueComparator(valueComparator);
        return this;
    }

    /**
     * Sets the default value for the specified key (typically done at 
     * initialization). 
     * 
     * @param key the key with which the specified value is to be associated.
     * @param defaultValue the default value to be associated with the 
     *        specified key.
     * @return the previous default value associated with specified key, or
     *         <code>null</code> if there was no mapping for key. A
     *         <code>null</code> return can also indicate that the map
     *         previously associated <code>null</code> with the specified key.
     * @throws NullPointerException if the key is <code>null</code>.
     */
    public Object/*{V}*/putDefault(Object/*{K}*/key, Object/*{V}*/defaultValue) {
        return (Object/*{V}*/) ((FastMap) _mapRef.getDefault()).put(key,
                defaultValue);
    }
    /**
     * Returns the number of key-value mappings in this map.
     * 
     * @return this map's size.
     */
    public int size() {
        return ((FastMap) _mapRef.get()).size();
    }

    /**
     * Indicates if this map contains no key-value mappings.
     * 
     * @return <code>true</code> if this map contains no key-value mappings;
     *         <code>false</code> otherwise.
     */
    public boolean isEmpty() {
        return ((FastMap) _mapRef.get()).isEmpty();
    }

    /**
     * Indicates if this map contains a mapping for the specified key.
     * 
     * @param key the key whose presence in this map is to be tested.
     * @return <code>true</code> if this map contains a mapping for the
     *         specified key; <code>false</code> otherwise.
     * @throws NullPointerException if the key is <code>null</code>.
     */
    public boolean containsKey(Object key) {
        return ((FastMap) _mapRef.get()).containsKey(key);
    }

    /**
     * Indicates if this map associates one or more keys to the 
     * specified value.
     * 
     * @param value the value whose presence in this map is to be tested.
     * @return <code>true</code> if this map maps one or more keys to the
     *         specified value.
     * @throws NullPointerException if the key is <code>null</code>.
     */
    public boolean containsValue(Object value) {
        return ((FastMap) _mapRef.get()).containsValue(value);
    }

    /**
     * Returns the value to which this map associates the specified key.
     * 
     * @param key the key whose associated value is to be returned.
     * @return the value to which this map maps the specified key, or
     *         <code>null</code> if there is no mapping for the key.
     * @throws NullPointerException if key is <code>null</code>.
     */
    public Object/*{V}*/get(Object key) {
        return (Object/*{V}*/) ((FastMap) _mapRef.get()).get(key);
    }

    /**
     * Associates the specified value with the specified key in this map.
     * 
     * @param key the key with which the specified value is to be associated.
     * @param value the value to be associated with the specified key.
     * @return the previous value associated with specified key, or
     *         <code>null</code> if there was no mapping for key. A
     *         <code>null</code> return can also indicate that the map
     *         previously associated <code>null</code> with the specified key.
     * @throws NullPointerException if the key is <code>null</code>.
     */
    public Object/*{V}*/put(Object/*{K}*/key, Object/*{V}*/value) {
        return (Object/*{V}*/) localMap().put(key, value);
    }

    /**
     * Copies all of the mappings from the specified map to this map.
     * 
     * @param map the mappings to be stored in this map.
     * @throws NullPointerException the specified map is <code>null</code>,
     *         or the specified map contains <code>null</code> keys.
     */
    public void putAll(Map/*<? extends K, ? extends V>*/map) {
        localMap().putAll(map);
    }

    /**
     * Removes the mapping for this key from this map if present
     * (sets the local value to <code>null</code>).
     * 
     * @param key the key whose value is set to <code>null</code>
     * @return <code>put(key, null)</code>
     * @throws NullPointerException if the key is <code>null</code>.
     */
    public Object/*{V}*/remove(Object key) {
        return put((Object/*{K}*/)key, null);
    }

    /**
     * Removes all mappings from this map (sets the local values to
     * <code>null</code>).
     */
    public void clear() {
        FastMap localMap = localMap();
        for (FastMap.Entry e = localMap.head(), end = localMap.tail(); (e = (FastMap.Entry) e.getNext()) != end;) {
            e.setValue(null);
        }
    }

    /**
     * Returns a {@link FastCollection} view of the keys contained in this map. 
     * 
     * @return a set view of the keys contained in this map
     *         (instance of {@link FastCollection}).
     */
    public Set/*<K>*/keySet() {
        return localMap().keySet();
    }

    /**
     * Returns a {@link FastCollection} view of the values contained in this
     * map. 
     * 
     * @return a collection view of the values contained in this map 
     *         (instance of {@link FastCollection}).
     */
    public Collection/*<V>*/values() {
        return localMap().values();
    }

    /**
     * Returns a {@link FastCollection} view of the mappings contained in this
     * map. 
     * 
     * @return a collection view of the mappings contained in this map
     *         (instance of {@link FastCollection}).
     */
    public Set/*<Map.Entry<K,V>>*/entrySet() {
        return localMap().entrySet();
    }

    /**
     * Returns the local map or creates one on the stack and populates 
     * it from inherited settings.
     * 
     * @return a shared fast map belonging to the current local context.
     */
    private FastMap/*<K,V>*/localMap() {
        FastMap localMap = (FastMap) _mapRef.getLocal();
        return (localMap != null) ? localMap : newLocalMap();
    }

    private FastMap newLocalMap() {
        FastMap parentMap = (FastMap) _mapRef.get();
        FastMap localMap = FastMap.newInstance(); 
        localMap.setShared(true);
        localMap.setKeyComparator(parentMap.getKeyComparator());
        localMap.setValueComparator(parentMap.getValueComparator());
        localMap.putAll(parentMap);
        _mapRef.set(localMap);
        return localMap;
    }
}
