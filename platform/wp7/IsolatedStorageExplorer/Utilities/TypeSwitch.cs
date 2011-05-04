using System;

namespace IsolatedStorageExplorer.Utilities
{
    /// <summary>
    /// Utility class used as a type switch (cases are extension menthods)
    /// </summary>
    public class TypeSwitch
    {
        /// <summary>
        /// Default constructor
        /// </summary>
        public TypeSwitch(Object obj)
        {
            Object = obj;
        }

        /// <summary>
        /// Object used for the switch
        /// </summary>
        public Object Object { get; private set; }

        /// <summary>
        /// Delegate used to handle the case
        /// </summary>
        /// <typeparam name="T">The type that matches</typeparam>
        /// <param name="handler">The handler</param>
        public delegate void HandleCase<T>(T handler);
    }
}