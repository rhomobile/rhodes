package j2me.lang;

public class AssertMe {

    public static void assertme(boolean bTrue)
    {
    	if ( !bTrue )
    		throw new RuntimeException();
    }
	
}
