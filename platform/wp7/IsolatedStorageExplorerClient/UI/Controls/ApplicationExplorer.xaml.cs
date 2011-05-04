using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Linq;
using System.Windows.Controls;
using IsolatedStorageExplorerClient.Client;
using IsolatedStorageExplorerClient.ClientOperationService;
using IsolatedStorageExplorerClient.UI.Controls.OperationsPanel;
using IsolatedStorageExplorerClient.UI.Controls.StorageTreeView;
using Microsoft.Win32;

namespace IsolatedStorageExplorerClient.UI.Controls
{
    /// <summary>
    /// Interaction logic for ApplicationExplorer.xaml
    /// </summary>
    public partial class ApplicationExplorer
    {
        private readonly Guid _applicationSessionId;
        private readonly IsolatedStorageRootItem _root = new IsolatedStorageRootItem();

        public ApplicationExplorer()
        {
            InitializeComponent();
            storageTreeView.Items.Add(_root);
            _root.IsExpanded = true;
            ExplorerClient.Instance.OnApplicationSessionEnded += OnApplicationSessionEnded;
            ExplorerClient.Instance.OperationProgressReceived += OperationProgressReceived;
            ExplorerClient.Instance.OnApplicationSessionLocked += Instance_OnApplicationSessionLocked;
            ExplorerClient.Instance.OnApplicationSessionUnlocked += Instance_OnApplicationSessionUnlocked;
        }




        public ApplicationExplorer(Guid applicationSessionId)
            : this()
        {
            _applicationSessionId = applicationSessionId;
            _root.RefreshRequested += RootRefreshRequested;
            _root.AddNewFolderRequested += RootAddNewFolderRequested;
            _root.AddExistingItemRequested += RootAddExistingItemRequested;
            RequestIsolatedStorageInformation(applicationSessionId);
        }





        /// <summary>
        /// Handles application session end
        /// </summary>
        /// <param name="applicationSessionId"></param>
        void OnApplicationSessionEnded(Guid applicationSessionId)
        {
            if (applicationSessionId == _applicationSessionId)
                Cleanup();
        }


        void Instance_OnApplicationSessionUnlocked(Guid applicationSessionId)
        {
            if (CurrentApplication == null) return;
            if (applicationSessionId != CurrentApplication.ApplicationSessionId) return;
            if (CurrentApplication.LatestIsolatedStorageInfo == null)
            {
                if(messageBoxContainer.Children.OfType<UpdateStorageInformationOperation>().Count()==0)
                {
                    RequestIsolatedStorageInformation(CurrentApplication.ApplicationSessionId);
                }
            }
        }

        void Instance_OnApplicationSessionLocked(Guid applicationSessionId)
        {
            if (CurrentApplication == null) return;
            if (applicationSessionId != CurrentApplication.ApplicationSessionId) return;
            messageBoxContainer.Children.Clear();
            var op = new ApplicationLocked();
            op.ShowLocked(CurrentApplication, messageBoxContainer.ActualWidth, storageTreeView);
            messageBoxContainer.Children.Add(op);
        }

        /// <summary>
        /// Updates the structure when an OperationCompleted is received
        /// </summary>
        void OperationProgressReceived(Guid applicationSessionId, OperationProgress progress)
        {
            if (!(progress is OperationCompleted)) return;
            if (applicationSessionId != CurrentApplication.ApplicationSessionId) return;
            var opCompleted = progress as OperationCompleted;
            if (opCompleted.NewIsolatedStorageInformation == null) return;
            CurrentApplication.LatestIsolatedStorageInfo = opCompleted.NewIsolatedStorageInformation;
            _root.DeviceType = CurrentApplication.ApplicationInformation.DeviceType;
            UpdateIsolatedStorage(opCompleted.NewIsolatedStorageInformation);
        }

        /// <summary>
        /// Posts an empty operation to get the storage structure
        /// </summary>
        private void RequestIsolatedStorageInformation(Guid applicationSessionId)
        {
            messageBoxContainer.Children.Clear();
            var updateOp = new UpdateStorageInformationOperation();
            updateOp.PerformUpdateRequest(applicationSessionId, messageBoxContainer.ActualWidth);
            messageBoxContainer.Children.Add(updateOp);
        }

        /// <summary>
        /// Gets the currently selected application
        /// </summary>
        private ApplicationInstance CurrentApplication
        {
            get
            {
                return
                    ExplorerClient.Instance.Applications.FirstOrDefault(
                        app => app.ApplicationSessionId == _applicationSessionId);
            }
        }


        /// <summary>
        /// Updates the storage info
        /// </summary>
        /// <param name="newIsolatedStorageInformation"></param>
        private void UpdateIsolatedStorage(IsolatedStorageInformation newIsolatedStorageInformation)
        {
            UpdateCollection(_root.Items, newIsolatedStorageInformation.Directories,
                             newIsolatedStorageInformation.Files);




        }

        /// <summary>
        /// Updates the tree structure based on the received information
        /// </summary>
        private void UpdateCollection(ItemCollection items, IEnumerable<DirectoryInformation> subDirectories, IEnumerable<FileInformation> filesInDirectory)
        {
            var dirs = items.OfType<DirectoryTreeItem>().ToList();
            var files = items.OfType<FileTreeViewItem>().ToList();

            //Remove directories that no longer exist
            foreach (var dir in dirs)
            {
                var match = subDirectories.FirstOrDefault(subDir => subDir.Name.ToLower() == dir.MappedDirectory.Name.ToLower());
                if (match == null) items.Remove(dir);
            }
            dirs = items.OfType<DirectoryTreeItem>().ToList();
            foreach (var directory in subDirectories)
            {
                var match = dirs.FirstOrDefault(subDir => subDir.MappedDirectory.Name.ToLower() == directory.Name.ToLower());
                if (match == null)
                {
                    match = new DirectoryTreeItem { MappedDirectory = directory };
                    match.AddNewFolderRequested += DirectoryAddNewFolderRequested;
                    match.AddExistingItemRequested += DirectoryAddExistingItemRequested;
                    match.DeleteRequested += DirectoryDeleteRequested;
                    match.Tag = "DIR_" + directory.Name.ToLower();
                    items.Add(match);
                }

                UpdateCollection(match.Items, directory.Directories, directory.Files);

            }

            //Remove files that no longer exist
            foreach (var file in files)
            {
                var match = filesInDirectory.FirstOrDefault(filObj => filObj.FileName.ToLower() == file.MappedFile.FileName.ToLower());
                if (match == null) items.Remove(file);
            }

            files = items.OfType<FileTreeViewItem>().ToList();
            foreach (var fileInformation in filesInDirectory)
            {
                var match = files.FirstOrDefault(filObj => filObj.MappedFile.FileName.ToLower() == fileInformation.FileName.ToLower());
                if (match != null) continue;
                match = new FileTreeViewItem
                            {
                                MappedFile = fileInformation,
                                Tag = "FILE_" + fileInformation.FileName.ToLower()
                            };
                match.DeleteRequested += match_DeleteRequested;
                match.DownloadFileRequested += match_DownloadFileRequested;
                items.Add(match);
            }

            items.SortDescriptions.Clear();
            items.SortDescriptions.Add(new SortDescription("Tag", ListSortDirection.Ascending));


        }



        private void Cleanup()
        {
            ExplorerClient.Instance.OnApplicationSessionEnded -= OnApplicationSessionEnded;
            ExplorerClient.Instance.OperationProgressReceived -= OperationProgressReceived;
        }
    }



}
