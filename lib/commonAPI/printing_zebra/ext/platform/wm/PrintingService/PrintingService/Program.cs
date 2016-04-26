using System;
using System.Linq;
using System.Collections.Generic;
using System.Windows.Forms;
using System.IO;

using rho;

namespace PrintingService
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [MTAThread]
        static void Main()
        {
            FileStream stream;

            try
            {
                stream = new FileStream("lock-ps.txt", FileMode.OpenOrCreate, FileAccess.ReadWrite, FileShare.None);
            }
            catch(Exception ex)
            {
                Logger.Write("Exception on open lock.txt: " + ex.Message);
                return;
            }

            Application.Run(new MainForm());
        }
    }
}