package com.rho.printingepson;

import com.rhomobile.rhodes.api.RhoApiFactory;

public class PrintingEpsonFactory
        extends RhoApiFactory< PrintingEpson, PrintingEpsonSingleton>
        implements IPrintingEpsonFactory {

    @Override
    protected PrintingEpsonSingleton createSingleton() {
        return new PrintingEpsonSingleton(this);
    }

    @Override
    protected PrintingEpson createApiObject(String id) {
        return new PrintingEpson(id);
    }
}
