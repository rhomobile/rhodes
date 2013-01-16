package com.motorolasolutions.rhoelements;

import java.util.List;
import java.util.Map;

public interface IMethodResult {
    void set(final String res);
    void set(final List<String> res);
    void set(final Map<String, String> res);
}
