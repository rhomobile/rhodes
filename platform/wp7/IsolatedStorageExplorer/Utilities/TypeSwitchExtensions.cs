namespace IsolatedStorageExplorer.Utilities
{
    /// <summary>
    /// Extension method collection for the type switch
    /// </summary>
    public static class TypeSwitchExtensions
    {
        /// <summary>
        /// Case method used by the type switch
        /// </summary>
        public static TypeSwitch Case<T>(this TypeSwitch s, TypeSwitch.HandleCase<T> handler)
            where T : class
        {
            if (s == null) return null;
            if (s.Object is T)
            {
                if (handler != null)
                {
                    handler(s.Object as T);
                    return null;
                }
            }
            return s;
        }


    }
}