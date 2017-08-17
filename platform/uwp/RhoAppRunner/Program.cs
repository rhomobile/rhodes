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
            MultiTargetingConnectivity UWPSDK = new MultiTargetingConnectivity(CultureInfo.CurrentCulture.LCID, false);

            bool useEmulator = true;
            ConnectableDevice cDevice = null;
            IDevice UWPDevice = null;

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
                    cDevice = UWPSDK.GetConnectableDevices(false).Last(d => (d.Name.StartsWith("Mobile ") && d.Name.Contains("2GB") && d.IsEmulator()));
                else
                    cDevice = UWPSDK.GetConnectableDevices(false).First(d => d.Name.StartsWith("Device") || d.Name.StartsWith("Windows Phone 10 Device") || 
                    d.Name.StartsWith("Windows Phone Device") || d.Name.StartsWith("Windows phone"));
            }
            catch
            {
                Console.WriteLine("Cannot find Windows Phone Emulator/Device.");
                return 3;
            }

            Console.WriteLine("Connecting to " + cDevice.Name);
            try
            {
                UWPDevice = cDevice.Connect(true);
            }
            catch
            {
                Console.WriteLine("Failed to connect to Windows Phone Emulator/Device.");
                return 4;
            }
            Console.WriteLine("Windows Phone Emulator/Device Connected...");

            Guid appID = Guid.Parse(args[0]);

            IRemoteApplication app;
            if (UWPDevice.IsApplicationInstalled(appID))
            {
                if (args[4] == "emulibs")
                {
                    Console.WriteLine("Library is already installed");
                    return 0;
                }
                Console.WriteLine("Updating sample APPX to Windows Phone Emulator/Device...");

                app = UWPDevice.GetApplication(appID);

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

            Console.WriteLine("Installing sample APPX to Windows Phone Emulator/Device...");
            try
            {
                app = UWPDevice.InstallApplication(appID, appID, args[1], args[2], args[3]);
                Console.WriteLine("Sample APPX installed to Windows Phone Emulator...");

                Console.WriteLine("Launching sample app on Windows Phone Emulator...");
                app.Launch();
                Console.WriteLine("Launched sample app on Windows Phone Emulator...");
            }
            catch(Exception e)
            {
                Console.WriteLine("Launching error: " + e.Message);
            }

            return 0;
        }
    }
}
