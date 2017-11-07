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
                System.Diagnostics.Debug.WriteLine("Timer " + _strID + " created");

            }
            private Windows.System.Threading.ThreadPoolTimer timer;

            private IMethodResult result = null;
            private bool alive = false;

            public override void start(int interval, IMethodResult oResult)
            {
                alive = true;
                result = oResult;

                timer = Windows.System.Threading.ThreadPoolTimer.CreateTimer((source) => {
                    dispatchInvoke(() => {
                        if (result != null)
                        {
                            //result.set(_strID); TODO: fix it
                        }
                        alive = false;
                        result = null;
                        System.Diagnostics.Debug.WriteLine("Timer " + _strID + " callback");
                    });
                }, TimeSpan.FromMilliseconds(interval));
                System.Diagnostics.Debug.WriteLine("Timer " + _strID + " started: " + (interval).ToString() + "ms");
            }

            public override void stop(IMethodResult oResult)
            {
                timer.Cancel();
                alive = false;
                result = null;
                System.Diagnostics.Debug.WriteLine("Timer " + _strID + " stopped");
            }

            public override void isAlive(IMethodResult oResult)
            {
                System.Diagnostics.Debug.WriteLine("Timer " + _strID + " is alive = " + (alive?"true":"false"));
                oResult.set(alive);
            }
        }

        public class TimerSingleton : TimerSingletonBase
        {
            public TimerSingleton()
            {
                // initialize singleton instance in C# here
            }

            private int currentId = 0;
            public override void create(IMethodResult oResult)
            {
                System.Diagnostics.Debug.WriteLine("Creating timer from singleton");
                // implement this method in C# here
                currentId++;
                String newId = (currentId).ToString();
                getTimerByID(newId);
                oResult.set(newId);

            }
        }

        public class TimerFactory : TimerFactoryBase
        {
            /*public virtual ITimerImpl getImpl(string id) {
                getSingletonImpl();
                return new Timer("");
            }*/
        }
    }

}
