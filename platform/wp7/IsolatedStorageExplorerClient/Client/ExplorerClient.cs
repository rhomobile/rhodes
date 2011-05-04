namespace IsolatedStorageExplorerClient.Client
{
    /// <summary>
    /// Singleton class representing the isolated storage explorer API client
    /// </summary>
    internal sealed partial class ExplorerClient
    {
        static ExplorerClient _instance;
        static readonly object Padlock = new object();
        ExplorerClient()
        {
            SessionState = SessionState.Closed;
            SetupSessionLogic();
            SetupApplicationMonitoring();
        }

        

        public static ExplorerClient Instance
        {
            get
            {
                lock (Padlock)
                {
                    return _instance ?? (_instance = new ExplorerClient());
                }
            }
        }


       
    }
}
