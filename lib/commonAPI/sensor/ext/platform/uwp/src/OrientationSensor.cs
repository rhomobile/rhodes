using System;
using System.Collections.Generic;
using System.Net;
using Windows.UI.Core;
using System.Threading.Tasks;
using rhoruntime;
using Windows.Devices.Sensors;

namespace rho
{
    namespace SensorImpl
    {
        public class OrientationSensor : Sensor
        {
            
            private SimpleOrientation _screenOrientation = SimpleOrientation.Faceup;
            private static string UPSIDE_DOWN = "Portrait Down";
	        private static string LANDSCAPE_LEFT = "Landscape Left";
	        private static string LANDSCAPE_RIGHT = "Landscape Right";
	        private static string NORMAL = "Portrait Up";

            bool status = false;

            public OrientationSensor(string id) : base(id)
            {
                type = id;
            }

            public override void getMinimumGap(IMethodResult oResult)
            {
                oResult.set(200);
            }

            public override void setMinimumGap(int minimumGap, IMethodResult oResult)
            {
                
            }

            public override void getStatus(IMethodResult oResult)
            {
                if (status)
                {
                    oResult.set(SENSOR_STATUS_STARTED);
                }else
                {
                    oResult.set(SENSOR_STATUS_READY);
                }
            }

            public override void start(IMethodResult oResult)
            {
                status = true;
                readData(oResult);
            }

            public override void readData(IMethodResult oResult)
            {
                Dictionary<string, string> result = new Dictionary<string, string>();
                try
                {
                    _screenOrientation = SimpleOrientationSensor.GetDefault().GetCurrentOrientation();
                    result.Add("status", SENSOR_STATUS_OK);
                    switch (_screenOrientation) {
                        case SimpleOrientation.Faceup:
                            result.Add("deviceorientation_value", NORMAL);
                            break;
                        case SimpleOrientation.Rotated180DegreesCounterclockwise:
                            result.Add("deviceorientation_value", UPSIDE_DOWN);
                            break;
                        case SimpleOrientation.Rotated90DegreesCounterclockwise:
                            result.Add("deviceorientation_value", LANDSCAPE_RIGHT);
                            break;
                        case SimpleOrientation.Rotated270DegreesCounterclockwise:
                            result.Add("deviceorientation_value", LANDSCAPE_LEFT);
                            break;
                        default:
                            result.Add("deviceorientation_value", NORMAL);
                            break;
                    }
                    
                    result.Add("type", type);
                }
                catch (Exception e) {
                    _screenOrientation = SimpleOrientation.Faceup;
                    result.Add("status", SENSOR_STATUS_ERROR);
                    result.Add("message", e.Message);
                }


                oResult.set(result);

            }

            public override void stop(IMethodResult oResult)
            {
                status = false;
            }


        }
    }
}