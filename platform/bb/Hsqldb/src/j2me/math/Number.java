package j2me.math;

public class Number{

	public static boolean isNumber(Object obj){
    	if ( obj instanceof Long || obj instanceof Integer ||
    		 obj instanceof Short || obj instanceof Byte ||
    		 obj instanceof Double || obj instanceof Float )
    		return true;
    	
		return false;
	}
	
    public static int intValue(Object obj){
    	if ( obj instanceof Integer )
    		return ((Integer)obj).intValue();
    	else if ( obj instanceof Long )
    		return (int)((Long)obj).longValue();
    	else if ( obj instanceof Short )
    		return ((Short)obj).shortValue();
    	else if ( obj instanceof Byte )
    		return ((Byte)obj).byteValue();
    	
    	return Integer.parseInt(obj.toString());
    }
	
    public static long longValue(Object obj){
    	if ( obj instanceof Long )
    		return ((Long)obj).longValue();
    	else if ( obj instanceof Integer )
    		return ((Integer)obj).longValue();
    	else if ( obj instanceof Short )
    		return ((Short)obj).shortValue();
    	else if ( obj instanceof Byte )
    		return ((Byte)obj).byteValue();
    	
    	return Long.parseLong(obj.toString());
    }
    
    public static double doubleValue(Object obj){
    	if ( obj instanceof Double )
    		return ((Double)obj).doubleValue();
    	else if ( obj instanceof Float )
    		return ((Float)obj).doubleValue();
    	else if ( obj instanceof Long )
    		return ((Long)obj).longValue();
    	else if ( obj instanceof Integer )
    		return ((Integer)obj).longValue();
    	else if ( obj instanceof Short )
    		return ((Short)obj).shortValue();
    	else if ( obj instanceof Byte )
    		return ((Byte)obj).byteValue();
    	
    	return Double.parseDouble(obj.toString());
    }
    
    /*
    
	private Object m_oValue;
	
	public Number(Object val){
		m_oValue = val;
	}
	public Number(Integer val){
		m_oValue = val;
	}
    
    public int intValue(){
    	return 0;
    }
    public long longValue(){
    	return 0;
    }

    public float floatValue(){
    	return 0;
    }
    public double doubleValue(){
    	return 0;
    }

    public byte byteValue() {
    	return (byte)intValue();
    }

    public short shortValue() {
    	return (short)intValue();
    }
    
    */

};
