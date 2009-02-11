package org.garret.perst;

/**
 * Rectangle with integer cooordinates. This class is used in spatial index.
 */
public class Rectangle {
    private int top;
    private int left;
    private int bottom;
    private int right;

    /**
     * Smallest Y coordinate of the rectangle
     */
    public final int getTop() { 
        return top;
    }

    /**
     * Smallest X coordinate of the rectangle
     */
    public final int getLeft() { 
        return left;
    }

    /**
     * Greatest Y coordinate  of the rectangle
     */
    public final int getBottom() { 
        return bottom;
    }

    /**
     * Greatest X coordinate  of the rectangle
     */
     public final int getRight() { 
        return right;
    }

    /**
     * Rectangle area
     */
    public final long area() { 
        return (long)(bottom-top)*(right-left);
    }

    /**
     * Area of covered rectangle for two sepcified rectangles
     */
    public static long joinArea(Rectangle a, Rectangle b) {
        int left = (a.left < b.left) ? a.left : b.left;
        int right = (a.right > b.right) ? a.right : b.right;
        int top = (a.top < b.top) ? a.top : b.top;
        int bottom = (a.bottom > b.bottom) ? a.bottom : b.bottom;
        return (long)(bottom-top)*(right-left);
    }

    /**
     * Clone rectangle 
     */
    public Object clone() { 
        Rectangle r = new Rectangle();
        r.top = this.top;
        r.left = this.left;
        r.bottom = this.bottom;
        r.right = this.right;
        return r;
    }

    /**
     * Create copy of the rectangle
     */
    public Rectangle(Rectangle r) {
        this.top = r.top;
        this.left = r.left;
        this.bottom = r.bottom;
        this.right = r.right;
    }

    /**
     * Construct rectangle with specified coordinates
     */
    public Rectangle(int top, int left, int bottom, int right) { 
        Assert.that(top <= bottom && left <= right);
        this.top = top;
        this.left = left;
        this.bottom = bottom;
        this.right = right;
    }

    /**
     * Default constructor for PERST
     */
    public Rectangle() {}

    /**
     * Join two rectangles. This rectangle is updates to contain cover of this and specified rectangle.
     * @param r rectangle to be joined with this rectangle
     */
    public final void join(Rectangle r) { 
        if (left > r.left) { 
            left = r.left;
        }
        if (right < r.right) { 
            right = r.right;
        }
        if (top > r.top) { 
            top = r.top;
        }
        if (bottom < r.bottom) { 
            bottom = r.bottom;
        }
    }
    

    /**
     * Non destructive join of two rectangles. 
     * @param a first joined rectangle
     * @param b second joined rectangle
     * @return rectangle containing cover of these two rectangles
     */
    public static Rectangle join(Rectangle a, Rectangle b) {
        Rectangle r = new Rectangle(a);
        r.join(b);
        return r;
    }

    /**
     * Checks if this rectangle intersects with specified rectangle
     */
    public final boolean intersects(Rectangle r) { 
        return left <= r.right && top <= r.bottom && right >= r.left && bottom >= r.top;
    }

    /**
     * Checks if this rectangle contains the specified rectangle
     */
    public final boolean contains(Rectangle r) { 
        return left <= r.left && top <= r.top && right >= r.right && bottom >= r.bottom;
    }

    /**
     * Check if two rectangles are equal
     */
    public boolean equals(Object o) { 
        if (o instanceof Rectangle) { 
            Rectangle r = (Rectangle)o;
            return left == r.left && top == r.top && right == r.right && bottom == r.bottom;
        }
        return false;
    }
    
    /**
     * Hash code consists of all rectangle coordinates
     */
    public int hashCode() { 
        return top ^ (bottom << 1) ^ (left << 2) ^ (right << 3);
    }

    public String toString() { 
        return "top=" + top + ", left=" + left + ", bottom=" + bottom + ", right=" + right;
    }
}
    
    








