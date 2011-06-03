using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Media.Animation;
using System.Windows.Shapes;

namespace rho.views
{
    public partial class RhoTabHeader : UserControl
    {
        public RhoTabHeader()
        {
            InitializeComponent();
        }

        public RhoTabHeader(String label, String image)
        {
            InitializeComponent();
            lbl.Text = label;
            img.Source = new BitmapImage(new Uri(image, UriKind.Relative));
        }
    }
}
