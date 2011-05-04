using System.IO;
using System.Linq;
using System.Threading;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Threading;
using IsolatedStorageExplorerClient.UI.Resources.Converters;
using IsolatedStorageExplorerClient.UI.Utilities;

namespace IsolatedStorageExplorerClient.UI.Controls.StorageTreeView
{
    /// <summary>
    /// Interaction logic for NewFolderTreeViewItem.xaml
    /// </summary>
    public partial class NewFolderTreeViewItem : TreeViewItem
    {
        private readonly StructuredTreeViewItemHeader _header = new StructuredTreeViewItemHeader();
        private bool _finishedSent = false;
        public NewFolderTreeViewItem()
        {
            InitializeComponent();
            Header = _header;
            _header.IconImage.Source = IconHandler.GetFolderIcon(false);
            var editBlock = new TextBox();
            _header.NameLabel.Content = editBlock;
            editBlock.KeyUp += editBlock_KeyUp;
            editBlock.PreviewTextInput += EditBlockPreviewTextInput;
            editBlock.LostKeyboardFocus += editBlock_LostKeyboardFocus;

        }

        void EditBlockPreviewTextInput(object sender, TextCompositionEventArgs e)
        {
            var chars = Path.GetInvalidPathChars().ToList().Concat(Path.GetInvalidFileNameChars().ToList());
            var textChars = e.Text.ToCharArray();
            foreach (var textChar in textChars)
            {
                if(chars.Contains(textChar))
                {
                    e.Handled = true;
                    base.OnPreviewTextInput(e); 
                }
            }
        }

       

        public event FinishedEditingHandler FinishedEditing;
        public delegate void FinishedEditingHandler(NewFolderTreeViewItem parent, string text);

        public void BeginEdit()
        {
            var textBox = (_header.NameLabel.Content as TextBox);
            textBox.Text = "New Folder";
            textBox.MinWidth = 75;
            FocusHelper.Focus(textBox);
            textBox.SelectAll();
            _finishedSent = false;

        }


        void BroadcastFinishedEditing()
        {
            if (FinishedEditing != null && !_finishedSent)
            {
                _finishedSent = true;
                FinishedEditing(this, (_header.NameLabel.Content as TextBox).Text);
            }
        }

        void editBlock_LostKeyboardFocus(object sender, KeyboardFocusChangedEventArgs e)
        {
            BroadcastFinishedEditing();
        }

        void editBlock_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Return || e.Key == Key.Enter)
            {
                BroadcastFinishedEditing();
            }
        }

    }

    public static class FocusHelper
    {
        public static void Focus(UIElement element)
        {
            if (!element.Focus())
            {
                element.Dispatcher.BeginInvoke(DispatcherPriority.Input, new ThreadStart(delegate()
                    {
                        element.Focus();

                    }));
            }
        }
    }


}
