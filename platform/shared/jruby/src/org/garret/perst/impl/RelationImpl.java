package org.garret.perst.impl;
import  org.garret.perst.*;

public class RelationImpl extends Relation {
    public int size() {
        return link.size();
    }
    
    public void setSize(int newSize) {
        link.setSize(newSize);
    }
    
    public IPersistent get(int i) {
        return link.get(i);
    }

    public IPersistent getRaw(int i) {
        return link.getRaw(i);
    }

    public void set(int i, IPersistent obj) {
        link.set(i, obj);
    }

    public void remove(int i) {
        link.remove(i);
    }

    public void insert(int i, IPersistent obj) {
        link.insert(i, obj);
    }

    public void add(IPersistent obj) {
        link.add(obj);
    }

    public void addAll(IPersistent[] arr) {
        link.addAll(arr);
    }

    public void addAll(IPersistent[] arr, int from, int length) {
        link.addAll(arr, from, length);
    }

    public void addAll(Link anotherLink) {
        link.addAll(anotherLink);
    }

    public IPersistent[] toArray() {
        return link.toArray();
    }

    public IPersistent[] toRawArray() {
        return link.toRawArray();
    }

    public IPersistent[] toArray(IPersistent[] arr) {
        return link.toArray(arr);
    }

    public boolean contains(IPersistent obj) {
        return link.contains(obj);
    }

    public boolean containsObject(IPersistent obj) {
        return link.containsObject(obj);
    }

    public int indexOf(IPersistent obj) {
        return link.indexOf(obj);
    }
       
    public boolean containsElement(int i, IPersistent obj) {
        return link.containsElement(i, obj);
    }

    public void clear() {
        link.clear();
    }

    public Iterator iterator() {
        return link.iterator();
    }

    public void pin() { 
        link.pin();
    }
    
    public void unpin() { 
        link.unpin();
    }
    
    RelationImpl() {}

    RelationImpl(IPersistent owner) { 
        super(owner);
        link = new LinkImpl(8);
    }

    Link link;
}
