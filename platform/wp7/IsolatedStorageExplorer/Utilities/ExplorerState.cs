using System.Runtime.Serialization;

namespace IsolatedStorageExplorer.Utilities
{
    [DataContract]
    public class ExplorerState
    {
        [DataMember]
        public bool Started { get; set; }

        [DataMember]
        public string Hostname { get; set; }
    }
}
