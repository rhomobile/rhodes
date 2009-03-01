package org.garret.perst;

/**
 * There are two kind of links in Perst: embedded and implemented as standalone object (collection).
 * This interface extends Link interface with getOwner/setOwner methods allowing to bind link with 
 * its container. Such binding is needed for embedded link implementation to make modify() in link methods
 * work properly. This interface is needed mostly if you are going to provide you own implementation of embedded
 * links.
 */
public interface EmbeddedLink extends Link 
{
    /**
     * Set container object for this embedded link
     * @param obj container object
     */
    void setOwner(IPersistent obj);    

    /**
     * Get container object for this embedded link
     * @return container object
     */
    IPersistent getOwner();
}