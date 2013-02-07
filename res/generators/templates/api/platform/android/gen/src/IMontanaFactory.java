package com.<%= $cur_module.parent.downcase %>.<%= $cur_module.name.downcase %>;

import com.rhomobile.rhodes.api.IRhoApiFactory;
import com.rhomobile.rhodes.api.IRhoApiSingletonFactory;

public interface I<%= $cur_module.name %>Factory
    extends IRhoApiFactory<I<%= $cur_module.name %>>,
            IRhoApiSingletonFactory<I<%= $cur_module.name %>Singleton> {

    @Override
    I<%= $cur_module.name %>Singleton getApiSingleton();

    @Override
    <%= $cur_module.name %> getApiObject(String id);

}
