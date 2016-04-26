
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Reflection;
using System.Collections;

using rho.collections;

namespace rho
{
namespace protocol
{
namespace server
{
    public interface IMethodHodler
    {
        object[] Params { get; }

        string Name { get; set; }

        Type[] ParamsTypes { get; }

        rho.protocol.shared.IMethodResult Result { get; }
    }

    public interface IMethodHodlerFill : IMethodHodler
    {
        void fill(object[] parameters);
    }
}
}
}
