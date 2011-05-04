using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.IO;
using System.IO.IsolatedStorage;
using System.Linq;
using System.Threading;
using IsolatedStorageExplorer.ApplicationOperationsService;
using IsolatedStorageExplorer.Clients;
using IsolatedStorageExplorer.Utilities;

namespace IsolatedStorageExplorer
{
    /// <summary>
    /// Class used to perform operations sent by clients
    /// </summary>
    internal class OperationPerformer
    {
        /// <summary>
        /// Stores the operation that needs to be performed
        /// </summary>
        private readonly Operation _operationToPerform;

        /// <summary>
        /// Stores a reference to the operation worker
        /// </summary>
        private readonly BackgroundWorker _backgroundWorker;

        /// <summary>
        /// Stores a reference to the Explorer client factory for this session
        /// </summary>
        private readonly ClientFactory _clientFactory;

        /// <summary>
        /// Stores the cancel flag for the operation
        /// </summary>
        private bool _cancelled;

        /// <summary>
        /// Default constructor
        /// </summary>
        public OperationPerformer(Operation operationToPerform, BackgroundWorker operationsWorker, ClientFactory clientFactory)
        {
            _operationToPerform = operationToPerform;
            _backgroundWorker = operationsWorker;
            _clientFactory = clientFactory;
        }


        public void PerformOperation()
        {
            if (_operationToPerform == null) return;
            if (_backgroundWorker == null) return;
            if (!_backgroundWorker.IsBusy) return;
            if (_backgroundWorker.CancellationPending) return;

            var exceptionText = string.Empty;
            RequestBase failedRequest = null;
            var progress = 0;
            BroadcastOperationReceived();

            foreach (var requestBase in _operationToPerform.OperationsToExecute)
            {
                if (_backgroundWorker.CancellationPending) return;
                if (_cancelled) break;
                try
                {
                    new TypeSwitch(requestBase)
                        .Case<CreateDirectoryRequest>(ExecuteCreateDirectoryRequest)
                        .Case<UploadFileRequest>(ExcuteUploadFileRequest)
                        .Case<DownloadFileRequest>(ExecuteDownloadFileRequest)
                        .Case<DeleteDirectoryRequest>(ExecuteDeleteDirectoryRequest)
                        .Case<DeleteFileRequest>(ExecuteDeleteFileRequest);

                    if (_cancelled) break;

                    progress += ((100 / _operationToPerform.OperationsToExecute.Count) *
                                 _operationToPerform.OperationsToExecute.IndexOf(requestBase));
                    var opProgress = new OperationProgress
                                         {
                                             OperationId = _operationToPerform.Id,
                                             TotalProgress = progress,
                                         };
                    BroadcastOperationReportedProgress(opProgress);
                    if (CancelPending(_clientFactory))
                    {
                        _cancelled = true;
                        break;
                    }
                }
                catch (Exception exception)
                {
                    exceptionText = exception.Message;
                    failedRequest = requestBase;
                    break;
                }
            }
            if (_cancelled)
            {
                BroadcastOperationCanceled(progress);
                return;
            }
            if (failedRequest != null)
            {
                var operationFailed = new OperationFailedDueToException
                {
                    OperationId = _operationToPerform.Id,
                    TotalProgress = progress,
                    ExceptionMessage = exceptionText,
                    FailedRequestId = failedRequest.Id,
                    NewIsolatedStorageInformation = GetIsolatedStorageInformation()
                };
                BroadcastOperationReportedProgress(operationFailed);
                return;
            }

            var operationCompleted = new OperationCompleted
            {
                OperationId = _operationToPerform.Id,
                TotalProgress = 100,
                NewIsolatedStorageInformation = GetIsolatedStorageInformation()
            };
            BroadcastOperationReportedProgress(operationCompleted);

        }

        /// <summary>
        /// Deletes a file
        /// </summary>
        /// <param name="deleteFileRequest"></param>
        private static void ExecuteDeleteFileRequest(DeleteFileRequest deleteFileRequest)
        {
            using (var isolatedStorageFile = IsolatedStorageFile.GetUserStoreForApplication())
            {
                isolatedStorageFile.DeleteFile(deleteFileRequest.FilePath);
            }
        }

        /// <summary>
        /// Handles directory delete requests
        /// </summary>
        /// <param name="deleteDirectoryRequest"></param>
        private static void ExecuteDeleteDirectoryRequest(DeleteDirectoryRequest deleteDirectoryRequest)
        {
            DeleteFolder(deleteDirectoryRequest.DirectoryPath);
        }

