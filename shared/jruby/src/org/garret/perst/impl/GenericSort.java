package org.garret.perst.impl;

public class GenericSort { 
    static void sort(GenericSortArray arr) { 
        sort1(arr, 0, arr.size());
    }


    private static void sort1(GenericSortArray x, int off, int len) {
        // Insertion sort on smallest arrays
        if (len < 7) {
            for (int i=off; i<len+off; i++) {
                for (int j=i; j>off && x.compare(j-1, j) > 0; j--) {
                    x.swap(j, j-1);
                }
            }
            return;
        }

        // Choose a partition element, v
        int m = off + (len >> 1);       // Small arrays, middle element
        if (len > 7) {
            int l = off;
            int n = off + len - 1;
            if (len > 40) {        // Big arrays, pseudomedian of 9
                int s = len/8;
                l = med3(x, l,     l+s, l+2*s);
                m = med3(x, m-s,   m,   m+s);
                n = med3(x, n-2*s, n-s, n);
            }
            m = med3(x, l, m, n); // Mid-size, med of 3
        }
        // Establish Invariant: v* (<v)* (>v)* v*
        int a = off, b = a, c = off + len - 1, d = c, diff;
        while(true) {
            while (b <= c && (diff = x.compare(b, m)) <= 0) {
                if (diff == 0) {
                    x.swap(a++, b);
                }
                b++;
            }
            while (c >= b && (diff = x.compare(c, m)) >= 0) {
                if (diff == 0) {
                    x.swap(c, d--);
                }
                c--;
            }
            if (b > c) {
                break;
            }
            x.swap(b++, c--);
        }

        // Swap partition elements back to middle
        int s, n = off + len;
        s = Math.min(a-off, b-a  );  vecswap(x, off, b-s, s);
        s = Math.min(d-c,   n-d-1);  vecswap(x, b,   n-s, s);

        // Recursively sort non-partition-elements
        if ((s = b-a) > 1) {
            sort1(x, off, s);
        }
        if ((s = d-c) > 1) {
            sort1(x, n-s, s);
        }
    }

    private static void vecswap(GenericSortArray x, int a, int b, int n) {
        for (int i=0; i<n; i++, a++, b++) {
            x.swap(a, b);
        }
    }

    private static int med3(GenericSortArray x, int a, int b, int c) {
        return (x.compare(a, b) < 0 ?
                (x.compare(b, c) < 0 ? b : x.compare(a, c) < 0 ? c : a) :
                (x.compare(b, c) > 0 ? b : x.compare(a, c) > 0 ? c : a));
    }
}