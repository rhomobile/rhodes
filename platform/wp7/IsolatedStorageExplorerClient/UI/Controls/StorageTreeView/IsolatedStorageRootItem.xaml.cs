using System;
using System.Windows;
using System.Windows.Controls.Primitives;
using IsolatedStorageExplorerClient.ApplicationMonitorService;
using IsolatedStorageExplorerClient.UI.Resources.Converters;
using IsolatedStorageExplorerClient.UI.Utilities;

namespace IsolatedStorageExplorerClient.UI.Controls.StorageTreeView
{
    /// <summary>
    /// Interaction logic for IsolatedStorageRootItem.xaml
    /// </summary>
    public partial class IsolatedStorageRootItem
    {
        /// <summary>
        /// Stores the header control used for this item
        /// </summary>
        private readonly StructuredTreeViewItemHeader _header = new StructuredTreeViewItemHeader();

        /// <summary>
        /// Default constructor
        /// </summary>
        public IsolatedStorageRootItem()
        {
            InitializeComponent();
            Header = _header;
            _header.NameLabel.Content = "Isolated Storage File";
            MouseRightButtonUp += MouseRightButtonUpSelects;
        }

        /// <summary>
        /// Handles right click events to show the context menu and select the item
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        void MouseRightButtonUpSelects(object sender, System.Windows.Input.MouseButtonEventArgs e)
        {
            IsSelected = true;
            ContextMenu.IsOpen = true;
            ContextMenu.Placement = PlacementMode.MousePoint;
            e.Handled = true;
        }
        /// <summary>
        /// Sets the device type shown in the header
        /// </summary>
        public DeviceType DeviceType
        {
            set
            {
                _header.IconImage.Height = 20;
                _header.IconImage.Width = 20;
                _header.IconImage.Source = IconHandler.GetIsolatedStorageFileIcon();
            }
        }

        private void NewFolderMenuItemClick(object sender, RoutedEventArgs e)
        {
            if (AddNewFolderRequested != null)
            {
                AddNewFolderRequested(this);
            }
        }

        private void ExistingItemMenuItemClick(object sender, RoutedEventArgs e)
        {
            if (AddExistingItemRequested != null)
            {
                AddExistingItemRequested(this);
            }
        }

        private void RefreshMenuItemClick(object sender, RoutedEventArgs e)
        {
            if(RefreshRequested!=null)
            {
                RefreshRequested(this);
            }
        }

        /// <summary>
        /// Event triggered when the Add new folder menu item is selected
        /// </summary>
        public event AddNewFolderRequestedHandler AddNewFolderRequested;
        public delegate void AddNewFolderRequestedHandler(IsolatedStorageRootItem sender);


        /// <summary>
        /// Event triggered when the Add Exiting Item menu item is selected
        /// </summary>
        public event AddExistingItemRequestedHandler AddExistingItemRequested;
        public delegate void AddExistingItemRequestedHandler(IsolatedStorageRootItem sender);



        /// <summary>
        /// Event triggered when the Refresh menu item is selected
        /// </summary>
        public event RefreshRequestedHandler RefreshRequested;
        public delegate void RefreshRequestedHandler(IsolatedStorageRootItem sender);
    }
}
