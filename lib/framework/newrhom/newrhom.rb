require 'newrhom/newrhom_object_factory'
require 'newrhom/newrhom_model'


module Rhom
	class Rhom

		attr_accessor :models
		@@newrhom = nil
  
		def self.get_instance
			@@newrhom ||= self.new
		end

    def initialize
      @models = {}
    end

    def load_models(app_manifest_filename = nil)
      if app_manifest_filename
        _load_models(app_manifest_filename)
      else
        _load_models(Rho::RhoFSConnector::get_app_manifest_filename)
      end
    end

    @all_models_loaded = false
    def load_all_sources()
      return if @all_models_loaded

      begin
        models.values.each do |src|
          next if src.loaded
                
          _load_model(src.model_name)
        end
            
        _init_models
      rescue Exception => e
        puts "Error load_all_sources: #{e}"
        puts "Trace: #{e.backtrace}"
      end
    end

    private
if Rho::System.isRhoSimulator
    def _load_models(app_manifest_filename = nil)
        require 'rhoappmanifest'
        
        fappManifest = Rho::AppManifest.enumerate_models(Rho::RhoFSConnector.get_app_path('app').chop)
        _load_models_from_file(fappManifest)
        fappManifest.close
    end
else
    def _load_models(app_manifest_filename = nil)
        f = File.open(app_manifest_filename)
        _load_models_from_file(f)    
        f.close
    end
end

    def _load_models_from_file(f)    
      f.each do |line|
        str = line.chomp
        if str != nil and str.length > 0 
            model_name = File.basename(File.dirname(str))
            _add_model(model_name, str)
        end
      end

      _init_models
    end

    def _add_model(model_name, model_file)
    	return if @models.has_key?(model_name)
      @models[model_name] = ::Rho::NewORMModel.new(model_name)
      @models[model_name].setProperty('file_path', model_file)
      _load_model(model_name)
   		# create DbTables
    end

    def _load_model(model_name)
      return if !models.has_key?(model_name) || models[model_name].loaded

      RhomObjectFactory.init_object(models[model_name])
      require "#{models[model_name].getProperty('file_path')}"

      modelClass = nil
      modelClass = Object.const_get(model_name) if Object.const_defined?(model_name)
      if modelClass
        unless modelClass.respond_to?( :klass_model )
          puts "ERROR: Invalid model definition. Add 'include Rhom::PropertyBag' or 'include Rhom::FixedSchema' to model class"
        end    
      else
        puts "ERROR: cannot load model : #{modelClass}"
      end    
      modelClass    
    end

    def _init_models()
      @all_models_loaded = true

      models.values.each do |model|
        modelClass = Object.const_get(model.model_name) if Object.const_defined?(model.model_name)
        if modelClass
          modelClass.init_model
        else
          puts "ERROR: cannot load model : #{modelClass}"
        end         
      end

      # after all is done - create blob attrib triggers
      ::Rho::RHO.get_db_partitions() do |partition, db|
        System.update_blob_attribs(partition, -1)
      end
    end


    class << Rhom
      def client_id
        ::Rho::NewORM.getClientId
      end

      def getModel(modelname)
        ::Rho::NewORMModel.getModel(modelname)
      end

      def have_local_changes
        ::Rho::NewORM.haveLocalChanges
      end

      def database_local_reset
        ::Rho::NewORM.databaseLocalReset
      end

      def database_client_reset(reset_local_models=true)
        ::Rho::NewORM.databaseClientReset(reset_local_models)
      end

      def database_full_reset_ex(argsHash = {})
        reset_client_info = argsHash[:reset_client_info] || false
        reset_local_models = argsHash[:reset_local_models] || false
        reset_models = argsHash[:models] || []
        ::Rho::NewORM.databaseFullResetEx(reset_models, reset_client_info, reset_local_models)
      end

      def database_full_reset(reset_client_info=false, reset_local_models=true)
        ::Rho::NewORM.databaseFullReset(reset_client_info, reset_local_models)
      end

      def database_full_reset_and_logout
        ::Rho::NewORM.databaseFullResetAndLogout
      end

      def database_fullclient_reset_and_logout
        ::Rho::NewORM.databaseFullclientResetAndLogout
      end
    end
	end
end