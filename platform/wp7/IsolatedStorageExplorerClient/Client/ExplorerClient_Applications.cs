using System;
using System.ComponentModel;
using System.Linq;
using System.ServiceModel;
using System.Threading;
using System.Windows.Threading;
using IsolatedStorageExplorerClient.ApplicationMonitorService;
using IsolatedStorageExplorerClient.Client.Communication;
using IsolatedStorageExplorerClient.ClientOperationService;
using System.Collections.ObjectModel;
using IsolatedStorageExplorerClient.UI.Controls;

namespace IsolatedStorageExplorerClient.Client
{
    internal sealed partial class ExplorerClient : IApplicationMonitorServiceCallback, IClientOperationServiceCallback
    {
        private readonly ObservableCollection<ApplicationInstance> _applications = new ObservableCollection<ApplicationInstance>();
        private ApplicationMonitorServiceClient _monitorClient;
        readonly BackgroundWorker _monitorWorker = new BackgroundWorker();

        /// <summary>
        /// List of application sessions that are running
        /// </summary>
        public ObservableCollection<ApplicationInstance> Applications
        {
            get { return _applications; }
        }

        /// <summary>
        /// Sets up the worker and client used for application monitoring
        /// </summary>
        private void SetupApplicationMonitoring()
        {
            _monitorWorker.DoWork += ApplicationMonitorWorkerDoWork;
            _monitorWorker.WorkerSupportsCancellation = true;
            SessionStateChanged += HandleSessionStateChangedForAppMonitoring;
        }

        /// <summary>
        /// Handles state changes for application monitoring
        /// </summary>
        /// <param name="newstate"></param>
        private void HandleSessionStateChangedForAppMonitoring(SessionState newstate)
        {
            if (newstate == SessionState.Registered)
            {
                StartApplicationSessionMonitoring();
            }
            else
            {
                ResetMonitoringClient();
            }
        }

        /// <summary>
        /// Work performed by the application monitoring worker
        /// </summary>
        private void ApplicationMonitorWorkerDoWork(object sender, DoWorkEventArgs e)
        {
            try
            {
                _monitorClient.StartMonitor(Instance.SessionToken);
                while (true)
                {
                    _monitorClient.Ping();
                    Thread.Sleep(2000);
                }
            }
            catch
            {

            }
        }

        /// <summary>
        /// Starts application session monitoring
        /// </summary>
        private void StartApplicationSessionMonitoring()
        {
            ResetMonitoringClient();
            _monitorWorker.RunWorkerAsync();
        }

        /// <summary>
        /// Resets the client used for application session monitoring
        /// </summary>
        private void ResetMonitoringClient()
        {
            try
            {
                if (_monitorClient != null)
                {
                    _monitorClient.Close();
                }
            }
            catch
            {

            }
            _monitorClient = ClientProvider.GetApplicationMonitorServiceClient(new InstanceContext(this));
        }


        public void SessionStarted(Guid applicationSessionId, ApplicationInformation applicationInformation)
        {
            BroadcastApplicationSessionStart(applicationSessionId, applicationInformation);
        }

        public void SessionLocked(Guid applicationSessionId, bool lockedByCurrentClient)
        {
            if (lockedByCurrentClient) return;
            BroadcastApplicationSessionLocked(applicationSessionId);
        }

        private void BroadcastApplicationSessionLocked(Guid applicationSessionId)
        {
            UiDispatcher.Invoke(DispatcherPriority.Render,
                                new Action<Guid>((appSessionId) =>
                                {

                                    if (Instance.OnApplicationSessionLocked != null)
                                    {
                                        Instance.OnApplicationSessionLocked(appSessionId);
                                    }
                                }), applicationSessionId);
        }

        public void SessionUnlocked(Guid applicationSessionId)
        {
            BroadcastSessionUnlocked(applicationSessionId);
        }

        private void BroadcastSessionUnlocked(Guid applicationSessionId)
        {
            UiDispatcher.Invoke(DispatcherPriority.Render,
                                new Action<Guid>((appSessionId) =>
                                {

                                    if (Instance.OnApplicationSessionUnlocked != null)
                                    {
                                        Instance.OnApplicationSessionUnlocked(appSessionId);
                                    }
                                }), applicationSessionId);
        }

