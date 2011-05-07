using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Threading;
using System.Reflection;
using Microsoft.SmartDevice.Connectivity;

namespace RhoAppRunner
{
    class Program
    {
        static void Main(string[] args)
        {
            DatastoreManager dsmgrObj = new DatastoreManager(1033);
            Platform WP7SDK = dsmgrObj.GetPlatforms().Single(p => p.Name == "Windows Phone 7");

            bool useEmulator = true;
            Device WP7Device = null;

            if (args[4] == "dev")
                useEmulator = false;

            if (useEmulator)
                WP7Device = WP7SDK.GetDevices().Single(d => d.Name == "Windows Phone 7 Emulator");
            else
                WP7Device = WP7SDK.GetDevices().Single(d => d.Name == "Windows Phone 7 Device");

            Console.WriteLine("Connecting to Windows Phone 7 Emulator/Device...");
            WP7Device.Connect();
            Console.WriteLine("Windows Phone 7 Emulator/Device Connected...");

            Guid appID = new Guid(args[0]);
            RemoteApplication app;
            if (WP7Device.IsApplicationInstalled(appID))
            {
                Console.WriteLine("Updating sample XAP to Windows Phone 7 Emulator/Device...");

                app = WP7Device.GetApplication(appID);

                //app.Uninstall();
                app.UpdateApplication(args[1],
                                      args[2],
                                      args[3]);

                Console.WriteLine("Sample XAP Updated on Windows Phone 7 Emulator/Device...");

                Console.WriteLine("Launching sample app on Windows Phone 7 Emulator...");
                app.Launch();
                Console.WriteLine("Launched sample app on Windows Phone 7 Emulator...");

                return;
            }

            Console.WriteLine("Installing sample XAP to Windows Phone 7 Emulator/Device...");

            app = WP7Device.InstallApplication(
                appID,
                appID,
                args[1],
                args[2],
                args[3]);

            Console.WriteLine("Sample XAP installed to Windows Phone 7 Emulator...");

            Console.WriteLine("Launching sample app on Windows Phone 7 Emulator...");
            app.Launch();
            Console.WriteLine("Launched sample app on Windows Phone 7 Emulator...");
        }
    }
}
