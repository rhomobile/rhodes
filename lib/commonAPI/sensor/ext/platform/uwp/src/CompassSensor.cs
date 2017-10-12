using System;
using System.Collections.Generic;
using System.Net;
using Windows.UI.Core;
using System.Threading.Tasks;
using rhoruntime;
using Windows.Devices.Sensors;
using Windows.Foundation;

namespace rho
{
    namespace SensorImpl
    {
        public class CompassSensor : Sensor
        {
            private Compass _compass = Compass.GetDefault();

            bool status = false;

            public CompassSensor(string id) : base(id)
            {
                type = id;
                _compass = Compass.GetDefault();

                if (_compass != null)
                {
                    uint minReportInterval = _compass.MinimumReportInterval;
                    uint reportInterval = minReportInterval > 16 ? minReportInterval : 16;
                    _compass.ReportInterval = reportInterval;
                    _compass.ReadingChanged += new TypedEventHandler<Compass, CompassReadingChangedEventArgs>(ReadingChanged);
                }

            }




            public override void getMinimumGap(IMethodResult oResult)
            {
                oResult.set(100);
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
                if (_compass != null)
                {
                    status = true;
                    
                }
                readData(oResult);
            }

            string txtMagnetic = "0";
            string txtNorth = "0";

            private async void ReadingChanged(object sender, CompassReadingChangedEventArgs e)
            {
                await dispatcher.RunAsync(CoreDispatcherPriority.Normal, () =>
                {
                    try
                    {
                        CompassReading reading = e.Reading;
                        txtMagnetic = String.Format("{0,5:0.00}", reading.HeadingMagneticNorth);
                        if (reading.HeadingTrueNorth.HasValue)
                            txtNorth = String.Format("{0,5:0.00}", reading.HeadingTrueNorth);
                    }catch(Exception exc)
                    {

                    }
                });
            }



            public override void readData(IMethodResult oResult)
            {
                Dictionary<string, string> result = new Dictionary<string, string>();
                
                if(_compass != null)
                {
                    result.Add("type", type);
                    result.Add("status", SENSOR_STATUS_OK);
                    result.Add("ecompass_value", txtNorth);
                }else {
                    result.Add("status", SENSOR_STATUS_ERROR);
                    result.Add("type", type);
                    result.Add("message", "No available sensor of this type");
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