using System;

namespace rho.common
{
    public class RhoEmptyProfiler : RhoProfiler
    {
        public override void CREATE_COUNTER(String name)
        {
        }

        public override void DESTROY_COUNTER(String name)
        {
        }

        public override void FLUSH_COUNTER(String name, String msg)
        {
        }

        public override void START(String name)
        {
        }

        public override void STOP(String name)
        {
        }
    }
}
