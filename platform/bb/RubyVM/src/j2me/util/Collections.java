package j2me.util;

import j2me.lang.ArrayMe;

public class Collections  
{
    private static final int INSERTIONSORT_THRESHOLD = 7;
	
    private static void swap(Object[] x, int a, int b) {
    	Object t = x[a];
    	x[a] = x[b];
    	x[b] = t;
    }
    
    private static void mergeSort(Object[] src,
			  Object[] dest,
			  int low, int high, int off,
			  Comparator c) {
    int length = high - low;

	// Insertion sort on smallest arrays
	if (length < INSERTIONSORT_THRESHOLD) {
	  for (int i=low; i<high; i++)
		for (int j=i; j>low && c.compare(dest[j-1], dest[j])>0; j--)
		    swap(dest, j, j-1);
	  return;
	}
		
  	  // Recursively sort halves of dest into src
	  int destLow  = low;
	  int destHigh = high;
	  low  += off;
	  high += off;
	  int mid = (low + high) >> 1;
	  mergeSort(dest, src, low, mid, -off, c);
	  mergeSort(dest, src, mid, high, -off, c);
		
	  // If list is already sorted, just copy from src to dest.  This is an
	  // optimization that results in faster sorts for nearly ordered lists.
	  if (c.compare(src[mid-1], src[mid]) <= 0) {
	     System.arraycopy(src, low, dest, destLow, length);
	     return;
	  }
		
	  // Merge sorted halves (now in src) into dest
	  for(int i = destLow, p = low, q = mid; i < destHigh; i++) {
	      if (q >= high || p < mid && c.compare(src[p], src[q]) <= 0)
	          dest[i] = src[p++];
	      else
	          dest[i] = src[q++];
	  }
	}
	
    public static void sort(List list, Comparator c)
    {
    	Object[] a = list.toArray();
    	Object[] aux = ArrayMe.clone(a);
    	
    	mergeSort(aux, a, 0, a.length, 0, c);
    	ListIterator i = list.listIterator();
    	for (int j=0; j<a.length; j++) {
    	    i.next();
    	    i.set(a[j]);
    	}
    }
    
    public static void reverse(List list)
    {
    	int length = list.size();

    	if ( length < 2 )
    		return;
    	
        for (int i = 0; i < length >> 1; i++) {
        	Object temp = list.get(i);
        	list.set(i, list.get(length - i - 1) );
        	list.set(length - i - 1, temp );
        }
    }
}
