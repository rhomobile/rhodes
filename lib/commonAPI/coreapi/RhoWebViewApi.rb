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


if Rho::System.isRhoSimulator || System.get_property('platform') == 'APPLE' || System.get_property('platform') == 'WINDOWS' || System.get_property('platform') == 'WINDOWS_DESKTOP' || System.get_property('platform') == 'WP8' || System.get_property('platform') == 'ANDROID'
 class WebView
    class << self
        alias_method :orig_execute_js, :execute_js
    end
    
    def self.execute_js(func, index = -1, vals = nil)
        if (vals && 0 < vals.size)
            func += '('
            vals.each do |val|
                func += val
                func += ',' if val != vals.last
            end
            func += ');'
        end
        orig_execute_js(func, index)
    end
    
    def self.set_menu_items(items)
        ar = []
        
        items.each do |key, value|
            ar << {key=>value}
        end
        
        Rho::Application.nativeMenu = ar
    end
 end
else
 module WebView
    class << self
        alias_method :orig_execute_js, :execute_js
    end
    
    def self.execute_js(func, index = -1, vals = nil)
        if (vals && 0 < vals.size)
            func += '('
            vals.each do |val|
                func += val
                func += ',' if val != vals.last
            end
            func += ');'
        end
        orig_execute_js(func, index)
    end
 end
end
