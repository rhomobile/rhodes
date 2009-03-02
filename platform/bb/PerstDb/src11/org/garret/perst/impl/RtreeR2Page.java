package org.garret.perst.impl;

import org.garret.perst.*;

public class RtreeR2Page extends Persistent { 
    static final int card = (Page.pageSize-ObjectHeader.sizeof-4*3)/(8*4+4);
    static final int minFill = card/2;

    int           n;
    RectangleR2[] b;
    Link          branch;

    public void writeObject(IOutputStream out) {
        out.writeInt(n);
        for (int i = 0; i < card; i++) { 
            out.writeDouble(b[i].getTop());
            out.writeDouble(b[i].getLeft());
            out.writeDouble(b[i].getBottom());
            out.writeDouble(b[i].getRight());
        }
        out.writeLink(branch);
    }

    public void readObject(IInputStream in) {
        n = in.readInt();
        b = new RectangleR2[card];
        for (int i = 0; i < card; i++) { 
            b[i] = new RectangleR2(in.readDouble(), in.readDouble(), in.readDouble(), in.readDouble());
        }
        branch = in.readLink();
    }

    RtreeR2Page(Storage storage, IPersistent obj, RectangleR2 r) {
        branch = storage.createLink(card);
        branch.setSize(card);
        b = new RectangleR2[card]; 
        setBranch(0, new RectangleR2(r), obj);
        n = 1;
        for (int i = 1; i < card; i++) { 
            b[i] = new RectangleR2();
        }        
    }
    
    RtreeR2Page(Storage storage, RtreeR2Page root, RtreeR2Page p) { 
        branch = storage.createLink(card);
        branch.setSize(card);
        b = new RectangleR2[card]; 
        n = 2;
        setBranch(0, root.cover(), root);
        setBranch(1, p.cover(), p);
        for (int i = 2; i < card; i++) { 
            b[i] = new RectangleR2();
        }        
    }

    public RtreeR2Page() {}

    RtreeR2Page insert(Storage storage, RectangleR2 r, IPersistent obj, int level) {
        modify();
        if (--level != 0) { 
            // not leaf page
            int i, mini = 0;
            double minIncr = Double.MAX_VALUE;
            double minArea = Double.MAX_VALUE;
            for (i = 0; i < n; i++) { 
                double area = b[i].area();
                double incr = RectangleR2.joinArea(b[i], r) - area;
                if (incr < minIncr) { 
                    minIncr = incr;
                    minArea = area;
                    mini = i;
                } else if (incr == minIncr && area < minArea) { 
                    minArea = area;
                    mini = i;
                }                    
            }
            RtreeR2Page p = (RtreeR2Page)branch.get(mini);
            RtreeR2Page q = p.insert(storage, r, obj, level);
            if (q == null) { 
                // child was not split
                b[mini].join(r);
                return null;
            } else { 
                // child was split
                setBranch(mini, p.cover(),  p);
                return addBranch(storage, q.cover(), q);
            }
        } else { 
            return addBranch(storage, new RectangleR2(r), obj);
        }
    }

    int remove(RectangleR2 r, IPersistent obj, int level, ArrayList reinsertList) {
        if (--level != 0) { 
            for (int i = 0; i < n; i++) { 
                if (r.intersects(b[i])) { 
                    RtreeR2Page pg = (RtreeR2Page)branch.get(i);
                    int reinsertLevel = pg.remove(r, obj, level, reinsertList);
                    if (reinsertLevel >= 0) { 
                        if (pg.n >= minFill) { 
                            setBranch(i, pg.cover(), pg);
                            modify();
                        } else { 
                            // not enough entries in child
                            reinsertList.add(pg);
                            reinsertLevel = level - 1;
                            removeBranch(i);
                        }
                        return reinsertLevel;
                    }
                }
            }
        } else {
            for (int i = 0; i < n; i++) { 
                if (branch.containsElement(i, obj)) { 
                    removeBranch(i);
                    return 0;
                }
            }
        }
        return -1;        
    }


    void find(RectangleR2 r, ArrayList result, int level) {
        if (--level != 0) { /* this is an internal node in the tree */
            for (int i = 0; i < n; i++) { 
                if (r.intersects(b[i])) {
                    ((RtreeR2Page)branch.get(i)).find(r, result, level); 
                }
            }
        } else { /* this is a leaf node */
            for (int i = 0; i < n; i++) { 
                if (r.intersects(b[i])) { 
                    result.add(branch.get(i));
                }
            }
        }
    }

    void purge(int level) {
        if (--level != 0) { /* this is an internal node in the tree */
            for (int i = 0; i < n; i++) { 
                ((RtreeR2Page)branch.get(i)).purge(level);
            }
        }
        deallocate();
    }
    
