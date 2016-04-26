using System;
using System.Linq;
using System.Collections.Generic;
using System.Text;

using rho;
using rho.collections;
using rho.protocol.server;
using rho.protocol.shared;

namespace rho
{
    namespace holder
    {
        public class OnlyResult : IMethodHodlerFill
        {
            IMethodResult m_result;

            public string Name { get; set; }

            public OnlyResult() {}

            public OnlyResult(IMethodResult result)
            {
                m_result = result;
            }

            public object[] Params
            {
                get
                {
                    object[] _params = new object[] {m_result};
                    return _params;
                }
            }

            public Type[] ParamsTypes 
            { 
                get
                {
                    Type[] types = new Type[] { typeof(IMethodResult) };
                    return types;
                }
            }

            public void fill(object[] parameters)
            {
                m_result = (IMethodResult)parameters[0];
            }

            public IMethodResult Result
            {
                get
                {
                    return m_result;
                }
            }
        }

        public class StringListAndResult : IMethodHodlerFill
        {
            string m_string;
            IReadOnlyList<string> m_list;
            IMethodResult m_result;

            public string Name { get; set; }

            public StringListAndResult() { }

            public StringListAndResult(string str, IReadOnlyList<string> list, IMethodResult result)
            {
                m_string = str;
                m_list = list;
                m_result = result;
            }

            public object[] Params
            {
                get
                {
                    object[] _params = new object[3];

                    _params[0] = m_string;
                    _params[1] = m_list;
                    _params[2] = m_result;

                    return _params;
                }
            }

            public Type[] ParamsTypes
            {
                get
                {
                    Type[] types = new Type[] { typeof(string), typeof(IReadOnlyList<string>), typeof(IMethodResult) };
                    return types;
                }
            }

            public void fill(object[] parameters)
            {
                m_string = (string)parameters[0];
                m_list = (IReadOnlyList<string>)parameters[1];
                m_result = (IMethodResult)parameters[2];
            }

            public IMethodResult Result
            {
                get
                {
                    return m_result;
                }
            }
        }

        public class DictionaryAndResult : IMethodHodlerFill
        {
            IReadOnlyDictionary<string, string> m_dictinary;
            IMethodResult                       m_result;

            public string Name { get; set; }

            public DictionaryAndResult() {}

            public DictionaryAndResult(IReadOnlyDictionary<string, string> dict, IMethodResult result)
            {
                m_dictinary = dict;
                m_result    = result;
            }

            public object[] Params
            {
                get
                {
                    object[] _params = new object[2];

                    _params[0] = m_dictinary;
                    _params[1] = m_result;

                    return _params;
                }
            }

            public Type[] ParamsTypes 
            { 
                get
                {
                    Type[] types = new Type[] { typeof(IReadOnlyDictionary<string, string>), typeof(IMethodResult) };
                    return types;
                }
            }

            public void fill(object[] parameters)
            {
                m_dictinary = (IReadOnlyDictionary<string, string>)parameters[0];
                m_result    = (IMethodResult)parameters[1];
            }

            public IMethodResult Result 
            {
                get
                {
                    return m_result;
                }
            }
        }

        public class DictnaryAndStringAndResult : IMethodHodlerFill
        {
            string                              m_string;
            IReadOnlyDictionary<string, string> m_dictinary;
            IMethodResult                       m_result;

            public string Name { get; set; }

            public DictnaryAndStringAndResult() { }

            public DictnaryAndStringAndResult(string rhoString, IReadOnlyDictionary<string, string> dict, IMethodResult result)
            {
                m_string    = rhoString;
                m_dictinary = dict;
                m_result    = result;
            }

            public object[] Params
            {
                get
                {
                    object[] _params = new object[3];

                    _params[0] = m_string;
                    _params[1] = m_dictinary;
                    _params[2] = m_result;

                    return _params;
                }
            }

            public Type[] ParamsTypes
            {
                get
                {
                    Type[] types = new Type[] { typeof(string), typeof(IReadOnlyDictionary<string, string>), typeof(IMethodResult) };
                    return types;
                }
            }

            public void fill(object[] parameters)
            {
                m_string    = (string)parameters[0];
                m_dictinary = (IReadOnlyDictionary<string, string>)parameters[1];
                m_result    = (IMethodResult)parameters[2];
            }

            public IMethodResult Result
            {
                get
                {
                    return m_result;
                }
            }
        }

        public class StringStringIntIntAndResult : IMethodHodlerFill
        {
            string m_string;
            string m_string2;
            int m_int;
            int m_int2;
            IMethodResult m_result;

            public string Name { get; set; }

            public StringStringIntIntAndResult() { }

            public StringStringIntIntAndResult(string rhoString, string rhoString2, int rhoInt1, int rhoInt2, IMethodResult result)
            {
                m_string = rhoString;
                m_string2 = rhoString2;
                m_int = rhoInt1;
                m_int2 = rhoInt2;
                m_result = result;
            }

