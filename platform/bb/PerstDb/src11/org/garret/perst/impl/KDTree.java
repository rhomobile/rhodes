package org.garret.perst.impl;

import org.garret.perst.*;
import java.util.*;

public class KDTree extends PersistentResource implements MultidimensionalIndex {

    KDTreeNode root;
    int        nMembers;
    int        height;
    MultidimensionalComparator comparator;

    public void writeObject(IOutputStream out) {
        out.writeObject(root);
        out.writeInt(nMembers);
        out.writeInt(height);
        out.writeObject(comparator);
    }

    public void readObject(IInputStream in) {
        root = (KDTreeNode)in.readObject();
        nMembers = in.readInt();
        height = in.readInt();
        comparator = (MultidimensionalComparator)in.readObject();
    }

    public KDTree() {} 

    KDTree(Storage storage, MultidimensionalComparator comparator) { 
        super(storage);
        this.comparator = comparator;
    }

    public MultidimensionalComparator getComparator() { 
        return comparator;
    }

    static final int OK = 0;
    static final int NOT_FOUND = 1;
    static final int TRUNCATE  = 2;
    
    static class KDTreeNode extends Persistent 
    {
        KDTreeNode  left;
        KDTreeNode  right;
        IPersistent obj;
        boolean     deleted;
        
        KDTreeNode(IPersistent obj) { 
            this.obj = obj;
        }        

        public KDTreeNode() {}

        public void writeObject(IOutputStream out) { 
            out.writeObject(left);
            out.writeObject(right);
            out.writeObject(obj);
            out.writeBoolean(deleted);
        }
        
        public void readObject(IInputStream in) {
            left = (KDTreeNode)in.readObject();
            right = (KDTreeNode)in.readObject();
            obj = (Persistent)in.readObject();
            deleted = in.readBoolean();
        }

        public void load() {
            super.load();
            obj.load();
        }

        public boolean recursiveLoading() {
            return false;
        }

        int insert(IPersistent ins, MultidimensionalComparator comparator, int level) 
        { 
            load();
            int diff = comparator.compare(ins, obj, level % comparator.getNumberOfDimensions());
            if (diff == MultidimensionalComparator.EQ && deleted) { 
                obj.deallocate();
                modify();
                obj = ins;
                deleted = false;
                return level;
            } else if (diff != MultidimensionalComparator.GT) { 
                if (left == null) { 
                    modify();
                    left = new KDTreeNode(ins);
                    return level+1;
                } else { 
                    return left.insert(ins, comparator, level + 1);
                }
            } else { 
                if (right == null) { 
                    modify();
                    right = new KDTreeNode(ins);
                    return level+1;
                } else { 
                    return right.insert(ins, comparator, level + 1);
                }
            }
        }
        
        int remove(IPersistent rem, MultidimensionalComparator comparator, int level) 
        { 
            load();
            if (obj == rem) { 
                if (left == null && right == null) { 
                    deallocate();
                    return TRUNCATE;
                } else {
                    modify();
                    obj = comparator.cloneField(obj, level % comparator.getNumberOfDimensions());
                    deleted = true;
                    return OK;
                }  
            }
            int diff = comparator.compare(rem, obj, level % comparator.getNumberOfDimensions());
            if (diff != MultidimensionalComparator.GT && left != null) {
                int result = left.remove(rem, comparator, level + 1);
                if (result == TRUNCATE) { 
                    modify();
                    left = null;
                    return OK;
                } else if (result == OK) { 
                    return OK;
                }
            } 
            if (diff != MultidimensionalComparator.LT && right != null) { 
                int result = right.remove(rem, comparator, level + 1);
                if (result == TRUNCATE) { 
                    modify();
                    right = null;
                    return OK;
                } else if (result == OK) { 
                    return OK;
                }
            }
            return NOT_FOUND;
        }
                
        public void deallocate() { 
            load();
            if (deleted) { 
                obj.deallocate();
            }
            if (left != null) { 
                left.deallocate();
            }
            if (right != null) { 
                right.deallocate();
            }
            super.deallocate();
        }
    }

    public void optimize() { 
        Iterator itr = iterator();
        int n = nMembers;
        IPersistent[] members = new IPersistent[n];
        for (int i = 0; i < n; i++) { 
            members[i] = (IPersistent)itr.next();
        }
        Random rnd = new Random(System.currentTimeMillis());
        for (int i = 0; i < n; i++) { 
            int j = (rnd.nextInt() & 0x7FFFFFFF) % n;
            IPersistent tmp = members[j];
            members[j] = members[i];
            members[i] = tmp;
        }
        clear();
        for (int i = 0; i < n; i++) { 
            add(members[i]);
        }
    }           

    public boolean add(IPersistent obj) 
    { 
        modify();
        if (root == null) {
            root = new KDTreeNode(obj);
            height = 1;
        } else {  
            int level = root.insert(obj, comparator, 0);
            if (level >= height) { 
                height = level+1;
            }
        }
        nMembers += 1;
        return true;
    }

