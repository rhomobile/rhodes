package org.garret.perst.impl;

import org.garret.perst.*;
import java.util.*;

public class PTrie extends PersistentResource implements PatriciaTrie 
{ 
    private PTrieNode rootZero;
    private PTrieNode rootOne;
    private int       count;

    public void writeObject(IOutputStream out) {
        out.writeObject(rootZero);
        out.writeObject(rootOne);
        out.writeInt(count);
    }

    public void readObject(IInputStream in) {
        rootZero = (PTrieNode)in.readObject();
        rootOne  = (PTrieNode)in.readObject();
        count = in.readInt();
    }

    public ArrayList elements() { 
        ArrayList list = new ArrayList();
        fill(list, rootZero);
        fill(list, rootOne);
        return list;
    }

    public int size() { 
        return count;
    }

    public IPersistent[] toArray() { 
        return (IPersistent[])elements().toArray();
    }

    public IPersistent[] toArray(IPersistent[] arr) { 
        return (IPersistent[])elements().toArray(arr);
    }

    public Iterator iterator() { 
        return elements().iterator();
    }

    private static void fill(ArrayList list, PTrieNode node) { 
        if (node != null) {
            list.add(node.obj);
            fill(list, node.childZero);
            fill(list, node.childOne);
        }
    }

    private static int firstBit(long key, int keyLength)
    {
        return (int)(key >>> (keyLength - 1)) & 1;
    }

    private static int getCommonPartLength(long keyA, int keyLengthA, long keyB, int keyLengthB)
    {
        if (keyLengthA > keyLengthB) {
            keyA >>>= keyLengthA - keyLengthB;
            keyLengthA = keyLengthB;
        } else {
            keyB >>>= keyLengthB - keyLengthA;
            keyLengthB = keyLengthA;
        }
        long diff = keyA ^ keyB;
        
        int count = 0;
        while (diff != 0) {
            diff >>>= 1;
            count += 1;
        }
        return keyLengthA - count;
    }

    public IPersistent add(PatriciaTrieKey key, IPersistent obj) 
    { 
        modify();
        count += 1;

        if (firstBit(key.mask, key.length) == 1) {
            if (rootOne != null) { 
                return rootOne.add(key.mask, key.length, obj);
            } else { 
                rootOne = new PTrieNode(key.mask, key.length, obj);
                return null;
            }
        } else { 
            if (rootZero != null) { 
                return rootZero.add(key.mask, key.length, obj);
            } else { 
                rootZero = new PTrieNode(key.mask, key.length, obj);
                return null;
            }
        }            
    }
    
    public IPersistent findBestMatch(PatriciaTrieKey key) 
    {
        if (firstBit(key.mask, key.length) == 1) {
            if (rootOne != null) { 
                return rootOne.findBestMatch(key.mask, key.length);
            } 
        } else { 
            if (rootZero != null) { 
                return rootZero.findBestMatch(key.mask, key.length);
            } 
        }
        return null;
    }
    

    public IPersistent findExactMatch(PatriciaTrieKey key) 
    {
        if (firstBit(key.mask, key.length) == 1) {
            if (rootOne != null) { 
                return rootOne.findExactMatch(key.mask, key.length);
            } 
        } else { 
            if (rootZero != null) { 
                return rootZero.findExactMatch(key.mask, key.length);
            } 
        }
        return null;
    }
    
    public IPersistent remove(PatriciaTrieKey key) 
    { 
        if (firstBit(key.mask, key.length) == 1) {
            if (rootOne != null) { 
                IPersistent obj = rootOne.remove(key.mask, key.length);
                if (obj != null) { 
                    modify();
                    count -= 1;
                    if (rootOne.isNotUsed()) { 
                        rootOne.deallocate();
                        rootOne = null;
                    }
                    return obj;
                }
            }  
        } else { 
            if (rootZero != null) { 
                IPersistent obj = rootZero.remove(key.mask, key.length);
                if (obj != null) { 
                    modify();
                    count -= 1;
                    if (rootZero.isNotUsed()) { 
                        rootZero.deallocate();
                        rootZero = null;
                    }
                    return obj;
                }
            }  
        }
        return null;
    }

    public void clear() 
    {
        if (rootOne != null) { 
            rootOne.deallocate();
            rootOne = null;
        }
        if (rootZero != null) { 
            rootZero.deallocate();
            rootZero = null;
        }
        count = 0;
    }

    public static class PTrieNode extends Persistent 
    {
        long        key;
        int         keyLength;
        IPersistent obj;
        PTrieNode   childZero;
        PTrieNode   childOne;

        public void writeObject(IOutputStream out) {
            out.writeLong(key);
            out.writeInt(keyLength);
            out.writeObject(obj);
            out.writeObject(childZero);
            out.writeObject(childOne);
        }
        
        public void readObject(IInputStream in) {
            key = in.readLong();
            keyLength = in.readInt();
            obj = in.readObject();
            childZero = (PTrieNode)in.readObject();
            childOne  = (PTrieNode)in.readObject();
        }


        PTrieNode(long key, int keyLength, IPersistent obj)
        {
            this.obj = obj;
            this.key = key;
            this.keyLength = keyLength; 
        }

        public PTrieNode() {}

