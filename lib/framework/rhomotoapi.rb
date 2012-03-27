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

if Object.const_defined?('RhoElementsExt')

module Barcode

class << self

    def take_barcode( callback, hashParams = {} )
        enable(callback, hashParams)
        RhoElementsExt.meta_proc('scanner', 'start', "")
    end

    def enable( callback = nil, hashParams = {} )
        strParams = ""
        scannerID = nil
        hashParams.each do |key, value|
            if key.to_s() == 'deviceName'
                scannerID = value    
                next
            end
            
            strParams += "#{key}:#{value};"
        end

        strParams += "decodeEvent:url('#{callback}');" if callback
        
        if scannerID
            strParams += "enabled:#{scannerID};"
        else
            strParams += "enable;" 
        end    
                
        RhoElementsExt.meta_proc('scanner', strParams, "")
    end

    def enumerate(callback=nil)
    
        if callback
            RhoElementsExt.meta_proc('scanner', "EnumScannerEvent:url('#{callback}');enumerate", "")
        else
            RhoElementsExt.meta_proc('scanner', 'enumerate', '')    
        end    
        
    end

	def disable
		RhoElementsExt.meta_proc('scanner', 'disable', '')
	end
	
	def start
		RhoElementsExt.meta_proc('scanner', 'start', '')
	end
	
	def stop
		RhoElementsExt.meta_proc('scanner', 'stop', '')
	end

	def setEmml(argument)
		RhoElementsExt.meta_proc('scanner', argument.to_s, '')
	end
    
    def method_missing(name, *args)
      unless name == Fixnum
        if name[name.length()-1] == '='
          Scanner.send(name.to_s.chop().to_sym , args[0] ? args[0].to_s : "") 
        else
          Scanner.send(name.to_sym)
        end
      end
    end
      
    def rho_process_moto_callback(params)
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
end

end