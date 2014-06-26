package com.rhomobile.rhodes.api;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;

import com.rhomobile.rhodes.Logger;

public class RhoApiPropertyBag implements
        IRhoApiPropertyBag {
    private static final String TAG = RhoApiPropertyBag.class.getSimpleName();

    private HashMap<String, String> mProperties;
    private List<String> mAllowedNames;

    public boolean isAllowed(String name) {
        return (mAllowedNames == null) || mAllowedNames.contains(name);
    }

    public boolean isAllAllowed(Set<String> names) {
        return (mAllowedNames == null) || mAllowedNames.containsAll(names);
    }

    public Map<String, String> getPropertiesMap() { return mProperties; }

    public RhoApiPropertyBag(String id) {
        mAllowedNames = null;
        mProperties = new HashMap<String, String>();
    }

    public RhoApiPropertyBag(List<String> allowedNames, String id) {
        mAllowedNames = allowedNames;
        mProperties = new HashMap<String, String>();
    }

    //@Override
    //public void clearAllProperties(IMethodResult result) {
    //    getPropertiesMap().clear();
    //}

    @Override
    public void getAllProperties(IMethodResult result) {
        if (mAllowedNames == null) {
            for (String key: getPropertiesMap().keySet()) {
                MethodResult propResult = new MethodResult(false);
                getProperty(key, propResult);
                propResult.collectSelf(key, result);
            }
        } else {
            for (String key: mAllowedNames) {
                MethodResult propResult = new MethodResult(false);
                getProperty(key, propResult);
                propResult.collectSelf(key, result);
            }
        }
        result.set();
    }

    @Override
    public void getProperty(String name, IMethodResult result) {
        if (isAllowed(name)) {
            result.set(getPropertiesMap().get(name));
        } else {
            result.setArgError("Unknown property: '" + name + "'");
        }
    }

    @Override
    public void getProperties(List<String> names, IMethodResult result) {
        for(String name: names) {
            if (isAllowed(name)) {
                MethodResult propResult = new MethodResult(false);
                getProperty(name, propResult);
                propResult.collectSelf(name, result);
            } else {
                result.setArgError("Unknown property: '" + name + "'");
                return;
            }
        }
        result.set();
    }

    @Override
    public void setProperty(String name, String val, IMethodResult result) {
        if (isAllowed(name)) {
            getPropertiesMap().put(name, val);
        } else {
            result.setArgError("Unknown property: '" + name + "'");
        }
    }

    @Override
    public void setProperties(Map<String, String> props, IMethodResult result) {
        Set<String> names = props.keySet();
        if (isAllAllowed(names)) {
            for(String name: names) {
                setProperty(name, props.get(name), result);
            }
        } else {
            names.removeAll(mAllowedNames);
            StringBuffer strNames = new StringBuffer();
            for (String name: names) {
                strNames.append("'").append(name).append("', ");
            }
            strNames.setLength(strNames.length() - 2);
            result.setArgError("Unknown properties: " + strNames);
        }

    }

    public void clearAllProperties(Map<String, Class<?> > customProps, RhoApiObject apiObject, IMethodResult result) {
        getPropertiesMap().clear();

        for(Map.Entry<String, Class<?> > prop: customProps.entrySet()) {
            Class<? extends RhoApiObject> clazz = apiObject.getClass().asSubclass(RhoApiObject.class);
            try {
                if (prop.getValue().isPrimitive()) {
                    Logger.W(TAG, "Cannot clear value for property of primitive type (" + prop.getValue().getSimpleName() + "): '" + prop.getKey() + "', object id: " + apiObject.getId());
                } else {
                    MethodResult propResult = new MethodResult(false);
                    Method method = clazz.getMethod(makeAccessorName("set", prop.getKey()), prop.getValue(), IMethodResult.class);
                    method.invoke(apiObject, null, propResult);
                    if (propResult.getResultType() == MethodResult.ResultType.typeArgError.ordinal()) {
                        result.setArgError(propResult.getString());
                        return;
                    } else if (propResult.getResultType() == MethodResult.ResultType.typeError.ordinal()) {
                        result.setError(propResult.getString());
                        return;
                    }
                }
            } catch (NoSuchMethodException e) {
                Logger.E(TAG, e);
                result.setError("Property accessor does not exist (" + e.getMessage() + "): '" + prop.getKey() + "', object id: " + apiObject.getId());
                return;
            } catch (IllegalArgumentException e) {
                Logger.E(TAG, e);
                result.setError("Property accessor is wrong (" + e.getMessage() + "): '" + prop.getKey() + "', object id: " + apiObject.getId());
                return;
            } catch (IllegalAccessException e) {
                Logger.E(TAG, e);
                result.setError("Property accessor is inaccessible (" + e.getMessage() + "): '" + prop.getKey() + "', object id: " + apiObject.getId());
                return;
            } catch (InvocationTargetException e) {
                Logger.E(TAG, e);
                result.setError("Cannot invoke property accessor (" + e.getMessage() + "): '" + prop.getKey() + "', object id: " + apiObject.getId());
                return;
            }
        }
    }

    public void getAllProperties(Map<String, Class<?> > customProps, RhoApiObject apiObject, IMethodResult result) {
        
        MethodResult propsResult = new MethodResult(false);
        getAllProperties(propsResult);
        propsResult.mergeSelf(result);

        for(Map.Entry<String, Class<?> > prop: customProps.entrySet()) {
            Class<? extends RhoApiObject> clazz = apiObject.getClass().asSubclass(RhoApiObject.class);
            try {
                MethodResult propResult = new MethodResult(false);
                Method method = clazz.getMethod(makeAccessorName("get", prop.getKey()), IMethodResult.class);
                method.invoke(apiObject, propResult);
                if (propResult.getResultType() == MethodResult.ResultType.typeArgError.ordinal()) {
                    result.setArgError(propResult.getString());
                    return;
                } else if (propResult.getResultType() == MethodResult.ResultType.typeError.ordinal()) {
                    result.setError(propResult.getString());
                    return;
                } else {
                    propResult.collectSelf(prop.getKey(), result);
                }
            } catch (NoSuchMethodException e) {
                Logger.E(TAG, e);
                result.setError("Property accessor does not exist (" + e.getMessage() + "): '" + prop.getKey() + "', object id: " + apiObject.getId());
                return;
            } catch (IllegalArgumentException e) {
                Logger.E(TAG, e);
                result.setError("Property accessor is wrong (" + e.getMessage() + "): '" + prop.getKey() + "', object id: " + apiObject.getId());
                return;
            } catch (IllegalAccessException e) {
                Logger.E(TAG, e);
                result.setError("Property accessor is inaccessible (" + e.getMessage() + "): '" + prop.getKey() + "', object id: " + apiObject.getId());
                return;
            } catch (InvocationTargetException e) {
                Logger.E(TAG, e);
                result.setError("Cannot invoke property accessor (" + e.getMessage() + "): '" + prop.getKey() + "', object id: " + apiObject.getId());
                return;
            }
        }

        result.set();
    }

    public void getProperty(String name, Map<String, Class<?> > customProps, RhoApiObject apiObject, IMethodResult result) {
        if (isAllowed(name)) {
            if (customProps.keySet().contains(name)) {
                Class<? extends RhoApiObject> clazz = apiObject.getClass().asSubclass(RhoApiObject.class);
                try {
                    MethodResult propResult = new MethodResult(false);
                    Method method = clazz.getMethod(makeAccessorName("get", name), IMethodResult.class);
                    method.invoke(apiObject, propResult);
                    if (propResult.getResultType() == MethodResult.ResultType.typeArgError.ordinal()) {
                        result.setArgError(propResult.getString());
                        return;
                    } else if (propResult.getResultType() == MethodResult.ResultType.typeError.ordinal()) {
                        result.setError(propResult.getString());
                        return;
                    } else {
                        propResult.setSelf(result);
                    }
                } catch (NoSuchMethodException e) {
                    Logger.E(TAG, e);
                    result.setError("Property accessor does not exist (" + e.getMessage() + "): '" + name + "', object id: " + apiObject.getId());
                    return;
                } catch (IllegalArgumentException e) {
                    Logger.E(TAG, e);
                    result.setError("Property accessor is wrong (" + e.getMessage() + "): '" + name + "', object id: " + apiObject.getId());
                    return;
                } catch (IllegalAccessException e) {
                    Logger.E(TAG, e);
                    result.setError("Property accessor is inaccessible (" + e.getMessage() + "): '" + name + "', object id: " + apiObject.getId());
                    return;
                } catch (InvocationTargetException e) {
                    Logger.E(TAG, e);
                    result.setError("Cannot invoke property accessor (" + e.getMessage() + "): '" + name + "', object id: " + apiObject.getId());
                    return;
                }
            } else {
                getProperty(name, result);
            }
        } else {
            result.setArgError("Unknown property: '" + name + "'");
        }
    }

    public void getProperties(List<String> names, Map<String, Class<?> > customProps, RhoApiObject apiObject, IMethodResult result) {
        for(String name: names) {
            MethodResult propResult = new MethodResult(false);
            getProperty(name, customProps, apiObject, propResult);
            if (propResult.getResultType() == MethodResult.ResultType.typeArgError.ordinal()) {
                result.setArgError(propResult.getString());
                return;
            } else if (propResult.getResultType() == MethodResult.ResultType.typeError.ordinal()) {
                result.setError(propResult.getString());
                return;
            } else {
                propResult.collectSelf(name, result);
            }
        }
        result.set();
    }

    public void setProperty(String name, String value, Map<String, Class<?> > customProps, RhoApiObject apiObject, IMethodResult result) {
        if (isAllowed(name)) {
            if (customProps.keySet().contains(name)) {
                Class<? extends RhoApiObject> clazz = apiObject.getClass().asSubclass(RhoApiObject.class);
                try {
                    Method[] methods = clazz.getMethods();
                    Method method = null;
                    String accessorName = makeAccessorName("set", name);
                    for(Method curMethod: methods) {
                        if (curMethod.getName().equals(accessorName)) {
                            method = curMethod;
                            break;
                        }
                    }
                    if (method == null) {
                        throw new NoSuchMethodException(accessorName);
                    }
                    Class<?> paramType = method.getParameterTypes()[0];
                    if (paramType.equals(String.class)) {
                        method.invoke(apiObject, value, result);
                    } else if (paramType.equals(boolean.class)) {
                        boolean boolValue = Boolean.parseBoolean(value);
                        method.invoke(apiObject, boolValue, result);
                    } else if (paramType.equals(int.class)) {
                        int intValue = Integer.parseInt(value);
                        method.invoke(apiObject, intValue, result);
                    } else if (paramType.equals(double.class)) {
                        double doubleValue = Double.parseDouble(value);
                        method.invoke(apiObject, doubleValue, result);
                    } else {
                        result.setArgError("Wrong property type (" + paramType.getSimpleName() + "): " + name + "', object id: " + apiObject.getId());
                    }
                } catch (NoSuchMethodException e) {
                    Logger.E(TAG, e);
                    result.setError("Property accessor does not exist (" + e.getMessage() + "): '" + name + "', object id: " + apiObject.getId());
                    return;
                } catch (IllegalArgumentException e) {
                    Logger.E(TAG, e);
                    result.setError("Property accessor is wrong (" + e.getMessage() + "): '" + name + "', object id: " + apiObject.getId());
                    return;
                } catch (IllegalAccessException e) {
                    Logger.E(TAG, e);
                    result.setError("Property accessor is inaccessible (" + e.getMessage() + "): '" + name + "', object id: " + apiObject.getId());
                    return;
                } catch (InvocationTargetException e) {
                    Logger.E(TAG, e);
                    result.setError("Cannot invoke property accessor (" + e.getMessage() + "): '" + name + "', object id: " + apiObject.getId());
                    return;
                }
            } else {
                getProperty(name, result);
            }
        } else {
            result.setArgError("Unknown property: '" + name + "'");
        }
    }

    public void setProperties(Map<String, String> props, Map<String, Class<?> > customProps, RhoApiObject apiObject, IMethodResult result) {
        for(Map.Entry<String, String> prop: props.entrySet()) {
            MethodResult propResult = new MethodResult(false);
            setProperty(prop.getKey(), prop.getValue(), customProps, apiObject, propResult);
            if (propResult.getResultType() == MethodResult.ResultType.typeArgError.ordinal()) {
                result.setArgError(propResult.getString());
                return;
            } else if (propResult.getResultType() == MethodResult.ResultType.typeError.ordinal()) {
                result.setError(propResult.getString());
                return;
            }
        }
    }
    
    private static String makeAccessorName(String prefix, String name) {
        StringBuilder sb = new StringBuilder(prefix).append(name);
        sb.setCharAt(prefix.length(), Character.toUpperCase(name.charAt(0)));  
        return sb.toString();
    }
}
