package j2me.lang;

public class StringBufferMe {
/*
    public static int indexOf(StringBuffer strThis, String str) {
    	return indexOf(strThis,str, 0);
    }

    public static int indexOf(StringBuffer strThis, String str, int fromIndex) {
    	return strThis.toString().indexOf(str, fromIndex);
    }
    
    public static StringBuffer replace( StringBuffer strThis, int start, int end, String str) {
    	AbstractStringBuilder_replace(strThis, start, end, str);
        return strThis;
    }

    public static StringBuffer AbstractStringBuilder_replace(StringBuffer strThis, int start, int end, String str) {
        if (start < 0)
        	throw new StringIndexOutOfBoundsException(start);
        
        int count = strThis.length();
		if (end > count)
		    end = count;
		if (start > end)
		    throw new StringIndexOutOfBoundsException("start > end");

		int len = str.length();
		int newCount = count + len - (end - start);
		if (newCount > strThis.capacity())
			strThis.ensureCapacity(newCount);
	
		if ( len != end-start)
		{
			for( int i = 0; i < count-end; i++ )
				strThis.setCharAt(start + len+i, strThis.charAt(end+i));
		}
		int i = start;
		for( ; i < Math.min(count,len); i++ )
			strThis.setCharAt(i, str.charAt(i-start));

		for( ; i < len; i++ )
			strThis.append(str.charAt(i-start));
		
//        System.arraycopy(value, end, value, start + len, count - end);
//        str.getChars(value, start);
        count = newCount;
        return strThis;
    }*/
    
}
