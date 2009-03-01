package org.garret.perst;

/**
 * R2 rectangle class. This class is used in spatial index.
 */
public class RectangleR2 {
    private double top;
    private double left;
    private double bottom;
    private double right;

    /**
     * Smallest Y coordinate of the rectangle
     */
    public final double getTop() { 
        return top;
    }

    /**
     * Smallest X coordinate of the rectangle
     */
    public final double getLeft() { 
        return left;
    }

    /**
     * Greatest Y coordinate  of the rectangle
     */
    public final double getBottom() { 
        return bottom;
    }

    /**
     * Greatest X coordinate  of the rectangle
     */
     public final double getRight() { 
        return right;
    }

    /**
     * Rectangle area
     */
    public final double area() { 
        return (bottom-top)*(right-left);
    }

    /**
     * Area of covered rectangle for two sepcified rectangles
     */
    public static double joinArea(RectangleR2 a, RectangleR2 b) {
        double left = (a.left < b.left) ? a.left : b.left;
        double right = (a.right > b.right) ? a.right : b.right;
        double top = (a.top < b.top) ? a.top : b.top;
        double bottom = (a.bottom > b.bottom) ? a.bottom : b.bottom;
        return (bottom-top)*(right-left);
    }

    /**
     * Clone rectangle 
     */
    public Object clone() { 
        RectangleR2 r = new RectangleR2();
        r.top = this.top;
        r.left = this.left;
        r.bottom = this.bottom;
        r.right = this.right;
        return r;
    }

    /**
     * Create copy of the rectangle
     */
    public RectangleR2(RectangleR2 r) {
        this.top = r.top;
        this.left = r.left;
        this.bottom = r.bottom;
        this.right = r.right;
    }

    /**
     * Construct rectangle with specified coordinates
     */
    public RectangleR2(double top, double left, double bottom, double right) { 
        Assert.that(top <= bottom && left <= right);
        this.top = top;
        this.left = left;
        this.bottom = bottom;
        this.right = right;
    }

    /**
     * Default constructor for PERST
     */
    public RectangleR2() {}

    /**
     * Join two rectangles. This rectangle is updates to contain cover of this and specified rectangle.
     * @param r rectangle to be joined with this rectangle
     */
    public final void join(RectangleR2 r) { 
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
    public static RectangleR2 join(RectangleR2 a, RectangleR2 b) {
        RectangleR2 r = new RectangleR2(a);
        r.join(b);
        return r;
    }

    /**
     * Checks if this rectangle intersects with specified rectangle
     */
    public final boolean intersects(RectangleR2 r) { 
        return left <= r.right && top <= r.bottom && right >= r.left && bottom >= r.top;
    }

    /**
     * Checks if this rectangle contains the specified rectangle
     */
    public final boolean contains(RectangleR2 r) { 
        return left <= r.left && top <= r.top && right >= r.right && bottom >= r.bottom;
    }

    /**
     * Check if two rectangles are equal
     */
    public boolean equals(Object o) { 
        if (o instanceof RectangleR2) { 
            RectangleR2 r = (RectangleR2)o;
            return left == r.left && top == r.top && right == r.right && bottom == r.bottom;
        }
        return false;
    }
    
    /**
     * Hash code consists of all rectangle coordinates
     */
    public int hashCode() { 
        return (int)(Double.doubleToLongBits(top) ^ (Double.doubleToLongBits(bottom) << 1) 
                     ^ (Double.doubleToLongBits(left) << 2) ^ (Double.doubleToLongBits(right) << 3));
    }

    public String toString() { 
        return "top=" + top + ", left=" + left + ", bottom=" + bottom + ", right=" + right;
    }
}
    
    








