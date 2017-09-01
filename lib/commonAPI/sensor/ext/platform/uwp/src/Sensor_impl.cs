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

            SortedDictionary<string, Sensor> keeper = new SortedDictionary<string, Sensor>();

            public SensorSingleton()
            {
               
            }

            public override void makeSensorByType(string type, IMethodResult oResult)
            {
                if (!keeper.ContainsKey(type))
                {
                    switch (type){
                        case SensorBase.SENSOR_TYPE_ORIENTATION:
                            keeper.Add(type, new OrientationSensor());
                            break;
                        default:
                            oResult.set("nil");
                            break;
                    }
                    
                }

                if (keeper.ContainsKey(type))
                {
                    oResult.set(type);
                }
                
            }

            public Sensor getModuleByID(string id)
            {
                if (keeper.ContainsKey(id))
                {
                    return keeper[id];
                }
                else
                {
                    return new Sensor();
                }
            }


            public override void enumerate(IMethodResult oResult)
            {
                LinkedList<string> list = new LinkedList<string>();
                
                list.AddLast(SensorBase.SENSOR_TYPE_ORIENTATION);
                oResult.set((IReadOnlyList<string>)list);
            }
        }

        public class SensorFactory : SensorFactoryBase{

            

        }
    }

}
