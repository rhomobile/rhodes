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

#require 'time'
require 'rho/render'
require 'rho/rhoapplication'
require 'rhom'
require 'rhofsconnector'
require 'rho/rhoerror'
require 'rhom/rhom_source'

module Rho
  def self.get_app
    RHO.get_instance().get_app('app')
  end

  def self.support_transitions?()
	is_bb6 = System::get_property('platform') == 'Blackberry' && (System::get_property('os_version') =~ /^6\.0/)
    System::get_property('platform') == 'APPLE' || System::get_property('platform') == 'ANDROID' || is_bb6
  end

  def self.close_database(arg_partition=nil)
    if arg_partition
        puts "Close database for partition: #{arg_partition}"    
        ::Rho::RHO.get_db_partitions()[arg_partition].close
    else
        ::Rho::RHO.get_db_partitions().each do |partition, db|
            puts "Close database for partition: #{partition}"
            db.close
        end
    end    
  end
    
  class RHO
    APPLICATIONS = {}
    APPNAME = 'app'
    
    @@rho_framework = nil
    @@native_bar_initialized = nil
    
    def self.get_instance
        @@rho_framework
    end

    def initialize(app_manifest_filename=nil)
      puts "Calling RHO.initialize"

      if app_manifest_filename
        load_models_from_file(app_manifest_filename)
      else
        load_models_from_file(Rho::RhoFSConnector::get_app_manifest_filename)
      end

      # Initialize application and sources
      @@rho_framework = self
      @db_partitions = {}

      partition = 'user'
      @db_partitions[partition] = Rhom::RhomDbAdapter.new(Rho::RhoFSConnector::get_db_fullpathname(partition), partition)
    end

=begin    
    def initialize(app_manifest_filename=nil)
      puts "Calling RHO.initialize"
      
      if app_manifest_filename
        process_model_dirs(app_manifest_filename)
      else
        process_model_dirs(Rho::RhoFSConnector::get_app_manifest_filename)
      end
      
      # Initialize application and sources
      @@rho_framework = self
      @db_partitions = {}
      init_sources()
    end
=end    
    attr_reader :db_partitions
    
    def self.get_src_db(src_name=nil)
        if src_name
            src_partition = Rho::RhoConfig.sources[src_name]['partition']
            @@rho_framework.db_partitions[src_partition]
        else
            @@rho_framework.db_partitions['user']
        end    
    end

    def self.get_user_db
        @@rho_framework.db_partitions['user']
    end

    def self.get_db_partitions
        @@rho_framework.db_partitions
    end
    
    #def self.get_application_db
    #    @@rho_framework.db_application
    #end
    
    def init_app
      puts "init_app"
      begin
        get_app(APPNAME)
      rescue Exception => e
        trace_msg = e.backtrace.join("\n")
        puts 'Application initialize failed: ' + e.inspect + ";Trace: #{trace_msg}"
      end    
    end

    def init_nativebar
      return if @@native_bar_initialized

      begin
        get_app(APPNAME).init_nativebar
      rescue Exception => e
        trace_msg = e.backtrace.join("\n");
        puts "Application's native bar initialization failed: #{e.inspect}; Trace: #{trace_msg}"
      end
      @@native_bar_initialized = true
    end

    def on_config_conflicts(conflicts)
      begin
          get_app(APPNAME).on_reinstall_config_update(conflicts)
      rescue Exception => e
        trace_msg = e.backtrace.join("\n")
        puts 'Application on_reinstall_config_update failed: ' + e.inspect + ";Trace: #{trace_msg}"
      end    
    end

    def activate_app
      begin
          get_app(APPNAME).on_activate_app
      rescue Exception => e
        trace_msg = e.backtrace.join("\n")
        puts 'Application activate failed: ' + e.inspect + ";Trace: #{trace_msg}"
      end    
    end

    def deactivate_app
      begin
          get_app(APPNAME).on_deactivate_app
      rescue Exception => e
        trace_msg = e.backtrace.join("\n")
        puts 'Application deactivate failed: ' + e.inspect + ";Trace: #{trace_msg}"
      end    
    end

    def ui_created
      begin
        get_app(APPNAME).on_ui_created
      rescue Exception => e
        trace_msg = e.backtrace.join("\n")
        puts '"UI created" callback failed: ' + e.inspect + ";Trace: #{trace_msg}"
      end
    end

    def ui_destroyed
      begin
        get_app(APPNAME).on_ui_destroyed
        @@native_bar_initialized = false
      rescue Exception => e
        trace_msg = e.backtrace.join("\n");
        puts '"UI destroyed" callback failed: ' + e.inspect + ";Trace: #{trace_msg}"
      end
    end

    # make sure we close the database file
    #def self.finalize
      #Rhom::RhomDbAdapter::close
      #puts 'finalize'
      #if @@rho_framework
      #  @@rho_framework.db_application.close()
      #  @@rho_framework.db_user.close()
        
      #  @@rho_framework = nil
      #end
    #end
    
    def raise_rhoerror(errCode)
        raise Rho::RhoError.new(errCode)
    end
    
    def set_app(appname, app)
        APPLICATIONS[appname] = app
    end
    
    def get_app(appname)
      if (APPLICATIONS[appname].nil?)
        require 'application' #RhoApplication::get_app_path(appname)+'application'
        #APPLICATIONS[appname] = Object.const_get('AppApplication').new
        Object.const_get('AppApplication').new
        unless APPLICATIONS[appname].initialized?
          msg = "RhoApplication was not correctly initialized (forget to call 'super' in AppApplication.initialize ?)"
          puts msg
          raise msg
        end
      end
      APPLICATIONS[appname]
    end
    
