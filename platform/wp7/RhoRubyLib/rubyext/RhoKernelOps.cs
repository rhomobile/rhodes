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
        private static CRhodesApp RhodesApp { get { return CRhodesApp.Instance; } }

        public static Object __rhoGetCallbackObject(Object/*!*/ self, int index)
        {
            return RhodesApp.getCallbackObject(index); ;
        }

        public static Object __rho_exist_in_resources(Object/*!*/ self, String path)
        {
            return CRhoFile.isResourceFileExist(path);
        }

    }
}
