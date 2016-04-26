package com.rho.rhoelements.activex;

public class ABS extends ActiveX
{
	private ABS(){};
    static public ABS getInstance()
    {
    	// decide here if this class should be a singleton
    	ABS temp = new ABS(); 
    	return temp;
    }
}
