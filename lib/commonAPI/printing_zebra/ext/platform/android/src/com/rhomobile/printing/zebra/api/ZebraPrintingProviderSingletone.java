package com.rhomobile.printing.zebra.api;

import com.rhomobile.printing.zebra.impl.ZebraPrintingProviderImpl;

public class ZebraPrintingProviderSingletone  {

private static IZebraPrintingProvider ourZebraPrintingProvider = null;

public static IZebraPrintingProvider getInstance() {
	if (ourZebraPrintingProvider == null) {
		ourZebraPrintingProvider = new ZebraPrintingProviderImpl();
	}
	return ourZebraPrintingProvider;
}


}