        /// <summary>
        /// Handles file download requests
        /// </summary>
        /// <param name="request"></param>
        private void ExecuteDownloadFileRequest(DownloadFileRequest request)
        {
            using (var isoFile = IsolatedStorageFile.GetUserStoreForApplication())
            {
                var file = isoFile.OpenFile(request.FilePath, FileMode.Open, FileAccess.Read);
                while (file.Position < file.Length)
                {
                    var completeSent = new AutoResetEvent(false);
                    var chunk = new byte[12288];
                    file.Read(chunk, 0, chunk.Length);
                    Exception ex = null;
                    var client = _clientFactory.GetApplicationStreamingServiceClient();
                    client.UploadStreamChunkCompleted += (s, e) =>
                                                             {
                                                                 ex = e.Error;
                                                                 completeSent.Set();
                                                             };
                    client.UploadStreamChunkAsync(_operationToPerform.ApplicationSessionId, request.DownloadStreamId, chunk, file.Length - file.Position);
                    completeSent.WaitOne();
                    if (ex != null) throw (ex);
                }
            }
        }

       
        /// <summary>
        /// Handles file upload requests
        /// </summary>
        /// <param name="request"></param>
        private void ExcuteUploadFileRequest(UploadFileRequest request)
        {
            using (var isoFile = IsolatedStorageFile.GetUserStoreForApplication())
            {
                var file = isoFile.OpenFile(request.FilePath, FileMode.CreateNew, FileAccess.Write);
                var completedUpload = false;
                Exception exception = null;
                while (!completedUpload)
                {
                    var completed = new AutoResetEvent(false);
                    var client = _clientFactory.GetApplicationStreamingServiceClient();
                    client.GetNextStreamChunkCompleted += (s, e) =>
                    {
                        if (e.Error != null)
                        {
                            exception = e.Error;
                            completed.Set();
                            return;
                        }
                        if (e.Result == null || e.Result.Length == 0)
                        {
                            completedUpload = true;
                            completed.Set();
                            return;
                        }
                        var chunk = e.Result;
                        file.Write(chunk, 0, chunk.Length);
                        completed.Set();
                    };
                    client.GetNextStreamChunkAsync(_operationToPerform.ApplicationSessionId, request.UploadStreamId);
                    completed.WaitOne();
                    if (exception != null)
                    {
                        throw exception;
                    }
                    if (completedUpload) return;
                    if (!CancelPending(_clientFactory)) continue;
                    _cancelled = true;
                    return;
                }
            }
        }


        /// <summary>
        /// Creates a directory
        /// </summary>
        /// <param name="createDirectoryRequest"></param>
        private static void ExecuteCreateDirectoryRequest(CreateDirectoryRequest createDirectoryRequest)
        {
            using (var isolatedStorageFile = IsolatedStorageFile.GetUserStoreForApplication())
            {
                isolatedStorageFile.CreateDirectory(createDirectoryRequest.DirectoryPath);
            }
        }




      


        /// <summary>
        /// Gets the current isolated storage information
        /// </summary>
        public IsolatedStorageInformation GetIsolatedStorageInformation()
        {
            var information = new IsolatedStorageInformation();
            try
            {

                using (var isoFile = IsolatedStorageFile.GetUserStoreForApplication())
                {

                    information = new IsolatedStorageInformation
                                      {
                                          AvailableSpace = isoFile.AvailableFreeSpace,
                                          Quota = isoFile.Quota,
                                          Files = new ObservableCollection<FileInformation>(),
                                          Directories = new ObservableCollection<DirectoryInformation>(),
                                      };
                    var files = isoFile.GetFileNames().ToList();
                    foreach (var file in files)
                    {
                        var fileInfo = GetFileInformation(file);
                        information.Files.Add(fileInfo);
                    }
                    var directories = isoFile.GetDirectoryNames().ToList();
                    foreach (var dirInfo in directories.Select(directory => GetDirectoryInfo(directory, isoFile)))
                    {
                        information.Directories.Add(dirInfo);
                    }
                    return information;
                }
            }
            catch { }
            return information;
        }

        /// <summary>
        /// Builds the directory information based on the directory path
        /// </summary>
        private static DirectoryInformation GetDirectoryInfo(string directory, IsolatedStorageFile isoFile)
        {
            var information = new DirectoryInformation
                                  {
                                      Path = directory,
                                      Name = Path.GetFileName(directory),
                                      Files = new ObservableCollection<FileInformation>(),
                                      Directories = new ObservableCollection<DirectoryInformation>(),

                                  };
            var files = isoFile.GetFileNames(directory + "/").ToList();
            foreach (var file in files)
            {
                var fileInfo = GetFileInformation(Path.Combine(directory, file));
                information.Files.Add(fileInfo);
            }
            var directories = isoFile.GetDirectoryNames(directory + "/").ToList();
            foreach (var dir in directories)
            {
                var dirInfo = GetDirectoryInfo(Path.Combine(directory, dir), isoFile);
                information.Directories.Add(dirInfo);
            }

            return information;
        }

        /// <summary>
        /// Builds the file information based on the file path
        /// </summary>
        private static FileInformation GetFileInformation(string file)
        {
            return new FileInformation
                       {
                           Extension = Path.GetExtension(file),
                           FileName = Path.GetFileName(file),
                           Path = file,
                       };
        }


