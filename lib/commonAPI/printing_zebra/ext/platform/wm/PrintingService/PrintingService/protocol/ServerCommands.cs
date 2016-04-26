using System;
using System.Linq;
using System.Collections.Generic;
using System.Text;
using System.MessagingCE;

using rho.protocol.client;
using rho.server;

namespace rho
{
namespace protocol
{
namespace server
{
    public interface IServerCommand : IQueueCommand
    {
        void executeCommand(IClientsManager manager);
    }

    public interface IServerCommand2 : IServerCommand
    {
        string Name { get; }

        string rhoString { get; }
    }

    public class NullCommand : IServerCommand2
    {
        public string Name { get; set; }

        public string rhoString { get; set; }

        public bool isNull 
        {
            get
            {
                return true;
            }
        }

        public void setQueueForAnswer(P2PMessageQueue queue) { }

        public void executeCommand(IClientsManager manager) { }
    }

    public class RegisterClientCommand : IServerCommand2
    {
        public string Name { get; set; }

        public string rhoString  { get; set; }

        public bool isNull 
        {
            get
            {
                return !Name.Equals("registerClient");
            }
        }

        public void setQueueForAnswer(P2PMessageQueue queue) { }

        public void executeCommand(rho.server.IClientsManager manager) 
        {
            manager.registerClient(rhoString);
        }
    }

    public class UnregisterClientCommand : IServerCommand2
    {
        public string Name { get; set; }

        public string rhoString { get; set; }

        public bool isNull
        {
            get
            {
                return !Name.Equals("unregisterClient");
            }
        }

        public void setQueueForAnswer(P2PMessageQueue queue) { }

        public void executeCommand(rho.server.IClientsManager manager)
        {
            manager.unregisterClient(rhoString);
        }
    }
}
}
}
