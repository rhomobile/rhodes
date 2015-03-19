using System;
using System.Collections.Generic;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Ink;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Shapes;
using rhoruntime;

namespace rho {

namespace DeviceMemoryImpl
{
    public class DeviceMemory : IDeviceMemoryImpl
    {
        public DeviceMemory()
        {
            var _runtime = new DeviceMemoryRuntimeComponent(this);
        }

        public void enable(IReadOnlyDictionary<string, string> propertyMap, IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void start(IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void stop(IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void disable(IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void take(IReadOnlyDictionary<string, string> propertyMap, IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void getProperty(string propertyName, IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void getProperties(IReadOnlyList<string> arrayofNames, IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void getAllProperties(IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void setProperty(string propertyName, string propertyValue, IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void setProperties(IReadOnlyDictionary<string, string> propertyMap, IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void clearAllProperties(IMethodResult oResult)
        {
            // implement this method in C# here
        }
    }

    public class DeviceMemorySingleton : IDeviceMemorySingletonImpl
    {
        public DeviceMemorySingleton()
        {
            var _runtime = new DeviceMemorySingletonComponent(this);
        }

        public void enumerate(IMethodResult oResult)
        {
            // implement this method in C# here
        }
    }

    public class DeviceMemoryFactory : IDeviceMemoryFactoryImpl
    {
        public IDeviceMemoryImpl getImpl() {
            return new DeviceMemory();
        }
        public IDeviceMemorySingletonImpl getSingletonImpl() {
            return new DeviceMemorySingleton();
        }
    }
}

}
