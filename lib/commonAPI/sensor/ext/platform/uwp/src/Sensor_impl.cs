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
            protected string type = "null";

            public Sensor(string id) : base(id)
            {
                type = id;
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
                oResult.set(type);
            }

            public override void getStatus(IMethodResult oResult)
            {
                // implement this method in C# here
            }

            public override void start(IMethodResult oResult)
            {
                readData(oResult);
                // implement this method in C# here
            }

            public override void readData(IMethodResult oResult)
            {
                Dictionary<string, string> result = new Dictionary<string, string>();
                result.Add("status", SENSOR_STATUS_ERROR);
                result.Add("type", type);
                result.Add("message", "No available sensor of this type");

                oResult.set(result);
            }

            public override void stop(IMethodResult oResult)
            {
                // implement this method in C# here
            }
        }

        public class SensorSingleton : SensorSingletonBase
        {

            SortedDictionary<string, ISensorImpl> keeper = new SortedDictionary<string, ISensorImpl>();

            public SensorSingleton()
            {
               
            }


            public override void makeSensorByType(string type, IMethodResult oResult)
            {
                if (!keeper.ContainsKey(type))
                {
                    switch (type){
                        case SensorBase.SENSOR_TYPE_ORIENTATION:
                            keeper.Add(type, new OrientationSensor(type));
                            break;
                        case SensorBase.SENSOR_TYPE_DEVICE_ORIENTATION:
                            keeper.Add(type, new OrientationSensor(type));
                            break;
                        case SensorBase.SENSOR_TYPE_ACCELEROMETER:
                            keeper.Add(type, new AccelerometerSensor(type));
                            break;
                        case SensorBase.SENSOR_TYPE_LINEAR_ACCELERATION:
                            keeper.Add(type, new AccelerometerSensor(type));
                            break;
                        case SensorBase.SENSOR_TYPE_ECOMPASS:
                            keeper.Add(type, new CompassSensor(type));
                            break;
                        default:
                            keeper.Add(type, new Sensor(type));
                            break;
                    }
                    
                }

                if (keeper.ContainsKey(type))
                {
                    oResult.set(type);
                }
                
            }

            /*public Sensor getModuleByID(string id)
            {
                
            }*/


            public override void enumerate(IMethodResult oResult)
            {
                List<string> list = new List<string>();
                list.Add(SensorBase.SENSOR_TYPE_ORIENTATION);
                list.Add(SensorBase.SENSOR_TYPE_DEVICE_ORIENTATION);
                list.Add(SensorBase.SENSOR_TYPE_ACCELEROMETER);
                list.Add(SensorBase.SENSOR_TYPE_LINEAR_ACCELERATION);
                list.Add(SensorBase.SENSOR_TYPE_ECOMPASS);
                oResult.set(list.AsReadOnly());
            }
        }

        public class SensorFactory : SensorFactoryBase{

            

        }
    }

}
