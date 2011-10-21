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

using Microsoft.Scripting.Utils;
using Microsoft.Scripting.Runtime;
using IronRuby.Runtime;
using IronRuby.Builtins;
using System;
using System.Runtime.InteropServices;
using Microsoft.Phone.Tasks;
using rho.common;

namespace rho.rubyext
{
    [RubyModule("Camera")]
    public static class RhoCamera
    {
        private static RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() :
            new RhoLogger("RhoCamera");
        private static CRhoRuby RhoRuby { get { return CRhoRuby.Instance; } }
        private static CRhodesApp RHODESAPP() { return CRhodesApp.Instance; }
        private static String m_callback;

        #region Private Implementation Details

        #endregion

        #region Private Instance & Singleton Methods


        [RubyMethodAttribute("take_picture", RubyMethodAttributes.PublicSingleton)]
        public static void takePicture(RubyModule/*!*/ self, [NotNull]String/*!*/ callback)
        {
            try
            {
                m_callback = callback;
                CameraCaptureTask cameraCaptureTask;
                cameraCaptureTask = new CameraCaptureTask();
                cameraCaptureTask.Completed += new EventHandler<PhotoResult>(cameraTask_Completed);
                cameraCaptureTask.Show();
            }
            catch (Exception ex)
            {
                Exception rubyEx = self.Context.CurrentException;
                if (rubyEx == null)
                {
                    rubyEx = RubyExceptionData.InitializeException(new RuntimeError(ex.Message.ToString()), ex.Message);
                }
                LOG.ERROR("take_picture", ex);
                throw rubyEx;
            }
        }

        [RubyMethodAttribute("choose_picture", RubyMethodAttributes.PublicSingleton)]
        public static void choosePicture(RubyModule/*!*/ self, [NotNull]String/*!*/ callback)
        {
            try
            {
                m_callback = callback;
                PhotoChooserTask photoChooserTask;
                photoChooserTask = new PhotoChooserTask();
                photoChooserTask.Completed += new EventHandler<PhotoResult>(cameraTask_Completed);
                photoChooserTask.Show();
            }
            catch (Exception ex)
            {
                Exception rubyEx = self.Context.CurrentException;
                if (rubyEx == null)
                {
                    rubyEx = RubyExceptionData.InitializeException(new RuntimeError(ex.Message.ToString()), ex.Message);
                }
                LOG.ERROR("take_picture", ex);
                throw rubyEx;
            }
        }

        #endregion

        #region Helpers

        private static void cameraTask_Completed(object sender, PhotoResult e)
        {
            switch (e.TaskResult)
            {
                case TaskResult.OK:
                    RhoClassFactory.createNetRequest().pushData(m_callback, "status=ok&image_uri=" + e.OriginalFileName + "&rho_callback=1", null);
                    break;
                case TaskResult.None:
                    RhoClassFactory.createNetRequest().pushData(m_callback, "status=error&message=Error&rho_callback=1", null);
                    break;
                case TaskResult.Cancel:
                    RhoClassFactory.createNetRequest().pushData(m_callback, "status=cancel&message=User cancelled operation&rho_callback=1", null);
                    break;
                default:
                    break;
            }

            //Code to display the photo on the page in an image control named myImage.
            //System.Windows.Media.Imaging.BitmapImage bmp = new System.Windows.Media.Imaging.BitmapImage();
            //bmp.SetSource(e.ChosenPhoto);
            //myImage.Source = bmp;
        }

        #endregion

    }
}
