package com.rho.mediaplayer;

import com.rhomobile.rhodes.api.RhoApiFactory;

public class MediaplayerFactory
        extends RhoApiFactory< Mediaplayer, MediaplayerSingleton>
        implements IMediaplayerFactory {

    @Override
    protected MediaplayerSingleton createSingleton() {
        return new MediaplayerSingleton(this);
    }

    @Override
    protected Mediaplayer createApiObject(String id) {
        return new Mediaplayer(id);
    }
}
