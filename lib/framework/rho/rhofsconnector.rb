
module Rho
  class RhoFSConnector

    class << self
	
      def get_app_path(appname)
        File.join(__rhoGetCurrentDir(), 'apps/'+appname+'/')
      end
      
      def get_base_app_path
        File.join(__rhoGetCurrentDir(), 'apps/')
      end
      
      def get_app_manifest_filename
        File.join(__rhoGetCurrentDir(), 'apps/app_manifest.txt')
      end
      
      def get_rhoconfig_filename
        File.join(__rhoGetCurrentDir(), 'apps/rhoconfig.txt')
      end

      def get_model_path(appname, modelname)
        File.join(__rhoGetCurrentDir(), 'apps/'+appname+'/'+modelname+'/')
      end

      def get_db_fullpathname(postfix)
          File.join(__rhoGetCurrentDir(), 'db/syncdb' + postfix + '.sqlite')
      end

      def get_blob_folder()
        File.join(__rhoGetCurrentDir(), 'db/db-files')
      end
      
      def get_blob_path(relative_path)
        File.join(__rhoGetCurrentDir(), relative_path)
      end
      
    end
  end # RhoApplication
end # Rho