
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

      def get_db_fullpathname
        if defined? SYNC_DB_FILE
	        File.join(SYNC_DB_FILE)
        else
	        File.join(__rhoGetCurrentDir(), 'db/syncdb.sqlite')
        end
      end
    end
  end # RhoApplication
end # Rho