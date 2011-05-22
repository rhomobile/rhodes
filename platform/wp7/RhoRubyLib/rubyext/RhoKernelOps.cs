using System;
using Microsoft.Scripting.Utils;
using Microsoft.Scripting.Runtime;
using IronRuby.Runtime;
using IronRuby.Builtins;
using rho.common;

namespace rho.rubyext
{
    public static class RhoKernelOps
    {
        private static RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() :
            new RhoLogger("RhoKernelOps");

        static CRhoRuby RhoRuby { get { return CRhoRuby.Instance; } }

        private static CRhodesApp RhodesApp { get { return CRhodesApp.Instance; } }

        public static Object __rhoGetCallbackObject(Object/*!*/ self, int index)
        {
            Object res = null;
            try
            {
                res = RhodesApp.getCallbackObject(index);
            }
            catch (Exception ex)
            {
                LOG.HandleRubyException(ex, RhoRuby.rubyContext.CurrentException, "__rhoGetCallbackObject");
            }

            return res;
        }

        public static Object __rho_exist_in_resources(Object/*!*/ self, String path)
        {
            Object res = null;
            try
            {
                res = CRhoFile.isResourceFileExist(path);
            }
            catch (Exception ex)
            {
                LOG.HandleRubyException(ex, RhoRuby.rubyContext.CurrentException, "__rho_exist_in_resources");
            }

            return res;
        }

    }
}
