using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Shapes;
using Microsoft.Phone.Controls;
using rho.rubyext;

namespace rho.views
{
    public partial class RhoDateTimeDlg : PhoneApplicationPage 
    {

        public DateTime m_dateValue;
        public DateTime m_timeValue;
        public bool m_cancel = false;

        public RhoDateTimeDlg()
        {
            InitializeComponent();
        }

        private void ClosePickerPage()
        {
            NavigationService.GoBack();
        }


        private void DatePicker_ValueChanged(object sender, DateTimeValueChangedEventArgs e)
        {

        }

        private void TimePicker_ValueChanged(object sender, DateTimeValueChangedEventArgs e)
        {

        }

        private void OnDoneButtonClick(object sender, EventArgs e)
        {
            m_dateValue = dateField.Value.Value;//.Date.ToShortDateString();
            m_timeValue = timeField.Value.Value;//.ToShortTimeString();
            m_cancel = false;
            ClosePickerPage();
        }

        private void OnCancelButtonClick(object sender, EventArgs e)
        {
            m_cancel = true;
            ClosePickerPage();
        }
    }
}
