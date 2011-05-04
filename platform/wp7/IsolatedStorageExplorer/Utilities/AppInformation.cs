using System;
using System.Linq;
using System.Windows;
using IsolatedStorageExplorer.ApplicationSessionService;
using Microsoft.Phone.Info;

namespace IsolatedStorageExplorer.Utilities
{
    /// <summary>
    /// Utility class used to generate the information provided by the current application
    /// </summary>
    public static class AppInformation
    {
        /// <summary>
        /// Generated a new instance of the ApplicationInformation and sends it back
        /// </summary>
        public static ApplicationInformation ApplicationInformation
        {
            get
            {
                var info = new ApplicationInformation
                {
                    DeviceType =
                        (DeviceType)Enum.Parse(typeof(DeviceType), Microsoft.Devices.Environment.DeviceType.ToString(), true),
                    DeviceName = DeviceExtendedProperties.GetValue("DeviceName").ToString()
                };
              
                var assembly = Application.Current.GetType().Assembly;
                var version = assembly.FullName.Split(new[] { "," }, StringSplitOptions.RemoveEmptyEntries).FirstOrDefault(p => p.Contains("Version"));
                if (version == null)
                {
                    info.Version = null;
                }
                else
                {
                    version = version.Replace("Version=", string.Empty);
                    info.Version = version;
                }
                info.ApplicationName = assembly.FullName.Substring(0, assembly.FullName.IndexOf(","));
                return info;

            }
        }
    }
}
