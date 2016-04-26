using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.MessagingCE;
using System.Text;
using System.Threading;
using System.Xml.Serialization;
using System.Reflection;

using Newtonsoft.Json;

using rho.PrinterZebraImpl;
using rho.protocol.client;

using PrintingService;

namespace rho
{
    public class ClientThread : PrintingThread
    {
        private const string fromQueuePrefix = "p2p-from-client";
        private const string toQueuePrefix = "p2p-to-client";

        private const string END_OF_MESSAGE_TAG = "endofmessage";

        private static readonly object g_syncObject = new object();

        private P2PMessageQueue m_toClientQueue   = null; //new P2PMessageQueue(false, fromQueuePrefix);
        private P2PMessageQueue m_fromClientQueue = null; //new P2PMessageQueue(true, toQueuePrefix);

        private MethodConvertorFactory<PrinterZebra> m_factory = new MethodConvertorFactory<PrinterZebra>();
        private MethodConvertorFactory<PrinterZebraSingleton> m_factorySingleton = new MethodConvertorFactory<PrinterZebraSingleton>();

        private MethodSelector<OnlyResultMethod> m_methodSelector = new MethodSelector<OnlyResultMethod>("rho.protocol.client");

        public ClientThread(string uuid, MainForm mainForm) : base(mainForm)
        {
            m_toClientQueue   = new P2PMessageQueue(false, fromQueuePrefix + uuid);
            m_fromClientQueue = new P2PMessageQueue(true, toQueuePrefix + uuid);
        }
        
        public void closeQueues()
        {
            m_toClientQueue.Close();
            m_fromClientQueue.Close();
        }

        protected override void checkForMessage()
        {
            Message         message  = new Message();
            ReadWriteResult result   = m_fromClientQueue.Receive(message);
            IClientMethod   clMethod = null;

            if (result == ReadWriteResult.OK)
            {
                string data = Encoding.UTF8.GetString(message.MessageBytes, 0, message.MessageBytes.GetLength(0));

                Logger.Write("receive data from client: " + data);

                clMethod = (IClientMethod)m_methodSelector.selectMethod(data);

                clMethod.setQueueForAnswer(m_toClientQueue);
            }          

            processMessage(clMethod);
        }

        private void processEndofAnswer()
        {
            byte[] convertJson = Encoding.UTF8.GetBytes(END_OF_MESSAGE_TAG);
            Message message = new Message(convertJson);
            m_toClientQueue.Send(message);
        }

        private void processMessage(rho.protocol.client.IClientMethod clientMethod)
        {
            try
            {
                lock(g_syncObject)
                {
                    rho.protocol.server.IMethodHodler selectHolder = null;

                    if (clientMethod.PrinterID.Length == 0)
                    {
                        PrinterZebraSingleton singleton = PrinterManager.Instance.getPrinterSingleton();

                        selectHolder = m_factorySingleton.convert(clientMethod);

                        if (selectHolder != null)
                        {
                            rho.MethodRunnable<PrinterZebraSingleton> c = new rho.MethodRunnable<PrinterZebraSingleton>(selectHolder);
                            c.runInObject(singleton);
                        }
                    }
                    else
                    {
                        PrinterZebra printer = PrinterManager.Instance.getPrinterByID(clientMethod.PrinterID);

                        printer.applicationForm = m_applicationForm;

                        selectHolder = m_factory.convert(clientMethod);

                        if (selectHolder != null)
                        {
                            rho.MethodRunnable<PrinterZebra> c = new rho.MethodRunnable<PrinterZebra>(selectHolder);
                            c.runInObject(printer);
                        }
                    }
                }
            }
            catch (System.Exception ex)
            {
                Logger.Write("Catch main extension: " + ex.Message);
            }

            processEndofAnswer();
        }

        protected override void finalizeThread()
        {
            m_fromClientQueue.Close();
        }

    }
}
