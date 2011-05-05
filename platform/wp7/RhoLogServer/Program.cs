using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;


namespace RhoLogServer
{
    class Program
    {
        static void Main(string[] args)
        {
            String path = args.Length > 0 ? args[0] : ""; 
            SocketServer server = new SocketServer(path);
            while (true)
            {
                Thread.Sleep(100);
            }
        }
    }
}
