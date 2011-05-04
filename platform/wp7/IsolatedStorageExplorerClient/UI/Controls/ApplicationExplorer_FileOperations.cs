using System;
using System.IO;
using System.Windows;
using IsolatedStorageExplorerClient.UI.Controls.OperationsPanel;
using IsolatedStorageExplorerClient.UI.Controls.StorageTreeView;
using Microsoft.Win32;

namespace IsolatedStorageExplorerClient.UI.Controls
{
    public partial class ApplicationExplorer
    {
        void match_DownloadFileRequested(FileTreeViewItem sender)
        {
            var saveFileDialog = new SaveFileDialog
                                     {
                                         FileName = sender.MappedFile.FileName,
                                         OverwritePrompt = true,
                                     };
            if (saveFileDialog.ShowDialog() != true) return;
            try
            {
                var stream = saveFileDialog.OpenFile();
                messageBoxContainer.Children.Clear();
                var operation = new DownloadFileOperation();
                messageBoxContainer.Children.Add(operation);
                operation.DownloadFile(sender.MappedFile.Path, stream,
                    CurrentApplication, messageBoxContainer.ActualWidth,
                                       storageTreeView);

            }
            catch (Exception)
            {
                MessageBox.Show("Saving the file has failed.\nChoose a different save location and try again.",
                    "WP7 Isolated Storage Explorer", MessageBoxButton.OK, MessageBoxImage.Error);
            }


        }

        void match_DeleteRequested(FileTreeViewItem sender)
        {
            var result =
                MessageBox.Show(
                    "'" + sender.MappedFile.FileName + "' will be deleted permanently.",
                    "WP7 Isolated Storage Explorer", MessageBoxButton.OKCancel, MessageBoxImage.Exclamation);
            if (result != MessageBoxResult.OK) return;
            messageBoxContainer.Children.Clear();
            var operation = new DeleteFileOperation();
            messageBoxContainer.Children.Add(operation);
            operation.DeleteFile(sender.MappedFile, CurrentApplication, messageBoxContainer.ActualWidth,
                                      storageTreeView);
        }
    }
}