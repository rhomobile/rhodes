using System.Windows.Controls.Primitives;
using IsolatedStorageExplorerClient.ClientOperationService;
using IsolatedStorageExplorerClient.UI.Utilities;

namespace IsolatedStorageExplorerClient.UI.Controls.StorageTreeView
{
    /// <summary>
    /// Control used to represent a directory in the explorer tree view
    /// </summary>
    public partial class DirectoryTreeItem
    {
        /// <summary>
        /// Stores the directory information
        /// </summary>
        private DirectoryInformation _mappedDirectory;

        /// <summary>
        /// Header control for the directory tree item
        /// </summary>
        private readonly StructuredTreeViewItemHeader _header = new StructuredTreeViewItemHeader();

        /// <summary>
        /// Default constructor
        /// </summary>
        public DirectoryTreeItem()
        {
            InitializeComponent();
            Header = _header;
            Expanded += DirectoryTreeItemExpandedCollapsed;
            Collapsed += DirectoryTreeItemExpandedCollapsed;
            MouseRightButtonUp += MouseRightButtonUpSelects;
        }

        /// <summary>
        /// Selects the tree item and shows the context menu on right click
        /// </summary>
        void MouseRightButtonUpSelects(object sender, System.Windows.Input.MouseButtonEventArgs e)
        {
            IsSelected = true;
            ContextMenu.IsOpen = true;
            ContextMenu.Placement = PlacementMode.MousePoint;
            e.Handled = true;
        }

        /// <summary>
        /// Updates the header information when the item is expanded or collapsed
        /// </summary>
        void DirectoryTreeItemExpandedCollapsed(object sender, System.Windows.RoutedEventArgs e)
        {
            UpdateHeader();
        }

       
        
        /// <summary>
        /// Gets/Sets the directory information that the current item represents
        /// </summary>
        public DirectoryInformation MappedDirectory
        {
            get { return _mappedDirectory; }
            set
            {
                _mappedDirectory = value;
                UpdateHeader();
            }
        }

        /// <summary>
        /// Updates the header based on the information available
        /// </summary>
        private void UpdateHeader()
        {
            if (_mappedDirectory == null) return;
            _header.NameLabel.Content = _mappedDirectory.Name;
            _header.IconImage.Source = IconHandler.GetFolderIcon(IsExpanded);
        }

        /// <summary>
        /// Handles Add new folder menu item requests
        /// </summary>
        private void NewFolderMenuItemClick(object sender, System.Windows.RoutedEventArgs e)
        {
            if (AddNewFolderRequested != null)
            {
                AddNewFolderRequested(this);
            }
        }

        /// <summary>
        /// Handles Add Existing menu item requests
        /// </summary>
        private void ExistingItemMenuItemClick(object sender, System.Windows.RoutedEventArgs e)
        {
            if (AddExistingItemRequested != null)
            {
                AddExistingItemRequested(this);
            }
        }

        /// <summary>
        /// Handles Delete menu item requests
        /// </summary>
        private void DeleteMenuItemClick(object sender, System.Windows.RoutedEventArgs e)
        {
            if (DeleteRequested != null)
            {
                DeleteRequested(this);
            }
        }


        /// <summary>
        /// Event triggered when the Add new folder menu item is selected
        /// </summary>
        public event AddNewFolderRequestedHandler AddNewFolderRequested;
        public delegate void AddNewFolderRequestedHandler(DirectoryTreeItem parent);


        /// <summary>
        /// Event triggered when the Add Exiting Item menu item is selected
        /// </summary>
        public event AddExistingItemRequestedHandler AddExistingItemRequested;
        public delegate void AddExistingItemRequestedHandler(DirectoryTreeItem parent);


        /// <summary>
        /// Event triggered when the Delete menu item is selected
        /// </summary>
        public event DeleteRequestedHandler DeleteRequested;
        public delegate void DeleteRequestedHandler(DirectoryTreeItem parent);

    }
}
