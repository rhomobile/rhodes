using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using rhoruntime;

namespace RhoRuntimeInvoker
{
    [Windows.Foundation.Metadata.AllowForWeb]
    public sealed class RuntimeInvoker
    {
        public String invoke(String cmd)
        {
            System.Diagnostics.Debug.WriteLine("Invoking JS code: " + cmd);
            String str = CRhoRuntime.getInstance().onJSInvoke(cmd);
            System.Diagnostics.Debug.WriteLine(cmd + " : " + str);
            return str;
        }
    }    

}