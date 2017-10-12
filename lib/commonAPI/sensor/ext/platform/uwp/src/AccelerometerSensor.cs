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
        public class AccelerometerSensor : Sensor
        {
            private Accelerometer _accelerometer;

            bool status = false;

            public AccelerometerSensor(string id) : base(id)
            {
                type = id;

                _accelerometer = Accelerometer.GetDefault();

                if (_accelerometer != null)
                {
                    uint minReportInterval = _accelerometer.MinimumReportInterval;
                    uint reportInterval = minReportInterval > 16 ? minReportInterval : 16;
                    _accelerometer.ReportInterval = reportInterval;
                    _accelerometer.ReadingChanged += new TypedEventHandler<Accelerometer, AccelerometerReadingChangedEventArgs>(ReadingChanged);
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
                if (_accelerometer != null)
                {
                    status = true;
                    
                }
                readData(oResult);
            }

            string txtXAxis = "0";
            string txtYAxis = "0";
            string txtZAxis = "0";


            private async void ReadingChanged(object sender, AccelerometerReadingChangedEventArgs e)
            {
                await dispatcher.RunAsync(CoreDispatcherPriority.Normal, () =>
                {
                    AccelerometerReading reading = e.Reading;
                    txtXAxis = String.Format("{0,5:0.00}", reading.AccelerationX);
                    txtYAxis = String.Format("{0,5:0.00}", reading.AccelerationY);
                    txtZAxis = String.Format("{0,5:0.00}", reading.AccelerationZ);

                });
            }

            public override void readData(IMethodResult oResult)
            {
                Dictionary<string, string> result = new Dictionary<string, string>();

                if (_accelerometer != null) {
                    result.Add("type", type);
                    result.Add("status", SENSOR_STATUS_OK);
                    result.Add("accelerometer_x", txtXAxis);
                    result.Add("accelerometer_y", txtYAxis);
                    result.Add("accelerometer_z", txtZAxis);

                }else{
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