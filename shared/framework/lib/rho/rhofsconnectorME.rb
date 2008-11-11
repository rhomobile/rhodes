
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
  			  'syncdb_.dbs'
  		  end
  	  end
	  
      def enum_files(paths, filename) # :yield: path
		yield get_app_path('Rhosugar/Account') + filename
		yield get_app_path('Rhosugar/Case') + filename  
		yield get_app_path('Rhosugar/Employee') + filename  
  	  end
		  
    end

  end # RhoApplication
end # Rho