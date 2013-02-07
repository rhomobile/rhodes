package com.rhomobile.rhodes.api;

public abstract class RhoApiDefaultId<IApi extends IRhoApiObject, IFactory extends IRhoApiFactory<IApi> >
    implements IRhoApiDefaultId {

    private IFactory mFactory;
    private IApi mDefault;
    
    public RhoApiDefaultId(IFactory factory) {
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

}