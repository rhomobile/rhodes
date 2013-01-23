package com.motorolasolutions.rhoelements;


public interface IRhoApiFactory <IApi extends IRhoApiObject>{
    IApi getApiObject(String strID);
}
