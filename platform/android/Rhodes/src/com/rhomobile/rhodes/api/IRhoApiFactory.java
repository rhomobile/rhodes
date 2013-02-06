package com.rhomobile.rhodes.api;

public interface IRhoApiFactory <IApi extends IRhoApiObject>{
    IApi getApiObject(String strID);
}
