using System.Windows.Controls.Primitives;
using IsolatedStorageExplorerClient.ClientOperationService;
using IsolatedStorageExplorerClient.UI.Utilities;

namespace IsolatedStorageExplorerClient.UI.Controls.StorageTreeView
{
    /// <summary>
    /// Interaction logic for FileTreeViewItem.xaml
    /// </summary>
    public partial class FileTreeViewItem
    {
        /// <summary>
        /// Stores the file information that the current item represents
        /// </summary>
        private FileInformation _mappedFileInfo;

        /// <summary>
        /// Stores the header control used for this item
        /// </summary>
        private readonly StructuredTreeViewItemHeader _header = new StructuredTreeViewItemHeader();

        /// <summary>
        /// Default constructor
        /// </summary>
        public FileTreeViewItem()
        {
            InitializeComponent();
            Header = _header;
            Height = 25;
            MouseRightButtonUp += MouseRightButtonUpSelects;
        }

        /// <summary>
        /// Handles mouse right button up events and shows the context menu
        /// </summary>
        void MouseRightButtonUpSelects(object sender, System.Windows.Input.MouseButtonEventArgs e)
        {
            IsSelected = true;
            ContextMenu.IsOpen = true;
            ContextMenu.Placement = PlacementMode.MousePoint;
            e.Handled = true;
        }

        
        
        /// <summary>
        /// Gets/Sets the file information that this item represents
        /// </summary>
        public FileInformation MappedFile
        {
            get { return _mappedFileInfo; }
            set
            {
                _mappedFileInfo = value;
                UpdateHeaderInformation();
            }
        }

        /// <summary>
        /// Updates the header based on the information available
        /// </summary>
        private void UpdateHeaderInformation()
        {
            if (_mappedFileInfo == null) return;
            _header.NameLabel.Content = _mappedFileInfo.FileName;
            _header.IconImage.Source = IconHandler.GetFileIcon(_mappedFileInfo.FileName);
        }

        /// <summary>
        /// Handles Download File menu item requests
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void DownloadFileMenuItemClick(object sender, System.Windows.RoutedEventArgs e)
        {
            if (DownloadFileRequested != null)
            {
                DownloadFileRequested(this);
            }
        }


        /// <summary>
        /// Handles Delete menu item requests
        /// </summary>
        private void DeleteMenuItemClick(object sender, System.Windows.RoutedEventArgs e)
        {
            if(DeleteRequested!=null)
            {
                DeleteRequested(this);
            }
        }


        /// <summary>
        /// Event triggered when the Download File menu item is selected
        /// </summary>
        public event DownloadFileRequestedHandler DownloadFileRequested;
        public delegate void DownloadFileRequestedHandler(FileTreeViewItem sender);


        /// <summary>
        /// Event triggered when the Delete menu item is selected
        /// </summary>
        public event DeleteRequestedHandler DeleteRequested;
        public delegate void DeleteRequestedHandler(FileTreeViewItem sender);
    }
}