        private void BroadcastApplicationSessionStart(Guid applicationSessionId, ApplicationInformation applicationInformation)
        {
            UiDispatcher.Invoke(DispatcherPriority.Render, new Action<Guid, ApplicationInformation>((sessionId, appinfo) =>
            {
                var newInstance = new ApplicationInstance
                {
                    ApplicationSessionId = sessionId,
                    ApplicationInformation = appinfo,
                    Explorer = new ApplicationExplorer(applicationSessionId)
                };
                Instance.Applications.Add(newInstance);
                if (Instance.OnApplicationSessionStarted != null)
                {
                    Instance.OnApplicationSessionStarted(newInstance.ApplicationSessionId, newInstance);
                }
            }), applicationSessionId, applicationInformation);
        }


        public void SessionEnded(Guid applicationSessionId)
        {
            if (OnApplicationSessionEnded == null) return;

            BroadcastApplicationSessionEnded(applicationSessionId);

        }

        private void BroadcastApplicationSessionEnded(Guid applicationSessionId)
        {
            UiDispatcher.Invoke(DispatcherPriority.Render,
                                new Action<Guid>((appSessionId) =>
                                                     {
                                                         var expiredAppInstance =
                                                             Instance.Applications.Where(
                                                                 app =>
                                                                 app.ApplicationSessionId ==
                                                                 applicationSessionId).
                                                                 FirstOrDefault();
                                                         Instance.Applications.Remove(
                                                             expiredAppInstance);
                                                         if (Instance.OnApplicationSessionEnded != null)
                                                         {
                                                             Instance.OnApplicationSessionEnded(appSessionId);
                                                         }
                                                     }), applicationSessionId);
        }

        /// <summary>
        /// Gets the application instance by application session ID
        /// </summary>
        public ApplicationInstance GetApplicationBySessionId(Guid applicationSessionId)
        {
            return _applications.FirstOrDefault(app => app.ApplicationSessionId == applicationSessionId);
        }





        /// <summary>
        /// Triggered when a new application session has started
        /// </summary>
        public event OnApplicationSessionStartedHandler OnApplicationSessionStarted;
        public delegate void OnApplicationSessionStartedHandler(Guid applicationSessionId, ApplicationInstance applicationInstance);

        /// <summary>
        /// Triggered when an application session has ended
        /// </summary>
        public event OnApplicationSessionEndedHandler OnApplicationSessionEnded;
        public delegate void OnApplicationSessionEndedHandler(Guid applicationSessionId);


        /// <summary>
        /// Triggered when an application session has been locked in a different client
        /// </summary>
        public event OnApplicationSessionLockedHandler OnApplicationSessionLocked;
        public delegate void OnApplicationSessionLockedHandler(Guid applicationSessionId);

        /// <summary>
        /// Triggered when an application session has been unlocked 
        /// </summary>
        public event OnApplicationSessionUnlockedHandler OnApplicationSessionUnlocked;
        public delegate void OnApplicationSessionUnlockedHandler(Guid applicationSessionId);


        public event OperationProgressReceivedHandler OperationProgressReceived;
        public delegate void OperationProgressReceivedHandler(Guid applicationSessionId, OperationProgress progress);

        public void OperationProgress(Guid applicationSessionId, OperationProgress progress)
        {
            BroadcastOperationProgressReceived(applicationSessionId, progress);
        }

        public bool PerformOperation(Operation operation)
        {
            var continueEvent = new AutoResetEvent(false);
            var sent = false;
            var sendThread = new Thread(() =>
                                            {
                                                try
                                                {
                                                    var client = ClientProvider.GetClientOperationServiceClient(new InstanceContext(this));
                                                    client.StartOperationSession(SessionToken);
                                                    client.PostOperation(operation);
                                                    sent = true;
                                                }
                                                catch { }
                                                continueEvent.Set();
                                            }) { IsBackground = true };
            sendThread.Start();
            continueEvent.WaitOne();
            return sent;
        }


        private void BroadcastOperationProgressReceived(Guid applicationSessionId, OperationProgress progress)
        {
            UiDispatcher.Invoke(DispatcherPriority.Render, new Action<Guid, OperationProgress>((appId, reportedProgress) =>
            {
                if (Instance.OperationProgressReceived != null)
                {
                    Instance.OperationProgressReceived(appId, reportedProgress);
                }
            }), applicationSessionId, progress);
        }
    }
}
