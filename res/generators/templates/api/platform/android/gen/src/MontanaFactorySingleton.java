package <%= JavaGen::make_package_name($cur_module) %>;

public class <%= $cur_module.name %>FactorySingleton {
    private static I<%= $cur_module.name %>Factory mFactory;
    public static void setInstance(I<%= $cur_module.name %>Factory factory) {
        mFactory = factory;
    }
    public static I<%= $cur_module.name %>Factory getInstance() {
        return mFactory;
    }
}
