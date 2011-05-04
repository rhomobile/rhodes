using IsolatedStorageExplorerClient.ClientStreamingService;

namespace IsolatedStorageExplorerClient.UI.Controls.OperationsPanel
{
    public class ClientStreamingServiceCallback : IClientStreamingServiceCallback
    {




        public byte[] GetNextChunk()
        {
            return GetNextChunkDelegate == null ? null : GetNextChunkDelegate();
        }

        public void DownloadStreamChunkAvailable(byte[] chunk, long byteLeft)
        {
            if (DownloadStreamChunkAvailableDelegate != null)
            {
                DownloadStreamChunkAvailableDelegate(chunk, byteLeft);
            }
        }

        public GetNextChunkHandler GetNextChunkDelegate;
        public delegate byte[] GetNextChunkHandler();

        public DownloadStreamChunkAvailableHandler DownloadStreamChunkAvailableDelegate;
        public delegate void DownloadStreamChunkAvailableHandler(byte[] chunk, long byteLeft);
    }
}