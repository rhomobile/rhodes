package com.rhomobile.rhodes.api;

import java.util.List;

public interface IRhoApiProps extends IRhoApiObject {
    void getProps(IMethodResult result);
    void getProps(String name, IMethodResult result);
    void getProps(List<String> names, IMethodResult result);
}
