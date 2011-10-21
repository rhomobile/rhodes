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

module Rho

class NativeTabbar

  def self.create(params)
    NativeBar.create_native_tabbar(Rho::RhoApplication::TABBAR_TYPE, params)
  end

  def self.create_vertical(params)
    NativeBar.create_native_tabbar(Rho::RhoApplication::VTABBAR_TYPE, params)
  end

  def self.remove()
    NativeBar.remove_native_tabbar()
  end

  def self.switch_tab(tab_index)
    NativeBar.native_tabbar_switch_tab(tab_index)
  end

  def self.set_tab_badge(tab_index, value)
    NativeBar.native_tabbar_set_tab_badge(tab_index, value)
  end

  def self.get_current_tab
      webview = System::get_property('webview_framework')
      if System::get_property('platform') == 'APPLE' or System::get_property('platform') == 'ANDROID' or (!webview.nil? and webview.start_with?('WEBKIT'))
          return NativeBar.native_tabbar_get_current_tab
      else
          return 0
      end
  end

end


end
