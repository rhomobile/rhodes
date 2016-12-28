package com.rho.clipboard;

import com.rhomobile.rhodes.api.RhoApiFactory;

public class ClipboardFactory
        extends RhoApiFactory< Clipboard, ClipboardSingleton>
        implements IClipboardFactory {

    @Override
    protected ClipboardSingleton createSingleton() {
        return new ClipboardSingleton(this);
    }

    @Override
    protected Clipboard createApiObject(String id) {
        return new Clipboard(id);
    }
}
