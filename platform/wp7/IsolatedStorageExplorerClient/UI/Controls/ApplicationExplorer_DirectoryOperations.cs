using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Linq;
using System.Text;
using System.Windows;
using IsolatedStorageExplorerClient.UI.Controls.OperationsPanel;
using IsolatedStorageExplorerClient.UI.Controls.StorageTreeView;
using Microsoft.Win32;

namespace IsolatedStorageExplorerClient.UI.Controls
{
    public partial class ApplicationExplorer
    {
        /// <summary>
        /// Handles Add Existing Item requests from a directory tree item
        /// </summary>
        void DirectoryAddExistingItemRequested(DirectoryTreeItem parent)
        {
            var selectFileDialog = new OpenFileDialog { Multiselect = false };
            if (selectFileDialog.ShowDialog() != true) return;
            var file = selectFileDialog.FileName;
            var fileStream = selectFileDialog.OpenFile();
            messageBoxContainer.Children.Clear();
            parent.IsExpanded = true;
            var op = new UploadFileOperation();
            messageBoxContainer.Children.Add(op);
            op.UploadFile(Path.Combine(parent.MappedDirectory.Path, Path.GetFileName(file)), fileStream,
                          CurrentApplication, messageBoxContainer.ActualWidth, storageTreeView);
        }

        /// <summary>
        /// Handles Add New Folder requests from a directory tree item
        /// </summary>
        /// <param name="parent"></param>
        void DirectoryAddNewFolderRequested(DirectoryTreeItem parent)
        {
            var newFolder = new NewFolderTreeViewItem { IsSelected = true, Tag = "DIR_NEW FOLDER" };
            parent.Items.Add(newFolder);
            parent.Items.SortDescriptions.Clear();
            parent.Items.SortDescriptions.Add(new SortDescription("Tag", ListSortDirection.Ascending));
            newFolder.BringIntoView();
            newFolder.BeginEdit();
            newFolder.FinishedEditing += (s, e) =>
            {
                parent.Items.Remove(newFolder);
                var createDirOp = new CreateNewFolderOperation();
                messageBoxContainer.Children.Add(createDirOp);
                createDirOp.CreateDirectory(Path.Combine(parent.MappedDirectory.Path, e), CurrentApplication,
                                            messageBoxContainer.ActualWidth, storageTreeView);
            };
        }


        void DirectoryDeleteRequested(DirectoryTreeItem parent)
        {
            var result =
                MessageBox.Show(
                    "'" + parent.MappedDirectory.Name + "' and all its contents will be deleted permanently.",
                    "WP7 Isolated Storage Explorer", MessageBoxButton.OKCancel, MessageBoxImage.Exclamation);
            if (result != MessageBoxResult.OK) return;
            messageBoxContainer.Children.Clear();
            var operation = new DeleteDirectoryOperation();
            messageBoxContainer.Children.Add(operation);
            operation.DeleteDirectory(parent.MappedDirectory, CurrentApplication, messageBoxContainer.ActualWidth,
                                      storageTreeView);
        }
    }
}
