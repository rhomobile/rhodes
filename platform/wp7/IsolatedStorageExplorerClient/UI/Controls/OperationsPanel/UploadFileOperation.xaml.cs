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

namespace IsolatedStorageExplorerClient.UI.Controls.OperationsPanel
{
    /// <summary>
    /// Interaction logic for UploadFileOperation.xaml
    /// </summary>
    public partial class UploadFileOperation
    {

        private Guid _operationId;

        public UploadFileOperation()
        {
            InitializeComponent();
        }

        public void UploadFile(string file, Stream fileStream, ApplicationInstance currentApplication, double actualWidth, TreeView storageTreeView)
        {
            messageLabel.Content = "Waiting for application to confirm operation...";
            Show(actualWidth);
            storageTreeView.IsEnabled = false;
            var position = 0;
            var callback = new ClientStreamingServiceCallback
                               {
                                   GetNextChunkDelegate =
                                       () =>
                                       {
                                           if (position == fileStream.Length) return null;
                                           fileStream.Position = position;
                                           var buffer = new byte[12228];
                                           var bytes = fileStream.Read(buffer, 0, buffer.Length);
                                           position += bytes;
                                           Dispatcher.
                                               Invoke(DispatcherPriority.Normal,
                                                      new Action(() =>
                                                                     {
                                                                         messageLabel.Content = "Upload in progress...";
                                                                         var progressStep = fileStream.Length / 100.0;
                                                                         var progress = position / progressStep;
                                                                         progressBar.IsIndeterminate = false;
                                                                         progressBar.Value = progress;
                                                                     }));
                                           return buffer;
                                       }
                               };
            var client = ClientProvider.GetClientStreamingServiceClient(new InstanceContext(callback));
            var streamId = client.BeginUploadStream(ExplorerClient.Instance.SessionToken);

            var op = GetNewOperation(currentApplication.ApplicationSessionId);
            _operationId = op.Id;
            op.OperationsToExecute.Add(new UploadFileRequest() { Id = Guid.NewGuid(), UploadStreamId = streamId, FilePath = file });

            ExplorerClient.Instance.
                OperationProgressReceived +=
                (id, s) =>
                {
                    if (s.OperationId != op.Id) return;
                    if (s is OperationReceived)
                    {
                        messageLabel.Content = "Starting upload...";
                        CancelButton.IsEnabled = true;
                    }
                    if (s is OperationCompleted || s is OperationFailedDueToException)
                    {
                        storageTreeView.IsEnabled = true;
                        fileStream.Close();
                        Hide();
                    }
                    if (s is OperationFailedDueToException)
                    {
                        MessageBox.Show(
                            "Upload has failed.\nDuring the upload process an error has occured. Please check your changes and try again.",
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
