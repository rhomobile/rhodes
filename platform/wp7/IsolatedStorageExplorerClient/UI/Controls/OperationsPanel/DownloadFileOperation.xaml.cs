using System;
using System.Collections.Generic;
using System.IO;
using System.ServiceModel;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media.Animation;
using System.Windows.Threading;
using IsolatedStorageExplorerClient.Client;
using IsolatedStorageExplorerClient.Client.Communication;
using IsolatedStorageExplorerClient.ClientOperationService;
using IsolatedStorageExplorerClient.ClientStreamingService;

namespace IsolatedStorageExplorerClient.UI.Controls.OperationsPanel
{
    /// <summary>
    /// Interaction logic for UploadFileOperation.xaml
    /// </summary>
    public partial class DownloadFileOperation
    {

        private Guid _operationId;

        public DownloadFileOperation()
        {
            InitializeComponent();
        }

        public void DownloadFile(string file, Stream fileStream, ApplicationInstance currentApplication, double actualWidth, TreeView storageTreeView)
        {
            messageLabel.Content = "Waiting for application to confirm operation...";
            Show(actualWidth);
            storageTreeView.IsEnabled = false;

            long length = -1;
            var callback = new ClientStreamingServiceCallback
                               {
                                   DownloadStreamChunkAvailableDelegate =
                                       (chunk, bytesLeft) =>
                                       {
                                           if (length == -1) length = bytesLeft;
                                           fileStream.Write(chunk, 0, chunk.Length);
                                           Dispatcher.
                                               Invoke(DispatcherPriority.Normal,
                                                      new Action(() =>
                                                                     {
                                                                         messageLabel.Content = "Download in progress...";
                                                                         var progressStep = length / 100.0;
                                                                         var progress = fileStream.Position / (progressStep == 0 ? 1 : progressStep);
                                                                         progressBar.IsIndeterminate = false;
                                                                         progressBar.Value = progress;
                                                                     }));

                                       }
                               };
            var client = ClientProvider.GetClientStreamingServiceClient(new InstanceContext(callback));
            var streamId = client.BeginDownloadStream(ExplorerClient.Instance.SessionToken);

            var op = GetNewOperation(currentApplication.ApplicationSessionId);
            _operationId = op.Id;
            op.OperationsToExecute.Add(new DownloadFileRequest() { Id = Guid.NewGuid(), DownloadStreamId = streamId, FilePath = file });

            ExplorerClient.Instance.
                OperationProgressReceived +=
                (id,s) =>
                {
                    if (s.OperationId != op.Id) return;
                    if (s is OperationReceived)
                    {
                        messageLabel.Content = "Starting download...";
                        CancelButton.IsEnabled = true;
                    }
                    if (s is OperationCompleted || s is OperationFailedDueToException)
                    {
                        storageTreeView.IsEnabled = true;
                        Hide();
                        fileStream.Close();
                        fileStream.Dispose();
                    }
                    if (s is OperationFailedDueToException)
                    {
                        MessageBox.Show(
                            "Download has failed.\nDuring the download process an error has occured. Please check your changes and try again.",
                            "WP7 Isolated Storage Explorer", MessageBoxButton.OK, MessageBoxImage.Error);
                    }


                };
            ExplorerClient.Instance.PerformOperation(op);


        }

        private static Operation GetNewOperation(Guid appSessionId)
        {
            return new Operation()
            {
                Id = Guid.NewGuid(),
                ClientSessionId = ExplorerClient.Instance.SessionToken,
                ApplicationSessionId = appSessionId,
                OperationsToExecute = new List<RequestBase>()
            };
        }

        private void CancelButtonClick(object sender, RoutedEventArgs e)
        {
            messageLabel.Content = "Cancelling...";
            var client = ClientProvider.GetClientOperationServiceClient(new InstanceContext(ExplorerClient.Instance));
            client.CancelOperation(_operationId);
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
    }





}