if defined?( RHODES_EMULATOR )
    def load_models_from_file(app_manifest_filename=nil)
        require 'rhoappmanifest'
        
        fappManifest = Rho::AppManifest.enumerate_models(Rho::RhoFSConnector.get_app_path('app').chop)
        _load_models_from_file(fappManifest)
        fappManifest.close
    end
else
    def load_models_from_file(app_manifest_filename=nil)
if defined? RHO_WP7    
		return unless Rho::file_exist?(app_manifest_filename)
end
        f = File.open(app_manifest_filename)
        _load_models_from_file(f)    
        f.close
    end
end

    def _load_models_from_file(f)    
      f.each do |line|
        str = line.chomp
        if str != nil and str.length > 0 
            puts "model file: #{str}"
            model_name = File.basename(File.dirname(str))

            Rho::RhoConfig::add_source(model_name, {:loaded => false, :file_path => str})
        end
        
      end
    end
    
    # Return the directories where we need to load configuration files
    def process_model_dirs(app_manifest_filename=nil)
      File.open(app_manifest_filename) do |f|
        f.each do |line|
            str = line.chomp
            if str != nil and str.length > 0 
                #puts "model file: #{str}"
                modelName = File.basename(File.dirname(str))
                Rhom::RhomObjectFactory.init_object(modelName)
                require str

                puts "model name: #{modelName}"            

                modelClass = nil 
                modelClass = Object.const_get(modelName) if Object.const_defined?(modelName)
                if modelClass
                    puts "model class found"                            
                    if modelClass.respond_to?( :get_model_params )
                        Rho::RhoConfig::add_loaded_source(modelName,modelClass.get_model_params())
                        modelClass.reset_model_params()
                    else
                        puts "ERROR: Invalid model definition. Add 'include Rhom::PropertyBag' or 'include Rhom::FixedSchema' to model class"
                    end    
                else
                    puts "ERROR: cannot load model : #{modelClass}"
                end    
            end
        end        
      end
    end
    
    # Return the directories where we need to load configuration files
