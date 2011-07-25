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

module Rhom
  module RhomObject    
    def strip_braces(str=nil)
      str ? str.gsub(/\{/,"").gsub(/\}/,"") : nil
    end
  
    # use djb hash function to generate temp object id
    def djb_hash(str, len)
      hash = 5381
      for i in (0..len) 
        hash = ((hash << 5) + hash) + str[i].to_i
      end
      return hash
    end
    
    def extract_options(arr=[])
      arr.last.is_a?(Hash) ? arr.pop : {}
    end
    
    @@reserved_names = {"object" => "1",
                      "source_id" => "1",
                      "update_type" => "1",
                      "attrib_type" => "1",
                      "set_notification" => "1",
                      "clear_notification" => "1" }
    

    def RhomObject.method_name_reserved?(method)
      @@reserved_names.has_key?(method)
    end
  end # RhomObject
end # Rhom