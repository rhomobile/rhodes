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

namespace Barcode1Impl
{
    public class Barcode1 : Barcode1Runtime.IBarcode1Impl
    {
        public Barcode1()
        {
            var _runtime = new Barcode1Runtime.Barcode1RuntimeComponent(this);
        }

	    public void enable(IReadOnlyDictionary<string, string> propertyMap, Barcode1Runtime.IMethodResult oResult)
	    {
            // implement this method in C# here
	    }

	    public void start(Barcode1Runtime.IMethodResult oResult)
	    {
            // implement this method in C# here
        }

	    public void stop(Barcode1Runtime.IMethodResult oResult)
	    {
            // implement this method in C# here
        }

	    public void disable(Barcode1Runtime.IMethodResult oResult)
	    {
            // implement this method in C# here
        }

        public void take(IReadOnlyDictionary<string, string> propertyMap, Barcode1Runtime.IMethodResult oResult)
	    {
            // implement this method in C# here
        }
    }
}
