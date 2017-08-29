using System;
using System.Collections.Generic;
using System.Net;
using Windows.UI.Core;
using System.Threading.Tasks;
using rhoruntime;

namespace rho {

namespace SensorImpl
{
    abstract public class SensorBase : ISensorImpl
    {
        protected string _strID = "1";
        protected long _nativeImpl = 0;
        protected CoreDispatcher dispatcher = null;
        protected SensorRuntimeComponent _runtime;

        public const string SENSOR_STATUS_ERROR = "error";
        public const string SENSOR_STATUS_NOT_READY = "not_ready";
        public const string SENSOR_STATUS_READY = "ready";
        public const string SENSOR_STATUS_STARTED = "started";
        public const string SENSOR_TYPE_ACCELEROMETER = "Accelerometer";
        public const string SENSOR_TYPE_AMBIENT_LIGHT = "AmbientLight";
        public const string SENSOR_TYPE_DEVICE_ORIENTATION = "DeviceOrientation";
        public const string SENSOR_TYPE_ECOMPASS = "ECompass";
        public const string SENSOR_TYPE_GRAVITY = "Gravity";
        public const string SENSOR_TYPE_GYROSCOPE = "Gyroscope";
        public const string SENSOR_TYPE_HUMIDITY = "Humidity";
        public const string SENSOR_TYPE_LINEAR_ACCELERATION = "LinearAcceleration";
        public const string SENSOR_TYPE_MAGNETOMETER = "Magnetometer";
        public const string SENSOR_TYPE_MOTION = "Motion";
        public const string SENSOR_TYPE_ORIENTATION = "Orientation";
        public const string SENSOR_TYPE_PRESSURE = "Pressure";
        public const string SENSOR_TYPE_PROXIMITY = "Proximity";
        public const string SENSOR_TYPE_PROXIMITY_LONG_RANGE = "ProximityLongRange";
        public const string SENSOR_TYPE_ROTATION = "Rotation";
        public const string SENSOR_TYPE_TEMPERATURE = "Temperature";
        public const string SENSOR_TYPE_TILT_ANGLE = "TiltAngle";

        public SensorBase()
        {
            _runtime = new SensorRuntimeComponent(this);
            dispatcher = Windows.UI.Core.CoreWindow.GetForCurrentThread().Dispatcher;
        }

        public long getNativeImpl()
        {
            return _nativeImpl;
        }

        public virtual void setNativeImpl(string strID, long native)
        {
            _strID = strID;
            _nativeImpl = native;
        }

        public void DispatchInvoke(Action a)
        {
            if (dispatcher != null) {
              var ignore = dispatcher.RunAsync(CoreDispatcherPriority.Normal, () =>
              {try{a();} catch (Exception ex) {System.Diagnostics.Debug.WriteLine("Invoke in UI Thread exception");} });
            }else{a();}
        }

        public void getProperty(string propertyName, IMethodResult oResult)
        {
            _runtime.getProperty(propertyName, oResult);
        }

        public void getProperties(IReadOnlyList<string> arrayofNames, IMethodResult oResult)
        {
            _runtime.getProperties(arrayofNames, oResult);
        }

        public void getAllProperties(IMethodResult oResult)
        {
            _runtime.getAllProperties(oResult);
        }

        public void setProperty(string propertyName, string propertyValue, IMethodResult oResult)
        {
            _runtime.setProperty(propertyName, propertyValue, oResult);
        }

        public void setProperties(IReadOnlyDictionary<string, string> propertyMap, IMethodResult oResult)
        {
            _runtime.setProperties(propertyMap, oResult);
        }

        abstract public void getMinimumGap(IMethodResult oResult);
        abstract public void setMinimumGap(int minimumGap, IMethodResult oResult);
        abstract public void getType(IMethodResult oResult);
        abstract public void getStatus(IMethodResult oResult);
        abstract public void start(IMethodResult oResult);
        abstract public void readData(IMethodResult oResult);
        abstract public void stop(IMethodResult oResult);
    }

    abstract public class SensorSingletonBase : ISensorSingletonImpl
    {
        protected SensorSingletonComponent _runtime;

        public SensorSingletonBase()
        {
            _runtime = new SensorSingletonComponent(this);
        }

        abstract public void makeSensorByType(string type, IMethodResult oResult);
        abstract public void enumerate(IMethodResult oResult);
    }

    public class SensorFactoryBase : ISensorFactoryImpl
    {
        public virtual ISensorImpl getImpl() {
            return new Sensor();
        }
        public ISensorSingletonImpl getSingletonImpl() {
            return new SensorSingleton();
        }
    }
}

}
