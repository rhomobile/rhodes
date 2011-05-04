using System;
using System.ComponentModel;
using System.IO;
using IsolatedStorageExplorerClient.UI.Controls.OperationsPanel;
using IsolatedStorageExplorerClient.UI.Controls.StorageTreeView;
using Microsoft.Win32;

namespace IsolatedStorageExplorerClient.UI.Controls
{
    public partial class ApplicationExplorer
    {
        void RootAddExistingItemRequested(IsolatedStorageRootItem sender)
        {
            var selectFileDialog = new OpenFileDialog { Multiselect = false };
            if (selectFileDialog.ShowDialog() != true) return;
            var file = selectFileDialog.FileName;
            var fileStream = selectFileDialog.OpenFile();
            messageBoxContainer.Children.Clear();
            sender.IsExpanded = true;
            var op = new UploadFileOperation();
            messageBoxContainer.Children.Add(op);
            op.UploadFile(Path.GetFileName(file), fileStream,
                          CurrentApplication, messageBoxContainer.ActualWidth, storageTreeView);
        }

        void RootAddNewFolderRequested(IsolatedStorageRootItem sender)
        {
            var newFolder = new NewFolderTreeViewItem { IsSelected = true, Tag = "DIR_NEW FOLDER" };
            sender.Items.Add(newFolder);
            sender.Items.SortDescriptions.Clear();
            sender.Items.SortDescriptions.Add(new SortDescription("Tag", ListSortDirection.Ascending));
            newFolder.BringIntoView();
            newFolder.BeginEdit();
            newFolder.FinishedEditing += (s, e) =>
            {
                sender.Items.Remove(newFolder);
                var createDirOp = new CreateNewFolderOperation();
                messageBoxContainer.Children.Add(createDirOp);
                createDirOp.CreateDirectory(e, CurrentApplication, messageBoxContainer.ActualWidth, storageTreeView);
            };
        }

        void RootRefreshRequested(IsolatedStorageRootItem sender)
        {
            RequestIsolatedStorageInformation(CurrentApplication.ApplicationSessionId);
        }
    }
}