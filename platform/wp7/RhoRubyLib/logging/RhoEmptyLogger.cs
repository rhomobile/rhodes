using System;
using rho.logging;

namespace rho.common
{
    public class RhoEmptyLogger : RhoLogger
    {
        protected override void logMessage(int severity, String msg, Exception e, boolean bOutputOnly)
        {
        }

        public override void FATAL(String message, Exception e)
        {
		    processFatalError();
	    }

	    public override void FATAL(String message) {
		    processFatalError();
	    }

        public override void FATAL(Exception e)
        {
		    processFatalError();
	    }

	    public RhoEmptyLogger() : base(""){
	
	    }
    }
}
