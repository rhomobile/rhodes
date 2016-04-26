using System;
using System.Linq;
using System.Collections.Generic;
using System.Text;
using System.Reflection;

using rho.collections;
using rho.protocol.server;

namespace rho
{
    public class MethodRunnable<RunnableClass> where RunnableClass : new()
    {
        protected IMethodHodler m_methodHolder = null;

        public MethodRunnable(IMethodHodler methodHolder)
        {
            m_methodHolder = methodHolder;
        }

        public void run()
        {
            Type type = typeof(RunnableClass);
            MethodInfo method = type.GetMethod(m_methodHolder.Name);

            RunnableClass invokeClass = new RunnableClass();

            method.Invoke(invokeClass, m_methodHolder.Params);
        }

        public void runInObject(RunnableClass runObj)
        {
            if (runObj != null)
            {
                Type type = typeof(RunnableClass);
                MethodInfo method = type.GetMethod(m_methodHolder.Name);

                method.Invoke(runObj, m_methodHolder.Params);
            }
        }
    }

    public class MethodConvertorFactory<RunnableClass> where RunnableClass : new()
    {
        private const string nsNameForFactory = "rho.holder";

        private Type[] getTypesInNamespace(Assembly assembly, string nameSpace)
        {
            return assembly.GetTypes().Where(t => String.Equals(t.Namespace, nameSpace, StringComparison.Ordinal)).ToArray();
        }
        
        private IMethodHodler fillHolderFromClient(rho.protocol.client.IClientMethod clientMethod, IMethodHodlerFill holder)
        {
            holder.fill(clientMethod.Params);
            holder.Name = clientMethod.Name;
            return holder;
        }

        public IMethodHodler convert(rho.protocol.client.IClientMethod serverData)
        {
            if (serverData == null)
                return null;

            Type       type   = typeof(RunnableClass);
            MethodInfo method = type.GetMethod(serverData.Name);

            if (method == null)
                return null;

            ParameterInfo[] methodTypesinfo = method.GetParameters();

            Type[] typeList = getTypesInNamespace(Assembly.GetExecutingAssembly(), nsNameForFactory);

            foreach (Type currType in typeList)
            {
                IMethodHodler holder = (IMethodHodler)Activator.CreateInstance(currType);
                Type[] holderTypes = holder.ParamsTypes;

                if (holderTypes.Length != methodTypesinfo.Length)
                    continue;

                bool isAllEqual = true;
                for (int i=0; i<holderTypes.Length; ++i)
                {
                    string methodParamType = methodTypesinfo[i].ParameterType.AssemblyQualifiedName;
                    string holderParamType = holderTypes[i].AssemblyQualifiedName;

                    if (!methodParamType.Equals(holderParamType))
                        isAllEqual = false;
                }

                if (!isAllEqual)
                    continue;

                return fillHolderFromClient(serverData, (IMethodHodlerFill)holder);
            }

            return null;
        }
    }
}
