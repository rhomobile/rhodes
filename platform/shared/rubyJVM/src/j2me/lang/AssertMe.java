package j2me.lang;

public class AssertMe {

    public static void rho_assert(boolean bTrue)
    {
    	if ( !bTrue )
    		throw new RuntimeException();
    }
	
}
