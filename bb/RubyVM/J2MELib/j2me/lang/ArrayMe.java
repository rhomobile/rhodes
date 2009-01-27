package j2me.lang;

public class ArrayMe {

    public static int[] clone(int[] arThis)
    {
    	//TODO: clone
    	throw new RuntimeException("Not Implemented");
    	//return arThis;
    }
    public static byte[] clone(byte[] arThis)
    {
    	//TODO: clone
    	throw new RuntimeException("Not Implemented");
    	//return arThis;
    }
    public static char[] clone(char[] arThis)
    {
    	//TODO: clone
    	throw new RuntimeException("Not Implemented");
    	//return arThis;
    }
    public static Object[] clone(Object[] arThis)
    {
    	Object[] copy = new Object[arThis.length];
    	for( int i = 0; i < arThis.length; i++ )
    		copy[i] = arThis[i];
    	
    	return copy;
    }
	
}
