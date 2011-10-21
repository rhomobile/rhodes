/*------------------------------------------------------------------------
* (The MIT License)
*
* Copyright (c) 2008-2011 Rhomobile, Inc.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*
* http://rhomobile.com
*------------------------------------------------------------------------*/

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
                WP7Device = WP7SDK.GetDevices().First(d => d.Name.StartsWith("Windows Phone Emulator") || d.Name.StartsWith("Windows Phone 7 Emulator"));
            else
                WP7Device = WP7SDK.GetDevices().First(d => d.Name.StartsWith("Windows Phone Device") || d.Name.StartsWith("Windows Phone 7 Device"));

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
