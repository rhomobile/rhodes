using System.ComponentModel;
using System.Windows.Controls;
using IsolatedStorageExplorerClient.Client;
using IsolatedStorageExplorerClient.UI.Controls;
using IsolatedStorageExplorerClient.UI.Controls.ConnectionStates;

namespace IsolatedStorageExplorerClient.UI
{
    /// <summary>
    /// Interaction logic for ExplorerWindow.xaml
    /// </summary>
    public partial class ExplorerWindow
    {
        private bool _firstRun = true;

        public ExplorerWindow()
        {
            InitializeComponent();
            applicationCombo.SelectionChanged += ApplicationComboSelectionChanged;
            applicationCombo.DataContext = ExplorerClient.Instance.Applications;
            ExplorerClient.Instance.Applications.CollectionChanged += ApplicationsCollectionChanged;
            ExplorerClient.Instance.SessionStateChanged += ExplorerSessionStateChanged;
            ExplorerClient.Instance.UiDispatcher = Dispatcher;
            HeaderGrid.Visibility = System.Windows.Visibility.Collapsed;
            contentScrollviewer.Content = new ConnectingUI();
            if (!DesignerProperties.GetIsInDesignMode(this))
            {
                ExplorerClient.Instance.ConnectAndRegister();
            }
            InitializeUi();
        }

        void ExplorerSessionStateChanged(SessionState newState)
        {
            if (newState == SessionState.Registered)
            {
                _firstRun = false;
                HeaderGrid.Visibility = System.Windows.Visibility.Visible;
                contentScrollviewer.Content = new NoConnectedClientsUI();
            }

            if (newState == SessionState.Faulted || newState == SessionState.Closed || newState == SessionState.Closing)
            {
                if (HeaderGrid.Visibility != System.Windows.Visibility.Collapsed)
                    HeaderGrid.Visibility = System.Windows.Visibility.Collapsed;
                if (!(contentScrollviewer.Content is DisconnectedUI))
                    contentScrollviewer.Content = new DisconnectedUI();
            }
        }

        private void InitializeUi()
        {
            if (ExplorerClient.Instance.SessionState == SessionState.Registered)
            {
                applicationSelectionGrid.Visibility = System.Windows.Visibility.Visible;
                applicationCombo.SelectedIndex = 0;
            }
        }

        private ApplicationInstance GetSelectedApplicationInstance()
        {
            return (applicationCombo.SelectedValue as ApplicationInstance);
        }

        /// <summary>
        /// Handles the combobox selection for applications
        /// </summary>
        void ApplicationsCollectionChanged(object sender, System.Collections.Specialized.NotifyCollectionChangedEventArgs e)
        {
            if (ExplorerClient.Instance.Applications.Count != 0 && GetSelectedApplicationInstance() == null)
            {
                applicationCombo.SelectedIndex = 0;
            }

            if (ExplorerClient.Instance.Applications.Count == 0 && !(contentScrollviewer.Content is DisconnectedUI))
            {
                contentScrollviewer.Content = new NoConnectedClientsUI();
            }
        }

        /// <summary>
        /// Switch between application instances
        /// </summary>
        void ApplicationComboSelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (ExplorerClient.Instance.SessionState != SessionState.Registered) return;
            var selectedApplicationInstance = GetSelectedApplicationInstance();
            if (selectedApplicationInstance == null)
            {
                if (ExplorerClient.Instance.Applications.Count != 0)
                {
                    applicationCombo.SelectedIndex = 0;
                    var newSelectedApplicationInstance = GetSelectedApplicationInstance();
                    if (newSelectedApplicationInstance == null)
                    {
                        contentScrollviewer.Content = new NoConnectedClientsUI(); 
                    }
                    else
                    {
                        contentScrollviewer.Content = newSelectedApplicationInstance.Explorer;
                    }
                }
            }
            else
            {
                contentScrollviewer.Content = selectedApplicationInstance.Explorer;
            }
        }










    }
}
