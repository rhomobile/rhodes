package org.garret.perst;

import java.util.Date;

/**
 * Base class for version of versioned object. All versions are kept in version history.
 */
public class Version extends PersistentResource  
{
    /**
     * Get version history containing this versioned object
     */
    public synchronized VersionHistory getVersionHistory() { 
        return history;
    }

    /**
     * Get predecessors of this version
     * @return array of predecessor versions
     */
    public synchronized Version[] getPredecessors() { 
        return (Version[])predecessors.toArray(new Version[predecessors.size()]);
    }

    /**
     * Get successors of this version
     * @return array of predecessor versions
     */     
    public synchronized Version[] getSuccessors() {
        return (Version[])successors.toArray(new Version[successors.size()]);
    }

    /**
     * Check if version is checked-in
     * @return <code>true</code> if version belongs to version history
     */
    public boolean isCheckedIn() { 
        return id != null;
    }

   /**
     * Check if version is checked-out
     * @return <code>true</code> if version is cjust created and not checked-in yet 
     * (and so belongs to version history)
     */
    public boolean isCheckedOut() { 
        return id == null;
    }

    public Object clone() { 
        return new Version();
    }

    /**
     * Create new version which will be direct successor of this version.
     * This version has to be checked-in in order to be placed in version history.
     */
    public Version newVersion() { 
        Version newVersion = (Version)clone();
        newVersion.predecessors = getStorage().createLink(1);
        newVersion.predecessors.add(this);
        newVersion.successors = getStorage().createLink(1);
        newVersion.labels = new String[0];
        newVersion.id = null;
        newVersion.oid = 0;
        newVersion.state = 0;            
        return newVersion;
    }

    /**
     * Check-in new version. This method inserts in version history version created by 
     * <code>Version.newVersion</code> or <code>VersionHistory.checkout</code> method
     */
    public void checkin() { 
        synchronized (history) { 
            Assert.that(isCheckedOut());
            for (int i = 0; i < predecessors.size(); i++) { 
                Version predecessor = (Version)predecessors.get(i);
                synchronized(predecessor) { 
                    if (i == 0) { 
                        id = predecessor.constructId();
                    }
                    predecessor.successors.add(this);
                }
            }
            date = new Date();
            history.versions.add(this);
            history.current = this;
            modify();
        }
    }

    /**
     * Make specified version predecessor of this version. 
     * This method can be used to perfrom merge of two versions (merging of version data 
     * should be done by application itself)
     * @param predecessor version to merged with
     */
    public void addPredecessor(Version predecessor) { 
        synchronized(predecessor) {
            synchronized(this) { 
                predecessors.add(predecessor);
                if (isCheckedIn()) { 
                    predecessor.successors.add(this);
                }
            }
        }
    }

    /**
     * Get date of version creation 
     * @return date when this version was created
     */
    public Date getDate() { 
        return date;
    }

    /**
     * Get labels associated with this version
     * @return array of labels assigned to this version 
     */
    public synchronized String[] getLabels() { 
        return labels;
    }

    /**
     * Add new label to this version
     * @param label label to be associated with this version
     */
    public synchronized void addLabel(String label) { 
        String[] newLabels = new String[labels.length+1];
        System.arraycopy(labels, 0, newLabels, 0, labels.length);
        newLabels[newLabels.length-1] = label;
        labels = newLabels;
        modify();
    }

    /**
     * Check if version has specified label
     * @param label version label
     */
    public synchronized boolean hasLabel(String label) { 
        for (int i = 0; i < labels.length; i++) { 
            if (labels[i].equals(label)) { 
                return true;
            }
        }
        return false;
    }
    
    /**
     * Get identifier of the version 
     * @return version identifier  automatically assigned by system
     */
    public String getId() { 
        return id;
    }

    /**
     * Constructor of the root version. All other versions should be created using 
     * <code>Version.newVersion</code> or <code>VersionHistory.checkout</code> methods
     */
    protected Version(Storage storage) { 
        super(storage);
        successors = storage.createLink(1);
        predecessors = storage.createLink(1);
        labels = new String[0];
        date = new Date();
        id = "1";
    }

    /**
     * Default constuctor. Not directly accessible.
     */
    private Version() {}


    private String constructId() { 
        int suffixPos = id.lastIndexOf('.');
        int suffix = Integer.parseInt(id.substring(suffixPos+1));
        String nextId = suffixPos < 0 
            ? Integer.toString(suffix+1) 
            : id.substring(0, suffixPos) + Integer.toString(suffix+1);
        if (successors.size() != 0) { 
            nextId += '.' + successors.size() + ".1";
        }
        return nextId;
    }

    private Link     successors;
    private Link     predecessors;
    private String[] labels;
    private Date     date;
    private String   id;
    VersionHistory   history;
}
