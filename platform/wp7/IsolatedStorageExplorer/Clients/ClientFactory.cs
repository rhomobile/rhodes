using System.ServiceModel;
using System.ServiceModel.Channels;
using IsolatedStorageExplorer.ApplicationOperationsService;
using IsolatedStorageExplorer.ApplicationSessionService;
using IsolatedStorageExplorer.ApplicationStreamingService;

namespace IsolatedStorageExplorer.Clients
{
    /// <summary>
    /// Class used to create and configure the clients used by the explorer
    /// </summary>
    public class ClientFactory
    {
        /// <summary>
        /// Stores the hostname for the address
        /// </summary>
        public string Hostname { get; private set; }

        /// <summary>
        /// Default constructor
        /// </summary>
        /// <param name="hostname">The host name used for clients</param>
        public ClientFactory(string hostname)
        {
            Hostname = hostname;
        }

        /// <summary>
        /// Returns a new client for the ApplicationSession Service
        /// </summary>
        public ApplicationSessionServiceClient GetApplicationSessionServiceClient()
        {
            var transport = new HttpTransportBindingElement();
            var binding = new CustomBinding();
            binding.Elements.Add(new BinaryMessageEncodingBindingElement());
            binding.Elements.Add(transport);
            return new ApplicationSessionServiceClient(
                binding,
                new EndpointAddress(
                    string.Format("http://{0}:2502/ApplicationSessionService/ApplicationSessionService.svc", Hostname)));
        }

        /// <summary>
        /// Returns a new client for the ApplicationOperations Service
        /// </summary>
        public ApplicationOperationsServiceClient GetApplicationOperationsServiceClient()
        {
            var transport = new HttpTransportBindingElement();
            var binding = new CustomBinding();
            binding.Elements.Add(new BinaryMessageEncodingBindingElement());
            binding.Elements.Add(transport);
            return new ApplicationOperationsServiceClient(
                binding,
                new EndpointAddress(
                    string.Format("http://{0}:2502/ApplicationOperationsService/ApplicationOperationsService.svc", Hostname)));
        }


        /// <summary>
        /// Returns a new client for the ApplicationOperations Service
        /// </summary>
        public ApplicationStreamingServiceClient GetApplicationStreamingServiceClient()
        {
            var transport = new HttpTransportBindingElement();
            var binding = new CustomBinding();
            binding.Elements.Add(new BinaryMessageEncodingBindingElement());
            binding.Elements.Add(transport);
            return new ApplicationStreamingServiceClient(
                binding,
                new EndpointAddress(
                    string.Format("http://{0}:2502/ApplicationStreamingService/ApplicationStreamingService.svc", Hostname)));
        }
    }
}