        /// <summary>
        /// Utility method that deletes a directory and its content
        /// </summary>
        /// <param name="directoryPath"></param>
        /// <returns></returns>
        public static bool DeleteFolder(string directoryPath)
        {
            var path = SanitizePath(directoryPath);
            using (var isf = IsolatedStorageFile.GetUserStoreForApplication())
            {
                return isf.DirectoryExists(path) && DeleteRecursively(path);
            }
        }

        /// <summary>
        /// Utility method that cleans a path
        /// </summary>
        /// <param name="fileLocation"></param>
        /// <returns></returns>
        private static string SanitizePath(string fileLocation)
        {
            var path = fileLocation.Split(new[] { "/", @"\" }, StringSplitOptions.RemoveEmptyEntries);
            var cleanPath = path.Aggregate(string.Empty, Path.Combine);
            return cleanPath;
        }

        /// <summary>
        /// Utility method that removes all content under a path recursively
        /// </summary>
        /// <param name="path"></param>
        /// <returns></returns>
        private static bool DeleteRecursively(string path)
        {

            var files = GetFilesInDirectory(path);
            if (files.Any(file => !DeleteFile(Path.Combine(path, file))))
            {
                throw new Exception("Could not delete file");
            }

            var folders = GetSubdirectories(path);
            if (folders.Any(folder => !DeleteRecursively(Path.Combine(path, folder))))
            {
                throw new Exception("Could not delete folder");
            }

            using (var isf = IsolatedStorageFile.GetUserStoreForApplication())
            {
                isf.DeleteDirectory(path);
            }
            return true;


        }
        /// <summary>
        /// Utility method that deletes a file
        /// </summary>
        /// <param name="fileLocation"></param>
        /// <returns></returns>
        public static bool DeleteFile(string fileLocation)
        {
            var path = SanitizePath(fileLocation);
            using (var isf = IsolatedStorageFile.GetUserStoreForApplication())
            {
                if (isf.FileExists(path))
                {
                    isf.DeleteFile(path);
                    return true;
                }
                return false;
            }
        }

        /// <summary>
        /// Utility method that returns all files under a directory
        /// </summary>
        /// <param name="directory"></param>
        /// <returns></returns>
        public static IEnumerable<string> GetFilesInDirectory(string directory)
        {
            var path = SanitizePath(directory);
            using (var isf = IsolatedStorageFile.GetUserStoreForApplication())
            {
                if (isf.DirectoryExists(path))
                {
                    var files = isf.GetFileNames(Path.Combine(path, "*"));
                    return files;
                }
                return new List<string>();
            }

        }

        /// <summary>
        /// Utility method that returns all subdirectories
        /// </summary>
        public static IEnumerable<string> GetSubdirectories(string profilesDirPath)
        {
            var path = SanitizePath(profilesDirPath);
            using (var isf = IsolatedStorageFile.GetUserStoreForApplication())
            {
                if (isf.DirectoryExists(path))
                {
                    var subDirectories = isf.GetDirectoryNames(Path.Combine(path, "*"));
                    return subDirectories;
                }
                return new List<string>();
            }
        }

        /// <summary>
        /// Calls the application operations service to get the cancel status for the current operation
        /// </summary>
        private bool CancelPending(ClientFactory clientFactory)
        {
            var cancelPending = false;
            var clientPending = clientFactory.GetApplicationOperationsServiceClient();
            var completedEvent = new AutoResetEvent(false);
            clientPending.CancelPendingCompleted += (s, e) =>
            {
                if (e.Error == null)
                {
                    cancelPending = e.Result;
                }
                completedEvent.Set();
            };
            clientPending.CancelPendingAsync(_operationToPerform.ApplicationSessionId, _operationToPerform.Id);
            completedEvent.WaitOne();
            return cancelPending;
        }

        /// <summary>
        /// Triggers the OperationReportedProgress with and OperationReceived message
        /// </summary>
        private void BroadcastOperationReceived()
        {
            if (OperationReportedProgress != null)
            {
                OperationReportedProgress(new OperationReceived { OperationId = _operationToPerform.Id, ReceiveTime = DateTime.UtcNow, TotalProgress = 0 });
            }
        }

        /// <summary>
        /// Triggers the delegate handler for OperationReportedProgress
        /// </summary>
        public void BroadcastOperationReportedProgress(OperationProgress progress)
        {
            if (OperationReportedProgress != null)
            {
                OperationReportedProgress(progress);
            }
        }
        
        /// <summary>
        /// Triggers the OperationReportedProgress with and OperationCanceled message
        /// </summary>
        /// <param name="progress"></param>
        private void BroadcastOperationCanceled(int progress)
        {
            if (OperationReportedProgress != null)
            {
                OperationReportedProgress(new OperationCanceled
                {
                    OperationId = _operationToPerform.Id,
                    TotalProgress = progress,
                    NewIsolatedStorageInformation = GetIsolatedStorageInformation()
                });
            }
        }

        /// <summary>
        /// Delegate used by the Explorer to get progress from the operation performer
        /// </summary>
        public OperationReportedProgressHandler OperationReportedProgress;
        public delegate void OperationReportedProgressHandler(OperationProgress progress);
    }
}