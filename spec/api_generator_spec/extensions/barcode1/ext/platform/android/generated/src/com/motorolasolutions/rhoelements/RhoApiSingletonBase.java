package com.motorolasolutions.rhoelements;

public abstract class RhoApiSingletonBase<IApi extends IRhoApiObject, IFactory extends IRhoApiFactory<IApi> >
    implements IRhoApiSingleton {

    private IFactory mFactory;
    private IApi mDefault;
    
    public RhoApiSingletonBase(IFactory factory) {
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