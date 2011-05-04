using System;
using System.ComponentModel;
using System.Threading;
using System.Windows;
using IsolatedStorageExplorer.ApplicationOperationsService;
using IsolatedStorageExplorer.Clients;
using IsolatedStorageExplorer.Utilities;
using Microsoft.Phone.Shell;

namespace IsolatedStorageExplorer
{
    public sealed class Explorer
    {
        /// <summary>
        /// Current instance of the explorer
        /// </summary>
        static Explorer _instance;

        /// <summary>
        /// Value indicating how much the wait between connection attempts
        /// </summary>
        private int _reconnectPause = 0;


        /// <summary>
        /// Value indicating the explorer start
        /// </summary>
        private bool _started = false;

        /// <summary>
        /// Lock used for simple thread safety
        /// </summary>
        static readonly object Padlock = new object();

        /// <summary>
        /// Worker thread that performs the explorer operations
        /// </summary>
        readonly BackgroundWorker _explorerWorker = new BackgroundWorker();

        /// <summary>
        /// Event used to signal the explorer to exit
        /// </summary>
        private readonly AutoResetEvent _explorerExitEvent = new AutoResetEvent(false);

        /// <summary>
        /// Gets the current client factory
        /// </summary>
        private ClientFactory Clients { get; set; }

        /// <summary>
        /// Stores the current session ID
        /// </summary>
        public Guid SessionId
        {
            get;
            private set;
        }

        /// <summary>
        /// Stores the hostname used by the current explorer session
        /// </summary>
        public string Hostname
        {
            get;
            private set;
        }

        /// <summary>
        /// Gets the current thread safe instance
        /// </summary>
        private static Explorer Instance
        {
            get
            {
                lock (Padlock)
                {
                    return _instance ?? (_instance = new Explorer());
                }
            }
        }
        /// <summary>
        /// Default constructor
        /// </summary>
        Explorer()
        {
            try
            {
                PhoneApplicationService.Current.Deactivated += PhoneApplicationServiceDeactivated;
            }
            catch { }
            _explorerWorker.DoWork += ExplorerWorkerDoWork;
            _explorerWorker.RunWorkerCompleted += ExplorerWorkerRunWorkerCompleted;
        }

        void ExplorerWorkerRunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            Instance._reconnectPause = 1000;
            Start(Hostname);
        }

        void PhoneApplicationServiceDeactivated(object sender, DeactivatedEventArgs e)
        {
            try
            {
                PhoneApplicationService.Current.State["wp7IsolatedStorageExplorerState"] = new ExplorerState
                                                                                               {
                                                                                                   Hostname = Hostname,
                                                                                                   Started =
                                                                                                       Instance._started
                                                                                               };
            }
            catch { }

        }



        /// <summary>
        /// Start the explorer client
        /// </summary>
        /// <param name="hostname">The IP or hostname of the service host computer</param>
        public static void Start(string hostname)
        {
            if (string.IsNullOrEmpty(hostname)) return;
            Instance.Hostname = hostname;
            Instance._started = true;
            Instance.Clients = new ClientFactory(Instance.Hostname);
            Instance._explorerWorker.RunWorkerAsync(Instance.Hostname);
        }

        /// <summary>
        /// Work performed by the explorer worker
        /// </summary>
        void ExplorerWorkerDoWork(object sender, DoWorkEventArgs e)
        {

            try
            {
                if (Instance._reconnectPause != 0) Thread.Sleep(Instance._reconnectPause);
                var sessionId = Guid.Empty;
                var reset = new AutoResetEvent(false);
                var client = Instance.Clients.GetApplicationSessionServiceClient();
                client.GetApplicationSessionIdCompleted += (s, args) =>
                                                               {
                                                                   sessionId = args.Result;
                                                                   reset.Set();
                                                               };
                client.GetApplicationSessionIdAsync(AppInformation.ApplicationInformation);
                reset.WaitOne();
                if (sessionId == Guid.Empty) return;
                Instance.SessionId = sessionId;
                var beatStarted = StartHeartBeat();
                if (!beatStarted) return;
                StartOperationsPolling();
                Instance._explorerExitEvent.WaitOne();
            }
            catch (Exception)
            {

            }
        }


        /// <summary>
        /// Creates a client that polls for 
        /// </summary>
        private void StartOperationsPolling()
        {
            var operationsWorker = new BackgroundWorker();
            operationsWorker.DoWork += OperationsWorkerDoWork;
            operationsWorker.WorkerSupportsCancellation = true;
            operationsWorker.RunWorkerAsync();
        }

        /// <summary>
        /// Polls for new operations and performs them
        /// </summary>
        private void OperationsWorkerDoWork(object sender, DoWorkEventArgs e)
        {
            try
            {
                while (!e.Cancel && !_explorerWorker.CancellationPending && _explorerWorker.IsBusy)
                {
                    try
                    {
                        var client = Instance.Clients.GetApplicationOperationsServiceClient();
                        Operation operationToPerform = null;
                        var canContinueEvent = new AutoResetEvent(false);
                        client.GetNextOperationCompleted += (s, args) =>
                                                                {
                                                                    if (args.Error == null)
                                                                        operationToPerform = args.Result;
                                                                    canContinueEvent.Set();
                                                                };
                        client.GetNextOperationAsync(Instance.SessionId);
                        canContinueEvent.WaitOne();
                        if (operationToPerform == null) continue;
                        var performer = new OperationPerformer(operationToPerform,
                                                               sender as BackgroundWorker,
                                                               Clients)
                                            {
                                                OperationReportedProgress = PostOperationProgress
                                            };
                        performer.PerformOperation();


                    }
                    catch
                    {
                    }
                }
            }
            catch (Exception)
            {

            }
        }

        /// <summary>
        /// Posts operation progress reported by the performers
        /// </summary>
        private static void PostOperationProgress(OperationProgress progress)
        {
            try
            {
                var completed = new AutoResetEvent(false);
                var client = Instance.Clients.GetApplicationOperationsServiceClient();
                client.PostOperationProgressCompleted += (s, e) => completed.Set();
                client.PostOperationProgressAsync(Instance.SessionId, progress);
                completed.WaitOne(3000);
            }
            catch
            {
                return;
            }
        }


        /// <summary>
        /// Starts the heartbeat client to keep the session alive.
        /// </summary>
        private bool StartHeartBeat()
        {
            var beatStarted = false;
            var waitForBeat = new AutoResetEvent(false);
            var client = Instance.Clients.GetApplicationSessionServiceClient();
            client.HearbeatCompleted += (s, e) =>
                                            {
                                                if (!_explorerWorker.IsBusy) return;
                                                if (e.Error != null)
                                                {
                                                    Instance._explorerExitEvent.Set();
                                                    return;
                                                }
                                                beatStarted = true;
                                                waitForBeat.Set();
                                                client.HearbeatAsync(SessionId);
                                            };
            client.HearbeatAsync(SessionId);
            waitForBeat.WaitOne();
            return beatStarted;
        }



        /// <summary>
        /// Reconnects the explorer, restoring host name and state
        /// </summary>
        public static void RestoreFromTombstone()
        {
            if (!Instance._started)
                try
                {
                    var lastState = PhoneApplicationService.Current.State["wp7IsolatedStorageExplorerState"] as ExplorerState;
                    if (lastState != null)
                    {
                        if (lastState.Started) Start(lastState.Hostname);
                    }
                }
                catch { }
        }




    }
}
