package org.garret.perst;

/**
 * Double linked list element.
 */

public class L2ListElem extends PersistentResource { 
    protected L2ListElem next;
    protected L2ListElem prev;
    
    public L2ListElem() { 
        next = prev = this;
    }


    /**
     * Get next list element. 
     * Been call for the last list element, this method will return first element of the list 
     * or list header
     */
    public L2ListElem getNext() { 
        return next;
    }

    /**
     * Get previous list element. 
     * Been call for the first list element, this method will return last element of the list 
     * or list header
     */
    public L2ListElem getPrev() { 
        return prev;
    }

    /**
     * Make list empty. 
     * This method should be applied to list header. 
     */
    public void prune() { 
        modify();
        next = prev = this;
    }

    /**
     * Link specified element in the list after this element
     * @param elem element to be linked in the list after this elemen
     */
    public void linkAfter(L2ListElem elem) { 
        modify();
        next.modify();
        elem.modify();
        elem.next = next;
        elem.prev = this;
        next.prev = elem;
        next = elem;
    }

    /**
     * Link specified element in the list before this element
     * @param elem element to be linked in the list before this elemen
     */
    public void linkBefore(L2ListElem elem) { 
        modify();
        prev.modify();
        elem.modify();
        elem.next = this;
        elem.prev = prev;
        prev.next = elem;
        prev = elem;
    }

    /**
     * Remove element from the list
     */
    public void unlink() { 
        next.modify();
        prev.modify();
        next.prev = prev;
        prev.next = next;
    }
}