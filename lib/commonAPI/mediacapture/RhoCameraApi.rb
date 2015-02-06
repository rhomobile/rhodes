#------------------------------------------------------------------------
# (The MIT License)
# 
# Copyright (c) 2008-2013 Rhomobile, Inc.
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


if Rho::System.platform == 'APPLE' || Rho::System.platform == 'ANDROID' || Rho::System.platform == 'WP8'

   class Camera
	
       def self.take_picture(callback_url, options = nil)
            if options == nil
                options = {}
            end
            cam_type = 'back'
            if options['camera_type'] != nil
                 cam_type =  options['camera_type']
            end
            cam_type = 'back' if cam_type == 'main'
if Rho::System.platform != 'APPLE' 
            unless options['fileName']
                options['fileName'] = File.join(Rho::RhoApplication.get_blob_folder(), Rho::RhoConfig.generate_id.to_s)
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
                    selected_cam.takePicture(options, callback_url)
                end
            end
       end

       def self.choose_picture(callback_url, options = nil)
            options = {} unless options
if Rho::System.platform != 'APPLE' 
            unless options['fileName']
                options['fileName'] = File.join(Rho::RhoApplication.get_blob_folder(), Rho::RhoConfig.generate_id.to_s)
            end
end
            Camera.choosePicture(options, callback_url)
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
