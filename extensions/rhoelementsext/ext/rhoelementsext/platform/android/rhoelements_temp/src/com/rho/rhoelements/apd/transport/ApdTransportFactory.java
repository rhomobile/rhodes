/**
 * 
 */
package com.rho.rhoelements.apd.transport;

import com.rho.rhoelements.apd.ApdConfiguration;
import com.rho.rhoelements.apd.ApdConfiguration.Transport;

/**
 * @author FPC843
 *
 */
public class ApdTransportFactory
{
	public static ApdTransport createTransport(ApdConfiguration printer) throws Exception
	{
		ApdTransport res = null;
		if (printer != null)
		{
			if (printer.getTransport() == Transport.Bluetooth)
			{
				res = new ApdBluetooth(printer);
			}
			else if (printer.getTransport() == Transport.Wlan)
			{
				res = new ApdWlan(printer);
			}
		}
		return res;
	}
}