            public object[] Params
            {
                get
                {
                    object[] _params = new object[5];

                    _params[0] = m_string;
                    _params[1] = m_string2;
                    _params[2] = m_int;
                    _params[3] = m_int2;
                    _params[4] = m_result;

                    return _params;
                }
            }

            public Type[] ParamsTypes
            {
                get
                {
                    Type[] types = new Type[] { typeof(string), typeof(string), typeof(int), typeof(int), typeof(IMethodResult) };
                    return types;
                }
            }

            public void fill(object[] parameters)
            {
                m_string  = (string)parameters[0];
                m_string2 = (string)parameters[1];
                m_int     = (int)parameters[2];
                m_int2    = (int)parameters[3];
                m_result  = (IMethodResult)parameters[4];
            }

            public IMethodResult Result
            {
                get
                {
                    return m_result;
                }
            }
        }

        public class StringAndResult : IMethodHodlerFill
        {
            string m_string;
            IMethodResult m_result;

            public string Name { get; set; }

            public StringAndResult() { }

            public StringAndResult(string rhoString, IMethodResult result)
            {
                m_string = rhoString;
                m_result = result;
            }

            public object[] Params
            {
                get
                {
                    object[] _params = new object[2];

                    _params[0] = m_string;
                    _params[1] = m_result;

                    return _params;
                }
            }

            public Type[] ParamsTypes
            {
                get
                {
                    Type[] types = new Type[] { typeof(string), typeof(IMethodResult) };
                    return types;
                }
            }

            public void fill(object[] parameters)
            {
                m_string = (string)parameters[0];
                m_result = (IMethodResult)parameters[1];
            }

            public IMethodResult Result
            {
                get
                {
                    return m_result;
                }
            }
        }

        public class IntAndResult : IMethodHodlerFill
        {
            int           m_int;
            IMethodResult m_result;

            public string Name { get; set; }

            public IntAndResult() { }

            public IntAndResult(int rhoInt, IMethodResult result)
            {
                m_int = rhoInt;
                m_result = result;
            }

            public object[] Params
            {
                get
                {
                    object[] _params = new object[2];

                    _params[0] = m_int;
                    _params[2] = m_result;

                    return _params;
                }
            }

            public Type[] ParamsTypes
            {
                get
                {
                    Type[] types = new Type[] { typeof(int), typeof(IMethodResult) };
                    return types;
                }
            }

            public void fill(object[] parameters)
            {
                m_int    = (int)parameters[0];
                m_result = (IMethodResult)parameters[2];
            }

            public IMethodResult Result
            {
                get
                {
                    return m_result;
                }
            }
        }

        public class StringIntIntHashAndResult : IMethodHodlerFill
        {
            string                              m_string;
            int                                 m_int;
            int                                 m_int2;
            IReadOnlyDictionary<string, string> m_dictinary;
            IMethodResult                       m_result;

            public string Name { get; set; }

            public StringIntIntHashAndResult() { }

            public StringIntIntHashAndResult(string rhoString, int rhoInt, int rhoInt2, IReadOnlyDictionary<string, string> rhoHash, IMethodResult result)
            {
                m_string    = rhoString;
                m_int       = rhoInt;
                m_int2      = rhoInt2;
                m_dictinary = rhoHash;
                m_result    = result;
            }

            public object[] Params
            {
                get
                {
                    object[] _params = new object[5];

                    _params[0] = m_string;
                    _params[1] = m_int;
                    _params[2] = m_int2;
                    _params[3] = m_dictinary;
                    _params[4] = m_result;

                    return _params;
                }
            }

            public Type[] ParamsTypes
            {
                get
                {
                    Type[] types = new Type[] { typeof(string), typeof(int), typeof(int), typeof(IReadOnlyDictionary<string, string>), typeof(IMethodResult) };
                    return types;
                }
            }

            public void fill(object[] parameters)
            {
                m_string    = (string)parameters[0];
                m_int       = (int)parameters[1];
                m_int2      = (int)parameters[2];
                m_dictinary = (IReadOnlyDictionary<string, string>)parameters[3];
                m_result    = (IMethodResult)parameters[4];
            }

            public IMethodResult Result
            {
                get
                {
                    return m_result;
                }
            }
        }
     
        public class ListAndResult : IMethodHodlerFill
        {
            IReadOnlyList<string> m_list;
            IMethodResult         m_result;

            public string Name { get; set; }

            public ListAndResult() {}

            public ListAndResult(IReadOnlyList<string> list, IMethodResult result)
            {
                m_list   = list;
                m_result = result;
            }

            public object[] Params
            {
                get
                {
                    object[] _params = new object[2];

                    _params[0] = m_list;
                    _params[1] = m_result;

                    return _params;
                }
            }

            public Type[] ParamsTypes 
            { 
                get
                {
                    Type[] types = new Type[] { typeof(IReadOnlyList<string>), typeof(IMethodResult) };
                    return types;
                }
            }

            public void fill(object[] parameters)
            {
                m_list   = (IReadOnlyList<string>)parameters[0];
                m_result = (IMethodResult)parameters[1];
            }

            public IMethodResult Result 
            {
                get
                {
                    return m_result;
                }
            }
        }        
    }
}
