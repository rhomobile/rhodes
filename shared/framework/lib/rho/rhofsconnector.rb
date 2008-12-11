module Rho
  class RhoFSConnector

    class << self
	
      def get_app_path(appname)
        File.join(File.dirname(File.expand_path(__FILE__)), '../../apps/'+appname+'/')
      end
      
      def get_base_app_path
        File.join(File.dirname(File.expand_path(__FILE__)), '../../apps/')
      end
      
      def get_app_manifest_filename
        File.join(File.dirname(File.expand_path(__FILE__)), '../../apps/app_manifest.txt')
      end

      def get_model_path(appname, modelname)
        File.join(File.dirname(File.expand_path(__FILE__)), '../../apps/'+appname+'/'+modelname+'/')
      end

	    def get_db_fullpathname
  		  if defined? SYNC_DB_FILE
  			  File.join(File.dirname(File.expand_path(__FILE__)), SYNC_DB_FILE)
  		  else
  			  File.join(File.dirname(File.expand_path(__FILE__)), '../../db/syncdb.sqlite')
  		  end
  	  end
    end

  end # RhoApplication
end # Rho