#------------------------------------------------------------------------
# (The MIT License)
# 
# Copyright (c) 2008-2011 Rhomobile, Inc.
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
# 
# http://rhomobile.com
#------------------------------------------------------------------------

if Rho::System.isRhoSimulator || %w(WINDOWS_DESKTOP WINDOWS WP7 WP8 UWP ANDROID APPLE).include?(System.get_property('platform'))

class Alert
  class << self

  def show_popup(param)
    if param.is_a?(String)
      Rho::Notification.showPopup(:message => param, :buttons=>['OK'])
    elsif param.is_a?(Hash)
      if param[:callback] && param[:callback].length() > 0
        Rho::Notification.showPopup(param, param[:callback])
      else
        Rho::Notification.showPopup(param)
      end
    end
  end

  def hide_popup()
    Rho::Notification.hidePopup()
  end

  def vibrate(duration_ms=2500)
    Rho::Notification.vibrate(duration_ms)
  end

  def play_file(file_name, media_type="")
    Rho::Notification.playFile(file_name, media_type)
  end

  def show_status(title, message, hide_label)
    Rho::Notification.showStatus(title, message, hide_label)
  end

  end #self
end #Alert

end
