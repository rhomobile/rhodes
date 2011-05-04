using System;
using IsolatedStorageExplorerClient.ApplicationMonitorService;
using IsolatedStorageExplorerClient.ClientOperationService;
using IsolatedStorageExplorerClient.UI.Controls;

namespace IsolatedStorageExplorerClient.Client
{
    public class ApplicationInstance
    {
        public Guid ApplicationSessionId { get; set; }
        public ApplicationInformation ApplicationInformation { get; set; }
        public IsolatedStorageInformation LatestIsolatedStorageInfo { get; set; }
        public ApplicationExplorer Explorer { get; set; }
    }
}
