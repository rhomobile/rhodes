using System;
using System.Collections.Generic;
using System.Net;
using Windows.UI.Core;
using System.Threading.Tasks;
using rhoruntime;

namespace rho {

namespace SensorImpl
{
    public class Sensor : SensorBase
    {
        public Sensor()
        {
            // initialize class instance in C# here
        }

        public override void getMinimumGap(IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public override void setMinimumGap(int minimumGap, IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public override void getType(IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public override void getStatus(IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public override void start(IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public override void readData(IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public override void stop(IMethodResult oResult)
        {
            // implement this method in C# here
        }
    }

    public class SensorSingleton : SensorSingletonBase
    {
        public SensorSingleton()
        {
            // initialize singleton instance in C# here
        }

        public override void makeSensorByType(string type, IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public override void enumerate(IMethodResult oResult)
        {
            // implement this method in C# here
        }
    }

    public class SensorFactory : SensorFactoryBase
    {
    }
}

}
