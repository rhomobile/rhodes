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

        public void enable(IReadOnlyDictionary<string, string> propertyMap, InstrumentationRuntime.IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void start(InstrumentationRuntime.IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void stop(InstrumentationRuntime.IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void disable(InstrumentationRuntime.IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void take(IReadOnlyDictionary<string, string> propertyMap, InstrumentationRuntime.IMethodResult oResult)
        {
            // implement this method in C# here
        }
    }
}
