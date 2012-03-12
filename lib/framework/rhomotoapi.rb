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

module Barcode
    def self.take_barcode( callback, hashParams = {} )
        strParams = ""
        scannerID = nil
        hashParams.each do |key, value|
            if key.to_s() == 'deviceName'
                scannerID = value    
                next
            end
            
            strParams += "#{key}:#{value};"
        end

        strParams += "decodeEvent:url('#{callback}');"
        if scannerID
            strParams += "enabled:#{scannerID};start;"        
        else
            strParams += "enabled;start;"                    
        end    
                
        RhoElementsExt.meta_proc('scanner', strParams, "")
    end

    def self.enumerate(callback)
    
        RhoElementsExt.meta_proc('scanner', "EnumScannerEvent:url('#{callback}');enumerate", "")
    end
    
    def self.rho_process_moto_callback(params)
        puts "rho_process_moto_callback : #{params}"        
        
        return unless params['event'] == 'Decode'
        if params['data']
            params['barcode'] = params['data']
            params['status'] = 'ok'
        else
            params['status'] = 'cancel'            
        end    
    end
end