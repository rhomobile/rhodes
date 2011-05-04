using System;
using System.ComponentModel;
using System.ServiceModel;
using System.Threading;
using System.Windows.Threading;
using IsolatedStorageExplorerClient.Client.Communication;
using IsolatedStorageExplorerClient.ClientSessionService;

namespace IsolatedStorageExplorerClient.Client
{
    internal sealed partial class ExplorerClient
    {
        /// <summary>
        /// Worker thread used to register the client and ping the server
        /// </summary>
        private readonly BackgroundWorker _registerThread = new BackgroundWorker();

        /// <summary>
        /// Keeps track of the current client
        /// </summary>
        private ClientSessionServiceClient _registrationClient;

        /// <summary>
        /// The current session token (Guid.Empty if not registered)
        /// </summary>
        public Guid SessionToken { get; private set; }

        /// <summary>
        /// Current session state
        /// </summary>
        public SessionState SessionState { get; private set; }

        /// <summary>
        /// The dispatcher used to broadcast events (for UI)
        /// </summary>
        public Dispatcher UiDispatcher { get; set; }


        /// <summary>
        /// Creates the worker thread
        /// </summary>
        private void SetupSessionLogic()
        {
            _registerThread.DoWork += RegisterThreadDoWork;
            _registerThread.WorkerReportsProgress = true;
            _registerThread.WorkerSupportsCancellation = true;
            _registerThread.RunWorkerCompleted += RegisterThreadRunWorkerCompleted;

        }

        void RegisterThreadRunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            _delayConnect = 3000;
            Instance.ResetMonitoringClient();
            Instance.ConnectAndRegister();
        }

        private int _delayConnect = 0;
        /// <summary>
        /// Connect and register logic performed by the background worker
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        void RegisterThreadDoWork(object sender, DoWorkEventArgs e)
        {
            try
            {
                if (Instance._delayConnect != 0)
                {
                    Thread.Sleep(Instance._delayConnect);
                }
                var client = Instance._registrationClient;
                Instance.SessionState = SessionState.Registering;
                BroadcastNewSessionState(SessionState.Registering);
                Instance.SessionToken = client.GetSessionToken();
                Instance.SessionState = SessionState.Registered;
                BroadcastNewSessionState(SessionState.Registered);
                Instance._delayConnect = 0;
                while (!Instance._registerThread.CancellationPending)
                {
                    client.Ping();
                    Thread.Sleep(500);
                }
                if (client.InnerChannel.State == CommunicationState.Opened)
                {
                    client.Close();
                }
            }
            catch (Exception exception)
            {

            }
        }

        /// <summary>
        /// Creates a new session client
        /// </summary>
        private void SetupClient()
        {
            try
            {
                if (_registrationClient != null) return;
                _registrationClient = ClientProvider.GetSessionServiceClient();
                _registrationClient.InnerChannel.Closed += SessionClientInnerChannelClosed;
                _registrationClient.InnerChannel.Closing += SessionClientInnerChannelClosing;
                _registrationClient.InnerChannel.Opened += SessionClientInnerChannelOpened;
                _registrationClient.InnerChannel.Opening += SessionClientInnerChannelOpening;
                _registrationClient.InnerChannel.Faulted += SessionClientInnerChannelFaulted;
            }
            catch (Exception ex)
            {
                var a = ex;
            }
        }

        /// <summary>
        /// Connects to the service and registers the client
        /// </summary>
        public void ConnectAndRegister()
        {
            if (_registerThread.IsBusy) return;
            SetupClient();
            _registerThread.RunWorkerAsync();
        }

        /// <summary>
        /// Disconnects the client
        /// </summary>
        public void Disconnect()
        {
            if (!_registerThread.IsBusy) throw new Exception("Session manager is not connected.");
            _registerThread.CancelAsync();
        }

        /// <summary>
        /// Handles fault states for the session client
        /// </summary>
        void SessionClientInnerChannelFaulted(object sender, EventArgs e)
        {
            Instance.SessionState = SessionState.Faulted;
            BroadcastNewSessionState(SessionState.Faulted);
            ResetSessionClient();
            SetupClient();
        }

        /// <summary>
        /// Cleans resources used by the session client and creates a new instance
        /// </summary>
        private void ResetSessionClient()
        {
            UiDispatcher.Invoke(DispatcherPriority.Render, new Action(() => Applications.Clear()));
            if (_registrationClient != null)
            {
                try
                {
                    _registrationClient.InnerChannel.Closed -= SessionClientInnerChannelClosed;
                    _registrationClient.InnerChannel.Closing -= SessionClientInnerChannelClosing;
                    _registrationClient.InnerChannel.Opened -= SessionClientInnerChannelOpened;
                    _registrationClient.InnerChannel.Opening -= SessionClientInnerChannelOpening;
                    _registrationClient.InnerChannel.Faulted -= SessionClientInnerChannelFaulted;
                }
                catch { }
            }
            _registrationClient = null;
        }

        /// <summary>
        /// Handles Opening events for the session client
        /// </summary>
        void SessionClientInnerChannelOpening(object sender, EventArgs e)
        {
            Instance.SessionState = SessionState.Connecting;

            BroadcastNewSessionState(SessionState.Connecting);
        }

        /// <summary>
        /// Handles Opened events for the session client
        /// </summary>
        void SessionClientInnerChannelOpened(object sender, EventArgs e)
        {
            Instance.SessionState = SessionState.Connected;

            BroadcastNewSessionState(SessionState.Connected);
        }

        /// <summary>
        /// Handles Closing events for the session client
        /// </summary>
        void SessionClientInnerChannelClosing(object sender, EventArgs e)
        {
            Instance.SessionState = SessionState.Closing;

            BroadcastNewSessionState(SessionState.Closing);
        }

        /// <summary>
        /// Handles Closed events for the session client
        /// </summary>
        void SessionClientInnerChannelClosed(object sender, EventArgs e)
        {
            Instance.SessionState = SessionState.Closed;
            BroadcastNewSessionState(SessionState.Closed);
            ResetSessionClient();
            SetupClient();
        }

        /// <summary>
        /// Triggers the session state changed event
        /// </summary>
        private void BroadcastNewSessionState(SessionState newState)
        {
            UiDispatcher.Invoke(DispatcherPriority.Render, new Action<SessionState>((state) =>
            {
                if (Instance.SessionStateChanged != null)
                {
                    Instance.SessionStateChanged(state);
                }
            }), newState);

        }

        /// <summary>
        /// Event triggered when the session state of the client has changed
        /// </summary>
        public event SessionStateChangedHandler SessionStateChanged;
        public delegate void SessionStateChangedHandler(SessionState newState);
    }
}
