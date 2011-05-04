using System;
using System.Configuration;
using System.IO;
using System.Linq;
using System.Reflection;
using System.ServiceModel;
using System.ServiceModel.Channels;
using System.ServiceModel.Configuration;
using IsolatedStorageExplorerClient.ApplicationMonitorService;
using IsolatedStorageExplorerClient.ClientOperationService;
using IsolatedStorageExplorerClient.ClientSessionService;
using IsolatedStorageExplorerClient.ClientStreamingService;

namespace IsolatedStorageExplorerClient.Client.Communication
{
    public static class ClientProvider
    {




        public static ClientSessionServiceClient GetSessionServiceClient()
        {
            Binding binding;
            EndpointAddress endpointAddress;
            var gotConfig = GetClientConfig<IClientSessionService>(out endpointAddress, out binding);
            if (!gotConfig) throw new Exception("Missing configuration!!!");
            return new ClientSessionServiceClient(binding, endpointAddress);
        }

        public static ApplicationMonitorServiceClient GetApplicationMonitorServiceClient(InstanceContext callback)
        {
            Binding binding;
            EndpointAddress endpointAddress;
            var gotConfig = GetClientConfig<IApplicationMonitorService>(out endpointAddress, out binding);
            if (!gotConfig) throw new Exception("Missing configuration!!!");
            return new ApplicationMonitorServiceClient(callback, binding, endpointAddress);
        }


        public static ClientOperationServiceClient GetClientOperationServiceClient(InstanceContext callback)
        {
            Binding binding;
            EndpointAddress endpointAddress;
            var gotConfig = GetClientConfig<IClientOperationService>(out endpointAddress, out binding);
            if (!gotConfig) throw new Exception("Missing configuration!!!");
            return new ClientOperationServiceClient(callback, binding, endpointAddress);
        }


        public static ClientStreamingServiceClient GetClientStreamingServiceClient(InstanceContext callback)
        {
            Binding binding;
            EndpointAddress endpointAddress;
            var gotConfig = GetClientConfig<IClientStreamingService>(out endpointAddress, out binding);
            if (!gotConfig) throw new Exception("Missing configuration!!!");
            return new ClientStreamingServiceClient(callback, binding, endpointAddress);
        }




        private static bool GetClientConfig<T>(out EndpointAddress endpointAddress, out Binding binding)
        {
            var map = new ExeConfigurationFileMap
                              {
                                  ExeConfigFilename =
                                      Path.Combine(Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location),
                                                   "client.config")
                              };
            var config = ConfigurationManager.OpenMappedExeConfiguration(map, ConfigurationUserLevel.None);
            var sectionGroup = ServiceModelSectionGroup.GetSectionGroup(config);

            foreach (var endpoint in sectionGroup.Client.Endpoints)
            {
                var element = endpoint as ChannelEndpointElement;
                if (element.Contract.Contains(typeof(T).Name))
                {
                    endpointAddress = new EndpointAddress(element.Address);
                    var configuredBindings =
                        (from bindingObj in sectionGroup.Bindings.BindingCollections
                         where bindingObj.ConfiguredBindings.Count != 0
                         select bindingObj).ToList();
                    foreach (var bindingCollectionElement in configuredBindings)
                    {
                        foreach (var configuredBinding in bindingCollectionElement.ConfiguredBindings)
                        {

                            if (configuredBinding.Name == element.Name)
                            {

                                binding = GetBinding(configuredBinding);
                                configuredBinding.ApplyConfiguration(binding);
                                return true;
                            }
                        }
                    }
                }
            }
            endpointAddress = null;
            binding = null;
            return false;
        }

        private static Binding GetBinding(IBindingConfigurationElement configurationElement)
        {
            if (configurationElement is CustomBindingElement)
                return new CustomBinding();
            else if (configurationElement is BasicHttpBindingElement)
                return new BasicHttpBinding();
            else if (configurationElement is NetMsmqBindingElement)
                return new NetMsmqBinding();
            else if (configurationElement is NetNamedPipeBindingElement)
                return new NetNamedPipeBinding();
            else if (configurationElement is NetPeerTcpBindingElement)
                return new NetPeerTcpBinding();
            else if (configurationElement is NetTcpBindingElement)
                return new NetTcpBinding();
            else if (configurationElement is WSDualHttpBindingElement)
                return new WSDualHttpBinding();
            else if (configurationElement is WSHttpBindingElement)
                return new WSHttpBinding();
            else if (configurationElement is WSFederationHttpBindingElement)
                return new WSFederationHttpBinding();
            return null;
        }


    }

}
