using System;
using System.Drawing;
using System.IO;
using System.Runtime.InteropServices;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using IsolatedStorageExplorerClient.ApplicationMonitorService;
using Microsoft.Win32;

namespace IsolatedStorageExplorerClient.UI.Utilities
{
    public static class IconHandler
    {
        [DllImport("shell32.dll")]
        static extern IntPtr ExtractAssociatedIcon(IntPtr hinst, string file, ref int index);


        [DllImport("shell32.dll", EntryPoint = "ExtractIconA", CharSet = CharSet.Ansi, SetLastError = true, ExactSpelling = true)]
        private static extern IntPtr ExtractIcon(int hInst, string lpszExeFileName, int nIconIndex);

        [DllImport("shell32.dll", CharSet = CharSet.Auto)]
        private static extern uint ExtractIconEx(string szFileName, int nIconIndex, IntPtr[] phiconLarge, IntPtr[] phiconSmall, uint nIcons);


        [StructLayout(LayoutKind.Sequential)]
        public struct SHFILEINFO
        {
            public IntPtr hIcon;
            public IntPtr iIcon;
            public uint dwAttributes;
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 260)]
            public string szDisplayName;
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 80)]
            public string szTypeName;
        };

        public class User32
        {

            [DllImport("User32.dll")]
            public static extern int DestroyIcon(IntPtr hIcon);
        }

        public class Win32
        {
            public const uint SHGFI_ICON = 0x100;
            public const uint SHGFI_LARGEICON = 0x0;

            [DllImport("shell32.dll")]
            public static extern IntPtr SHGetFileInfo(string pszPath, uint dwFileAttributes, ref SHFILEINFO psfi, uint cbSizeFileInfo, uint uFlags);
        }

        public static BitmapImage GetFileIcon(string filename)
        {
            var source = GetIconFromRegistry(filename) ?? GetIconFromShell(filename);
            return source ?? DefaultFileIcon;

        }

        private static BitmapImage DefaultFileIcon
        {
            get
            {
                var bi = new BitmapImage();
                bi.BeginInit();
                bi.UriSource = new Uri("/IsolatedStorageExplorerClient;component/UI/Resources/Images/file.png", UriKind.RelativeOrAbsolute);
                bi.EndInit();
                return bi;
            }
        }


        public static BitmapImage IconToBitmapImage(Icon icon)
        {
            BitmapImage imageSource = null;
            if (icon != null)
            {
                using (var memstream = new MemoryStream())
                {
                    imageSource = new BitmapImage();
                    using (var bmp = icon.ToBitmap())
                    {
                        bmp.Save(memstream, System.Drawing.Imaging.ImageFormat.Png);
                    }
                    memstream.Seek(0, SeekOrigin.Begin);
                    imageSource.BeginInit();
                    imageSource.CacheOption = BitmapCacheOption.OnLoad;

                    imageSource.StreamSource = memstream;
                    imageSource.EndInit();
                }
            }
            return imageSource;
        }

        private static BitmapImage GetIconFromShell(string filename)
        {
            try
            {
                var shinfo = new SHFILEINFO();

                Win32.SHGetFileInfo(filename, 0,
                                    ref shinfo, (uint)Marshal.SizeOf(shinfo),
                                    Win32.SHGFI_ICON | Win32.SHGFI_LARGEICON);

                var myIcon = Icon.FromHandle(shinfo.hIcon);
                var imageSource = IconToBitmapImage(myIcon);
                User32.DestroyIcon(shinfo.hIcon);
                return imageSource;
            }
            catch (Exception)
            {
                return null;
            }
        }

        private static BitmapImage GetIconFromRegistry(string filename)
        {
            BitmapImage bitmapImage;
            try
            {
                string iconLocation = null;
                string iconFile = null;
                int iconIndex = 0;
                bool hasIndex = false;
                var extension = Path.GetExtension(filename);
                if (extension == null) return null;
                using (var rkRoot = Registry.ClassesRoot)
                {
                    using (var extensionKey = rkRoot.OpenSubKey(extension.ToLower()))
                    {
                        if (extensionKey == null) return null;
                        var defaultValue = extensionKey.GetValue("");
                        var defaultIcon = string.Format("{0}\\DefaultIcon", defaultValue);
                        using (var rkFileIcon = rkRoot.OpenSubKey(defaultIcon))
                        {
                            if (rkFileIcon == null) return null;
                            var value = rkFileIcon.GetValue("");
                            if (value == null) return null;
                            iconLocation = value.ToString().Replace("\"", "");
                        }
                    }
                }

                if (iconLocation.Contains(","))
                {
                    hasIndex = true;
                    iconFile = iconLocation.Substring(0, iconLocation.IndexOf(","));
                    iconIndex = int.Parse(iconLocation.Substring(iconLocation.IndexOf(",") + 1));
                }
                else
                {
                    iconFile = iconLocation;
                }



                var lIcon = ExtractIcon(0, iconFile, iconIndex);
                var icon = Icon.FromHandle(lIcon);
                bitmapImage = IconToBitmapImage(icon);
                User32.DestroyIcon(lIcon);

                return bitmapImage;

            }
            catch (Exception ex)
            {

            }
            return null;
        }

        public static BitmapImage GetFolderIcon(bool folderOpen)
        {
            var bi = new BitmapImage();
            bi.BeginInit();
            bi.UriSource = folderOpen
                               ? new Uri(
                                     "/IsolatedStorageExplorerClient;component/UI/Resources/Images/folder_open.png",
                                     UriKind.RelativeOrAbsolute)
                               : new Uri(
                                     "/IsolatedStorageExplorerClient;component/UI/Resources/Images/folder_closed.png",
                                     UriKind.RelativeOrAbsolute);
            bi.EndInit();
            return bi;

        }

        public static ImageSource GetDeviceIcon(DeviceType value)
        {
            var bi = new BitmapImage();
            bi.BeginInit();
            bi.UriSource = new Uri(
                value == DeviceType.Device
                    ? "/IsolatedStorageExplorerClient;component/UI/Resources/Images/device.png"
                    : "/IsolatedStorageExplorerClient;component/UI/Resources/Images/emulator.png",
                UriKind.RelativeOrAbsolute);
            bi.EndInit();
            return bi;

        }

        public static ImageSource GetIsolatedStorageFileIcon()
        {
            var bi = new BitmapImage();
            bi.BeginInit();
            bi.UriSource =
                new Uri("/IsolatedStorageExplorerClient;component/UI/Resources/Images/isolatedStorageFile.png",
                UriKind.RelativeOrAbsolute);
            bi.EndInit();
            return bi;
        }
    }
}
