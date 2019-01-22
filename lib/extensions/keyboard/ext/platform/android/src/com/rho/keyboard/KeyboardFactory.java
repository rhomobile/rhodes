package com.rho.keyboard;

import com.rhomobile.rhodes.api.RhoApiFactory;

public class KeyboardFactory
        extends RhoApiFactory< Keyboard, KeyboardSingleton>
        implements IKeyboardFactory {

    @Override
    protected KeyboardSingleton createSingleton() {
        return new KeyboardSingleton(this);
    }

    @Override
    protected Keyboard createApiObject(String id) {
        return new Keyboard(id);
    }
}
