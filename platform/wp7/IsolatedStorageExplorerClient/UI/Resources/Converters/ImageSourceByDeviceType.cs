using System;
using System.Globalization;
using System.Windows.Data;
using IsolatedStorageExplorerClient.ApplicationMonitorService;

namespace IsolatedStorageExplorerClient.UI.Resources.Converters
{
    [ValueConversion(typeof(object), typeof(string))]
    public class ImageSourceByDeviceType : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return (DeviceType)value == DeviceType.Device ? "/IsolatedStorageExplorerClient;component/UI/Resources/Images/device.png" : "/IsolatedStorageExplorerClient;component/UI/Resources/Images/emulator.png";
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }
}
