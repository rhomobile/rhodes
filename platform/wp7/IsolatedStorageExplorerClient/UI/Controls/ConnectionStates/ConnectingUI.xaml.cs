using System;
using System.Windows.Controls;
using IsolatedStorageExplorerClient.Client;

namespace IsolatedStorageExplorerClient.UI.Controls.ConnectionStates
{
    /// <summary>
    /// Interaction logic for ConnectingUI.xaml
    /// </summary>
    public partial class ConnectingUI
    {
        public ConnectingUI()
        {
            InitializeComponent();
            ExplorerClient.Instance.SessionStateChanged += ExplorerSessionStateChanged;
        }

        private void ExplorerSessionStateChanged(SessionState newstate)
        {
            switch (newstate)
            {
                case SessionState.Connecting:
                    this.messageLabel.Content = "Connecting to the service...";
                    break;
                case SessionState.Connected:
                    this.messageLabel.Content = "Connected to the service...";
                    break;
                case SessionState.Registering:
                    this.messageLabel.Content = "Registering the explorer...";
                    break;
                case SessionState.Registered:
                    this.messageLabel.Content = "Registered as an explorer...";
                    break;
                case SessionState.Faulted:
                    break;
                case SessionState.Closing:
                    break;
                case SessionState.Closed:
                    break;
                default:
                    throw new ArgumentOutOfRangeException("newstate");
            }
        }
    }
}
