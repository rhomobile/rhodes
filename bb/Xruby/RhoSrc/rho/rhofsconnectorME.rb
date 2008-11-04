
module Rho
  class RhoFSConnector

    class << self
	
      def get_app_path(appname)
		    'apps/'+appname+'/'
      end
      
      def get_base_app_path
	      'apps/'  
      end

      def get_model_path(appname, modelname)
		    'apps/'+appname+'/'+modelname+'/'  
      end

	    def get_db_fullpathname
  		  if defined? SYNC_DB_FILE
  			  SYNC_DB_FILE
  		  else
  			  'syncdb.dbs'
  		  end
  	  end
	  
	    def enum_files(paths, filename) # :yield: path
  		  get_app_path('Rhosugar') + filename
  	  end
		  
    end

  end # RhoApplication
end # Rho