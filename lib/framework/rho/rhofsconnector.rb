
module Rho
  class RhoFSConnector

    class << self
	
      def get_app_path(appname)
        File.join(__rhoGetCurrentDir(), RHO_APPS_DIR+appname+'/')
      end
      
      def get_base_app_path
        File.join(__rhoGetCurrentDir(), RHO_APPS_DIR)
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
          File.join(__rhoGetCurrentDir(), 'db/syncdb' + postfix + '.sqlite')
end          
      end

      def get_blob_folder()
if defined?( RHODES_EMULATOR )                        
        File.join(__rhoGetCurrentDir(), RHO_EMULATOR_DIR + '/db/db-files')
else
        File.join(__rhoGetCurrentDir(), 'db/db-files')
end        
      end

      def get_public_folder()
        File.join(__rhoGetCurrentDir(), RHO_APPS_DIR + 'public')
      end

      def get_blob_path(relative_path)
        cur_dir = __rhoGetCurrentDir()
if defined?( RHODES_EMULATOR )
        cur_dir = File.join(cur_dir, RHO_EMULATOR_DIR)
end
        
        if cur_dir && cur_dir.length()>0
            File.join(cur_dir, relative_path)
        else
            relative_path
        end    
      end
      
    end
  end # RhoApplication
end # Rho