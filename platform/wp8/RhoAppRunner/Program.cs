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
using System.Globalization;
using System.IO;
using System.Linq;
using Microsoft.SmartDevice.Connectivity.Interface;
using Microsoft.SmartDevice.MultiTargeting.Connectivity;

namespace RhoAppRunner
{
    class Program
    {
        static int Main(string[] args)
        {
            MultiTargetingConnectivity WP8SDK = new MultiTargetingConnectivity(CultureInfo.CurrentCulture.LCID, false);
            //foreach (ConnectableDevice device in WP8SDK.GetConnectableDevices(false))
            //    Console.WriteLine(device.Name);

            bool useEmulator = true;
            ConnectableDevice cDevice = null;
            IDevice WP8Device = null;

            if (args.Length < 5)
            {
                Console.WriteLine("Invalid parameters");
                return 1;
            }

            args[2] = args[2].Replace('/', '\\');
            args[3] = args[3].Replace('/', '\\');
            if (args.Length > 5)
                args[5] = args[5].Replace('/', '\\');

            if (args[4] == "dev")
                useEmulator = false;

            try
            {
                if (useEmulator)
                    cDevice = WP8SDK.GetConnectableDevices(false).First(d => d.Name.StartsWith("Mobile "));
                else
                    cDevice = WP8SDK.GetConnectableDevices(false).First(d => d.Name.StartsWith("Device") || d.Name.StartsWith("Windows Phone 8 Device") || d.Name.StartsWith("Windows Phone Device"));
            }
            catch
            {
                Console.WriteLine("Cannot find Windows Phone 8.0 Emulator/Device.");
                return 3;
            }

            Console.WriteLine("Connecting to " + cDevice.Name);
            try
            {
                WP8Device = cDevice.Connect();
            }
            catch
            {
                Console.WriteLine("Failed to connect to Windows Phone 8 Emulator/Device.");
                return 4;
            }
            Console.WriteLine("Windows Phone 8 Emulator/Device Connected...");

            Guid appID = new Guid(args[0]);
            IRemoteApplication app;
            if (WP8Device.IsApplicationInstalled(appID))
            {
                Console.WriteLine("Updating sample XAP to Windows Phone 8 Emulator/Device...");

                app = WP8Device.GetApplication(appID);

                if (args.Length == 6)
                {
                    var remoteIso = app.GetIsolatedStore();
                    string targetDesktopFilePath = @args[5];
                    try
                    {
                        remoteIso.ReceiveFile(Path.DirectorySeparatorChar + "rho" + Path.DirectorySeparatorChar + "rholog.txt", targetDesktopFilePath, true);
                    }
                    catch (Exception e)
                    {
                        Console.WriteLine("Can't receive rholog.txt from the " + (useEmulator ? "emulator" : "device") + ": " + e.Message);
                        return 2;
                    }
                    return 0;
                }

                app.Uninstall();
            }            

            Console.WriteLine("Installing sample XAP to Windows Phone 8 Emulator/Device...");

            app = WP8Device.InstallApplication(
                appID,
                appID,
                args[1],
                args[2],
                args[3]);

            Console.WriteLine("Sample XAP installed to Windows Phone 8 Emulator...");

            Console.WriteLine("Launching sample app on Windows Phone 8 Emulator...");
            app.Launch();
            Console.WriteLine("Launched sample app on Windows Phone 8 Emulator...");
            return 0;
        }
    }
}
