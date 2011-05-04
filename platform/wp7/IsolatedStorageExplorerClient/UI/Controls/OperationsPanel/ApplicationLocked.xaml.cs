using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using IsolatedStorageExplorerClient.Client;

namespace IsolatedStorageExplorerClient.UI.Controls.OperationsPanel
{
    /// <summary>
    /// Interaction logic for ApplicationLocked.xaml
    /// </summary>
    public partial class ApplicationLocked : UserControl
    {
        private Guid _applicationId;
        private TreeView _tree = null;

        public ApplicationLocked()
        {
            InitializeComponent();
        }



        public void Show(double panelWidth)
        {
            Measure(new Size(panelWidth, double.MaxValue));
            var size = DesiredSize;
            var animation = new DoubleAnimation
            {
                To = size.Height,
                From = 0,
                Duration = TimeSpan.FromMilliseconds(200)
            };
            BeginAnimation(HeightProperty, animation);
        }


        public void Hide()
        {
            var animation = new DoubleAnimation { To = 0, Duration = TimeSpan.FromMilliseconds(200) };
            BeginAnimation(HeightProperty, animation);
        }

        public void ShowLocked(ApplicationInstance currentApplication, double actualWidth, TreeView storageTree)
        {
            _applicationId = currentApplication.ApplicationSessionId;
            _tree = storageTree;
            if (_tree != null) _tree.IsEnabled = false;
            ExplorerClient.Instance.OnApplicationSessionUnlocked += Instance_OnApplicationSessionUnlocked;
            Show(actualWidth);
        }

        void Instance_OnApplicationSessionUnlocked(Guid applicationSessionId)
        {
            if (_applicationId != applicationSessionId) return;
            if (_tree != null) _tree.IsEnabled = true;
            ExplorerClient.Instance.OnApplicationSessionUnlocked -= Instance_OnApplicationSessionUnlocked;
            Hide();
        }
    }
}
