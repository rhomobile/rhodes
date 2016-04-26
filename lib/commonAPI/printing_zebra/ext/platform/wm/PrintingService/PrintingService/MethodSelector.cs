using System;
using System.Linq;
using System.Collections.Generic;
using System.Text;
using System.Reflection;
using System.MessagingCE;

using Newtonsoft.Json;

using rho.protocol;

namespace rho
{
    class MethodSelector<DefaultMethodClass> 
        where DefaultMethodClass : IQueueCommand, new()
    {
        private string m_asseblyName = null;

        public MethodSelector(string asseblyName)
        {
            m_asseblyName = asseblyName;
        }

        protected Type[] getTypesInNamespace(Assembly assembly, string nameSpace)
        {
            return assembly.GetTypes().Where(t => String.Equals(t.Namespace, nameSpace, StringComparison.Ordinal)).ToArray();
        }

        public IQueueCommand selectMethod(string msgData)
        {
            IQueueCommand method = null;

            Type[] typeList = getTypesInNamespace(Assembly.GetExecutingAssembly(), m_asseblyName);

            foreach (Type currType in typeList)
            {
                if (currType.IsInterface)
                    continue;

                method = (IQueueCommand)Activator.CreateInstance(currType);
                method = (IQueueCommand)JsonConvert.DeserializeObject(msgData, currType);

                if (!method.isNull)
                    break;

                method = null;
            }

            if (method == null)
            {
                method = (IQueueCommand)JsonConvert.DeserializeObject<DefaultMethodClass>(msgData);
            }

            return method;
        }
    }
}
