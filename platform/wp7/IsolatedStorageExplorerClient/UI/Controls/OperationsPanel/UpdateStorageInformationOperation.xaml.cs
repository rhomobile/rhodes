using System;
using System.Collections.Generic;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media.Animation;
using IsolatedStorageExplorerClient.Client;
using IsolatedStorageExplorerClient.ClientOperationService;

namespace IsolatedStorageExplorerClient.UI.Controls.OperationsPanel
{
    /// <summary>
    /// Interaction logic for UpdateStorageInformationOperation.xaml
    /// </summary>
    public partial class UpdateStorageInformationOperation : UserControl
    {
        public UpdateStorageInformationOperation()
        {
            InitializeComponent();
        }

        public void PerformUpdateRequest(Guid applicationSessionId, double width)
        {
            messageLabel.Content = "Updating isolated storage file information...";
            var op = GetNewOperation(applicationSessionId);
            ExplorerClient.Instance.OperationProgressReceived += (id,s) =>
            {
                if (s.OperationId == op.Id && s is OperationCompleted)
                {
                    Hide();
                }
            };
            Show(width);
            ExplorerClient.Instance.PerformOperation(op);
        }

        private static Operation GetNewOperation(Guid appSessionId)
        {
            return new Operation
                       {
                           Id = Guid.NewGuid(),
                           ClientSessionId = ExplorerClient.Instance.SessionToken,
                           ApplicationSessionId = appSessionId,
                           OperationsToExecute = new List<RequestBase>()
                       };
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
