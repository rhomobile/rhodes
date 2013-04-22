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


if Rho::System.getProperty('platform') == 'APPLE' || Rho::System.getProperty('platform') == 'ANDROID'

   class Camera
	
       def self.convert_options(opt)
           if opt != nil
               res_opt = {}
               opt.each do |key,value|
                    if value == true
                          value = 'true'
                    end
                    if value == false
                          value = 'false'
                    end
                    if key == 'camera_type' and value == 'main'
                        value = 'back'
                    end
                    res_opt[key.to_s] = value.to_s
               end	
               return res_opt
           end
           return nil
       end

       def self.take_picture(callback_url, options = nil)
            if options == nil
                options = {}
            end
            cam_type = 'back'
            if options != nil
                 if options['camera_type'] != nil
                      cam_type =  options['camera_type']
                 end 
            end
            cams = Camera.enumerate
            if cams != nil
                if cams.size > 0
                    selected_cam = cams[0]
                    cams.each do |cam|
                       if cam.cameraType == cam_type
                           selected_cam = cam
                       end 
                    end
                    selected_cam.takePicture(convert_options(options), callback_url)
                end
            end
       end

       def self.choose_picture(callback_url, options = nil)
            options = {} unless options
            Camera.choosePicture(convert_options(options), callback_url)
       end

       def self.get_camera_info(cam_type)
            cam_type = 'back' if cam_type == 'main'
            cams = Camera.enumerate
            if cams != nil
                cams.each do |cam|
                     if cam.cameraType == cam_type
                         info ={ "max_resolution" => { "width" => cam.maxWidth.to_s, "height" => cam.maxHeight.to_s } }
                         return info 
                     end 
                end
            end
            return nil	
       end
   end
end
