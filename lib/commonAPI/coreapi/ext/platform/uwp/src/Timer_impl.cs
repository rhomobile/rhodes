using System;
using System.Collections.Generic;
using System.Net;
using Windows.UI.Core;
using System.Threading.Tasks;
using rhoruntime;
using rhodes;

namespace rho {

namespace TimerImpl
{
    public class Timer : TimerBase
    {
        public Timer(string id) : base(id)
        {
            // initialize class instance in C# here
        }

        public override void start(int interval, IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public override void stop(IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public override void isAlive(IMethodResult oResult)
        {
            // implement this method in C# here
        }
    }

    public class TimerSingleton : TimerSingletonBase
    {
        public TimerSingleton()
        {
            // initialize singleton instance in C# here
        }

        public override void create(IMethodResult oResult)
        {
            // implement this method in C# here
        }
    }

    public class TimerFactory : TimerFactoryBase
    {
    }
}

}
