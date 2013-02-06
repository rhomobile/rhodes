package com.rhomobile.rhodes.api;

public abstract class RhoApiSingleton<IApi extends IRhoApiObject, IFactory extends IRhoApiFactory<IApi> >
    implements IRhoApiSingleton {

    private IFactory mFactory;
    private IApi mDefault;
    
    public RhoApiSingleton(IFactory factory) {
        mFactory = factory;
    }

    public String getDefaultID() {
        if (mDefault == null) {
            mDefault = mFactory.getApiObject(getInitialDefaultID());
        }
        return mDefault.getId();
    }

    public void setDefaultID(final String id) {
        mDefault = mFactory.getApiObject(id);
    }

    protected abstract String getInitialDefaultID();
    public abstract void enumerate(IMethodResult res);

}