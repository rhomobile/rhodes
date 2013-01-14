package com.motorolasolutions.rhoelements;

public abstract class ApiSingletonBase<IApi extends IRhoApiObject> {

    private IApi sDefault;

    public String getDefaultID() {
        if (sDefault == null) {
            sDefault = getApiObject(createDefaultId());
        }
        return sDefault.getId();
    }

    public void setDefaultID(final String id) {
        sDefault = getApiObject(id);
    }

    public abstract IApi getApiObject(final String id);
    protected abstract String createDefaultId();
    public abstract void enumerate(IMethodResult res);

}