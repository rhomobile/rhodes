using System;
using System.Linq;
using System.Collections.Generic;
using System.Text;
using System.MessagingCE;
using System.Threading;

using rho.protocol.server;

using PrintingService;

namespace rho
{
namespace server
{
    public interface IClientsManager 
    {
        void registerClient(string clientGuid);

        void unregisterClient(string clientGuid);
    }

    public class ClientsManager : PrintingThread, IClientsManager
    {
        private const string serviceQueueName = "printing-service-0B77E505-1745-4cfc-9A26-3B5B6AFB9377";

        private P2PMessageQueue m_serviceClientQueue = null;

        private MethodSelector<NullCommand> m_methodSelector = new MethodSelector<NullCommand>("rho.protocol.server");

        private Dictionary<string, ClientThread> m_clients = new Dictionary<string, ClientThread>();

        public ClientsManager(MainForm appForm) : base(appForm)
        {
            m_serviceClientQueue = new P2PMessageQueue(true, serviceQueueName);
        }

        protected override void checkForMessage()
        {
            Message         message = new Message();
            ReadWriteResult result = m_serviceClientQueue.Receive(message);
            //ReadWriteResult result = ReadWriteResult.OK;
            IServerCommand  clMethod = null;

            if (result == ReadWriteResult.OK)
            {
                string data = Encoding.UTF8.GetString(message.MessageBytes, 0, message.MessageBytes.GetLength(0));
                //string data = "{ \"Name\": \"registerClient\", \"PrinterID\": \"\", \"rhoString\": \"manual_common_spec\" }";

                Logger.Write("[manager] receive data from client: " + data);

                clMethod = (IServerCommand)m_methodSelector.selectMethod(data);
            }

            processMessage(clMethod);
        }

        public void processMessage(IServerCommand clMethod)
        {
            clMethod.executeCommand(this);
        }

        public void registerClient(string clientGuid)
        {
            Logger.Write("[manager] register new client: " + clientGuid);

            if (m_clients != null && !m_clients.ContainsKey(clientGuid))
            {
                Logger.Write("start register [manager]: " + clientGuid);

                ClientThread newClient = new ClientThread(clientGuid, m_applicationForm);
                
                newClient.startThread();
                m_clients.Add(clientGuid, newClient);
            }
            else
            {
                Logger.Write("[manager] register new client repeatedly");
            }
        }

        public void unregisterClient(string clientGuid)
        {
            Logger.Write("[manager] unregister new client: " + clientGuid);

            if (m_clients != null && m_clients.ContainsKey(clientGuid))
            {
                Logger.Write("[manager] start unregister: " + clientGuid);

                ClientThread newClient = m_clients[clientGuid];
                
                newClient.stopThread();                
                newClient.joinThread();
                newClient.closeQueues();

                m_clients.Remove(clientGuid);
            }
            else
            {
                Logger.Write("[manager] unregister new client repeatedly");
            }
        }

        protected override void finalizeThread()
        {
        }
    }
}
}
