package com.motorolasolutions.rhoelements;

import java.util.List;

public interface IBarcode1 extends IRhoApiObject {
    void getProps(IMethodResult result);
    void getProps(final String name, IMethodResult result);
    void getProps(final List<String> names, IMethodResult result);
};
