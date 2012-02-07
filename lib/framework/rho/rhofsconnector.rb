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
  class RhoFSConnector

    class << self
	
      def get_app_path(appname)
        File.join(__rhoGetCurrentDir(), RHO_APPS_DIR+appname+'/')
      end
      
      def get_base_app_path
        File.join(__rhoGetCurrentDir(), RHO_APPS_DIR)
      end

      def get_user_path
          if System::get_property('platform') == 'APPLE'
                File.join(__rhoGetUserDir(), RHO_APPS_DIR)
          else
                File.join(__rhoGetCurrentDir(), RHO_APPS_DIR)
          end
      end
      
      def get_app_manifest_filename
        File.join(__rhoGetCurrentDir(), RHO_APPS_DIR + 'app_manifest.txt')
      end
      
      def get_rhoconfig_filename
        File.join(__rhoGetCurrentDir(), RHO_APPS_DIR + 'rhoconfig.txt')
      end

      def get_model_path(appname, modelname)
        File.join(__rhoGetCurrentDir(), RHO_APPS_DIR+appname+'/'+modelname+'/')
      end

      def get_db_fullpathname(postfix)
if defined?( RHODES_EMULATOR )                  
          File.join(__rhoGetCurrentDir(), RHO_EMULATOR_DIR + '/db/syncdb' + postfix + '.sqlite')
else
          if System::get_property('platform') == 'APPLE'
                File.join(__rhoGetDBDir(), 'db/syncdb' + postfix + '.sqlite')
          else
                File.join(__rhoGetCurrentDir(), 'db/syncdb' + postfix + '.sqlite')
          end
          
end          
      end

      def get_blob_folder()
if defined?( RHODES_EMULATOR )                        
        File.join(__rhoGetCurrentDir(), RHO_EMULATOR_DIR + '/db/db-files')
else
          if System::get_property('platform') == 'APPLE'
                return File.join(__rhoGetDBDir(), 'db/db-files')
          else
                return File.join(__rhoGetCurrentDir(), 'db/db-files')
          end
        
end        
      end

      def get_public_folder()
        File.join(__rhoGetCurrentDir(), RHO_APPS_DIR + 'public')
      end

      def get_blob_path(relative_path)
        cur_dir = __rhoGetCurrentDir()
          if System::get_property('platform') == 'APPLE'
                 cur_dir = __rhoGetDBDir()
          end
if defined?( RHODES_EMULATOR )
        cur_dir = __rhoGetCurrentDir()
        cur_dir = File.join(cur_dir, RHO_EMULATOR_DIR)
end
        ress = ''  
        if cur_dir && cur_dir.length()>0
            ress = File.join(cur_dir, relative_path)
        else
            ress = relative_path.start_with?('/') ? relative_path : "/" + relative_path
            
        end
        #puts '$$$ blob path ['+ress.to_s+']  for ['+relative_path.to_s+']'  
        return ress 
      end
      
    end
  end # RhoApplication
end # Rho