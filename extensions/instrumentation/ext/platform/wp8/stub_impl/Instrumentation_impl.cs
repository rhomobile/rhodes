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

namespace InstrumentationImpl
{
    public class Instrumentation : InstrumentationRuntime.IInstrumentationImpl
    {
        public Instrumentation()
        {
            var _runtime = new InstrumentationRuntime.InstrumentationRuntimeComponent(this);
        }

        public void simulate_key_event_code(int keycode, InstrumentationRuntime.IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void simulate_key_event_string(string str, InstrumentationRuntime.IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void simulate_touch_event(int event_type, int x, int y, InstrumentationRuntime.IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void screen_capture(string pszFilename, InstrumentationRuntime.IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void get_allocated_memory(InstrumentationRuntime.IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void delete_file(string str, InstrumentationRuntime.IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void file_exists(string str, InstrumentationRuntime.IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void re_simulate_navigation(InstrumentationRuntime.IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void getProperty(string propertyName, InstrumentationRuntime.IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void getProperties(IReadOnlyList<string> arrayofNames, InstrumentationRuntime.IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void getAllProperties(InstrumentationRuntime.IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void setProperty(string propertyName, string propertyValue, InstrumentationRuntime.IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void setProperties(IReadOnlyDictionary<string, string> propertyMap, InstrumentationRuntime.IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void clearAllProperties(InstrumentationRuntime.IMethodResult oResult)
        {
            // implement this method in C# here
        }
    }

    public class InstrumentationSingleton : InstrumentationRuntime.IInstrumentationSingletonImpl
    {
        public InstrumentationSingleton()
        {
            var _runtime = new InstrumentationRuntime.InstrumentationSingletonComponent(this);
        }

        public void enumerate(InstrumentationRuntime.IMethodResult oResult)
        {
            // implement this method in C# here
        }
    }
}