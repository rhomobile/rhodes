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
  class Application

    class << self
      def get_app_path(appname)
        File.join( Rho::Application.appsBundleFolder, appname + "/")
      end
      
      def get_base_app_path
        Rho::Application.appsBundleFolder          
      end

      def get_user_path
        Rho::Application.userFolder                  
      end
      
      def get_model_path(appname, modelname)
        Rho::Application.modelFolderPath(modelname)
      end
      
      def get_blob_folder()
        Rho::Application.databaseBlobFolder()
      end

      def get_public_folder()
        Rho::Application.publicFolder()          
      end
      
      def get_blob_path(relative_path)
        Rho::Application.expandDatabaseBlobFilePath(relative_path)
      end
      
      def defaultNativeMenu()
        make_default_native_menu()        
      end
      
    end
end
end

module Rho
  class RhoFSConnector

    class << self
	
      def get_app_path(appname)
        File.join( Rho::Application.appsBundleFolder, appname + "/")
      end
      
      def get_base_app_path
        Rho::Application.appsBundleFolder          
      end

      def get_user_path
        Rho::Application.userFolder                  
      end

      def get_app_manifest_filename
        Rho::Application.modelsManifestPath
      end
      
      def get_rhoconfig_filename
        Rho::Application.configPath
      end
      
      def get_model_path(appname, modelname)
        Rho::Application.modelFolderPath(modelname)
      end

      def get_db_fullpathname(postfix)
        Rho::Application.databaseFilePath(postfix)
      end
            
      def get_blob_folder()
        Rho::Application.databaseBlobFolder()
      end

      def get_public_folder()
        Rho::Application.publicFolder()          
      end
      
      def get_blob_path(relative_path)
        Rho::Application.expandDatabaseBlobFilePath(relative_path)
      end
      
    end
  end # RhoApplication
end # Rho
