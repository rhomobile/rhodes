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


 
class NativeView
  
  def initialize( typename, tab_index, id )
     puts 'Ruby NativeView.initialize()'
     @nv_typename = typename
     @tab_index = tab_index
     @nv_id = id
  end

  # return string with view_type of this native view
  def get_viewtype
      return @nv_typename
  end  

  # return tab index where this native view was opened
  def get_tabindex
      return @tab_index
  end

  # execute navigate() function in NativeView native object linked to this object
  # message - string
  def navigate(message)
      puts 'NativeView.navigate('+message+')'
      RhodesNativeViewManager.navigate_native_view(@nv_id, message)	
  end

 
  # destory this native view
  def destroy
     RhodesNativeViewManager.destroy_native_view(@nv_id)		
  end
 
 
end
 


 
class NativeViewManager
 
  # use constant as index in create_native_view() or in WebView.navigate() with native view prefix
  # with this constant native view will open in separated modal topmost full screen window (not instead WebView in main window)
  OPEN_IN_MODAL_FULL_SCREEN_WINDOW = 11111

  # make and return NativeView object
  # view_type - string with registered native view type
  # tab_index - tab index where native view will opened, can be OPEN_IN_MODAL_FULL_SCREEN_WINDOW
  # params - HASH or another ruby type wich will be transferred to native view with call create()
  # this function execute NativeView fabric porvided in native code and link native NativeView 
  # object received from fabric to Ruby NativeView object returned by this function
  def self.create_native_view(view_type, tab_index, params = nil)
      id = RhodesNativeViewManager.create_native_view(view_type, tab_index, params) 
      if id < 0
          puts "ERROR: NativeViewManager can not make NativeView with this parameters !"
          raise "ERROR: NativeViewManager can not make NativeView with this parameters !"
          return nil 
      end
      c = NativeView.new(view_type, tab_index, id) 	
      if c == nil
          puts 'ERROR - Ruby NativeView object is NIL !'
      end
      return c
  end

end
 
 
end