        IPersistent add(long key, int keyLength, IPersistent obj) 
        {
            if (key == this.key && keyLength == this.keyLength) {
                modify();
                IPersistent prevObj = this.obj;
                this.obj = obj;
                return prevObj;
            }
            int keyLengthCommon = getCommonPartLength(key, keyLength, this.key, this.keyLength);
            int keyLengthDiff = this.keyLength - keyLengthCommon;
            long keyCommon = key >>> (keyLength - keyLengthCommon);
            long keyDiff = this.key - (keyCommon << keyLengthDiff);
            if (keyLengthDiff > 0) {
                modify();
                PTrieNode newNode = new PTrieNode(keyDiff, keyLengthDiff, this.obj);
                newNode.childZero = childZero;
                newNode.childOne = childOne;
                
                this.key = keyCommon;
                this.keyLength = keyLengthCommon;
                this.obj = null;
                
                if (firstBit(keyDiff, keyLengthDiff) == 1) {
                    childZero = null;
                    childOne = newNode;
                } else {
                    childZero = newNode;
                    childOne = null;
                }
            }
            
            if (keyLength > keyLengthCommon) {
                keyLengthDiff = keyLength - keyLengthCommon;
                keyDiff = key - (keyCommon << keyLengthDiff);
                
                if (firstBit(keyDiff, keyLengthDiff) == 1) {
                    if (childOne != null) {
                        return childOne.add(keyDiff, keyLengthDiff, obj);
                    } else { 
                        modify();
                        childOne = new PTrieNode(keyDiff, keyLengthDiff, obj);
                        return null;
                    }
                } else {
                    if (childZero != null) { 
                        return childZero.add(keyDiff, keyLengthDiff, obj);
                    } else { 
                        modify();
                        childZero = new PTrieNode(keyDiff, keyLengthDiff, obj);
                        return null;
                    }
                }
            } else {
                IPersistent prevObj = this.obj;
                this.obj = obj;
                return prevObj;
            }            
        }
    
        
        IPersistent findBestMatch(long key, int keyLength) 
        {             
            if (keyLength > this.keyLength) { 
                int keyLengthCommon = getCommonPartLength(key, keyLength, this.key, this.keyLength);
                int keyLengthDiff = keyLength - keyLengthCommon;
                long keyCommon = key >>> keyLengthDiff;
                long keyDiff = key - (keyCommon << keyLengthDiff);

                if (firstBit(keyDiff, keyLengthDiff) == 1) {
                    if (childOne != null) { 
                        return childOne.findBestMatch(keyDiff, keyLengthDiff);
                    }
                } else {
                    if (childZero != null) { 
                        return childZero.findBestMatch(keyDiff, keyLengthDiff);
                    }
                }
            }
            return obj;
        }
				
        IPersistent findExactMatch(long key, int keyLength) 
        {             
            if (keyLength >= this.keyLength) { 
                if (key == this.key && keyLength == this.keyLength) { 
                    return obj;
                } else { 
                    int keyLengthCommon = getCommonPartLength(key, keyLength, this.key, this.keyLength);
                    if (keyLengthCommon == this.keyLength) { 
                        int keyLengthDiff = keyLength - keyLengthCommon;
                        long keyCommon = key >>> keyLengthDiff;
                        long keyDiff = key - (keyCommon << keyLengthDiff);
                        
                        if (firstBit(keyDiff, keyLengthDiff) == 1) {
                            if (childOne != null) { 
                                return childOne.findBestMatch(keyDiff, keyLengthDiff);
                            }
                        } else {
                            if (childZero != null) { 
                                return childZero.findBestMatch(keyDiff, keyLengthDiff);
                            } 
                        }
                    }
                }
            }
            return null;
        }		

        boolean isNotUsed() { 
            return obj == null && childOne == null && childZero == null;
        }

        IPersistent remove(long key, int keyLength) 
        {             
            if (keyLength >= this.keyLength) { 
                if (key == this.key && keyLength == this.keyLength) { 
                    IPersistent obj = this.obj;
                    this.obj = null;
                    return obj;
                } else { 
                    int keyLengthCommon = getCommonPartLength(key, keyLength, this.key, this.keyLength);
                    int keyLengthDiff = keyLength - keyLengthCommon;
                    long keyCommon = key >>> keyLengthDiff;
                    long keyDiff = key - (keyCommon << keyLengthDiff);
                    
                    if (firstBit(keyDiff, keyLengthDiff) == 1) {
                        if (childOne != null) { 
                            IPersistent obj = childOne.findBestMatch(keyDiff, keyLengthDiff);
                            if (obj != null) { 
                                if (childOne.isNotUsed()) {
                                    modify();
                                    childOne.deallocate();
                                    childOne = null;
                                }
                                return obj;                                    
                            }
                        }
                    } else {
                        if (childZero != null) { 
                            IPersistent obj = childZero.findBestMatch(keyDiff, keyLengthDiff);
                            if (obj != null) { 
                                if (childZero.isNotUsed()) { 
                                    modify();
                                    childZero.deallocate();
                                    childZero = null;
                                }
                                return obj;                                    
                            }
                        } 
                    }
                }
            }
            return null;
        }		

        public void deallocate() 
        {
            if (childOne != null) { 
                childOne.deallocate();
            }
            if (childZero != null) { 
                childZero.deallocate();
            }
            super.deallocate();
        }
    }
}