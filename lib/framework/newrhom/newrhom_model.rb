module NewRhom

  module NewRhomBaseModel
    def init_model
      puts "Model '#{klass_model.model_name}': init_model"
      partition = klass_model.partition                
      db = ::Rho::RHO.get_db_partitions()[partition]
      if !db
        db = Rhom::RhomDbAdapter.new(Rho::RhoFSConnector::get_db_fullpathname(partition), partition)
        ::Rho::RHO.get_db_partitions()[partition] = db
      end

      _load_associations
      _init_db(db)
    end

    def _load_associations
=begin
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
=end
    end

    def _init_db(db)
      migrate_schema = ""

      #db.start_transaction
      begin
        _init_db_source(db)
        _init_schema(db)
        #db.commit
      rescue Exception => e
        trace_msg = e.backtrace.join("\n")
        puts "exception when _init_db: #{e}; Trace:" + trace_msg
                
        #db.rollback
      end
    end

    def _init_db_source(db)
      puts "_init_db_source '#{klass_model.model_name}"
      
      db_data = db.select_from_table('sources','sync_priority,source_id,partition, sync_type, schema, schema_version, associations, blob_attribs, name')
      puts "_init_db_source has this already: #{db_data.inspect}"

      existing_attribs = db.select_from_table('sources','sync_priority,source_id,partition, sync_type, schema, schema_version, associations, blob_attribs, name', {'name' => klass_model.model_name} )[0]
      puts "_init_db_source '#{klass_model.model_name}': #{existing_attribs.inspect}"

      klass_model.initDbSource

      existing_attribs = db.select_from_table('sources','sync_priority,source_id,partition, sync_type, schema, schema_version, associations, blob_attribs, name', {'name' => klass_model.model_name} )[0]
      puts "_init_db_source after update '#{klass_model.model_name}': #{existing_attribs.inspect}"
    end

    def _init_schema(db)
      db_data = db.select_from_table('sources','sync_priority,source_id,partition, sync_type, schema, schema_version, associations, blob_attribs, name')
      puts "_init_db_schema has this already: #{db_data.inspect}"

      klass_model.initDbSchema

      existing_attribs = db.select_from_table('sources','sync_priority,source_id,partition, sync_type, schema, schema_version, associations, blob_attribs, name', {'name' => klass_model.model_name} )[0]
      puts "_init_db_schema after update '#{klass_model.model_name}': tableExists : #{db.table_exist?(klass_model.model_name)}, sql : #{existing_attribs.inspect}"
    end

    def set(name,value)
  	  if ( name == :sync )
  	    if ( value )
  	        klass_model.sync_type = 'incremental' unless klass_model.sync_type != 'none'
            klass_model.partition = 'user' unless klass_model.partition != 'local'
  	    else
  	        klass_model.sync_type = 'none'
            klass_model.partition = 'local'
  	    end        
  	  else
  	    klass_model.setProperty(name.to_s, value.to_s)
  	  end
    end

    def enable(name)
      set(name,true)
    end

    def fixed_schema?
      klass_model.fixed_schema == true
    end

    def property(name,type=:string)
      klass_model.setSchemaProperty(name.to_s, type.to_s)
    end

    def belongs_to(name, owner)
      if owner.is_a?(Array)       
        owner.each do |src|        
          klass_model.setBelongsTo(name.to_s, src.to_s)
        end
      else
        klass_model.setBelongsTo(name.to_s, owner.to_s)
      end  
    end
   
    def index(name,cols)
      return unless fixed_schema?
      
      klass_model.setSchemaIndex(name.to_s, cols, false)
    end

    def unique_index(name,cols)
      return unless fixed_schema?
      
      klass_model.setSchemaIndex(name.to_s, cols, true)
    end
  end
  
  module NewRhomFixedSchema
    include NewRhomBaseModel

    def self.included(model)
      puts " we are in included and #{model.klass_model.model_name}, #{model.klass_model.fixed_schema}"
      model.extend NewRhomFixedSchema
      model.klass_model.fixed_schema = true
    end
  end

  module NewRhomPropertyBag
    include NewRhomBaseModel
    
    def self.included(model)
      model.extend NewRhomPropertyBag
      model.klass_model.fixed_schema = false
    end
  end
end
