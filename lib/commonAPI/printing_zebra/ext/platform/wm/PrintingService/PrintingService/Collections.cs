using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Collections;

namespace rho
{
    namespace collections
    {
        public class IReadOnlyDictionary<K, V> : Dictionary<K, V>
        {
        }
        
        public class IReadOnlyList<V> : List<V>
        {
        }
    }
}
