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

if (((System.get_property('platform') != 'ANDROID') and Object.const_defined?('RhoElementsExt')) or
   ((System.get_property('platform') == 'ANDROID') and System.get_property('is_motorola_device')))

=begin
if System.get_property('platform') != 'ANDROID'

module Camera
    @@current_image_path = nil
    
class << self
    def take_picture( callback, hashParams = {})
        #puts "take_picture : #{callback}; #{hashParams}"

        RhoElementsExt.meta_proc('keyCapture', "KeyValue:All; Dispatch:True; KeyEvent:url('#{callback+'_key'}');", "")

        time = Time.now
        @@current_image_path = File.join( Rho::RhoFSConnector::get_blob_folder(), "Image_#{time.month}_#{time.mday}_#{time.year}_#{time.hour}_#{time.min}_#{time.sec}.bmp" )
        puts "@@current_image_path : #{@@current_image_path}"
        
        strParams = ""
        strParams += "imagerEvent:url('#{callback}');" if callback
        strParams += "destination:url('#{@@current_image_path}');"
        strParams += "left:#{hashParams['left'] ? hashParams['left'] : 0};"
        strParams += "top:#{hashParams['top'] ? hashParams['top'] : 0};"
        strParams += "width:#{hashParams['desired_width'] ? hashParams['desired_width'] : System.get_property('screen_width')};";
        strParams += "height:#{hashParams['desired_height'] ? hashParams['desired_height'] : System.get_property('screen_height')};";
        strParams += "aim:on;"
        strParams += "lamp:on;" if hashParams['flash_mode'] && hashParams['flash_mode'] != 'off'
        
        strParams += "enable;"
        
        puts "meta_proc : #{strParams}"
        RhoElementsExt.meta_proc('imager', strParams, "")
    end

    def rho_process_moto_callback(params)
        #puts "Camera::rho_process_moto_callback : #{params}"        

        return unless @@current_image_path
        
        if @@current_image_path && params['transferResult']
            RhoElementsExt.meta_proc('imager', "disable;", "")
            
            if params['transferResult'].index('OK')
                params['image_uri'] = File.join( '/db/db-files', File.basename(@@current_image_path) )
                params['status'] = 'ok'
            else
                params['status'] = 'cancel'
                params['message'] = 'User canceled operation.'
            end
            
            @@current_image_path = nil
        end

        if params['keyValue'] == '13'
            RhoElementsExt.meta_proc('imager', "capture;", "")
        end
        
    end
    
end
end
end
=end

module Barcode

class << self

    def take_barcode( callback, hashParams = {} )
        enable(callback, hashParams)
        # Work around bug(?) in enable meta which is non blocking and processed asynchronously
        sleep 0.5 if System.get_property('platform') == 'ANDROID'
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
        #puts "rho_process_moto_callback : #{params}"        
        
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