=begin
    def self.process_rhoconfig
      begin
        File.open(Rho::RhoFSConnector.get_rhoconfig_filename) do |f|
          f.each do |line|
              # Skip empty or commented out lines
              next if line =~ /^\s*(#|$)/
              parts = line.chomp.split('=', 2)
              key = parts[0]
              value = nil
              if key and defined? RHO_ME
                value = rho_get_app_property(key.strip)
              end
              
              if !value
                value = parts[1] if parts[1]
              end
                
              if key and value
                val = value.strip.gsub(/\'|\"/,'')
                val = val == 'nil' ? nil : val
                puts "rhoconfig: #{key} => #{val}"
                Rho::RhoConfig.add_config(key.strip,val)
              end  
          end                
        end
      rescue Exception => e
        puts "Error opening rhoconfig.txt: #{e}, using defaults."
      end
    end
=end
    
    def self.check_sources_migration(uniq_sources)
      uniq_sources.each do |source|
          check_source_migration(source)
      end
    end

    def self.check_source_migration(source)
        return unless source.has_key?('migrate_version')

        db = ::Rho::RHO.get_src_db(source['name'])	  

        if !get_instance().get_app(APPNAME).on_migrate_source(source['migrate_version'], source)
            db.execute_batch_sql(source['schema']['sql'])
        end  

        db.update_into_table('sources', {"schema_version"=>source['schema_version']},{"name"=>source['name']})
    end

    def reset_db_on_sync_user_changed() 
      puts "reset_db_on_sync_user_changed"
      begin
          get_app(APPNAME).on_sync_user_changed
      rescue Exception => e
        trace_msg = e.backtrace.join("\n")
        puts 'Application reset_db_on_sync_user_changed failed: ' + e.inspect + ";Trace: #{trace_msg}"
      end       
    end
    
    def load_server_sources(data)
        puts "load_server_sources : #{data}"
        
        begin
            res = Rho::JSON.parse(data)

			skip_schema = false
			if res['skip_schema']
				skip_schema = res['skip_schema'].to_i() > 0
			end
			puts "skip_schema = #{skip_schema}"
            if res['partition']
                str_partition = res['partition']
                puts "reload sources for partition: #{str_partition}"
                db = @db_partitions[ str_partition ]

                #puts "sources before: #{Rho::RhoConfig::sources()}"
                
                db.start_transaction
                begin
					mapProps = {}
					mapFreezed = {}
					
					Rho::RhoConfig::sources().each do |key,value|
						if value['partition']==str_partition
							mapProps[key] = value['property']
							mapFreezed[key] = value['freezed']
						end
					end
										
                    Rho::RhoConfig::sources().delete_if {|key, value| value['partition']==str_partition }
                    arSrcs = db.select_from_table('sources','source_id, name, sync_priority, partition, sync_type, schema, schema_version, associations, blob_attribs',
                        {'partition'=>str_partition} )
                    arSrcs.each do |src|
                        
                        if src 
							if src['schema'] && src['schema'].length() > 0
								if !skip_schema
									#puts "src['schema'] :  #{src['schema']}"
									hashSchema = Rho::JSON.parse(src['schema'])
									#puts "hashSchema :  #{hashSchema}"
                            
									src['schema'] = hashSchema
									src['schema']['sql'] = ::Rho::RHO.make_createsql_script( src['name'], hashSchema)
									src['schema_version'] = hashSchema['version']
                            
									db.update_into_table('sources', {"schema"=>src['schema']['sql'], "schema_version"=>src['schema_version']},{"name"=>src['name']})
                            
									#if str_partition != 'user'
									#    @db_partitions['user'].update_into_table('sources', {"schema"=>src['schema']['sql'], "schema_version"=>src['schema_version']},{"name"=>src['name']})
									#end
								end
							else
								props = mapProps[src['name']]
								if props
									src['property'] = props
								end
								
								freezed = mapFreezed[src['name']]
								if freezed
									src['freezed'] = freezed
								end
							end
                        end

                        src[:loaded] = true
                        Rho::RhoConfig::sources()[ src['name'] ] = src
                        
                    end
                    db.commit
                rescue Exception => e
                    db.rollback
                    raise
                end                
                
                ::Rho::RHO.init_sync_source_properties(Rho::RhoConfig::sources().values())
				#puts "sources after: #{Rho::RhoConfig::sources()}"            
                return
            end
        rescue Exception => e
            puts "Error load_server_sources: #{e}"
            puts "Trace: #{e.backtrace}"
        end
        
        raise ArgumentError, "load_server_sources should be called only from bulk sync with partition parameter!"     
    end

    def self.load_all_sources
        Rho::RHO.get_instance().load_all_sync_sources()
    end
    
    @all_models_loaded = false
    def load_all_sync_sources()
        return if @all_models_loaded
        puts "load_all_sync_sources"

        begin
            Rho::RhoConfig.sources.values.each do |src|
                next if src[:loaded]
                
                load_model(src['name'],false)
            end
            
            init_sources()
        rescue Exception => e
            puts "Error load_all_sync_sources: #{e}"
            puts "Trace: #{e.backtrace}"
        end
    end

    def load_model(modelName, init_db = true)
        return nil if !Rho::RhoConfig.sources.has_key?(modelName) || Rho::RhoConfig.sources[modelName][:loaded]
        Rho::RhoConfig.sources[modelName][:loaded] = true

        puts "load_model: #{modelName}"        
        
        Rhom::RhomObjectFactory.init_object(modelName)
        require "#{Rho::RhoConfig.sources[modelName][:file_path]}"

        puts "model name: #{modelName}"            

        modelClass = nil 
        modelClass = Object.const_get(modelName) if Object.const_defined?(modelName)
        if modelClass
            puts "model class found"                            
            if modelClass.respond_to?( :get_model_params )
                Rho::RhoConfig::add_loaded_source(modelName,modelClass.get_model_params())
                modelClass.reset_model_params()

                partition = Rho::RhoConfig::sources[modelName]['partition']                
                db = ::Rho::RHO.get_db_partitions()[partition]
                if !db
                    db = Rhom::RhomDbAdapter.new(Rho::RhoFSConnector::get_db_fullpathname(partition), partition)
                    ::Rho::RHO.get_db_partitions()[partition] = db
                end

                if init_db
                    hash_migrate = {}                
                    db.start_transaction
                    begin
                        uniq_sources = [Rho::RhoConfig::sources[modelName]]
                        init_db_sources(db, uniq_sources, partition, hash_migrate)

                        puts "Migrate schema sources: #{hash_migrate}"
                        
                        ::Rho::RHO.init_schema_sources_partition(uniq_sources, hash_migrate, partition, db)
                        ::Rho::RHO.check_sources_migration(uniq_sources)
                        
                        ::Rho::RHO.init_sync_source_properties(uniq_sources)
                        
                        #SyncEngine.update_blob_attribs(partition, Rho::RhoConfig::sources[modelName]['source_id'].to_i() )
                        db.commit
                    rescue Exception => e
                        trace_msg = e.backtrace.join("\n")
                        puts "exception when init_db_sources: #{e}; Trace:" + trace_msg
                        
                        db.rollback
                    end
                end                    
            else
                puts "ERROR: Invalid model definition. Add 'include Rhom::PropertyBag' or 'include Rhom::FixedSchema' to model class"
            end    
        else
            puts "ERROR: cannot load model : #{modelClass}"
        end
        
        modelClass    
    end

    def find_src_byname(uniq_sources, src_name)
        uniq_sources.each do |source|        
            return source if src_name == source['name']
        end
        
        nil
    end

    # setup the sources table and model attributes for all applications
    def init_sources()
        return unless defined? Rho::RhoConfig::sources

        @all_models_loaded = true
    
        uniq_sources = Rho::RhoConfig::sources.values
        puts 'init_sources: ' #+ uniq_sources.inspect

        uniq_sources.each do |source|
            source['str_associations'] = ""    
        end
        
        uniq_sources.each do |source|
          partition = source['partition']
          @db_partitions[partition] = nil unless @db_partitions[partition]
          
          if source['belongs_to']
            source['belongs_to'].each do |hash_pair|    
                attrib = hash_pair.keys[0]
                src_name = hash_pair.values[0]
                
                associationsSrc = find_src_byname(uniq_sources, src_name)
                if !associationsSrc
                    puts "Error: belongs_to '#{source['name']}' : source name '#{src_name}' does not exist."
                    next
                end
                
                str_associations = associationsSrc['str_associations']
                str_associations = "" unless str_associations
                str_associations += ',' if str_associations.length() > 0
                
                str_associations += source['name'] + ',' + attrib
                associationsSrc['str_associations'] = str_associations
            end
          end
        end
        
        #user partition should alwayse exist
        @db_partitions['user'] = nil unless @db_partitions['user']
        hash_migrate = {}
        puts "@db_partitions : #{@db_partitions}"
        @db_partitions.each do |partition, db|
            db = Rhom::RhomDbAdapter.new(Rho::RhoFSConnector::get_db_fullpathname(partition), partition) unless db
            db.start_transaction
            begin
                init_db_sources(db, uniq_sources, partition,hash_migrate)
                #SyncEngine.update_blob_attribs(partition, -1 )
                
                db.commit
            rescue Exception => e
                trace_msg = e.backtrace.join("\n")
                puts "exception when init_db_sources: #{e}; Trace:" + trace_msg
                
                db.rollback
            end
                
            @db_partitions[partition] = db
        end

        puts "Migrate schema sources: #{hash_migrate}"
        ::Rho::RHO.init_schema_sources(hash_migrate)
        ::Rho::RHO.check_sources_migration(uniq_sources)
        
        #@db_partitions.each do |partition, db|
        #    SyncEngine.update_blob_attribs(partition, -1 )
        #end
        
        ::Rho::RHO.init_sync_source_properties(uniq_sources)
    end

    def self.init_sync_source_properties(uniq_sources)

        uniq_sources.each do|src|
            ['pass_through', 'full_update'].each do |prop|
                next unless src.has_key?(prop)
                SyncEngine.set_source_property(src['source_id'], prop, src[prop] ? src[prop].to_s() : '' )
            end            
        end
        
        uniq_sources.each do|src|
            if src.has_key?('freezed') || !src['schema'].nil?
				hash_props = !src['schema'].nil? ? src['schema']["property"] : src["property"]
				if (!hash_props.nil?)
					str_props = hash_props.keys.join(',')
					SyncEngine.set_source_property(src['source_id'], 'freezed', str_props )
				end
            end            
        end
        
    end
    
    def self.processIndexes(index_param, src_name, is_unique)

        return "" unless index_param
        
        strUnique = 'UNIQUE' if is_unique
        strRes = ""
        if index_param.is_a?( String )
            strRes = index_param
        else
            nInd = 0
            
            index_param.each do |index_name, index_cols|
                strCols = ""
                if index_cols.is_a?(String)
                    index_cols = index_cols.split(',')    
                end
                
                index_cols.each do |col|
                    strCols += ',' if strCols.length() > 0
                    strCols += "\"#{col}\""
                end

                #strIndName = "rhoIndex" + (is_unique ? "U" : "" ) + "_#{nInd}"
                strIndex = "CREATE #{strUnique} INDEX \"#{index_name}\" ON \"#{src_name}\" (#{strCols});\r\n"
                strRes += strIndex
                nInd += 1
            end
        end
        strRes
    end
        
    def self.init_schema_sources(hash_migrate)
        uniq_sources = Rho::RhoConfig::sources.values
        puts 'init_schema_sources'

        ::Rho::RHO.get_db_partitions().each do |partition, db|
            db.start_transaction
            begin
                init_schema_sources_partition(uniq_sources, hash_migrate, partition, db)
                db.commit
            rescue Exception => e
                trace_msg = e.backtrace.join("\n")
                puts "exception when init_schema_sources: #{e}; Trace:" + trace_msg
                
                db.rollback
            end
        end
        
        puts 'END init_schema_sources'

    end
    
    def self.init_schema_sources_partition(uniq_sources, hash_migrate, partition, db)
        uniq_sources.each do |source|
          next unless partition == source['partition']
          next unless source['schema']
          
          raise ArgumentError, "schema parameter should be Hash!" unless source['schema'].is_a?(Hash)
          
          call_migrate = false
          if db.table_exist?(source['name'])
            next unless hash_migrate.has_key?(source['name'])
            call_migrate = true 
          end
          
			strCreate = make_createsql_script(source['name'], source['schema'])
			
			#puts "source['schema'] :  #{source['schema']}"
			#hashSchema = Rho::JSON.parse(source['schema'])
			#puts "hashSchema :  #{hashSchema}"
			
			#src['schema'] = hashSchema
			#source['schema']['sql'] = strCreate
			#src['schema_version'] = hashSchema['version']
			#strCreate = source['schema']
			#puts "strCreate: #{strCreate}"
        
          if call_migrate
            db.update_into_table('sources', {"schema"=>strCreate},{"name"=>source['name']})
            #::Rho::RHO.get_user_db().update_into_table('sources', {"schema"=>strCreate},{"name"=>source['name']}) if db != ::Rho::RHO.get_user_db()
                        
            source['migrate_version'] = hash_migrate[ source['name'] ]
            source['schema']['sql'] = strCreate
          else
            db.execute_batch_sql(strCreate)
            db.update_into_table('sources', {"schema"=>strCreate, "schema_version"=>source['schema_version']},{"name"=>source['name']})
            
            #::Rho::RHO.get_user_db().update_into_table('sources', {"schema"=>strCreate, "schema_version"=>source['schema_version']},{"name"=>source['name']}) if db != ::Rho::RHO.get_user_db()
          end
        
        end
        
        SyncEngine.update_blob_attribs(partition, -1 )
    end

    def self.make_createsql_script(name,schema_attr)
        strCreate = schema_attr['sql']
        strCreate = "" unless strCreate
        if schema_attr['property']
            arCols = schema_attr['property']
            strCols = arCols.collect do |col, type|
                "\"#{col}\" " + 
                case type[0]
                    when :integer
                        'integer'
                    when :float
                        'float'
                    when :date
                        'integer'
                    when :time
                        'integer'
                    else
                        'varchar'
                end + " default null"
            end.join(',')

            strCols += ',' if strCols.length() > 0
            strCols += "\"object\" varchar(255) PRIMARY KEY"
            strCreate = "CREATE TABLE \"#{name}\" ( #{strCols} )"
        end

        strCreate += ";\r\n" if strCreate && strCreate.length() > 0
        strCreate += processIndexes(schema_attr['index'], name, false)
        strCreate += ";\r\n" if strCreate && strCreate.length() > 0
        strCreate += processIndexes(schema_attr['unique_index'], name, true)
        
        strCreate
    end
    
    def process_blob_attribs(source, db)
      return source['str_blob_attribs'] if source['str_blob_attribs']
      source['str_blob_attribs'] = ""
      
      if source['schema']
        props = source['schema']['property']
      else
        props = source['property']
      end
      
      if props    
        str = ""
        props.each do |name, ar_type|
            if ar_type && ar_type.is_a?(String)
                ar_type = ar_type.split(',')    
            end
            
            if ar_type && ar_type[0].to_s == 'blob'
                str += ',' if str.length()>0
                str += name.to_s() + ','
                str += (ar_type[1].to_s() == 'overwrite' ? '1' : '0')
            end
        end
        
        source['str_blob_attribs'] = str
      end
            
      source['str_blob_attribs']
      
    end

    def get_start_id(db_sources, db_partition)
        start_id = 0
        db_sources.each do |db_src|
            next unless db_partition == db_src['partition']
            
            src_id = db_src['source_id']
            start_id = src_id if src_id > start_id
        end        
        
        if start_id < Rho::RhoConfig.max_config_srcid()[db_partition]        
            start_id = Rho::RhoConfig.max_config_srcid()[db_partition]+2 
        else
            start_id += 1
        end
            
        puts "start_id: #{start_id}"
        start_id
    end
    
    def find_src_byname(db_sources, name)
        db_sources.each do |db_src|
            return  db_src if db_src['name'] == name
        end        
        
        nil            
    end
    
    def init_db_sources(db, uniq_sources, db_partition, hash_migrate)
        puts "init_db_sources"
        
        db_sources = db.select_from_table('sources','sync_priority,source_id,partition, sync_type, schema_version, associations, blob_attribs, name' )
        start_id = get_start_id(db_sources, db_partition)
        
        uniq_sources.each do |source|
          partition = source['partition']        
          next unless partition == db_partition
          #puts "init_db_sources(#{source['name']}) : #{source}"
          name = source['name']
          sync_priority = source['sync_priority']
          sync_type = source['sync_type']
          schema_version = source['schema_version']
          associations = source['str_associations']
          blob_attribs = process_blob_attribs(source, db)
          #puts "blob_attribs : #{blob_attribs}"
          
          #attribs = db.select_from_table('sources','sync_priority,source_id,partition, sync_type, schema_version, associations, blob_attribs', {'name'=>name})
          attribs = find_src_byname(db_sources,name)
          
          if attribs
            if attribs['sync_priority'].to_i != sync_priority.to_i
                db.update_into_table('sources', {"sync_priority"=>sync_priority},{"name"=>name})
            end
            if attribs['sync_type'] != sync_type
                db.update_into_table('sources', {"sync_type"=>sync_type},{"name"=>name})
            end
            if attribs['schema_version'] != schema_version
                if db_partition == partition
                    hash_migrate[name] = attribs['schema_version']
                else    
                    db.update_into_table('sources', {"schema_version"=>schema_version},{"name"=>name})
                end    
            end
            if attribs['partition'] != partition
                db.update_into_table('sources', {"partition"=>partition},{"name"=>name})
            end
            if attribs['associations'] != associations
                db.update_into_table('sources', {"associations"=>associations},{"name"=>name})
            end
            if attribs['blob_attribs'] != blob_attribs
                db.update_into_table('sources', {"blob_attribs"=>blob_attribs},{"name"=>name})
            end
            
            if !source['source_id']
                source['source_id'] = attribs['source_id'].to_i
                Rho::RhoConfig::sources[name]['source_id'] = attribs['source_id'].to_i
            end
            
          else
          
            #puts "Rho::RhoConfig.use_bulk_model :  #{Rho::RhoConfig.use_bulk_model}"
            if Rho::RhoConfig.use_bulk_model.to_s != 'true' && Rho::RhoConfig.use_bulk_model.to_s != '1'
                if !source['source_id']
                    source['source_id'] = start_id
                    Rho::RhoConfig::sources[name]['source_id'] = start_id
                    
                    start_id += 1
                end
              
                db.insert_into_table('sources',
                    {"source_id"=>source['source_id'],"name"=>name, "sync_priority"=>sync_priority, "sync_type"=>sync_type, "partition"=>partition,
                    "schema_version"=>source['schema_version'], 'associations'=>associations, 'blob_attribs'=>blob_attribs })
            end                
          end
          
        end
        
    end
    
    def serve(req)
      begin
        RhoProfiler.start_counter('CTRL_ACTION')            
      
        puts "RHO serve: " + (req ? "#{req['request-uri']}" : '')
        res = init_response
        get_app(req['application']).send :serve, req, res
        
        init_nativebar
        Rho::RhoController.clean_cached_metadata()
        Rho::RhoConfig.clean_cached_changed
        ret = send_response(res)
        RhoProfiler.stop_counter('CTRL_ACTION') 
        return ret
      rescue Exception => e
        ret = send_error(e)
        RhoProfiler.stop_counter('CTRL_ACTION') 
        return ret
      ensure
        RhoApplication::set_current_controller(nil)
      end
    end

    def serve_hash(req)
      begin
        RhoProfiler.start_counter('CTRL_ACTION')            
        puts "RHO serve: " + (req ? "#{req['request-uri']}" : '')
        res = init_response
        get_app(req['application']).send :serve, req, res

        init_nativebar
        Rho::RhoController.clean_cached_metadata()
        Rho::RhoConfig.clean_cached_changed
        ret = send_response_hash(res)
        RhoProfiler.stop_counter('CTRL_ACTION') 
        return ret
      rescue Exception => e
        ret = send_error(e,500,true)
        RhoProfiler.stop_counter('CTRL_ACTION') 
        return ret
      ensure
        RhoApplication::set_current_controller(nil)
      end
      
    end
    
    def serve_index(index_name, req)
      begin
        RhoProfiler.start_counter('INDEX_ACTION')            
      
    	# TODO: Removed hardcoded appname
    	get_app(APPNAME).set_menu
      
        puts "RHO serve_index: " + (req ? "#{req['request-uri']}" : '')
        res = init_response
        res['request-body'] = RhoController::renderfile(index_name, req, res)

        init_nativebar
        Rho::RhoController.clean_cached_metadata()
        Rho::RhoConfig.clean_cached_changed
        ret = send_response(res)
        RhoProfiler.stop_counter('INDEX_ACTION')            
        return ret
      rescue Exception => e
        ret = send_error(e)
        RhoProfiler.stop_counter('INDEX_ACTION')            
        return ret
      end
    end

    def serve_index_hash(index_name, req)
      begin
        RhoProfiler.start_counter('INDEX_ACTION')            
      
    	# TODO: Removed hardcoded appname
    	get_app(APPNAME).set_menu
      
        puts "RHO serve_index: " + (req ? "#{req['request-uri']}" : '')
        res = init_response
        res['request-body'] = RhoController::renderfile(index_name, req, res)

        init_nativebar
        Rho::RhoController.clean_cached_metadata()
        Rho::RhoConfig.clean_cached_changed
        ret = send_response_hash(res)
        RhoProfiler.stop_counter('INDEX_ACTION')            
        return ret
        
      rescue Exception => e
        ret = send_error(e, 500, true)
        RhoProfiler.stop_counter('INDEX_ACTION')            
        return ret
      end
    end


    def make_httpdate(t)
      _RFC2822_DAY_NAME = [
        'Sun', 'Mon', 'Tue', 'Wed', 'Thu', 'Fri', 'Sat'
      ]
      _RFC2822_MONTH_NAME = [
        'Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun',
        'Jul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec'
      ]
    
        sprintf('%s, %02d %s %d %02d:%02d:%02d GMT',
          _RFC2822_DAY_NAME[t.wday],
          t.day, _RFC2822_MONTH_NAME[t.mon-1], t.year,
          t.hour, t.min, t.sec)
    end
    
    def init_response(status=200,message="OK",body="")
      res = Hash.new
      res['status'] = status
      res['message'] = message
      res['headers'] = 
        {
        'Date' => make_httpdate(Time.now),#.httpdate,
        'Content-Type' => 'text/html; charset=utf-8',
        'Content-Length' => 0,
        'Connection' => 'close' 
      }
      res['request-body'] = body
      res
    end

    CR   = "\x0d"
    LF   = "\x0a"
    CRLF = "\x0d\x0a"
    
    def send_response(res)
      res['headers']['Content-Length'] = !res['request-body'].nil? && res['request-body'].is_a?(String) ? res['request-body'].bytesize : 0
      data = "HTTP/1.1 #{res['status'].to_s} #{res['message']}" + CRLF
      res['headers'].each{|key, value|
        tmp = key.gsub(/\bwww|^te$|\b\w/){|s| s.upcase }
        data << "#{tmp}: #{value}" << CRLF
      }
      data << "Pragma: no-cache" << CRLF
      data << "Cache-Control: must-revalidate" << CRLF
      data << "Cache-Control: no-cache" << CRLF
      data << "Cache-Control: no-store" << CRLF
      data << "Expires: 0" << CRLF

      data << CRLF
      if ( !res['request-body'].nil? && res['request-body'].is_a?(String))
        data << res['request-body']
      end
        
      data
    end

    def send_response_hash(res)
      resp = Hash.new
      res['headers']['Content-Length'] = !res['request-body'].nil? && res['request-body'].is_a?(String) ? res['request-body'].bytesize : 0
      res['headers'].each{|key, value|
          tmp = key.gsub(/\bwww|^te$|\b\w/){|s| s.upcase }
          resp[tmp] = value
      }
      resp['request-body'] = !res['request-body'].nil? && res['request-body'].is_a?(String) ? res['request-body'] : ""
      resp['status'] = res['status']        
      resp['message'] = res['message']
      
      resp
    end
    
    @@current_exception = nil
    def self.current_exception
        @@current_exception
    end
        
    def send_error(exception=nil,status=500,hash=false)
      if exception
        trace_msg = exception.backtrace.join("\n")
	    puts "App error: #{exception.message}\n #{trace_msg}"
	  end  
      body=''
      
      err_page = nil
      if exception && exception.is_a?(::Rhom::RecordNotFound)
        err_page = RhoApplication::get_app_path(APPNAME) + 'E400' + RHO_ERB_EXT
        err_page = nil unless ::Rho::file_exist?(err_page)
      elsif exception
        err_page = RhoApplication::get_app_path(APPNAME) + 'E500' + RHO_ERB_EXT
        err_page = nil unless ::Rho::file_exist?(err_page)
      end

      if err_page
          @@current_exception = exception
          puts 'show error: ' + @@current_exception.inspect
          body = RhoController::renderfile(err_page)
          
          return send_response_hash(init_response(200,"OK",body)) if hash
      
          return send_response(init_response(200,"OK",body))
      end
      
      body << <<-_HTML_STRING_
        <html>
            <head>
                <meta name="viewport" content="width=320"/>
            </head>
            <body>
                <h2>Server Error</h2>
                <p>
      _HTML_STRING_
      body << 'Error: ' << exception.message << "<br/>" if exception
      body << "Trace: <br/>\n" << exception.backtrace.join("<br/>\n") if exception
      body << <<-_HTML_STRING_
                </p>    
            </body>
        </html>
        
      _HTML_STRING_
      if ( hash )
        send_response_hash(init_response(status,"Server error",body))
      else
        send_response(init_response(status,"Server error",body))
      end
    end
  end # RHO
  
  # Generic configuration class which accepts hashes with unique keys
  class RhoConfig
    
    @@sources = {}
    #@@config = {'start_path' => '/app', 'options_path' => '/app/Settings'}
    @@max_config_srcid = {}
    @@max_config_srcid['user'] = 1
    @@max_config_srcid['app'] = 20001
    @@max_config_srcid['local'] = 40001
    
    class << self
      def method_missing(name, *args)
        unless name == Fixnum
          if name[name.length()-1] == '='
            RhoConf.set_property_by_name(name.to_s.chop,args[0] ? args[0].to_s : "") 
          else
            RhoConf.get_property_by_name(name.to_s)             
          end
        end
      end
      
      def exists?(name)
        RhoConf.is_property_exists(name)  
      end
      
      #def reload
      #  @@config = {'start_path' => '/app', 'options_path' => '/app/Settings'}
      #  Rho::RHO.process_rhoconfig
      #end
      
      def max_config_srcid
        @@max_config_srcid
      end

      def show_log
        RhoConf.show_log
      end
      
      def send_log
        RhoConf.send_log
      end

      def read_log(limit=0)
        RhoConf.read_log(limit)
      end
      
      def sources
        @@sources
      end
      
      @@cached_changed = nil
      def clean_cached_changed
        @@cached_changed = nil
      end
      
      def source_changed?(name, partition='user')
        return Object.const_get(name).changed?() if Object.const_defined?(name)
        
        if !@@cached_changed
            @@cached_changed = {}
            db = ::Rho::RHO.get_db_partitions[partition]
            db_ids = db.execute_sql("SELECT source_id,name FROM sources" )
            changes = db.execute_sql("SELECT DISTINCT(source_id) FROM changed_values" )
            
            changes.each do |changed_item|
                
                changed_name = nil
                db_ids.each do |db_item|
                    if db_item['source_id'] == changed_item['source_id']
                        changed_name = db_item['name']
                        break
                    end    
                end
                
                @@cached_changed[changed_name] = true if changed_name
            end
        end
        
        @@cached_changed.has_key?(name) ? @@cached_changed[name] : false
      end
      
      #def config
      #  @@config
      #end
      
      #def add_config(key,value)
      #  @@config[key] = value if key # allow nil value
      #end

      def add_source(modelname, props)
        @@sources[modelname] = props
        @@sources[modelname]['name'] ||= modelname
      end
        
      def add_loaded_source(modelname, new_source=nil)
        return if !modelname || modelname.length() == 0# || @@sources[modelname]
        
        puts "#{modelname} : #{new_source}"
        @@sources[modelname] = new_source ? new_source.clone() : {}
        @@sources[modelname]['name'] ||= modelname
        @@sources[modelname][:loaded] = true
        
        if @@sources[modelname]['sync_priority']
            @@sources[modelname]['sync_priority'] = @@sources[modelname]['sync_priority'].to_i()
        else
            @@sources[modelname]['sync_priority'] = 1000
        end

        #@@sources[modelname]['sync_type'] = 'none' if !@@sources[modelname]['sync']
        @@sources[modelname]['sync_type'] ||= 'none'
        
        if @@sources[modelname]['partition']
            @@sources[modelname]['partition'] = @@sources[modelname]['partition'].to_s
        else    
            @@sources[modelname]['partition'] ||= @@sources[modelname]['sync_type'] != 'none' ? 'user' : 'local'
        end    
        
        if @@sources[modelname]['source_id']
            @@sources[modelname]['source_id'] = @@sources[modelname]['source_id'].to_i()
        end
        
        @@max_config_srcid[@@sources[modelname]['partition']] = @@sources[modelname]['source_id'] if @@sources[modelname]['source_id'] && @@max_config_srcid[@@sources[modelname]['partition']] < @@sources[modelname]['source_id']
      end
      
      @@g_base_temp_id = nil
      def generate_id
        @@g_base_temp_id = ((Time.now.to_f - Time.mktime(2009,"jan",1,0,0,0,0).to_f) * 10**6) unless @@g_base_temp_id
        
        @@g_base_temp_id = @@g_base_temp_id + 1
        @@g_base_temp_id
      end
      
    end
  end # RhoConfig
  
    module AsyncHttp

        def self.make_auth_header(args)
          auth = args[:authentication] if args.has_key?(:authentication)
          auth = args [:authorization] if args.has_key?(:authorization)
          
          return nil unless auth.is_a? Hash
          return nil if auth[:type].nil?

          raise "Authorization type #{auth[:type].inspect.to_s} is unsupported" if auth[:type].to_s != 'basic'
          raise "Username or password should be specified for 'basic' authentication" if auth[:username].nil? or auth[:password].nil?

          plain = 'Basic ' + [auth[:username].to_s + ':' + auth[:password].to_s].pack('m')
          # Remove trailing \n
          plain[0..-2]
        end

        @@user_agent = nil
        def self.preprocess_headers(args)
          hdrs = args[:headers]
          hdrs = {} if hdrs.nil?

          if hdrs['User-Agent'].nil?
            if !@@user_agent
                platform = System.get_property('platform')
                device = System.get_property('device_name')
                version = System.get_property('os_version')
                @@user_agent = "Mozilla-5.0 (#{platform}; #{device}; #{version})"
            end
            hdrs['User-Agent'] =  @@user_agent   
          end

          auth = make_auth_header(args)
          hdrs['Authorization'] = auth unless auth.nil?

          args[:headers] = hdrs
        end

        def self.check_ssl_verify_peer(args)
            ret = args[:ssl_verify_peer]
            ret = true if !ret.is_a? TrueClass and !ret.is_a? FalseClass
            args[:ssl_verify_peer] = ret ? 1 : 0
        end

	    def self.process_result(res, callback)
            return res if callback && callback.length() > 0
            
            _params = ::Rho::RhoSupport::parse_query_parameters res
            ::Rho::RhoController.process_rho_object(_params)
            _params
        end

        def self.get(args)
            preprocess_headers(args)
            check_ssl_verify_peer(args)
            process_result( AsyncHttp.do_request("GET", args ), args[:callback] )
        end
        def self.post(args)
            preprocess_headers(args)
            check_ssl_verify_peer(args)
            process_result( AsyncHttp.do_request("POST", args ), args[:callback] )
        end
        def self.download_file(args)
            preprocess_headers(args)
            check_ssl_verify_peer(args)
            process_result( AsyncHttp.do_request("Download", args ), args[:callback] )
        end
        def self.upload_file(args)
            preprocess_headers(args)
            check_ssl_verify_peer(args)
            process_result( AsyncHttp.do_request("Upload", args ), args[:callback] )
        end
        
    end

    module Timer
        def self.start( interval_ms, callback, callback_data )
            System.start_timer(interval_ms, callback, callback_data)
        end
        
        def self.stop( callback )
            System.stop_timer( callback )
        end        
    end
      
end # Rho

module SyncEngine
    def self.get_user_name
        Rho::RhoConfig.rho_sync_user        
    end
    
    def self.on_sync_create_error( src_name, objects, action )
        raise ArgumentError, 'on_sync_create_error src_name should be string' unless src_name.is_a?(String)
        
        Object.const_get(src_name).on_sync_create_error(objects, action)
    end

    def self.push_changes( src_name )
        raise ArgumentError, 'push_changes src_name should be string' unless src_name.is_a?(String)
        
        Object.const_get(src_name).push_changes()
    end

    def self.on_sync_update_error( src_name, objects, action, rollback_data = nil )
        raise ArgumentError, 'on_sync_update_error src_name should be string' unless src_name.is_a?(String)
    
        Object.const_get(src_name).on_sync_update_error(objects, action, rollback_data)
    end

    def self.on_sync_delete_error( src_name, objects, action )
        raise ArgumentError, 'on_sync_delete_error src_name should be string' unless src_name.is_a?(String)
    
        Object.const_get(src_name).on_sync_delete_error(objects, action)
    end
    
    def self.search(args)
        searchParams = ""

        searchParams += '&offset=' + Rho::RhoSupport.url_encode(args[:offset]) if args[:offset]
        searchParams += '&max_results=' + Rho::RhoSupport.url_encode(args[:max_results]) if args[:max_results]

        callbackParams = args[:callback_param] ? args[:callback_param] : ""

        if args[:search_params]
            args[:search_params].each do |key,value|
              searchParams += '&' + "search[#{Rho::RhoSupport.url_encode(key)}]" + '=' + Rho::RhoSupport.url_encode(value)
              callbackParams += '&' + "search_params[#{Rho::RhoSupport.url_encode(key)}]" + '=' + Rho::RhoSupport.url_encode(value)
            end  
        end

        SyncEngine.dosearch( args[:source_names], args[:from] ? args[:from] : 'search',
            searchParams, args[:sync_changes] ? args[:sync_changes] : false, args[:progress_step] ? args[:progress_step] : -1,
            args[:callback], callbackParams )
    end
end

#at_exit do
	#::Rhom::RhomDbAdapter.close
#end


class Module
    alias base_const_missing const_missing
    def const_missing(name)
        puts "const_missing: #{name}"
        
        res = Rho::RHO.get_instance().load_model(name.to_s)
        return res if res
        
        return base_const_missing(name)
    end
end

module Kernel   
    def require_source(name)
        Rho::RHO.get_instance().load_model(name.to_s)        
    end
    def require_model(name)
        Rho::RHO.get_instance().load_model(name.to_s)        
    end
    
end    

if !defined?(RHO_WP7) && !defined?( RHO_ME )
module WebView

    class << self
        alias_method :orig_execute_js, :execute_js
    end

    def self.execute_js(func, index = -1, vals = nil)
        if (vals && 0 < vals.size)
            func += '('
            vals.each do |val|
                func += val
                func += ',' if val != vals.last
            end
            func += ');'
        end
       orig_execute_js(func, index)                
    end
end
end