    public boolean remove(IPersistent obj) 
    {
        if (root == null) { 
            return false;
        }
        int result = root.remove(obj, comparator, 0);
        if (result == NOT_FOUND) { 
            return false;
        } 
        modify();
        if (result == TRUNCATE) { 
            root = null;
        }
        nMembers -= 1;
        return true;
    }

    public Iterator iterator() { 
        return iterator(null, null);
    }

    public Iterator iterator(Object pattern) { 
        return iterator(pattern, pattern);
    }

    public Iterator iterator(Object low, Object high) { 
        return new KDTreeIterator(low, high);
    }

    public IPersistent[] queryByExample(Object pattern) { 
        return queryByExample(pattern, pattern);
    }

    public IPersistent[] queryByExample(Object low, Object high) { 
        Iterator i = iterator(low, high);
        ArrayList list = new ArrayList();
        while (i.hasNext()) { 
            list.add(i.next());
        }
        return (IPersistent[])list.toArray(new IPersistent[list.size()]);
    }

    public int size() { 
        return nMembers;
    }

    public int getHeight() { 
        return height;
    }

    public void clear() {
        if (root != null) { 
            root.deallocate();
            modify();
            root = null;
            nMembers = 0;
            height = 0;
        }
    }

    public boolean contains(IPersistent member) {
        Iterator i = iterator(member);
        while (i.hasNext()) { 
            if (i.next() == member) { 
                return true;
            }
        }
        return false;
    } 

    public void deallocate() {
        if (root != null) { 
            root.deallocate();
        }
        super.deallocate();
    }

    int compareAllComponents(Object pattern, IPersistent obj) 
    { 
        int n = comparator.getNumberOfDimensions();
        int result = MultidimensionalComparator.EQ;
        for (int i = 0; i < n; i++) { 
            int diff = comparator.compare(pattern, obj, i);
            if (diff == MultidimensionalComparator.RIGHT_UNDEFINED) { 
                return diff;
            } else if (diff == MultidimensionalComparator.LT) { 
                if (result == MultidimensionalComparator.GT) { 
                    return MultidimensionalComparator.NE;
                } else { 
                    result = MultidimensionalComparator.LT;
                }
            } else if (diff == MultidimensionalComparator.GT) { 
                if (result == MultidimensionalComparator.LT) { 
                    return MultidimensionalComparator.NE;
                } else { 
                    result = MultidimensionalComparator.GT;
                }
            }
        }
        return result;
    }
                

    public class KDTreeIterator extends Iterator 
    { 
        KDTreeNode[] stack;
        int          sp;
        int          nDims;
        Object       high;
        Object       low;
        KDTreeNode   curr;
        KDTreeNode   next;
        int          currLevel;

        
        KDTreeIterator(Object low, Object high) { 
            this.low = low;
            this.high = high;
            nDims = comparator.getNumberOfDimensions();
            stack = new KDTreeNode[height+1];
            getMin(root);
        }
        
        public int getLevel() {
            return currLevel;
        }

        private boolean getMin(KDTreeNode node) { 
            if (node != null) { 
                while (true) { 
                    node.load();
                    stack[sp++] = node;
                    int diff = low == null 
                        ? MultidimensionalComparator.LEFT_UNDEFINED 
                        : comparator.compare(low, node.obj, (sp-1) % nDims);
                    if (diff != MultidimensionalComparator.GT && node.left != null) { 
                        node = node.left;
                    } else { 
                        return true;
                    }
                }
            }                         
            return false;
        }

        public boolean hasNext() {
            if (next != null) { 
                return true;
            }
            while (sp != 0) { 
                KDTreeNode node = stack[--sp];                    
                if (node != null) { 
                    if (!node.deleted) { 
                        int result;
                        if ((low == null 
                             || (result = compareAllComponents(low, node.obj)) == MultidimensionalComparator.LT 
                             || result == MultidimensionalComparator.EQ)
                            && (high == null 
                                || (result = compareAllComponents(high, node.obj)) == MultidimensionalComparator.GT 
                                || result == MultidimensionalComparator.EQ))
                        {
                            next = node;
                            currLevel = sp;
                        }
                    }
                    if (node.right != null 
                        && (high == null 
                            || comparator.compare(high, node.obj, sp % nDims) != MultidimensionalComparator.LT)) 
                    { 
                        stack[sp++] = null;
                        if (!getMin(node.right)) { 
                            sp -= 1;
                        }
                    }
                    if (next != null) { 
                        return true;
                    }
                }
            }
            return false;
        }                                

        public Object next() { 
            if (!hasNext()) { 
                throw new NoSuchElementException();
            }
            curr = next;
            next = null;
            return curr.obj;
        }
        
        public int nextOid() { 
            return ((IPersistent)next()).getOid();
        }
        
        public void remove() { 
            if (curr == null) { 
                throw new IllegalStateException();
            }
            curr.modify();
            curr.obj = comparator.cloneField(curr.obj, currLevel % nDims);
            curr.deleted = true;
            curr = null;
        }
    }
}

