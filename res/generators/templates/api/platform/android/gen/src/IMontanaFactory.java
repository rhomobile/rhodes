package <%= JavaGen::make_package_name($cur_module) %>;

import com.rhomobile.rhodes.api.IRhoApiFactory;
import com.rhomobile.rhodes.api.IRhoApiSingletonFactory;

public interface I<%= $cur_module.name %>Factory
    extends IRhoApiFactory<I<%= $cur_module.name %>>,
            IRhoApiSingletonFactory<I<%= $cur_module.name %>Singleton> {

    @Override
    I<%= $cur_module.name %>Singleton getApiSingleton();

    @Override
    I<%= $cur_module.name %> getApiObject(String id);

}