    final void setBranch(int i, RectangleR2 r, IPersistent obj) { 
        b[i] = r;
        branch.set(i, obj);
    }

    final void removeBranch(int i) {
        n -= 1;
        System.arraycopy(b, i+1, b, i, n-i);
        branch.remove(i);
        branch.setSize(card);
        modify();
    }

    final RtreeR2Page addBranch(Storage storage, RectangleR2 r, IPersistent obj) { 
        if (n < card) { 
            setBranch(n++, r, obj);
            return null;
        } else { 
            return splitPage(storage, r, obj);
        }
    }

    final RtreeR2Page splitPage(Storage storage, RectangleR2 r, IPersistent obj) { 
        int i, j, seed0 = 0, seed1 = 0;
        double[] rectArea = new double[card+1];
        double   waste;
        double   worstWaste = Double.MIN_VALUE;
        //
        // As the seeds for the two groups, find two rectangles which waste 
        // the most area if covered by a single rectangle.
        //
        rectArea[0] = r.area();
        for (i = 0; i < card; i++) { 
            rectArea[i+1] = b[i].area();
        }
        RectangleR2 bp = r;
        for (i = 0; i < card; i++) { 
            for (j = i+1; j <= card; j++) { 
                waste = RectangleR2.joinArea(bp, b[j-1]) - rectArea[i] - rectArea[j];
                if (waste > worstWaste) {
                    worstWaste = waste;
                    seed0 = i;
                    seed1 = j;
                }
            }
            bp = b[i];
        }       
        byte[] taken = new byte[card];
        RectangleR2 group0, group1;
        double      groupArea0, groupArea1;
        int         groupCard0, groupCard1;
        RtreeR2Page pg;

        taken[seed1-1] = 2;
        group1 = new RectangleR2(b[seed1-1]);

        if (seed0 == 0) { 
            group0 = new RectangleR2(r);
            pg = new RtreeR2Page(storage, obj, r);
        } else { 
            group0 = new RectangleR2(b[seed0-1]);
            pg = new RtreeR2Page(storage, branch.getRaw(seed0-1), group0);
            setBranch(seed0-1, r, obj);
        }
        groupCard0 = groupCard1 = 1;
        groupArea0 = rectArea[seed0];
        groupArea1 = rectArea[seed1];
        //
        // Split remaining rectangles between two groups.
        // The one chosen is the one with the greatest difference in area 
        // expansion depending on which group - the rect most strongly 
        // attracted to one group and repelled from the other.
        //
        while (groupCard0 + groupCard1 < card + 1 
               && groupCard0 < card + 1 - minFill
               && groupCard1 < card + 1 - minFill)
        {
            int betterGroup = -1, chosen = -1;
            double biggestDiff = -1;
            for (i = 0; i < card; i++) { 
                if (taken[i] == 0) { 
                    double diff = (RectangleR2.joinArea(group0, b[i]) - groupArea0)
                              - (RectangleR2.joinArea(group1, b[i]) - groupArea1);
                    if (diff > biggestDiff || -diff > biggestDiff) { 
                        chosen = i;
                        if (diff < 0) { 
                            betterGroup = 0;
                            biggestDiff = -diff;
                        } else { 
                            betterGroup = 1;
                            biggestDiff = diff;
                        }
                    }
                }
            }
            Assert.that(chosen >= 0);
            if (betterGroup == 0) { 
                group0.join(b[chosen]);
                groupArea0 = group0.area();
                taken[chosen] = 1;
                pg.setBranch(groupCard0++, b[chosen], branch.getRaw(chosen));
            } else {
                groupCard1 += 1;
                group1.join(b[chosen]);
                groupArea1 = group1.area();
                taken[chosen] = 2;
            }
        }
        //
        // If one group gets too full, then remaining rectangle are
        // split between two groups in such way to balance cards of two groups.
        //
        if (groupCard0 + groupCard1 < card + 1) { 
            for (i = 0; i < card; i++) { 
                if (taken[i] == 0) { 
                    if (groupCard0 >= groupCard1) { 
                        taken[i] = 2;
                        groupCard1 += 1;
                    } else { 
                        taken[i] = 1;
                        pg.setBranch(groupCard0++, b[i], branch.getRaw(i));               
                    }
                }
            }
        }
        pg.n = groupCard0;
        n = groupCard1;
        for (i = 0, j = 0; i < groupCard1; j++) { 
            if (taken[j] == 2) {
                setBranch(i++, b[j], branch.getRaw(j));
            }
        }
        return pg;
    }   

    final RectangleR2 cover() {
        RectangleR2 r = new RectangleR2(b[0]);
        for (int i = 1; i < n; i++) { 
            r.join(b[i]);
        }
        return r;
    }
}
